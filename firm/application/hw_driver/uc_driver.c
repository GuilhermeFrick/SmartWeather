/**
 * @file uc_driver.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "gpio_driver.h"
#include "sysclk_driver.h"
#include "uart_driver.h"
#include "adc_driver.h"

struct uc_driver_info_s
{
    uint32_t sys_clock;
    uint8_t  uart_qty;
    uint8_t  adc_qty;
    uint8_t  i2c_qty;
}UC_DRIVER = {
    .sys_clock = 0UL,
    .uart_qty = 1,
    .adc_qty  = 1,
    .i2c_qty  = 1
};


void UcDriverInit(void)
{
  
	 ADC_Init(); 
	SystemClockInit();
  Uart0Init();
	
		
    UC_DRIVER.sys_clock = SystemCoreClock();
}
