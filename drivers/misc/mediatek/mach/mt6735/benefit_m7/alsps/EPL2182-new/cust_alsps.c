#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <mach/upmu_common.h>
#if defined(CONFIG_T93_PROJ)
 #if defined(CONFIG_T93E_TS_PROJ)
  #define PS_HIGH 0X3E8
  #define PS_LOW  0x320
 #elif defined(CONFIG_T93T_5512_PROJ)
  #define PS_HIGH 0X44c
  #define PS_LOW  0x41a
 #elif defined(CONFIG_T93Q_LF_PROJ)
  #define PS_HIGH 0X352
  #define PS_LOW  0x320
 #elif defined(CONFIG_T93H_KLT_PROJ)
 #if defined(CONFIG_T93H_KLT_BOM22_PROJ)
  #define PS_HIGH 700
  #define PS_LOW  600
 #else
  #define PS_HIGH 0X44c
  #define PS_LOW  0x41a
 #endif
 #elif defined(CONFIG_T93BM_DG_PROJ)
  #define PS_HIGH 1000
  #define PS_LOW  900
 #else
  #define PS_HIGH 0X226
  #define PS_LOW  0x210
 #endif
#elif defined(CONFIG_T25_PROJ)
	#define PS_HIGH 850
	#define PS_LOW  700
#elif defined(CONFIG_T99C_JQ_PROJ)
	#define PS_HIGH 500
	#define PS_LOW  250

#elif defined(CONFIG_T99F_LF_PROJ)
 #define PS_HIGH 3000
 #define PS_LOW  2000
#elif defined(CONFIG_T89D_KLT_PROJ)
	#define PS_HIGH 1200
	#define PS_LOW  1000
#elif defined(CONFIG_T89J_JK_PROJ)
	#define PS_HIGH 1088
	#define PS_LOW  416
#else
#define PS_HIGH 0X1200
#define PS_LOW  0x1000
#endif

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 2,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    //.i2c_addr   = {0x0C, 0x48, 0x78, 0x00},

    /* MTK: modified to support AAL */
	.als_level	= {5, 8, 16, 32, 60, 90, 140, 180, 400, 900, 4000, 7000, 8000, 10000, 20000},
	.als_value	= {0, 10, 20, 40, 90, 120, 160, 225, 280, 360, 640, 1280, 2600, 3800, 4700, 10240},
    .ps_threshold_high = PS_HIGH,
    .ps_threshold_low = PS_LOW,
    .is_batch_supported_ps = false,
    .is_batch_supported_als = false,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

