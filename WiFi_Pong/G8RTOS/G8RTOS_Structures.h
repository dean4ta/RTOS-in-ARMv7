/*
 * G8RTOS_Structure.h
 *
 *  Created on: Jan 12, 2017
 *      Author: Raz Aloni
 */

#ifndef G8RTOS_STRUCTURES_H_
#define G8RTOS_STRUCTURES_H_

#include "G8RTOS.h"
#include "G8RTOS_Scheduler.h" //included in G8RTOS.h

/*********************************************** Data Structure Definitions ***********************************************************/

#define MAX_NAME_LENGTH 16

typedef uint32_t threadId_t;

/*
 *  Thread Control Block:
 *      - Every thread has a Thread Control Block
 *      - The Thread Control Block holds information about the Thread Such as the Stack Pointer, Priority Level, and Blocked Status
 *      - For Lab 2 the TCB will only hold the Stack Pointer, next TCB and the previous TCB (for Round Robin Scheduling)
 */

/* Create tcb struct here */
typedef struct tcb_t{
    struct tcb_t* Previous_TCB;
    struct tcb_t* Next_TCB;
    int32_t Stack_Pointer;
    semaphore_t *blocked;
    bool Asleep;
    uint32_t Sleep_Count;
    uint8_t Priority;
    bool Alive;
    threadId_t threadID;
    char threadName[MAX_NAME_LENGTH];
    bool isBall;
    uint8_t ballNum;
}tcb_t;



/*
 *  Periodic Thread Control Block:
 *      - Holds a function pointer that points to the periodic thread to be executed
 *      - Has a period in us
 *      - Holds Current time
 *      - Contains pointer to the next periodic event - linked list
 */
typedef struct ptcb_t{
    void (*Handler)(void);
    uint32_t Period;
    uint32_t Excecute_Time;
    uint32_t Current_Time;
    struct ptcb_t* Previous_PTCB;
    struct ptcb_t* Next_PTCB;
}ptcb_t;


/*********************************************** Data Structure Definitions ***********************************************************/


/*********************************************** Public Variables *********************************************************************/

tcb_t * CurrentlyRunningThread;

/*********************************************** Public Variables *********************************************************************/




#endif /* G8RTOS_STRUCTURES_H_ */
