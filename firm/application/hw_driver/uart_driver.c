/**
 * @file uart_driver.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include "hw_ints.h"
#include "hw_memmap.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "interrupt.h"
#include "pin_map.h"
#include "rom.h"
#include "rom_map.h"
#include "sysctl.h"
#include "uart.h"
#include "uart_driver.h"

void Uart0Init(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_IntMasterEnable();
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    MAP_UARTConfigSetExpClk(UART0_BASE, 
                            MAP_SysCtlClockGet(), 
                            115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    

}

void UARTIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    while(MAP_UARTCharsAvail(UART0_BASE))
    {
        MAP_UARTCharPutNonBlocking(UART0_BASE,
                                   MAP_UARTCharGetNonBlocking(UART0_BASE));
       ///RTOSDelay(1);
       SysCtlDelay(SysCtlClockGet() / (1000 * 3));
    }
}

void UartSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
        
    }
}
