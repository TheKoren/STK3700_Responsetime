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
#include "inttypes.h"

#include "FreeRTOS.h" // Ez legyen az első FreeRTOS header
#include "task.h"
#include <em_timer.h>
#include "semphr.h"
#include "mytimer.h"


uint32_t reactiontick;

typedef enum {IDLE, RUNNING, DONE} phase;

typedef struct ProgramControl {
  phase phase;
  uint32_t reactiontime;
  uint16_t lightsensor;
  uint32_t seed;
}ProgramControl;

ProgramControl Control = {IDLE, 0, 0,0};

static void prvTaskLightSensor(void *pvParam)
{
  while(1)
  {
      I2C_Work();
      vTaskDelay((10 * configTICK_RATE_HZ) / 1000); // Sleep for 10ms
  }
}

SemaphoreHandle_t reactmeasure;


uint32_t tick0;

static void prvTaskStart(void *pvParam)
{
  if((startcode == 'f') & (Control.phase == IDLE) )
    {
      startcode = '0';
      Control.phase = RUNNING;
      Control.seed = TIMER_CounterGet(TIMER1);
      srand(Control.seed);
      xSemaphoreGive(reactmeasure);
    }
  startcode = '0';
  vTaskDelay(configTICK_RATE_HZ/1000);
}

static void prvTaskReaction(void *pvParam)
{
  while (1)
    {
      xSemaphoreTake(reactmeasure, portMAX_DELAY);
      if (Control.phase == RUNNING)
        {
          kijelzoCountdown ();
          tick0 = xTaskGetTickCount ();
          while (1);  // Waiting for button press
        }
    }

}
SemaphoreHandle_t processdata;

static void prvTaskProcessData(void *pvParam)
{
  while(1)
    {
      xSemaphoreTake(processdata, portMAX_DELAY);
      /*
       * Send Data to UART. Valid Data stored in Control structure
       */
      if(Control.phase == DONE)
        {
          /*
           * Code...
           */
          printf("%u", Control.lightsensor);
          printf("\n");
          printf("%lu", Control.reactiontime);
          printf("\n");
          printf("\n");
          Control.phase = IDLE;
        }
    }
}

static void prvTaskLCD(void *pvParam)
{
  while(1)
    {
      if(Control.phase == DONE)
        {
          SegmentLCD_LowerNumber(Control.lightsensor);
          SegmentLCD_Number(Control.reactiontime);
        }
      vTaskDelay(configTICK_RATE_HZ);
    }
}

void GPIO_ODD_IRQHandler (void)
{
  BaseType_t xSwitchRequired;
  GPIO_IntClear (1 << 9);
  if (Control.phase == RUNNING)
    {
      Control.lightsensor = luxvalue;
      Control.reactiontime = xTaskGetTickCountFromISR () - tick0;
      Control.phase = DONE;

      xSemaphoreGiveFromISR(processdata, /*NULL*/&xSwitchRequired);
      portYIELD_FROM_ISR(xSwitchRequired);
    }
}

void app_init(void)
{
  kijelzoInit();
  uartInit();
  myI2C_Init();
  myTimer1_Init();

  xTaskCreate
   (
       prvTaskLightSensor,
       "LightSensor",
       configMINIMAL_STACK_SIZE,
       NULL,
       tskIDLE_PRIORITY + 2,
       NULL
   );

  reactmeasure = xSemaphoreCreateBinary();

  xTaskCreate
   (
       prvTaskStart,
       "Start",
       configMINIMAL_STACK_SIZE,
       NULL,
       tskIDLE_PRIORITY + 3,
       NULL
   );

  xTaskCreate
   (
       prvTaskReaction,
       "Reaction",
       configMINIMAL_STACK_SIZE,
       NULL,
       tskIDLE_PRIORITY + 1,
       NULL
   );

  processdata = xSemaphoreCreateBinary();

  xTaskCreate
  (
      prvTaskProcessData,
      "ProcessData",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 1,
      NULL
  );

  xTaskCreate
  (
      prvTaskLCD,
      "LCD",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 2,
      NULL
  );

  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  GPIO_ExtIntConfig(gpioPortB, 9, 9, false, true, true);

  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  vTaskStartScheduler();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
