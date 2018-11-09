/*
 * UART.c
 *
 *  Created on: Oct 1, 2018
 *      Author: dean4ta
 */

#include "msp.h"
#include <driverlib.h>
#include <UART_deanCustom.h>

/*
Configuration for UART
*/
static const eUSCI_UART_Config Uart115200Config =
{
EUSCI_A_UART_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
6, // BRDIV
8, // UCxBRF
0, // UCxBRS
EUSCI_A_UART_NO_PARITY, // No Parity
EUSCI_A_UART_LSB_FIRST, // LSB First
EUSCI_A_UART_ONE_STOP_BIT, // One stop bit
EUSCI_A_UART_MODE, // UART mode
EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION// Oversampling
};

/*
 * UART Tx function
 */
void uartTransmitString(char * s)
{
    // Loop while not null
    while(*s)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE, *s++);
    }
}

/*
 * UART initialization
 */
void uartInit()
{
    // select the GPIO functionality
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // configure the digital oscillator
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    // configure the UART with baud rate 115200
    MAP_UART_initModule(EUSCI_A0_BASE, &Uart115200Config);

    // enable the UART
    MAP_UART_enableModule(EUSCI_A0_BASE);
}
