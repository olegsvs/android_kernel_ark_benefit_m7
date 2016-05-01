#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <mach/upmu_common.h>


#if defined(CONFIG_T93_PROJ)
 #if defined(CONFIG_T93E_HAIERT81_TS_PROJ)||defined(CONFIG_T93E_TS_PROJ)
   #define PS_HIGH 0X44c
  #define PS_LOW  0x3e8
 #elif 0//defined(CONFIG_T93E_TS_PROJ)
  #define PS_HIGH 0X3E8
  #define PS_LOW  0x320
 #elif defined(CONFIG_T93Q_LF_PROJ)
  #define PS_HIGH 0X320
  #define PS_LOW  0x2ee
 #elif defined(CONFIG_T93K_PROJ)
  #define PS_HIGH 0X3e8
  #define PS_LOW  0x384
 #else
  #define PS_HIGH 0X226
  #define PS_LOW  0x210
 #endif
#endif

#if defined(CONFIG_T89P_YX_PROJ)
 #define PS_HIGH 2500
 #define PS_LOW  2400
#endif

#ifndef PS_HIGH
 #define PS_HIGH 0X1200
 #define PS_LOW  0x1000
#endif
#include <cust_alsps.h>
//#include <mach/mt6575_pm_ldo.h>
static struct alsps_hw cust_alsps_hw = {
	.i2c_num    = 2,
	.polling_mode_ps = 0,
	.power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
	.power_vol  = VOL_DEFAULT,          /*LDO is not used*/
	.i2c_addr   = {0x92, 0x48, 0x78, 0x00},
	.als_level	= {5, 8, 16, 32, 60, 90, 140, 180, 400, 900, 4000, 7000, 8000, 10000, 20000},
	.als_value	= {0, 10, 20, 40, 90, 120, 160, 225, 280, 360, 640, 1280, 2600, 3800, 4700, 10240},
    .ps_threshold_high = PS_HIGH,
    .ps_threshold_low = PS_LOW,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

