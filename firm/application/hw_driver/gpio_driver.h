/**
 * @file gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/** \addtogroup  GPIO GPIO
 *  \ingroup HwDriver
 * @{
 */
#ifndef _GPIO_DRIVER_H
#define _GPIO_DRIVER_H
#include <stdint.h>
#include <stdbool.h>
/*!
 *  \brief Enumerates all the ports that can be used in the functions of this module
 */
typedef enum{
    PORT_INV = -1,        /**<Invalid port*/
    PORTA = 0x40004000,   /**<Select port A*/
    PORTB = 0x40005000,   /**<Select port B*/
    PORTC = 0x40006000,   /**<Select port C*/
    PORTD = 0x40007000,   /**<Select port D*/
    PORTE = 0x40024000,   /**<Select port E*/
    PORTF = 0x40025000,   /**<Select port F*/
}gpio_port_t;
/*!
 *  \brief Enumerates all the pins that can be used in the functions of this module
 */
typedef enum{
    PIN0 = 0,   /**<Select pin 0*/
    PIN1 ,       /**<Select pin 1*/
    PIN2 ,       /**<Select pin 2*/
    PIN3 ,       /**<Select pin 3*/
    PIN4 ,       /**<Select pin 4*/
    PIN5 ,       /**<Select pin 5*/
    PIN6 ,       /**<Select pin 6*/
    PIN7 ,       /**<Select pin 7*/
}gpio_pin_t;
/** 
  * \brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_RESET = 0U,
  GPIO_SET   = 1U
}gpio_state_t;

typedef enum
{
  NO_PULL = 0U,
  PULL_UP
}gpio_pull_t;

typedef enum
{
  MODE_IN   = 0U,
  MODE_OUT,
  MODE_HW
}gpio_mode_t;

typedef enum 
{
    TRIGGER_FALLING_EDGE  = 0x00000000,
    TRIGGER_BOTH_EDGE     = 0x00000001,
    TRIGGER_RISING_EDGE   = 0x00000004,
    TRIGGER_HIGH_LEVEL    = 0x00000006,
    TRIGGER_LOW_LEVEL     = 0x00000002,
    TRIGGER_DISCRETE_INT  = 0x00010000
}gpio_trigger_t;

typedef enum{
  LED_RED  = 1,
  LED_BLUE = 2,
  LED_GREN = 3,
}LEDS_e;


void GpioInit(void);
void GpioClockEnable(gpio_port_t port);
void GpioInitInputIT(gpio_port_t port, uint8_t pin,gpio_trigger_t trigger);
void GpioInitInput(gpio_port_t port, uint8_t pin, gpio_pull_t pull_up);
void GpioInitOutput (gpio_port_t port,uint8_t pin);
void GpioInitLed(gpio_port_t port, uint8_t pin);
gpio_state_t GpioReadPin(gpio_port_t port,gpio_pin_t pin);
void GpioWritePin(gpio_port_t port,uint8_t pin,uint8_t value);
void GpioWriteLed(gpio_port_t port,uint8_t pin,uint8_t value);
void GpioTogglePin(gpio_port_t port, uint8_t pin);
uint16_t GpioReadPortInput(gpio_port_t port);
uint16_t GpioReadPortOutput(gpio_port_t port);

#endif /* GPIO_H */
/** @}*/
