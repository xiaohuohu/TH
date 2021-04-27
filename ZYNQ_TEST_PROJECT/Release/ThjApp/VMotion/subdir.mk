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
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\ADT_GUI" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\2440Include" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ThjApp\App" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ThjApp\Inc" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ThjApp\VMotion" -I"D:\qt_linux\TH\A9\YF_TH08HB_A01\2021.4.25\ZYNQ_TEST_PROJECT\ZYNQ_LIB\BaseInc" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


