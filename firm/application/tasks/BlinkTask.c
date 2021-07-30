/**
 *\file BlinkTask.c
 *\author Guilherme Frick de Oliveira (guiherme.oliveira_irede@perto.com.br)
 *\brief {Source file of the BlinkTask}
 *\version 1.0
 *\date 2021-01-19
 *
 *
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BlinkTask.h"
#include "gpio_driver.h"
#include "WrapperRTOS.h"


#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

TaskHandle_t BlinkTaskHandle = NULL; /**<Handle to BlinkTask*/
static void  BlinkLedFunc(void *params);

void BlinkTaskCreate(void)
{

    if (BlinkTaskHandle == NULL)
    {
        GpioClockEnable(PORTF);
        GpioInitLed(PORTF,LED_BLUE);
        GpioWriteLed(PORTF,LED_BLUE,GPIO_SET);
        xTaskCreate(BlinkLedFunc, (char const *)"BlinkLed", BLINK_TASK_STACK_SIZE, NULL, 1, &BlinkTaskHandle);
				
    }
		
}

static void BlinkLedFunc(void *param)
{
  printf("%d TICK:",portTICK_RATE_MS);
	while (1)
    {
				GpioWriteLed(PORTF,LED_BLUE,GPIO_SET);
        RTOSDelay(10);
        GpioWriteLed(PORTF,LED_BLUE,GPIO_RESET);
        RTOSDelay(10);
    }
}
