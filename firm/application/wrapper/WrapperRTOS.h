/**
 * @file WrapperRTOS.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/** \addtogroup  WrapperRTOS Wrapper RTOS 
 *  \ingroup Wrapper
 * @{
 */
#ifndef WRAPPER_RTOS_H
#define WRAPPER_RTOS_H
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TICK_VALUE_MS  (1000 / configTICK_RATE_HZ)  /**<Tick value in ms*/
#define TICK_TO_MS(x)  (x*TICK_VALUE_MS)            /**<Macro to convert RTOS ticks in ms*/

extern volatile uint32_t RTOS_debug_tick;

void TickRTOSDebugTimer(uint32_t increment);
void * RTOSMallocUntil (size_t WantedSize, uint32_t timeout);
void * RTOSMalloc (size_t WantedSize);
void RTOSFree(void* buffer);
void RTOSDelay(uint32_t millisec);
uint32_t RTOSGetTick(void);
uint32_t RTOSGetTimeMs(void);
bool RTOSMutexGive(void * Mutex);
bool RTOSMutexTake(void * Mutex);
void * RTOSMutexCreate(void);
void RTOSYield(void);
void RTOSEnterCritical(void);
void RTOSExitCritical(void);
uint32_t RTOSGetElapsedTime (uint32_t InitialTime);
uint32_t RTOSGetElapsedTimeFromISR(uint32_t InitialTime);
uint32_t RTOSGetTickFromISR(void);

#endif //WRAPPER_RTOS_H
/** @}*/ //WrapperRTOS
