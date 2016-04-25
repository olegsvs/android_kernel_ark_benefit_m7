/*
 FILENAME.
      fps_trigger.c - FringerPrint Sensor signal trigger routine

      $PATH:

 FUNCTIONAL DESCRIPTION.
      FingerPrint Sensor trigger routine

 MODIFICATION HISTORY.

 NOTICE.
      Copyright (C) 2000-2014 EgisTec All Rights Reserved.
*/

#include "fps_trigger.h"
#include <linux/delay.h>
/*
	Interrupt description
*/

#define EH570_INT_DETECTION_PERIOD	 1
#define EH570_DETECTION_THRESHOLD	4
struct interrupt_desc {
	int gpio;
	int number;
	char *name;
	volatile int int_count;
	struct timer_list timer;
};

/*
	FPS interrupt table
*/

/*
static struct interrupt_desc fps_ints[] = {
	{EXYNOS4_GPX1(4) , 0, "BUT0" , 0}
};
*/

static struct interrupt_desc fps_ints[] = {
//	{1234 , 0, "BUT0" , 0} /* TINY4412CON15 XEINT12 pin */ //jeff 0408
	{0 , 0, "BUT0" , 0}//{16 , 0, "BUT0" , 0}
};

static volatile char interrupt_values[] = {
	'1', '1', '1', '1', '1', '1', '1', '1'
};

static DECLARE_WAIT_QUEUE_HEAD(interrupt_waitq);

static volatile int ev_press = 1;  /*  it should be init to 0  */

/*
   FUNCTION NAME.
      interrupt_timer_routine

   FUNCTIONAL DESCRIPTION.
      basic interrupt timer inital routine


 ENTRY PARAMETERS.
      gpio             - gpio address

 EXIT PARAMETERS.
      Function Return
*/

static void interrupt_timer_routine(
	unsigned long _data
)
{
	struct interrupt_desc *bdata = (struct interrupt_desc *)_data;
	pr_err("FPS interrupt count = %d" , bdata->int_count);
	if (bdata->int_count >= EH570_DETECTION_THRESHOLD) {
		ev_press = 1;
		pr_err("FPS triggered !!!!!!!\n");
	}
	bdata->int_count = 0;
	wake_up_interruptible(&interrupt_waitq);
}

/*
 FUNCTION NAME.
      fingerprint_interrupt

 FUNCTIONAL DESCRIPTION.
      finger print interrupt callback routine


 ENTRY PARAMETERS.
      irq
      dev_id

 EXIT PARAMETERS.
      Function Return
*/

static irqreturn_t fingerprint_interrupt(
	int irq,
	void *dev_id
)
{
	/*
	struct interrupt_desc *bdata = (struct interrupt_desc *)dev_id;
	mod_timer(&bdata->timer, jiffies + msecs_to_jiffies(5000));
	printk(KERN_ERR "enter to modify the finger interrupt routine\n");
	return IRQ_HANDLED;
	*/
	struct interrupt_desc *bdata = (struct interrupt_desc *)dev_id;
	if (!bdata->int_count)
		mod_timer(&bdata->timer,
			jiffies + msecs_to_jiffies(EH570_INT_DETECTION_PERIOD));
	bdata->int_count++;
	return IRQ_HANDLED;
}

/*
 FUNCTION NAME.
      Interrupt_Init

 FUNCTIONAL DESCRIPTION.
      button initial routine


 ENTRY PARAMETERS.
      gpio             - gpio address

 EXIT PARAMETERS.
      Function Return int
*/

int Interrupt_Init(void)
{
	int i;
	int irq;
	int err = 0;

	for (i = 0; i < ARRAY_SIZE(fps_ints); i++) {
		if (!fps_ints[i].gpio)
			continue;
/*
set timer function to handle the interrupt
*/
	setup_timer(&fps_ints[i].timer, interrupt_timer_routine,
				(unsigned long)&fps_ints[i]);

/*
set the IRQ function and GPIO. then setting the interrupt trigger type
*/
	irq = gpio_to_irq(fps_ints[i].gpio);
	err = request_irq(irq, fingerprint_interrupt, IRQ_TYPE_EDGE_RISING,
					fps_ints[i].name, (void *)&fps_ints[i]);
		if (err)
			break;
	}

	if (err) {
		i--;
		for (; i >= 0; i--) {
			if (!fps_ints[i].gpio)
				continue;
			irq = gpio_to_irq(fps_ints[i].gpio);
			disable_irq(irq);
			free_irq(irq, (void *)&fps_ints[i]);
			del_timer_sync(&fps_ints[i].timer);
		}
		return -EBUSY;
	}
	ev_press = 0;
	return 0;
}

/*
 FUNCTION NAME.
      Interrupt_Free

 FUNCTIONAL DESCRIPTION.
      free all interrupt resource


 ENTRY PARAMETERS.
      gpio             - gpio address

 EXIT PARAMETERS.
      Function Return int
*/

int Interrupt_Free(void)
{
	int irq, i;

	for (i = 0; i < ARRAY_SIZE(fps_ints); i++) {
		if (!fps_ints[i].gpio)
			continue;
		irq = gpio_to_irq(fps_ints[i].gpio);
		free_irq(irq, (void *)&fps_ints[i]);

		del_timer_sync(&fps_ints[i].timer);
	}

	return 0;
}

/*
 FUNCTION NAME.
      fps_interrupt_read

 FUNCTIONAL DESCRIPTION.
      FPS interrupt read status


 ENTRY PARAMETERS.


 EXIT PARAMETERS.
      Function Return int
*/

int fps_interrupt_read(
	struct file *filp,
	char __user *buff,
	size_t count,
	loff_t *offp
)
{
	return 0;
}

/*
 FUNCTION NAME.
      fps_interrupt_read

 FUNCTIONAL DESCRIPTION.
      FPS interrupt read status


 ENTRY PARAMETERS.

      wait      poll table structure

 EXIT PARAMETERS.
      Function Return int
*/

unsigned int fps_interrupt_poll(
	struct file *file,
	struct poll_table_struct *wait
)
{
	unsigned int mask = 0;
	fps_ints[0].int_count = 0;
	poll_wait(file, &interrupt_waitq, wait);
	if (ev_press) {
		mask |= POLLIN | POLLRDNORM;
		ev_press = 0;
	}
	return mask;
}

void fps_interrupt_abort(void)
{
	ev_press = 0;
	wake_up_interruptible(&interrupt_waitq);
}

