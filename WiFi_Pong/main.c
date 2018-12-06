/*
 * main.c
 *
 *  Created on: October 28, 2018
 *     Authors: Dean and Isaac
 */
#include "cc3100_usage.h"
#include "msp.h"
#include <driverlib.h>
#include <stdio.h>
#include "BSP.h"
#include "G8RTOS.h"
#include "LCDlib.h"
#include "Game.h"

#define playerType Host
void main(void)
{
    G8RTOS_Init();
    G8RTOS_Launch();
    while(1){
    }

}
