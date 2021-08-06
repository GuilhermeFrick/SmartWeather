
#include "FreeRTOS.h"
#include "task.h"
#include "BlinkTask.h"
#include "SensorTask.h"
#include "uc_driver.h"
#include "gpio_driver.h"
#include "WrapperRTOS.h"


int main(void)
{
  UcDriverInit();
  BlinkTaskCreate();
  SensorTaskCreate();
  vTaskStartScheduler();
  while (1)
  {
  }
}
