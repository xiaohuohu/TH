################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Test/Xadc_Temp_Voltage/Xadc_Temp_Voltage_Test.c \
../src/Test/Xadc_Temp_Voltage/xadc_temp_voltage.c 

OBJS += \
./src/Test/Xadc_Temp_Voltage/Xadc_Temp_Voltage_Test.o \
./src/Test/Xadc_Temp_Voltage/xadc_temp_voltage.o 

C_DEPS += \
./src/Test/Xadc_Temp_Voltage/Xadc_Temp_Voltage_Test.d \
./src/Test/Xadc_Temp_Voltage/xadc_temp_voltage.d 


# Each subdirectory must supply rules for building sources it contributes
src/Test/Xadc_Temp_Voltage/%.o: ../src/Test/Xadc_Temp_Voltage/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"E:\ZYNQ_TEST\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


