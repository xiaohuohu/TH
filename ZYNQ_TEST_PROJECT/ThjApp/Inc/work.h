#ifndef		_WORK_H__
#define		_WORK_H__

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        work.h
  创建人：       范志州
  创建日期：		2004/11/19
  主要功能描述:  中断，加工以及数据解码的一些处理操作 
  		
  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/03
       主要修改内容:   更改了中断开关的一些处理方式以及数据加工，解码的一些功能
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

void	UpdateLeavingTime(BOOL init,BOOL disp,BOOL DataInit);//更新剩余时间
void	CloseDisplay(FP32 tt);

void	UpdateProbe(BOOL init);
void	UpdateCylinder(BOOL init);

void	UpdateHandBox(BOOL flag);

//void  	InitLed(void);
//判断归零灯是否点亮
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