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
#include "kijelzo.h"
#include <stdio.h>
#include "inttypes.h"

#include "FreeRTOS.h" // Ez legyen az első FreeRTOS header
#include "task.h"
#include <em_timer.h>
#include "semphr.h"
#include "mytimer.h"


uint32_t reactiontick;

typedef enum {IDLE, START, RUNNING, DONE} phase;

typedef struct ProgramControl {
  phase phase;
  uint16_t reactiontime;
  uint16_t lightsensor;
  uint32_t seed;
}ProgramControl;
ProgramControl Control;


static void prvTaskLightSensor(void *pvParam)
{
  while(1)
  {
      I2C_Work();
      vTaskDelay((10 * configTICK_RATE_HZ) / 1000); // Sleep for 10ms
  }
}

SemaphoreHandle_t reactmeasure;


uint16_t tick0;

static void prvTaskStart (void *pvParam)
{
  while (1)
    {
      char c;
      c = getchar();
      if ((c == 'f') & (Control.phase == IDLE))
        {
          c = '0';
          Control.phase = START;
          Control.seed = TIMER_CounterGet (TIMER1);
          srand (Control.seed);
          xSemaphoreGive(reactmeasure);
        }
      c = '0';
      vTaskDelay (configTICK_RATE_HZ / 1000);
    }
}

static void prvTaskReaction(void *pvParam)
{
  while (1)
    {
      xSemaphoreTake(reactmeasure, portMAX_DELAY); // Elvileg itt megpusztul, ha egyszer már megkapta
      if (Control.phase == START)
        {
          kijelzoCountdown ();
          Control.phase = RUNNING;
          tick0 = xTaskGetTickCount();
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
          char buf1[5];
          char buf2[5];
          sprintf(buf1,"%05d",Control.lightsensor);
          printf("Measured light: ");
          printf("%s LUX\r\n", buf1);
          sprintf(buf2,"%05d",Control.reactiontime);
          printf("Measured Time: ");
          printf("%s ms\r\n", buf2);
          SegmentLCD_LowerNumber(Control.lightsensor);
          SegmentLCD_Number(Control.reactiontime);
          Control.phase = IDLE;
        }
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
  Control.lightsensor = 0;
  Control.phase = IDLE;
  Control.reactiontime = 0;
  Control.seed = 0;
  kijelzoInit();
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



  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  GPIO_ExtIntConfig(gpioPortB, 9, 9, false, true, true);

  NVIC_EnableIRQ(GPIO_ODD_IRQn);

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
