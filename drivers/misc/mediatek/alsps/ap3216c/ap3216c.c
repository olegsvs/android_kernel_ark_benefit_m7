/*
 * This file is part of the AP3216C sensor driver for MTK platform.
 * AP3216C is combined proximity, ambient light sensor and IRLED.
 *
 * Contact: YC Hou <yc.hou@liteonsemi.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 *
 * Filename: ap3216c_mtk.c
 *
 * Summary:
 *	AP3216C sensor dirver.
 *
 * Modification History:
 * Date     By       Summary
 * -------- -------- -------------------------------------------------------
 * 05/11/12 YC		 Original Creation (Test version:1.0)
 * 05/30/12 YC		 Modify AP3216C_check_and_clear_intr return value and exchange
 *                   AP3216C_get_ps_value return value to meet our spec.
 * 05/30/12 YC		 Correct shift number in AP3216C_read_ps.
 * 05/30/12 YC		 Correct ps data formula.
 * 05/31/12 YC		 1. Change the reg in clear int function from low byte to high byte 
 *                      and modify the return value.
 *                   2. Modify the eint_work function to filter als int.
 * 06/04/12 YC		 Add PS high/low threshold instead of using the same value.
 * 07/12/12 YC		 Add wakelock to prevent entering suspend when early suspending.
 *
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <linux/wakelock.h>
#include <linux/version.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#include <asm/io.h>
#include <cust_eint.h>
#include <cust_alsps.h>
#include "ap3216c.h"
#include "cust_gpio_usage.h"
#include <mach/eint.h>
#include <linux/sched.h>
#include <alsps.h>
#include <linux/batch.h>
/******************************************************************************
 * configuration
*******************************************************************************/
/*----------------------------------------------------------------------------*/

#define AP3216C_DEV_NAME     "AP3216C"
#define POWER_NONE_MACRO MT65XX_POWER_NONE
/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[ALS/PS] "
#define APS_FUN(f)               printk(KERN_INFO APS_TAG"%s\n", __FUNCTION__)
#define APS_ERR(fmt, args...)    printk(KERN_ERR  APS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define APS_LOG(fmt, args...)    printk(KERN_INFO APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)    printk(KERN_INFO APS_TAG fmt, ##args)                 
/******************************************************************************
 * extern functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
static struct i2c_client *AP3216C_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id AP3216C_i2c_id[] = {{AP3216C_DEV_NAME,0},{}};
static struct i2c_board_info __initdata i2c_ap3216c={ I2C_BOARD_INFO("AP3216C", (0x3C >> 1))};
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id); 
static int AP3216C_i2c_remove(struct i2c_client *client);
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))	
static int AP3216C_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
#endif
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_suspend(struct i2c_client *client, pm_message_t msg);
static int AP3216C_i2c_resume(struct i2c_client *client);

static struct AP3216C_priv *g_AP3216C_ptr = NULL;

struct wake_lock chrg_lock;
static int lock = 0;

static int ap3216c_local_init(void);
static int ap3216c_remove(void);
static int ap3216c_init_flag =-1; // 0<==>OK -1 <==> fail
static struct sensor_init_info ap3216c_init_info = {
		.name = "ap3216c",
		.init = ap3216c_local_init,
		.uninit = ap3216c_remove,	
};

/*----------------------------------------------------------------------------*/
typedef enum {
    CMC_BIT_ALS    = 1,
    CMC_BIT_PS     = 2,
} CMC_BIT;
/*----------------------------------------------------------------------------*/
struct AP3216C_i2c_addr {    /*define a series of i2c slave address*/
    u8  write_addr;  
    u8  ps_thd;     /*PS INT threshold*/
};
/*----------------------------------------------------------------------------*/
struct AP3216C_priv {
    struct alsps_hw  *hw;
    struct i2c_client *client;
    struct work_struct  eint_work;

    /*i2c address group*/
    struct AP3216C_i2c_addr  addr;
    
    /*misc*/
    u16		    als_modulus;
    atomic_t    i2c_retry;
    atomic_t    als_suspend;
    atomic_t    als_debounce;   /*debounce time after enabling als*/
    atomic_t    als_deb_on;     /*indicates if the debounce is on*/
    atomic_t    als_deb_end;    /*the jiffies representing the end of debounce*/
    atomic_t    ps_mask;        /*mask ps: always return far away*/
    atomic_t    ps_debounce;    /*debounce time after enabling ps*/
    atomic_t    ps_deb_on;      /*indicates if the debounce is on*/
    atomic_t    ps_deb_end;     /*the jiffies representing the end of debounce*/
    atomic_t    ps_suspend;

#ifdef CONFIG_HAS_WAKELOCK
            struct wake_lock ps_lock;
#endif    

    /*data*/
    u16         als;
    u16          ps;
    u8          _align;
    u16         als_level_num;
    u16         als_value_num;
    u32         als_level[C_CUST_ALS_LEVEL-1];
    u32         als_value[C_CUST_ALS_LEVEL];
    int           ps_cali;

    atomic_t    als_cmd_val;    /*the cmd value can't be read, stored in ram*/
    atomic_t    ps_cmd_val;     /*the cmd value can't be read, stored in ram*/
    atomic_t    ps_thd_val_h;   /*the cmd value can't be read, stored in ram*/
	atomic_t    ps_thd_val_l;   /*the cmd value can't be read, stored in ram*/
    ulong       enable;         /*enable mask*/
    ulong       pending_intr;   /*pending interrupt*/

    /*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
    struct early_suspend    early_drv;
#endif     
};
/*----------------------------------------------------------------------------*/
static struct i2c_driver AP3216C_i2c_driver = {	
	.probe      = AP3216C_i2c_probe,
	.remove     = AP3216C_i2c_remove,
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))	
	.detect     = AP3216C_i2c_detect,
#endif
	.suspend    = AP3216C_i2c_suspend,
	.resume     = AP3216C_i2c_resume,
	.id_table   = AP3216C_i2c_id,
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))	
	.address_data = &AP3216C_addr_data,
#endif
	.driver = {
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))	
		.owner          = THIS_MODULE,
#endif
		.name           = AP3216C_DEV_NAME,
	},
};

static struct AP3216C_priv *AP3216C_obj = NULL;
#ifndef MTK_AUTO_DETECT_ALSPS
static struct platform_driver AP3216C_alsps_driver;
#endif

static DEFINE_MUTEX(AP3216C_mutex);

int AP3216C_read_reg(struct i2c_client *client, u8 addr , char * data )
{
        int res = 0;
        u8 buffer[1];
        u8 reg_value[1];
                
        mutex_lock(&AP3216C_mutex);
        
        buffer[0]=addr;
        res = i2c_master_send(client, buffer, 0x1);
        if(res <= 0)
            goto EXIT_ERR;

        res = i2c_master_recv(client, reg_value, 0x1);
        if(res <= 0)
            goto EXIT_ERR;
        
        mutex_unlock(&AP3216C_mutex);
        *data= reg_value[0];

        return res;
        
EXIT_ERR:
        mutex_unlock(&AP3216C_mutex);
        APS_ERR("AP3216C read fail\n");
	return -1;    

}
int AP3216C_write_reg(struct i2c_client *client, u8 * data, u8 count )
{
	int res = 0;

        mutex_lock(&AP3216C_mutex);
   
	res = i2c_master_send(client, data, count);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
                        
        mutex_unlock(&AP3216C_mutex);

        return res;
        
EXIT_ERR:
        mutex_unlock(&AP3216C_mutex);
        APS_ERR("AP3216C write fail\n");
	return -1;    

}
/*----------------------------------------------------------------------------*/
#define ALS_CALIBRATION_FLOAT  (1000)
u16 cali = 1000;
long int g_pscali = 0;
int AP3216C_read_als(struct i2c_client *client, u16 *data);
int AP3216C_read_ps(struct i2c_client *client, u16 *data);
int AP3216C_als_calibration(struct i2c_client *client, int std_ligth_src)
{
	int err = 0;
	u16 als_data;

	if(err = AP3216C_read_als(client, &als_data))
	{
		APS_ERR("ALS calibration fialed!\n");
		return err;
	}

	cali =  std_ligth_src * ALS_CALIBRATION_FLOAT / als_data;
	return 0;
}
/*----------------------------------------------------------------------------*/
int AP3216C_clear_ps_calibration(struct i2c_client *client)
{
	int err = 0;
	u8 databuf[2];
	
	databuf[0] = AP3216C_LSC_CALIBRATION_L;    
	databuf[1] = 0;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("PS calibartion L failed!!\n");
		return err;
	}

	databuf[0] = AP3216C_LSC_CALIBRATION_H;    
	databuf[1] = 0;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("PS calibartion H failed!!\n");
		return err;
	}

	return 0;
}

int AP3216C_ps_calibration(struct i2c_client *client)
{
	int err = 0;
	u16 ps_data;
	u8 databuf[2];
	int i;

	for(i=0; i<20; ++i)
	{
		if(err = AP3216C_read_ps(client, &ps_data))
			return err;
		g_pscali += ps_data;
	}
	g_pscali /= 20;
	if(g_pscali > 511)
	{
		g_pscali = 511;
	}
	ps_data = g_pscali;
	APS_LOG("AP3216C_ps_calibration -1- %x\n",ps_data);
   
	databuf[0] = AP3216C_LSC_CALIBRATION_L;    
	databuf[1] = ps_data & 0x01;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("PS calibartion L failed!!\n");
		return err;
	}

	databuf[0] = AP3216C_LSC_CALIBRATION_H;    
	databuf[1] = (ps_data >> 1) & 0xFF;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("PS calibartion H failed!!\n");
		return err;
	}

	return 0;
}

int AP3216C_update_ps_calibration_from_nvram(struct i2c_client *client, int ps_nvram_data)
{
	int err = 0;
	u8 databuf[2];

	APS_LOG("- %x\n",ps_nvram_data);
	if(ps_nvram_data > 511)
	{
		ps_nvram_data = 511;
	}

	databuf[0] = AP3216C_LSC_CALIBRATION_L;    
	databuf[1] = ps_nvram_data & 0x01;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("L failed!!\n");
		return err;
	}

	databuf[0] = AP3216C_LSC_CALIBRATION_H;    

	databuf[1] = (ps_nvram_data >> 1) & 0xFF;
	err = i2c_master_send(client, databuf, 0x2);
	if(err <= 0)
	{
		APS_ERR("H failed!!\n");
		return err;
	}

	return 0;
}


/*----------------------------------------------------------------------------*/
int AP3216C_get_addr(struct alsps_hw *hw, struct AP3216C_i2c_addr *addr)
{
	if(!hw || !addr)
	{
		return -EFAULT;
	}
	addr->write_addr= hw->i2c_addr[0];
	return 0;
}
/*----------------------------------------------------------------------------*/
static void AP3216C_power(struct alsps_hw *hw, unsigned int on) 
{
	static unsigned int power_on = 0;

	if(hw->power_id != POWER_NONE_MACRO)
	{
		if(power_on == on)
		{
			APS_LOG("ignore power control: %d\n", on);
		}
		else if(on)
		{
			if(!hwPowerOn(hw->power_id, hw->power_vol, "AP3216C")) 
			{
				APS_ERR("power on fails!!\n");
			}
		}
		else
		{
			if(!hwPowerDown(hw->power_id, "AP3216C")) 
			{
				APS_ERR("power off fail!!\n");   
			}
		}
	}
	power_on = on;
}
/*----------------------------------------------------------------------------*/
static int AP3216C_enable_als(struct i2c_client *client, int enable)
{
		struct AP3216C_priv *obj = i2c_get_clientdata(client);
		u8 databuf[2];	  
		int res = 0;
		u8 buffer[1];
		u8 reg_value[1];
	
		if(client == NULL)
		{
			APS_DBG("CLIENT CANN'T EQUAL NULL\n");
			return -1;
		}


#if 0

		buffer[0]=AP3216C_LSC_ENABLE;
		res = i2c_master_send(client, buffer, 0x1);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}
		res = i2c_master_recv(client, reg_value, 0x1);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}
#else
                res = AP3216C_read_reg(client, AP3216C_LSC_ENABLE, reg_value );
                if(res < 0)
                {
                    goto EXIT_ERR;
                }

#endif		
		if(enable)
		{
			databuf[0] = AP3216C_LSC_ENABLE;	
			//databuf[1] = reg_value[0] |0x01;
			databuf[1] = test_bit(CMC_BIT_PS, &obj->enable)  ? 3: 0 +1 ;
			//res = i2c_master_send(client, databuf, 0x2);
			res =AP3216C_write_reg(client,databuf, 2);
			if(res < 0)
			{
				goto EXIT_ERR;
			}
			atomic_set(&obj->als_deb_on, 1);
			atomic_set(&obj->als_deb_end, jiffies+atomic_read(&obj->als_debounce)/(1000/HZ));
			APS_DBG("AP3216C ALS enable\n");
		}
		else
		{
			databuf[0] = AP3216C_LSC_ENABLE;	
			//databuf[1] = reg_value[0] &0xFE;
			databuf[1] = test_bit(CMC_BIT_PS, &obj->enable)  ? 2: 0 +0 ;
			//res = i2c_master_send(client, databuf, 0x2);
			res =AP3216C_write_reg(client,databuf, 2);
			if(res < 0)
			{
				goto EXIT_ERR;
			}
			atomic_set(&obj->als_deb_on, 0);
			APS_DBG("AP3216C ALS disable\n");
		}
		return 0;
		
	EXIT_ERR:
		APS_ERR("AP3216C_enable_als fail\n");
		return res;
}

/*----------------------------------------------------------------------------*/
static int AP3216C_enable_ps(struct i2c_client *client, int enable)
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);
	u8 databuf[2];    
	int res = 0;
	u8 buffer[1];
	u8 reg_value[1];

	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUAL NULL\n");
		return -1;
	}
#if 0
	buffer[0]=AP3216C_LSC_ENABLE;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, reg_value, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
 #else
 
         res = AP3216C_read_reg(client, AP3216C_LSC_ENABLE, reg_value );
         if(res < 0)
         {
             goto EXIT_ERR;
         }
 
#endif	
	if(enable)
	{
		databuf[0] = AP3216C_LSC_ENABLE;    
		databuf[1] = reg_value[0] |0x02;
//		res = i2c_master_send(client, databuf, 0x2);
		res =AP3216C_write_reg(client,databuf, 2);

		if(res < 0)
		{
			goto EXIT_ERR;
		}
		atomic_set(&obj->ps_deb_on, 1);
		atomic_set(&obj->ps_deb_end, jiffies+atomic_read(&obj->ps_debounce)/(1000/HZ));

		mt_eint_unmask(CUST_EINT_ALS_NUM);
		APS_DBG("AP3216C PS enable\n");
	}
	else
	{
		databuf[0] = AP3216C_LSC_ENABLE;    
		databuf[1] = reg_value[0] &0xfd;
//		res = i2c_master_send(client, databuf, 0x2);
                res =AP3216C_write_reg(client,databuf, 2);

		if(res < 0)
		{
			goto EXIT_ERR;
		}
		atomic_set(&obj->ps_deb_on, 0);
		APS_DBG("AP3216C PS disable\n");

		if(0 == obj->hw->polling_mode_ps)
		{
			cancel_work_sync(&obj->eint_work);
			mt_eint_mask(CUST_EINT_ALS_NUM);
		}
	}
	return 0;
	
EXIT_ERR:
	APS_ERR("AP3216C_enable_ps fail\n");
	return res;
}
/*----------------------------------------------------------------------------*/
static int AP3216C_check_and_clear_intr(struct i2c_client *client) 
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);
	int res;
	u8 buffer[1], ints[1];
#if 0
	/* Get Int status */
	buffer[0] = AP3216C_LSC_INT_STATUS;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, ints, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
#else
        res = AP3216C_read_reg(client, AP3216C_LSC_INT_STATUS, ints );
        if(res < 0)
        {
            goto EXIT_ERR;
        }


#endif
#if 0
	/* Clear ALS int flag */
	buffer[0] = AP3216C_LSC_ADATA_H;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}

	/* Clear PS int flag */
	buffer[0] = AP3216C_LSC_PDATA_H;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
#else
        res = AP3216C_read_reg(client, AP3216C_LSC_ADATA_H, buffer );
        if(res < 0)
        {
            goto EXIT_ERR;
        }
        res = AP3216C_read_reg(client, AP3216C_LSC_PDATA_H, buffer );
        if(res < 0)
        {
            goto EXIT_ERR;
        }

#endif
	return ints[0];

EXIT_ERR:
	APS_ERR("AP3216C_check_and_clear_intr fail\n");
	return -1;
}
/*----------------------------------------------------------------------------*/
void AP3216C_eint_func(void)
{
	struct AP3216C_priv *obj = g_AP3216C_ptr;
	if(!obj)
	{
		return;
	}
	
	schedule_work(&obj->eint_work);
}

/*----------------------------------------------------------------------------*/
// This function depends the real hw setting, customers should modify it. 2012/5/10 YC. 
int AP3216C_setup_eint(struct i2c_client *client)
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);        

	g_AP3216C_ptr = obj;
	
	mt_set_gpio_dir(GPIO_ALS_EINT_PIN, GPIO_DIR_IN);
	mt_set_gpio_mode(GPIO_ALS_EINT_PIN, GPIO_ALS_EINT_PIN_M_EINT);
	mt_set_gpio_pull_enable(GPIO_ALS_EINT_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_ALS_EINT_PIN, GPIO_PULL_UP);

//	mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_ALS_SENSITIVE);
//	mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_TYPE);
	mt_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
	mt_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_TYPE, AP3216C_eint_func, 0);

	mt_eint_unmask(CUST_EINT_ALS_NUM);  
	return 0;
}

/*----------------------------------------------------------------------------*/
static int AP3216C_init_client(struct i2c_client *client)
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);
	u8 databuf[2];    
	int res = 0;
   
	databuf[0] = AP3216C_LSC_ENABLE;    
	databuf[1] = 0x00;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = AP3216C_LSC_INT_LOW_THD_LOW;    
	databuf[1] = atomic_read(&obj->ps_thd_val_l) & 0x03;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = AP3216C_LSC_INT_HIGH_THD_LOW;    
	databuf[1] = atomic_read(&obj->ps_thd_val_h) & 0x03;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = AP3216C_LSC_INT_LOW_THD_HIGH;    
	databuf[1] = (atomic_read(&obj->ps_thd_val_l) & 0x3FC) >> 2;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = AP3216C_LSC_INT_HIGH_THD_HIGH;    
	databuf[1] = (atomic_read(&obj->ps_thd_val_h) & 0x3FC) >> 2;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

/*Improve the transmission power of PS*/

	databuf[0] = 0x20;
#if defined(T855_PROJ) && defined(T855A_OQ_PROJ)
	databuf[1] = 0x2d;
#else
	databuf[1] = 0x25;
#endif
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = 0x21;
	databuf[1] = 0x23;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = 0x22;
	databuf[1] = 0x00;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = 0x19;
	databuf[1] = 0xff;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	databuf[0] = 0x10;
	databuf[1] = 0x00;
	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		goto EXIT_ERR;
		return AP3216C_ERR_I2C;
	}

	if(res = AP3216C_setup_eint(client))
	{
		APS_ERR("setup eint: %d\n", res);
		return res;
	}
	if((res = AP3216C_check_and_clear_intr(client)) < 0)
	{
		APS_ERR("check/clear intr: %d\n", res);
	}
	
	return AP3216C_SUCCESS;

EXIT_ERR:
	APS_ERR("init dev: %d\n", res);
	return res;
}

/****************************************************************************** 
 * Function Configuration
******************************************************************************/
int AP3216C_read_als(struct i2c_client *client, u16 *data)
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);	 
	u8 als_value_low[1], als_value_high[1];
	u8 buffer[1];
	int res = 0;
	
	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUAL NULL\n");
		return -1;
	}
#if 0
	// get ALS adc count
	buffer[0]=AP3216C_LSC_ADATA_L;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, als_value_low, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	
	buffer[0]=AP3216C_LSC_ADATA_H;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, als_value_high, 0x01);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
#else
        res = AP3216C_read_reg(client, AP3216C_LSC_ADATA_L, als_value_low );
        if(res < 0)
        {
            goto EXIT_ERR;
        }
        res = AP3216C_read_reg(client, AP3216C_LSC_ADATA_H, als_value_high);
        if(res < 0)
        {
            goto EXIT_ERR;
        }

#endif
	*data = als_value_low[0] | (als_value_high[0]<<8);

	if (*data < 0)
	{
		*data = 0;
		APS_DBG("als_value is invalid!!\n");
		goto EXIT_ERR;
	}

#if 0
	*data *= cali;
	*data /= ALS_CALIBRATION_FLOAT;
#endif

	return 0;

EXIT_ERR:
	APS_ERR("AP3216C_read_als fail\n");
	return res;
}
/*----------------------------------------------------------------------------*/
static int AP3216C_get_als_value(struct AP3216C_priv *obj, u16 als)
{
	int idx;
	int invalid = 0;
	for(idx = 0; idx < obj->als_level_num; idx++)
	{
		if(als < obj->hw->als_level[idx])
		{
			break;
		}
	}
	
	if(idx >= obj->als_value_num)
	{
		APS_ERR("exceed range\n"); 
		idx = obj->als_value_num - 1;
	}
	
	if(1 == atomic_read(&obj->als_deb_on))
	{
		unsigned long endt = atomic_read(&obj->als_deb_end);
		if(time_after(jiffies, endt))
		{
			atomic_set(&obj->als_deb_on, 0);
		}
		
		if(1 == atomic_read(&obj->als_deb_on))
		{
			invalid = 1;
		}
	}

	if(!invalid)
	{
		APS_DBG("ALS: %05d => %05d\n", als, obj->hw->als_value[idx]);	
		return obj->hw->als_value[idx];
	}
	else
	{
		APS_ERR("ALS: %05d => %05d (-1)\n", als, obj->hw->als_value[idx]);    
		return -1;
	}
}
/*----------------------------------------------------------------------------*/
int AP3216C_read_ps(struct i2c_client *client, u16 *data)
{
	struct AP3216C_priv *obj = i2c_get_clientdata(client);       
	u8 ps_value_low[1], ps_value_high[1];
	u8 buffer[1];
	int res = 0;
	printk(KERN_ERR"AP3216C_read_ps START\n");
	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUAL NULL\n");
		return -1;
	}

#if 0
	buffer[0]=AP3216C_LSC_PDATA_L;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, ps_value_low, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}

	buffer[0]=AP3216C_LSC_PDATA_H;
	res = i2c_master_send(client, buffer, 0x1);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
	res = i2c_master_recv(client, ps_value_high, 0x01);
	if(res <= 0)
	{
		goto EXIT_ERR;
	}
#else
        res = AP3216C_read_reg(client, AP3216C_LSC_PDATA_L, ps_value_low );
        if(res < 0)
        {
            goto EXIT_ERR;
        }
        res = AP3216C_read_reg(client, AP3216C_LSC_PDATA_H, ps_value_high);
        if(res < 0)
        {
            goto EXIT_ERR;
        }

#endif
	*data = (ps_value_low[0] & 0x0f) | ((ps_value_high[0] & 0x3f) << 4);
	printk(KERN_ERR"AP3216C_read_ps_data = %d\n",*data);

	return 0;    

EXIT_ERR:
	APS_ERR("AP3216C_read_ps fail\n");
	return res;
}
/*----------------------------------------------------------------------------*/
/* 
   for AP3216C_get_ps_value:
	return 1 = object close,
	return 0 = object far away. 2012/5/10 YC   // exchange 0 and 1 2012/5/30 YC 
*/
static int AP3216C_get_ps_value(struct AP3216C_priv *obj, u16 ps)
{
	int val;
	int invalid = 0;

	if(ps > atomic_read(&obj->ps_thd_val_h))
		val = 0;  /*close*/
	else if(ps < atomic_read(&obj->ps_thd_val_l))
		val = 1;  /*far away*/

#if 0
	if(atomic_read(&obj->ps_suspend))
	{
		invalid = 1;
	}
	else if(1 == atomic_read(&obj->ps_deb_on))
	{
		unsigned long endt = atomic_read(&obj->ps_deb_end);
		if(time_after(jiffies, endt))
		{
			atomic_set(&obj->ps_deb_on, 0);
		}
		
		if (1 == atomic_read(&obj->ps_deb_on))
		{
			invalid = 1;
		}
	}
#endif
	if(!invalid)
	{
		APS_DBG("AP3216C_PS:  %05d => %05d\n", ps, val);
		return val;
	}	
	else
	{
		return -1;
	}	
}


/*----------------------------------------------------------------------------*/
static void AP3216C_eint_work(struct work_struct *work)
{
	struct AP3216C_priv *obj = (struct AP3216C_priv *)container_of(work, struct AP3216C_priv, eint_work);
	int err;
	hwm_sensor_data sensor_data;

	if((err = AP3216C_check_and_clear_intr(obj->client)) < 0)
	{
		APS_ERR("AP3216C_eint_work check intrs: %d\n", err);
	}
	else if (err & 0x01)
	{
		// ALS interrupt. User should add their code here if they want to handle ALS Int.
	}
	else if (err & 0x02)
	{
		AP3216C_read_ps(obj->client, &obj->ps);
		sensor_data.values[0] = AP3216C_get_ps_value(obj, obj->ps);
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;			

		//let up layer to know
		if((err = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
		{
		  APS_ERR("call hwmsen_get_interrupt_data fail = %d\n", err);
		}
	}
	
	mt_eint_unmask(CUST_EINT_ALS_NUM);      
}


/****************************************************************************** 
 * Function Configuration
******************************************************************************/
static int AP3216C_open(struct inode *inode, struct file *file)
{
	file->private_data = AP3216C_i2c_client;

	if (!file->private_data)
	{
		APS_ERR("null pointer!!\n");
		return -EINVAL;
	}
	
	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int AP3216C_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}
static int set_psensor_threshold(struct i2c_client *client)
{
	u8 databuf[3];    
	int res = 0;
	struct AP3216C_priv *obj = i2c_get_clientdata(client);

        databuf[0] = AP3216C_LSC_INT_LOW_THD_LOW;    
        databuf[1] = atomic_read(&obj->ps_thd_val_l) & 0x03;
        res = i2c_master_send(client, databuf, 0x2);
        if(res <= 0)
        {
            goto EXIT_ERR;
            return AP3216C_ERR_I2C;
        }

        databuf[0] = AP3216C_LSC_INT_HIGH_THD_LOW;    
        databuf[1] = atomic_read(&obj->ps_thd_val_h) & 0x03;
        res = i2c_master_send(client, databuf, 0x2);
        if(res <= 0)
        {
            goto EXIT_ERR;
            return AP3216C_ERR_I2C;
        }

        databuf[0] = AP3216C_LSC_INT_LOW_THD_HIGH;    
        databuf[1] = (atomic_read(&obj->ps_thd_val_l) & 0x3FC) >> 2;
        res = i2c_master_send(client, databuf, 0x2);
        if(res <= 0)
        {
            goto EXIT_ERR;
            return AP3216C_ERR_I2C;
        }

        databuf[0] = AP3216C_LSC_INT_HIGH_THD_HIGH;    
        databuf[1] = (atomic_read(&obj->ps_thd_val_h) & 0x3FC) >> 2;
        res = i2c_master_send(client, databuf, 0x2);
        if(res <= 0)
        {
            goto EXIT_ERR;
            return AP3216C_ERR_I2C;
        }
        	return AP3216C_SUCCESS;

        EXIT_ERR:
    	APS_ERR("set_psensor_threshold dev: %d\n", res);
    	return res;


}
/*----------------------------------------------------------------------------*/
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))
static int AP3216C_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
#else
static int AP3216C_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
	struct i2c_client *client = (struct i2c_client*)file->private_data;
	struct AP3216C_priv *obj = i2c_get_clientdata(client);  

        int ps_result;
	int err = 0;
	void __user *ptr = (void __user*) arg;
	int dat;
	int ps_cali;
	int threshold[2];
        
	uint32_t enable;
	uint32_t std_light_src;
	uint32_t ps_calibration_from_nvram;
	switch (cmd)
	{
		case ALSPS_SET_PS_MODE:
			if(copy_from_user(&enable, ptr, sizeof(enable)))
			{
				err = -EFAULT;
				goto err_out;
			}
			if(enable)
			{
				if(err = AP3216C_enable_ps(obj->client, 1))
				{
					APS_ERR("enable ps fail: %d\n", err); 
					goto err_out;
				}
				
				set_bit(CMC_BIT_PS, &obj->enable);
			}
			else
			{
				if(err = AP3216C_enable_ps(obj->client, 0))
				{
					APS_ERR("disable ps fail: %d\n", err); 
					goto err_out;
				}
				
				clear_bit(CMC_BIT_PS, &obj->enable);
			}
			break;

		case ALSPS_GET_PS_MODE:
			enable = test_bit(CMC_BIT_PS, &obj->enable) ? (1) : (0);
			if(copy_to_user(ptr, &enable, sizeof(enable)))
			{
				err = -EFAULT;
				goto err_out;
			}
			break;

		case ALSPS_GET_PS_DATA:    
			if(err = AP3216C_read_ps(obj->client, &obj->ps))
			{
				goto err_out;
			}
			
			dat = AP3216C_get_ps_value(obj, obj->ps);
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}  
			break;

		case ALSPS_GET_PS_RAW_DATA:    
			if(err = AP3216C_read_ps(obj->client, &obj->ps))
			{
				goto err_out;
			}
			
			dat = obj->ps;
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}  
			break;              

		case ALSPS_SET_ALS_MODE:
			if(copy_from_user(&enable, ptr, sizeof(enable)))
			{
				err = -EFAULT;
				goto err_out;
			}
			if(enable)
			{
				if(err = AP3216C_enable_als(obj->client, 1))
				{
					APS_ERR("enable als fail: %d\n", err); 
					goto err_out;
				}
				set_bit(CMC_BIT_ALS, &obj->enable);
			}
			else
			{
				if(err = AP3216C_enable_als(obj->client, 0))
				{
					APS_ERR("disable als fail: %d\n", err); 
					goto err_out;
				}
				clear_bit(CMC_BIT_ALS, &obj->enable);
			}
			break;

		case ALSPS_GET_ALS_MODE:
			enable = test_bit(CMC_BIT_ALS, &obj->enable) ? (1) : (0);
			if(copy_to_user(ptr, &enable, sizeof(enable)))
			{
				err = -EFAULT;
				goto err_out;
			}
			break;

		case ALSPS_GET_ALS_DATA: 
			if(err = AP3216C_read_als(obj->client, &obj->als))
			{
				goto err_out;
			}

			dat = AP3216C_get_als_value(obj, obj->als);
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}              
			break;

		case ALSPS_GET_ALS_RAW_DATA:    
			if(err = AP3216C_read_als(obj->client, &obj->als))
			{
				goto err_out;
			}

			dat = obj->als;
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}              
			break;
		// ALS calibration cmd.
/*		case ALSPS_SET_ALS_CALIBRATION: 
			if(copy_from_user(&std_light_src, ptr, sizeof(std_light_src)))
			{
				err = -EFAULT;
				goto err_out;
			}
   
			if(err = AP3216C_als_calibration(obj->client, std_light_src))
			{
				goto err_out;
			}
			break;
		// PS calibration cmd.
		case ALSPS_SET_PS_CALIBRATION: 
			if(err = AP3216C_enable_ps(obj->client, 1))
			{
				APS_ERR("enable ps fail: %d\n", err); 
				goto err_out;
			}				
			set_bit(CMC_BIT_PS, &obj->enable);

			if(err = AP3216C_ps_calibration(obj->client))
			{
				goto err_out;
			}
			if(err = AP3216C_enable_ps(obj->client, 0))
			{
				APS_ERR("disable ps fail: %d\n", err); 
				goto err_out;
			}				
			clear_bit(CMC_BIT_PS, &obj->enable);
			dat = g_pscali;
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			} 
			break;

		case ALSPS_CLEAR_PS_CALIBRATION: 
			if(err = AP3216C_enable_ps(obj->client, 1))
			{
				APS_ERR("enable ps fail: %d\n", err); 
				goto err_out;
			}				
			set_bit(CMC_BIT_PS, &obj->enable);

			if(err = AP3216C_clear_ps_calibration(obj->client))
			{
			goto err_out;
			}
			if(err = AP3216C_enable_ps(obj->client, 0))
			{
				APS_ERR("disable ps fail: %d\n", err); 
				goto err_out;
			}				
			clear_bit(CMC_BIT_PS, &obj->enable);
			break;
		case ALSPS_UPDATE_PS_CALIBRATION_FROM_NVRAM:
			if(copy_from_user(&ps_calibration_from_nvram, ptr, sizeof(ps_calibration_from_nvram)))
			{
				err = -EFAULT;
				goto err_out;
			}
			if(ps_calibration_from_nvram)
			{
				if(err = AP3216C_enable_ps(obj->client, 1))
				{
					APS_ERR("enable ps fail: %d\n", err); 
					goto err_out;
				}				
				set_bit(CMC_BIT_PS, &obj->enable);

				if(err = AP3216C_update_ps_calibration_from_nvram(obj->client, ps_calibration_from_nvram))
				{
					goto err_out;
				}
				
				if(err = AP3216C_enable_ps(obj->client, 0))
				{
					APS_ERR("disable ps fail: %d\n", err); 
					goto err_out;
				}				
				clear_bit(CMC_BIT_PS, &obj->enable);
			}
			break;*/

                /*----------------------------------for factory mode test---------------------------------------*/
                case ALSPS_GET_PS_TEST_RESULT:
                    if((err = AP3216C_read_ps(obj->client, &obj->ps)))
                    {
                        goto err_out;
                    }
                    if(obj->ps > atomic_read(&obj->ps_thd_val_h))
                        {
                            ps_result = 0;
                        }
                    else    ps_result = 1;
                    
                    if(copy_to_user(ptr, &ps_result, sizeof(ps_result)))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }              
                    break;
                
                case ALSPS_IOCTL_CLR_CALI:
                    if(copy_from_user(&dat, ptr, sizeof(dat)))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                    if(dat == 0)
                        obj->ps_cali = 0;
                    break;
                
                case ALSPS_IOCTL_GET_CALI:
                    ps_cali = obj->ps_cali ;
                    if(copy_to_user(ptr, &ps_cali, sizeof(ps_cali)))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                    break;
                
                case ALSPS_IOCTL_SET_CALI:
                    if(copy_from_user(&ps_cali, ptr, sizeof(ps_cali)))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                
                    obj->ps_cali = ps_cali;
                    break;
                
                case ALSPS_SET_PS_THRESHOLD:
                    if(copy_from_user(threshold, ptr, sizeof(threshold)))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                    APS_ERR("%s set threshold high: 0x%x, low: 0x%x\n", __func__, threshold[0],threshold[1]); 
                    atomic_set(&obj->ps_thd_val_h,  (threshold[0]+obj->ps_cali));
                    atomic_set(&obj->ps_thd_val_l,  (threshold[1]+obj->ps_cali));//need to confirm
                
                    set_psensor_threshold(obj->client);
                    
                    break;
                    
                case ALSPS_GET_PS_THRESHOLD_HIGH:
                    threshold[0] = atomic_read(&obj->ps_thd_val_h) - obj->ps_cali;
                    APS_ERR("%s get threshold high: 0x%x\n", __func__, threshold[0]); 
                    if(copy_to_user(ptr, &threshold[0], sizeof(threshold[0])))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                    break;
                    
                case ALSPS_GET_PS_THRESHOLD_LOW:
                    threshold[0] = atomic_read(&obj->ps_thd_val_l) - obj->ps_cali;
                    APS_ERR("%s get threshold low: 0x%x\n", __func__, threshold[0]); 
                    if(copy_to_user(ptr, &threshold[0], sizeof(threshold[0])))
                    {
                        err = -EFAULT;
                        goto err_out;
                    }
                    break;
                /*------------------------------------------------------------------------------------------*/

		default:
			APS_ERR("%s not supported = 0x%04x", __FUNCTION__, cmd);
			err = -ENOIOCTLCMD;
			break;
	}

	err_out:
	return err;    
}
/*----------------------------------------------------------------------------*/
static struct file_operations AP3216C_fops = {
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))
	.owner = THIS_MODULE,
#endif
	.open = AP3216C_open,
	.release = AP3216C_release,
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))
	.ioctl = AP3216C_ioctl,
#else
	.unlocked_ioctl = AP3216C_ioctl,
#endif
};
/*----------------------------------------------------------------------------*/
static struct miscdevice AP3216C_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "als_ps",
	.fops = &AP3216C_fops,
};
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_suspend(struct i2c_client *client, pm_message_t msg) 
{
	APS_FUN();    
	return 0;
}
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_resume(struct i2c_client *client)
{
	APS_FUN();
	return 0;
}
/*----------------------------------------------------------------------------*/
static void AP3216C_early_suspend(struct early_suspend *h) 
{   
	struct AP3216C_priv *obj = container_of(h, struct AP3216C_priv, early_drv);   
	int err;
	APS_FUN();    

	if(!obj)
	{
		APS_ERR("null pointer!!\n");
		return;
	}

	atomic_set(&obj->als_suspend, 1);
	if(test_bit(CMC_BIT_ALS, &obj->enable))
	{
		if(err = AP3216C_enable_als(obj->client, 0))
		{
			APS_ERR("disable als fail: %d\n", err); 
		}
	}

}
/*----------------------------------------------------------------------------*/
static void AP3216C_late_resume(struct early_suspend *h)
{   
	struct AP3216C_priv *obj = container_of(h, struct AP3216C_priv, early_drv);         
	int err;
	APS_FUN();

	if(!obj)
	{
		APS_ERR("null pointer!!\n");
		return;
	}

	atomic_set(&obj->als_suspend, 0);
	if(test_bit(CMC_BIT_ALS, &obj->enable))
	{
		if(err = AP3216C_enable_als(obj->client, 1))
		{
			APS_ERR("enable als fail: %d\n", err);        

		}
	}

}

int AP3216C_ps_operate(void* self, uint32_t command, void* buff_in, int size_in,
		void* buff_out, int size_out, int* actualout)
{
	int err = 0;
	int value;
	hwm_sensor_data* sensor_data;
	struct AP3216C_priv *obj = (struct AP3216C_priv *)self;
	
	switch (command)
	{
		case SENSOR_DELAY:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				APS_ERR("Set delay parameter error!\n");
				err = -EINVAL;
			}
			// Do nothing
			break;

		case SENSOR_ENABLE:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				APS_ERR("Enable sensor parameter error!\n");
				err = -EINVAL;
			}
			else
			{				
				value = *(int *)buff_in;
				if(value)
				{
					if(err = AP3216C_enable_ps(obj->client, 1))
					{
						APS_ERR("enable ps fail: %d\n", err); 
						return -1;
					}
#ifdef CONFIG_HAS_WAKELOCK
                     wake_lock(&obj->ps_lock);
#endif                     
                    
					set_bit(CMC_BIT_PS, &obj->enable);
				}
				else
				{
					if(err = AP3216C_enable_ps(obj->client, 0))
					{
						APS_ERR("disable ps fail: %d\n", err); 
						return -1;
					}
#ifdef CONFIG_HAS_WAKELOCK
                     wake_unlock(&obj->ps_lock);
#endif                     
                    
					clear_bit(CMC_BIT_PS, &obj->enable);
				}
			}
			break;

		case SENSOR_GET_DATA:
			if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
			{
				APS_ERR("get sensor data parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				sensor_data = (hwm_sensor_data *)buff_out;	
				AP3216C_read_ps(obj->client, &obj->ps);
				
				sensor_data->values[0] = AP3216C_get_ps_value(obj, obj->ps);
				sensor_data->value_divide = 1;
				sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;			
			}
			break;
		default:
			APS_ERR("proxmy sensor operate function no this parameter %d!\n", command);
			err = -1;
			break;
	}
	
	return err;
}

int AP3216C_als_operate(void* self, uint32_t command, void* buff_in, int size_in,
		void* buff_out, int size_out, int* actualout)
{
	int err = 0;
	int value;
	hwm_sensor_data* sensor_data;
	struct AP3216C_priv *obj = (struct AP3216C_priv *)self;

	switch (command)
	{
		case SENSOR_DELAY:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				APS_ERR("Set delay parameter error!\n");
				err = -EINVAL;
			}
			// Do nothing
			break;

		case SENSOR_ENABLE:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				APS_ERR("Enable sensor parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				value = *(int *)buff_in;				
				if(value)
				{
					if(err = AP3216C_enable_als(obj->client, 1))
					{
						APS_ERR("enable als fail: %d\n", err); 
						return -1;
					}
					set_bit(CMC_BIT_ALS, &obj->enable);
				}
				else
				{
					if(err = AP3216C_enable_als(obj->client, 0))
					{
						APS_ERR("disable als fail: %d\n", err); 
						return -1;
					}
					clear_bit(CMC_BIT_ALS, &obj->enable);
				}
				
			}
			break;

		case SENSOR_GET_DATA:
			if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
			{
				APS_ERR("get sensor data parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				sensor_data = (hwm_sensor_data *)buff_out;
				AP3216C_read_als(obj->client, &obj->als);
			#if defined(MTK_AAL_SUPPORT)
				sensor_data->values[0] = obj->als;
			#else
				sensor_data->values[0] = AP3216C_get_als_value(obj, obj->als);
			#endif
				sensor_data->value_divide = 1;
				sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
			}
			break;
		default:
			APS_ERR("light sensor operate function no this parameter %d!\n", command);
			err = -1;
			break;
	}
	
	return err;
}

#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,0,0))
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) 
{    
	strcpy(info->type, AP3216C_DEV_NAME);
	return 0;
}
#endif
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct AP3216C_priv *obj;
	struct hwmsen_object obj_ps, obj_als;
	int err = 0;
	if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
	{
		err = -ENOMEM;
		goto exit;
	}
	memset(obj, 0, sizeof(*obj));
	AP3216C_obj = obj;

	obj->hw = get_cust_alsps_ap3216c_hw();
	AP3216C_get_addr(obj->hw, &obj->addr);

	INIT_WORK(&obj->eint_work, AP3216C_eint_work);
	obj->client = client;
	i2c_set_clientdata(client, obj);	
	atomic_set(&obj->als_debounce, 300);
	atomic_set(&obj->als_deb_on, 0);
	atomic_set(&obj->als_deb_end, 0);
	atomic_set(&obj->ps_debounce, 300);
	atomic_set(&obj->ps_deb_on, 0);
	atomic_set(&obj->ps_deb_end, 0);
	atomic_set(&obj->ps_mask, 0);
	atomic_set(&obj->als_suspend, 0);
	atomic_set(&obj->als_cmd_val, 0xDF);
	atomic_set(&obj->ps_cmd_val,  0xC1);
	atomic_set(&obj->ps_thd_val_h,  obj->hw->ps_threshold_high);
	atomic_set(&obj->ps_thd_val_l,  obj->hw->ps_threshold_low);
	obj->enable = 0;
	obj->pending_intr = 0;
	obj->als_level_num = sizeof(obj->hw->als_level)/sizeof(obj->hw->als_level[0]);
	obj->als_value_num = sizeof(obj->hw->als_value)/sizeof(obj->hw->als_value[0]);  
	obj->als_modulus = (400*100*40)/(1*1500);

	BUG_ON(sizeof(obj->als_level) != sizeof(obj->hw->als_level));
	memcpy(obj->als_level, obj->hw->als_level, sizeof(obj->als_level));
	BUG_ON(sizeof(obj->als_value) != sizeof(obj->hw->als_value));
	memcpy(obj->als_value, obj->hw->als_value, sizeof(obj->als_value));
	atomic_set(&obj->i2c_retry, 3);
	set_bit(CMC_BIT_ALS, &obj->enable);
	set_bit(CMC_BIT_PS, &obj->enable);

	
	AP3216C_i2c_client = client;

	if(err = AP3216C_init_client(client))
	{
		goto exit_init_failed;
	}
	APS_LOG("AP3216C_init_client() OK!\n");

	if(err = misc_register(&AP3216C_device))
	{
		APS_ERR("AP3216C_device register failed\n");
		goto exit_misc_device_register_failed;
	}
	obj_ps.self = AP3216C_obj;
	if(1 == obj->hw->polling_mode_ps)
	{
		obj_ps.polling = 1;
	}
	else
	{
		obj_ps.polling = 0;
	}
	
	obj_ps.sensor_operate = AP3216C_ps_operate;
	if(err = hwmsen_attach(ID_PROXIMITY, &obj_ps))
	{
		APS_ERR("attach fail = %d\n", err);
		goto exit_create_attr_failed;
	}
	printk("hct-alsps in func %s line %d\n",__func__,__LINE__);
	obj_als.self = AP3216C_obj;
	obj_als.polling = 1;
	obj_als.sensor_operate = AP3216C_als_operate;
	if(err = hwmsen_attach(ID_LIGHT, &obj_als))
	{
		APS_ERR("attach fail = %d\n", err);
		goto exit_create_attr_failed;
	}

#if defined(CONFIG_HAS_EARLYSUSPEND)
	obj->early_drv.level    = EARLY_SUSPEND_LEVEL_DISABLE_FB - 1,
	obj->early_drv.suspend  = AP3216C_early_suspend,
	obj->early_drv.resume   = AP3216C_late_resume,    
	register_early_suspend(&obj->early_drv);

	wake_lock_init(&chrg_lock, WAKE_LOCK_SUSPEND, "ap3216c_wake_lock");
#endif

#ifdef CONFIG_HAS_WAKELOCK
       wake_lock_init(&obj->ps_lock, WAKE_LOCK_SUSPEND, "ps wakelock");
#endif 

	APS_LOG("%s: OK\n", __func__);
	ap3216c_init_flag = 0;
	return 0;

	exit_create_attr_failed:
	misc_deregister(&AP3216C_device);
	exit_misc_device_register_failed:
	exit_init_failed:
	exit_kfree:
	kfree(obj);
	exit:
	AP3216C_i2c_client = NULL;           
	ap3216c_init_flag = -1;
	APS_ERR("%s: err = %d\n", __func__, err);
	return err;
}
/*----------------------------------------------------------------------------*/
static int AP3216C_i2c_remove(struct i2c_client *client)
{
	int err;	

	if(err = misc_deregister(&AP3216C_device))
	{
		APS_ERR("misc_deregister fail: %d\n", err);    
	}
	
	AP3216C_i2c_client = NULL;
	i2c_unregister_device(client);
	kfree(i2c_get_clientdata(client));

	return 0;
}

/*----------------------------------------------------------------------------*/
static int ap3216c_local_init(void)
{
	struct alsps_hw *hw = get_cust_alsps_ap3216c_hw();

	AP3216C_power(hw, 1);
	if(i2c_add_driver(&AP3216C_i2c_driver))
	{
		APS_ERR("add driver error\n");
		return -1;
	} 
	if(-1 == ap3216c_init_flag)
	{
	   return -1;
	}	
	return 0;
}
/*----------------------------------------------------------------------------*/
static int ap3216c_remove(void)
{
	struct alsps_hw *hw = get_cust_alsps_ap3216c_hw();
	APS_FUN();    
	AP3216C_power(hw, 0);    
	i2c_del_driver(&AP3216C_i2c_driver);
	return 0;
}
/*----------------------------------------------------------------------------*/
static int __init AP3216C_init(void)
{
	struct alsps_hw *hw = get_cust_alsps_ap3216c_hw();
	APS_DBG("%s: i2c_number=%d\n", __func__,hw->i2c_num);
	printk("<ALS/PS> %s\n", __func__);
	i2c_register_board_info(hw->i2c_num, &i2c_ap3216c, 1);
	alsps_driver_add(&ap3216c_init_info);
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit AP3216C_exit(void)
{
	APS_FUN();
}
// late_initcall(AP3216C_init);
/*----------------------------------------------------------------------------*/
module_init(AP3216C_init);
module_exit(AP3216C_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("YC Hou");
MODULE_DESCRIPTION("AP3216C driver");
MODULE_LICENSE("GPL");
