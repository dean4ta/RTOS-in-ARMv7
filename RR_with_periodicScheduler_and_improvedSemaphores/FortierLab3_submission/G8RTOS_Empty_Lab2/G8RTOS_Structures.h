/*
 * G8RTOS_Structure.h
 *
 *  Created on: Jan 12, 2017
 *      Author: Raz Aloni
 */

#ifndef G8RTOS_STRUCTURES_H_
#define G8RTOS_STRUCTURES_H_

#include "G8RTOS.h"
#include <stdbool.h>

/*********************************************** Data Structure Definitions ***********************************************************/

/*
 *  Thread Control Block:
 *      - Every thread has a Thread Control Block
 *      - The Thread Control Block holds information about the Thread Such as the Stack Pointer, Priority Level, and Blocked Status
 *      - For Lab 2 the TCB will only hold the Stack Pointer, next TCB and the previous TCB (for Round Robin Scheduling)
 */

/* Create tcb struct here */

/*********************************************** Data Structure Definitions ***********************************************************/
typedef struct tcb{
	int32_t *sp;	//pointer to stack, valid for threads not running?
	struct tcb *next;	//linked-list pointer
	struct tcb *prev;	//linked-list pointer
	semaphore_t *blocked;	//blocking semaphore
	bool asleep;	//is prescribed sleep
	uint32_t sleepCount; //how long to sleep in ms
} tcb_t;

/*
 *  Periodic Thread Control Block:
 *      - Holds a function pointer that points to the periodic thread to be executed
 *      - Has a period in us
 *      - Holds Current time
 *      - Contains pointer to the next periodic event - linked list
 */

/* Create periodic thread struct here */

/*********************************************** Data Structure Definitions ***********************************************************/
typedef struct ptcb{
    void (*handler)(void); //function pointer to periodic event handler
    uint32_t period;
    uint32_t executeTime;
    uint32_t currentTime;
    struct ptcb *prev;
    struct ptcb *next;
} ptcb_t;

/*********************************************** Public Variables *********************************************************************/

tcb_t * CurrentlyRunningThread;

/*********************************************** Public Variables *********************************************************************/




#endif /* G8RTOS_STRUCTURES_H_ */
