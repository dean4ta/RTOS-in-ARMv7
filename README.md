# RTOS-in-ARMv7
My implementation of a RTOS using the ARMv7 architecture on a TI MSP432 chip

## v1
- RTOS with round robin scheduler and naive semaphores (spin-lock)

## v2
- sleeping functionality added to Round Robin threads
- Periodic threads added (executed at highest priority)
- semaphores have ability to block threads requesting an unavailable resource and unblocking when resource is free
