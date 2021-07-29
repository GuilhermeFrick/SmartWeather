/**
 * @file uart_driver.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _UART_DRIVER_H
#define _UART_DRIVER_H
#include <stdint.h>
void Uart0Init(void);
void UartSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
#endif
