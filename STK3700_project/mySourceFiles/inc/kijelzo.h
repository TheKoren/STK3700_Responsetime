/*
 * kijelzo.h
 *
 *
 *
 */

#ifndef MYSOURCEFILES_INC_KIJELZO_H_
#define MYSOURCEFILES_INC_KIJELZO_H_

/* Includes */
#include "segmentlcdconfig.h"
#include "segmentlcd.h"
#include <stdlib.h>


/* Function declarations */

void kijelzoInit(void);

void kijelzoGeckoSign(int on);

void kijelzoCountdown(void);

void kijelzoBlinkGecko(void);

void kijelzoTestLoop(void);

void kijelzoTestSetup(void);

void kijelzoPrint(const char *str);


#endif /* MYSOURCEFILES_INC_KIJELZO_H_ */
