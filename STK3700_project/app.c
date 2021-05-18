/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include "em_gpio.h"
#include <i2ccom.h>
#include "uart.h"
#include "kijelzo.h"
#include <stdio.h>

#include "FreeRTOS.h" // Ez legyen az első FreeRTOS header
#include "task.h"
#include <em_timer.h>
#include "semphr.h"
#include "mytimer.h"


uint32_t reactiontick;

static void prvTaskLightSensor(void *pvParam)
{
  while(1){
  //I2C_Work();
  }
  vTaskDelete(NULL); // Aktuális taszk törlése
}
SemaphoreHandle_t btnPressed;

static void prvTaskBtn(void *pvParam)
{
  while(1)
    {
      xSemaphoreTake(btnPressed, portMAX_DELAY);
      SegmentLCD_Number(reactiontick*0.07314);
    }
}

void GPIO_ODD_IRQHandler(void)
{
  BaseType_t  xSwitchRequired;
  GPIO_IntClear(1 << 9);

  reactiontick = TIMER_CounterGet(TIMER1);
  TIMER_Enable(TIMER1, false);
  TIMER_CounterSet(TIMER1, 0);
  xSemaphoreGiveFromISR(btnPressed, /*NULL*/ &xSwitchRequired);
  portYIELD_FROM_ISR(xSwitchRequired);
}

void app_init(void)
{
  kijelzoInit();
  uartInit();
  kijelzoPrint("Hello!");
  printf("Hello World\r\n");
  myI2C_Init();
  myTimer1_Init();

  xTaskCreate
  (
      prvTaskLightSensor,
      "LightSensor",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 1,
      NULL
  );
  btnPressed = xSemaphoreCreateBinary();

  xTaskCreate
  (
      prvTaskBtn,
      "Btn",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 1,
      NULL
  );

  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  GPIO_ExtIntConfig(gpioPortB, 9, 9, false, true, true);

  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  TIMER_Enable(TIMER1, true);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
