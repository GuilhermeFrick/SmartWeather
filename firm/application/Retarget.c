/*!
 * \file      Retarget.c
 * \author    Luiz Guilherme Rizzatto Zucchi (luiz.zucchi@perto.com.br)
 * \brief     Source file with debug interface definition
 * \version   1.1
 * \date      2020-09-30
 * \copyright Copyright (c) 2020
 */
#include <stdio.h>
#include <stdint.h>
#include "Retarget.h"
#include "uart_driver.h"

/*!
 * \brief       Function to send a character through standart output
 * \param[in]   ch: Character to transmit
 * \return      Character to transmit
 */

FILE __stdout; //STDOUT
FILE __stdin;  //STDIN

int fputc(int c, FILE * stream)
{
	uint8_t ch = (uint8_t) c;
  UartSend(&ch, 1);
	return c; 
}


