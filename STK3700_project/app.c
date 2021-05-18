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
#include <em_timer.h>
#include "FreeRTOS.h" // Ez legyen az első FreeRTOS header
#include "task.h"
#include "mytasks.h"

static void prvTaskLightSensor(void *pvParam)
{
  while(1){
  I2C_Work();}
  vTaskDelete(NULL); // Aktuális taszk törlése
}

void app_init(void)
{
  I2C_Initfasz();

  xTaskCreate
  (
      prvTaskLightSensor,
      "LightSensor",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 1,
      NULL
  );

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
