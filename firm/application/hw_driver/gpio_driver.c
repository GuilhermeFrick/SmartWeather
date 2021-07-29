/**
 * @file gpio.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdlib.h> 
#include "gpio_driver.h"
#include "gpio.h"
#include "sysctl.h"


/*!
 *  \brief      Initializes GPIO module
 */
void GpioInit(void)
{
   
    // todo: remover esses pinos
}
/*!
 *  \brief      Function to enable clock on a GPIO port
 *	\param[in]  port: port to be enable
 */
void GpioClockEnable(gpio_port_t port)
{   
    uint32_t _port = NULL;
    switch (port)
    {
        case PORTA:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
            _port = SYSCTL_PERIPH_GPIOA;
            break;
        case PORTB:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            _port = SYSCTL_PERIPH_GPIOB;
            break;
        case PORTC:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
            _port = SYSCTL_PERIPH_GPIOC;
            break;
        case PORTD:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            _port = SYSCTL_PERIPH_GPIOD;
            break;
        case PORTE:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            _port = SYSCTL_PERIPH_GPIOE;
            break;
        case PORTF:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
            _port = SYSCTL_PERIPH_GPIOF;
        default:
            break;
    }
    if(_port!=NULL)
    {
        while(!SysCtlPeripheralReady(_port))
        {
        }
    }
    
}
/*!
 *  \brief      Function to get current status of a GPIO input pin
 *	\param[in]  port: port to be read
 *	\param[in]  pin: pin to be read
 *  \return     Current GPIO pin status
 *  \retval     0: pin is in reset state (GND)
 *  \retval     1: pin is in set state (3.3V)
 */
gpio_state_t GpioReadPin(gpio_port_t port, gpio_pin_t pin)
{
    return (gpio_state_t)GPIOPinRead(port,1UL << pin);
    
}
/*!
 *  \brief      Function to set status of a GPIO pin
 *	\param[in]  port: port to be written
 *	\param[in]  pin: pin to be written
 *	\param[in]  value: value to be written  (0 | 1)
 */
void GpioWritePin(gpio_port_t port, uint8_t pin, uint8_t value)
{
    GPIOPinWrite(port, 1UL << pin, value << pin);
}
/*!
 *  \brief      Function to set status of a GPIO pin
 *	\param[in]  port: port to be written
 *	\param[in]  pin: pin to be written
 *	\param[in]  value: value to be written  (0 | 1)
 */
void GpioWriteLed(gpio_port_t port, uint8_t pin, uint8_t value)
{
    GPIOPinWrite(port, 1UL << pin, value << pin);
}

void GpioTogglePin(gpio_port_t port, uint8_t pin)
{
    //GPIOPinWrite(port,1UL << pin, value ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
/*!
 *  \brief      Function to ge status of a GPIO port
 *	\param[in]  port: port to be read
 *  \return     Current GPIO port status
 */
uint16_t GpioReadPortInput(gpio_port_t port)
{
    //GPIO_TypeDef *gpio = (GPIO_TypeDef *)(AHB2PERIPH_BASE + 0x0400U * port);
    //return (uint16_t)gpio->IDR;
    return GPIO_RESET;
}
/*!
 *  \brief      Function to ge status of a GPIO port
 *	\param[in]  port: port to be read
 *  \return     Current GPIO port status
 */
uint16_t GpioReadPortOutput(gpio_port_t port)
{
    //GPIO_TypeDef *gpio = (GPIO_TypeDef *)(AHB2PERIPH_BASE + 0x0400U * port);
    //return (uint16_t)gpio->ODR;
    return GPIO_RESET;
}
/*!
 *  \brief      Initializes the selected pin as input
 *	\param[in]  port: port to be initialized as input
 *	\param[in]  pin: to be initialized as input
 */
void GpioInitInput(gpio_port_t port, uint8_t pin, gpio_pull_t pull_up)
{
    GPIOPinTypeGPIOInput(port,1UL << pin);
}
/*!
 *  \brief      Initializes the selected pin as input with the IT settings
 *	\param[in]  port: port to be initialized as input
 *	\param[in]  pin: to be initialized as input
 */
void GpioInitInputIT(gpio_port_t port, uint8_t pin,gpio_trigger_t trigger)
{
   
   
    //GPIO_InitTypeDef GPIO_InitStruct;

    //GPIO_CLK_ENABLE(port);

    //GPIO_InitStruct.Pin   = 1U << pin;
    //GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
    //GPIO_InitStruct.Pull  = GPIO_NOPULL;
    //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    //HAL_GPIO_Init((GPIO_TypeDef *)(AHB2PERIPH_BASE + 0x0400U * port), &GPIO_InitStruct);
}
/*!
 *  \brief      Initializes the selected pin as output with the default settings
 *	\param[in]  port: port to be initialized as output
 *	\param[in]  pin: to be initialized as output
 */
void GpioInitOutput(gpio_port_t port, uint8_t pin)
{ 
    GPIOPinTypeGPIOOutput(port,1UL << pin);
}

void GpioInitLed(gpio_port_t port, uint8_t pin)
{
    GPIOPinTypeGPIOOutput(port,1UL << pin);
    
}
