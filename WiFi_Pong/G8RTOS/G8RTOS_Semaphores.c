/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_CriticalSection.h"
#include "G8RTOS.h"

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
    uint32_t PriMask;
    PriMask = StartCriticalSection();
	*s = value;
	EndCriticalSection(PriMask);
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
    uint32_t PriMask;
    PriMask = StartCriticalSection();
    // The semaphore is available, decrement it
    (*s)--;

    //If semaphore is less than zero, it is unavailable and the thread becomes blocked
    if((*s)< 0)
    {
        //block thread
        CurrentlyRunningThread->blocked = s;

        //Yield to allow another thread to run
        EndCriticalSection(PriMask);
        StartContextSwitch();
        return 0;
    }
    EndCriticalSection(PriMask);
}
/*
 * Signals the completion of the usage of a semaphore
 * 	- Increments the semaphore value by 1
 * Param "s": Pointer to semaphore to be signalled
 * THIS IS A CRITICAL SECTION
 */

void G8RTOS_SignalSemaphore(semaphore_t *s)
{
    uint32_t PriMask;
    PriMask = StartCriticalSection();

    (*s)++;

    if((*s) <= 0)
    {
        tcb_t *pt = (tcb_t *)CurrentlyRunningThread->Next_TCB;
        while(pt->blocked != s)
        {
            pt = pt->Next_TCB;
        }
        pt->blocked = 0;
    }
    EndCriticalSection(PriMask);
}
/*********************************************** Public Functions *********************************************************************/

void G8RTOS_ReleaseSemaphore(semaphore_t *s)
{
    uint32_t PriMask;
    PriMask = StartCriticalSection();
    (*s)++;
    EndCriticalSection(PriMask);
}
