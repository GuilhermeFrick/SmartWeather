/**
 * @file SensorManager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/** @addtogroup   SensorManager Sensor Manager
 * @{
 */
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H
#include "filter.h"
#include "InputList.h"


#define SENSOR_NAME_SIZE    24  /**<Allocated size to sensor name (bytes)*/
/*!
 *  \brief Return codes of the functions
 */

typedef enum
{
    SM_RET_RUNNING  = 1,       /**<Debounce process in progress, manager is running*/
    SM_RET_OK       = 0,       /**<The function executed succesfully*/   
    SM_RET_ERROR    =-1,       /**<Generic error*/
    SM_RET_ERR_INIT =-2,       /**<Error initializing sensor*/
    SM_MUTEX_NULL_ERROR = -3, /**<Mutex not created*/
    SM_MUTEX_TAKE_ERROR = -4, /**<Error taking the mutex*/
    SM_MUTEX_GIVE_ERROR = -5, /**<Error giving the mutex*/
    SM_MEM_ERROR        = -6, /**<Insufficient memory*/
    SM_INV_SENSOR       = -7, /**<Invalid sensor selected*/
}SensorManagerRet_e;

/*!
 *  \brief Indicates whether the sensor is enabled
 */
typedef enum {
	ENABLE_SENS   = 0,  /**<Sensor is enabled*/
	DISABLE_SENS  = 1,  /**<Sensor is disabled*/
}SensorEnable_e;

/*!
 *  \brief Indicates sensor logic
 */
typedef enum {
    DIRECT_SENS  = 0,   /**<Direct logic: 0: normal, 1: violation*/
    INVERSE_SENS = 1,   /**<Inverse logic: 1: normal, 0: violation*/
    DIRECT_INT   = 2,   /**<Direct logic with interrupt: 0: normal, 1: violation*/
    INVERSE_INT  = 3,   /**<Inverse logic with interrupt: 0: normal, 1: violation*/
    LOGIC_SENS   = 4,   /**<Logic sensor, special treatment*/
} SensorType_e;
/*!
 *  \brief Indicates possible log configurations
 */
typedef enum {
	LOG_DISABLE = 0,	/**<Logs is disable to this sensor*/	
	LOG_RISING  = 1,	/**<Logs when the sensor breaches*/	
	LOG_FALLING = 2,	/**<Logs when the sensor returns to normal*/	
	LOG_BOTH    = 3		/**<Logs any sensor change*/
}EnableLog_e;
/*!
 *  \brief Structure to configure board sensors
 */

SensorManagerRet_e SensorManagerInit(void);
SensorManagerRet_e SensorManagerProcess(void);
SensorStatus_t SensorGetPin(InputList_e label);
SensorStatus_t SensorManagerGetStatus(InputList_e label);
int32_t SensorGetName(InputList_e label, uint8_t *buff, uint32_t size_of_buff);
int32_t SensorVerboseInfo(InputList_e label, uint8_t *buff, uint32_t size_of_buff);

#endif /*SENSOR_MANAGER_H*/
