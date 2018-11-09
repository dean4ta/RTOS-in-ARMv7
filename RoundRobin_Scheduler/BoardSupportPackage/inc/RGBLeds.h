#ifndef RGBLEDS_H_
#define RGBLEDS_H_

#include "msp.h"
// Function declarations go here

//??? Header file or c file with definitions
/* Enums for RGB LEDs */
typedef enum device
{
    BLUE = 0,
    GREEN = 1,
    RED = 2
} unit_desig;

/*
 * LP3943_ColorSet
 * This function will set the frequencies of the PWM duty cycle
 * for each register of the specified unit
 */
static void LP3943_ColorSet(uint32_t unit, uint32_t PWM_DATA);

/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode. The operating modes are ON, OFF, PWM1, and PWM2
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA);

/*
 * I2C transmit
 * inputs:
 * unit;        Red, Green, or Blue driver on bus
 * memAddr;     in the slave addressed to
 * data;        to send to the slave at memAddr
 */
void i2cTxWr(uint32_t unit, uint32_t memAddr, uint32_t data[], uint32_t dataCount);

/*
 * init_RGBLEDS
 * perform the necesarry initializations for RGB LEDs
 */
// slau356h section 26
// UCBxCTLW0
//??? necessary for header file?
/*
#define SevenbitMasterAddr = (0 << 15)
#define SevenbitSlaveAddr = (0 << 14)
#define SingleMaster = (0 << 13)
#define SetAsMaster = (1 << 11)
#define i2cMode = (3 << 9)
#define syncMode = (1 << 8)
#define clockSource = (2 << 6)
#define TxOrRx = (1 << 4)
*/

void init_RGBLEDS();

#endif //RGBLEDS_H_
