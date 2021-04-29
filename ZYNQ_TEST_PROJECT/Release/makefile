################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ZYNQ_LIB/BaseInc/subdir.mk
-include ThjApp/VMotion/subdir.mk
-include ThjApp/App/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
ZYNQ_TEST_PROJECT.elf.size \


# All Target
all: ZYNQ_TEST_PROJECT.elf secondary-outputs

# Tool invocations
ZYNQ_TEST_PROJECT.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM gcc linker'
	arm-xilinx-eabi-gcc -Wl,-T -Wl,../src/lscript.ld -L../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/lib -o "ZYNQ_TEST_PROJECT.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

ZYNQ_TEST_PROJECT.elf.size: ZYNQ_TEST_PROJECT.elf
	@echo 'Invoking: ARM Print Size'
	arm-xilinx-eabi-size ZYNQ_TEST_PROJECT.elf  |tee "ZYNQ_TEST_PROJECT.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) ZYNQ_TEST_PROJECT.elf
	-@echo ' '

post-build:
	-bootgen -image C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\bootimage/ZYNQ_TEST_PROJECT.bif -o C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\bootimage/A9Rom.bin -w
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
