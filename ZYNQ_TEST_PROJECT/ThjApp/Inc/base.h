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

#define      TEACH_TASK_PRIO 					(OS_LOWEST_PRIO-21)	//�̵��������,���ȼ����Ե�һЩ
#define      CHECK_KEY_TASK_PRIO    			(OS_LOWEST_PRIO-20)	//�����������,���ȼ��θ�
#define		 ASSISTANT_TASK_PRIO				(OS_LOWEST_PRIO-22)

#define		 VM_MOTION_TASK_PRIO				(OS_LOWEST_PRIO-59)	//�����˶�����

#define		 GET_COMDATA_TASK_PRIO				(OS_LOWEST_PRIO-19)	//������ť����

#define      CHECK_PROBE_TASK_PRIO  			(OS_LOWEST_PRIO-18)	//̽��������

#define		 RUN_CHECKIO_TASK_PRIO				(OS_LOWEST_PRIO-17)	//IO�������
#define      DATAUPACK_TASK_PRIO    			(OS_LOWEST_PRIO-16)	//���ݽ�������
#define		 DISPLAY_RUNSPEEDCOUNT_TASK_PRIO	OS_LOWEST_PRIO-15	//��ʾ�ӹ��ٶ�����
#define		 DISPLAY_RUNLINE_TASK_PRIO			OS_LOWEST_PRIO-14	//��ʾ������������
#define		 DISPLAY_MSG_TASK_PRIO				OS_LOWEST_PRIO-13	//��ȡ��Ϣ���в���ʾ����
#define      START_TASK_PRIO 	   				OS_LOWEST_PRIO-12	//��ʼ����
#define      LED_OUT_TASK_PRIO      			OS_LOWEST_PRIO-11	//LED����ʾ
#define		 DISPLAY_XYZ_TASK_PRIO				OS_LOWEST_PRIO-10	//����ˢ������
//#define      DISPRUNTIME_TASK_PRIO 	   			OS_LOWEST_PRIO-9	//��ʾ����ʱ��
#define      DISPLAY_TIME_TASK_PRIO      		OS_LOWEST_PRIO-8	//ʱ��ˢ������


#define		 TEST_STK_TASK_PRIO					(OS_LOWEST_PRIO-3)	//��ջ����������


#define          CHECK_PROBE_TASK_ID			CHECK_PROBE_TASK_PRIO
#define          CHECK_KEY_TASK_ID				CHECK_KEY_TASK_PRIO
#define          TEACH_TASK_ID					TEACH_TASK_PRIO
#define			 VM_MOTION_TASK_ID				VM_MOTION_TASK_PRIO

#define		 RUN_CHECKIO_TASK_ID				RUN_CHECKIO_TASK_PRIO
#define		 GET_COMDATA_TASK_ID				GET_COMDATA_TASK_PRIO
#define		 ASSISTANT_TASK_ID				    ASSISTANT_TASK_PRIO
#define		 DATAUPACK_TASK_ID					DATAUPACK_TASK_PRIO
#define		 DISPLAY_RUNSPEEDCOUNT_TASK_ID		DISPLAY_RUNSPEEDCOUNT_TASK_PRIO
#define		 DISPLAY_RUNLINE_TASK_ID			DISPLAY_RUNLINE_TASK_PRIO
#define		 DISPLAY_XYZ_TASK_ID				DISPLAY_XYZ_TASK_PRIO
#define		 DISPLAY_MSG_TASK_ID				DISPLAY_MSG_TASK_PRIO
#define      START_TASK_ID						START_TASK_PRIO
#define      LED_OUT_TASK_ID					LED_OUT_TASK_PRIO
#define      DISPLAY_TIME_TASK_ID				DISPLAY_TIME_TASK_PRIO
//#define		 DISPRUNTIME_TASK_ID				DISPRUNTIME_TASK_PRIO

#define		 TEST_STK_TASK_ID					TEST_STK_TASK_PRIO

#define		 KEY_QUEUE_SIZE				10		  
#define      LEDOUT_QUEUE_SIZE     		8         
#define      MSG_QUEUE_SIZE     		6         
#define      RUNLINE_QUEUE_SIZE     	20        
//#define	 KEY_QUEUE_SIZE	  		10

#define		TEST_STK_EN			0	//�����Ƿ����ö�ջ�������

//#define		BOOL 				BOOLEAN //���䶨��BOOL�ͱ�����ȷ������ǰ�з���ͳһ

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))
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
void	CBmp_GetFullScr24Bit(char *filename,INT16S isVirtual);
void	CDC_OutCenText32(CRect rect,char *string,INT32S color,INT32S bk);

INT32S SuperPassword(INT32S ID ,INT32S SerialNo);
void	CDC_OutRightText24(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
INT16S 	CTelCom_SendString(INT8U *buf,INT32S len,INT32S lTimeout);
void	CDC_Rectangle3(CRect rect,INT32S color);
void	CDC_Rectangle4(INT16S x1,INT16S y1,INT16S x2,INT16S y2,INT32S color);
//void	CDC_DrawWin26(INT16S x,INT16S y,INT16S w,INT16S h,char *title);
void	CDC_DrawEdit_1(CRect	rect,INT32S color);


//void	CDC_OutCenTextKzk26(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutCenTextKzk24(CRect rect,char *string,INT32S color,INT32S bk);
//void	CDC_OutCenTextSzk20(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutCenTextSzk24(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutCenTextKzk22(CRect rect,char *string,INT32S color,INT32S bk);
//void	CDC_OutCenTextKzk16(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_OutCenTextKzk22Newline(CRect rect,char *string,INT32S color,INT32S bk);

void 	CDC_SetRectangle1(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT32S color,INT16S style);
void 	CDC_SetRectangle2(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT32S color,INT16S style);
void CDC_SetRectangle3(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT16S titleLen,INT32S color,INT16S style);
void	CDC_OutCenText1(INT16S x1,INT16S x2,INT16S y,char *string,INT32S color,INT32S bk);
void	CDC_DrawWin1(INT16S x,INT16S y,INT16S w,INT16S h,char *title);
void	CDC_OutLeftTextSzk24(CRect rect,char *string,INT32S color,INT32S bk);
void	CDC_TextSelect_24(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk);
void	CDC_AsciiCharMzk_24(INT16S x,INT16S y,  INT8S ch,INT32S color,INT32S bk);
void	CDC_DrawEdit_2(CRect	rect,INT32S color);
INT8S*  axisMsg(INT16S ch,INT8S* msg,BOOL bMsg);
void 	ps_debugout(char *fmt, ...);
void	Uart_Printf(char *fmt, ...);


#define	SELFGRAPH	1

#if SELFGRAPH>0
INT16S		getmaxx(void);
INT16S		getmaxy(void);
#endif

# endif
