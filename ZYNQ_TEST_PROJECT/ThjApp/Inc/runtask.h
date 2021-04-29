#ifndef		__RUNTASK_H__
#define		__RUNTASK_H__
/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        runtask.h
  �����ˣ�       ��־��
  �������ڣ�		2004/11/19
  ��Ҫ��������:  �ӹ�������ִ��֮���� 
  		
  �޸ļ�¼��
   1.  �޸���:	      ��־��
       �޸�����:	      2005/08/04
       ��Ҫ�޸�����:   
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