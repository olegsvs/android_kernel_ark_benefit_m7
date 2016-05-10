
#ifndef _LINUX_POCKET_MOD_H
#define _LINUX_POCKET_MOD_H

extern int is_screen_on;
#ifdef CONFIG_MTK_STK3X1X_DRIVER
int stk3x1x_pocket_detection_check(void);
#endif
#ifdef CONFIG_MTK_EPL2182_NEW
int epl2182_pocket_detection_check(void);
#endif
int device_is_pocketed(void);

#endif //_LINUX_POCKET_MOD_H
