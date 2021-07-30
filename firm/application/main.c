
#include "FreeRTOS.h"
#include "task.h"
#include "BlinkTask.h"
#include "uc_driver.h"
#include "gpio_driver.h"
#include "WrapperRTOS.h"


int main(void)
{
  UcDriverInit();
  BlinkTaskCreate();
  vTaskStartScheduler();
  while (1)
  {
  }
}
