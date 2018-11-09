/*
 * G8RTOS_IPC.c
 *
 *  Created on: Jan 10, 2017
 *      Author: Daniel Gonzalez
 */
#include <stdint.h>
#include "msp.h"
#include "G8RTOS_IPC.h"
#include "G8RTOS_Semaphores.h"

/*********************************************** Defines ******************************************************************************/

#define FIFOSIZE 16
#define MAX_NUMBER_OF_FIFOS 4

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/*
 * FIFO struct will hold
 *  - buffer
 *  - head
 *  - tail
 *  - lost data
 *  - current size
 *  - mutex
 */

/* Create FIFO struct here */
typedef struct FIFO{
	uint32_t buffer[FIFOSIZE];
	uint32_t *head;
	uint32_t *tail;
	uint32_t lostData;
	semaphore_t currentSize;
	semaphore_t mutex;
} FIFO_t;

/* Array of FIFOS */
static FIFO_t FIFOs[4];

/*********************************************** Data Structures Used *****************************************************************/

/*
 * Initializes FIFO Struct
 */
int G8RTOS_InitFIFO(uint32_t FIFOIndex)
{
    FIFOs[FIFOIndex].head = &FIFOs[FIFOIndex].buffer;
    FIFOs[FIFOIndex].tail = &FIFOs[FIFOIndex].buffer;
    FIFOs[FIFOIndex].lostData = 0;
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].currentSize, 0);
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].mutex, 1);
}

/*
 * Reads FIFO
 *  - Waits until CurrentSize semaphore is greater than zero
 *  - Gets data and increments the head ptr (wraps if necessary)
 * Param: "FIFOChoice": chooses which buffer we want to read from
 * Returns: uint32_t Data from FIFO
 */
uint32_t readFIFO(uint32_t FIFOChoice)
{
    uint32_t readData;
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].currentSize); //ensure there is actually data to read and decrement currentSize
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].mutex); //ensure we can read from FIFO
    readData = *(FIFOs[FIFOChoice].head); //read data
    ++FIFOs[FIFOChoice].head;
    if(FIFOs[FIFOChoice].head >= &FIFOs[FIFOChoice].buffer[FIFOSIZE]){
    	FIFOs[FIFOChoice].head = &FIFOs[FIFOChoice].buffer[0]; //wrap head
    }
    G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex); //inc mutex
    return readData;
}

/*
 * Writes to FIFO
 *  Writes data to Tail of the buffer if the buffer is not full
 *  Increments tail (wraps if ncessary)
 *  Param "FIFOChoice": chooses which buffer we want to read from
 *        "Data': Data being put into FIFO
 *  Returns: error code for full buffer if unable to write
 */
int writeFIFO(uint32_t FIFOChoice, uint32_t Data)
{
	G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].mutex);
	if(FIFOs[FIFOChoice].currentSize >= FIFOSIZE){
		++FIFOs[FIFOChoice].lostData;
        G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);
		int Error = -1;
		return Error;
	}
	else{
		*(FIFOs[FIFOChoice].tail) = Data;
		++FIFOs[FIFOChoice].tail;
		if(FIFOs[FIFOChoice].tail >= &FIFOs[FIFOChoice].buffer[FIFOSIZE]){
			FIFOs[FIFOChoice].tail = &FIFOs[FIFOChoice].buffer[0]; //wrap tail
		}
		//data has been written
		G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].currentSize);
		G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);
		int Success = 1;
		return Success;
	}
}

