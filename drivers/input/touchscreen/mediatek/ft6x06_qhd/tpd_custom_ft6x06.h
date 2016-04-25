#ifndef TOUCHPANEL_H__
#define TOUCHPANEL_H__
#include <linux/rtpm_prio.h>
//#include "tpd.h"
#include "cust_gpio_usage.h"
#include <pmic_drv.h>
#include <cust_eint.h>
/* Pre-defined definition */
#define TPD_TYPE_CAPACITIVE
#define TPD_TYPE_RESISTIVE
#define TPD_POWER_SOURCE         
#define TPD_I2C_NUMBER           1	//0
#define TPD_WAKEUP_TRIAL         60
#define TPD_WAKEUP_DELAY         100

#define TPD_VELOCITY_CUSTOM_X 12
#define TPD_VELOCITY_CUSTOM_Y 16


#define TPD_DELAY                (2*HZ/100)
#define TPD_RES_X                540		//huangxi modify 20130510
#define TPD_RES_Y                960
#define TPD_CALIBRATION_MATRIX  {962,0,0,0,1600,0,0,0};

//huangxi add 20130716
//#define TPD_X_RES (480)
//#define TPD_Y_RES (800)
#define TPD_WARP_Y(y) 	(TPD_RES_Y - y)
#define TPD_WARP_X(x) 	(TPD_RES_X - x)


//huangxi add 20130510
#define TPD_DEVICE	"ft6x06"


//#define TPD_HAVE_CALIBRATION
//add at 20150327 by zhu for tp buttom
#define TPD_HAVE_BUTTON
//add at 20150327 by zhu for tp buttom end
//#define TPD_HAVE_TREMBLE_ELIMINATION
//#define TPD_HAVE_BUTTON
#define TPD_BUTTON_HEIGH        (100)
#define TPD_KEY_COUNT           1 //3
//add at 20150327 by zhu for tp buttom
//#define TPD_KEYS                { KEY_MENU, KEY_BACK}
#define TPD_KEYS                { KEY_FINGER}
//add at 20150327 by zhu for tp buttom end
//  480 -60,   480- 380        854-1030
//add at 20150327 by zhu for tp buttom
//#define TPD_KEYS_DIM            {{TPD_RES_X - 60,TPD_RES_Y - 1030,160,TPD_BUTTON_HEIGH},{TPD_RES_X - 380,TPD_RES_Y - 1030,160,TPD_BUTTON_HEIGH}}   //S05  new saihua  4.68
#define TPD_KEYS_DIM            {{60,1035,TPD_BUTTON_WIDTH,TPD_BUTTON_HEIGH}}//{{480,-70,60,TPD_BUTTON_HEIGH}} 
//add at 20150327 by zhu for tp buttom end
//#define TPD_KEYS_DIM            {{60,900,160,TPD_BUTTON_HEIGH},{420,900,160,TPD_BUTTON_HEIGH}}   //S05  new saihua  4.63

#endif /* TOUCHPANEL_H__ */
