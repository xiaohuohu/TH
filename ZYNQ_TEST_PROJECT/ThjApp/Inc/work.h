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

#define 	TAGBUFCOUNT	 2						//缓存弹簧条数。					

//#define			DEBUGDISP


WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S		g_TextColor6;
WORK_EXT	INT32S		g_TextColor7;
WORK_EXT	INT32S 		g_TextColor8;
WORK_EXT	INT32S		g_TextColor9;
WORK_EXT	INT32S		g_TextColor10;
WORK_EXT	INT32S		g_TextColor11;

WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S		g_nTechfrmBk;
WORK_EXT	INT32S 		g_nTitleBk; 
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;




void    DriverEnd(void);
void    DriverEnd2(void);

//void	InstallDriverInterrupt(void);
//void	UnInstallDriverInterrupt(void);

void	DisplayXYZ( BOOL cur);

INT16S	RunAllData(void);

//void FunC(INT16S page,INT16S num);


INT16S		UnpackData(void);
void		UnpackErrDisp(INT16S no);
BOOL		DataCopy(void);


void	SuspendTasks(INT8S no);
void	ResumeTasks(INT8S no);

void	UpdateLeavingTime(BOOL init,BOOL disp,BOOL DataInit);//更新剩余时间
void	CloseDisplay(FP32 tt);

void	UpdateProbe(BOOL init);
void	UpdateCylinder(BOOL init);

void	UpdateHandBox(BOOL flag);

void  	InitLed(void);
//判断归零灯是否点亮
void	BackZeroLed(void);
BOOL	WorkFindZero(INT16S ch);
void	Warning(INT16S x,INT16S y,INT16S w,INT16S h,char *msg,char *title,INT32S tc);

void	RunEnd(void);
INT32S	SpeedMax(INT32S speed,INT32S p1,INT32S p2,INT32S p3,INT32S p4);
void	HandFunc(void);
void	PressKeyMove(INT16S ch,INT16S col,INT16S dir);

INT32S 	GetYLong(INT32S index);					//用于查找并返回索引行的Y值，用于Y座标显示
BOOL 	ExitScrProtect(void);

#endif
