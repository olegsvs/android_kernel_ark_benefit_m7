/*
 * FILENAME.
 *      fps_trigger.h - FPS trigger routine
 *
 *      $PATH:
 *
 * FUNCTIONAL DESCRIPTION.
 *      1. FPS trigger control
 *
 * MODIFICATION HISTORY.
 *
 * NOTICE.
 *      Copyright (C) 2000-2014 EgisTec All Rights Reserved.
 */

#ifndef _FPS_TRIGGER_CONTROL_H
#define _FPS_TRIGGER_CONTROL_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include <linux/gpio.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/mt_spi.h>


/* interrupt init */
int Interrupt_Init(void);

/* interrupt free */
int Interrupt_Free(void);

/* interrupt read status */
int fps_interrupt_read(
	struct file *filp,
	char __user *buff,
	size_t count,
	loff_t *offp
);

void fps_interrupt_abort(void);

/* interrupt polling */
unsigned int fps_interrupt_poll(
	struct file *file,
	struct poll_table_struct *wait
);
#endif
