################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThjApp/App/CEdit.c \
../ThjApp/App/CSetTimer.c \
../ThjApp/App/DispDir.c \
../ThjApp/App/FileFrm.c \
../ThjApp/App/Language.c \
../ThjApp/App/LedOut.c \
../ThjApp/App/MergePack.c \
../ThjApp/App/StrTool.c \
../ThjApp/App/TESTkey.c \
../ThjApp/App/afxmsg.c \
../ThjApp/App/base.c \
../ThjApp/App/bmp1.c \
../ThjApp/App/cbutton.c \
../ThjApp/App/ctrlcard.c \
../ThjApp/App/data.c \
../ThjApp/App/dispmsg.c \
../ThjApp/App/disptime.c \
../ThjApp/App/dog.c \
../ThjApp/App/faceColorStyle.c \
../ThjApp/App/ilist.c \
../ThjApp/App/int_pack.c \
../ThjApp/App/main.c \
../ThjApp/App/mainfrm.c \
../ThjApp/App/msgdlg.c \
../ThjApp/App/paraset.c \
../ThjApp/App/password.c \
../ThjApp/App/runtask.c \
../ThjApp/App/setdog.c \
../ThjApp/App/settime.c \
../ThjApp/App/syspara.c \
../ThjApp/App/techfrm.c \
../ThjApp/App/telcom.c \
../ThjApp/App/testfrm.c \
../ThjApp/App/userdog.c \
../ThjApp/App/work.c 

OBJS += \
./ThjApp/App/CEdit.o \
./ThjApp/App/CSetTimer.o \
./ThjApp/App/DispDir.o \
./ThjApp/App/FileFrm.o \
./ThjApp/App/Language.o \
./ThjApp/App/LedOut.o \
./ThjApp/App/MergePack.o \
./ThjApp/App/StrTool.o \
./ThjApp/App/TESTkey.o \
./ThjApp/App/afxmsg.o \
./ThjApp/App/base.o \
./ThjApp/App/bmp1.o \
./ThjApp/App/cbutton.o \
./ThjApp/App/ctrlcard.o \
./ThjApp/App/data.o \
./ThjApp/App/dispmsg.o \
./ThjApp/App/disptime.o \
./ThjApp/App/dog.o \
./ThjApp/App/faceColorStyle.o \
./ThjApp/App/ilist.o \
./ThjApp/App/int_pack.o \
./ThjApp/App/main.o \
./ThjApp/App/mainfrm.o \
./ThjApp/App/msgdlg.o \
./ThjApp/App/paraset.o \
./ThjApp/App/password.o \
./ThjApp/App/runtask.o \
./ThjApp/App/setdog.o \
./ThjApp/App/settime.o \
./ThjApp/App/syspara.o \
./ThjApp/App/techfrm.o \
./ThjApp/App/telcom.o \
./ThjApp/App/testfrm.o \
./ThjApp/App/userdog.o \
./ThjApp/App/work.o 

C_DEPS += \
./ThjApp/App/CEdit.d \
./ThjApp/App/CSetTimer.d \
./ThjApp/App/DispDir.d \
./ThjApp/App/FileFrm.d \
./ThjApp/App/Language.d \
./ThjApp/App/LedOut.d \
./ThjApp/App/MergePack.d \
./ThjApp/App/StrTool.d \
./ThjApp/App/TESTkey.d \
./ThjApp/App/afxmsg.d \
./ThjApp/App/base.d \
./ThjApp/App/bmp1.d \
./ThjApp/App/cbutton.d \
./ThjApp/App/ctrlcard.d \
./ThjApp/App/data.d \
./ThjApp/App/dispmsg.d \
./ThjApp/App/disptime.d \
./ThjApp/App/dog.d \
./ThjApp/App/faceColorStyle.d \
./ThjApp/App/ilist.d \
./ThjApp/App/int_pack.d \
./ThjApp/App/main.d \
./ThjApp/App/mainfrm.d \
./ThjApp/App/msgdlg.d \
./ThjApp/App/paraset.d \
./ThjApp/App/password.d \
./ThjApp/App/runtask.d \
./ThjApp/App/setdog.d \
./ThjApp/App/settime.d \
./ThjApp/App/syspara.d \
./ThjApp/App/techfrm.d \
./ThjApp/App/telcom.d \
./ThjApp/App/testfrm.d \
./ThjApp/App/userdog.d \
./ThjApp/App/work.d 


# Each subdirectory must supply rules for building sources it contributes
ThjApp/App/%.o: ../ThjApp/App/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O2 -I../ZYNQ_LIB -I../ZYNQ_LIB/uCOS/uCOS-III -I../ZYNQ_LIB/uCOS/uCOS-III/Source -I../ZYNQ_LIB/uCOS/uCOS-III/Cfg/Template -I../ZYNQ_LIB/uCOS/uCOS-III/Ports/ARM-Cortex-A/Generic/GNU -I../ZYNQ_LIB/uCOS/uC-CPU -I../ZYNQ_LIB/uCOS/uC-CPU/Cfg/Template -I../ZYNQ_LIB/uCOS/uC-CPU/ARM-Cortex-A/GNU -I../ZYNQ_LIB/uCOS/uC-LIB -I../ZYNQ_LIB/uCOS/uC-LIB/Cfg/Template -I../ZYNQ_LIB/BaseInc -I../ZYNQ_LIB/Peripherals -I../ZYNQ_LIB/lwip -I../ZYNQ_LIB/lwip/ipv4 -I../MOTION_LIB/Motion -I../LCDGUI_LIB/ADT_GUI -I../src -I../src/ftpd -I../src/Base -I../src/Test -I../src/modbus -I../src/EmacLite -I../src/Test/PC_TEST -I../src/Test/ExtKeyBoard -I../src/Test/RemIOBoard -I../src/Test/PRB06_PeripDev -I../src/Test/Xadc_Temp_Voltage -I../src/Test/MTC500_Info -I../../ZYNQ_TEST_PROJECT_bsp/ps7_cortexa9_0/include -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\fatfs" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\key" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\can" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\i2c" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\rtc" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\spi" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\touch" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\ttc" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\uart" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\Peripherals\usb" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\arch" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\lwip\lwip_net" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source\config" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Source" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II\Ports\ARM-Cortex-A\Generic\RealView" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Drivers\ZC7xxx" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\OS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CAN\Source" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\ARM-Cortex-A\RealView" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-CPU\Cache" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-INT" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\uC-LIB" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\A9-2440\motion" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\OS\uCOS-II" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\uCOS\BSP\CAN" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT_bsp\ps7_cortexa9_0\include" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\ADT_GUI" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ZYNQ_LIB\2440Include" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ThjApp\Inc" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ThjApp\App" -I"C:\Users\zg\Desktop\TH\A9_DB\YG\YG_TH08R_B03\DB016A031B\E.2020.12.10\ZYNQ_TEST_PROJECT\ThjApp\VMotion" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


