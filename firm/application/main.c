#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "uart_driver.h"
#include "gpio_driver.h"
#include "uc_driver.h"
#include  "sysclk_driver.h"





int main(void)
{
    volatile uint32_t ui32Loop;

    UcDriverInit();

    GpioClockEnable(PORTF);
    GpioInitLed(PORTF,LED_BLUE);
   

    while(1)
    {
 
		GpioWriteLed(PORTF,LED_BLUE,GPIO_SET);

        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
        printf("clock:%d\n\r",SystemCoreClock());
				GpioWriteLed(PORTF,LED_BLUE,GPIO_RESET);

        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
		
    }
}
