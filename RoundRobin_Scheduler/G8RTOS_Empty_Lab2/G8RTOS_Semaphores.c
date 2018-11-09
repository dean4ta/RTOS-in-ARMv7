/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
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
    while(*s == 0){
    	EndCriticalSection(status);//enable interrupts for a period of time
    	//wait some time to let resource free up
    	for(int i=0; i < 128; ++i){}
    	status = StartCriticalSection();//disable interrupts to check *s
    }
    *s = *s-1;
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
	(*s)++;
    EndCriticalSection(status);
}

/*********************************************** Public Functions *********************************************************************/


