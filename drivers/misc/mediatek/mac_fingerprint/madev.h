#ifndef MADEV_H
#define MADEV_H

#define MTK // MTK平台
#ifdef MTK

	#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/cdev.h>
#include <linux/types.h>

#include <mach/mt_pm_ldo.h>
#include <mach/mt_spi.h>
#include <cust_gpio_usage.h>
#include <mach/mt_gpio.h>
	#define FBUF  (1024*15)	// 帧大小
#else 	// 三星、高通平台
	#define W 		120		// 宽
	#define H 		120		// 高
	#define FBUF  	((W+1)*(H+1)+1) // 帧大小
#endif

#define MA_READ_FRAME_REG 	0x78

#define DRV_DEBUG		0x100	// 驱动信息
#define DRV_IRQEN		0x101	// 驱动使能
#define DRV_SPI_SPEED   0x102	// SPI速度
#define DRV_FBUF		0x103	// FBUF大小
#define DRV_AUTOTEST_FBUF		0x104

#ifndef msleep
#define msleep(n) 	\
do { 	\
long timeout = (n) * HZ / 1000; \
while(timeout > 0) \
{ 	\
timeout = schedule_timeout(timeout); \
} 	\
}while(0);
#endif

#endif /* SPIDEV_H */
