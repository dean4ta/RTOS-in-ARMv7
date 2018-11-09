/*
 * G8RTOS_Scheduler.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Scheduler.h"
//added by Dean 10/1/18
#include "G8RTOS_Structures.h"
//end add

/*
 * G8RTOS_Start exists in asm
 */
extern void G8RTOS_Start();

/* System Core Clock From system_msp432p401r.c */
extern uint32_t SystemCoreClock;

/*
 * Pointer to the currently running Thread Control Block
 */
extern tcb_t * CurrentlyRunningThread;

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Defines ******************************************************************************/

/* Status Register with the Thumb-bit Set */
#define THUMBBIT 0x01000000
#define R_14 0x14141414
#define R_12 0x12121212
#define R_3 0x03030303
#define R_2 0x02020202
#define R_1 0x01010101
#define R_0 0x00000000
#define R_11 0x11111111
#define R_10 0x10101010
#define R_9 0x09090909
#define R_8 0x08080808
#define R_7 0x07070707
#define R_6 0x06060606
#define R_5 0x05050505
#define R_4 0x04040404

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/* Thread Control Blocks
 *	- An array of thread control blocks to hold pertinent information for each thread
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/* Thread Stacks
 *	- An array of arrays that will act as invdividual stacks for each thread
 */
static int32_t threadStacks[MAX_THREADS][STACKSIZE];

/* Periodic Event Threads
 * - An array of periodic events to hold pertinent information for each thread
 */
static ptcb_t Pthread[MAXPTHREADS];

/*********************************************** Data Structures Used *****************************************************************/


/*********************************************** Private Variables ********************************************************************/

/*
 * Current Number of Threads currently in the scheduler
 */
static uint32_t NumberOfThreads;

/*
 * Current Number of Periodic Threads currently in the scheduler
 */
static uint32_t NumberOfPthreads;

/*********************************************** Private Variables ********************************************************************/


/*********************************************** Private Functions ********************************************************************/

/*
 * Initializes the Systick and Systick Interrupt
 * The Systick interrupt will be responsible for starting a context switch between threads
 * Param "numCycles": Number of cycles for each systick interrupt
 */
static void InitSysTick(uint32_t numCycles)
{
	// from lab 1 (1ms) 48MHz (48k ticks)
    SysTick_Config(48000);
}

/*
 * Chooses the next thread to run.
 * Lab 2 Scheduling Algorithm:
 * 	- Simple Round Robin: Choose the next running thread by selecting the currently running thread's next pointer
 */
void G8RTOS_Scheduler()
{
    tcb_t *temp_thread = CurrentlyRunningThread->next;
    while((temp_thread->blocked != 0) || (temp_thread->asleep == true)){ //check for next thread that is not blocked or asleep
    //while((temp_thread->blocked != 0)){ //check for next thread that is not blocked or asleep
        temp_thread = temp_thread->next;
    }
    CurrentlyRunningThread = temp_thread;
}

/*
 * SysTick Handler
 * Currently the Systick Handler will only increment the system time
 * and set the PendSV flag to start the scheduler
 *
 * In the future, this function will also be responsible for sleeping threads and periodic threads
 */
void SysTick_Handler()
{
	//check if periodic thread should start
    ++SystemTime;
    for (int i=0; i < MAXPTHREADS; ++i){
        if(Pthread[i].executeTime == SystemTime){
            //update execute time and call function
            Pthread[i].executeTime = Pthread[i].period+SystemTime;
            ((void(*)())Pthread[i].handler)();
        }
    }

    //check if a currently running thread has been asleep for prescribed time
    tcb_t *ptr = CurrentlyRunningThread;
    for(int i = 0; i < MAX_THREADS; ++i){
        if((ptr->asleep == true) && (ptr->sleepCount == SystemTime)){
           ptr->asleep = false;
        }
        ptr = ptr->next;
    }

    //inc SystemTime (globe var)
    //set PendSV flag
    ICSR |= ICSR_PENDSVSET;
}

/*********************************************** Private Functions ********************************************************************/


/*********************************************** Public Variables *********************************************************************/

/* Holds the current time for the whole System */
uint32_t SystemTime;

/*********************************************** Public Variables *********************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Sets variables to an initial state (system time and number of threads)
 * Enables board for highest speed clock and disables watchdog
 */
void G8RTOS_Init()
{
	//set clock speed
    ClockSys_SetMaxFreq();  //48MHz
    //init GPIO for i2c stuff
    uint32_t green = 0x00000001;
    uint32_t blue = 0x00000000;
    uint32_t red = 0x00000002;
    init_RGBLEDS();
    //temp
    uint16_t chosenREDs = 0b0000000000000001;
    LP3943_LedModeSet(red, chosenREDs);
    uint16_t chosenGRNs = 0b0000000000000010;
    LP3943_LedModeSet(green, chosenGRNs);
    uint16_t chosenBLUs = 0b0000000000000100;
    LP3943_LedModeSet(blue, chosenBLUs);

    //SystemTime to 0 (SystemTime incs every SysTick)
    SystemTime = 0;
    //set NumberOfThreads to 0
    NumberOfThreads = 0;
}

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes the Systick
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
int G8RTOS_Launch()
{
	//disable interrupt
    //enable systick
    SysTick_Config(48000);
    SysTick_enableInterrupt();
    NVIC_SetPriority(PendSV_IRQn, OSINT_PRIORITY);
    NVIC_SetPriority(SysTick_IRQn, OSINT_PRIORITY);
    //Start OS
    G8RTOS_Start();
}

/*
 * FUNCTION ADDED BY DEAN
 * Sets the initial stack for debugging purposes
 * Param "thread": chosen thread to initialize with values 
 *                 (recommended to use this function immediately after
 *                 calling G8RTOS_AddThread() and using NumberOfThreads
 *                 as thread parameter)
 */
void SetInitialStack(int thread){
    threadStacks[thread][STACKSIZE-1] = THUMBBIT;
    threadStacks[thread][STACKSIZE-3] = R_14;
    threadStacks[thread][STACKSIZE-4] = R_12;
    threadStacks[thread][STACKSIZE-5] = R_3;
    threadStacks[thread][STACKSIZE-6] = R_2;
    threadStacks[thread][STACKSIZE-7] = R_1;
    threadStacks[thread][STACKSIZE-8] = R_0;
    threadStacks[thread][STACKSIZE-9] = R_11;
    threadStacks[thread][STACKSIZE-10] = R_10;
    threadStacks[thread][STACKSIZE-11] = R_9;
    threadStacks[thread][STACKSIZE-12] = R_8;
    threadStacks[thread][STACKSIZE-13] = R_7;
    threadStacks[thread][STACKSIZE-14] = R_6;
    threadStacks[thread][STACKSIZE-15] = R_5;
    threadStacks[thread][STACKSIZE-16] = R_4;
}

/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are stil available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread to hold a "fake context"
 * 	- Sets stack tcb stack pointer to top of thread stack
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */
int G8RTOS_AddThread(void (*threadToAdd)(void))
{
	//Check MAX_THREADS and NumberOfThreads to see if thread can be added
    if(NumberOfThreads >= MAX_THREADS){
        int Error = -1;
        return Error; //at max thread capacity
    }
    else{
        if(NumberOfThreads == 0){ //adding the first thread
            //set currently runnning thread to val
            CurrentlyRunningThread = &threadControlBlocks[NumberOfThreads];

            threadControlBlocks[NumberOfThreads].sp = &threadStacks[NumberOfThreads][STACKSIZE-16]; //point to first value in the correct stack
            threadControlBlocks[NumberOfThreads].next = CurrentlyRunningThread; //based on round robin list
            threadControlBlocks[NumberOfThreads].prev = CurrentlyRunningThread;
        }
        else{ //adding other threads
            threadControlBlocks[NumberOfThreads].sp = &threadStacks[NumberOfThreads][STACKSIZE-16];
            threadControlBlocks[NumberOfThreads].prev = &threadControlBlocks[NumberOfThreads-1]; //point the previous thread to the current one
            threadControlBlocks[NumberOfThreads].next = &threadControlBlocks[0]; //make this the last thread
            //change up the previous thread next pointer value to the created thread
            threadControlBlocks[NumberOfThreads-1].next = &threadControlBlocks[NumberOfThreads];
            //change up the next thread previous pointer to the the thread created
            threadControlBlocks[0].prev = &threadControlBlocks[NumberOfThreads];
        }
        threadControlBlocks[NumberOfThreads].blocked = 0; //is this right? Blocked is a pointer...
        threadControlBlocks[NumberOfThreads].asleep = false;
        threadControlBlocks[NumberOfThreads].sleepCount = 0;
        int Success = 1; //successfully added thread
        SetInitialStack(NumberOfThreads);
        threadStacks[NumberOfThreads][STACKSIZE-2] = (int32_t)(threadToAdd);
        CurrentlyRunningThread = &threadControlBlocks[NumberOfThreads];
        ++NumberOfThreads;
        return Success;
    }
}

/*
 * Adds periodic threads to G8RTOS Scheduler
 * Function will initialize a periodic event struct to represent event.
 * The struct will be added to a linked list of periodic events
 * Param Pthread To Add: void-void function for P thread handler
 * Param period: period of P thread to add in ms
 * Returns: Error code for adding threads
 */
int G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period)
{
    //Check MAXPTHREADS and NumberOfThreads to see if thread can be added
    if(NumberOfPthreads >= MAXPTHREADS){
        int Error = -1;
        return Error; //at max thread capacity
    }
    else{
        if(NumberOfPthreads == 0){//this is the only thread
            Pthread[NumberOfPthreads].next = &Pthread[NumberOfPthreads];
            Pthread[NumberOfPthreads].prev = &Pthread[NumberOfPthreads];
        }
        else{
            Pthread[NumberOfPthreads].next = &Pthread[0];
            Pthread[NumberOfPthreads].prev = &Pthread[NumberOfPthreads-1];
        }
        Pthread[NumberOfPthreads].period = period;
        Pthread[NumberOfPthreads].currentTime = NumberOfPthreads; //a value 0 to 3 to stagger Pthreads. (time is t*1/4ms)
        Pthread[NumberOfPthreads].executeTime = Pthread[NumberOfPthreads].period+SystemTime+Pthread[NumberOfPthreads].currentTime;
        Pthread[NumberOfPthreads].handler = (int32_t)PthreadToAdd; //function pointer
        int Success = 1;
        ++NumberOfPthreads;
        return Success;
    }
}

/*
 * Puts the current thread into a sleep state.
 *  param durationMS: Duration of sleep time in ms
 */
void sleep(uint32_t durationMS)
{
    CurrentlyRunningThread->sleepCount = durationMS + SystemTime;
    CurrentlyRunningThread->asleep = true; //may need to add library
    //Start context switch
    ICSR |= ICSR_PENDSVSET; //set PendSV flag
}

/*********************************************** Public Functions *********************************************************************/
