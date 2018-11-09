/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_CriticalSection.h"

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes a semaphore to a given value
 * Param "s": Pointer to semaphore
 * Param "value": Value to initialize semaphore to
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value)
{
    int status; //used for saving the PRIMASK when disabling/ enabling interrupts
    status = StartCriticalSection();
	*s = value;
	EndCriticalSection(status);
}

/*
 * Waits for a semaphore to be available (value greater than 0)
 * 	- Decrements semaphore when available
 * 	- Spinlocks to wait for semaphore
 * Param "s": Pointer to semaphore to wait on
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_WaitSemaphore(semaphore_t *s)
{
    int status; //used for saving the PRIMASK when disabling/ enabling interrupts
    status = StartCriticalSection(); //disable interrupts to check *s
    (*s)--; //decrement the semaphore first
    if(*s < 0){
        // change the value of blocked in the CurrentlyRunningThread to the semaphore
        CurrentlyRunningThread->blocked = s; //value should no longer be 0
        //Start context switch
        EndCriticalSection(status);
        ICSR |= ICSR_PENDSVSET; //set PendSV flag
        return;
    }
    EndCriticalSection(status); //endable interrupts
}

/*
 * Signals the completion of the usage of a semaphore
 * 	- Increments the semaphore value by 1
 * Param "s": Pointer to semaphore to be signalled
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_SignalSemaphore(semaphore_t *s)
{
	int status;
    status = StartCriticalSection();
    (*s)++; //increment
    //check if semaphore is negative????? WHYYY??? to see if the semaphore is blocking anything
    if((*s) <= 0){ 
        //tcb_t *pt = (tcb_t *)&CurrentlyRunningThread->next;
        tcb_t *pt = (tcb_t *)CurrentlyRunningThread->next;
        while(pt->blocked != s){ //find the thread blocked by semaphore
            pt = pt->next;
        }
        pt->blocked = 0; //why should I unblock if the semaphore is saying the resource is unavailable
    }
    EndCriticalSection(status);
}

/*********************************************** Public Functions *********************************************************************/


