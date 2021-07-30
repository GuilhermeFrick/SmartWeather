/**
 * @file WrapperRTOS.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "WrapperRTOS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/*!
 *  \brief      Allocate memory in the indicated pointer, with timeout
 *  \param[in]  WantedSize: Size of the buffer to be allocated in bytes
 *  \param[in]  timeout: Timeout to allocate memory in ms
 *  \return     On success, a pointer to the memory block allocated by the function. \n
 *              If the function failed to allocate the requested block of memory, a null pointer is returned.
 */
void *RTOSMallocUntil(size_t WantedSize, uint32_t timeout)
{
    uint8_t *buffer;
    uint32_t tryOuts = timeout / 10;

    buffer = pvPortMalloc(WantedSize);
    while (buffer == NULL)
    {
        if (tryOuts-- == 0)
            break;
        vTaskDelay(1);
        buffer = pvPortMalloc(WantedSize);
    }

    return buffer;
}
/*!
 *  \brief      Allocates a block of size bytes of memory
 *  \param[in]  WantedSize: Size of the memory block, in bytes
 *  \return     On success, a pointer to the memory block allocated by the function. \n
 *              If the function failed to allocate the requested block of memory, a null pointer is returned.
 */
void *RTOSMalloc(size_t WantedSize)
{
    return pvPortMalloc(WantedSize);
}
/*!
 *  \brief      Free memory allocated on the buffer pointer
 *  \param[in]  buffer: pointer to a memory block previously allocated
 */
void RTOSFree(void *buffer)
{
    if (buffer != NULL)
    {
        vPortFree(buffer);
    }
}
/*!
 *  \brief      Wait for Timeout (Time Delay)
 *  \param[in]  millisec: time delay value in ms
 */
void RTOSDelay(uint32_t millisec)
{
    if (millisec >= TICK_VALUE_MS)
        vTaskDelay(pdMS_TO_TICKS(millisec));
    else
        vTaskDelay(1);
}
/*!
 *  \brief      Returns the current value of the RTOS tick timer in 10 ms
 *  \return     Tick value
 */
uint32_t RTOSGetTick(void)
{
    return xTaskGetTickCount();
}
/*!
 *  \brief      Returns the current value of the RTOS tick timer in ms
 *  \return     Tick value
 */
uint32_t RTOSGetTimeMs(void)
{
    return TICK_TO_MS(xTaskGetTickCount());
}
/*!
 *  \brief      Creates a mutex to handle critical sessions
 *  \return  	If the mutex was created successfully then a handle to the created mutex is returned.
 *              If the mutex was not created then NULL is returned.
 */
void *RTOSMutexCreate(void)
{
    return xSemaphoreCreateMutex();
}
/*!
 *  \brief      Obtain a mutex
 *  \param[in]  Mutex: a handle to the semaphore being taken - obtained when the mutex was created.
 *  \return  	Result of operation
 *  \retval     true: mutex successfully obtained
 *  \retval     false: error in obtaining the mutex
 *  \warning    Stronger function must be declared externally
 */
bool RTOSMutexTake(void *Mutex)
{
    if (Mutex == NULL)
    {
        return false;
    }
    return (xSemaphoreTake(Mutex, (TickType_t)200) == pdTRUE);
}
/*!
 *  \brief      Release a mutex
 *  \param[in]  Mutex: a handle to the semaphore being released - obtained when the semaphore was created.
 *  \return  	Result of operation
 *  \retval     true: mutex successfully released
 *  \retval     false: mutex release error
 *  \warning    Stronger function must be declared externally
 */
bool RTOSMutexGive(void *Mutex)
{
    if (Mutex == NULL)
    {
        return false;
    }
    return (xSemaphoreGive(Mutex) == pdTRUE);
}
/*!
 *  \brief  Request a context switch to another task
 */
void RTOSYield(void)
{
    taskYIELD();
}
/*!
 *  \brief  Enter a critical code session
 */
void RTOSEnterCritical(void)
{
    taskENTER_CRITICAL();
}
/*!
 *  \brief  Exit from critical code session
 */
void RTOSExitCritical(void)
{
    taskEXIT_CRITICAL();
}
/*!
 *  \brief 		Function that calculates the elapsed time from an initial time (use \ref RTOSGetTimeMs)
 *	\param[in]  InitialTime: Initial time for calculation
 *  \return 	Elapsed time from initial time in milliseconds
 *  \note       This function corrects the error caused by overflow
 */
uint32_t RTOSGetElapsedTime(uint32_t InitialTime)
{
    uint32_t actualTime;
    actualTime = RTOSGetTimeMs();

    if (InitialTime <= actualTime)
        return (actualTime - InitialTime);
    else
        return (((0xFFFFFFFFUL) - InitialTime) + actualTime);
}

/*!
 *  \brief 		Function that calculates the elapsed time from an initial time (use \ref RTOSGetTimeMs)
 *	\param[in]  InitialTime: Initial time for calculation
 *  \return 	Elapsed time from initial time in milliseconds
 *  \note       This function corrects the error caused by overflow
 */
uint32_t RTOSGetElapsedTimeFromISR(uint32_t InitialTime)
{
    uint32_t actualTime;
    actualTime = (uint32_t)xTaskGetTickCountFromISR();

    if (InitialTime <= actualTime)
        return (actualTime - InitialTime);
    else
        return (((0xFFFFFFFFUL) - InitialTime) + actualTime);
}

uint32_t RTOSGetTickFromISR(void)
{
    return (uint32_t)xTaskGetTickCountFromISR();
}
