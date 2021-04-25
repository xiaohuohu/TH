#ifndef __A9_2440LIB_H__
#define __A9_2440LIB_H__

//===================================================================
// File Name : A9_2440lib.h
// Function  : TQ2440
// Version   : 1.0
//===================================================================

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

#define DebugOut Uart_Printf

#define	DELAYUNIT	1 //延时单位为100时该参数为1,延时单位为1时该参数为100

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))

#define ONESEC0 (62500)	             //16us resolution, max 1.04 sec
#define ONESEC1 (31250)	             //32us resolution, max 2.09 sec
#define ONESEC2 (15625)	             //64us resolution, max 4.19 sec
#define ONESEC3 (7812)	             //128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))  //@60Mhz, 128*4us resolution, max 32.53 sec


#ifndef	NULL
#define NULL 0
#endif


#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)
//void StartPointAfterPowerOffWakeUp(void); //purnnamu:Mar.29.2002

/*
// 2440lib.c
void Delay(int time);              //Watchdog Timer is used.

void *malloc(unsigned nbyte);
void free(void *pt);

void Port_Init(void);
void Uart_Select(int ch);
void Uart_TxEmpty(int ch);
void Uart_Init(int mclk,int baud);
char Uart_Getch(void);
char Uart_GetKey(void);
void Uart_GetString(char *string);
int  Uart_GetIntNum(void);
int Uart_GetIntNum_GJ(void) ;
void Uart_SendByte(int data);
char *hex2char(unsigned int val);

//void Uart_SendDWORD(DWORD d, BOOL cr);
void Uart_Printf(char *fmt,...);
void Uart_Printf2(char *fmt,...);
void Uart_SendString(char *pt);

void Timer_Start(int divider);    //Watchdog Timer is used.
int  Timer_Stop(void);            //Watchdog Timer is used.

void LED_Display(int data);
//void Beep(U32 freq, U32 ms) ;
void Beep(int data);
void LCD_Power(int sta);

void ChangeMPllValue(int m,int p,int s);
void ChangeClockDivider(void);
void ChangeUPllValue(int m,int p,int s);

void outportw(U16, U32);
U16 inportw(U32);
//void ExitCritical(int *i);

*/

//void NullPrintf(char *fmt,...){


//}
//#define	DbgOut	NullPrintf

// -- 机器型号 -- //
#define CNC4640				1
#define CNC4500				2
#define CNC8860				3
#define TP3540				4
#define CNC49xx				5
#define TH08HA				6
#define	HY700				7
#define	TV5600				8
#define	MCD604				9
#define	CNC4650				10
#define QC600A				11
#define ADT8848C			12
#define TH08R				13

//-- 按键板类型 --//
#define	KEYBOARD_TO_NULL		0			// -- 没有按键板
#define	KEYBOARD_TO_FPGA		1			// -- 与FPGA相连接的按键板类型
#define	KEYBOARD_TO_ARM			2			// -- 与ARM连接的按键板类型
#define	KEYBOARD_TO_ARM_GPIO	3			// -- 直接用ARM解码按键板类型[TV5600]


//-- 显示屏类型 --//
/* 屏幕大部分都由分辨率区分，只有 TP3540(640*480)的有点区别，
 * 考虑到和老程序兼容，这部分暂不作改动，配置结构体时，
 * "屏幕类型"这一项配置和 "机器型号" 一样
 */

//-- 触摸屏类型 --//
#define	TB_NULL				0				// -- 不具备此功能
#define	TB_W240H320			1				// -- Width=240 Height=320
#define	TB_W640H480			2				// -- Width=640 Height=480
#define	TB_W800H600			3				// -- Width=800 Height=600
#define	TB_W800H480			4				// -- Width=800 Height=480

#define	TB_ADC(t)			((0x01<<7)|(t&0x7f))// -- 现只有 TP3540 使用此方式

//
typedef struct DEVICECFGINFO
{
	unsigned char	MachineType;	// 机器型号
	unsigned char	KeyType;		// 按键类型
	unsigned char	ScreenType;		// 显示屏类型
	unsigned char	TouchType;		// 触摸屏类型
}DeviceCFGInfo;

//
//
/* --------------------------------------------------------------------------
函数名：	SetMachineType
功能说明：
			选择控制器类型，不调用此函数则默认为CNC4640,如用户配置本库支持以外
			的机型时，用户可以自定义机器类型，如下：

struct UNITCFGINFO	CNC_CNC4640 =		//定义机器配置结构体变量
{
	CNC4640,				// 机器型号
	KEYBOARD_TO_FPGA,		// 按键板类型
	CNC4640,				// 显示屏类型
	TB_NULL,				// 触摸屏类型
};
DeviceReg_t=CNC_CNC4640;	// 使配置有效

注意：
	如用户用自定义方式定制机器，则不再需要调用 SetMachineType(int typeid)
	如果键盘类型选择 KEYBOARD_TO_ARM 在此配置完后还需调用 SetKeyType()
	对键盘进行初始化
	如:
	void Main()
	{
		...
	}

	void MainStask()
	{
		...
		SetKeyType(CNC_CNC4640.KeyType);
		...
		for(;;)
		{

		}
	}

 >> 所有配置信息必须要在以上宏定义中选择
-------------------------------------------------------------------------- */
int 	SetMachineType(int typeid);			//选择控制器类型函数，不调用则默认为CNC4640控制器
//int 	SetUartPrintfEnable(int mode);		//设定UartPrintf函数是否有效，0关闭串口输出 非0使能
//extern 	DeviceCFGInfo	DeviceReg_t;


#ifdef __cplusplus
}
#endif

#endif
