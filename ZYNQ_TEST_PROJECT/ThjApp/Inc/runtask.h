#ifndef		__RUNTASK_H__
#define		__RUNTASK_H__
/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        runtask.h
  创建人：       范志州
  创建日期：		2004/11/19
  主要功能描述:  加工过程中执行之任务 
  		
  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/04
       主要修改内容:   
   2. ...
*************************************************/

#include	"control.h"

INT32S ReadAdc(INT32S ch);
INT32S	ProcessReadAdc(INT32S ch);

void	Check_Probe_Task(void *data); 
void    Run_CheckIO_Task(void *data);

void    Display_RunSpeedCount_Task(void *data);
void    Display_RunLine_Task(void *data);
void    Display_XYZ_Task(void *data);
//void    DispRunTime_Task(void *data);

void	RunGotoLine( INT16S n ,BOOL update);
void	RunDisplayLine(INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor);

void	DataUpack_Task(void *data);
void    Assistant_Task(void *data);
void    Get_ComData_Task(void *data);

#endif