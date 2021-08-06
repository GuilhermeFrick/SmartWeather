/**
 * @file ONE_WIRE.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ONE_WIRE_H
#define ONE_WIRE_H

#include <stdint.h>

typedef enum OneWire_Ret_Definition
{
    ONE_WIRE_RET_OK = 0,
    ONE_WIRE_RET_ERROR = -1,
    ONE_WIRE_MUTEX_ERROR = -2,
    ONE_WIRE_MUTEX_GIVE_ERROR = -3,
    ONE_WIRE_MUTEX_TAKE_ERROR = -4,
}OneWire_Ret_e;

typedef enum OneWire_Bit_Definition
{
    ONE_WIRE_RESET = 0UL,
    ONE_WIRE_SET
}OneWire_Bit_e;


OneWire_Ret_e OneWire_Init(void);
OneWire_Bit_e OneWire_Reset(void);
OneWire_Bit_e OneWire_ReadBit(void);
OneWire_Ret_e OneWire_WriteBit(OneWire_Bit_e bit_value);
OneWire_Ret_e OneWire_WriteByte(uint8_t data);
OneWire_Ret_e OneWire_ReadByte(uint8_t *pBuffer, uint8_t Size);


#endif
