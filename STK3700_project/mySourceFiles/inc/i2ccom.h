/*
 * i2ccom.h
 *
 *  Created on: 15 May 2021
 *      Author: koren
 */

#ifndef MYSOURCEFILES_INC_I2CCOM_H_
#define MYSOURCEFILES_INC_I2CCOM_H_

/* Includes*/
#include <em_cmu.h>
#include <i2cspm.h>
#include <segmentlcd.h>


/* Function declarations */

void myI2C_Init(void);

void I2C_Work(void);

#endif /* MYSOURCEFILES_INC_I2CCOM_H_ */
