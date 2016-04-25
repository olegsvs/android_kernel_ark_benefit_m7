#ifndef CUST_EINT_FOR_KEY_H
#define CUST_EINT_FOR_KEY_H

#define DEVICE_DEBUG
#define DEVICE_NAME				"cust_gpio_key"
#define LOG_TAG				"cust_gpio_key"
#ifdef DEVICE_DEBUG
#define DBG_LOG(fmt,arg...) printk(LOG_TAG " %s: " fmt "\n", __func__ ,##arg)
#else
#define DBG_LOG(fmt,arg...)  
#endif

#endif/*CUST_EINT_FOR_KEY_H*/
