# ifndef 	___BASE_H___
# define 	___BASE_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        base.h
  创建人：       范志州
  创建日期：	2006/02/20
  主要功能描述:  各个任务的优先级以及ID分配，各个常用全局函数声明，另外包含了按键检测任务

  修改记录：
   1.  
*************************************************/

#include "os_cpu.h"
#include "rect.h"


//#define			PRINTBMP

#ifdef      	TEST_GLOBALS
#define     	TEST_EXT
#else
#define     	TEST_EXT  			extern
#endif

#define          TASK_STK_SIZE     	    2048 //堆栈分配的基准（单位:OS_STK)

#define      TEACH_TASK_PRIO 					(OS_LOWEST_PRIO-21)//教导编程任务,优先级可以低一些
#define      CHECK_KEY_TASK_PRIO    			(OS_LOWEST_PRIO-20)//按键检测任务,优先级次高
#define		 ASSISTANT_TASK_PRIO				(OS_LOWEST_PRIO-22)
#define		 VM_MOTION_TASK_PRIO				(OS_LOWEST_PRIO-59)	//虚拟运动任务
//#define		 GET_COMDATA_TASK_PRIO				(OS_LOWEST_PRIO-20)
#define      CHECK_PROBE_TASK_PRIO  			(OS_LOWEST_PRIO-19)
#define		 RUN_CHECKIO_TASK_PRIO				(OS_LOWEST_PRIO-18)
#define      DATAUPACK_TASK_PRIO    			(OS_LOWEST_PRIO-17)
#define		 DISPLAY_RUNSPEEDCOUNT_TASK_PRIO	OS_LOWEST_PRIO-16
#define		 DISPLAY_RUNLINE_TASK_PRIO			OS_LOWEST_PRIO-15
#define		 DISPLAY_MSG_TASK_PRIO				OS_LOWEST_PRIO-14
#define      START_TASK_PRIO 	   				OS_LOWEST_PRIO-13//起始任务
#define      LED_OUT_TASK_PRIO      			OS_LOWEST_PRIO-12
#define		 DISPLAY_XYZ_TASK_PRIO				OS_LOWEST_PRIO-11//坐标刷新任务
#define      DISPRUNTIME_TASK_PRIO 	   			OS_LOWEST_PRIO-10
#define      DISPLAY_TIME_TASK_PRIO      		OS_LOWEST_PRIO-9//时间刷新任务


#define		 TEST_STK_TASK_PRIO					(OS_LOWEST_PRIO-3)//堆栈检测测试任务


#define          CHECK_PROBE_TASK_ID			CHECK_PROBE_TASK_PRIO
#define          CHECK_KEY_TASK_ID				CHECK_KEY_TASK_PRIO
#define          TEACH_TASK_ID					TEACH_TASK_PRIO
#define			 VM_MOTION_TASK_ID				VM_MOTION_TASK_PRIO

#define		 RUN_CHECKIO_TASK_ID				RUN_CHECKIO_TASK_PRIO
//#define		 GET_COMDATA_TASK_ID				GET_COMDATA_TASK_PRIO
#define		 ASSISTANT_TASK_ID				    ASSISTANT_TASK_PRIO
#define		 DATAUPACK_TASK_ID					DATAUPACK_TASK_PRIO
#define		 DISPLAY_RUNSPEEDCOUNT_TASK_ID		DISPLAY_RUNSPEEDCOUNT_TASK_PRIO
#define		 DISPLAY_RUNLINE_TASK_ID			DISPLAY_RUNLINE_TASK_PRIO
#define		 DISPLAY_XYZ_TASK_ID				DISPLAY_XYZ_TASK_PRIO
#define		 DISPLAY_MSG_TASK_ID				DISPLAY_MSG_TASK_PRIO
#define      START_TASK_ID						START_TASK_PRIO
#define      LED_OUT_TASK_ID					LED_OUT_TASK_PRIO
#define      DISPLAY_TIME_TASK_ID				DISPLAY_TIME_TASK_PRIO
#define		 DISPRUNTIME_TASK_ID				DISPRUNTIME_TASK_PRIO

#define		 TEST_STK_TASK_ID					TEST_STK_TASK_PRIO

#define		 KEY_QUEUE_SIZE				10		  
#define      LEDOUT_QUEUE_SIZE     		8         
#define      MSG_QUEUE_SIZE     		6         
#define      RUNLINE_QUEUE_SIZE     	20        
//#define	 KEY_QUEUE_SIZE	  		10

#define		TEST_STK_EN			0	//定义是否启用堆栈检测任务

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))


//#define		BOOL 				BOOLEAN //补充定义BOOL型变量以确保跟以前叫法的统一

//*
//定义扩展型任务的数据结构，以统计任务名，任务执行次数，任务本次执行时间，任务总的执行时间
typedef struct
{
	INT8S   TaskName[30];
	INT16U  TaskCtr;
	INT16U  TaskExecTime;
	INT32U  TaskTotExecTime;
} TASK_USER_DATA;

TEST_EXT TASK_USER_DATA  TaskUserData[OS_LOWEST_PRIO];
//*
//定义扩展型任务的数据结构，以统计任务名，任务执行次数，任务本次执行时间，任务总的执行时间

typedef struct{
	INT16U 	x;
	INT16U 	y;
	INT16U 	w;
	INT16U 	h;
	INT8U	text[40];
}MENU;


#define DebugDisp Uart_Printf
//#define	DebugDisp //

#define	PAI	3.1415926

TEST_EXT TASK_USER_DATA  TaskUserData[OS_LOWEST_PRIO];


//BOOL	ReadBiosSys(void);
//BOOL	ReadBiosHzk(INT16S ch);
//BOOL	HzkToXMS(INT16U handle,INT32S len,FS_FILE *fp);
//BOOL	InitHzk(void);
BOOL	CopyFile(INT8S *dest,INT8S *src);

INT32S	Rand(INT32S sd);
void 	ExitSystem(INT16S ex); 


void	LedOut(INT8U num,INT8U status);
void	AllLedOff(void);

BOOL	IsPressKey(void);
INT16U 	GetKey(BOOL imreturn);	
void	ClearKeyQueue(void);
void    Check_Key_Task(void *data);
INT16U	KeyMBoxGetKey(void);
BOOL	PressKeyTime(INT16U ti);

void   DispTaskStat(INT8U id);
void   TaskStartCreateTasks(void);

BOOL	ArcComp(FP32 x[],FP32 cen[]);
void    CDC_AsciiChar24(INT16S x,INT16S y,  INT8S ch,INT32S color,INT32S bk);
void	CDC_OutCenText24(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutCenText24Ex(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_DrawWin24(INT16S x,INT16S y,INT16S w,INT16S h,char *title);
//void	CBmp_GetFullScr24Bit(char *filename,INT16S isVirtual);
//char	CreatBMP(void *filename,INT16S Wide,INT16S Hight,INT16S color);
INT32S  SuperPassword(INT32S ID ,INT32S SerialNo);
void	CDC_OutRightText24(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);

void	CDC_DrawArcEdit(INT16S x, INT16S y, INT16S w, INT16S h,INT32S color);
void	CDC_TextCen24(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk);	
void	CDC_TextCen32(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk);
void	CDC_TextSelect24(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk);

void 	CDC_OutText24Ex(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void 	CDC_OutText_32(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void	CDC_TextSelect_32(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk);
void	CDC_TextCen_32(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk);
void 	CDC_OutText_48(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void	CDC_DrawEdit_2(CRect	rect,INT32S color);

void	CDC_OutRightText24Ex(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutLeftText24Ex(CRect rect,char *string,INT32S color,INT32S bk);
//---------------左右对齐输出固定长度字符串--------------//
void CDC_OutTextLR_UTF(INT16S x,INT16S y,char *string,INT32S color,INT32S bk,int zktype,int ilen,int iLR);


void 	ps_debugout(char *fmt, ...);

void	Uart_Printf(char *fmt, ...);



#define	SELFGRAPH	1

#if SELFGRAPH>0
	INT16S		getmaxx(void);
	INT16S		getmaxy(void);
#endif

#endif
