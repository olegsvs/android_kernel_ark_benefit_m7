/*
 FILENAME
      eh570_data_transfer.h - FringerPrint Sensor operation header.
      $PATH:
 FUNCTIONAL DESCRIPTION.
      FingerPrint Sensor transfer function description.

 MODIFICATION HISTORY.
 NOTICE.
      Copyright (C) 2000-2014 EgisTec All Rights Reserved.
*/
#ifndef _EH570_DATA_TRANSFER_H_
#define _EH570_DATA_TRANSFER_H_

#include "eh570.h"

int eh570_io_read_register(struct eh570_data *eh570, u8 *addr, u8 *buf);
int eh570_io_write_register(struct eh570_data *eh570, u8 *buf);
int eh570_io_get_one_image(struct eh570_data *eh570, u8 *buf, u8 *image_buf);
int eh570_read_register(struct eh570_data *eh570, u8 addr, u8 *buf);
int eh570_mass_read(struct eh570_data *eh570, u8 addr, u8 *buf, int read_len);
#endif
