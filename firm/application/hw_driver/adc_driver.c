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

static void *ADCMutex = NULL; /**<Mutex to handle global values*/


const float RESOL_ADC = 4096.0F; /**< ADC resolution = 12 bits */
const float VREF_ADC  = 3.3F;    /**< ADC reference voltage  3.3V */



typedef struct 
{
    adc_channel_list_e  channel_label;
    adc_enable_e enabled;
    uint32_t adc_block;
    uint32_t mode;
    adc_seq_e sequ_num;
    uint32_t trigger_mode;
    uint32_t priority_sample_seq;
    uint32_t adc_step;
    
}adc_driver_config_s;

static uint32_t raw_value[AIN_QTY];


static const adc_driver_config_s ADC_DRIVER_CONFIG [] = {

    {.channel_label = TEMP_SENSOR,.enabled = ADC_ENABLED, .adc_block = ADC0_BASE, .mode = (ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END), .sequ_num = ADC_SS3,.trigger_mode = ADC_TRIGGER_PROCESSOR,.priority_sample_seq = 0,.adc_step =0}, 

};

static uint32_t       ConfiguredAdcsQty = 0; 


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
        if(!RTOSMutexTake(ADCMutex))
        {
            ret = ADC_MUTEX_TAKE_ERROR;
            break;
        }

        ConfiguredAdcsQty = sizeof(ADC_DRIVER_CONFIG) / sizeof(*ADC_DRIVER_CONFIG);
        
        for (uint8_t i= 0 ; i < ConfiguredAdcsQty; i++)
        { 
            adc_channel_list_e channel_label = ADC_DRIVER_CONFIG[i].channel_label;
    
            if(ADC_DRIVER_CONFIG[channel_label].enabled == ADC_ENABLED)
            {
               
                if (ADC_DRIVER_CONFIG[channel_label].adc_block == ADC0_BASE)
                {
                    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
                }
                else if (ADC_DRIVER_CONFIG[channel_label].adc_block == ADC1_BASE)
                {
                    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
                }
                
                ADCSequenceConfigure(ADC_DRIVER_CONFIG[channel_label].adc_block, 
                                    ADC_DRIVER_CONFIG[channel_label].sequ_num,
                                    ADC_DRIVER_CONFIG[channel_label].trigger_mode, 
                                    ADC_DRIVER_CONFIG[channel_label].priority_sample_seq);
                ADCSequenceStepConfigure(ADC_DRIVER_CONFIG[channel_label].adc_block,
                                        ADC_DRIVER_CONFIG[channel_label].sequ_num,
                                        ADC_DRIVER_CONFIG[channel_label].adc_step,
                                        ADC_DRIVER_CONFIG[channel_label].mode);                 
                ADCSequenceEnable(ADC_DRIVER_CONFIG[channel_label].adc_block,ADC_DRIVER_CONFIG[channel_label].sequ_num);
                ADCIntClear(ADC_DRIVER_CONFIG[channel_label].adc_block, ADC_DRIVER_CONFIG[channel_label].sequ_num);
               
            }

            raw_value[channel_label] = 0;

        }
        if (!RTOSMutexGive(ADCMutex))
        {
            ret = ADC_MUTEX_GIVE_ERROR;
            break;
        }
        
    }while (0);
    
    return ret;
}

adc_ret_e ADC_GetRaw(adc_channel_list_e channel, uint32_t *raw_value_ad)
{
    adc_ret_e ret = ADC_RET_OK;
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
        if (channel >=AIN_QTY)
        {
            *raw_value_ad =0;
            ret= ADC_CHANNEL_INVALID;
            break; 
        }
        *raw_value_ad = raw_value[channel];

        if (!RTOSMutexGive(ADCMutex))
        {
            ret = ADC_MUTEX_GIVE_ERROR;
            break;
        }

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
adc_ret_e ADC_GetVoltage(adc_channel_list_e channel, float *voltage)
{
    adc_ret_e ret = ADC_RET_OK;

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
        if (channel >=AIN_QTY)
        {
            *voltage =0.0;
            ret= ADC_CHANNEL_INVALID;
            break; 
        }
        *voltage = (((float)raw_value[channel] * VREF_ADC) / RESOL_ADC);

         if (!RTOSMutexGive(ADCMutex))
        {
            ret = ADC_MUTEX_GIVE_ERROR;
            break;
        }

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
adc_ret_e ADC_ReadProcess(void)
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
        for (uint8_t i= 0 ; i < ConfiguredAdcsQty; i++)
        { 
            adc_channel_list_e channel_label = ADC_DRIVER_CONFIG[i].channel_label;
            
            if(ADC_DRIVER_CONFIG[channel_label].enabled == ADC_ENABLED)
            {
                ADCProcessorTrigger(ADC_DRIVER_CONFIG[channel_label].adc_block,ADC_DRIVER_CONFIG[channel_label].sequ_num);
                while(!ADCIntStatus(ADC_DRIVER_CONFIG[channel_label].adc_block, ADC_DRIVER_CONFIG[channel_label].sequ_num, false))
                {
                }
                ADCIntClear(ADC_DRIVER_CONFIG[channel_label].adc_block,ADC_DRIVER_CONFIG[channel_label].sequ_num);
                ADCSequenceDataGet(ADC_DRIVER_CONFIG[channel_label].adc_block,ADC_DRIVER_CONFIG[channel_label].sequ_num, &raw_value[channel_label]); 
            }
        }

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
    adc_ret_e ret = ADC_RET_OK;
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

        *temperature = ((1475 * 1023) - (2250 * raw_value[TEMP_SENSOR])) / 10230;
        if(!RTOSMutexGive(ADCMutex))
        {
            ret = ADC_MUTEX_GIVE_ERROR;
            break;
        }

    } while (0);

    return ret;
}

