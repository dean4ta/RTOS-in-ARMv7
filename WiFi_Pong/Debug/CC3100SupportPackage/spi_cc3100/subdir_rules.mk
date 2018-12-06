################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CC3100SupportPackage/spi_cc3100/spi_cc3100.obj: ../CC3100SupportPackage/spi_cc3100/spi_cc3100.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/BoardSupportPackage/DriverLib" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/G8RTOS" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/BoardSupportPackage/inc" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/simplelink/include" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/board" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/simplelink" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/SL_Common" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/dean4ta/OneDrive - University of Florida/Documents/Fall 2018 UF/uP II/code/Lab5MicroP2/CC3100SupportPackage" --include_path="C:/ti/ccsv8/ccs_base/arm/include" --include_path="C:/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/spi_cc3100/spi_cc3100.d_raw" --obj_directory="CC3100SupportPackage/spi_cc3100" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


