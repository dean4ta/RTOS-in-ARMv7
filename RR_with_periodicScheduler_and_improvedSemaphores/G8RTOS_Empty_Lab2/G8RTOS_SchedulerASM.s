; G8RTOS_SchedulerASM.s
; Holds all ASM functions needed for the scheduler
; Note: If you have an h file, do not have a C file and an S file of the same name

	; Functions Defined
	.def G8RTOS_Start, PendSV_Handler

	; Dependencies
	.ref CurrentlyRunningThread, G8RTOS_Scheduler

	.thumb		; Set to thumb mode
	.align 2	; Align by 2 bytes (thumb mode uses allignment by 2 or 4)
	.text		; Text section

; Need to have the address defined in file 
; (label needs to be close enough to asm code to be reached with PC relative addressing)
RunningPtr: .field CurrentlyRunningThread, 32

; G8RTOS_Start
;	Sets the first thread to be the currently running thread
;	Starts the currently running thread by setting Link Register to tcb's Program Counter
G8RTOS_Start:

	.asmfunc
	LDR R0, RunningPtr ;
	LDR R1, [R0] ;
	LDR SP, [R1] ;
	POP {R4-R11} ;
	POP {R0-R3} ;
	POP {R12} ;
	ADD SP, SP, #4 ;
	POP {LR} ;
	ADD SP, SP, #4 ;
	CPSIE I ;
	BX LR ;

	.endasmfunc

; PendSV_Handler
; - Performs a context switch in G8RTOS
; 	- Saves remaining registers into thread stack
;	- Saves current stack pointer to tcb
;	- Calls G8RTOS_Scheduler to get new tcb
;	- Set stack pointer to new stack pointer from new tcb
;	- Pops registers from thread stack
PendSV_Handler:
	
	.asmfunc
	; ************************************ Dean Code Below ************************************ 
	; PendSV Call will push R0-R3, R12-R15, and PSR to stack
	PUSH {R4 - R11} ; push remaining registers (does stack need to be changed before this step?)
	LDR R0, RunningPtr ; save current stack pointer to current tcb
	LDR R1, [R0] ;
	STR SP, [R1] ;
	PUSH {R0, LR} ;
	BL G8RTOS_Scheduler ;
	POP {R0, LR} ;
	LDR R1, [R0] ;
	LDR SP, [R1] ;
	POP {R4-R11} ;
	BX LR ;

	; ************************************ Dean Code Above ************************************ 
	.endasmfunc
	
	; end of the asm file
	.align
	.end
