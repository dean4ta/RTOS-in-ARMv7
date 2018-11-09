#include <UART_deanCustom.h>
#include "msp.h"
#include "BSP.h"
#include "driverlib.h"
#include "G8RTOS_Empty_Lab2/G8RTOS.h"


/*********************************************** Public Variables *********************************************************************/
int Count0, Count1, Count2, Count3;
semaphore_t s_led;
semaphore_t s_i2cSensor;
uint32_t green = 0x00000001;
uint32_t blue = 0x00000000;
uint32_t red = 0x00000002;

/*********************************************** Public Functions *********************************************************************/

/**************************************************** Tasks **************************************************************************/
void Task0(void){
	Count0 = 0;
    int16_t accelerometerX;
    uint16_t led;
	while(1){
		Count0++;
	    G8RTOS_WaitSemaphore(&s_i2cSensor);
		//read shit
	    bmi160_read_accel_x(&accelerometerX);
		//inc/ signal semaphore
	    G8RTOS_SignalSemaphore(&s_i2cSensor);
		//with read shit determine LED pattern
	    if(accelerometerX > 14000)
            led = 0xFF00;
        else if(accelerometerX > 12000)
            led = 0x7F00;
        else if(accelerometerX > 10000)
            led = 0x3F00;
        else if(accelerometerX > 8000)
            led = 0x1F00;
        else if(accelerometerX > 6000)
            led = 0x0F00;
        else if(accelerometerX > 4000)
            led = 0x0700;
        else if(accelerometerX > 2000)
            led = 0x0300;
        else if(accelerometerX > 0)
            led = 0x0100;
        else if(accelerometerX > -2000)
            led = 0x0080;
        else if(accelerometerX > -4000)
            led = 0x00C0;
        else if(accelerometerX > -6000)
            led = 0x00E0;
        else if(accelerometerX > -8000)
            led = 0x00F0;
        else if(accelerometerX > -10000)
            led = 0x00F8;
        else if(accelerometerX > -12000)
            led = 0x00FC;
        else if(accelerometerX > -14000)
            led = 0x00FE;
        else if(accelerometerX > -16000)
            led = 0x00FF;
		//wait semaphore led
		G8RTOS_WaitSemaphore(&s_led);
		//put pattern on LED
		LP3943_LedModeSet(red, led);
		//inc/ signal semaphore
		G8RTOS_SignalSemaphore(&s_led);
		//optional for loop wait
		for(int i=0;i<3000;++i){}
	}
}
void Task1(void){
	Count1 = 0;
	int16_t	gyroZ;
	uint16_t led;
	while(1){
		Count1++;
		G8RTOS_WaitSemaphore(&s_i2cSensor);
        //read shit
		bmi160_read_gyro_z(&gyroZ);
        //inc/ signal semaphore
        G8RTOS_SignalSemaphore(&s_i2cSensor);
        //with read shit determine LED pattern
		if(gyroZ > 7000)
			led = 0xFF00;
		else if(gyroZ > 6000)
			led = 0x7F00;
		else if(gyroZ > 5000)
			led = 0x3F00;
		else if(gyroZ > 4000)
			led = 0x1F00;
		else if(gyroZ > 3000)
			led = 0x0F00;
		else if(gyroZ > 2000)
			led = 0x0700;
		else if(gyroZ > 1000)
			led = 0x0300;
		else if(gyroZ > 0)
			led = 0x0100;
		else if(gyroZ > -1000)
			led = 0x0080;
		else if(gyroZ > -2000)
			led = 0x00C0;
		else if(gyroZ > -3000)
			led = 0x00E0;
		else if(gyroZ > -4000)
			led = 0x00F0;
		else if(gyroZ > -5000)
			led = 0x00F8;
		else if(gyroZ > -6000)
			led = 0x00FC;
		else if(gyroZ > -7000)
			led = 0x00FE;
		else if(gyroZ > -8000)
			led = 0x00FF;
        //wait semaphore led
        G8RTOS_WaitSemaphore(&s_led);
        //put pattern on LED
        LP3943_LedModeSet(blue, led);
        //inc/ signal semaphore
        G8RTOS_SignalSemaphore(&s_led);
        //optional for loop wait
        for(int i=0;i<3000;++i){}
	}
}
void Task2(void){
	Count2 = 0;
	uint16_t lightData;
	uint16_t led;
	while(1){
		Count2++;
		while(!sensorOpt3001Read(&lightData));
		if(lightData > 49000)
			led = 0xFFFF;
		else if(lightData > 45500)
			led = 0x7FFF;
		else if(lightData > 42000)
			led = 0x3FFF;
		else if(lightData > 38500)
			led = 0x1FFF;
		else if(lightData > 35500)
			led = 0x0FFF;
		else if(lightData > 31500)
			led = 0x07FF;
		else if(lightData > 28000)
			led = 0x03FF;
		else if(lightData > 26000)
			led = 0x01FF;
		else if(lightData > 24500)
			led = 0x00FF;
		else if(lightData > 21000)
			led = 0x007F;
		else if(lightData > 17500)
			led = 0x003F;
		else if(lightData > 14000)
			led = 0x001F;
		else if(lightData > 10500)
			led = 0x000F;
		else if(lightData > 7000)
			led = 0x0007;
		else if(lightData > 3500)
			led = 0x0003;
		else if(lightData < 3500 )
			led = 0x0001;
		//wait semaphore led
        G8RTOS_WaitSemaphore(&s_led);
        //put pattern on LED
        LP3943_LedModeSet(green, led);
        //inc/ signal semaphore
        G8RTOS_SignalSemaphore(&s_led);
        //optional for loop wait
        for(int i=0;i<3000;++i){}
	}
}
void Task3(void){
	//should never enter with MAX_THREADS set to 3
    Count3 = 0;
	while(1){
		Count3++;

	}
}

/**************************************************** Tasks **************************************************************************/


/**
 * main.c
 */
void main(void)
{
    // stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    uint32_t clockFreqBefore, clockFreqAfter;
	clockFreqBefore =  ClockSys_GetSysFreq();
	G8RTOS_Init();
	clockFreqAfter = ClockSys_GetSysFreq();
	
	//init semaphores
    G8RTOS_InitSemaphore(&s_led, 1);
    G8RTOS_InitSemaphore(&s_i2cSensor, 1);

	//Testing AddThread function (MAX_THREADS set to 3)
	int addOneThread, addSecondThread, addThirdThread, addFourthThread;
	addOneThread = G8RTOS_AddThread((uint32_t)(&Task0));
	addSecondThread = G8RTOS_AddThread((uint32_t)(&Task1));
	addThirdThread = G8RTOS_AddThread((uint32_t)(&Task2));
	addFourthThread = G8RTOS_AddThread((uint32_t)(&Task3));

	//Test init accel
	BSP_InitBoard();
	//bmi160_initialize_sensor();
	int16_t accelData;
	bmi160_read_accel_x(&accelData);

	//Launch function
	G8RTOS_Launch();
	//nothing after Launch will run


	uartInit();
	char str[255];
	snprintf(str, 255, "Clock Before MaxFreq: %d \n \r", clockFreqBefore);
	uartTransmitString(str);
	snprintf(str, 255, "Clock After MaxFreq: %d \n \r", clockFreqAfter);
	uartTransmitString(str);
	if(addOneThread==1){
		snprintf(str, 255, "Successfully added thread: %d \n \r", addOneThread);
	}
	else{
		snprintf(str, 255, "Failed to add thread: %d \n \r", addOneThread);
	}
	uartTransmitString(str);
	snprintf(str, 255, "Second Thread result: %d \n \r", addSecondThread);
	uartTransmitString(str);
	snprintf(str, 255, "Third Thread result: %d \n \r", addThirdThread);
	uartTransmitString(str);
	snprintf(str, 255, "Fourth Thread result: %d \n \r", addFourthThread);
	uartTransmitString(str);
	while(1);
}

