/*
 * G8RTOS.h
 */

#ifndef G8RTOS_H_
#define G8RTOS_H_

#include <stdint.h>
#include "driverlib.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_Scheduler.h"
#include "G8RTOS_Structures.h"
#include "G8RTOS_IPC.h"


#define ICSR (*((volatile unsigned int*)(0xe000ed04)))
#define ICSR_PENDSVSET (1 << 28)
#define joystickFIFO 0
#define tempFIFO 1
#define lightFIFO 2


#endif /* G8RTOS_H_ */
