/*
 * mytimer.h
 *
 *  Created on: 18 May 2021
 *      Author: koren
 */

#ifndef MYSOURCEFILES_INC_MYTIMER_H_
#define MYSOURCEFILES_INC_MYTIMER_H_

#include "em_timer.h"
#include "kijelzo.h"
#include "em_cmu.h"

/* With 1024 Prescale, f(timer) = 13.672 kHz */
#define TIMER1_FREQ 13.672;

volatile _Bool timerflag;
volatile _Bool mydelayflag;

void myTimer1_Init(void);

//void myDelay_ms(uint32_t ms);


#endif /* MYSOURCEFILES_INC_MYTIMER_H_ */
