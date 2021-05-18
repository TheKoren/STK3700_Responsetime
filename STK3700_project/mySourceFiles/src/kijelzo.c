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
#include "ustimer.h"

/* private variables */
static SegmentLCD_BlockMode_TypeDef fullRow[SEGMENT_LCD_NUM_BLOCK_COLUMNS] = {BLOCKMODE_ARG_FULL};
//static SegmentLCD_BlockMode_TypeDef emptyRow[SEGMENT_LCD_NUM_BLOCK_COLUMNS] = {BLOCKMODE_ARG_EMPTY};



/* Function implementations */
void kijelzoInit(void)
{
    SegmentLCD_Init(false);
    SegmentLCD_AllOff();
}

void kijelzoGeckoSign(int on)
{
    SegmentLCD_Symbol(LCD_SYMBOL_GECKO, on);
}

void kijelzoCountdown(void)
{
    SegmentLCD_LowerNumber(3);
    USTIMER_Delay(S_FOR_DELAY);
    SegmentLCD_LowerNumber(2);
    USTIMER_Delay(S_FOR_DELAY);
    SegmentLCD_LowerNumber(1);
    USTIMER_Delay(MS_FOR_DELAY);

    //TODO: call random number generator and delay that much
    USTIMER_Delay(S_FOR_DELAY);

    SegmentLCD_Block(fullRow, fullRow);
}

void kijelzoBlinkGecko(void)
{
    kijelzoGeckoSign(1);
    USTIMER_Delay(500000);
    kijelzoGeckoSign(0);
    USTIMER_Delay(500000);

}

void kijelzoTestLoop(void)
{
    kijelzoCountdown();
    USTIMER_Delay(5 * S_FOR_DELAY);
}

void kijelzoTestSetup(void)
{
    USTIMER_Init();
    kijelzoInit();
    kijelzoGeckoSign(1);
}

void kijelzoPrint(const char *str)
{
    SegmentLCD_Write(str);
}

