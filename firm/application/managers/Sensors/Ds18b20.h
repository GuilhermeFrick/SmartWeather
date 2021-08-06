/**
 * @file Ds18b20.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef DS18B20_H
#define DS18B20_H
#include <stdint.h>
typedef enum DS18b20_Ret_Definition
{
    DS18B20_RET_OK = 0,
    DS18B20_RET_ERROR = -1,

}DS18b20_Ret_e;

#define MATCH_ROM_CMD 0x55
#define SKIP_ROM_CMD  0xCC
#define SEARCH_ROM_CMD 0xF0
#define COPY_MEM_CMD 0x48
#define CONVERT_TEMP_CMD 0x44
#define READ_MEM_CMD 0xBE
#define temp_MSB 1
#define temp_LSB 0
#define DS18S20_family_code 0x10
DS18b20_Ret_e Ds18b20_DeviceAddr(uint8_t addr);
DS18b20_Ret_e Ds18b20_FindFirstDevice(bool *FirstDeviceStatus);
DS18b20_Ret_e Ds18b20_FindNexttDevice(bool *next_device_status);
DS18b20_Ret_e Ds18b20_SetThresholds(uint32_t T_high, uint32_t T_low);
DS18b20_Ret_e Ds18b20_GetTemp(const float *temperature);
DS18b20_Ret_e Ds18b20_GetRawTemp(void);




#endif