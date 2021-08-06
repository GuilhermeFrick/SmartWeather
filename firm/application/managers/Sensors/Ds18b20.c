/**
 * @file Ds18b20.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "OneWire.h"
#include "Ds18b20.h"
#include <stdbool.h>
static uint8_t RomAdrr[8];
static bool device_done;
static uint8_t last_dicrepancy;
typedef struct Ds18b20ProtocolDefinition
{
    uint8_t state;
    uint8_t byte_index;
    uint8_t mask;
    uint8_t bit_pos;
    uint8_t discrepancy_marker;
    bool bit_b;
    bool b_status;
    bool next_b;
}Ds18b20Protocol_t;

/**
 * @brief 
 * 
 * @param addr 
 * @return DS18b20_Ret_e 
 */
DS18b20_Ret_e Ds18b20_DeviceAddr(uint8_t addr)
{
    DS18b20_Ret_e ret = DS18B20_RET_OK;

    uint8_t byte = 0;

    if(addr == MATCH_ROM_CMD)
    {
        OneWire_WriteByte(MATCH_ROM_CMD);
        for(byte = 0; byte<8; byte++)
        {
            OneWire_WriteByte(RomAdrr[byte]);
        }
    }
    else
    {
        OneWire_WriteByte(SKIP_ROM_CMD);
    }

    return ret;
}
/**
 * @brief 
 * 
 * @param next_device_status 
 * @return DS18b20_Ret_e 
 */
DS18b20_Ret_e Ds18b20_FindNexttDevice(bool *next_device_status)
{
    DS18b20_Ret_e ret = DS18B20_RET_OK;
    Ds18b20Protocol_t *DS_DRIVER;
    DS_DRIVER->state = 0;
    DS_DRIVER->byte_index = 0;
    DS_DRIVER->mask = 1;
    DS_DRIVER->bit_pos = 1;
    DS_DRIVER->discrepancy_marker = 0;
    DS_DRIVER->bit_pos = false;
    DS_DRIVER->b_status = false;
    DS_DRIVER->next_b = false;

    for (DS_DRIVER->byte_index = 0; DS_DRIVER->byte_index < 8; DS_DRIVER->byte_index++)
    {
        RomAdrr[DS_DRIVER->byte_index]=0x00;
    }
    if(DS_DRIVER->b_status || device_done)
    {
        last_dicrepancy = 0;
        next_device_status = false;
        return DS18B20_RET_OK;
    }
    OneWire_WriteByte(SEARCH_ROM_CMD);
    DS_DRIVER->byte_index = 0;
    do
    {
        DS_DRIVER->state =0;
        if(OneWire_ReadBit()!=ONE_WIRE_RESET)
        {
            DS_DRIVER->state =2;
        }
        OneWireDelayus(120);
        if(OneWire_ReadBit()!=ONE_WIRE_RESET)
        {
            DS_DRIVER->state |=1;
        }
        OneWireDelayus(120);
        if( DS_DRIVER->state == 3)
        {
            break;
        }
        else
        {
            if(DS_DRIVER->state > 0)
            {
                DS_DRIVER->bit_b = (bool)(DS_DRIVER->state>>1);
            }
            else
            {
                if(DS_DRIVER->bit_pos<last_dicrepancy)
                {
                    DS_DRIVER->bit_b = ((RomAdrr[DS_DRIVER->byte_index] & DS_DRIVER->mask)>0);
                }
                else
                {
                    DS_DRIVER->bit_b = (DS_DRIVER->bit_pos == last_dicrepancy);
                }
                if(DS_DRIVER->bit_b == 0)
                {
                    DS_DRIVER->discrepancy_marker = DS_DRIVER->bit_pos;
                }
            }

            if(DS_DRIVER->bit_b !=0)
            {
                RomAdrr[DS_DRIVER->byte_index]|=DS_DRIVER->mask;
            }
            else
            {
                RomAdrr[DS_DRIVER->byte_index] &= ~DS_DRIVER->mask;
            }
            OneWire_WriteBit((OneWire_Bit_e)DS_DRIVER->bit_b);
            DS_DRIVER->bit_pos++;
            DS_DRIVER->mask <<=1;
            if(DS_DRIVER->mask ==0)
            {
                DS_DRIVER->byte_index++;
                DS_DRIVER->mask = 1;
            }
        }
        
    } while (DS_DRIVER->byte_index < 8);

    if(DS_DRIVER->bit_pos < 65)
    {
        last_dicrepancy = 0;
    }
    else
    {
        last_dicrepancy = DS_DRIVER->discrepancy_marker;
        device_done = (last_dicrepancy == 0);
        DS_DRIVER->next_b = true;
    }

    *next_device_status = DS_DRIVER->next_b;

    return ret;
    
}
/**
 * @brief 
 * 
 * @param FirstDeviceStatus 
 * @return DS18b20_Ret_e 
 */
DS18b20_Ret_e Ds18b20_FindFirstDevice(bool *FirstDeviceStatus)
{
    DS18b20_Ret_e ret = DS18B20_RET_OK;

    last_dicrepancy = 0;
    device_done = false;
    Ds18b20_FindNexttDevice(FirstDeviceStatus);
    return ret;
}
/**
 * @brief 
 * 
 * @param T_high 
 * @param T_low 
 * @return DS18b20_Ret_e 
 */
DS18b20_Ret_e Ds18b20_SetThresholds(uint8_t T_high, uint8_t T_low)
{
    DS18b20_Ret_e ret = DS18B20_RET_OK;
    OneWire_Reset();
    Ds18b20_DeviceAddr(MATCH_ROM_CMD);
    OneWire_WriteByte(T_high);
    OneWire_WriteByte(T_low);
    OneWireDelayus(10);
    OneWire_Reset();
    Ds18b20_DeviceAddr(MATCH_ROM_CMD);
    OneWire_WriteByte(COPY_MEM_CMD);
    OneWireDelayus(10);
    return ret;
}

DS18b20_Ret_e Ds18b20_GetRawTemp(void)
{
    uint8_t  byte = 0;
    uint32_t tmp = 0;
    uint32_t highres;
    uint8_t scrpad[9];

    OneWire_Reset();
    Ds18b20_DeviceAddr(MATCH_ROM_CMD);
    OneWirePinModeOutput();
    OneWireWritePin(ONE_WIRE_SET);
    OneWire_WriteByte(CONVERT_TEMP_CMD);
    OneWireDelayus(750);
    OneWire_Reset();
    Ds18b20_DeviceAddr(MATCH_ROM_CMD);
    OneWire_WriteByte(READ_MEM_CMD);

    for (byte = 0; byte < 9; byte++)
    {
        OneWire_ReadByte(scrpad[byte],1);
    }
    tmp = 0;
    tmp = (unsigned long)((unsigned long)scrpad[temp_MSB] << 8);
    tmp |= (unsigned long)(scrpad[temp_LSB]);

     if(RomAddr[0] == DS18S20_family_code)
    {                                       \
        tmp >>= 1;             
        tmp = ((unsigned long)tmp << 8);
                                           
        tmp -= ((unsigned long)temp_res >> 2);
                                                       
        highres = scrpad[cnt_per_s] - scrpad[cnt_remain];
        highres = ((unsigned long)highres << 8);
        if(scrpad[cnt_per_s])
        {                             
            highres = highres / ((unsigned long)scrpad[cnt_per_s]);
        }
                   
        highres = highres + tmp;
    }
    else
    {
        highres = tmp;
        highres <<= 4;
    }                                           
    return highres; 

}

DS18b20_Ret_e Ds18b20_GetTemp(const float *temperature)
{
   *temperature =  (float)DS1820_get_raw_temp() / (float)temp_res);
}