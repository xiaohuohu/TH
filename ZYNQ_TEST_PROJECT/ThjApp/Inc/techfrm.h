# ifndef	___TECH_FRM_H___
# define	___TECH_FRM_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        techfrm.h
  创建人：       范志州
  创建日期：		2004/11/19
  主要功能描述:  编程界面，包括表格的创建，刷新，各种程序编辑操作 
  		
  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/05
       主要修改内容:   更改了任务的执行创建挂起恢复等功能要求
   2. ...
*************************************************/


//#include    "includes.h"
#include	"os_cpu.h"
#include	"ilist.h"
#include	"rect.h"
#include	"data.h"
#include	"control.h"

#define	ROWS	8
#define COLS2 	7	    //两轴系统对应的列数
#define COLS3   8       //三轴系统对应的列数
#define COLS4  	9		//四轴系统对应的列数

void	CTeach_Init(void);		
void	CTeach_Destroy(void);
void	CTeach_Create(void);
void	CTeach_InvidateView(BOOL   init);

void	CTeach_DrawGrid(void);
void	UpdateFileDestProbe(INT16S no,INT16S dir,INT16S IsEnter);
void	UpdateStep(INT16S dk);
void	UpdateSpeedCountProbe(void);

void	UpdateAxisStatus(void);
BOOL	UpdateCommand(INT8S *str);

void	CTeach_Update( INT16S row,INT16S col );
void	CTeach_Update2( INT16S row,INT16S col );
void	CTeach_UpdatePage( INT16S page  );
void	CTeach_DisplayLine( INT16S data,INT16S row,DRAWSTATUS statust,INT32S qColor,INT32S bColor);
void	CTeach_DrawItem(INT16S row,INT16S col,DRAWSTATUS status,INT32S qColor,INT32S bColor);
void	CTeach_ViewConvert(INT16S row,INT16S col,INT8S *buf);

void	CTeach_InitData(void);
void	CTeach_AddData(void);
void	CTeach_Insert(void);
void	CTeach_Delete(void);
void	CTeach_DleAll(void);

void	Teach_Task(void *data);
void	CTeach_LoopCheck(void);
void	CTeach_OnKey(INT16S key,INT16S tkey);

BOOL	CTeach_ProcessError(COMMAND *ptr, INT16S col ,INT8S *string);
void	CTeach_ParaSelect(void);
void	CTeach_UpdateSpeed(int speed );
BOOL	CTeach_DataOk(void);
BOOL	CTeach_BackZero(int ch);
BOOL	CTeach_GotoStart(void);
void 	CTeach_ModeDisp(void);
void	UpdateCurAxis(INT32S AxisNum);
# endif

