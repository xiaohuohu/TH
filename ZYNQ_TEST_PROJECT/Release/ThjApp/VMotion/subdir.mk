################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThjApp/VMotion/THMotion.c \
../ThjApp/VMotion/VMQUEUE_OP.c 

OBJS += \
./ThjApp/VMotion/THMotion.o \
./ThjApp/VMotion/VMQUEUE_OP.o 

C_DEPS += \
./ThjApp/VMotion/THMotion.d \
./ThjApp/VMotion/VMQUEUE_OP.d 


# Each subdirectory must supply rules for building sources it contributes
ThjApp/VMotion/%.o: ../ThjApp/VMotion/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\ADT_GUI" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ZYNQ_LIB\2440Include" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ThjApp\Inc" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ThjApp\App" -I"C:\Users\zg\Desktop\TH\A9\CNC820C_A01\I.2020.12.18\ZYNQ_TEST_PROJECT\ThjApp\VMotion" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


