/**
 * @file syclk_driver.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _SYSCLK_DRIVER_H
#define _SYSCLK_DRIVER_H

#include <stdint.h>

void SystemClockInit(void);
uint32_t SystemCoreClock(void);


#endif
