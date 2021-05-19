/*
 * kijelzo.c
 *
 *
 *
 */

/* Defines */
#define BLOCKMODE_ARG_EMPTY 0,0,0,0,0,0,0
#define BLOCKMODE_ARG_FULL 3,3,3,3,3,3,3
#define MS_FOR_DELAY 1000
#define S_FOR_DELAY (1000*MS_FOR_DELAY)


/* Includes */
#include "kijelzo.h"

/* private variables */
static SegmentLCD_BlockMode_TypeDef fullRow[SEGMENT_LCD_NUM_BLOCK_COLUMNS] = {BLOCKMODE_ARG_FULL};
static SegmentLCD_BlockMode_TypeDef emptyRow[SEGMENT_LCD_NUM_BLOCK_COLUMNS] = {BLOCKMODE_ARG_EMPTY};



/* Function implementations */
void kijelzoInit(void)
{
    SegmentLCD_Init(true);
    SegmentLCD_AllOff();
}

void kijelzoCountdown(void)
{
    SegmentLCD_AllOff();
    SegmentLCD_LowerNumber(3);
    sl_udelay_wait(S_FOR_DELAY);
    SegmentLCD_LowerNumber(2);
    sl_udelay_wait(S_FOR_DELAY);
    SegmentLCD_LowerNumber(1);
    sl_udelay_wait(S_FOR_DELAY);
    SegmentLCD_Block(emptyRow, emptyRow);
    uint32_t random = rand() %  S_FOR_DELAY*3;
    sl_udelay_wait(random);
    SegmentLCD_Block(fullRow, fullRow);
}

