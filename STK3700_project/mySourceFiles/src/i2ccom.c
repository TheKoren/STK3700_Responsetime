/*
 * i2ccom.c
 *
 *  Created on: 15 May 2021
 *      Author: koren
 */

/* Includes */
#include "i2ccom.h"

/* Global variables */

I2C_TransferSeq_TypeDef seq;
uint8_t data_buf_0[3];
uint8_t data_buf_1[2];

/* Function implementations */

void myI2C_Init(void)
{
    SegmentLCD_Init(false);
    I2CSPM_Init_TypeDef i2cspm_init = I2CSPM_INIT_DEFAULT;

     i2cspm_init.port = I2C1;
     i2cspm_init.portLocation = 0;
     i2cspm_init.sdaPort = gpioPortC;
     i2cspm_init.sdaPin = 4;
     i2cspm_init.sclPort = gpioPortC;
     i2cspm_init.sclPin = 5;

     CMU_ClockEnable(cmuClock_GPIO, true);
     I2CSPM_Init(&i2cspm_init);

     seq.addr = 0x10 << 1;
     seq.flags = I2C_FLAG_WRITE;
     seq.buf[0].len = 3;
     seq.buf[0].data = data_buf_0;
     data_buf_0[0] = 0x00; // Access Conf reg
     data_buf_0[1] = 0x00; // LSB
     data_buf_0[2] = 0x00; // MSB

     I2CSPM_Transfer(I2C1, &seq);

     seq.flags = I2C_FLAG_WRITE_READ;
     seq.buf[0].len = 1;
     data_buf_0[0] = 0x04;

     seq.buf[1].len = 2;
     seq.buf[1].data = data_buf_1;
}

void I2C_Work(void)
{
  I2CSPM_Transfer(I2C1, &seq);
  luxvalue = 256 * data_buf_1[1] + data_buf_1[0];
}

