#ifndef		_WORK_H__
#define		_WORK_H__

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        work.h
  �����ˣ�       ��־��
  �������ڣ�		2004/11/19
  ��Ҫ��������:  �жϣ��ӹ��Լ����ݽ����һЩ������� 
  		
  �޸ļ�¼��
   1.  �޸���:	      ��־��
       �޸�����:	      2005/08/03
       ��Ҫ�޸�����:   �������жϿ��ص�һЩ����ʽ�Լ����ݼӹ��������һЩ����
   2. ...
*************************************************/

#include	"os_cpu.h"

#ifdef      	WORK_GLOBALS
#define     	WORK_EXT
#else
#define     	WORK_EXT  			extern
#endif

#define 	TAGBUFCOUNT	 2	
//#define			DEBUGDISP

void    DriverEnd(void);

void    DriverEnd2(void);


void	InstallDriverInterrupt(void);


void	UnInstallDriverInterrupt(void);


void	DisplayXYZ( BOOL cur);


INT16S	RunAllData(void);


INT16S	UnpackData(void);
void	UnpackErrDisp(INT16S no);
BOOL	DataCopy(void);


void	SuspendTasks(INT8S no);
void	ResumeTasks(INT8S no);

void	UpdateLeavingTime(BOOL init,BOOL disp,BOOL DataInit);//����ʣ��ʱ��
void	CloseDisplay(FP32 tt);

void	UpdateProbe(BOOL init);
void	UpdateCylinder(BOOL init);

void	UpdateHandBox(BOOL flag);

//void  	InitLed(void);
//�жϹ�����Ƿ����
//void	BackZeroLed(void);
BOOL	WorkFindZero(INT16S ch);
void	Warning(INT16S x,INT16S y,INT16S w,INT16S h,char *msg,char *title,INT32S tc);

void	RunEnd(void);
INT32S	SpeedMax(INT32S speed,INT32S p1,INT32S p2,INT32S p3,INT32S p4);
void	RunWarningDisplay(INT16S no);
BOOL  CheckHardLimit(INT16S ch, INT32S pos);
BOOL  CheckSoftLimit(INT16S ch, INT32S pos);
FP32 GetYLong(INT32S index);
BOOL ExitScrProtect(void);
#endif