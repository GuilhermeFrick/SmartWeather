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
    ADC_CHANNEL_INVALID  = -3,
    ADC_MUTEX_NULL       = -4, /**<Mutex was not created*/
    ADC_MUTEX_TAKE_ERROR = -5, /**<Error taking the mutex*/
    ADC_MUTEX_GIVE_ERROR = -6, /**<Error giving the mutex*/
} adc_ret_e;

typedef enum
{
    ADC0 = 0x00000000,
    ADC1 = 0x00000001,
}adc_port_e;

typedef enum
{
    AIN0,
    AIN1,
    AIN2,
    AIN3,
    AIN4,
    AIN5,
    AIN6,
    AIN7,
    AIN8,
    AIN9,
    AIN10,
    AIN11,
    TEMP_SENSOR,
    AIN_QTY
}adc_channel_list_e;

typedef enum
{
    SINGLE_ENDED = 0,
    DIFERENTIAL,
    TEMP_INT_SENSOR,
}adc_mode_e;

typedef enum
{
    ADC_SS0 =0,
    ADC_SS1,
    ADC_SS2,
    ADC_SS3
}adc_seq_e;

typedef enum
{
    ADC_DISABLED = 0,
    ADC_ENABLED 
}adc_enable_e;

adc_ret_e ADC_Init(void);
//adc_ret_e ADC_Deinit(void);
//uint32_t  ADC_GetChannelCount(void);
adc_ret_e ADC_GetTemp(uint32_t *temperature);
//adc_ret_e ADC_GetVref(float *voltage);
//adc_ret_e ADC_GetVbat(float *voltage);
adc_ret_e ADC_GetRaw(adc_channel_list_e channel, uint32_t *raw_value_ad);
adc_ret_e ADC_GetVoltage(adc_channel_list_e channel, float *voltage);
adc_ret_e ADC_ReadProcess(void);

/** @}*/ // End of ADC
#endif   /*__ __ADC_H */
