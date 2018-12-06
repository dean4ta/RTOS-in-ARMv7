
#include <G8RTOS_Scheduler.h>
#include "msp.h"
#include <driverlib.h>
#include <stdio.h>
#include "BSP.h"
#include "G8RTOS.h"
#include "Bean.h"


uint16_t accel2LED(int16_t accelData,int16_t multiplier){
    uint16_t LEDData = 0;
    if(accelData>=0){
        for(int i=0;i<8;i++){
            if(accelData > i*multiplier){
                LEDData |= (0x0100<<(i));
            }
            else{
                break;
            }
        }
    }
        else{
            for(int i=0;i<8;i++){
                if(-accelData > i*multiplier){
                    LEDData |= (0x0080>>(i));
                }
                else{
                    break;
                }
        }
    }
        return LEDData;
    }

uint16_t light2LED(uint16_t lightData,int16_t multiplier){
    uint16_t LEDData = 0;
    if(lightData>=0){
        for(int i=0;i<16;i++){
            if(lightData>(i*multiplier)){
                LEDData |= (0x8000>>(i));
            }
            else{
                break;
            }
        }
    }
        return LEDData;
    }
