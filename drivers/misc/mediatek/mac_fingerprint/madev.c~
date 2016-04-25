#include <asm/memory.h>
#include <asm/uaccess.h>
#include <asm/dma.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/earlysuspend.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/time.h>
#include "madev.h"
#if 0
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/cdev.h>
#include <linux/types.h>

#include <mach/mt_pm_ldo.h>
#include <mach/mt_spi.h>
#include <cust_gpio_usage.h>
#include <mach/mt_gpio.h>
#endif





#define MODE_NAME  "AFS120"

#ifndef MTK
#define MASPI_MAJOR		172	/* assigned */
#else
unsigned int  MASPI_MAJOR = 0;	/* assigned */
#define MADEV_EINT_PIN     	(GPIO0| 0x80000000)
#define MADEV_EN_PIN         (GPIO1| 0x80000000) 
struct cdev sdev;
#endif
#define MASPI_MINOR		32	/* ... up to 256 */

struct maspi {
	u32 speed; // xfer??

	dev_t devt;
	spinlock_t spi_lock;
	struct spi_device *spi;
	struct list_head dev_entry;
	struct class *cls;

	struct spi_message msg;
	struct spi_transfer xfer;
	struct early_suspend	early_suspend;
};
static struct maspi *smas = NULL;
static u8 *sbuf = NULL;

static DECLARE_BITMAP(minors, MASPI_MINOR);
static LIST_HEAD(dev_list);
static DEFINE_MUTEX(dev_lock);

int mas_frame(struct spi_device *spi, char *buf);

/*--------------------------------- print -----------------------------------*/
static u8 sdeb = 1; // ???1?????0???

//#define DEBUG

void printd(char *fmt, ...) {
#ifdef DEBUG
	if (sdeb == 1) {
		va_list args;
		va_start(args, fmt);
		vprintk(fmt, args);
		va_end(args);
	}
#endif
}

void printw(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

void printn(const char *func, char *buf, int len) {
	int i;

	printd("%s: buf:\n", func);
	for (i = 0; i < len; i++) {
		printd("%.2x ", buf[i]);
		if (i > 0 && (i + 1) % 30 == 0) printd("\n");
	}
	printd("\n");
}


#ifdef MTK
void finger_en_h(void) {
	mt_set_gpio_out(MADEV_EN_PIN, 1);
}

void finger_en_l(void) {
	mt_set_gpio_out(MADEV_EN_PIN, 0);
}

static void mt_spi_set_mod(void) {
	//set shutdown mode ,set pin default staus as high 
	mt_set_gpio_mode(MADEV_EN_PIN,GPIO_MODE_00);  
	mt_set_gpio_dir(MADEV_EN_PIN,GPIO_DIR_OUT);
	mt_set_gpio_pull_select(MADEV_EN_PIN, GPIO_PULL_UP);
	mt_set_gpio_pull_enable(MADEV_EN_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_out(MADEV_EN_PIN,GPIO_OUT_ONE);
	mdelay(10);

	//set dir pull to save power
#if 1
	//mt_set_gpio_mode(GPIO_SPI_CS_PIN, GPIO_SPI_CS_PIN_M_GPIO);
	mt_set_gpio_mode(GPIO_SPI_CS_PIN, GPIO_SPI_CS_PIN_M_SPI_CSA);
	mt_set_gpio_dir(GPIO_SPI_CS_PIN, GPIO_DIR_OUT);
	mt_set_gpio_pull_select(GPIO_SPI_CS_PIN, GPIO_PULL_UP);
	mt_set_gpio_pull_enable(GPIO_SPI_CS_PIN, GPIO_PULL_ENABLE);

	mt_set_gpio_mode(GPIO_SPI_SCK_PIN, GPIO_SPI_SCK_PIN_M_SPI_CKA);
	mt_set_gpio_dir(GPIO_SPI_SCK_PIN, GPIO_DIR_OUT);
	mt_set_gpio_pull_select(GPIO_SPI_SCK_PIN, GPIO_PULL_UP);
	mt_set_gpio_pull_enable(GPIO_SPI_SCK_PIN, GPIO_PULL_ENABLE);

	mt_set_gpio_mode(GPIO_SPI_MISO_PIN, GPIO_SPI_MISO_PIN_M_SPI_MIA);
	mt_set_gpio_dir(GPIO_SPI_MISO_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_select(GPIO_SPI_MISO_PIN, GPIO_PULL_UP);
	mt_set_gpio_pull_enable(GPIO_SPI_MISO_PIN, GPIO_PULL_ENABLE);

	mt_set_gpio_mode(GPIO_SPI_MOSI_PIN, GPIO_SPI_MOSI_PIN_M_SPI_MOA);
	mt_set_gpio_dir(GPIO_SPI_MOSI_PIN, GPIO_DIR_OUT);
	mt_set_gpio_pull_select(GPIO_SPI_MOSI_PIN, GPIO_PULL_UP);
	mt_set_gpio_pull_enable(GPIO_SPI_MOSI_PIN, GPIO_PULL_ENABLE);
	mdelay(10);
#else

	        mt_set_gpio_mode(GPIO65,GPIO_MODE_01);
		mt_set_gpio_dir(GPIO65,GPIO_DIR_OUT);
	        mt_set_gpio_pull_select(GPIO65, GPIO_PULL_UP);
		mt_set_gpio_pull_enable(GPIO65,GPIO_PULL_ENABLE);

		mt_set_gpio_mode(GPIO66,GPIO_MODE_01);
		mt_set_gpio_dir(GPIO66,GPIO_DIR_OUT);
	        mt_set_gpio_pull_select(GPIO66, GPIO_PULL_UP);
		mt_set_gpio_pull_enable(GPIO66,GPIO_PULL_ENABLE);

		mt_set_gpio_mode(GPIO67,GPIO_MODE_01);
		mt_set_gpio_dir(GPIO67,GPIO_DIR_IN);
	        mt_set_gpio_pull_select(GPIO67, GPIO_PULL_UP);
		mt_set_gpio_pull_enable(GPIO67,GPIO_PULL_ENABLE);

		mt_set_gpio_mode(GPIO68,GPIO_MODE_01);
		mt_set_gpio_dir(GPIO68,GPIO_DIR_OUT);
	        mt_set_gpio_pull_select(GPIO68, GPIO_PULL_UP);
		mt_set_gpio_pull_enable(GPIO68,GPIO_PULL_ENABLE);

		//mt_set_gpio_mode(GPIO65,GPIO_MODE_01);
		//mt_set_gpio_mode(GPIO66,GPIO_MODE_01);
		//mt_set_gpio_mode(GPIO67,GPIO_MODE_01);
		//mt_set_gpio_mode(GPIO68,GPIO_MODE_01);
	        mdelay(10);
#endif
	return;
}
#endif
/*---------------------------------- fops ------------------------------------*/
static int mas_open(struct inode *inode, struct file *filp) {
	int ret = -ENXIO;

	printd("%s: start\n", __func__);
	mutex_lock(&dev_lock);
#if  1//def MTK
	hwPowerOn(MT6328_POWER_LDO_VGP1, VOL_3000, MODE_NAME);
#endif
	list_for_each_entry(smas, &dev_list, dev_entry) {
		if (smas->devt == inode->i_rdev) {
			ret = 0;
			break;
		}
	}
	if (!ret) {
		nonseekable_open(inode, filp);
	} else {
		printw("%s: nothing for minor\n", __func__);
	}
	mutex_unlock(&dev_lock);
	printd("%s: end\n", __func__);

	return ret;
}

/* ????????
 * @buf ??????,
 * 		?: buf[0]???,?????
 * 		?: ??????len?????
 * @len ????len=0???len>0??
 * @????0???????
 */
int mas_sync(struct spi_device *spi, char *buf, int len) {
	int ret;

	printd("%s: start. len=%d\n", __func__, len);
	smas->xfer.tx_buf = buf + 1;
	smas->xfer.rx_buf = buf;
	smas->xfer.delay_usecs = 1;
	smas->xfer.len = len == 0 ? buf[0] : len;
	smas->xfer.bits_per_word = 8;
	smas->xfer.speed_hz = smas->speed;
	spi_message_init(&smas->msg);
	spi_message_add_tail(&smas->xfer, &smas->msg);
	ret = spi_sync(spi, &smas->msg);

	printd("%s: end. ret=%d\n", __func__, ret);
	return ret;
}

// 发送指令
int mas_send(char *buf, int len) {
	int ret = 0;
	struct mt_chip_conf *conf = (struct mt_chip_conf *) smas->spi->controller_data;
	conf->com_mod = FIFO_TRANSFER;
	spi_setup(smas->spi);

	ret = mas_sync(smas->spi, buf, len);
	msleep(10);
	return ret;
}
// ????
int mas_frame(struct spi_device *spi, char *buf) {
	int ret;

	printd("%s: start.\n", __func__);
	buf[0] = MA_READ_FRAME_REG;
	memset(buf+1, 0xff, FBUF-1);
	smas->xfer.tx_buf = buf;
	smas->xfer.rx_buf = buf;
	smas->xfer.len = FBUF;
	smas->xfer.bits_per_word = 8;
	smas->xfer.speed_hz = smas->speed;
	spi_message_init(&smas->msg);
	spi_message_add_tail(&smas->xfer, &smas->msg);
	ret = spi_sync(spi, &smas->msg);
	printd("%s: end. ret=%d\n", __func__, ret);

	return ret;
}

static ssize_t mas_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	int ret = 0;

	printd("%s: start. count=%d\n", __func__, count);
	if(count>FBUF) {
		printw("%s: read data too long.\n", __func__);
		return -EMSGSIZE;
	}
#ifdef MTK
	struct mt_chip_conf *conf = (struct mt_chip_conf *) smas->spi->controller_data;
	conf->com_mod = count==FBUF? DMA_TRANSFER: FIFO_TRANSFER;
	spi_setup(smas->spi);
#endif
	if(count==FBUF) { // ????
		ret = mas_frame(smas->spi, sbuf);
	} else { // ?????
		ret = mas_sync(smas->spi, sbuf, count);
	}
	if(ret) {
		printw("%s: mas_sync failed.\n", __func__);
	} else {
		if(count) {
			ret = copy_to_user(buf, sbuf, count);
			if(!ret) ret = count;
			else {
				ret = -EFAULT;
				printw("%s: copy_to_user failed.\n", __func__);
			}
		}
	}
	printd("%s: end. ret=%d\n", __func__, ret);

	return count==0? 0: ret;
}

static ssize_t mas_write(struct file *filp, const char __user *buf,size_t count, loff_t *f_pos) {
	int ret;
	printd("%s: start. count=%d\n", __func__, count);
	//printk("[spi hxq]:aaa 888\n");
		
		//finger_cs_l();
/*

	unsigned char pix_read[]={0x50,0x00,0x20,0x00};

	unsigned char i,j;
	
	
	for(i=0;i<10;i++)
		{
		for(j=0;j<4;j++)
			{
				spi_write_finger(&pix_read[i],1);
			printk("spi_write_finger---number=[%d]\n",i);
			}
		}
	*/
	
	if(count>FBUF) {
		printw("%s: write data too long.\n", __func__);
		return -EMSGSIZE;
	}
	memset(sbuf, 0, FBUF);
	ret = copy_from_user(sbuf, buf, count);
	if(ret) {
		printw("%s: copy_from_user failed.\n", __func__);
		ret = -EFAULT;
	} else {
		ret = count;
	}
	printd("%s: end. ret =%d\n", __func__, ret);

	return ret;
}

static long mas_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int ret = 0;

	printd("%s: start cmd=%d arg=%d\n", __func__, cmd, arg);
	switch (cmd) {
	case DRV_DEBUG:
		sdeb = (u8) arg;
		break;
	case DRV_SPI_SPEED:
		smas->speed = (u32)arg;
		break;
	case DRV_FBUF:
		ret = FBUF;
		break;
	case DRV_AUTOTEST_FBUF:
	{
		int i;
		const unsigned char cdat[] = { 0x41, 0x78, 0x4d, 0x41 };
		sbuf[0] = 3;
		sbuf[1] = 0x8c;
		sbuf[2] = 0xff;
		sbuf[3] = 0xff;
		if( mas_send(sbuf, 0) ) return 0;
		for(i=0; i<4; i++) {
			sbuf[0] = 4;
			sbuf[1] = i*4;
			sbuf[2] = cdat[i];
			sbuf[3] = 0xff;
			sbuf[4] = 0xff;
			if( mas_send(sbuf, 0)) return 0;
			if(sbuf[2]!=cdat[i]) return 0;
		}
		ret = 1;
		break;
	}
	}
	printd("%s: end\n", __func__);

	return ret;
}

static int mas_release(struct inode *inode, struct file *filp) {
	int ret = 0;

	printd("%s: start\n", __func__);
#if  0//def MTK
	if (hwPowerDown(MT6323_POWER_LDO_VGP1, MODE_NAME)!=1) {
		printw("%s: Fail to disable analog power\n", __func__);
	}
#endif
	printd("%s: end\n", __func__);

	return ret;
}

static const struct file_operations mas_fops = {
	.owner = THIS_MODULE,
	.write = mas_write,
	.read = mas_read,
	.unlocked_ioctl = mas_ioctl,
	.open = mas_open,
	.release = mas_release,
};

static int mas_probe(struct spi_device *spi) {
	int ret;
	unsigned long minor;

	printd("%s: start\n", __func__);
	//smas->speed = 10*000000;
	smas->spi = spi;
	spi->max_speed_hz = smas->speed;
	spi_setup(spi);
	spin_lock_init(&smas->spi_lock);
	INIT_LIST_HEAD(&smas->dev_entry);
	mutex_lock(&dev_lock);
	minor = find_first_zero_bit(minors, MASPI_MINOR);
	if (minor < MASPI_MINOR) {
		struct device *dev;
		smas->devt = MKDEV(MASPI_MAJOR, minor);
		dev = device_create(smas->cls, &spi->dev, smas->devt, smas, "madev0");
		ret = IS_ERR(dev) ? PTR_ERR(dev) : 0;
		if (ret == 0) {
			set_bit(minor, minors);
			list_add(&smas->dev_entry, &dev_list);
		}
	} else {
		printw("%s: no minor number available!\n", __func__);
		ret = -ENODEV;
		goto fail;
	}
	mutex_unlock(&dev_lock);
	spi_set_drvdata(spi, smas);
	printw("%s: insmod successfully.\n", __func__);
	return ret;
fail:
	printw("%s: insmod failed.\n", __func__);

	return ret;
}

static int mas_remove(struct spi_device *spi) {
	printd("%s: start\n", __func__);
	// make sure ops on existing fds can abort cleanly
	spin_lock_irq(&smas->spi_lock);
	smas->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&smas->spi_lock);

	// prevent new opens
	mutex_lock(&dev_lock);
	list_del(&smas->dev_entry);
	device_destroy(smas->cls, smas->devt);
	clear_bit(MINOR(smas->devt), minors);
	mutex_unlock(&dev_lock);
	printd("%s: end\n", __func__);

	return 0;
}

#ifndef MTK
static struct spi_driver mas_driver = {
	.driver = {
		.name = "ma_dev",
		.owner = THIS_MODULE,
	},
	.probe = mas_probe,
	.remove = __devexit_p(mas_remove),
};
#else

#ifdef CONFIG_HAS_EARLYSUSPEND
//static void mas_ts_suspend(struct device *dev, pm_message_t message)
static void mas_ts_suspend(struct early_suspend *handler)
{
//	struct spi_driver *drv = to_spi_driver(dev->driver);

	printd("%s: start\n", __func__);

/*	sbuf[0] = 3;
	sbuf[1] = 0x80;
	sbuf[2] = 0xff;
	sbuf[3] = 0xff;
	mas_send(sbuf, 0);
	*/
	mdelay(10);
	finger_en_l();

	/* suspend will stop irqs and dma; no more i/o */
		/* 挂起将定制终端和DMA，没有输入输出 */
/*	if (drv) {

		if (drv->suspend) {
			drv->suspend(to_spi_device(dev), message);
		} else {
			printw("%s: can't suspend\n", __func__);
		}
	}
	*/
}

//static void mas_ts_resume(struct device *dev) 
static void mas_ts_resume(struct early_suspend *handler) 
{
	//struct spi_driver *drv = to_spi_driver(dev->driver);

	printd("%s: start\n", __func__);
	finger_en_h();
	mdelay(10);	
	/* resume may restart the i/o queue */
	/* 重新开始输入输出队列 */
	/*if (drv) {
		if (drv->resume) {
			drv->resume(to_spi_device(dev));
		} else {
			printw("%s: can't resume\n", __func__);
		}
	}*/
	/*
	sbuf[0] = 3;
	sbuf[1] = 0x8c;
	sbuf[2] = 0xff;
	sbuf[3] = 0xff;
	mas_send(sbuf, 0);
	*/
}
#endif

struct spi_device_id device_id = {"ma_dev", 0};
#if 0
static struct spi_driver mas_driver = {
	.driver = {
		.name =	"ma_dev",
		.owner = THIS_MODULE,
	},
	.probe = mas_probe,
#if  0//def CONFIG_HAS_EARLYSUSPEND
	.suspend = mas_ts_suspend,
	.resume	= mas_ts_resume,
#endif
	.remove = __devexit_p(mas_remove),
	.id_table = &device_id,
};
#endif
#ifdef CONFIG_OF
static const struct of_device_id mas_of_match[] = {
	{ .compatible = "mediatek,ma_dev", },
	{},
};
#endif
static struct spi_driver mas_driver = {
	.driver = {
		.name =	"ma_dev",
		#ifdef CONFIG_OF
		.of_match_table = mas_of_match,
		#endif
		.owner = THIS_MODULE,
	},
	.probe = mas_probe,
	.remove = mas_remove,
	.id_table = &device_id,
};

static struct mt_chip_conf mt_conf = {
	.setuptime = 1,
	.holdtime = 1,
	.high_time = 4, // 10--6m 15--4m 20--3m 30--2m [ 60--1m 120--0.5m  300--0.2m]
	.low_time = 4,
	.cs_idletime = 2,
	.ulthgh_thrsh = 0,
	.cpol = 0,
	.cpha = 0,
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
static struct spi_board_info sfp_spi1_board_info[] __initdata = {
	{
		.modalias = "ma_dev",
		//.max_speed_hz = 16 * 1000000,
		.bus_num = 0,
		.chip_select = 1,
		.mode = SPI_MODE_0,
		.controller_data=&mt_conf
	}
};

#endif

/*---------------------------------- module ------------------------------------*/
static int __init mas_init(void) {
	int status;
	int ret;
	dev_t devno;
	printk("%s: start\n", __func__);
	mt_spi_set_mod();

	smas = kmalloc(sizeof(struct maspi), GFP_KERNEL);
	hwPowerOn(MT6328_POWER_LDO_VGP1, VOL_3000, MODE_NAME);
	if (!smas) {
		printw("%s: smas kzalloc failed.\n", __func__);
		return -ENOMEM;
	}

	smas->xfer.speed_hz = 10*1000*1000;
	if(KMALLOC_MAX_SIZE<FBUF) {
		printw("%s: KMALLOC_MAX_SIZE too small.\n", __func__);
	}

	sbuf = kmalloc(FBUF, GFP_KERNEL);

	if(!sbuf) {
		printw("%s: sbuf kmalloc failed.\n", __func__);
		kfree(smas);
		smas = NULL;
		return -ENOMEM;
	}

#ifdef MTK
	{
		
		spi_register_board_info(sfp_spi1_board_info, ARRAY_SIZE(sfp_spi1_board_info));

		status = alloc_chrdev_region(&devno, 0, 255, "maspidev");
		if(status < 0) {
			printw("%s: alloc_chrdev_region failed.ret=%d\n", __func__, ret);
			return status;
		}

		MASPI_MAJOR = MAJOR(devno);
		cdev_init(&sdev, &mas_fops);
	    sdev.owner = THIS_MODULE;
		status = cdev_add(&sdev, MKDEV(MASPI_MAJOR, 0), MASPI_MINOR);

		if(status != 0) {
			printw("%s: cdev_add failed. ret=%d\n", __func__, ret);
			return status;
		}
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	//device_init_wakeup(&sdev, 1);
	//smas->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	smas->early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 1;
	smas->early_suspend.suspend = mas_ts_suspend;
	smas->early_suspend.resume	= mas_ts_resume;
	register_early_suspend(&smas->early_suspend);
#endif
#else
	BUILD_BUG_ON(MASPI_MINOR > 256);
	ret = register_chrdev(MASPI_MAJOR, "madev", &mas_fops);
	if (ret < 0) {
		printw("%s: register_chrdev failed.\n", __func__);
		return ret;
	}
#endif

	smas->cls = class_create(THIS_MODULE, "madev");
	if (IS_ERR(smas->cls)) {
		printw("%s: class_create failed.\n", __func__);
		unregister_chrdev(MASPI_MAJOR, mas_driver.driver.name);
		return PTR_ERR(smas->cls);
	}

	ret = spi_register_driver(&mas_driver);
	if (ret < 0) {
		printw("%s: spi_register_driver failed.\n", __func__);
		class_destroy(smas->cls);
		unregister_chrdev(MASPI_MAJOR, mas_driver.driver.name);
	}

	printd("%s: end\n", __func__);
	return ret;
}


static void __exit mas_exit(void) {
	printd("%s: start\n", __func__);

	spi_unregister_driver(&mas_driver);
	class_destroy(smas->cls);
	unregister_chrdev(MASPI_MAJOR, mas_driver.driver.name);
#ifdef CONFIG_HAS_EARLYSUSPEND
	//device_init_wakeup(&sdev, 0);
	unregister_early_suspend(&smas->early_suspend);
#endif
	if(smas!=NULL) kfree(smas);
	if(sbuf!=NULL) kfree(sbuf);

	printd("%s: end\n", __func__);
}

module_init(mas_init);
module_exit(mas_exit);

MODULE_AUTHOR("zlchen");
MODULE_DESCRIPTION("for fprint driver");
MODULE_LICENSE("GPL");

