/* drivers/hwmon/mt6516/amit/em3071x.c - em3071x ALS/PS driver
 * 
 * Author: 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <linux/atomic.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/eint.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#include <asm/io.h>
#include <cust_eint.h>
#include <cust_alsps.h>
#include "em3071x.h"
#include <alsps.h>
/******************************************************************************
 * configuration
*******************************************************************************/
/*----------------------------------------------------------------------------*/
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#define EM3071X_DEV_NAME     "EM3071X"
/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[ALS/PS] "
#define APS_FUN(f)               pr_info(APS_TAG"%s %d \n", __FUNCTION__ , __LINE__)
#define APS_ERR(fmt, args...)    pr_err(APS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define APS_LOG(fmt, args...)    pr_info(APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)    pr_info(APS_TAG fmt, ##args)                 
/******************************************************************************
 * extern functions
*******************************************************************************/
/*for interrup work mode support --*/

#define PS_DRIVE 0Xb0  //50MA  //0XB8 //200MA  0XB6 100MA  //Psensor Çý¶¯Á¦
/*----------------------------------------------------------------------------*/
static struct i2c_client *em3071x_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id em3071x_i2c_id[] = {{EM3071X_DEV_NAME,0},{}};

/*----------------------------------------------------------------------------*/

static struct i2c_board_info __initdata i2c_em3071x={ I2C_BOARD_INFO(EM3071X_DEV_NAME, 0x48>>1)};

static int em3071x_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id); 
static int em3071x_i2c_remove(struct i2c_client *client);
static int em3071x_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
/*----------------------------------------------------------------------------*/
static int em3071x_i2c_suspend(struct i2c_client *client, pm_message_t msg);
static int em3071x_i2c_resume(struct i2c_client *client);
static int em3071x_setup_eint(struct i2c_client *client);
extern void mt_eint_mask(unsigned int eint_num);
extern void mt_eint_unmask(unsigned int eint_num);
extern void mt_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern void mt_eint_set_polarity(unsigned int eint_num, unsigned int pol);
extern unsigned int mt_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt_eint_registration(unsigned int eint_num, unsigned int flow,
							void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);

static struct em3071x_priv *g_em3071x_ptr = NULL;

static struct wake_lock chrg_lock;

static int intr_flag_value = 0;
static int em3071x_local_init(void);
static int em3071x_remove(void);

extern struct alsps_hw *get_cust_ep3071x_alsps_hw(void);


static DEFINE_MUTEX(sensor_lock);
static int em3071x_init_flag = -1; /* 0<==>OK -1 <==> fail */

static struct alsps_init_info em3071x_init_info = {
	.name = "em3071x",
	.init = em3071x_local_init,
	.uninit = em3071x_remove,
};
/*----------------------------------------------------------------------------*/
typedef enum {
    CMC_BIT_ALS    = 1,
    CMC_BIT_PS     = 2,
} CMC_BIT;
/*----------------------------------------------------------------------------*/
struct em3071x_i2c_addr {    /*define a series of i2c slave address*/
    u8  write_addr;  
    u8  ps_thd;     /*PS INT threshold*/
};
/*----------------------------------------------------------------------------*/
struct em3071x_priv {
    struct alsps_hw  *hw;
    struct i2c_client *client;
    struct work_struct  eint_work;

    /*i2c address group*/
    struct em3071x_i2c_addr  addr;
    
    /*misc*/
//    u16		    als_modulus;
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
	struct device_node *irq_node;
	int irq;

    /*data*/
    u16         als;
    u16          ps;
    u8          _align;
    u16         als_level_num;
    u16         als_value_num;
    u32         als_level[C_CUST_ALS_LEVEL-1];
    u32         als_value[C_CUST_ALS_LEVEL];

    atomic_t    als_cmd_val;    /*the cmd value can't be read, stored in ram*/
    atomic_t    ps_cmd_val;     /*the cmd value can't be read, stored in ram*/
    atomic_t    ps_thd_val;     /*the cmd value can't be read, stored in ram*/
    ulong       enable;         /*enable mask*/
    ulong       pending_intr;   /*pending interrupt*/

    /*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
    struct early_suspend    early_drv;
#endif     
};
/*----------------------------------------------------------------------------*/
static struct i2c_driver em3071x_i2c_driver = {	
	.probe      = em3071x_i2c_probe,
	.remove     = em3071x_i2c_remove,
	.suspend    = em3071x_i2c_suspend,
	.resume     = em3071x_i2c_resume,
	.id_table   = em3071x_i2c_id,
	.driver = {
		.owner          = THIS_MODULE,
		.name           = EM3071X_DEV_NAME,
	},
};



static int ps_enabled=0;

static struct em3071x_priv *em3071x_obj = NULL;

/*----------------------------------------------------------------------------*/
int em3071x_get_addr(struct alsps_hw *hw, struct em3071x_i2c_addr *addr)
{
	if(!hw || !addr)
	{
		return -EFAULT;
	}
	addr->write_addr= hw->i2c_addr[0];
	return 0;
}
/*----------------------------------------------------------------------------*/
static void em3071x_power(struct alsps_hw *hw, unsigned int on) 
{
	static unsigned int power_on = 0;

	//APS_LOG("power %s\n", on ? "on" : "off");
#ifdef __USE_LINUX_REGULATOR_FRAMEWORK__

#else
	if(hw->power_id != POWER_NONE_MACRO)
	{
		if(power_on == on)
		{
			APS_LOG("ignore power control: %d\n", on);
		}
		else if(on)
		{
			if(!hwPowerOn(hw->power_id, hw->power_vol, "EM3071X")) 
			{
				APS_ERR("power on fails!!\n");
			}
		}
		else
		{
			if(!hwPowerDown(hw->power_id, "EM3071X")) 
			{
				APS_ERR("power off fail!!\n");   
			}
		}
	}

#endif
	power_on = on;
}

/*----------------------------------------------------------------------------*/
static int em3071x_enable_als(struct i2c_client *client, int enable)
{
        struct em3071x_priv *obj = i2c_get_clientdata(client);
        u8 databuf[2];      
        int res = 0;
        u8 reg_value[1];
        u8 buffer[2];
        if(client == NULL)
        {
            APS_DBG("CLIENT CANN'T EQUL NULL\n");
            return -1;
        }

	      databuf[0] = EM3071X_CMM_ENABLE;    
            res = i2c_master_send(client, databuf, 0x1);
            if(res <= 0)
            {
                goto EXIT_ERR;
            }
            res = i2c_master_recv(client, databuf, 0x1);
            if(res <= 0)
            {
                goto EXIT_ERR;
            }

        if(enable)
        {			
            databuf[1] = ((databuf[0] & 0xF8) | 0x06);
    
            databuf[0] = EM3071X_CMM_ENABLE;    
            //databuf[1] = 0xBE;
            res = i2c_master_send(client, databuf, 0x2);
            if(res <= 0)
            {
                goto EXIT_ERR;
            }
           
            atomic_set(&obj->als_deb_on, 1);
            atomic_set(&obj->als_deb_end, jiffies+atomic_read(&obj->als_debounce)/(1000/HZ));
            APS_DBG("em3071x power on\n");
        }
        else
        {
            
            databuf[1] = (databuf[0] & 0xF8);
                     
            databuf[0] = EM3071X_CMM_ENABLE;    
        //    databuf[1] = 0xB8;    
            res = i2c_master_send(client, databuf, 0x2);
            if(res <= 0)
            {
                goto EXIT_ERR;
            }
            atomic_set(&obj->als_deb_on, 0);
            APS_DBG("EM3071X power off\n");
        }
		
        return 0;
        
    EXIT_ERR:
        APS_ERR("EM3071X_enable_als fail\n");
        return res;
}
/*----------------------------------------------------------------------------*/
static int em3071x_get_ps_value(struct em3071x_priv *obj, u16 ps)
{
	int val, mask = atomic_read(&obj->ps_mask);
	int invalid = 0;
	static int val_temp=1;

	printk("em3071x  get ps %d\n",ps);
	if((ps > atomic_read(&obj->ps_thd_val)))
	{
		val = 0;  /*close*/
		val_temp = 0;
		intr_flag_value = 1;
	}
	else if((ps < (atomic_read(&obj->ps_thd_val)-20)))
	{
		val = 1;  /*far away*/
		val_temp = 1;
		intr_flag_value = 0;
	}
	else
	       val = val_temp;	
				
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
	else if (obj->als > 1500)
	{
		//invalid = 1;
		APS_DBG("ligh too high will result to failt proximiy\n");
		return 1;  /*far away*/
	}

	if(!invalid)
	{
		//APS_DBG("PS:  %05d => %05d\n", ps, val);
		return val;
	}	
	else
	{
		return -1;
	}	
}

/*----------------------------------------------------------------------------*/
int em3071x_read_ps(struct i2c_client *client, u16 *data)
{
	struct em3071x_priv *obj = i2c_get_clientdata(client);    
	u16 ps_value;    
	u8 ps_value_low[1];
	u8 buffer[1];
	int res = 0;

	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUL NULL\n");
		return -1;
	}

	buffer[0]=EM3071X_CMM_PDATA_L;
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

	*data = ps_value_low[0];
	//zijian 
	//APS_DBG("ps_data=%d, low:%d \n ", *data, ps_value_low[0]);
	return 0;    

EXIT_ERR:
	APS_ERR("em3071x_read_ps fail\n");
	return res;
}
/*----------------------------------------------------------------------------*/
static int em3071x_enable_ps(struct i2c_client *client, int enable)
{
	struct em3071x_priv *obj = i2c_get_clientdata(client);
	u8 databuf[2];    
	int res = 0,err=0,val;
	u8 reg_value[1];
	u8 buffer[2];
	u8 offset_reg;
    int i,total_value;
	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUL NULL\n");
		return -1;
	}

	
	APS_DBG("em3071x_enable_ps, enable = %d\n", enable);

		databuf[0] = EM3071X_CMM_ENABLE;	
		res = i2c_master_send(client, databuf, 0x1);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}
		res = i2c_master_recv(client, databuf, 0x1);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}

	if(enable)
	{
		databuf[1] = ((databuf[0] & 0x07) | PS_DRIVE);  //0xb8 :200MA	 0XB6:100MA 
			
		databuf[0] = EM3071X_CMM_ENABLE;    

		res = i2c_master_send(client, databuf, 0x2);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}

		APS_DBG("em3071x power on\n");
		#if 1
		mdelay(10);
		total_value = 0;
		for (i=0;i<3;i++)
		{
	      em3071x_read_ps( client, &obj->ps);
	      total_value += obj->ps;
	      mdelay(100);
	    }
	    obj->ps = total_value/3;
		if(obj->ps >= 90 && obj->ps<=200)
		{
			atomic_set(&obj->ps_thd_val, obj->ps+40);
		}
		else
		{
			atomic_set(&obj->ps_thd_val,em3071x_obj->hw->ps_threshold);
		}
		printk("[ALS/PS] obj->ps_thd_val=%d\n",atomic_read(&obj->ps_thd_val));
	   #endif
		/*for interrup work mode support */
		if(0 == obj->hw->polling_mode_ps)
			{
					val = em3071x_get_ps_value(obj, obj->ps);

					err = ps_report_interrupt_data(val);
					if(err < 0)
					{
						APS_ERR("call hwmsen_get_interrupt_data fail = %d\n", err);
					}

				databuf[0] = EM3071X_CMM_INT_PS_LT;	
				databuf[1] = (u8)(((atomic_read(&obj->ps_thd_val))-20) & 0x00FF);
				res = i2c_master_send(client, databuf, 0x2);
				if(res <= 0)
				{
					goto EXIT_ERR;
					return EM3071X_ERR_I2C;
				}
				databuf[0] = EM3071X_CMM_INT_PS_HT;	
				databuf[1] = (u8)((atomic_read(&obj->ps_thd_val)) & 0x00FF);
				res = i2c_master_send(client, databuf, 0x2);
				if(res <= 0)
				{
					//goto EXIT_ERR;
					return EM3071X_ERR_I2C;
				}
			//zijian 
			//printk("zijian em3071x_enable_ps set interrupt\n");	

#if defined(CONFIG_OF)
				enable_irq(obj->irq);
#else
				mt_eint_unmask(CUST_EINT_ALS_NUM);
#endif

			
		}else{
			wake_lock(&chrg_lock);
			atomic_set(&obj->ps_deb_on, 1);
			atomic_set(&obj->ps_deb_end, jiffies+atomic_read(&obj->ps_debounce)/(1000/HZ));

		}
	}
	else
	{
		databuf[1] = (databuf[0] & 0x07);
		
		databuf[0] = EM3071X_CMM_ENABLE;    
                       
		res = i2c_master_send(client, databuf, 0x2);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}
		atomic_set(&obj->ps_deb_on, 0);
		APS_DBG("em3071x power off\n");

		/*for interrup work mode support */
		if(0 == obj->hw->polling_mode_ps)
		{
			cancel_work_sync(&obj->eint_work);
#if defined(CONFIG_OF)
				disable_irq_nosync(obj->irq);
#else
				mt_eint_mask(CUST_EINT_ALS_NUM);
#endif

		}else{
			wake_unlock(&chrg_lock);

		}
	}
	ps_enabled=enable;
	return 0;
	
EXIT_ERR:
	APS_ERR("em3071x_enable_ps fail\n");
	return res;
}

/*----------------------------------------------------------------------------*/
/*for interrup work mode support --*/
static int em3071x_check_and_clear_intr(struct i2c_client *client) 
{
	struct em3071x_priv *obj = i2c_get_clientdata(client);
	int res,intp;
	u8 buffer[2];

	buffer[0] = EM3071X_CMM_STATUS;
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
	//APS_ERR("em3071x_check_and_clear_intr status=0x%x\n", buffer[0]);
	if(0 != (buffer[0] & 0x80))
	{
		buffer[0] = EM3071X_CMM_STATUS;
		buffer[1] = 0x00;
		res = i2c_master_send(client, buffer, 0x2);
		if(res <= 0)
		{
			goto EXIT_ERR;
		}
		res = 0;
	}

	return res;

EXIT_ERR:
	APS_ERR("em3071x_check_and_clear_intr fail\n");
	return 1;
}

/*----------------------------------------------------------------------------*/
void em3071x_eint_func(void)
{
	struct em3071x_priv *obj = em3071x_obj;
	if(!obj)
	{
		return;
	}
//zijian test
//	printk("zijian , em3071x_eint_func \n");
	schedule_work(&obj->eint_work);
}
#if defined(CONFIG_OF)
static irqreturn_t em3071x_eint_handler(int irq, void *desc)
{
	struct em3071x_priv *obj = em3071x_obj;

	if (unlikely(obj == NULL)) {
		APS_ERR("%s--%d em3071x_obj is NULL!\n", __func__, __LINE__);
		return IRQ_HANDLED;
	}

	disable_irq_nosync(obj->irq);
	em3071x_eint_func();

	return IRQ_HANDLED;
}

#endif

/*----------------------------------------------------------------------------*/
/*for interrup work mode support --*/
int em3071x_setup_eint(struct i2c_client *client)
{
#if defined(CONFIG_OF)
		u32 ints[2] = {0};
		int err = 0;
#endif
	
		if (!em3071x_obj) {
			APS_ERR("em3071x_obj is null!!\n");
			return 0;
		}
	
		mt_set_gpio_dir(GPIO_ALS_EINT_PIN, GPIO_DIR_IN);
		mt_set_gpio_mode(GPIO_ALS_EINT_PIN, GPIO_ALS_EINT_PIN_M_EINT);
		mt_set_gpio_pull_enable(GPIO_ALS_EINT_PIN, GPIO_PULL_ENABLE);
		mt_set_gpio_pull_select(GPIO_ALS_EINT_PIN, GPIO_PULL_UP);
	
#if defined(CONFIG_OF)
		if (em3071x_obj->irq_node) {
			of_property_read_u32_array(em3071x_obj->irq_node, "debounce", ints, ARRAY_SIZE(ints));
			APS_LOG("ints[0]=%d, ints[1]=%d\n", ints[0], ints[1]);
			mt_gpio_set_debounce(ints[0], ints[1]);
	
			em3071x_obj->irq = irq_of_parse_and_map(em3071x_obj->irq_node, 0);
			APS_LOG("em3071x_obj->irq = %d\n", em3071x_obj->irq);
			if (!em3071x_obj->irq) {
				APS_ERR("irq_of_parse_and_map fail!!\n");
				return -EINVAL;
			}
			err = request_irq(em3071x_obj->irq, em3071x_eint_handler, IRQF_TRIGGER_NONE, "ALS-eint", NULL);
			if (err != 0) {
				APS_ERR("IRQ LINE NOT AVAILABLE!!\n");
				return -EINVAL;
			}
			enable_irq(em3071x_obj->irq);
		} else {
			APS_ERR("null irq node!!\n");
			return -EINVAL;
		}
#elif defined(CUST_EINT_ALS_TYPE)
		mt_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
		mt_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_TYPE, em3071x_eint_func, 0);
	
		mt_eint_mask(CUST_EINT_ALS_NUM);
#endif
		return 0;

}

static int em3071x_init_client(struct i2c_client *client)
{
	struct em3071x_priv *obj = i2c_get_clientdata(client);
	u8 databuf[2];    
	int res = 0;
	  	   
	   databuf[0] = EM3071X_CMM_ENABLE;	 
	   databuf[1] = PS_DRIVE;//0XBE 200MA
	   res = i2c_master_send(client, databuf, 0x2);
	   if(res <= 0)
	   {
			APS_FUN();
		   goto EXIT_ERR;
	   }
	   databuf[0] = 0x0F;	 
	   databuf[1] = 0x00;
	   res = i2c_master_send(client, databuf, 0x2);
	   if(res <= 0)
	   {
	  	   APS_FUN();
		   goto EXIT_ERR;
	   }
	   	/*for interrup work mode support*/
		if(0 == obj->hw->polling_mode_ps)
		{
			databuf[0] = EM3071X_CMM_INT_PS_LT;	
			databuf[1] = (u8)(((atomic_read(&obj->ps_thd_val))-20) & 0x00FF);
			res = i2c_master_send(client, databuf, 0x2);
			if(res <= 0)
			{
			    
				APS_FUN();
				goto EXIT_ERR;
				return EM3071X_ERR_I2C;
			}
			databuf[0] = EM3071X_CMM_INT_PS_HT;	
			databuf[1] = (u8)((atomic_read(&obj->ps_thd_val)) & 0x00FF);
			res = i2c_master_send(client, databuf, 0x2);
			if(res <= 0)
			{
				 APS_FUN();
				goto EXIT_ERR;
				return EM3071X_ERR_I2C;
			}

	}

	/*for interrup work mode support */
	if(res = em3071x_setup_eint(client))
	{
		APS_ERR("setup eint: %d\n", res);
		return res;
	}
	if(res = em3071x_check_and_clear_intr(client))
	{
		APS_ERR("check/clear intr: %d\n", res);
		//    return res;
	}
	
	return EM3071X_SUCCESS;

EXIT_ERR:
	APS_ERR("init dev: %d\n", res);
	return res;
}

/****************************************************************************** 
**
*****************************************************************************/
int em3071x_read_als(struct i2c_client *client, u16 *data)
{
	struct em3071x_priv *obj = i2c_get_clientdata(client);	 
	u16 c0_value;	 
	u8 als_value_low[1], als_value_high[1];
	u8 buffer[1];
	u16 atio;
	u16 als_value;
	int res = 0;
	
	if(client == NULL)
	{
		APS_DBG("CLIENT CANN'T EQUL NULL\n");
		return -1;
	}
//get adc channel 0 value
	buffer[0]=EM3071X_CMM_C0DATA_L;
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
	
	buffer[0]=EM3071X_CMM_C0DATA_H;
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
	
	c0_value = als_value_low[0] | ((0X0F&als_value_high[0])<<8);
	*data = c0_value;
	//printk("em3071x_read_als als_value = %d\t\n",c0_value);
	return 0;	 

	
	
EXIT_ERR:
	APS_ERR("em3071x_read_ps fail\n");
	return res;
}
/*----------------------------------------------------------------------------*/

static int em3071x_get_als_value(struct em3071x_priv *obj, u16 als)
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
		//APS_DBG("ALS: %05d => %05d\n", als, obj->hw->als_value[idx]);	
		return obj->hw->als_value[idx];
	}
	else
	{
		//APS_ERR("ALS: %05d => %05d (-1)\n", als, obj->hw->als_value[idx]);    
		return -1;
	}
}


/*for interrup work mode support --*/
static void em3071x_eint_work(struct work_struct *work)
{
	struct em3071x_priv *obj = (struct em3071x_priv *)container_of(work, struct em3071x_priv, eint_work);
	int err;
	hwm_sensor_data sensor_data;
	u8 buffer[1];
	u8 reg_value[1];
	u8 databuf[2];
	int res = 0;
	u8  need_drop = 0;
	
	if((err = em3071x_check_and_clear_intr(obj->client)))
	{
		APS_ERR("em3071x_eint_work check intrs: %d\n", err);
	}
	else
	{
		//get raw data
		em3071x_read_ps(obj->client, &obj->ps);
		em3071x_read_als(obj->client, &obj->als);
		APS_DBG("em3071x_eint_work rawdata ps=%d als_ch0=%d!\n",obj->ps,obj->als);
		sensor_data.values[0] = em3071x_get_ps_value(obj, obj->ps);
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;			
/*singal interrupt function add*/
		//let up layer to know
		err = ps_report_interrupt_data(sensor_data.values[0]);
		if(err < 0)
		{
	  		APS_ERR("call hwmsen_get_interrupt_data fail = %d\n", err);
		}
		
	}
	if((err = em3071x_check_and_clear_intr(obj->client)))
	{
		APS_ERR("em3071x_eint_work check intrs: %d\n", err);
		
	}
#if defined(CONFIG_OF)
	enable_irq(obj->irq);
#else
	mt_eint_unmask(CUST_EINT_ALS_NUM);
#endif 
	return;

EXIT_ERR:
#if defined(CONFIG_OF)
	enable_irq(obj->irq);
#else
	mt_eint_unmask(CUST_EINT_ALS_NUM);
#endif 
	APS_ERR("em3071x_eint_work error \n");
	return;
}


/****************************************************************************** 
 * Function Configuration
******************************************************************************/
static int em3071x_open(struct inode *inode, struct file *file)
{
	file->private_data = em3071x_i2c_client;

	if (!file->private_data)
	{
		APS_ERR("null pointer!!\n");
		return -EINVAL;
	}
	
	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int em3071x_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}


/*************************************************************/

static int em3071x_ioctl( struct file *file, unsigned int cmd,
       unsigned long arg)
{
	struct i2c_client *client = (struct i2c_client*)file->private_data;
	struct em3071x_priv *obj = i2c_get_clientdata(client);  
	int err = 0;
	void __user *ptr = (void __user*) arg;
	int dat;
	uint32_t enable;

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
				if(err = em3071x_enable_ps(obj->client, 1))
				{
					APS_ERR("enable ps fail: %d\n", err); 
					goto err_out;
				}
				
				set_bit(CMC_BIT_PS, &obj->enable);
			}
			else
			{
				if(err = em3071x_enable_ps(obj->client, 0))
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
			if(err = em3071x_read_ps(obj->client, &obj->ps))
			{
				goto err_out;
			}
			
			dat = em3071x_get_ps_value(obj, obj->ps);
			if(dat == -1)
			{
				err = -EFAULT;
				goto err_out;
			}
			
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}  
			break;

		case ALSPS_GET_PS_RAW_DATA:    
			if(err = em3071x_read_ps(obj->client, &obj->ps))
			{
				goto err_out;
			}
			//zijian
			/*
		 	if(obj->ps == 0)
			{
				err = -EFAULT;
				goto err_out;
			}*/
			
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
				if(err = em3071x_enable_als(obj->client, 1))
				{
					APS_ERR("enable als fail: %d\n", err); 
					goto err_out;
				}
				set_bit(CMC_BIT_ALS, &obj->enable);
			}
			else
			{
				if(err = em3071x_enable_als(obj->client, 0))
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
			if(err = em3071x_read_als(obj->client, &obj->als))
			{
				goto err_out;
			}

			dat = em3071x_get_als_value(obj, obj->als);
			if(copy_to_user(ptr, &dat, sizeof(dat)))
			{
				err = -EFAULT;
				goto err_out;
			}              
			break;

		case ALSPS_GET_ALS_RAW_DATA:    
			if(err = em3071x_read_als(obj->client, &obj->als))
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
		default:
			APS_ERR("%s not supported = 0x%04x", __FUNCTION__, cmd);
			err = -ENOIOCTLCMD;
			break;
	}

	err_out:
	return err;    
}
/*----------------------------------------------------------------------------*/
static struct file_operations em3071x_fops = {
	.owner = THIS_MODULE,
	.open = em3071x_open,
	.release = em3071x_release,
	.unlocked_ioctl = em3071x_ioctl,
};
/*----------------------------------------------------------------------------*/
static struct miscdevice em3071x_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "als_ps",
	.fops = &em3071x_fops,
};
/*----------------------------------------------------------------------------*/
static int em3071x_i2c_suspend(struct i2c_client *client, pm_message_t msg) 
{
//	struct em3071x_priv *obj = i2c_get_clientdata(client);    
//	int err;
	APS_FUN(); 
	if(ps_enabled)
	{
		return -EACCES;
	}
	
	return 0;
}
/*----------------------------------------------------------------------------*/
static int em3071x_i2c_resume(struct i2c_client *client)
{
//	struct em3071x_priv *obj = i2c_get_clientdata(client);        
	APS_FUN();
	return 0;
}
/*----------------------------------------------------------------------------*/
static void em3071x_early_suspend(struct early_suspend *h) 
{   /*early_suspend is only applied for ALS*/
	struct em3071x_priv *obj = container_of(h, struct em3071x_priv, early_drv);   
	int err;
	APS_FUN();    

	if(!obj)
	{
		APS_ERR("null pointer!!\n");
		return;
	}

	#if 1
	
	atomic_set(&obj->als_suspend, 1);
	
	mutex_lock(&sensor_lock);
		if(err = em3071x_enable_als(obj->client, 0))
		{
			APS_ERR("disable als fail: %d\n", err); 
		}
	mutex_unlock(&sensor_lock);
		
	#endif
}
/*----------------------------------------------------------------------------*/
static void em3071x_late_resume(struct early_suspend *h)
{   /*late_resume is only applied for ALS*/
	struct em3071x_priv *obj = container_of(h, struct em3071x_priv, early_drv);         
	int err;
	APS_FUN();

	if(!obj)
	{
		APS_ERR("null pointer!!\n");
		return;
	}

        #if 1
		
	atomic_set(&obj->als_suspend, 0);
	if(test_bit(CMC_BIT_ALS, &obj->enable))
	{
		mutex_lock(&sensor_lock);		
		if(err = em3071x_enable_als(obj->client, 1))
		{
			APS_ERR("enable als fail: %d\n", err);        

		}
		
		mutex_unlock(&sensor_lock);
	}
	#endif
}
static int em3071x_als_open_report_data(int open)
{

	return 0;
}

static int em3071x_als_enable_nodata(int en)
{
	struct em3071x_priv *obj = em3071x_obj;

	int value = 0;
	int err = 0;

	if (obj == NULL) {
		APS_ERR("em3071x_obj is null\n");
		return -1;
	}

	value = en;
	if(value)
	{
	
		mutex_lock(&sensor_lock);
		if(err = em3071x_enable_als(obj->client, 1))
		{
			APS_ERR("enable als fail: %d\n", err); 
			
			mutex_unlock(&sensor_lock);
			return -1;
		}
		
		mutex_unlock(&sensor_lock);
		set_bit(CMC_BIT_ALS, &obj->enable);
	}
	else
	{
		mutex_lock(&sensor_lock);
		if(err = em3071x_enable_als(obj->client, 0))
		{
		
			mutex_unlock(&sensor_lock);
			APS_ERR("disable als fail: %d\n", err); 
			return -1;
		}
		
		mutex_unlock(&sensor_lock);
		
		clear_bit(CMC_BIT_ALS, &obj->enable);
	}

	return 0;
}

static int em3071x_als_set_delay(u64 ns)
{
	return 0;
}

static int em3071x_als_get_data(int *value, int *status)
{
	struct em3071x_priv *obj = em3071x_obj;
	int err = 0;

	if (obj == NULL) {
		APS_ERR("em3071x_obj is null\n");
		return -1;
	}

	mutex_lock(&sensor_lock);
	em3071x_read_als(obj->client, &obj->als);
	mutex_unlock(&sensor_lock);
					
	*value = em3071x_get_als_value(obj, obj->als);

	*status = SENSOR_STATUS_ACCURACY_MEDIUM;


	return err;
}

static int em3071x_ps_open_report_data(int open)
{
	return 0;
}

static int em3071x_ps_enable_nodata(int en)
{
	struct em3071x_priv *obj = em3071x_obj;

	int value = 0;
	int err = 0;


	if (obj == NULL) {
		APS_ERR("em3071x_obj is null\n");
		return -1;
	}
	value = en;
	if(value)
	{
	
		mutex_lock(&sensor_lock);
		if(err = em3071x_enable_ps(obj->client, 1))
		{
			APS_ERR("enable ps fail: %d\n", err); 
			
			mutex_unlock(&sensor_lock);
			return -1;
		}
		mutex_unlock(&sensor_lock);
		
		set_bit(CMC_BIT_PS, &obj->enable);
		
	}
	else
	{
	
		mutex_lock(&sensor_lock);
		if(err = em3071x_enable_ps(obj->client, 0))
		{
			APS_ERR("disable ps fail: %d\n", err); 
			
			mutex_unlock(&sensor_lock);
			return -1;
		}
		
		mutex_unlock(&sensor_lock);
		clear_bit(CMC_BIT_PS, &obj->enable);
		
	}


	return 0;
}

static int em3071x_ps_set_delay(u64 ns)
{
	return 0;
}

static int em3071x_ps_get_data(int *value, int *status)
{
	struct em3071x_priv *obj = em3071x_obj;

	int err = 0;

	if (obj == NULL) {
		APS_ERR("em3071x_priv is null\n");
		return -1;
	}

	mutex_lock(&sensor_lock);
	em3071x_read_ps(obj->client, &obj->ps);
	printk("[ALS/PS] obj->ps=%d\n",obj->ps);
	//mdelay(160);
	em3071x_read_als(obj->client, &obj->als);
					
	mutex_unlock(&sensor_lock);
	//APS_ERR("em3071x_ps_operate als data=%d!\n",obj->als);
	*value = em3071x_get_ps_value(obj, obj->ps);

	*status = SENSOR_STATUS_ACCURACY_MEDIUM;	


	return 0;
}





/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static int em3071x_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct em3071x_priv *obj;
	struct als_control_path als_ctl = {0};
	struct als_data_path als_data = {0};
	struct ps_control_path ps_ctl = {0};
	struct ps_data_path ps_data = {0};

	int err = 0;

	if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
	{
		err = -ENOMEM;
		goto exit;
	}
	memset(obj, 0, sizeof(*obj));
	em3071x_obj = obj;

	APS_LOG("em3071x_init_client() in!\n");
	obj->hw = get_cust_ep3071x_alsps_hw();
	em3071x_get_addr(obj->hw, &obj->addr);

	/*for interrup work mode support --*/
	INIT_WORK(&obj->eint_work, em3071x_eint_work);
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
	atomic_set(&obj->ps_thd_val,  obj->hw->ps_threshold);
	obj->enable = 0;
	obj->pending_intr = 0;
	obj->als_level_num = sizeof(obj->hw->als_level)/sizeof(obj->hw->als_level[0]);
	obj->als_value_num = sizeof(obj->hw->als_value)/sizeof(obj->hw->als_value[0]);  
	
	BUG_ON(sizeof(obj->als_level) != sizeof(obj->hw->als_level));
	memcpy(obj->als_level, obj->hw->als_level, sizeof(obj->als_level));
	BUG_ON(sizeof(obj->als_value) != sizeof(obj->hw->als_value));
	memcpy(obj->als_value, obj->hw->als_value, sizeof(obj->als_value));
	atomic_set(&obj->i2c_retry, 3);
	set_bit(CMC_BIT_ALS, &obj->enable);
	set_bit(CMC_BIT_PS, &obj->enable);

	
	em3071x_i2c_client = client;

	//client->timing = 20;
#if defined(CONFIG_OF)
	obj->irq_node = of_find_compatible_node(NULL, NULL, "mediatek, ALS-eint");
#endif
	if(err = em3071x_init_client(client))
	{
		goto exit_init_failed;
	}
	APS_LOG("em3071x_init_client() OK!\n");

	if(err = misc_register(&em3071x_device))
	{
		APS_ERR("em3071x_device register failed\n");
		goto exit_misc_device_register_failed;
	}

	if (1 == obj->hw->polling_mode_ps) {
		ps_ctl.is_report_input_direct = false;
		ps_ctl.is_polling_mode = true;
		wake_lock_init(&chrg_lock, WAKE_LOCK_SUSPEND, "em3071x_wake_lock");
	} else{
		ps_ctl.is_report_input_direct = true;
		ps_ctl.is_polling_mode = false;

	}

	ps_ctl.open_report_data = em3071x_ps_open_report_data;
	ps_ctl.enable_nodata = em3071x_ps_enable_nodata;
	ps_ctl.set_delay = em3071x_ps_set_delay;
	ps_ctl.is_support_batch = obj->hw->is_batch_supported_ps;
	ps_ctl.is_use_common_factory = false;

	err = ps_register_control_path(&ps_ctl);
	if (err != 0) {
		APS_ERR("ps_register_control_path fail = %d\n", err);
		goto exit_create_attr_failed;
	}

	ps_data.get_data = em3071x_ps_get_data;
	ps_data.vender_div = 1;

	err = ps_register_data_path(&ps_data);

	if (err != 0) {
		APS_ERR("ps_register_data_path fail = %d\n", err);
		goto exit_create_attr_failed;
	}

	als_ctl.open_report_data = em3071x_als_open_report_data;
	als_ctl.enable_nodata = em3071x_als_enable_nodata;
	als_ctl.set_delay = em3071x_als_set_delay;
	als_ctl.is_support_batch = obj->hw->is_batch_supported_als;
	als_ctl.is_use_common_factory = false;
	als_ctl.is_report_input_direct = false;
	als_ctl.is_polling_mode = true;

	err = als_register_control_path(&als_ctl);
	if (err != 0) {
		APS_ERR("als_register_control_path fail = %d\n", err);
		goto exit_create_attr_failed;
	}

	als_data.get_data = em3071x_als_get_data;
	als_data.vender_div = 1;

	err = als_register_data_path(&als_data);

	if (err != 0) {
		APS_ERR("als_register_data_path fail = %d\n", err);
		goto exit_create_attr_failed;
	}


#if defined(CONFIG_HAS_EARLYSUSPEND)
	obj->early_drv.level    = EARLY_SUSPEND_LEVEL_DISABLE_FB - 1,
	obj->early_drv.suspend  = em3071x_early_suspend,
	obj->early_drv.resume   = em3071x_late_resume,    
	register_early_suspend(&obj->early_drv);
#endif
       em3071x_init_flag=0;
	APS_LOG("%s: OK\n", __func__);
	return 0;

	exit_create_attr_failed:
	misc_deregister(&em3071x_device);
	exit_misc_device_register_failed:
	exit_init_failed:
	//i2c_detach_client(client);
	exit_kfree:
	kfree(obj);
	exit:
	em3071x_i2c_client = NULL;           
//	MT6516_EINTIRQMask(CUST_EINT_ALS_NUM);  /*mask interrupt if fail*/
	APS_ERR("%s: err = %d\n", __func__, err);
em3071x_init_flag=-1;
	return err;
}
/*----------------------------------------------------------------------------*/
static int em3071x_i2c_remove(struct i2c_client *client)
{
	int err;	
	if(err = misc_deregister(&em3071x_device))
	{
		APS_ERR("misc_deregister fail: %d\n", err);    
	}
	
	em3071x_i2c_client = NULL;
	i2c_unregister_device(client);
	kfree(i2c_get_clientdata(client));

	return 0;
}

/*----------------------------------------------------------------------------*/
static int em3071x_local_init(void)
{
	struct alsps_hw *hw = get_cust_ep3071x_alsps_hw();

	em3071x_power(hw, 1);	
        APS_DBG("cch em3071x_local_init   \n");
		
	APS_DBG("%s, I2C = %d, addr =0x%x\n",__func__, hw->i2c_num,hw->i2c_addr[0]);
	if(i2c_add_driver(&em3071x_i2c_driver))
	{
		APS_ERR("add driver error\n");
		return -1;
	} 

	if (-1 == em3071x_init_flag) {
		APS_ERR("add driver--em3071x_init_flag check error\n");
		return -1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
static int em3071x_remove(void)
{
	struct alsps_hw *hw = get_cust_ep3071x_alsps_hw();
	APS_FUN();    
	em3071x_power(hw, 0);    
	i2c_del_driver(&em3071x_i2c_driver);

	em3071x_init_flag = -1;

	return 0;
}


/*----------------------------------------------------------------------------*/
static int __init em3071x_init(void)
{
	struct alsps_hw *hw = get_cust_ep3071x_alsps_hw();
	
	APS_LOG("%s: cch alsps  i2c_number=%d\n", __func__,hw->i2c_num); 
	//i2c_em3071x.addr = hw->i2c_addr[0] ;
	i2c_register_board_info(hw->i2c_num, &i2c_em3071x, 1);
	
	alsps_driver_add(&em3071x_init_info);
	return 0; 
}
/*----------------------------------------------------------------------------*/
static void __exit em3071x_exit(void)
{
	APS_FUN();

}
/*----------------------------------------------------------------------------*/
module_init(em3071x_init); 
module_exit(em3071x_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("binghua chen@epticore.com");
MODULE_DESCRIPTION("em3071x driver");
MODULE_LICENSE("GPL");
