# ifndef 	___BASE_H___
# define 	___BASE_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        base.h
  �����ˣ�       ��־��
  �������ڣ�	2006/02/20
  ��Ҫ��������:  ������������ȼ��Լ�ID���䣬��������ȫ�ֺ�����������������˰����������

  �޸ļ�¼��
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

#define          TASK_STK_SIZE     	    2048 //��ջ����Ļ�׼����λ:OS_STK)

#define      TEACH_TASK_PRIO 					(OS_LOWEST_PRIO-21)//�̵��������,���ȼ����Ե�һЩ
#define      CHECK_KEY_TASK_PRIO    			(OS_LOWEST_PRIO-20)//�����������,���ȼ��θ�
#define		 ASSISTANT_TASK_PRIO				(OS_LOWEST_PRIO-22)
#define		 VM_MOTION_TASK_PRIO				(OS_LOWEST_PRIO-59)	//�����˶�����
//#define		 GET_COMDATA_TASK_PRIO				(OS_LOWEST_PRIO-20)
#define      CHECK_PROBE_TASK_PRIO  			(OS_LOWEST_PRIO-19)
#define		 RUN_CHECKIO_TASK_PRIO				(OS_LOWEST_PRIO-18)
#define      DATAUPACK_TASK_PRIO    			(OS_LOWEST_PRIO-17)
#define		 DISPLAY_RUNSPEEDCOUNT_TASK_PRIO	OS_LOWEST_PRIO-16
#define		 DISPLAY_RUNLINE_TASK_PRIO			OS_LOWEST_PRIO-15
#define		 DISPLAY_MSG_TASK_PRIO				OS_LOWEST_PRIO-14
#define      START_TASK_PRIO 	   				OS_LOWEST_PRIO-13//��ʼ����
#define      LED_OUT_TASK_PRIO      			OS_LOWEST_PRIO-12
#define		 DISPLAY_XYZ_TASK_PRIO				OS_LOWEST_PRIO-11//����ˢ������
#define      DISPRUNTIME_TASK_PRIO 	   			OS_LOWEST_PRIO-10
#define      DISPLAY_TIME_TASK_PRIO      		OS_LOWEST_PRIO-9//ʱ��ˢ������


#define		 TEST_STK_TASK_PRIO					(OS_LOWEST_PRIO-3)//��ջ����������


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

#define		TEST_STK_EN			0	//�����Ƿ����ö�ջ�������

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))


//#define		BOOL 				BOOLEAN //���䶨��BOOL�ͱ�����ȷ������ǰ�з���ͳһ

//*
//������չ����������ݽṹ����ͳ��������������ִ�д��������񱾴�ִ��ʱ�䣬�����ܵ�ִ��ʱ��
typedef struct
{
	INT8S   TaskName[30];
	INT16U  TaskCtr;
	INT16U  TaskExecTime;
	INT32U  TaskTotExecTime;
} TASK_USER_DATA;

TEST_EXT TASK_USER_DATA  TaskUserData[OS_LOWEST_PRIO];
//*
//������չ����������ݽṹ����ͳ��������������ִ�д��������񱾴�ִ��ʱ�䣬�����ܵ�ִ��ʱ��

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
//---------------���Ҷ�������̶������ַ���--------------//
void CDC_OutTextLR_UTF(INT16S x,INT16S y,char *string,INT32S color,INT32S bk,int zktype,int ilen,int iLR);


void 	ps_debugout(char *fmt, ...);

void	Uart_Printf(char *fmt, ...);



#define	SELFGRAPH	1

#if SELFGRAPH>0
	INT16S		getmaxx(void);
	INT16S		getmaxy(void);
#endif

#endif
