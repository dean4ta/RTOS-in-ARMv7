################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CC3100SupportPackage/board.obj: ../CC3100SupportPackage/board.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/board.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/cc3100_usage.obj: ../CC3100SupportPackage/cc3100_usage.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/cc3100_usage.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/device.obj: ../CC3100SupportPackage/device.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/device.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/driver.obj: ../CC3100SupportPackage/driver.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/driver.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/flowcont.obj: ../CC3100SupportPackage/flowcont.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/flowcont.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/fs.obj: ../CC3100SupportPackage/fs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/fs.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/netapp.obj: ../CC3100SupportPackage/netapp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/netapp.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/netcfg.obj: ../CC3100SupportPackage/netcfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/netcfg.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/nonos.obj: ../CC3100SupportPackage/nonos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/nonos.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/socket.obj: ../CC3100SupportPackage/socket.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/socket.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/spawn.obj: ../CC3100SupportPackage/spawn.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/spawn.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/spi_cc3100.obj: ../CC3100SupportPackage/spi_cc3100.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/spi_cc3100.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CC3100SupportPackage/wlan.obj: ../CC3100SupportPackage/wlan.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/G8RTOS" --include_path="C:/Users/Isaac/workspace_v7/Lab4MicroP2/BoardSupportPackage/inc" --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Isaac/workspace_v7/Lab5MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/wlan.d_raw" --obj_directory="CC3100SupportPackage" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


