#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio_driver.h"
#include "sysctl.h"
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int
main(void)
{
    volatile uint32_t ui32Loop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GpioInitOutput(PORTF,PIN3);

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //
				GpioWritePin(PORTF,PIN3,GPIO_SET);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 2000; ui32Loop++)
        {
        }

        //
        // Turn off the LED.
        //
        GpioWritePin(PORTF,PIN3,GPIO_RESET);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}
