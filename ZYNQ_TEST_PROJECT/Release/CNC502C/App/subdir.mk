################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CNC502C/App/CEdit.c \
../CNC502C/App/DispDir.c \
../CNC502C/App/Language.c \
../CNC502C/App/LedOut.c \
../CNC502C/App/MergePack.c \
../CNC502C/App/StrTool.c \
../CNC502C/App/TESTkey.c \
../CNC502C/App/afxmsg.c \
../CNC502C/App/base.c \
../CNC502C/App/bmp1.c \
../CNC502C/App/cbutton.c \
../CNC502C/App/ctrlcard.c \
../CNC502C/App/data.c \
../CNC502C/App/dispmsg.c \
../CNC502C/App/disptime.c \
../CNC502C/App/dog.c \
../CNC502C/App/ilist.c \
../CNC502C/App/int_pack.c \
../CNC502C/App/keyboard.c \
../CNC502C/App/main.c \
../CNC502C/App/mainfrm.c \
../CNC502C/App/msgdlg.c \
../CNC502C/App/paraset.c \
../CNC502C/App/password.c \
../CNC502C/App/runtask.c \
../CNC502C/App/setdog.c \
../CNC502C/App/settime.c \
../CNC502C/App/syspara.c \
../CNC502C/App/techfrm.c \
../CNC502C/App/telcom.c \
../CNC502C/App/test.c \
../CNC502C/App/testfrm.c \
../CNC502C/App/userdog.c \
../CNC502C/App/work.c 

OBJS += \
./CNC502C/App/CEdit.o \
./CNC502C/App/DispDir.o \
./CNC502C/App/Language.o \
./CNC502C/App/LedOut.o \
./CNC502C/App/MergePack.o \
./CNC502C/App/StrTool.o \
./CNC502C/App/TESTkey.o \
./CNC502C/App/afxmsg.o \
./CNC502C/App/base.o \
./CNC502C/App/bmp1.o \
./CNC502C/App/cbutton.o \
./CNC502C/App/ctrlcard.o \
./CNC502C/App/data.o \
./CNC502C/App/dispmsg.o \
./CNC502C/App/disptime.o \
./CNC502C/App/dog.o \
./CNC502C/App/ilist.o \
./CNC502C/App/int_pack.o \
./CNC502C/App/keyboard.o \
./CNC502C/App/main.o \
./CNC502C/App/mainfrm.o \
./CNC502C/App/msgdlg.o \
./CNC502C/App/paraset.o \
./CNC502C/App/password.o \
./CNC502C/App/runtask.o \
./CNC502C/App/setdog.o \
./CNC502C/App/settime.o \
./CNC502C/App/syspara.o \
./CNC502C/App/techfrm.o \
./CNC502C/App/telcom.o \
./CNC502C/App/test.o \
./CNC502C/App/testfrm.o \
./CNC502C/App/userdog.o \
./CNC502C/App/work.o 

C_DEPS += \
./CNC502C/App/CEdit.d \
./CNC502C/App/DispDir.d \
./CNC502C/App/Language.d \
./CNC502C/App/LedOut.d \
./CNC502C/App/MergePack.d \
./CNC502C/App/StrTool.d \
./CNC502C/App/TESTkey.d \
./CNC502C/App/afxmsg.d \
./CNC502C/App/base.d \
./CNC502C/App/bmp1.d \
./CNC502C/App/cbutton.d \
./CNC502C/App/ctrlcard.d \
./CNC502C/App/data.d \
./CNC502C/App/dispmsg.d \
./CNC502C/App/disptime.d \
./CNC502C/App/dog.d \
./CNC502C/App/ilist.d \
./CNC502C/App/int_pack.d \
./CNC502C/App/keyboard.d \
./CNC502C/App/main.d \
./CNC502C/App/mainfrm.d \
./CNC502C/App/msgdlg.d \
./CNC502C/App/paraset.d \
./CNC502C/App/password.d \
./CNC502C/App/runtask.d \
./CNC502C/App/setdog.d \
./CNC502C/App/settime.d \
./CNC502C/App/syspara.d \
./CNC502C/App/techfrm.d \
./CNC502C/App/telcom.d \
./CNC502C/App/test.d \
./CNC502C/App/testfrm.d \
./CNC502C/App/userdog.d \
./CNC502C/App/work.d 


# Each subdirectory must supply rules for building sources it contributes
CNC502C/App/%.o: ../CNC502C/App/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\CNC502C\Inc" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\CNC502C\VMotion" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\CNC502C\App" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\ADT_GUI" -I"E:\1.TH_Project\1.XT_Code\TH08HB-A01\C.2019.3.21\ZYNQ_TEST_PROJECT\ZYNQ_LIB\2440Include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


