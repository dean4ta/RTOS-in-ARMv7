#include <UART_deanCustom.h>
#include "msp.h"
#include "BSP.h"
#include "driverlib.h"
#include "G8RTOS_Empty_Lab2/G8RTOS.h"


/*********************************************** Public Variables *********************************************************************/
int Count0, Count1, Count2, Count3, CountP0;
semaphore_t s_led;
semaphore_t s_i2cSensor;
uint32_t green = 0x00000001;
uint32_t blue = 0x00000000;
uint32_t red = 0x00000002;
int16_t accelerometerX;
static int32_t globalLight = 1;

/*********************************************** Public Functions *********************************************************************/

/**************************************************** Threads **************************************************************************/

void Periodic_Thread0(void){
    int16_t x_joystick;
    int16_t y_joystick;
    GetJoystickCoordinates(&x_joystick, &y_joystick);
    writeFIFO(joystickFIFO, x_joystick);
    BITBAND_PERI(P1->OUT, 0) ^= 1;
}

void Background_Thread0(void){
    int32_t temp;
    s32 uncomp;
    while(1){
    G8RTOS_WaitSemaphore(&s_i2cSensor);
    bme280_read_uncomp_temperature(&uncomp);
    G8RTOS_SignalSemaphore(&s_i2cSensor);
    temp = bme280_compensate_temperature_int32(uncomp);
    writeFIFO(tempFIFO, temp);
    BITBAND_PERI(P2->OUT, 0) ^= 1;
    sleep(500);
    }
}

void Background_Thread1(void){
    uint16_t lightData;
    while(1){
    G8RTOS_WaitSemaphore(&s_i2cSensor);
    sensorOpt3001Read(&lightData);
    G8RTOS_SignalSemaphore(&s_i2cSensor);
    writeFIFO(lightFIFO, lightData);
    BITBAND_PERI(P2->OUT, 1) ^= 1;
    sleep(200);
    }
}

uint32_t calcRMS(uint32_t input){
    uint32_t x1;
    uint32_t x0;
    uint32_t n = input;
    if(n>1){
        x1 = n;
        x0 = n;
        do{
            x0 = x1;
            x1 = ((x0 + (n/x0)) >> 1);
        }while((x1-x0) >= 1);
        return x1;
    }
}

void Background_Thread2(void){
    uint32_t lightData;
    uint32_t lightFloat;
    while(1){
        for(int i = 0; i<32 ; i++){
            lightData = readFIFO(lightFIFO);
            lightFloat += (lightData*lightData);
        }
        lightFloat /= 32;
        lightFloat = calcRMS(lightFloat);
        if(lightData < 5000){
            globalLight = 1;
        }else{
            globalLight = 0;
        }
        sleep(200);
    }
}

int32_t tempF;
void Background_Thread3(void){
    while(1){
    tempF = readFIFO(tempFIFO);
    tempF = (tempF*1.8) + 3200; //convert C to F
    G8RTOS_WaitSemaphore(&s_led);
    if(tempF>8400){
        LP3943_LedModeSet(RED,0xFF00);
        LP3943_LedModeSet(BLUE,0x0000);
    }
    else if(tempF>8100){
        LP3943_LedModeSet(RED,0xFE00);
        LP3943_LedModeSet(BLUE,0x0100);
    }
    else if(tempF>7800){
        LP3943_LedModeSet(RED,0xFC00);
        LP3943_LedModeSet(BLUE,0x0300);
    }
    else if(tempF>7500){
        LP3943_LedModeSet(RED,0xF800);
        LP3943_LedModeSet(BLUE,0x0700);
    }
    else if(tempF>7200){
        LP3943_LedModeSet(RED,0xF000);
        LP3943_LedModeSet(BLUE,0x0F00);
    }
    else if(tempF>6900){
        LP3943_LedModeSet(RED,0x8800);
        LP3943_LedModeSet(BLUE,0x1100);
    }
    else if(tempF>6600){
        LP3943_LedModeSet(RED,0x8800);
        LP3943_LedModeSet(BLUE,0x1100);
    }
    else if(tempF>6300){
        LP3943_LedModeSet(RED,0x8800);
        LP3943_LedModeSet(BLUE,0x1100);
    }
    else{
        LP3943_LedModeSet(RED,0x8800);
        LP3943_LedModeSet(BLUE,0x1100);
    }
    G8RTOS_SignalSemaphore(&s_led);
    for(int i=0;i<3000;i++);

    }
}
int16_t joystick_avg;
void Background_Thread4(void){
    while(1){
    joystick_avg = (joystick_avg + readFIFO(joystickFIFO))*0.5;
    G8RTOS_WaitSemaphore(&s_led);
    if(joystick_avg>6000){
        LP3943_LedModeSet(GREEN,0x000F);
    }
    else if(joystick_avg>4000){
        LP3943_LedModeSet(GREEN,0x000E);
    }
    else if(joystick_avg>2000){
        LP3943_LedModeSet(GREEN,0x000C);
    }
    else if(joystick_avg>500){
        LP3943_LedModeSet(GREEN,0x0008);
    }
    else if(joystick_avg>-500){
        LP3943_LedModeSet(GREEN,0x0000);
    }
    else if(joystick_avg>-2000){
        LP3943_LedModeSet(GREEN,0x0010);
    }
    else if(joystick_avg>-4000){
        LP3943_LedModeSet(GREEN,0x0030);
    }
    else if(joystick_avg>-6000){
        LP3943_LedModeSet(GREEN,0x0070);
    }
    else{
        LP3943_LedModeSet(GREEN,0x00F0);
    }
    G8RTOS_SignalSemaphore(&s_led);
        for(int i=0;i<3000;i++);
    }
}

void Periodic_Thread1(void){
    if(globalLight){
        BackChannelPrintIntVariable("Temperature in F",(tempF/100));
        BackChannelPrintIntVariable("Joystick Position",joystick_avg);
    }
}
void Background_Thread5(void){ //idle thread
    while(1){

    }
}



/*
void Task0(void){
	Count0 = 0;
    uint16_t led;
	while(1){
		Count0++;
	    G8RTOS_WaitSemaphore(&s_i2cSensor);
		//read shit
	    bmi160_read_accel_x(&accelerometerX);
		//inc/ signal semaphore
	    G8RTOS_SignalSemaphore(&s_i2cSensor);
	    writeFIFO(TEST, accelerometerX);
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
    int32_t testFIFO;
	Count2 = 0;
	uint16_t lightData;
	uint16_t led;
	while(1){
		Count2++;
        G8RTOS_WaitSemaphore(&s_i2cSensor);
		//while(!sensorOpt3001Read(&lightData));
		sensorOpt3001Read(&lightData);
        G8RTOS_SignalSemaphore(&s_i2cSensor);
        testFIFO = readFIFO(TEST);
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
        uint32_t sleepFor = 250;
        sleep(sleepFor);
	}
}

void Task3(void){ //periodic thread
    ++Count3;

}

void idleTask(void){
    Count3 = 0;
    while(1){
        Count3++;

    }
}
*/

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
	
	//init FIFOs
	G8RTOS_InitFIFO(joystickFIFO);
	G8RTOS_InitFIFO(tempFIFO);
	G8RTOS_InitFIFO(lightFIFO);

	//init semaphores
    G8RTOS_InitSemaphore(&s_led, 1);
    G8RTOS_InitSemaphore(&s_i2cSensor, 1);

    //add round robin threads
    int addThread0, addThread1, addThread2, addThread3, addThread4, addThread5;
    addThread0 = G8RTOS_AddThread((uint32_t)(&Background_Thread0));
    addThread1 = G8RTOS_AddThread((uint32_t)(&Background_Thread1));
    addThread2 = G8RTOS_AddThread((uint32_t)(&Background_Thread2));
    addThread3 = G8RTOS_AddThread((uint32_t)(&Background_Thread3));
    addThread4 = G8RTOS_AddThread((uint32_t)(&Background_Thread4));
    addThread5 = G8RTOS_AddThread((uint32_t)(&Background_Thread5));


	int addPThread0, addPThread1;
	uint32_t P0_time = 100;
	uint32_t P1_time = 1000;
	addPThread0 = G8RTOS_AddPeriodicEvent((uint32_t)(&Periodic_Thread0), P0_time);
	addPThread1 = G8RTOS_AddPeriodicEvent((uint32_t)(&Periodic_Thread1), P1_time);


    BSP_InitBoard();

	/*
	//Test init accel
	//bmi160_initialize_sensor();
	int16_t accelData;
	bmi160_read_accel_x(&accelData);
	 */

	//Launch function
	G8RTOS_Launch();
	//nothing after Launch will run

	/*
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
	*/
	while(1);
}

