/**
 * @file adc_driver.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "hw_memmap.h"
#include "adc.h"
#include "pin_map.h"
#include "sysctl.h"
#include "WrapperRTOS.h"
#include "adc_driver.h"

void *ADCMutex = NULL; /**<Mutex to handle global values*/
static adc_ret_e  ADC_ReadChannel(uint32_t channel, uint32_t *ad_value);
const float RESOL_ADC = 4096.0F; /**< ADC resolution = 12 bits */
const float VREF_ADC  = 3.3F;    /**< ADC reference voltage  3.3V */
adc_ret_e ADC_Init(void)
{
    adc_ret_e            ret       = ADC_RET_OK;
    
    do
    {		
        
        if (ADCMutex == NULL)
        {
            ADCMutex = RTOSMutexCreate();
        }
        if (ADCMutex == NULL)
        {
            ret = ADC_MUTEX_NULL;
            break;
        }
		
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
        ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
                            
        ADCSequenceEnable(ADC0_BASE, 3); 
        ADCIntClear(ADC0_BASE, 3);

        
    } while (0);

    return ret;
}

adc_ret_e ADC_GetRaw(uint8_t channel, uint32_t *raw_value)
{
    adc_ret_e ret = ADC_RET_ERROR;
    uint32_t  ad_raw_value;

    do
    {
        
        if (ADC_ReadChannel(1, &ad_raw_value) != ADC_RET_OK)
        {
            break;
        }

        *raw_value = ad_raw_value;

        ret = ADC_RET_OK;

    } while (0);

    return ret;
}
/**
 * @brief 
 * 
 * @param channel 
 * @param voltage 
 * @return adc_ret_e 
 */
adc_ret_e ADC_GetVoltage(uint8_t channel, float *voltage)
{
    adc_ret_e ret = ADC_RET_ERROR;
    uint32_t  ad_raw_value;

    do
    {
        if (ADC_ReadChannel(1, &ad_raw_value) != ADC_RET_OK)
        {
            break;
        }

        *voltage = (((float)ad_raw_value * VREF_ADC) / RESOL_ADC);

        ret = ADC_RET_OK;

    } while (0);

    return ret;
}

/**
 * @brief 
 * 
 * @param channel 
 * @param ad_value 
 * @return adc_ret_e 
 */
static adc_ret_e ADC_ReadChannel(uint32_t channel, uint32_t *ad_value)
{
    adc_ret_e              ret     = ADC_RET_OK;
    do
    {
	 
		if (ADCMutex == NULL)
        {
            ret = ADC_MUTEX_NULL;
            break;
        }
        if (!RTOSMutexTake(ADCMutex))
        {
            ret = ADC_MUTEX_TAKE_ERROR;
            break;
        }
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, ad_value);
       
        if(!RTOSMutexGive(ADCMutex))
        {
            ret = ADC_MUTEX_GIVE_ERROR;
            break;
        }
        ret = ADC_RET_OK;
        
    } while (0);

    return ret;
    
}

/**
 * @brief 
 * 
 * @param temperature 
 * @return adc_ret_e 
 */
adc_ret_e ADC_GetTemp(uint32_t *temperature)
{
    adc_ret_e ret = ADC_RET_ERROR;
    do
    {
        uint32_t ad_raw_value[1];
			
        if (ADC_ReadChannel(1,  ad_raw_value) != ADC_RET_OK)
        {
            break;
        }
        
         *temperature = ((1475 * 1023) - (2250 * ad_raw_value[0])) / 10230;

        ret = ADC_RET_OK;
    } while (0);

    return ret;
}
