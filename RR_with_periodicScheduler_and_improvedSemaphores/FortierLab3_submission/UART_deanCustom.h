/*
 * UART.h
 *
 *  Created on: Oct 1, 2018
 *      Author: dean4ta
 */

#ifndef UART_DEANCUSTOM_H_
#define UART_DEANCUSTOM_H_

#include "msp.h"
#include <driverlib.h>

/*
 * UART Tx function
 */
void uartTransmitString(char * s);

/*
 * UART initialization
 */
void uartInit();

#endif /* UART_DEANCUSTOM_H_ */
