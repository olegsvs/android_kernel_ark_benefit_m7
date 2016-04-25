//************************//
//base on ft5206 from other people
//************************//

#include "tpd.h"
#include <linux/interrupt.h>
#include <cust_eint.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>

#include "tpd_custom_ft6x06.h"
#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif
#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>

#include "cust_gpio_usage.h"


#define TPD_PROXIMITY		//huangxi add 20130605

#ifdef TPD_PROXIMITY
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
  
#define TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEVICE            "proximity-ft6x06"
#define TPD_PROXIMITY_DMESG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#if defined(TPD_PROXIMITY_DEBUG)
#undef TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEBUG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#else
#define TPD_PROXIMITY_DEBUG(arg...) 
#endif
#define TPD_PROXIMITY_ENABLE_REG	0xB0
static u8 tpd_proximity_flag = 0;
static u8 tpd_proximity_detect = 1;//0-->close ; 1--> far away
#endif



 #if defined(TPD_DEBUG)
#undef TPD_DEBUG
#define TPD_DEBUG(a,arg...) printk(TPD_DEVICE ": " a,##arg)
#else
#define TPD_DEBUG(arg...) 
#endif

 
extern struct tpd_device *tpd;
 
struct i2c_client *i2c_client = NULL;
struct task_struct *thread = NULL; 
 
static DECLARE_WAIT_QUEUE_HEAD(waiter);
 
 
static void tpd_eint_interrupt_handler(void);
 
 #if 0
 extern void mt65xx_eint_unmask(unsigned int line);
 extern void mt65xx_eint_mask(unsigned int line);
 extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
 extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
 extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
									  kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
									  kal_bool auto_umask);

 #endif
//#define TP_PROXIMITY_SENSOR		//jeff open 20140117

#ifdef TP_PROXIMITY_SENSOR
#define TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEVICE            "mtk-tpd-ft6x06"
#define TPD_PROXIMITY_DMESG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#if defined(TPD_PROXIMITY_DEBUG)
#undef TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEBUG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#else
#define TPD_PROXIMITY_DEBUG(arg...)
#endif
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
static u8 tpd_proximity_flag = 0;
static u8 tpd_proximity_detect = 1;//0-->close ; 1--> far away
int CTP_NORMAL_FLAG = 1;
u8 ft5206_proximity_stop_by_powerkey = 0;
#endif

//#define GT9XX_TP_PS	//jeff add 20140423
#ifdef GT9XX_TP_PS
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>

//#define TPD_PROXIMITY_VALID_REG                   0x814E
#define TPD_PROXIMITY_ENABLE_REG                  0xB0
//0x8042
static u8 tpd_proximity_flag = 0;
static u8 tpd_proximity_detect = 1;//0-->close ; 1--> far away
static u8 tpd_deep_sleep = 1;
#endif


static int tpd_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int  tpd_remove(struct i2c_client *client);
static int touch_event_handler(void *unused);
 

static int tpd_flag = 0;
static int point_num = 0;
static int p_point_num = 0;

//#define TPD_CLOSE_POWER_IN_SLEEP
 
#define TPD_OK 0
//register define
#define tpd_lcd_width                TPD_RES_X
//480
#define tpd_lcd_height                TPD_RES_Y
//854

#define DEVICE_MODE 0x00
#define GEST_ID 0x01
#define TD_STATUS 0x02

#define TOUCH1_XH 0x03
#define TOUCH1_XL 0x04
#define TOUCH1_YH 0x05
#define TOUCH1_YL 0x06

#define TOUCH2_XH 0x09
#define TOUCH2_XL 0x0A
#define TOUCH2_YH 0x0B
#define TOUCH2_YL 0x0C

#define TOUCH3_XH 0x0F
#define TOUCH3_XL 0x10
#define TOUCH3_YH 0x11
#define TOUCH3_YL 0x12

#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif
//register define

#define FSYSINFO	//jeff add 20140611
#ifdef FSYSINFO
static struct kobject *fsensor_sys_device;

static unsigned short TPD_PROXIMITY_SWITCH_FLAG = 0;  // 0-->OFF   1-->ON

//static u16 camera_i2c_id = 0x78; //OV5645_I2C_ADDR;
//static unsigned long s_write_addr;
//static DEVICE_ATTR(fwrite, 0666, write_camera_addr_show, write_camera_reg_store);
//static unsigned long s_read_addr;
static ssize_t read_camera_addr_show(struct device *dev,
                struct device_attribute *attr,
                char *buf)
{
    ssize_t rc = 0;
//    kal_uint16 get_byte=0;

//    iReadReg((u16) s_read_addr ,(u8*)&get_byte, camera_i2c_id);
    sprintf(buf, "%d", TPD_PROXIMITY_SWITCH_FLAG);
printk("Flag jeff tpd_ps TPD_PROXIMITY_SWITCH_FLAG=%d \n", TPD_PROXIMITY_SWITCH_FLAG);
    rc = strlen(buf) + 1;

    return rc;
}
static ssize_t read_camera_addr_store(struct device *dev,
                struct device_attribute *attr,
                const char *buf, size_t size)
{
    int rc = 0;

    rc = kstrtoul(buf, 0, &TPD_PROXIMITY_SWITCH_FLAG);
	printk("jeff tpd_ps SWITCH_FLAG=%d\n", TPD_PROXIMITY_SWITCH_FLAG);
    if (rc)
        return rc;

    return size;
}
static DEVICE_ATTR(switch, 0666, read_camera_addr_show, read_camera_addr_store);
//static DEVICE_ATTR(fsetid, 0666, set_id_camera_addr_show, set_id_camera_addr_store);

static int init_sys_device_name(void)
{
    int rc = 0;
    static int is_first_in = 1;

    if (!is_first_in) {
        return 0;
    }
	printk("jeff tpd_ps device reg.\n");
    is_first_in = 0;
    fsensor_sys_device = kobject_create_and_add("tpd_ps", NULL);	//"fsensor"
    if (fsensor_sys_device == NULL) {
        pr_info("%s: subsystem_register failed\n", __func__);
        rc = -ENXIO;
        return rc ;
    }
    rc = sysfs_create_file(fsensor_sys_device, &dev_attr_switch.attr);
    if (rc) {
        pr_info("%s: sysfs_create_file failed\n", __func__);
        kobject_del(fsensor_sys_device);
    }
    return rc;
}
#endif

 
#ifdef TP_PROXIMITY_SENSOR
static int tpd_get_ps_value(void)
{
	printk("tpd_proximity_detect=%d \r\n",tpd_proximity_detect);
	return tpd_proximity_detect;
}

static int tpd_enable_ps(int enable)
{
	 static char wdata = 0x01;
	 static char rdata = 0x00;
//	 printk("[kktest]tpd_enable_ps:en=%d\r\n",enable);
	if (enable)
	{
		i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &wdata);
		tpd_proximity_flag = 1;
		ft5206_proximity_stop_by_powerkey = 0;   //clear ft5206_proximity_stop_by_powerkey by xuke 
	}
	else
	{
		 i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &(rdata)); 
		 tpd_proximity_flag = 0;
	}
	msleep(20);
	return 0;
}

void ft5206_tpd_close_ps()
{
	hwm_sensor_data sensor_data;
	s32 ret = -1;
	if (tpd_proximity_detect == 0)
	{
//		printk("kktest ft5206_proximity_stop_by_powerkey set  1\n");
		ft5206_proximity_stop_by_powerkey = 1;

		tpd_proximity_detect = 1;	
		
		//tpd_eint_interrupt_handler();
	}
}

static int tpd_ps_operate(void* self, uint32_t command, void* buff_in, int size_in,
				   void* buff_out, int size_out, int* actualout)
{
	int err = 0;
	int value;
	hwm_sensor_data *sensor_data;

	switch (command)
	{
	printk("tpd_ps_operate: command=%d, buff_in=%d\r\n",command,*(int *)buff_in);
	case SENSOR_DELAY:
		if((buff_in == NULL) || (size_in < sizeof(int)))
		{
			err = -EINVAL;
		}
		// Do nothing
		break;

	case SENSOR_ENABLE:
		if((buff_in == NULL) || (size_in < sizeof(int)))
		{
			err = -EINVAL;
		}
		else
		{
			value = *(int *)buff_in;
			if(value)
			{
				if((tpd_enable_ps(1) != 0))
				{
					TPD_PROXIMITY_DMESG("enable ps fail: %d\n", err);
					return -1;
				}
			}
			else
			{
				if((tpd_enable_ps(0) != 0))
				{
					TPD_PROXIMITY_DMESG("disable ps fail: %d\n", err);
					return -1;
				}
			}
		}
		break;

	case SENSOR_GET_DATA:
		if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
		{
			TPD_PROXIMITY_DMESG("get sensor data parameter error!\n");
			err = -EINVAL;
		}
		else
		{
			sensor_data = (hwm_sensor_data *)buff_out;

			sensor_data->values[0] = tpd_get_ps_value();
			sensor_data->value_divide = 1;
			sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
		}
		break;

	default:
		TPD_PROXIMITY_DMESG("proxmy sensor operate function no this parameter %d!\n", command);
		err = -1;
		break;
	}

	return err;

}
#endif


#ifdef GT9XX_TP_PS
static s32 tpd_get_ps_value(void)
{
    return tpd_proximity_detect;
}

static int ft5x0x_i2c_txdata(char *txdata, int length)
{
    int ret;
    struct i2c_msg msg[] =
    {
        {
            .addr   = i2c_client->addr,
            .flags  = 0,
            .len    = length,
            .buf    = txdata,
        },
    };
    //msleep(1);
    ret = i2c_transfer(i2c_client->adapter, msg, 1);

    if(ret < 0)
    {
        pr_err("%s i2c write error: %d\n", __func__, ret);
    }

    return ret;
}

static int ft5x0x_write_reg(u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;
    buf[0] = addr;
    buf[1] = para;
    ret = ft5x0x_i2c_txdata(buf, 2);

    if(ret < 0)
    {
        pr_err("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }

    return 0;
}

static s32 tpd_enable_ps(s32 enable)
{
    u8  state;
    s32 ret = -1;

    if (enable)
    {
        state = 1;
        tpd_proximity_flag = 1;
        printk("gt9xx jeff TPD proximity function to be on.\n");
    }
    else
    {
        state = 0;
        tpd_proximity_flag = 0;
        printk("gt9xx jeff TPD proximity function to be off.\n");
    }

//    ret = i2c_write_bytes(i2c_client_point, TPD_PROXIMITY_ENABLE_REG, &state, 1);
	ret = ft5x0x_write_reg(TPD_PROXIMITY_ENABLE_REG, state);

    if (ret < 0)
    {
        printk("gt9xx jeff TPD %s proximity cmd failed.", state ? "enable" : "disable");
        return ret;
    }

    printk("gt9xx jeff TPD proximity function %s success.", state ? "enable" : "disable");
    return 0;
}

s32 tpd_ps_operate(void *self, u32 command, void *buff_in, s32 size_in,
                   void *buff_out, s32 size_out, s32 *actualout)
{
    s32 err = 0;
    s32 value;
    hwm_sensor_data *sensor_data;

    switch (command)
    {
        case SENSOR_DELAY:
            if ((buff_in == NULL) || (size_in < sizeof(int)))
            {
                printk("gt9xx jeff Set delay parameter error!");
                err = -EINVAL;
            }

            // Do nothing
            break;

        case SENSOR_ENABLE:
            if ((buff_in == NULL) || (size_in < sizeof(int)))
            {
                printk("gt9xx jeff Enable sensor parameter error!");
                err = -EINVAL;
            }
            else
            {
                value = *(int *)buff_in;
	#ifdef FSYSINFO	//jeff add 20140611
		if (TPD_PROXIMITY_SWITCH_FLAG == 0){
			printk("Flag jeff disable tpd_ps!!!00000000000!!!\n");
			err = tpd_enable_ps(0);
		} else {
			printk("Flag jeff enable tpd_ps!!!11111111111!!!\n");
			err = tpd_enable_ps(value);
		}
	#else
                err = tpd_enable_ps(value);
	#endif
            }

            break;

        case SENSOR_GET_DATA:
            if ((buff_out == NULL) || (size_out < sizeof(hwm_sensor_data)))
            {
                printk("gt9xx jeff Get sensor data parameter error!");
                err = -EINVAL;
            }
            else
            {
                sensor_data = (hwm_sensor_data *)buff_out;
                sensor_data->values[0] = tpd_get_ps_value();
                sensor_data->value_divide = 1;
                sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
            }

            break;

        default:
            printk("gt9xx jeff proxmy sensor operate function no this parameter %d!\n", command);
            err = -1;
            break;
    }

    return err;
}
#endif


struct touch_info {
    int y[3];
    int x[3];
    int p[3];
    int count;
};
 
 static const struct i2c_device_id tpd_id[] = {{TPD_DEVICE,0},{}};
 unsigned short force[] = {0,0x70,I2C_CLIENT_END,I2C_CLIENT_END}; 
 static const unsigned short * const forces[] = { force, NULL };
 //static struct i2c_client_address_data addr_data = { .forces = forces, };
 static struct i2c_board_info __initdata i2c_tpd={ I2C_BOARD_INFO(TPD_DEVICE, (0x70>>1))};
  
 static struct i2c_driver tpd_i2c_driver = {
	 .probe = tpd_probe,								   
	 .remove = tpd_remove,							 
	 .detect = tpd_detect,							 
	 .driver.name = TPD_DEVICE, 
	 .id_table = tpd_id,							 
	 .address_list = (const unsigned short*) forces,					 
 };
  

#ifdef TPD_PROXIMITY
static int ft5x0x_i2c_txdata(char *txdata, int length)
{
    int ret;
    struct i2c_msg msg[] =
    {
        {
            .addr   = i2c_client->addr,
            .flags  = 0,
            .len    = length,
            .buf    = txdata,
        },
    };
    //msleep(1);
    ret = i2c_transfer(i2c_client->adapter, msg, 1);

    if(ret < 0)
    {
        pr_err("%s i2c write error: %d\n", __func__, ret);
    }

    return ret;
}

static int ft5x0x_write_reg(u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;
    buf[0] = addr;
    buf[1] = para;
    ret = ft5x0x_i2c_txdata(buf, 2);

    if(ret < 0)
    {
        pr_err("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }

    return 0;
}

static int tpd_get_ps_value(void)
{
	return tpd_proximity_detect;
}

static int tpd_enable_ps(int enable)
{
	u8 state;
	int ret = -1;
	if (enable){
		state = 1;
		tpd_proximity_flag = 1;
		TPD_PROXIMITY_DEBUG("tpd-ps function is on\n");
	}else{
		state = 0;	
		tpd_proximity_flag = 0;
		TPD_PROXIMITY_DEBUG("tpd-ps function is off\n");
	}
	ft5x0x_write_reg(TPD_PROXIMITY_ENABLE_REG, state);
	return 0;
}

int tpd_ps_operate(void* self, uint32_t command, void* buff_in, int size_in,
		void* buff_out, int size_out, int* actualout)
{
	int err = 0;
	int value;
	hwm_sensor_data *sensor_data;
	
	switch (command)
	{
		case SENSOR_DELAY:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				TPD_PROXIMITY_DMESG("Set delay parameter error!\n");
				err = -EINVAL;
			}
			// Do nothing
			TPD_PROXIMITY_DMESG("Set delay do nothing!\n");	//huangxi add 20130609
			break;

		case SENSOR_ENABLE:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				TPD_PROXIMITY_DMESG("Enable sensor parameter error!\n");
				err = -EINVAL;
			}
			else
			{				
				value = *(int *)buff_in;
				if(value)
				{
					if((tpd_enable_ps(1) != 0))
					{
						TPD_PROXIMITY_DMESG("enable ps fail: %d\n", err); 
						return -1;
					}
//					set_bit(CMC_BIT_PS, &obj->enable);
				}
				else
				{
					if((tpd_enable_ps(0) != 0))
					{
						TPD_PROXIMITY_DMESG("disable ps fail: %d\n", err); 
						return -1;
					}
//					clear_bit(CMC_BIT_PS, &obj->enable);
				}
			}
			break;

		case SENSOR_GET_DATA:
			if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
			{
				TPD_PROXIMITY_DMESG("get sensor data parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				sensor_data = (hwm_sensor_data *)buff_out;				
				
				sensor_data->values[0] = tpd_get_ps_value();
				sensor_data->value_divide = 1;
				sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;		
			}
			break;
			
		default:
			TPD_PROXIMITY_DMESG("proxmy sensor operate function no this parameter %d!\n", command);
			err = -1;
			break;
	}
	
	return err;
	
}
#endif

 
static  void tpd_down(int x, int y, int p) {
	// input_report_abs(tpd->dev, ABS_PRESSURE, p);
        if(y>0){//by zhu
	 input_report_key(tpd->dev, BTN_TOUCH, 1);
	 input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	 //printk("D[%4d %4d %4d] ", x, y, p);
	 input_mt_sync(tpd->dev);
      }
//remove at 20150327 by zhu for button
 //  if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
//remove at 20150327 by zhu for button end
   {   
       tpd_button(x, y, 1);  
   }	 
	 TPD_DOWN_DEBUG_TRACK(x,y);
 }
 
static  int tpd_up(int x, int y,int *count) {
	 //if(*count>0) 
	 if(y>0){//by zhu
		 input_report_abs(tpd->dev, ABS_PRESSURE, 0);
		 input_report_key(tpd->dev, BTN_TOUCH, 0);
		 input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
		 input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
		 input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
		 //printk("U[%4d %4d %4d] ", x, y, 0);
		 input_mt_sync(tpd->dev);
		 TPD_UP_DEBUG_TRACK(x,y);
	//	 (*count)--;
	}
//remove at 20150327 by zhu for button
 //    if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
//remove at 20150327 by zhu for button end
     {   
        tpd_button(x, y, 0); 
     }   		 
		 return 1;
	 //} 
          return 0;
 }

 static int tpd_touchinfo(struct touch_info *cinfo, struct touch_info *pinfo)
 {
 #ifdef TPD_PROXIMITY
	int err;
	hwm_sensor_data sensor_data;
#endif

	int i = 0;
	char data[30] = {0};
	u16 high_byte,low_byte;

	p_point_num = point_num;

	i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 8, &(data[0]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x08, 8, &(data[8]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x10, 8, &(data[16]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0xa6, 1, &(data[24]));
	TPD_DEBUG("FW version=%x]\n",data[24]);
	
	TPD_DEBUG("received raw data from touch panel as following:\n");
	TPD_DEBUG("[data[0]=%x,data[1]= %x ,data[2]=%x ,data[3]=%x ,data[4]=%x ,data[5]=%x]\n",data[0],data[1],data[2],data[3],data[4],data[5]);
	TPD_DEBUG("[data[9]=%x,data[10]= %x ,data[11]=%x ,data[12]=%x]\n",data[9],data[10],data[11],data[12]);
	TPD_DEBUG("[data[15]=%x,data[16]= %x ,data[17]=%x ,data[18]=%x]\n",data[15],data[16],data[17],data[18]);

#ifdef TPD_PROXIMITY	/* added by huangxi 20130607 */
	if (tpd_proximity_flag == 1)
	{
		TPD_PROXIMITY_DEBUG("data[1]'s value is 0x%02X\n", data[1]);
		if (data[1] == 0xC0)	//close to
		{
			tpd_proximity_detect = 0;
		}
		else if (data[1] == 0xE0)	//leave
		{
			tpd_proximity_detect = 1;
		}
		else
		{
			TPD_PROXIMITY_DMESG("ps read data error. data[1]=0x%02X \n", data[1]);
		}
		//get raw data
		TPD_PROXIMITY_DEBUG(" ps change\n");
		//map and store data to hwm_sensor_data
	#if 0
		sensor_data.values[0] = tpd_get_ps_value();
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
		//let up layer to know
		if((err = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
		{
			TPD_PROXIMITY_DMESG("call hwmsen_get_interrupt_data fail = %d\n", err);
		}
	#endif
	}
/*end of added*/   
#endif	


	
	/* Device Mode[2:0] == 0 :Normal operating Mode*/
	if(data[0] & 0x70 != 0) return false; 

	/*get the number of the touch points*/
	point_num= data[2] & 0x0f;
	
	TPD_DEBUG("point_num =%d\n",point_num);
	
//	if(point_num == 0) return false;

	   TPD_DEBUG("Procss raw data...\n");

		
		for(i = 0; i < point_num; i++)
		{
			cinfo->p[i] = data[3+6*i] >> 6; //event flag 

	       /*get the X coordinate, 2 bytes*/
			high_byte = data[3+6*i];
			high_byte <<= 8;
			high_byte &= 0x0f00;
			low_byte = data[3+6*i + 1];
			cinfo->x[i] = high_byte |low_byte;

				//cinfo->x[i] =  cinfo->x[i] * 480 >> 11; //calibra
		
			/*get the Y coordinate, 2 bytes*/
			
			high_byte = data[3+6*i+2];
			high_byte <<= 8;
			high_byte &= 0x0f00;
			low_byte = data[3+6*i+3];
			cinfo->y[i] = high_byte |low_byte;

			  //cinfo->y[i]=  cinfo->y[i] * 800 >> 11;
		
			cinfo->count++;

			if (cinfo->y[i] <= TPD_RES_Y) 	//jeff add 20140827
			{	
				cinfo->x[i] = TPD_WARP_X(cinfo->x[i]);
				cinfo->y[i] = TPD_WARP_Y(cinfo->y[i]);
			}
			
		}
		TPD_DEBUG(" cinfo->x[0] = %d, cinfo->y[0] = %d, cinfo->p[0] = %d\n", cinfo->x[0], cinfo->y[0], cinfo->p[0]);	
		TPD_DEBUG(" cinfo->x[1] = %d, cinfo->y[1] = %d, cinfo->p[1] = %d\n", cinfo->x[1], cinfo->y[1], cinfo->p[1]);		
		TPD_DEBUG(" cinfo->x[2]= %d, cinfo->y[2]= %d, cinfo->p[2] = %d\n", cinfo->x[2], cinfo->y[2], cinfo->p[2]);	
		  
	 return true;

 };

static u8 reset_tpd_ps = 5;

 static int touch_event_handler(void *unused)
 {
  
    struct touch_info cinfo, pinfo;
	#ifdef TP_PROXIMITY_SENSOR
	int err;
	hwm_sensor_data sensor_data;
	static char rdata = 0x00;
	#endif

#ifdef GT9XX_TP_PS
	s32 err = 0;
	int ret;
	hwm_sensor_data sensor_data;
	u8 proximity_status;
	static char rdata = 0x00;
#endif

	 struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
	 sched_setscheduler(current, SCHED_RR, &param);
 
	 do
	 {
	  mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM); 

	        printk("FT5206 touch_event_handler \r\n");
		 set_current_state(TASK_INTERRUPTIBLE); 
		  wait_event_interruptible(waiter,tpd_flag!=0);
						 
			 tpd_flag = 0;
			 
		 set_current_state(TASK_RUNNING);
		 

#ifdef TP_PROXIMITY_SENSOR

//						if (tpd_proximity_flag == 1) {printk("[kktest] ps enable \r\n");}
						if (ft5206_proximity_stop_by_powerkey == 1)
						{
//							printk("[kktest]PROXIMITY STATUS:0x%02X\n", tpd_proximity_detect);
							sensor_data.values[0] = 1;
							sensor_data.value_divide = 1;
							sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
							hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data);
							ft5206_proximity_stop_by_powerkey = 2;		
						}
						else if (ft5206_proximity_stop_by_powerkey == 2)
						{
							tpd_enable_ps(0);
							ft5206_proximity_stop_by_powerkey = 3;
//							printk("[kktest] tpd_enable_ps : 0 \n");
						}
						else if (ft5206_proximity_stop_by_powerkey == 3)
						{
						}						
						else
						{
						  if (tpd_proximity_flag == 1)
						  {
						         i2c_smbus_read_i2c_block_data(i2c_client, 0x01, 1, &rdata);

//							  printk("[kktest] ft5206 0x01=%x  i2c_client = %d \r\n",rdata,i2c_client);
							  
							  if((rdata == 0xe0) ||(rdata == 0xc0))
							  {
								  if(rdata == 0xe0)// leave
								  {
									  tpd_proximity_detect = 1;
								  }
								  else  if(rdata == 0xc0) // close to
								  {
									  tpd_proximity_detect = 0;
								  }
								  //get raw data
								  TPD_PROXIMITY_DEBUG(" ps change\n");
								  //map and store data to hwm_sensor_data
								  sensor_data.values[0] = tpd_get_ps_value();
								  sensor_data.value_divide = 1;
								  sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
								  //let up layer to know
								  if((err = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
								  {
									  //TPD_PROXIMITY_DMESG("call hwmsen_get_interrupt_data fail = %d\n", err);
								  }
								  if (rdata == 0xe0)
								  	{
								  			 static char wdata = 0x01;
											 static char rdata = 0x00;
											  i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &(rdata)); 
											  msleep(40);
											  i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &wdata);
											  msleep(40);

//											  printk("[kktest] tp reset for rdata == 0xe0 start reset tpd_ps \r\n");
	 
								  	}
								  //return  false;
							  }		
							  else if (rdata == 0x00)
							  	{
								  			 static char wdata = 0x01;
											 static char rdata = 0x00;
											  i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &(rdata)); 
											  msleep(40);
											  i2c_smbus_write_i2c_block_data(i2c_client, 0xB0, 1, &wdata);
											  msleep(40);

//											  printk("[kktest] tp reset for rdata == 0x0 start reset tpd_ps \r\n");
							  	}
						  }
						}
#endif


#ifdef GT9XX_TP_PS
	if (tpd_proximity_flag == 1)
	{
	//	proximity_status = point_data[GTP_ADDR_LENGTH];
	//	GTP_DEBUG("REG INDEX[0x814E]:0x%02X\n", proximity_status);

	//	if (proximity_status & 0x60)                //proximity or large touch detect,enable hwm_sensor.
	//	{
	//		tpd_proximity_detect = 0;
			//sensor_data.values[0] = 0;
	//	}
	//	else
	//	{
	//		tpd_proximity_detect = 1;
			//sensor_data.values[0] = 1;
	//	}
		i2c_smbus_read_i2c_block_data(i2c_client, 0x01, 1, &rdata);
		if(rdata == 0xe0)// leave
		{
			tpd_proximity_detect = 1;
		}
		else  if(rdata == 0xc0) // close to
		{
			tpd_proximity_detect = 0;
		}

		//get raw data
		printk("gt9xx jeff  ps change\n");
		printk("gt9xx jeff PROXIMITY STATUS:0x%02X\n", tpd_proximity_detect);
		//map and store data to hwm_sensor_data
		sensor_data.values[0] = tpd_get_ps_value();
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
		//report to the up-layer
		ret = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data);

		if (ret)
		{
			printk("gt9xx jeff Call hwmsen_get_interrupt_data fail = %d\n", err);
		}
	}

#endif



		  if (tpd_touchinfo(&cinfo, &pinfo)) {
		  TPD_DEBUG("point_num = %d\n",point_num);
		  
		if(point_num >0){
			tpd_down(tpd_lcd_width-cinfo.x[0], tpd_lcd_height - cinfo.y[0], 1);
		if(point_num>1){
			tpd_down(tpd_lcd_width-cinfo.x[1], tpd_lcd_height - cinfo.y[1], 1);
		if(point_num >2)
			tpd_down(tpd_lcd_width-cinfo.x[2], tpd_lcd_height - cinfo.y[2], 1);
				}
		input_sync(tpd->dev);
		TPD_DEBUG("press --->\n");

		} else  {
		tpd_up(tpd_lcd_width-cinfo.x[0], tpd_lcd_height - cinfo.y[0]	, 0);
		TPD_DEBUG("release --->\n"); 
		input_mt_sync(tpd->dev);
		input_sync(tpd->dev);
		} 
        }

 }while(!kthread_should_stop());
 
	 return 0;
 }
 
 static int tpd_detect (struct i2c_client *client, int kind, struct i2c_board_info *info) 
 {
	 strcpy(info->type, TPD_DEVICE);	
	  return 0;
 }
 
 static void tpd_eint_interrupt_handler(void)
 {
	// TPD_DEBUG("TPD interrupt has been triggered\n");
//	printk("[kktest] --- %s ---\n", __func__);
	//printk("FT5206 TPD interrupt has been triggered\n");
	 tpd_flag = 1;
	 wake_up_interruptible(&waiter);
	 
 }
 #ifdef TPD_AUTO_UPGRADE_SUPPORT
 extern int tpd_auto_upgrade(struct i2c_client *client);
 #endif
 static int tpd_probe(struct i2c_client *client, const struct i2c_device_id *id)
 {	
 #ifdef TPD_PROXIMITY
	int err;
	struct hwmsen_object obj_ps;
#endif

	int retval = TPD_OK;
	char data;
#ifdef TP_PROXIMITY_SENSOR
	int err=0;
		struct hwmsen_object obj_ps;
#endif

#ifdef GT9XX_TP_PS
	int err;
	struct hwmsen_object obj_ps;
#endif

	i2c_client = client;
	printk("FT5206 tpd_probe \r'\n");
      hwPowerOn(PMIC_APP_CAP_TOUCH_VDD, VOL_2800, "TP");
#ifdef MT6575
    //power on, need confirm with SA
    hwPowerOn(PMIC_APP_CAP_TOUCH_VDD, VOL_2800, "TP");
    hwPowerOn(PMIC_APP_CAP_TOUCH_VDD, VOL_1800, "TP");      
#endif	
	#ifdef TPD_CLOSE_POWER_IN_SLEEP	 
	hwPowerDown(TPD_POWER_SOURCE,"TP");
	hwPowerOn(TPD_POWER_SOURCE,VOL_3300,"TP");
	msleep(100);
	#else
	#ifdef MT6573
	mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
  mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
	msleep(100);
	#endif
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	#endif

	// reset
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
	msleep(1);

	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(50);
      // set deep sleep off
      mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
      mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
      mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);  
      msleep(10);  


	mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
 
	  mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
	  mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
	  mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1); 
	  
	  //mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_POLARITY_HIGH, tpd_eint_interrupt_handler, 1); 
	  
	  mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);

	msleep(100);
	
#ifdef TPD_PROXIMITY
//	if(!(obj2 = kzalloc(sizeof(*obj2), GFP_KERNEL)))
//	{
//		err = -ENOMEM;
	//	goto exit;
//	}
//	memset(obj2, 0, sizeof(struct wxt_proximity_priv));

//	obj_ps.polling = 0;//interrupt mode
//	obj_ps.self = obj2;	//huangxi add 20130608
	obj_ps.polling = 1;//polling mode	//huangxi modify 20130608
	obj_ps.sensor_operate = tpd_ps_operate;
	if((err = hwmsen_attach(ID_PROXIMITY, &obj_ps)))
	{
		TPD_PROXIMITY_DMESG("attach fail = %d\n", err);
//		goto exit_create_attr_failed;
	}		
#endif

	
 #if 0
	if((i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 1, &data))< 0)
	   {
		   TPD_DMESG("FT5206 I2C transfer error, line: %d\n", __LINE__);
		   return -1; 
	   }
#endif
	tpd_load_status = 1;
#ifdef TPD_AUTO_UPGRADE_SUPPORT
	//tpd_auto_upgrade(i2c_client);
#endif

	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	 if (IS_ERR(thread))
		 { 
		  retval = PTR_ERR(thread);
		  TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", retval);
		}

	TPD_DMESG("Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");
#ifdef TP_PROXIMITY_SENSOR
		obj_ps.polling = 0;//interrupt mode
		obj_ps.sensor_operate = tpd_ps_operate;
		if((err = hwmsen_attach(ID_PROXIMITY, &obj_ps)))
		{
			printk("zhuoshineng gt868 proximity attach fail = %d\n", err);
			//		goto exit_create_attr_failed;
		}
#endif

#ifdef GT9XX_TP_PS
    //obj_ps.self = cm3623_obj;
    obj_ps.polling = 0;         //0--interrupt mode;1--polling mode;
    obj_ps.sensor_operate = tpd_ps_operate;

    if ((err = hwmsen_attach(ID_PROXIMITY, &obj_ps)))
    {
        printk("gt9xx jeff hwmsen attach fail, return:%d.", err);
    }

#endif

#ifdef FSYSINFO
	init_sys_device_name();
#endif

   return 0;
   
 }

 static int  tpd_remove(struct i2c_client *client)
 
 {

	 TPD_DEBUG("TPD removed\n");
 
   return 0;
 }
 
 
 static int tpd_local_init(void)
 {


  TPD_DMESG("Focaltech FT5206 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);
 
 
   if(i2c_add_driver(&tpd_i2c_driver)!=0)
   	{
  		TPD_DMESG("unable to add i2c driver.\n");
      	return -1;
    }
#ifdef TPD_HAVE_BUTTON     
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif   
  
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))    
    TPD_DO_WARP = 1;
    memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
    memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif 

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
    memcpy(tpd_calmat, tpd_def_calmat_local, 8*4);
    memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);	
#endif  
		TPD_DMESG("end %s, %d\n", __FUNCTION__, __LINE__);  
		tpd_type_cap = 1;
    return 0; 
 }

 static int tpd_resume(struct i2c_client *client)
 {
  int retval = TPD_OK;
#ifdef TPD_PROXIMITY
	if ((tpd_proximity_flag == 1) || (tpd_proximity_detect == 0))
	{
		TPD_PROXIMITY_DMESG("TPD wake up, but don't anything. flag=%d, detect=%d.\n", tpd_proximity_flag, tpd_proximity_detect);
		return 0;
	}
#endif

#ifdef GT9XX_TP_PS
//    if (tpd_proximity_flag == 1)
//    {
//    	printk("jeff TPD wake up, but don't anything. flag=%d, detect=%d.\n", tpd_proximity_flag, tpd_proximity_detect);
//        return 0;
//    }
    if (tpd_deep_sleep == 0)	//the system didn't really go into deep sleep. So, return directly.
        return 0;
#endif

   printk("[jeff test] --- %s --- wake up! tpd_ps_flag=%d, tpd_ps_detect=%d \n", __func__, tpd_proximity_flag, tpd_proximity_detect);	//jeff modify 20140807
   TPD_DEBUG("TPD wake up\n");
#ifdef TPD_CLOSE_POWER_IN_SLEEP	
	hwPowerOn(TPD_POWER_SOURCE,VOL_3300,"TP"); 
#else
#ifdef MT6573
	mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
#endif	
	msleep(100);
#ifdef TP_PROXIMITY_SENSOR
if(CTP_NORMAL_FLAG ==1)
{
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);  
	msleep(1);  
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);

	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);  	

	CTP_NORMAL_FLAG = 0;

}

#else
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);  
	msleep(1);  
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);  

#endif
   
#endif	
	
	 return retval;
 }
 
 static int tpd_suspend(struct i2c_client *client, pm_message_t message)
 {
	 int retval = TPD_OK;
	 static char data = 0x3;
 #ifdef TPD_PROXIMITY
	if ((tpd_proximity_flag == 1) || (tpd_proximity_detect == 0))
	{
		TPD_PROXIMITY_DMESG("TPD don't enter sleep. flag=%d, detect=%d.\n", tpd_proximity_flag, tpd_proximity_detect);
		return 0;
	}
#endif
//printk("[kktest] --- %s ---\n", __func__);

#ifdef GT9XX_TP_PS

    if (tpd_proximity_flag == 1)
    {
    	printk("jeff TPD don't enter sleep. flag=%d, detect=%d.\n", tpd_proximity_flag, tpd_proximity_detect);
    	tpd_deep_sleep = 0;
        return 0;
    }
    tpd_deep_sleep = 1;
#endif

   printk("[jeff test] -%s- enter sleep! tpd_ps_flag=%d, tpd_ps_detect=%d \n", __func__, tpd_proximity_flag, tpd_proximity_detect);	//jeff modify 20140807
	 
#ifdef TPD_CLOSE_POWER_IN_SLEEP	
	hwPowerDown(TPD_POWER_SOURCE,"TP");
#else
#ifdef TP_PROXIMITY_SENSOR
if(tpd_proximity_flag == 0)
{
	mt_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);

	
	i2c_smbus_write_i2c_block_data(i2c_client, 0xA5, 1, &data);  //TP enter sleep mode
	CTP_NORMAL_FLAG =1;
}
#else
	 TPD_DEBUG("TPD enter sleep\n");
	 mt_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);

	i2c_smbus_write_i2c_block_data(i2c_client, 0xA5, 1, &data);  //TP enter sleep mode
#endif
#ifdef MT6573
mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ZERO);
#endif

#endif
	 return retval;
 } 


 static struct tpd_driver_t tpd_device_driver = {
		 .tpd_device_name = "FT5206",
		 .tpd_local_init = tpd_local_init,
		 .suspend = tpd_suspend,
		 .resume = tpd_resume,
#ifdef TPD_HAVE_BUTTON
		 .tpd_have_button = 1,
#else
		 .tpd_have_button = 0,
#endif		
 };
 /* called when loaded into kernel */
 static int __init tpd_driver_init(void) {

	 printk("MediaTek FT5206 touch panel driver init\n");
	   i2c_register_board_info(1, &i2c_tpd, 1);
		 if(tpd_driver_add(&tpd_device_driver) < 0)
			 TPD_DMESG("add FT5206 driver failed\n");
	 return 0;
 }
 
 /* should never be called */
 static void __exit tpd_driver_exit(void) {
 
	 TPD_DMESG("MediaTek FT5206 touch panel driver exit\n");
	 //input_unregister_device(tpd->dev);
	 tpd_driver_remove(&tpd_device_driver);
 }
 

 
 module_init(tpd_driver_init);
 module_exit(tpd_driver_exit);


