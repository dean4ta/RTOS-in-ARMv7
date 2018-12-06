/*
 * G8RTOS_Scheduler.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include "G8RTOS_Scheduler.h"
#define TICKS_PER_QUANTUM 1
#define ERROR   1
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

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/* Thread Control Blocks
 *  - An array of thread control blocks to hold pertinent information for each thread
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/* Thread Stacks
 *  - An array of arrays that will act as invdividual stacks for each thread
 */
static int32_t threadStacks[MAX_THREADS][STACKSIZE];

/* Periodic Event Threads
 * - An array of periodic events to hold pertinent information for each thread
 */
static ptcb_t Pthread[MAXPTHREADS];

/*********************************************** Data Structures Used *****************************************************************/


/*********************************************** Private Variables ********************************************************************/

static uint16_t IDCounter = 0; //keep track of thread IDs

/*
 * Current Number of Threads currently in the scheduler
 */
static uint32_t NumberOfThreads;

/*
 * Current Number of Periodic Threads currently in the scheduler
 */
static uint32_t NumberOfPthreads;
/*Free pointer keeps track of the free pointers*/
static int32_t Free_Pointers[MAX_THREADS];
/*********************************************** Data Structures Used *****************************************************************/



/*********************************************** Private Functions ********************************************************************/

/*
 * Initializes the Systick and Systick Interrupt
 * The Systick interrupt will be responsible for starting a context switch between threads
 * Param "numCycles": Number of cycles for each systick interrupt
 */
static void InitSysTick(uint32_t numCycles)
{
    SysTick_Config(numCycles);
    SysTick_enableInterrupt();
}

/*
 * Chooses the next thread to run.
 * Lab 2 Scheduling Algorithm:
 * 	- Simple Round Robin: Choose the next running thread by selecting the currently running thread's next pointer
 */


int G8RTOS_Scheduler(uint32_t currentSP)
{
    CurrentlyRunningThread->Stack_Pointer = currentSP;
    tcb_t * tempRunningThread = CurrentlyRunningThread;
    uint16_t currentMaxPriority = 256;
    for(int i = 0; i < NumberOfThreads;i++){
        if((tempRunningThread->blocked == 0) && (tempRunningThread->Asleep == false)
&& (tempRunningThread->Alive == true) && (tempRunningThread->Priority < currentMaxPriority))
        {
            currentMaxPriority = tempRunningThread->Priority;
            CurrentlyRunningThread = tempRunningThread;
        }
        tempRunningThread = tempRunningThread->Next_TCB;
    }
    return CurrentlyRunningThread->Stack_Pointer;
}

/*
 * SysTick Handler
 * Currently the Systick Handler will only increment the system time
 * and set the PendSV flag to start the scheduler
 *
 * In the future, this function will also be responsible for sleeping threads and periodic threads
 */
#define ICSR (*((volatile unsigned int*)(0xe000ed04)))
#define ICSR_PENDSVSET (1<<28);
uint16_t ticks_count = 0;

void SysTick_Handler()
{
    SystemTime++;
    ticks_count++;
    for(int i = 0; i < NumberOfThreads; i++)
    {
        if(threadControlBlocks[i].Asleep == true)
        {
            if(threadControlBlocks[i].Sleep_Count == SystemTime){
                threadControlBlocks[i].Asleep = false;
            }
            else if(threadControlBlocks[i].Sleep_Count < SystemTime)
            {
                if(threadControlBlocks[i].isBall)
                {
                    destroyBall(threadControlBlocks[i].ballNum);
                }
                G8RTOS_KillThread(threadControlBlocks[i].threadID);
            }
        }
    }
    for(int i = 0; i < NumberOfPthreads; i++)
    {
        if(Pthread[i].Excecute_Time == SystemTime)
        {
            Pthread[i].Excecute_Time = Pthread[i].Period + SystemTime;
            ((void(*)())Pthread[i].Handler)();
        }
    }



    if(ticks_count == TICKS_PER_QUANTUM){
        ticks_count = 0;
        ICSR |= ICSR_PENDSVSET;
    }
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
    BSP_InitBoard(); //initialize hardware
    NumberOfThreads = 0; //set threads to zero
    NumberOfPthreads = 0; //set periodic threads to zero
    SystemTime = 0; //set system time to zero
    uint32_t newVTORTable = 0x20000000;
    memcpy((uint32_t *)newVTORTable, (uint32_t *)SCB->VTOR, 57*4); // 57 interrupt vectors to copy
    SCB->VTOR = newVTORTable;
    for(int i = 0; i < MAX_THREADS; i++){
        threadControlBlocks[i].Alive = false;
    }
    isHost = GetPlayerRole();
    if(isHost)
    {
        CreateGame();
    }
    else
    {
        JoinGame();
    }
}

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes the Systick
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
int G8RTOS_Launch()
{
    uint16_t currentMaxPriority = 256;
    for(int i = 0; i < NumberOfThreads;i++){
        if((threadControlBlocks[i].Alive == true) && (threadControlBlocks[i].Priority < currentMaxPriority))
        {
            currentMaxPriority = threadControlBlocks[i].Priority;
            CurrentlyRunningThread = &threadControlBlocks[i];
        }
    }

    InitSysTick((uint32_t)(ClockSys_GetSysFreq()*.001));//set systick to 1 ms
    Interrupt_enableInterrupt(FAULT_PENDSV);
    NVIC_SetPriority(SysTick_IRQn, 5);
    NVIC_SetPriority(PendSV_IRQn, 5);
    G8RTOS_Start(CurrentlyRunningThread->Stack_Pointer + 56);

    return NO_ERROR;
}


/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are still available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread to hold a "fake context"
 * 	- Sets stack tcb stack pointer to top of thread stack
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */
sched_ErrCode_t G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t priority, char* name)
{
    int32_t PRIMASK = StartCriticalSection();
    bool threadNotAssigned = true;
    uint32_t currentThread = 0;
	if(NumberOfThreads < MAX_THREADS){

	    for(int i = 0;i<MAX_THREADS;i++){
	        if(threadControlBlocks[i].Alive == false){
	            currentThread = i;
	            threadControlBlocks[currentThread].Stack_Pointer = &threadStacks[currentThread][STACKSIZE - 16];
	            threadNotAssigned = false;
	            break;
	        }
	    }
	    if(threadNotAssigned){
	        EndCriticalSection(PRIMASK);
	        return THREADS_INCORRECTLY_ALIVE;
	    }
	    if(NumberOfThreads == 0){
	        threadControlBlocks[currentThread].Next_TCB =  &threadControlBlocks[currentThread];
	        threadControlBlocks[currentThread].Previous_TCB =  &threadControlBlocks[currentThread];
	        CurrentlyRunningThread = &threadControlBlocks[currentThread];
	    }
	    else{
	        threadControlBlocks[currentThread].Next_TCB = CurrentlyRunningThread->Next_TCB;
	        threadControlBlocks[currentThread].Previous_TCB = CurrentlyRunningThread;
	        CurrentlyRunningThread->Next_TCB->Previous_TCB = &threadControlBlocks[currentThread];
	        CurrentlyRunningThread->Next_TCB = &threadControlBlocks[currentThread];

	    }
	    threadStacks[currentThread][STACKSIZE - 2] = (uint32_t)threadToAdd; //program counter
	    threadStacks[currentThread][STACKSIZE - 1] = THUMBBIT; //Dummy status register with thumb bit set
	    threadControlBlocks[currentThread].Priority = priority;
	    threadControlBlocks[currentThread].Alive = true;
	    threadControlBlocks[currentThread].threadID = ((IDCounter++) << 16);
	    strncpy(&threadControlBlocks[currentThread].threadName,name,MAX_NAME_LENGTH);
        NumberOfThreads++;
        EndCriticalSection(PRIMASK);
        return NO_ERROR;
	    }

	else{
	    EndCriticalSection(PRIMASK);
	    return THREAD_LIMIT_REACHED;
	}
}


/*
 * Adds periodic threads to G8RTOS Scheduler
 * Function will initialize a periodic event struct to represent event.
 * The struct will be added to a linked list of periodic events
 * Param Pthread To Add: void-void function for P thread handler
 * Param period: period of P thread to add
 * Returns: Error code for adding threads
 */
sched_ErrCode_t G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period)
{
    if(NumberOfPthreads < MAX_THREADS){

        if(NumberOfPthreads == 0){
            Pthread[NumberOfPthreads].Next_PTCB =  &Pthread[NumberOfPthreads];
            Pthread[NumberOfPthreads].Previous_PTCB =  &Pthread[NumberOfPthreads];
        }
        else{
            Pthread[NumberOfPthreads].Next_PTCB = &Pthread[0];
            Pthread[NumberOfPthreads].Previous_PTCB = &Pthread[NumberOfPthreads-1];
            Pthread[NumberOfPthreads-1].Next_PTCB = &Pthread[NumberOfPthreads];
            Pthread[0].Previous_PTCB = &Pthread[NumberOfPthreads];
        }
        Pthread[NumberOfPthreads].Handler = PthreadToAdd;
        Pthread[NumberOfPthreads].Excecute_Time = SystemTime + period;
        Pthread[NumberOfPthreads].Period = period;
        NumberOfPthreads++;
        return 0;
        }

    else{
        return ERROR;
    }
}

void Change_Period(uint8_t thread,uint8_t period)
{
    int32_t PRIMASK = StartCriticalSection();
    Pthread[thread].Period = period;
    EndCriticalSection(PRIMASK);

}

/*
 * Puts the current thread into a sleep state.
 *  param durationMS: Duration of sleep time in ms
 */
void sleep(uint32_t durationMS)
{
    CurrentlyRunningThread->Sleep_Count = SystemTime + durationMS;
    CurrentlyRunningThread->Asleep = true;
    StartContextSwitch();
}

void StartContextSwitch(void){
    ICSR |= ICSR_PENDSVSET;
}
void G8RTOS_Genocide(void)
{
    threadId_t currentThread = G8RTOS_GetThreadId();
    for(int i = 0;i < MAX_THREADS ; i++)
    {
        if(threadControlBlocks[i].Alive && (threadControlBlocks[i].threadID != currentThread))
        {
            G8RTOS_KillThread(threadControlBlocks[i].threadID);
        }
    }
}
threadId_t G8RTOS_GetThreadId(void){
    return CurrentlyRunningThread->threadID;
}

sched_ErrCode_t G8RTOS_KillThread(threadId_t threadId){
    int32_t PRIMASK = StartCriticalSection();
    bool threadNotFound = true;
    int test = 0;
    if(NumberOfThreads < 2){
        EndCriticalSection(PRIMASK);
        return CANNOT_KILL_LAST_THREAD;
    }
    for(int i = 0;i<MAX_THREADS;i++){
        if(threadControlBlocks[i].threadID == threadId){
            threadNotFound = false;
            if(threadControlBlocks[i].blocked)
            {
                G8RTOS_ReleaseSemaphore(threadControlBlocks[i].blocked);
                threadControlBlocks[i].blocked = 0;
            }
            threadControlBlocks[i].Alive = false;
            threadControlBlocks[i].Asleep = false;
            threadControlBlocks[i].Previous_TCB->Next_TCB = threadControlBlocks[i].Next_TCB;
            threadControlBlocks[i].Next_TCB->Previous_TCB = threadControlBlocks[i].Previous_TCB;
            NumberOfThreads--;
            EndCriticalSection(PRIMASK);
            if(&threadControlBlocks[i] == CurrentlyRunningThread)
            {
                StartContextSwitch();
            }
            return NO_ERROR;
        }
    }
        if(threadNotFound){
            EndCriticalSection(PRIMASK);
            return THREAD_DOES_NOT_EXIST;
        }
}


sched_ErrCode_t G8RTOS_KillSelf(void){
    return G8RTOS_KillThread(G8RTOS_GetThreadId());
}

sched_ErrCode_t G8RTOS_AddAPeriodicEvent(void(*AthreadToAdd)(void), uint8_t priority,IRQn_Type IRQn){
    if((IRQn < PSS_IRQn)|(IRQn > PORT6_IRQn)){
        return IRQn_INVALID;
    }
    if(priority > 6){
        return HWI_PRIORITY_INVALID;
    }
    __NVIC_SetVector(IRQn,(uint32_t)AthreadToAdd);
    __NVIC_SetPriority(IRQn,priority);
    NVIC_EnableIRQ(IRQn);
    return NO_ERROR;
}

/*********************************************** Public Functions *********************************************************************/
