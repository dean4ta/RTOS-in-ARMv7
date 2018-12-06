/*
 * RGBLeds.c
 *
 *  Created on: Sep 9, 2018
 *      Author: Isaac
 */
/*
 * Performs necessary initializations for RGB LEDs
 */
#include "msp.h"
#include <stdio.h>
#include <RGBLeds.h>


void init_RGBLEDS()
{
    uint16_t UNIT_OFF = 0x0000;

    // Software reset enable
    UCB2CTLW0 = UCSWRST;

    // Initialize I2C master
    //Set as master, I2C mode, Clock sync, SMCLK source, Transmitter
    UCB2CTLW0 |= (UCMST | UCMODE_3 | UCSYNC | UCSSEL__SMCLK | UCTR);

    // Set the Fclk as 400khz.
    //Presumes that SMCLK is selected as source and Fsmclk is 12 MHz..
    UCB2BRW = 30;

    // In conjunction with the next line, this sets the pins as I2C mode.
    // (Table found on p160 of SLAS826E)
    // Set P3.6 as UCB2_SDA and 3.7 as UCB2_SLC
    P3SEL0 |= 0xC0;
    P3SEL1 &= 0x3F;

    // Bitwise anding of all bits except UCSWRST.
    UCB2CTLW0 &= ~UCSWRST;

    LP3943_LedModeSet(RED, UNIT_OFF);
    LP3943_LedModeSet(GREEN, UNIT_OFF);
    LP3943_LedModeSet(BLUE, UNIT_OFF);

}

void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA)
{
    /*
     * LP3943_LedModeSet
     * This function will set each of the LEDs to the desired operating
     * mode.  The oeprating modes are on, off, PWM1 and PWM2.
     *
     * The units that can be written to are:
     * UNIT  |  0  |  Red
     * UNIT  |  1  |  Blue
     * UNIT  |  2  |  Green
     *
     * The registers to be written to are:
     * --------
     * | LS0  |  LED0-3 Selector        |
     * | LS1  |  LED4-7 Selector        |
     * | LS2  |  LED8-11 Selector       |
     * | LS3  |  LED12-16 Selector      |
     * --------
     */
    uint32_t sendString = 0x00000000;
    uint16_t i;
    for(i = 0; i < 16; i++){
            sendString |= ((uint32_t)(LED_DATA & (0x0001 << i))) << i;
        }
    UCB2I2CSA = 0x60 + unit;
    UCB2CTLW0 |= UCTXSTT;
    while(UCB2CTLW0 & UCTXSTT);
    UCB2TXBUF = 0x16;
    while(!(UCB2IFG & UCTXIFG0));
    UCB2TXBUF = sendString;
    while(!(UCB2IFG & UCTXIFG0));
    UCB2TXBUF = sendString>>8;
    while(!(UCB2IFG & UCTXIFG0));
    UCB2TXBUF = sendString>>16;
    while(!(UCB2IFG & UCTXIFG0));
    UCB2TXBUF = sendString>>24;
    while(!(UCB2IFG & UCTXIFG0));
    UCB2CTLW0 |= UCTXSTP;
}

