/*
 * Simple synchronous userspace interface to SPI devices
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 * Copyright (C) 2007 David Brownell (simplification, cleanup)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "eh570_data_transfer.h"
#include "eh570.h"
#include "eh570_define_jadeOpc.h"
#include "fps_trigger.h"

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
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/mt_spi.h>

/* reset pin for CK MKT 92XX */
//#define GPIO_FPS_RESET_PIN	GPIO55
#define GPIO_FPS_RESET_PIN	GPIO1//GPIO17

#ifdef EH570_SPI_DEBUG
#define DEBUG_PRINT(fmt, args...)	printk(fmt, ## args)
#else
/* Don't do anything in release builds */
#define DEBUG_PRINT(fmt, args...)
#endif

static DECLARE_BITMAP(minors, N_SPI_MINORS);
static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);
static unsigned bufsiz = 4096;
module_param(bufsiz, uint, S_IRUGO);
MODULE_PARM_DESC(bufsiz, "data bytes in biggest supported SPI message");

static struct mt_chip_conf spi_conf = {
	.setuptime = 1,
	.holdtime = 1,
	.high_time = 4,
	.low_time = 4,
//	.high_time = 10,//6,//4,  //jeff 20150408
//	.low_time = 10,//6,//4,

	.cs_idletime = 2,
	.ulthgh_thrsh = 0,
	.cpol = 1,
	.cpha = 1,
	.rx_mlsb = SPI_MSB,
	.tx_mlsb = SPI_MSB,
	.tx_endian = 0,
	.rx_endian = 0,
	.com_mod = DMA_TRANSFER,
	.pause = 0,
	.finish_intr = 1,
	.deassert = 0,
	.ulthigh = 0,
	.tckdly = 0,
};

/*-------------------------------------------------------------------------*/

static void eh570_reset(void)
{
	mt_set_gpio_mode(GPIO_FPS_RESET_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_FPS_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_FPS_RESET_PIN, GPIO_OUT_ZERO);
	msleep(20);
	mt_set_gpio_out(GPIO_FPS_RESET_PIN, GPIO_OUT_ONE);
}

static ssize_t eh570_read(struct file *filp,
						char __user *buf,
						size_t count,
						loff_t *f_pos)
{
	/*Implement by vendor if needed*/
	return 0;
}

static ssize_t eh570_write(struct file *filp,
						const char __user *buf,
						size_t count,
						loff_t *f_pos)
{
/*Implement by vendor if needed*/
	return 0;
}

static long eh570_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int			err = 0;
	int			retval = 0;
	struct eh570_data	*eh570;
	struct spi_device	*spi;
	u32			tmp;
	struct egis_ioc_transfer	*ioc = NULL;

	/* Check type and command number */
	if (_IOC_TYPE(cmd) != EGIS_IOC_MAGIC) {
		pr_err("%s _IOC_TYPE(cmd) != EGIS_IOC_MAGIC", __func__);
		return -ENOTTY;
	}

	/* Check access direction once here; don't repeat below.
	 * IOC_DIR is from the user perspective, while access_ok is
	 * from the kernel perspective; so they look reversed.
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE,
						(void __user *)arg,
						_IOC_SIZE(cmd));
	if (err == 0 && _IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ,
						(void __user *)arg,
						_IOC_SIZE(cmd));
	if (err) {
		pr_err("%s err", __func__);
		return -EFAULT;
	}

	/* guard against device removal before, or while,
	 * we issue this ioctl.
	 */
	eh570 = filp->private_data;
	spin_lock_irq(&eh570->spi_lock);
	spi = spi_dev_get(eh570->spi);
	spin_unlock_irq(&eh570->spi_lock);

	if (spi == NULL) {
		pr_err("%s spi == NULL", __func__);
		return -ESHUTDOWN;
	}

	mutex_lock(&eh570->buf_lock);

	/* segmented and/or full-duplex I/O request */
	if (_IOC_NR(cmd) != _IOC_NR(EGIS_IOC_MESSAGE(0))
					|| _IOC_DIR(cmd) != _IOC_WRITE) {
		retval = -ENOTTY;
		goto out;
	}

	tmp = _IOC_SIZE(cmd);
	if ((tmp == 0) || (tmp % sizeof(struct egis_ioc_transfer)) != 0) {
		retval = -EINVAL;
		goto out;
	}

	/* copy into scratch area */
	ioc = kmalloc(tmp, GFP_KERNEL);
	if (ioc == NULL) {
		retval = -ENOMEM;
		goto out;
	}
	if (__copy_from_user(ioc, (void __user *)arg, tmp)) {
		retval = -EFAULT;
		goto out;
	}

	/*
	 * Read register
	 * tx_buf include register address will be read
	 */
	if (ioc->opcode == JADE_REGISTER_READ) {
		u8 *address = ioc->tx_buf;
		u8 *result = ioc->rx_buf;
		DEBUG_PRINT("eh570 JADE_REGISTER_READ");

		retval = eh570_io_read_register(eh570, address, result);
		if (retval < 0)	{
			pr_err("%s JADE_REGISTER_READ error retval = %d"
			, __func__, retval);
			goto out;
		}
	}

	/*
	 * Write data to register
	 * tx_buf includes address and value will be wrote
	 */
	if (ioc->opcode == JADE_REGISTER_WRITE) {
		u8 *buf = ioc->tx_buf;
		retval = eh570_io_write_register(eh570, buf);
		if (retval < 0) {
			pr_err("%s JADE_REGISTER_WRITE error retval = %d"
			, __func__, retval);
			goto out;
		}
	}

	/*
	 * Get one frame data from sensor
	 */
	if (ioc->opcode == JADE_GET_ONE_IMG) {
		u8 *buf = ioc->tx_buf;
		u8 *image_buf = ioc->rx_buf;
		DEBUG_PRINT("eh570 JADE_GET_ONE_IMG");
		retval = eh570_io_get_one_image(eh570, buf, image_buf);
		if (retval < 0) {
			pr_err("%s JADE_GET_ONE_IMG error retval = %d"
			, __func__, retval);
			goto out;
		}
	}

	if (ioc->opcode == JADE_SENSOR_RESET)
		eh570_reset();

	/*
	 * Trigger inital routine
	 */
	if (ioc->opcode == INT_TRIGGER_INIT) {
		pr_err(">>> eh570 Trigger function init\n");
		retval = Interrupt_Init();
		pr_err("trigger init = %d\n", retval);
	}

	/*
	 * trigger
	 */
	if (ioc->opcode == INT_TRIGGER_CLOSE) {
		pr_err("<<< eh570 Trigger function close\n");
		retval = Interrupt_Free();
		pr_err("trigger close = %d\n", retval);
	}

/*
 * read interrupt status
 */
	if (ioc->opcode == INT_TRIGGER_ABORT)
		fps_interrupt_abort();

out:
	if (ioc != NULL)
		kfree(ioc);

	mutex_unlock(&eh570->buf_lock);
	spi_dev_put(spi);
	if (retval < 0)
		pr_err("%s retval = %d", __func__, retval);
	return retval;
}

#ifdef CONFIG_COMPAT
static long eh570_compat_ioctl(struct file *filp,
	unsigned int cmd,
	unsigned long arg)
{
	return eh570_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}
#else
#define eh570_compat_ioctl NULL
#endif /* CONFIG_COMPAT */

static int eh570_open(struct inode *inode, struct file *filp)
{
	struct eh570_data	*eh570;
	int			status = -ENXIO;

	DEBUG_PRINT("%s", __func__);
	mutex_lock(&device_list_lock);

	list_for_each_entry(eh570, &device_list, device_entry)
	{
		if (eh570->devt == inode->i_rdev) {
			status = 0;
			break;
		}
	}
	if (status == 0) {
		if (eh570->buffer == NULL) {
			eh570->buffer = kmalloc(bufsiz, GFP_KERNEL);
			if (eh570->buffer == NULL) {
				dev_dbg(&eh570->spi->dev, "open/ENOMEM\n");
				status = -ENOMEM;
			}
		}
		if (status == 0) {
			eh570->users++;
			filp->private_data = eh570;
			nonseekable_open(inode, filp);
		}
	} else
		pr_debug("eh570: nothing for minor %d\n", iminor(inode));

	mutex_unlock(&device_list_lock);
	return status;
}

static int eh570_release(struct inode *inode, struct file *filp)
{
	struct eh570_data	*eh570;

	DEBUG_PRINT("%s", __func__);
	mutex_lock(&device_list_lock);
	eh570 = filp->private_data;
	filp->private_data = NULL;

	/* last close? */
	eh570->users--;
	if (eh570->users == 0) {
		int	dofree;

		kfree(eh570->buffer);
		eh570->buffer = NULL;

		/* ... after we unbound from the underlying device? */
		spin_lock_irq(&eh570->spi_lock);
		dofree = (eh570->spi == NULL);
		spin_unlock_irq(&eh570->spi_lock);

		if (dofree)
			kfree(eh570);
	}
	mutex_unlock(&device_list_lock);

	return 0;
}

static const struct file_operations eh570_fops = {
	.owner =	THIS_MODULE,
	.write =	eh570_write,
	.read =		eh570_read,
	.unlocked_ioctl = eh570_ioctl,
	.compat_ioctl = eh570_compat_ioctl,
	.open =		eh570_open,
	.release =	eh570_release,
	.llseek =	no_llseek,
	.poll	=	fps_interrupt_poll
};

/*-------------------------------------------------------------------------*/

static struct class *eh570_class;

/*-------------------------------------------------------------------------*/

static int eh570_probe(struct spi_device *spi)
{
	struct eh570_data	*eh570;
	int			status;
	unsigned long		minor;

	DEBUG_PRINT("%s", __func__);

	/* Allocate driver data */
	eh570 = kzalloc(sizeof(*eh570), GFP_KERNEL);
	if (eh570 == NULL)
		return -ENOMEM;
///
#if 0
    mt_set_gpio_mode(GPIO65,1);
    mt_set_gpio_dir(GPIO65,GPIO_DIR_OUT);
    mt_set_gpio_pull_enable(GPIO65,GPIO_PULL_ENABLE);

    mt_set_gpio_mode(GPIO66,1);
    mt_set_gpio_dir(GPIO66,GPIO_DIR_OUT);
    mt_set_gpio_pull_enable(GPIO66,GPIO_PULL_ENABLE);

    mt_set_gpio_mode(GPIO67,1);
    mt_set_gpio_dir(GPIO67,GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO67,GPIO_PULL_ENABLE);

    mt_set_gpio_mode(GPIO68,1);
    mt_set_gpio_dir(GPIO68,GPIO_DIR_OUT);
    mt_set_gpio_pull_enable(GPIO68,GPIO_PULL_ENABLE);

    mt_set_gpio_mode(GPIO65,1);
    mt_set_gpio_mode(GPIO66,1);
    mt_set_gpio_mode(GPIO67,1);
    mt_set_gpio_mode(GPIO68,1);
#endif
///
//jeff add 20141128 begin
	hwPowerOn(MT6328_POWER_LDO_VGP1,VOL_3300,"eh570"); 
//jeff add 20141128 end

	/* Initialize the driver data */
	eh570->spi = spi;
	spi->controller_data = (void *) &spi_conf;
	spi->max_speed_hz = 12 * 1000 * 1000;
	spi_setup(spi);
	spin_lock_init(&eh570->spi_lock);
	mutex_init(&eh570->buf_lock);

	INIT_LIST_HEAD(&eh570->device_entry);

	/* If we can allocate a minor number, hook up this device.
	 * Reusing minors is fine so long as udev or mdev is working.
	 */
	mutex_lock(&device_list_lock);
	minor = find_first_zero_bit(minors, N_SPI_MINORS);
	if (minor < N_SPI_MINORS) {
		struct device *dev;
		eh570->devt = MKDEV(EH570_MAJOR, minor);
		dev = device_create(eh570_class, &spi->dev, eh570->devt,
				    eh570, "esfp0");
		status = IS_ERR(dev) ? PTR_ERR(dev) : 0;
	} else{
		dev_dbg(&spi->dev, "no minor number available!\n");
		status = -ENODEV;
	}
	if (status == 0) {
		set_bit(minor, minors);
		list_add(&eh570->device_entry, &device_list);
	}
	mutex_unlock(&device_list_lock);

	if (gpio_request(GPIO_FPS_RESET_PIN, "fpsreset"))
		pr_err("request GPIO for fpsreset failed\n");

	eh570_reset();
	if (status == 0)
		spi_set_drvdata(spi, eh570);
	else
		kfree(eh570);

	return status;
}

static int eh570_remove(struct spi_device *spi)
{
	struct eh570_data	*eh570 = spi_get_drvdata(spi);
	DEBUG_PRINT("%s", __func__);

	/* make sure ops on existing fds can abort cleanly */
	spin_lock_irq(&eh570->spi_lock);
	eh570->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&eh570->spi_lock);

	/* prevent new opens */
	mutex_lock(&device_list_lock);
	list_del(&eh570->device_entry);
	device_destroy(eh570_class, eh570->devt);
	clear_bit(MINOR(eh570->devt), minors);
	if (eh570->users == 0)
		kfree(eh570);
	mutex_unlock(&device_list_lock);

	return 0;
}

static struct spi_driver eh570_spi_driver = {
	.driver = {
		.name =		"eh570",
		.owner =	THIS_MODULE,
	},
	.probe =	eh570_probe,
	.remove =	eh570_remove,

};

/*-------------------------------------------------------------------------*/

static int __init eh570_init(void)
{
	int status;
	DEBUG_PRINT("%s", __func__);

	/* Claim our 256 reserved device numbers.  Then register a class
	 * that will key udev/mdev to add/remove /dev nodes.  Last, register
	 * the driver which manages those device numbers.
	 */
	BUILD_BUG_ON(N_SPI_MINORS > 256);
	status = register_chrdev(EH570_MAJOR, "spi", &eh570_fops);
	if (status < 0)
		return status;
	eh570_class = class_create(THIS_MODULE, "eh570");
	if (IS_ERR(eh570_class)) {
		unregister_chrdev(EH570_MAJOR, eh570_spi_driver.driver.name);
		return PTR_ERR(eh570_class);
	}

	status = spi_register_driver(&eh570_spi_driver);
	if (status < 0) {
		class_destroy(eh570_class);
		unregister_chrdev(EH570_MAJOR, eh570_spi_driver.driver.name);
	}
	return status;
}
module_init(eh570_init);

static void __exit eh570_exit(void)
{
	DEBUG_PRINT("%s", __func__);
	gpio_free(GPIO_FPS_RESET_PIN);
	spi_unregister_driver(&eh570_spi_driver);
	class_destroy(eh570_class);
	unregister_chrdev(EH570_MAJOR, eh570_spi_driver.driver.name);
}
module_exit(eh570_exit);

MODULE_AUTHOR("Wang YuWei, <robert.wang@egistec.com>");
MODULE_DESCRIPTION("SPI Interface for EH570");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:eh570");
