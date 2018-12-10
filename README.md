# RTOS-in-ARMv7
My implementation of a RTOS using the ARMv7 architecture on a TI MSP432 chip

## v1
- RTOS with round robin scheduler and naive semaphores (spin-lock)

## v2
- sleeping functionality added to Round Robin threads
- Periodic threads added (executed at highest priority)
- semaphores have ability to block threads requesting an unavailable resource and unblocking when resource is free

## v3
- dynamically kill/add thread created
- Aperiodic threads added to be triggered on interrupt
- WiFi functionality demonstrated with UDP protocol to other microcontroller running identical code
- 2 player Pong game using a router as access point
- *See demo video in the WiFi_Pong Folder*
