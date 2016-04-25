#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/poll.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/timer.h> 
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/earlysuspend.h>

#include <cust_eint.h>
#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>
#include <mach/mt_reg_base.h>
#include <mtk_kpd.h>		
#include <mach/irqs.h>
#include <mach/eint.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pmic_wrap.h>
#include <mach/mt_sleep.h>
#include <mach/sync_write.h>
#include "cust_eint_for_key.h"

extern void mt_eint_unmask(unsigned int line);
extern void mt_eint_mask(unsigned int line);
extern void mt_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern unsigned int mt_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt_eint_registration(unsigned int eint_num, unsigned int flag, void (EINT_FUNC_PTR) (void), unsigned int is_auto_umask);

static struct input_dev *kpd_input_dev;
static int bAlreadyRead = 0;
bool g_kpd_hall_on = false;


#ifdef CUST_EINT_FLIPHALL_NUM
static struct work_struct fliphall_work;
static struct workqueue_struct * fliphall_workqueue = NULL;
#endif
#ifdef CUST_EINT_SWITCH_DETECTOR_NUM
static struct work_struct switch_detector_work;
static struct workqueue_struct * switch_detector_workqueue = NULL;
#endif
#ifdef CUST_EINT_KEYMUTE_NUM
static struct work_struct keymute_work;
static struct workqueue_struct * keymute_workqueue = NULL;
static bool current_mute_status = false;
#endif
#ifdef CUST_EINT_FLIPHALL_NUM
static void kpd_fliphall_work_callback(struct work_struct *work)
{
    int  count =0, inversecount=0;
    bool Gpio_signal = 0;

    mt_eint_mask(CUST_EINT_FLIPHALL_NUM);
    mt_set_gpio_mode(GPIO_FLIPHALL_EINT_PIN, GPIO_FLIPHALL_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_FLIPHALL_EINT_PIN, GPIO_DIR_IN);

    while(count < 2 && inversecount < 2){
        Gpio_signal =  mt_get_gpio_in(GPIO_FLIPHALL_EINT_PIN);  // get signal
        if(Gpio_signal == 0){
            count++;
            inversecount = 0;
        } else {
            inversecount++;
            count = 0;
        }
        mdelay(20);
    }
    if(count) {
		DBG_LOG("chsj hall in\n");
		if(!g_kpd_hall_on){
			g_kpd_hall_on = true;	
			input_report_key(kpd_input_dev, KEY_F9, 1);
			input_report_key(kpd_input_dev, KEY_F9, 0);
			input_sync(kpd_input_dev);
		}
        mt_set_gpio_mode(GPIO_FLIPHALL_EINT_PIN, GPIO_FLIPHALL_EINT_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_FLIPHALL_EINT_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_FLIPHALL_NUM, MT_EINT_POL_POS);
        mt_eint_unmask(CUST_EINT_FLIPHALL_NUM);
    } else {
		DBG_LOG("chsj hall out\n");
		if(g_kpd_hall_on){
			g_kpd_hall_on = false;	
			input_report_key(kpd_input_dev, KEY_F10, 1);
			input_report_key(kpd_input_dev, KEY_F10, 0);
			input_sync(kpd_input_dev);
		}
        mt_set_gpio_mode(GPIO_FLIPHALL_EINT_PIN, GPIO_FLIPHALL_EINT_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_FLIPHALL_EINT_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_FLIPHALL_NUM, MT_EINT_POL_NEG);
        mt_eint_unmask(CUST_EINT_FLIPHALL_NUM);
    }
}

void kpd_fliphall_eint_handler()
{
    int ret = queue_work(fliphall_workqueue, &fliphall_work);
    if(!ret)
        DBG_LOG("fliphall_irq_handler:fliphall_work return %d", ret);
}
#endif
#ifdef CUST_EINT_SWITCH_DETECTOR_NUM
static void kpd_switch_detector_work_callback(struct work_struct *work)
{
    int  count =0, inversecount=0;
    bool Gpio_signal = 0;

    mt_eint_mask(CUST_EINT_SWITCH_DETECTOR_NUM);
    mt_set_gpio_mode(GPIO_SWITCH_DETECTOR_PIN, GPIO_SWITCH_DETECTOR_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_SWITCH_DETECTOR_PIN, GPIO_DIR_IN);

    while(count < 5 && inversecount < 5){
        Gpio_signal =  mt_get_gpio_in(GPIO_SWITCH_DETECTOR_PIN);  // get signal
        if(Gpio_signal == 0){
            count++;
            inversecount = 0;
        } else {
            inversecount++;
            count = 0;
        }
        msleep(60);
    }
    if(count) {
		DBG_LOG("chsj switch_detector in\n");
        input_report_key(kpd_input_dev, KEY_F11, 1);
        input_report_key(kpd_input_dev, KEY_F11, 0);
		input_sync(kpd_input_dev);
        mt_set_gpio_mode(GPIO_SWITCH_DETECTOR_PIN, GPIO_SWITCH_DETECTOR_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_SWITCH_DETECTOR_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_SWITCH_DETECTOR_NUM, MT_EINT_POL_POS);
        mt_eint_unmask(CUST_EINT_SWITCH_DETECTOR_NUM);
    } else {
		DBG_LOG("chsj switch_detector out\n");
        input_report_key(kpd_input_dev, KEY_F12, 1);
        input_report_key(kpd_input_dev, KEY_F12, 0);
        input_sync(kpd_input_dev);
        mt_set_gpio_mode(GPIO_SWITCH_DETECTOR_PIN, GPIO_SWITCH_DETECTOR_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_SWITCH_DETECTOR_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_SWITCH_DETECTOR_NUM, MT_EINT_POL_NEG);
        mt_eint_unmask(CUST_EINT_SWITCH_DETECTOR_NUM);
    }
}

void kpd_switch_detector_eint_handler()
{
    int ret = queue_work(switch_detector_workqueue, &switch_detector_work);
    if(!ret)
        DBG_LOG("switch_detector_irq_handler:switch_detector_work return %d", ret);
}
#endif
#ifdef CUST_EINT_KEYMUTE_NUM
static void kpd_keymute_work_callback(struct work_struct *work)
{
    int  count =0, inversecount=0;
    bool Gpio_signal = 0;

    mt_eint_mask(CUST_EINT_KEYMUTE_NUM);
    mt_set_gpio_mode(GPIO_KEYMUTE_EINT_PIN, GPIO_KEYMUTE_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_KEYMUTE_EINT_PIN, GPIO_DIR_IN);

	mdelay(100);
    while(count < 3 && inversecount < 3){
        Gpio_signal =  mt_get_gpio_in(GPIO_KEYMUTE_EINT_PIN);  // get signal
        if(Gpio_signal == 0){
            count++;
            inversecount = 0;
        } else {
            inversecount++;
            count = 0;
        }
        mdelay(20);
    }
	DBG_LOG("chsj keymute status %d %d\n",count,inversecount);
    if(count) {
		DBG_LOG("chsj keymute press\n");
		if(!current_mute_status){
			input_report_key(kpd_input_dev, KEY_MUTE, 1);
			input_report_key(kpd_input_dev, KEY_MUTE, 0);
			input_sync(kpd_input_dev);
			current_mute_status = true;
		}
        mt_set_gpio_mode(GPIO_KEYMUTE_EINT_PIN, GPIO_KEYMUTE_EINT_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_KEYMUTE_EINT_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_KEYMUTE_NUM, MT_EINT_POL_POS);
        mt_eint_unmask(CUST_EINT_KEYMUTE_NUM);
    } else {
		DBG_LOG("chsj keymute release\n");
		if(current_mute_status){
			input_report_key(kpd_input_dev, KEY_MUTE1, 1);
			input_report_key(kpd_input_dev, KEY_MUTE1, 0);
			input_sync(kpd_input_dev);
			current_mute_status = false;
		}
        mt_set_gpio_mode(GPIO_KEYMUTE_EINT_PIN, GPIO_KEYMUTE_EINT_PIN_M_EINT);
        mt_set_gpio_dir(GPIO_KEYMUTE_EINT_PIN, GPIO_DIR_IN);
        mt_eint_set_polarity(CUST_EINT_KEYMUTE_NUM, MT_EINT_POL_NEG);
        mt_eint_unmask(CUST_EINT_KEYMUTE_NUM);
    }
}

void kpd_keymute_eint_handler()
{
    int ret = queue_work(keymute_workqueue, &keymute_work);
    if(!ret)
        DBG_LOG("keymute_irq_handler:keymute_work return %d", ret);
}
#endif

static long cust_eint_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	DBG_LOG("eint ioctl cmd = %d \n",cmd);
	
	return 0;
}

static ssize_t cust_eint_write(struct file *file, const char *buffer,
		size_t count, loff_t *ppos)
{
	return count;
}

static ssize_t cust_eint_read(struct file *filp, char *buff,
		size_t count, loff_t *ppos){
	int i,err;
	char tmpBuff[128];
	if(bAlreadyRead){
		return 0;
	}
	memset(tmpBuff,0,128);
	memset(buff,0,count);
#ifdef CUST_EINT_FLIPHALL_NUM
	sprintf(tmpBuff,"%sGpio_get:hall gpio =%d, status=%d!\n",
			tmpBuff,GPIO_FLIPHALL_EINT_PIN&0xFFFF,mt_get_gpio_in(GPIO_FLIPHALL_EINT_PIN));
#endif
#ifdef CUST_EINT_SWITCH_DETECTOR_NUM
	sprintf(tmpBuff,"%sGpio_get:hall gpio =%d, status=%d!\n",
			tmpBuff,GPIO_SWITCH_DETECTOR_PIN&0xFFFF,mt_get_gpio_in(GPIO_SWITCH_DETECTOR_PIN));
#endif
#ifdef CUST_EINT_KEYMUTE_NUM
	sprintf(tmpBuff,"%sGpio_get:keymute gpio =%d, status=%d!\n",
			tmpBuff,GPIO_KEYMUTE_EINT_PIN&0xFFFF,mt_get_gpio_in(GPIO_KEYMUTE_EINT_PIN));
#endif
	if(!strlen(tmpBuff)){
		sprintf(tmpBuff,"No gpio set for key!\n");
	}
	err = copy_to_user((void *)buff, (const void *)(&tmpBuff[0]),
			min(strlen(tmpBuff), count));
	bAlreadyRead = 1;
	return err ? -EFAULT : min(strlen(tmpBuff), count);
}



static int cust_eint_open(struct inode *inode, struct file *file) {
	
    DBG_LOG("eint---open OK---\n");
	bAlreadyRead = 0;
    return 0;
}

static int cust_eint_close(struct inode *inode, struct file *file) {
    DBG_LOG("eint---close---\n");
	return 0;
}



static struct file_operations cust_eint_ops = {
	.owner			= THIS_MODULE,
	.open			= cust_eint_open,
	.release		= cust_eint_close, 
	.write          = cust_eint_write,
	.read			=	cust_eint_read,
	.unlocked_ioctl	=	cust_eint_ioctl,
	
};




static struct miscdevice ET_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &cust_eint_ops,
};

static int __init cust_eint_dev_init(void) {
	int ret;
	if((ret = misc_register(&ET_misc_dev)))
	{
		DBG_LOG("eint: misc_register register failed\n");
	}
	kpd_input_dev = input_allocate_device();
	if (!kpd_input_dev)
		return -ENOMEM;

	kpd_input_dev->name = DEVICE_NAME;

	__set_bit(EV_KEY, kpd_input_dev->evbit);
	ret = input_register_device(kpd_input_dev);
	if (ret) {
		DBG_LOG("register input device failed (%d)\n", ret);
		input_free_device(kpd_input_dev);
		return ret;
	}
#ifdef CUST_EINT_FLIPHALL_NUM
			int fliphall_gpio_signal = 0;
	
			__set_bit(EV_SW, kpd_input_dev->evbit);
			__set_bit(KEY_F9, kpd_input_dev->keybit);
			__set_bit(KEY_F10, kpd_input_dev->keybit);
	
			mt_set_gpio_mode(GPIO_FLIPHALL_EINT_PIN, GPIO_FLIPHALL_EINT_PIN_M_GPIO);
			mt_set_gpio_dir(GPIO_FLIPHALL_EINT_PIN, GPIO_DIR_IN);
			mt_set_gpio_pull_enable(GPIO_FLIPHALL_EINT_PIN,GPIO_PULL_ENABLE);
			mt_set_gpio_pull_select(GPIO_FLIPHALL_EINT_PIN,GPIO_PULL_UP);
			fliphall_gpio_signal = mt_get_gpio_in(GPIO_FLIPHALL_EINT_PIN);	// get signal
			fliphall_workqueue = create_singlethread_workqueue("Fliphall");
			INIT_WORK(&fliphall_work, kpd_fliphall_work_callback);
			//mt_eint_set_sens(CUST_EINT_FLIPHALL_NUM, CUST_EINT_FLIPHALL_SENSITIVE);
			mt_eint_set_hw_debounce(CUST_EINT_FLIPHALL_NUM, CUST_EINT_FLIPHALL_DEBOUNCE_CN);
			mt_eint_registration(CUST_EINT_FLIPHALL_NUM, (fliphall_gpio_signal == 1 ? CUST_EINTF_TRIGGER_LOW : CUST_EINTF_TRIGGER_HIGH),
					kpd_fliphall_eint_handler, false);
			queue_work(fliphall_workqueue, &fliphall_work);
			//mt_eint_unmask(CUST_EINT_FLIPHALL_NUM);
#endif
#ifdef CUST_EINT_SWITCH_DETECTOR_NUM
				int switch_detector_gpio_signal = 0;
		
				__set_bit(EV_SW, kpd_input_dev->evbit);
				__set_bit(KEY_F11, kpd_input_dev->keybit);
				__set_bit(KEY_F12, kpd_input_dev->keybit);
		
				mt_set_gpio_mode(GPIO_SWITCH_DETECTOR_PIN, GPIO_SWITCH_DETECTOR_PIN_M_GPIO);
				mt_set_gpio_dir(GPIO_SWITCH_DETECTOR_PIN, GPIO_DIR_IN);
				mt_set_gpio_pull_enable(GPIO_SWITCH_DETECTOR_PIN,GPIO_PULL_ENABLE);
				mt_set_gpio_pull_select(GPIO_SWITCH_DETECTOR_PIN,GPIO_PULL_UP);
				switch_detector_gpio_signal = mt_get_gpio_in(GPIO_SWITCH_DETECTOR_PIN); // get signal
				switch_detector_workqueue = create_singlethread_workqueue("switch_detector");
				INIT_WORK(&switch_detector_work, kpd_switch_detector_work_callback);
				//mt_eint_set_sens(CUST_EINT_SWITCH_DETECTOR_NUM, CUST_EINT_SWITCH_DETECTOR_SENSITIVE);
				mt_eint_set_hw_debounce(CUST_EINT_SWITCH_DETECTOR_NUM, CUST_EINT_SWITCH_DETECTOR_DEBOUNCE_CN);
				mt_eint_registration(CUST_EINT_SWITCH_DETECTOR_NUM, (switch_detector_gpio_signal == 1 ? CUST_EINTF_TRIGGER_LOW : CUST_EINTF_TRIGGER_HIGH),
						kpd_switch_detector_eint_handler, false);
				queue_work(switch_detector_workqueue, &switch_detector_work);
				//mt_eint_unmask(CUST_EINT_SWITCH_DETECTOR_NUM);
#endif
#ifdef CUST_EINT_KEYMUTE_NUM
				int keymute_gpio_signal = 0;
		
				__set_bit(EV_SW, kpd_input_dev->evbit);
				__set_bit(KEY_MUTE, kpd_input_dev->keybit);
				__set_bit(KEY_MUTE1, kpd_input_dev->keybit);
		
				mt_set_gpio_mode(GPIO_KEYMUTE_EINT_PIN, GPIO_KEYMUTE_EINT_PIN_M_GPIO);
				mt_set_gpio_dir(GPIO_KEYMUTE_EINT_PIN, GPIO_DIR_IN);
				mt_set_gpio_pull_enable(GPIO_KEYMUTE_EINT_PIN,GPIO_PULL_ENABLE);
				mt_set_gpio_pull_select(GPIO_KEYMUTE_EINT_PIN,GPIO_PULL_UP);
				keymute_gpio_signal = mt_get_gpio_in(GPIO_KEYMUTE_EINT_PIN);	// get signal
				keymute_workqueue = create_singlethread_workqueue("MuteKey");
				INIT_WORK(&keymute_work, kpd_keymute_work_callback);
				//mt_eint_set_sens(CUST_EINT_KEYMUTE_NUM, CUST_EINT_KEYMUTE_SENSITIVE);
				mt_eint_set_hw_debounce(CUST_EINT_KEYMUTE_NUM, CUST_EINT_KEYMUTE_DEBOUNCE_CN);
				mt_eint_registration(CUST_EINT_KEYMUTE_NUM, (keymute_gpio_signal == 1 ? CUST_EINTF_TRIGGER_LOW : CUST_EINTF_TRIGGER_HIGH),
						kpd_keymute_eint_handler, false);
				queue_work(keymute_workqueue, &keymute_work);
				//mt_eint_unmask(CUST_EINT_FLIPHALL_NUM);
#endif

	DBG_LOG("eint initialized\n");

	return ret;
}

static void __exit cust_eint_dev_exit(void) {
	input_free_device(kpd_input_dev);
	misc_deregister(&ET_misc_dev);	
}

module_init(cust_eint_dev_init);
module_exit(cust_eint_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHSJ");
MODULE_DESCRIPTION("CUST SWITCH EINT DRIVER");
