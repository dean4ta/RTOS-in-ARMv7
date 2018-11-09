#include "RGBLeds.h"
#include "msp.h"
// Function definitions go here

/*
 * I2C transmit
 * inputs:
 * unit;        Red, Green, or Blue driver on bus
 * memAddr;     in the slave addressed to
 * data;        to send to the slave at memAddr
 */
void i2cTxWr(uint32_t unit, uint32_t memAddr, uint32_t data[], uint32_t dataCount)
{
    //set slave address
    UCB2I2CSA = (unit + 0x60) & 0xff;
    //Generate start condition
    UCB2CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    //wait for start condition to be set
    while(UCB2CTLW0 & EUSCI_B_CTLW0_TXSTT);

    //send memory address to slave
    UCB2TXBUF = memAddr & 0xff;
    //wait for TXBUF to empty
    while(!(UCB2IFG & UCTXIFG0));

    //send data
    uint32_t i = 0;
    while(dataCount > 0)
    {
        UCB2TXBUF = data[i] & 0xff;
        //wait for TXBUF to empty
        while(!(UCB2IFG & UCTXIFG0));
        --dataCount;
        ++i;
    }

    //Generate Stop condition
    UCB2CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    //wait for stop condition to be confirmed
    while(UCB2CTLW0 & EUSCI_B_CTLW0_TXSTP);
}

static void LP3943_ColorSet(uint32_t unit, uint32_t PWM_DATA)
{
    unit += 0x60;

    //send(unit, write, PWM_DATA)
    //set slave address
    UCB2I2CSA = unit & 0xff;

    //wait for not busy signal


    //send data
    UCB2TXBUF = PWM_DATA & 0xff;
}

/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode. The operating modes are ON, OFF, PWM1, and PWM2
 * Just turn on the desired LEDs
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA)
{
    //unit = (unit + 0x60) & 0xff;
    uint32_t memAddress = 0x06 | (1 << 4);
    uint32_t longChosen = 0x0000 | LED_DATA;
    uint32_t expandedLEDs = 0x0000;
    expandedLEDs = ((longChosen & 1) << 0) | ((longChosen & 2) << 1) | ((longChosen & 4) << 2) | ((longChosen & 8) << 3) | ((longChosen & 16) << 4) | ((longChosen & 32) << 5) | ((longChosen & 64) << 6) | ((longChosen & 128) << 7);
    expandedLEDs |= ((longChosen & 256) << 8) | ((longChosen & 512) << 9) | ((longChosen & 1024) << 10) | ((longChosen & 2048) << 11) | ((longChosen & 4096) << 12) | ((longChosen & 8192) << 13) | ((longChosen & 16384) << 14) | ((longChosen & 32768) << 15);
    uint32_t dataTest[] = {0x00, 0x00, 0x00, 0x00};
    dataTest[0] = expandedLEDs & 0xff;
    dataTest[1] = (expandedLEDs >> 8) & 0xff;
    dataTest[2] = (expandedLEDs >> 16) & 0xff;
    dataTest[3] = (expandedLEDs >> 24) & 0xff;
    uint32_t size = 4;
    i2cTxWr(unit, memAddress, dataTest, size);
}

/*
 * init_RGBLEDS
 * perform the necessary initializations for RGB LEDs
 */
// slau356h section 26
// UCBxCTLW0
#define SevenbitMasterAddr = (0 << 15)
#define SevenbitSlaveAddr = (0 << 14)
#define SingleMaster = (0 << 13)
#define SetAsMaster = (1 << 11)
#define i2cMode = (3 << 9)
#define syncMode = (1 << 8)
#define clockSource = (2 << 6)
#define TxOrRx = (1 << 4)

void init_RGBLEDS()
{
    //uint16_t UNIT_OFF = 0x0000;

    // Software reset enabled
    UCB2CTLW0 = UCSWRST;

    // Init i2c master
    // set as master, i2c mode, clock sync, smclk cource, Tx
    //UCB2CTLW0 |= (SetAsMaster | i2cMode | syncMode | clockSource | TxOrRx);
    UCB2CTLW0 |= (1 << 11) | (3 << 9) | (1 << 8) | (2 << 6) | (1 << 4);

    // Set the Fclk as 400kHz
    // Presumes SMCLK is source  //30
    UCB2BRW = 60;

    // Set pins to i2c mode
    // slas826e p 160
    // Set P3.6 as UCB2_SDA and P3.7 as UCB2_SLC
    P3SEL0 |= 0xc0;
    P3SEL1 &= ~0xc0;

    //enable interrupts
    //UCB2IE = (1 << 13); //UCTXIE3 transmit interrupt enable 3

    // Bitwise anding of all bits except UCSSWRST (end UCB2CTLW0 init)
    UCB2CTLW0 &= ~UCSWRST;

    //LP3943_LedModeSet(RED, UNIT_OFF);
    //LP3943_LedModeSet(GREEN, UNIT_OFF);
    //LP3943_LedModeSet(BLUE, UNIT_OFF);
}
