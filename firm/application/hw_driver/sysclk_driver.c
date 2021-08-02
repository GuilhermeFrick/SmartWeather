/**
 * @file sysclk_driver.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 
#include <stdint.h>
#include <stdio.h>
#include "sysclk_driver.h"
#include "sysctl.h"
#include "systick.h"
#include "interrupt.h"

volatile uint32_t g_ui32Counter = 0;

/**
 * @brief 
 * 
 */
void SystemClockInit(void)
{
    

    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); 
    
		//SysTickPeriodSet(SysCtlClockGet()); 
    //IntMasterEnable();
    //SysTickIntEnable();
    //SysTickEnable(); 
    //g_ui32Counter=0;

}
/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t SystemCoreClock(void)
{
    return SysCtlClockGet();
}
/**
 * @brief 
 * 
 * @param config 
 */
void SystemDeepSleepConfig(uint32_t config)
{
    SysCtlDeepSleepClockSet(config);
}


