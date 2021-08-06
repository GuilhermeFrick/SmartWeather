/**
 * @file OneWire.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>
#include "OneWire.h"
/** \weakgroup   OneWireWeek  OneWire Week
 *  \ingroup OneWire
 * @{
 */
#ifndef __weak
#define __weak __attribute__((weak)) /**<Ensures weak macro is defined*/
#endif
__weak OneWire_Ret_e OneWireInitGPIO(void);
__weak OneWire_Ret_e OneWire_Take(void);
__weak OneWire_Ret_e OneWire_Give(void);
__weak OneWire_Bit_e OneWireReadPin(void);
__weak void OneWireWritePin(int state_pin);
__weak void OneWirePinModeInput(void);
__weak void OneWirePinModeOutput(void);
__weak void OneWireWaitMilliSec(uint32_t milisec);
__weak void OneWireDelayus(uint32_t delay_us);

/**
 * @brief 
 * 
 */
static const struct OneWireParams
{
    uint32_t presence_delay_us;
    uint32_t reset_pulse_ms;
    uint32_t start_pulse;
    uint32_t wait_bit_read_us;
    uint32_t write_bit_delay_us;
}ONE_WIRE_PARAM = {
    .presence_delay_us = 40,
    .reset_pulse_ms    = 480,
    .start_pulse = 2,
    .wait_bit_read_us = 5,
    .write_bit_delay_us = 100,
};

/**
 * @brief 
 * 
 * @return OneWire_Ret_e 
 */
OneWire_Ret_e OneWire_Init(void)
{
    OneWire_Ret_e ret = ONE_WIRE_RET_OK;
    do
    {
       OneWireInitGPIO();

    } while (0);
    

    return ret;
}
/**
 * @brief 
 * 
 * @return OneWire_Bit_e 
 */
OneWire_Bit_e OneWire_Reset(void)
{
    OneWire_Bit_e pulse_state = ONE_WIRE_RESET;
    OneWirePinModeOutput();
    OneWireWritePin(ONE_WIRE_RESET);
    OneWireDelayus(ONE_WIRE_PARAM.reset_pulse_ms);
    OneWireWritePin(ONE_WIRE_SET);
    OneWirePinModeInput();
    OneWireDelayus(ONE_WIRE_PARAM.presence_delay_us);
    pulse_state = OneWireReadPin();
    OneWireDelayus(424);
    return pulse_state;
}
/**
 * @brief 
 * 
 * @param bit_value 
 * @return OneWire_Ret_e 
 */
OneWire_Ret_e OneWire_WriteBit(OneWire_Bit_e bit_value)
{
    OneWire_Ret_e ret = ONE_WIRE_RET_OK;

    OneWirePinModeOutput();
    OneWireWritePin(ONE_WIRE_RESET);
    OneWireDelayus(ONE_WIRE_PARAM.start_pulse);
    if(bit_value !=ONE_WIRE_RESET)
    {
         OneWireWritePin(ONE_WIRE_SET);
    }
    OneWireDelayus(ONE_WIRE_PARAM.write_bit_delay_us);
    OneWireWritePin(ONE_WIRE_SET);
    return ret;
}
/**
 * @brief 
 * 
 * @return OneWire_Bit_e 
 */
OneWire_Bit_e OneWire_ReadBit(void)
{
    OneWire_Bit_e pulse_state = ONE_WIRE_RESET;
    OneWirePinModeOutput();
    OneWireWritePin(ONE_WIRE_RESET);
    OneWireDelayus(ONE_WIRE_PARAM.start_pulse);
    OneWirePinModeInput();
    OneWireDelayus(ONE_WIRE_PARAM.wait_bit_read_us);
    pulse_state = OneWireReadPin();
    return pulse_state;
}
/**
 * @brief 
 * 
 * @param DevAddress 
 * @param dev_register 
 * @param data 
 * @return OneWire_Ret_e 
 */
OneWire_Ret_e OneWire_WriteByte(uint8_t data)
{
    OneWire_Ret_e ret = ONE_WIRE_RET_OK;
    uint8_t byte = 0;
    uint8_t aux = 0;
    do
    {
        if (OneWire_Take() != ONE_WIRE_RET_OK)
        {
            ret = ONE_WIRE_MUTEX_TAKE_ERROR;
            break;
        }
        for (byte = 0; byte < 8; byte++)
        {
            aux = (data>>byte);
            aux &= 0x01;
            ret = OneWire_WriteBit((OneWire_Bit_e)aux);
        }
        OneWireDelayus(105);
        if (OneWire_Give() != ONE_WIRE_RET_OK)
        {
            ret = ONE_WIRE_MUTEX_TAKE_ERROR;
            break;
        }
    } while (0);
    
    

    return ret;
}
/**
 * @brief 
 * 
 * @param DevAddress 
 * @param pBuffer 
 * @param Size 
 * @return OneWire_Ret_e 
 */
OneWire_Ret_e OneWire_ReadByte(uint8_t *pBuffer, uint8_t Size)
{
    OneWire_Ret_e ret = ONE_WIRE_RET_OK;
    uint8_t byte = 0;
    do
    {
       if (OneWire_Take() != ONE_WIRE_RET_OK)
        {
            ret = ONE_WIRE_MUTEX_TAKE_ERROR;
            break;
        }
        for(byte = 0; byte<(Size*8);byte++)
        {
            *pBuffer |= (1<<byte);
        }
        OneWireDelayus(120);
        if (OneWire_Give() != ONE_WIRE_RET_OK)
        {
            ret = ONE_WIRE_MUTEX_TAKE_ERROR;
            break;
        }

    } while (0);
    
    return ret;
}

/**
 * @brief 
 * 
 * @return __weak 
 */
__weak OneWire_Ret_e OneWireInitGPIO(void)
{
    return ONE_WIRE_RET_OK;
}
/**
 * @brief 
 * 
 * @return __weak 
 */
__weak OneWire_Ret_e OneWire_Take(void)
{
    return ONE_WIRE_RET_OK;
}
/**
 * @brief 
 * 
 * @return __weak 
 */
__weak OneWire_Ret_e OneWire_Give(void)
{
    return ONE_WIRE_RET_OK;
}
/**
 * @brief 
 * 
 * @return __weak 
 */
__weak OneWire_Bit_e OneWireReadPin(void)
{
    return ONE_WIRE_RESET;
}
/**
 * @brief 
 * 
 * @param state_pin 
 * @return __weak 
 */
__weak void OneWireWritePin(int state_pin)
{

}
/**
 * @brief 
 * 
 * @return __weak 
 */
__weak void OneWirePinModeInput(void)
{
}
/**
 * @brief 
 * 
 * @return __weak 
 */
__weak void OneWirePinModeOutput(void)
{

}
/**
 * @brief 
 * 
 * @param milisec 
 * @return __weak 
 */
__weak void OneWireWaitMilliSec(uint32_t milisec)
{
}
/**
 * @brief 
 * 
 * @param delay_us 
 * @return __weak 
 */
__weak void OneWireDelayus(uint32_t delay_us)
{

}
