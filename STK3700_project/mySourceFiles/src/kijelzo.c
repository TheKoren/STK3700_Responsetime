/*
 * kijelzo.c
 *
 *
 *
 */

/* Includes */
#include "kijelzo.h"

/* Function implementations */

void kijelzoInit(void)
{
    SegmentLCD_Init(false);
    SegmentLCD_AllOff();
}

void kijelzoGeckoSign(void)
{
    SegmentLCD_Symbol(LCD_SYMBOL_GECKO, 1);
}
