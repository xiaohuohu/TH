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

#define	DELAYUNIT	1 //��ʱ��λΪ100ʱ�ò���Ϊ1,��ʱ��λΪ1ʱ�ò���Ϊ100

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

// -- �����ͺ� -- //
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

//-- ���������� --//
#define	KEYBOARD_TO_NULL		0			// -- û�а�����
#define	KEYBOARD_TO_FPGA		1			// -- ��FPGA�����ӵİ���������
#define	KEYBOARD_TO_ARM			2			// -- ��ARM���ӵİ���������
#define	KEYBOARD_TO_ARM_GPIO	3			// -- ֱ����ARM���밴��������[TV5600]


//-- ��ʾ������ --//
/* ��Ļ�󲿷ֶ��ɷֱ������֣�ֻ�� TP3540(640*480)���е�����
 * ���ǵ����ϳ�����ݣ��ⲿ���ݲ����Ķ������ýṹ��ʱ��
 * "��Ļ����"��һ�����ú� "�����ͺ�" һ��
 */

//-- ���������� --//
#define	TB_NULL				0				// -- ���߱��˹���
#define	TB_W240H320			1				// -- Width=240 Height=320
#define	TB_W640H480			2				// -- Width=640 Height=480
#define	TB_W800H600			3				// -- Width=800 Height=600
#define	TB_W800H480			4				// -- Width=800 Height=480

#define	TB_ADC(t)			((0x01<<7)|(t&0x7f))// -- ��ֻ�� TP3540 ʹ�ô˷�ʽ

//
typedef struct DEVICECFGINFO
{
	unsigned char	MachineType;	// �����ͺ�
	unsigned char	KeyType;		// ��������
	unsigned char	ScreenType;		// ��ʾ������
	unsigned char	TouchType;		// ����������
}DeviceCFGInfo;

//
//
/* --------------------------------------------------------------------------
��������	SetMachineType
����˵����
			ѡ����������ͣ������ô˺�����Ĭ��ΪCNC4640,���û����ñ���֧������
			�Ļ���ʱ���û������Զ���������ͣ����£�

struct UNITCFGINFO	CNC_CNC4640 =		//����������ýṹ�����
{
	CNC4640,				// �����ͺ�
	KEYBOARD_TO_FPGA,		// ����������
	CNC4640,				// ��ʾ������
	TB_NULL,				// ����������
};
DeviceReg_t=CNC_CNC4640;	// ʹ������Ч

ע�⣺
	���û����Զ��巽ʽ���ƻ�����������Ҫ���� SetMachineType(int typeid)
	�����������ѡ�� KEYBOARD_TO_ARM �ڴ������������� SetKeyType()
	�Լ��̽��г�ʼ��
	��:
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

 >> ����������Ϣ����Ҫ�����Ϻ궨����ѡ��
-------------------------------------------------------------------------- */
int 	SetMachineType(int typeid);			//ѡ����������ͺ�������������Ĭ��ΪCNC4640������
//int 	SetUartPrintfEnable(int mode);		//�趨UartPrintf�����Ƿ���Ч��0�رմ������ ��0ʹ��
//extern 	DeviceCFGInfo	DeviceReg_t;


#ifdef __cplusplus
}
#endif

#endif
