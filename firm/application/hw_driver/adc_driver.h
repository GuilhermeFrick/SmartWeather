/**
 * @file adc_driver.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H
#include <stdint.h>
/*!
 *  \brief Return codes of the functions
 */
typedef enum
{
    ADC_RET_OK           = 0,  /**<The function executed successfully*/
    ADC_RET_ERROR        = -1, /**<ADC handler error*/
    ADC_RET_ERR_INIT     = -2, /**<Error initializing ADC interface*/
    ADC_MUTEX_NULL       = -3, /**<Mutex was not created*/
    ADC_MUTEX_TAKE_ERROR = -4, /**<Error taking the mutex*/
    ADC_MUTEX_GIVE_ERROR = -5, /**<Error giving the mutex*/
} adc_ret_e;

adc_ret_e ADC_Init(void);
//adc_ret_e ADC_Deinit(void);
//uint32_t  ADC_GetChannelCount(void);
adc_ret_e ADC_GetTemp(uint32_t *temperature);
//adc_ret_e ADC_GetVref(float *voltage);
//adc_ret_e ADC_GetVbat(float *voltage);
adc_ret_e ADC_GetRaw(uint8_t channel, uint32_t *raw_value);
adc_ret_e ADC_GetVoltage(uint8_t channel, float *voltage);

/** @}*/ // End of ADC
#endif   /*__ __ADC_H */
