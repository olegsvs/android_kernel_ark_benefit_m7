#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <mach/mt_spi.h>
#include "eh570_data_transfer.h"
#include "eh570_define_jadeOpc.h"
#include "eh570_register_define.h"

int eh570_mass_read(struct eh570_data *eh570, u8 addr, u8 *buf, int read_len)
{
	ssize_t status;
	struct spi_device *spi;
	struct spi_message m;
	u8 *write_addr;
	/* Set start address */
	u8 *read_data = kzalloc(read_len + 1024, GFP_KERNEL);
	if (read_data == NULL)
		return -ENOMEM;

	write_addr = kzalloc(2, GFP_KERNEL);
	write_addr[0] = EH570_WRITE_ADDRESS;
	write_addr[1] = addr;

	/* Write and read data in one data query section */
	struct spi_transfer t_set_addr = {
		.tx_buf = write_addr,
		.len = 2,
	};
	struct spi_transfer t_read_data = {
		.tx_buf = read_data,
		.rx_buf = read_data,
		.len = read_len + 1024,
	};

	DEBUG_PRINT("%s read_len = %d\n", __func__, read_len);
	read_data[0] = EH570_READ_DATA;
	spi = eh570->spi;
	spi_message_init(&m);
	spi_message_add_tail(&t_set_addr, &m);
	spi_message_add_tail(&t_read_data, &m);
	status = spi_sync(spi, &m);

	kfree(write_addr);

	if (status == 0) {
		memcpy(buf, read_data + 3, read_len);
	} else {
	//	pr_err(KERN_ERR "%s read data error status = %d\n"
		//		, __func__, status);
	}
	kfree(read_data);

	return status;
}

/*
 * Read io register
 */
int eh570_io_read_register(struct eh570_data *eh570, u8 *addr, u8 *buf)
{
	ssize_t status = 0;
	struct spi_device *spi;
	struct spi_message m;
	int read_len = 1;
	u8 *result;
	u8 val, addrval;

	if (copy_from_user(&addrval, (const u8 __user *) (uintptr_t) addr
		, read_len)) {
		pr_err(KERN_ERR "%s buffer copy_from_user fail", __func__);
		status = -EFAULT;
		goto out;
	}

	u8 *write_addr = kzalloc(2, GFP_KERNEL);
	write_addr[0] = EH570_WRITE_ADDRESS;
	write_addr[1] = addrval;
	u8 *read_value = kzalloc(2, GFP_KERNEL);
	read_value[0] = EH570_READ_DATA;
	read_value[1] = 0x00;

	result = kzalloc(2, GFP_KERNEL);
	result[0] = 0xFF;
	result[1] = 0xFF;
	struct spi_transfer t_set_addr = {
		.tx_buf = write_addr,
		.len = 2,
	};
	struct spi_transfer t = {
		.tx_buf = read_value,
		.rx_buf = result,
		.len = 2,
	};

	DEBUG_PRINT("%s read_len = %d", __func__, read_len);

	spi = eh570->spi;

	/*Set address*/
	write_addr[1] = addrval;
	/*Start to read*/
	spi_message_init(&m);
	spi_message_add_tail(&t_set_addr, &m);
	spi_message_add_tail(&t, &m);
	status = spi_sync(spi, &m);
	if (status < 0) {
		//pr_err(KERN_ERR "%s read data error status = %d\n"
		//		, __func__, status);
		goto out;
	}

	val = result[1];


#ifdef EH570_SPI_DEBUG
	//printk("%s address = %d buf = %d", __func__, addr, val);
#endif
	if (copy_to_user((u8 __user *) (uintptr_t) buf, &val, read_len)) {
		pr_err(KERN_ERR "%s buffer copy_to_user fail status", __func__);
		status = -EFAULT;
		goto out;
	}
out:
	if (write_addr != NULL)
		kfree(write_addr);
	if (read_value != NULL)
		kfree(read_value);
	if (result != NULL)
		kfree(result);

	return status;
}

/*
 * Write data to register
 */
int eh570_io_write_register(struct eh570_data *eh570, u8 *buf)
{
	ssize_t status = 0;
	struct spi_device *spi;
	int write_len = 2;
	struct spi_message m;

	u8 *write_addr = kzalloc(2, GFP_KERNEL);
	write_addr[0] = EH570_WRITE_ADDRESS;
	write_addr[1] = 0x00;
	u8 *write_value = kzalloc(2, GFP_KERNEL);
	write_value[0] = EH570_WRITE_DATA;
	write_value[1] = 0x00;
	u8 *val = kzalloc(write_len, GFP_KERNEL);

	if (copy_from_user(val, (const u8 __user *) (uintptr_t) buf
		, write_len)) {
		pr_err(KERN_ERR "%s buffer copy_from_user fail", __func__);
		status = -EFAULT;
		goto out;
	}

	struct spi_transfer t1 = {
		.tx_buf = write_addr,
		.len = 2,
	};
	struct spi_transfer t2 = {
		.tx_buf = write_value,
		.len = 2,
	};

	DEBUG_PRINT("%s write_len = %d", __func__, write_len);

#ifdef EH570_SPI_DEBUG
	//DEBUG_PRINT("%s address = %x data = %x", __func__, val[0], val[1]);
#endif

	spi = eh570->spi;

	/*Set address*/
	write_addr[1] = val[0];
	/*Set value*/
	write_value[1] = val[1];
	spi_message_init(&m);
	spi_message_add_tail(&t1, &m);
	spi_message_add_tail(&t2, &m);
	status = spi_sync(spi, &m);
	if (status < 0) {
	//	pr_err(KERN_ERR "%s read data error status = %d",
		//		__func__, status);
		goto out;
	}

out:
	if (write_addr != NULL)
		kfree(write_addr);
	if (write_value != NULL)
		kfree(write_value);
	if (val != NULL)
		kfree(val);
	return status;
}

int eh570_read_register(struct eh570_data *eh570, u8 addr, u8 *buf)
{
	ssize_t	status;
	struct spi_device *spi;
	struct spi_message m;

	/*Set address*/
	u8 *write_addr = kzalloc(2, GFP_KERNEL);
	write_addr[0] = EH570_WRITE_ADDRESS;
	write_addr[1] = addr;
	u8 *read_value = kzalloc(2, GFP_KERNEL);
	read_value[0] = EH570_READ_DATA;
	read_value[1] = 0x00;
	u8 *result = kzalloc(2, GFP_KERNEL);
	result[0] = 0xFF;
	result[1] = 0xFF;

	struct spi_transfer t1 = {
		.tx_buf = write_addr,
		.len = 2,
	};
	struct spi_transfer t2 = {
		.tx_buf = read_value,
		.rx_buf	= result,
		.len = 2,
	};

	spi = eh570->spi;
	spi_message_init(&m);
	spi_message_add_tail(&t1, &m);
	spi_message_add_tail(&t2, &m);
	status = spi_sync(spi, &m);

	if (status == 0) {
		*buf = result[1];
		printk("eh570_read_register address = %x result = %x %x\n"
					, addr, result[0], result[1]);
	} else
		printk("read data error status =\n");

out:
	if (write_addr != NULL)
		kfree(write_addr);
	if (read_value != NULL)
		kfree(read_value);
	if (result != NULL)
		kfree(result);
	return status;
}

int eh570_io_get_one_image(
	struct eh570_data *eh570,
	u8 *buf,
	u8 *image_buf
	)
{
	uint8_t read_val;
	int32_t status;
	uint32_t fail_count = 0;
	uint8_t *tx_buf = (uint8_t *)buf;
	uint32_t read_count;
	uint8_t *work_buf = NULL;
	DEBUG_PRINT("%s", __func__);

	uint8_t *val = kzalloc(6, GFP_KERNEL);
	if (val == NULL)
		return -ENOMEM;

	if (copy_from_user(val, (const u8 __user *) (uintptr_t) tx_buf, 6)) {
		//pr_err(KERN_ERR "%s buffer copy_from_user fail", __func__);
		status = -EFAULT;
		goto end;
	}
	read_count = val[0] * val[1];          /* total pixel , width * hight */

	while (1) {
		status = eh570_read_register
				(eh570, FSTATUS_EH570_ADDR, &read_val);
		if (status < 0)
			goto end;

		if (read_val & FRAME_READY_MASK)
			break;

		if (fail_count >= 250) {
		//	pr_err(KERN_ERR "fail_count = %d", fail_count);
			status = -ETIME;
			goto end;
		}
		fail_count++;
	}

	work_buf = kzalloc(read_count, GFP_KERNEL);
	if (work_buf == NULL) {
		status = -ENOMEM;
		goto end;
	}

	status = eh570_mass_read(eh570, FDATA_EH570_ADDR, work_buf, read_count);
	if (status < 0) {
		//pr_err(KERN_ERR "%s call eh570_mass_read error status = %d\n "
			//	, __func__, status);
		goto end;
	}

	if (copy_to_user((u8 __user *) (uintptr_t) image_buf,
		work_buf, read_count)) {
		//pr_err(KERN_ERR "buffer copy_to_user fail status = %d", status);
		status = -EFAULT;
	}
end:
	kfree(val);
	kfree(work_buf);
	return status;
}



