/*
 * mytimer.c
 *
 *  Created on: 18 May 2021
 *      Author: koren
 */

#include "mytimer.h"

/*
 * Up-count: Counter counts up until it reaches the value in TIMERn_TOP, where it is reset to 0 before
counting up again.
 */
static TIMER_Init_TypeDef TIMER1_init = TIMER_INIT_DEFAULT;

/*  If HFPERCLK is 14MHz:
 *      14 MHz / 1024 (prescale) = 13.67 kHz
 *      ----> 0.07314 ms ---> 73.14 us is 1 tick
 *  for 1 second, 13672 tick needed
 *  for 0.5 second, 6836 tick needed
 *  ...
 */
void myTimer1_Init(void){

  CMU_ClockEnable(cmuClock_TIMER1, true);

  TIMER1_init.enable      = false;
  TIMER1_init.debugRun    = false;
  TIMER1_init.prescale    = timerPrescale1024;
  TIMER1_init.clkSel      = timerClkSelHFPerClk;
  TIMER1_init.count2x     = false;
  TIMER1_init.ati         = false;
  TIMER1_init.fallAction  = timerInputActionNone;
  TIMER1_init.riseAction  = timerInputActionStart;
  TIMER1_init.mode        = timerModeUp;
  TIMER1_init.dmaClrAct   = true;
  TIMER1_init.quadModeX4  = false;
  TIMER1_init.oneShot     = true;   // count up only once
  TIMER1_init.sync        = false;

  TIMER_Init(TIMER1, &TIMER1_init);

  TIMER_TopBufSet(TIMER1, 60000); // Joval tobb mint 1-2 másodperc

  /*
   * HASZNALANDO:
   * TIMER_CounterGet(TIMER1); // Timerből a cucc kiszedése
   * TIMER_CounterSet(TIMER1, 0); // Timer lenullázása
   * TIMER_Enable(TIMER1, true);  // Timer elindítása
   * TIMER_Enable(TIMER1, false); // Timer stop
   *
   */

};
