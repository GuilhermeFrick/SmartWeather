/**
 * @file SensorManager.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "SensorManager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "WrapperRTOS.h"
#include "InputList.h"
#include "filter.h"
#include "gpio_driver.h"


/** @addtogroup  SensorManagerPrivate Sensor Manager Private
 * @{
 */

/*!
 *  \brief Structure to configure board sensors
 */

typedef struct
{
    InputList_e    label;        /**<Sensor label*/
    SensorEnable_e enabled;      /**<Enables or disables the sensor*/
    SensorType_e   type;         /**<Sets if the sensor will have direct logic, inverse logic or is a logic sensor*/
    gpio_port_t    port;         /**<Port of the sensor pin*/
    gpio_pin_t     pin;          /**<Sensor pin number*/
    uint32_t       trigger_low;  /**<Debounce time from high logic to low logic*/
    uint32_t       trigger_high; /**<Debounce time from low logic to high logic*/
    EnableLog_e    log;          /**<Indicates when a sensor change will log*/
    const char *   name;         /**<Sensor name to verbose information (portuguese)*/
} SensorConfig_t;

// clang-format off

/**Sensor configuration */
static const SensorConfig_t SensorConfig[] = {
    {.label   = SENSOR_1, .enabled = ENABLE_SENS, .type  = INVERSE_SENS, .port = PORTC, .pin = PIN0, .trigger_low = 15, .trigger_high = 5, .log = LOG_DISABLE, .name = "Coin out conveyor belt"},
   

};

// clang-format on
/*!
 *  \brief  Sensor complete status structure
 */
typedef struct
{
    DebounceControl_t     DebounceControl; /**<Debounce control structure*/
    SensorStatus_t        old_status;      /**<Last sensor status*/
    uint32_t              timestamp;       /**<Timestamp since last sensor changed*/
    const SensorConfig_t *config;          /**<Pointer to sensor configuration*/
} Sensor_t;

static Sensor_t       Sensor[INPUT_QTY];       /**<Structure with sensors debounce filter configuration and status*/
static uint32_t       ConfiguredSensorsQty = 0;    /**<Number of sensors configured for this firmware*/
static void *         SensorManagerMutex   = NULL; /**<Mutex to handle global values*/
static SensorStatus_t ReadPin(uint8_t sensor_index);

SensorManagerRet_e SensorManagerInit(void)
{
    SensorManagerRet_e ret = SM_RET_OK;

    do
    {
        if (SensorManagerMutex == NULL)
        {
            SensorManagerMutex = xSemaphoreCreateMutex();
        }
        if (SensorManagerMutex == NULL)
        {
            ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 10) != pdTRUE)
        {
            ret = SM_MUTEX_TAKE_ERROR;
            break;
        }
        ConfiguredSensorsQty = sizeof(SensorConfig) / sizeof(*SensorConfig);
        for (InputList_e label = SENSOR_1; label < INPUT_QTY; label++)
        {
            Sensor[label].DebounceControl.state      = UNDEFINED_STATE;
            Sensor[label].DebounceControl.status_pin = SENSOR_CLEARED;
            Sensor[label].DebounceControl.status     = SENSOR_CLEARED;
            Sensor[label].DebounceControl.timestamp  = RTOSGetTimeMs();
        }
        for (uint8_t i = 0; i < ConfiguredSensorsQty; i++)
        {
            InputList_e label = SensorConfig[i].label;

            if (SensorConfig[i].enabled != ENABLE_SENS)
            {
                continue;
            }
            Sensor[label].config                       = &SensorConfig[i];
            Sensor[label].DebounceControl.trigger_high = SensorConfig[i].trigger_high;
            Sensor[label].DebounceControl.trigger_low  = SensorConfig[i].trigger_low;
            if (SensorConfig[i].type == DIRECT_SENS)
            {
                GpioInitInput(SensorConfig[i].port, SensorConfig[i].pin, PULL_UP);
            }
            else if (SensorConfig[i].type == INVERSE_SENS)
            {
                //GpioInitInputIT(SensorConfig[i].port, SensorConfig[i].pin, NO_PULL);
            }
            else if (SensorConfig[i].type == DIRECT_INT)
            {
                //GpioInitInputIT(SensorConfig[i].port, SensorConfig[i].pin);
            }
            else if (SensorConfig[i].type == INVERSE_INT)
            {
                //GpioInitInputIT(SensorConfig[i].port, SensorConfig[i].pin);
            }
        }
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            ret = SM_MUTEX_GIVE_ERROR;
            break;
        }
    } while (0);

    return ret;
}

SensorManagerRet_e SensorManagerProcess(void)
{
    SensorManagerRet_e ret = SM_RET_OK;

    do
    {
        if (SensorManagerMutex == NULL)
        {
            ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 10) != pdTRUE)
        {
            ret = SM_MUTEX_TAKE_ERROR;
            break;
        }
        for (uint8_t i = 0; i < ConfiguredSensorsQty; i++)
        {
            InputList_e label_i;

            label_i = SensorConfig[i].label;

            if (Sensor[label_i].config->enabled != ENABLE_SENS)
            {
                Sensor[label_i].DebounceControl.status_pin = SENSOR_CLEARED;
                continue;
            }

            Sensor[label_i].DebounceControl.status_pin = ReadPin(i);
            SensorDebounce(&Sensor[label_i].DebounceControl);

            if (Sensor[label_i].old_status != Sensor[label_i].DebounceControl.status)
            {
                Sensor[label_i].old_status = Sensor[label_i].DebounceControl.status;

                // TODO: Notify
            }
            if (Sensor[label_i].DebounceControl.status != Sensor[label_i].DebounceControl.status_pin)
            {
                ret = SM_RET_RUNNING;
            }
        }
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            ret = SM_MUTEX_GIVE_ERROR;
            break;
        }
    } while (0);

    return ret;
}

SensorStatus_t SensorGetPin(InputList_e label)
{
    SensorStatus_t in_pin = SENSOR_CLEARED;

    do
    {
        if (SensorManagerMutex == NULL)
        {
            // ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 100) != pdTRUE)
        {
            // ret = SM_MUTEX_TAKE_ERROR;
            break;
        }
        if (label >= INPUT_QTY)
        {
            in_pin = SENSOR_SET;
            break;
        }

        in_pin = Sensor[label].DebounceControl.status_pin;
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            // ret = SM_MUTEX_GIVE_ERROR;
        }

    } while (0);

    return in_pin;
}

SensorStatus_t SensorManagerGetStatus(InputList_e label)
{
    SensorStatus_t status_pin = SENSOR_CLEARED;
    do
    {
        if (SensorManagerMutex == NULL)
        {
            // ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (label >= INPUT_QTY)
        {
            status_pin = SENSOR_SET;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 100) != pdTRUE)
        {
            // ret = SM_MUTEX_TAKE_ERROR;
            break;
        }
        status_pin = Sensor[label].DebounceControl.status;
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            // ret = SM_MUTEX_GIVE_ERROR;
        }

    } while (0);

    return status_pin;
}

/*!
 * \brief 		Function to get sensor name in verbose format
 * \param[in]   label: sensor label
 * \param[in]   buff: buffer to be filled with string
 * \param[in]   size_of_buff: size of buff in bytes
 * \return      On success returns the number of bytes filled in the output buffer, on error returns \ref SensorManagerRet_e
 */
int32_t SensorGetName(InputList_e label, uint8_t *buff, uint32_t size_of_buff)
{
    int32_t ret       = 0;
    int32_t buff_size = 0;

    do
    {
        if (SensorManagerMutex == NULL)
        {
            ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (label >= INPUT_QTY)
        {
            ret = SM_RET_ERROR;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 100) != pdTRUE)
        {
            ret = SM_MUTEX_TAKE_ERROR;
            break;
        }

        if (strlen((char *)Sensor[label].config->name) > SENSOR_NAME_SIZE)
        {
            ret = SM_RET_ERROR;
            break;
        }
        if (Sensor[label].config->enabled != ENABLE_SENS)
        {
            ret = SM_RET_ERROR;
            break;
        }

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "%.24s", Sensor[label].config->name);

        ret = buff_size;
    } while (0);

    if (SensorManagerMutex != NULL)
    {
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            ret = SM_MUTEX_GIVE_ERROR;
        }
    }

    return ret;
}
/*!
 * \brief 		Function to get sensor information in verbose format
 * \param[in]   label: sensor label
 * \param[in]   buff: buffer to be filled with string
 * \param[in]   size_of_buff: size of buff in bytes
 * \return      On success returns the number of bytes filled in the output buffer, on error returns \ref SensorManagerRet_e
 */
int32_t SensorVerboseInfo(InputList_e label, uint8_t *buff, uint32_t size_of_buff)
{
    int32_t ret       = 0;
    int32_t buff_size = 0;

    do
    {
        if (SensorManagerMutex == NULL)
        {
            ret = SM_MUTEX_NULL_ERROR;
            break;
        }
        if (label >= INPUT_QTY)
        {
            ret = SM_RET_ERROR;
            break;
        }
        if (xSemaphoreTake(SensorManagerMutex, 100) != pdTRUE)
        {
            ret = SM_MUTEX_TAKE_ERROR;
            break;
        }

        if (strlen((char *)Sensor[label].config->name) > SENSOR_NAME_SIZE)
        {
            ret = SM_RET_ERROR;
            break;
        }
        if (Sensor[label].config->enabled != ENABLE_SENS)
        {
            ret = SM_RET_ERROR;
            break;
        }

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "%.24s\r\n", Sensor[label].config->name);

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "  Enable: %1d\r\n", Sensor[label].config->enabled);

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "  Status:%d  Pin:%d\r\n", Sensor[label].DebounceControl.status,
                              Sensor[label].DebounceControl.status_pin);

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "  Debounce: %d / %d\r\n",
                              Sensor[label].DebounceControl.trigger_high, Sensor[label].DebounceControl.trigger_low);

        buff_size += snprintf((char *)&buff[buff_size], size_of_buff - buff_size, "  Log: %d\r\n", Sensor[label].config->log);

        ret = buff_size;
    } while (0);

    if (SensorManagerMutex != NULL)
    {
        if (xSemaphoreGive(SensorManagerMutex) != pdTRUE)
        {
            ret = SM_MUTEX_GIVE_ERROR;
        }
    }

    return ret;
}

static SensorStatus_t ReadPin(uint8_t sensor_index)
{
    SensorStatus_t status_pin = SENSOR_CLEARED;

    if ((SensorConfig[sensor_index].type == DIRECT_SENS) || (SensorConfig[sensor_index].type == DIRECT_INT))
    {
        if (GpioReadPin(SensorConfig[sensor_index].port, SensorConfig[sensor_index].pin))
        {
            status_pin = SENSOR_SET;
        }
        else
        {
            status_pin = SENSOR_CLEARED;
        }
    }
    else if ((SensorConfig[sensor_index].type == INVERSE_SENS) || (SensorConfig[sensor_index].type == INVERSE_INT))
    {
        if (GpioReadPin(SensorConfig[sensor_index].port, SensorConfig[sensor_index].pin))
        {
            status_pin = SENSOR_CLEARED;
        }
        else
        {
            status_pin = SENSOR_SET;
        }
    }

    return status_pin;
}

uint32_t FilterGetTick(void)
{
    return RTOSGetTimeMs();
}
