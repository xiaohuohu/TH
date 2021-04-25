################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Base/base.c \
../src/Base/pageview.c \
../src/Base/task.c \
../src/Base/timer.c 

OBJS += \
./src/Base/base.o \
./src/Base/pageview.o \
./src/Base/task.o \
./src/Base/timer.o 

C_DEPS += \
./src/Base/base.d \
./src/Base/pageview.d \
./src/Base/task.d \
./src/Base/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Base/%.o: ../src/Base/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -I"E:\1.TH_Project\1.XT_Code\CNC502C-A01\C.2019.1.23\ZYNQ_TEST_PROJECT\src\Base" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


