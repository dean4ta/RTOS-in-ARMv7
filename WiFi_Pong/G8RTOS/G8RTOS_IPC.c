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
#define FIFOMASK (FIFOSIZE - 1)
#define MAX_NUMBER_OF_FIFOS 4
#define ERROR 1

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
typedef struct FIFO_t{
    int32_t buffer[FIFOSIZE];
    int32_t *head;
    int32_t *tail;
    uint32_t lost_data;
    semaphore_t size;
    semaphore_t mutex;
}FIFO_t;


/* Array of FIFOS */
static FIFO_t FIFOs[MAX_NUMBER_OF_FIFOS];

/*********************************************** Data Structures Used *****************************************************************/

/*
 * Initializes FIFO Struct
 */
int G8RTOS_InitFIFO(uint32_t FIFOIndex)
{
    FIFOs[FIFOIndex].head = &FIFOs[FIFOIndex].buffer;
    FIFOs[FIFOIndex].tail = &FIFOs[FIFOIndex].buffer;
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].size, 0);
    G8RTOS_InitSemaphore(&FIFOs[FIFOIndex].mutex, 1);
    FIFOs[FIFOIndex].lost_data = 0;
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
    uint32_t readValue;
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].mutex);
    G8RTOS_WaitSemaphore(&FIFOs[FIFOChoice].size);
    readValue = *(FIFOs[FIFOChoice].head);
    FIFOs[FIFOChoice].head++;
    if((uint32_t)FIFOs[FIFOChoice].head > (((uint32_t)&FIFOs[FIFOChoice].buffer) + ((FIFOSIZE-1)*4)))
    {
        FIFOs[FIFOChoice].head =  (uint32_t)&FIFOs[FIFOChoice].buffer;
    }
    G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].mutex);
    return readValue;
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

    if(FIFOs[FIFOChoice].size == FIFOSIZE){
        FIFOs[FIFOChoice].lost_data++;
        return ERROR;
    }
    else{
        *(FIFOs[FIFOChoice].tail) = Data;
        FIFOs[FIFOChoice].tail++;
        if((uint32_t)FIFOs[FIFOChoice].tail > (((uint32_t)&FIFOs[FIFOChoice].buffer) + ((FIFOSIZE-1)*4)))
        {
            FIFOs[FIFOChoice].tail =  (uint32_t)&FIFOs[FIFOChoice].buffer;
        }
        G8RTOS_SignalSemaphore(&FIFOs[FIFOChoice].size);
        return 0;
    }

}

