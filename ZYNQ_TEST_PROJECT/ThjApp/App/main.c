
/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        main.c
  �����ˣ�       ��־��
  �������ڣ�	 2006/02/20
  �汾��		 V2.00
  ��Ҫ��������:   ������ϵͳ�����ļ��Լ����ļ�

  �޸ļ�¼:
  1. ...
*************************************************/
#define  TEST_GLOBALS
#include "includes.h"

//���������Ķ�ջ��С

TEST_EXT	OS_STK           VmMotionStk[TASK_STK_SIZE*8];
TEST_EXT	OS_STK           TeachStk[TASK_STK_SIZE*8];
TEST_EXT	OS_STK           CheckKeyStk[TASK_STK_SIZE];
TEST_EXT	OS_STK           AssistantStk[TASK_STK_SIZE];
TEST_EXT	OS_STK           CheckProbeStk[TASK_STK_SIZE*8];
TEST_EXT	OS_STK           RunCheckIOStk[TASK_STK_SIZE*8];
TEST_EXT	OS_STK           DataUpackStk[TASK_STK_SIZE*4];
TEST_EXT	OS_STK           StartStk[TASK_STK_SIZE*8];
TEST_EXT	OS_STK           DisplayRunSpeedCountStk[TASK_STK_SIZE*4];
TEST_EXT	OS_STK			 DisplayXYZStk[TASK_STK_SIZE*2];
TEST_EXT	OS_STK			 DisplayRunLineStk[TASK_STK_SIZE*2];
TEST_EXT	OS_STK			 DisplayMsgStk[TASK_STK_SIZE];
TEST_EXT	OS_STK           LedOutStk[TASK_STK_SIZE];
TEST_EXT	OS_STK           DisplayTimeStk[TASK_STK_SIZE];
TEST_EXT	OS_STK           DispRunTimeStk[TASK_STK_SIZE];

TEST_EXT	OS_EVENT 		*KeyMBox;	 										//��������
TEST_EXT	OS_EVENT		*RunSpeedCountSem;
TEST_EXT	OS_EVENT		*RunLineQueue;
TEST_EXT	OS_EVENT		*FreeSem;											//��������ź���
TEST_EXT	OS_EVENT		*FileSem;											//�ļ������ź���,��ֹͬʱ���д���IO
TEST_EXT	OS_EVENT		*CardSem;
TEST_EXT	OS_EVENT		*EditSem;
TEST_EXT	OS_EVENT		*COMSem;        									//������ʴ��ڵ�SEM
TEST_EXT	OS_EVENT		*XMSSem;											//�������XMS��SEM
TEST_EXT	OS_EVENT		*UnpackDataSem;
TEST_EXT	OS_EVENT		*DispRunTimeSem;
TEST_EXT	OS_EVENT        *MsgQueue;  										//��ʾ��Ϣ����ָ��
TEST_EXT	OS_EVENT		*LedOutQueue;

void            			*LedOutQueueTbl[LEDOUT_QUEUE_SIZE]; 				// Storage for messages
void            			*MsgQueueTbl[MSG_QUEUE_SIZE]; 						// Storage for messages
void            			*RunLineQueueTbl[RUNLINE_QUEUE_SIZE]; 				// Storage for messages

TEST_EXT	INT16S 			g_iDlgLeft=0;
TEST_EXT	INT16S			g_iDlgTop=0;
TEST_EXT	INT16S			g_iDlgWidth=100;
TEST_EXT	INT16S			g_iDlgHeight=70;
TEST_EXT	INT16S			g_iDlgCurObject=0;
TEST_EXT	INT16S			g_iDlgIDOk=1;
TEST_EXT	BOOL			Ok_Exit=FALSE;
TEST_EXT	INT8S	   	 	g_pszDlgDispStr[0x80]="\0";
TEST_EXT	INT8S   		g_pszDlgWindowText[0x35]="\0";
TEST_EXT	INT32S   		g_lDlgTextColor=BLACKCOLOR;
TEST_EXT	INT32S			g_lBmpStartTime=-1;
TEST_EXT	BOOL			g_bSysParaCorrect=TRUE;
TEST_EXT	BOOL			g_bEnglish=FALSE;
TEST_EXT	BOOL			g_bLCD=FALSE;										//ָʾ�Ƿ�֧�ֽ���ģʽ
extern  	BASEINI 		g_Ini;
TEST_EXT	BOOL			g_bPM=FALSE;										//ָʾ�Ƿ�֧�ֽ���ģʽ
TEST_EXT	BOOL			g_bRam=FALSE;										//ָʾ�Ƿ��д���RAM�����Ƿ����

INT16U 						Har_Ver1(void);
INT16U						Har_Ver2(void);
void 						StringCopy(INT8S* buf,INT8S* message);
//struct 		ip_addr 		fromaddr;
u16_t 						fromport;

#define 	r_INT(x)        (*(volatile unsigned char *)(0x28000000+x))
#define 	IRON_RAM_SIZE	0x8000
#define		g_BkButton		DARKGREENCOLOR2

extern		int 			usb_max_devs ; 										// number of highest available usb device//fzz modify
//lcd_jg 		lcd_x;
void  						xs_lcd(INT8S *buff);
//extern 		void			LCD_xs(lcd_jg buff);
extern		void			PWM_Control(INT32U frequency,FP32 Ontime,INT8U channel);
#define		FREQ			1000

void 		InitLwIP(void);
void 		SetupLwIP(void);
void 		LCD_TEST(void);
void 		DRAM_Test(void);
void 		WRData_Test(void);
void 		CMPData_Test(void);
void 		AT24c08_Test(void);

OS_STK 		Task_Output_Stack[TASK_STK_SIZE];									//ִ��ѭ���������
void 		Task_Output(void *Id);
#define 	Task_Output_PRIO	OS_LOWEST_PRIO - 15

#define 	DA_STK_SIZE   	512
#define 	DA_prio      	OS_LOWEST_PRIO - 7
OS_STK           			DA_task[DA_STK_SIZE];
#define		OS_KEYLED_PRIO	OS_LOWEST_PRIO - 4
#define 	LED_STK_SIZE   	512
OS_STK		Led_Tey[LED_STK_SIZE];
extern 		OS_EVENT		*CDCSem;											//CDC��ͼ�ź�����ֹ��������ͬʱ��ͼ
void		Start_Task(void *data);												//��������������

#if	TEST_STK_EN>0
	void	Test_Stk_Task(void *data);											//��ջ�������
#endif

extern		OS_EVENT		*gMoveStopSEM0;
extern		OS_EVENT		*gMoveStopSEM1;

unsigned 	int 			buffth[1024*1024];
unsigned 	int 			buff[8192];
unsigned 	int 			ched[8192];
unsigned 	int 			kexx=0;

INT32S 		g_bInitbmp	=	FALSE;   //���LOGOͼƬ�Ƿ��ʼ���ɹ���FASLE--ͼƬ��ʼ��ʧ�ܣ�TRUE--ͼƬ��ʼ���ɹ���20140807

//TEST_EXT	BOOL			g_bRelease=FALSE;
TEST_EXT	BOOL			g_bRelease=TRUE;


void 		*yaffsfs_GetFileOpsSem(void);
void 		*FS_X_OS_GetLockDeviceOpSem (void);

OS_EVENT 	*FlashDeviceOpSem;

/*
struct  DEVICECFGINFO  YF_TH08HA =  										//����������ýṹ�����
{
 	TH08HA,    																// �����ͺ�
	KEYBOARD_TO_ARM,  														// ����������
 	TH08HA,    																// ��ʾ������
 	TB_NULL,    															// ����������
};
*/

void ZK_INIT(void)
{
	INT8U *pArray;

	if(MG_GetMem("ADT\\ZK\\MZK32X.BIN",(INT32U*)&pArray))
	{
		Uart_Printf("can't open MZK32X binfile\n");
		goto Err;
	}

	if(!CDC_LoadZKLIB(MZK_32,pArray))
	{
		Uart_Printf("can't register zk\n");
		goto Err;
	}


	if(MG_GetMem("ADT\\ZK\\YH32X.BIN",(INT32U*)&pArray))
	{
		Uart_Printf("can't open YH32X binfile\n");
		goto Err;
	}

	if(!CDC_LoadZKLIB(YH_32,pArray))
	{
		Uart_Printf("can't register zk\n");
		goto Err;
	}

	/****************************************************************/

	Err:
	return;
}


int main (void)
{

	INT16U ver ;
																//��ʼ��Ԥ�����ڴ�

	OSInit();																	//uCOS-II��ʼ��

	UCOS_OSII_Init();

	OSTimeSet(0);

    ver = OSVersion();

    Uart_Printf("system starting... ��%d\n",ver);



   	strcpy(TaskUserData[START_TASK_ID].TaskName, "StartTask");

	OSTaskCreateExt(Start_Task,
					(void *)0,
					&StartStk[TASK_STK_SIZE*1 - 1],
					START_TASK_PRIO,
					START_TASK_ID,
					&StartStk[0],
					TASK_STK_SIZE*1,
					&TaskUserData[START_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	OSStart();     																//����������

}

/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/




void  Start_Task (void *data)
{

	INT8U err;

	data = data;

	//OSStatInit();
	UCOS_Startup();

//	SetKeyType(YF_TH08HA.KeyType);

	LedOutQueue 	= OSQCreate(&LedOutQueueTbl[0], LEDOUT_QUEUE_SIZE);			/* Create a message queue                   */
	MsgQueue 		= OSQCreate(&MsgQueueTbl[0], MSG_QUEUE_SIZE);				/* Create a message queue                   */
	RunLineQueue 	= OSQCreate(&RunLineQueueTbl[0], RUNLINE_QUEUE_SIZE);		/* Create a message queue                   */
	KeyMBox	    	= OSMboxCreate((void *)0);
	RunSpeedCountSem= OSSemCreate(0); 											//ϵͳ����ʱ��ˢ�������ٶ��Լ���������

	CardSem = OSSemCreate(1);
	EditSem = OSSemCreate(0);
	FreeSem = OSSemCreate(0);
	FileSem = OSSemCreate(1);
	CDCSem	= OSSemCreate(1);
	COMSem	= OSSemCreate(1);
	XMSSem	= OSSemCreate(1);
	UnpackDataSem  = OSSemCreate(0);
	DispRunTimeSem = OSSemCreate(0);

	// ****************Ӳ����ʼ�� ***************** //
	{
		int ret;
		FATFS   	FatDev[4];

		// *******�ײ��ʼ������*****  //
		ret=HW_LowLevelInit(0,ZYNQ_LIB_BUS_TYPE);

		if(ret != ERR_NONE)
		{
			Uart_Printf("XXXX HW_LowLevelInit err: %d XXXX \r\n", ret);
		}

		/*��ʼ��������Ϣ*/
		A9_2440_SetMachineType(MT_TH08HA_KB48);


		/*---�ļ����ص��ʼ�� ---*/

		ret = f_mount(&FatDev[0],"0:",1);
		if(ret != FR_OK)
		{
			Uart_Printf("##0 fatfs f_mount error ## 0x%x\r\n",ret);
		}else
			Uart_Printf("##0 fatfs f_mount success!\r\n");

		ret = f_mount(&FatDev[1],"1:",1);
		if(ret != FR_OK)
		{
			Uart_Printf("##1 fatfs f_mount error ## 0x%x\r\n",ret);
		}
		else
			Uart_Printf("##1 fatfs f_mount success!\r\n");

		ret = f_mount(&FatDev[2],"2:",1);
		if(ret != FR_OK)
		{
			Uart_Printf("##2 fatfs f_mount error ## 0x%x\r\n",ret);
		}
		else
			Uart_Printf("##2 fatfs f_mount success!\r\n");

		InitXMS();
		CDC_Init(2);																//����Ϊ2��800*480  3Ϊ800*600
	//	LCD_Power(1);																//1�򿪣�0����


		LCD_Power(0);//������ֹ��ʼ������
	//	Uart_Printf("����\n");
		OSTimeDly(100);


		/*--- �������ʼ�� ---*/
		key_init();

		/*--- �������ʼ�� ---*/
		g_bRam = CheckRam();

		if(!g_bRam)
		{
			Uart_Printf("Ram Error!\n");
		}

		File_Init();								//�ļ��нṹ��ʼ��

		if(!InitHzk())																//�����ڶ������������ź���������ִ��
		{
			Uart_Printf("Hzk Init Failed!\n");
		}
	}
	//  *****Ӳ����ʼ��  end******* //


	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),BLACKCOLOR,100);

	LCD_Power(1);
	g_bLCD = TRUE;


	//  *******ע���� ******   //
	/*
	{
		INT32S 	i=0;
		BOOL 	bIdErr=TRUE;

		for(i=0; i<ID_NUM; i++)
		{
			g_iProductErron[i]=WriteProductId(g_iProductId[i]);						//ע��ID����ֹ���ش�����Ŀ��,DJ009B007A��ID Ϊ0xa44880e0
		}

		for(i=0;i<ID_NUM;i++)
		{
			bIdErr=bIdErr && (g_iProductErron[i]!=0);
		}

		if(bIdErr)
		{
			CDC_OutText24(200,240,"��ʶ�벻ƥ��,������ע��ID!!",REDCOLOR,0);
			CDC_OutText24(50,270,"ID Mismatches.Please Register a Right ID!!",REDCOLOR,0);

			while(1)
			{
				OSTimeDly(1);
			}
		}
	}*/


	//  *************	����NCP��Դ��   ****************   //

	{
		INT16U ErrNo;

		ErrNo = MG_UNMERGE((char *)"0:\\ADT\\NC_RES.NCP");
		if (ErrNo)
		{
			INT16U key;

			Uart_Printf("mg_unmerge error:%d\n",ErrNo);

			CDC_OutText24(20,125,g_bEnglish?"lack of file called NC_RES.NCP":"ȱ��NC_RES.NCP�ļ���",REDCOLOR,BLACKCOLOR);
			CDC_OutText24(20,150,g_bEnglish?"key <ENTER> recopy <NC_RES.NCP>":"��<ENTER>����ϵͳ",WHITECOLOR,BLACKCOLOR);

			while(1)
			{
				key = GetKey(TRUE);

			//	Uart_Printf("key=%d\n",key);

				if(key==TENTER)
				{
					break;
				}

				OSTimeDly(3);
			}
		}
		else
			Uart_Printf("load mg_unmerge success!\n");


		ZK_INIT();

		TransferLanguagePack(1);

	}

/**************************************************************/




	CDC_OutText24Ex(300,400,"Start System ...",WHITECOLOR,BLACKCOLOR);

	OSTimeDly(300);

	Uart_Printf("SysPara Initing...\n");

	CDC_OutText24Ex(300,400,"Loading System setting...",WHITECOLOR,BLACKCOLOR);

	if(!SysParaRead())
	{
		INT16U key=0;

		//if(!SysParaRead())
		{
			g_bSysParaCorrect=FALSE;
			SysParaInit();

			CDC_OutText24(200,200,"Press any key to continue...                ",REDCOLOR,BLACKCOLOR);
			CDC_OutText24(300,400,"System init...                ",WHITECOLOR,BLACKCOLOR);
			CTimer_Open(0);
			CTimer_SetTimes(20000,0);
			CTimer_Begin(0);

			while(1)
			{
				key = GetKey(TRUE);

				if(key)
					break;

				if( CTimer_IsTimeOut(0))
					break;

				OSTimeDly(10);														//������䲻��û��

			}

			CTimer_Close(0);

		}
	}
	else
	{
	//	SysParaWrite();
	}

	DisableParaInit();

	g_bEnglish = (g_Ini.iLanguage==0?FALSE:TRUE);
	OSTimeDly(300);

	Uart_Printf("CtrlCard Initing\n");

	CDC_OutText24Ex(300,400,g_bEnglish?"Hardware initing...         ":"Ӳ����ʼ��...            ",WHITECOLOR,BLACKCOLOR);

	CtrlCard_Init();

	if(!InitBoard())
	{
		CDC_PutColorBar(0,0,getmaxx(),getmaxy(),BLACKCOLOR,100);
		CDC_OutText24Ex(300,300,g_bEnglish?"Hardware Error!Press any key...":"Ӳ���쳣!�������...",WHITECOLOR,BLACKCOLOR);
		Uart_Printf("Not found ctrlcard!\n");
		GetKey(FALSE);
	}

	OSTimeDly(100);

	Uart_Printf("\r\n��Ŀ��:%s\r\n",PROJECT_ID);
	Uart_Printf("����汾:%d\r\n",VER);
	Uart_Printf("�޸�ʱ��:%s, %s\r\n",__DATE__,__TIME__);

	Uart_Printf("Create Task...\n");

	g_bInitbmp=InitDispBMP("\\PARA\\start.bmp");



	Uart_Printf("Create Task...\n");

	strcpy(TaskUserData[VM_MOTION_TASK_ID].TaskName, "Vm Motion Task");			//�����˶�����
	OSTaskCreateExt(vm_motion_task,
					(void *)0,
					&VmMotionStk[TASK_STK_SIZE*8 - 1],
					VM_MOTION_TASK_PRIO,
					VM_MOTION_TASK_ID,
					&VmMotionStk[0],
					TASK_STK_SIZE*8,
					&TaskUserData[VM_MOTION_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);


	strcpy(TaskUserData[CHECK_KEY_TASK_ID].TaskName, "Check Key Task");
	OSTaskCreateExt(Check_Key_Task,
					(void *)0,
					&CheckKeyStk[TASK_STK_SIZE - 1],
					CHECK_KEY_TASK_PRIO,
					CHECK_KEY_TASK_ID,
					&CheckKeyStk[0],
					TASK_STK_SIZE,
					&TaskUserData[CHECK_KEY_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	CheckPassword();  															//��������

	strcpy(TaskUserData[DATAUPACK_TASK_ID].TaskName, "Data Upack Task");
	OSTaskCreateExt(DataUpack_Task,
					(void *)0,
					&DataUpackStk[TASK_STK_SIZE*4-1],
					DATAUPACK_TASK_PRIO,
					DATAUPACK_TASK_ID,
					&DataUpackStk[0],
					TASK_STK_SIZE*4,
					&TaskUserData[DATAUPACK_TASK_ID],
					OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);


	strcpy(TaskUserData[TEACH_TASK_ID].TaskName,"Teach Task");
	OSTaskCreateExt(Teach_Task,
					(void *)0,
					&TeachStk[TASK_STK_SIZE*8 - 1],
					TEACH_TASK_PRIO,
					TEACH_TASK_ID,
					&TeachStk[0],
					TASK_STK_SIZE*8,
					&TaskUserData[TEACH_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR	|OS_TASK_OPT_SAVE_FP);

	for (;;)
	{
		OSSemPend(EditSem,0,&err);

		if(err == OS_NO_ERR)
		{
			CEdit_UpdateData(TRUE);
		}
	}
}


void  TaskStartCreateTasks (void)
{

	strcpy(TaskUserData[LED_OUT_TASK_ID].TaskName, "Led Out Task");
	OSTaskCreateExt(LedOut_Task,
					(void *)0,
					&LedOutStk[TASK_STK_SIZE - 1],
					LED_OUT_TASK_PRIO,
					LED_OUT_TASK_ID,
					&LedOutStk[0],
					TASK_STK_SIZE,
					&TaskUserData[LED_OUT_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR	|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[DISPLAY_TIME_TASK_ID].TaskName, "Disp Time Task");
	OSTaskCreateExt(Display_Time_Task,
					(void *)0,
					&DisplayTimeStk[TASK_STK_SIZE - 1],
					DISPLAY_TIME_TASK_PRIO,
					DISPLAY_TIME_TASK_ID,
					&DisplayTimeStk[0],
					TASK_STK_SIZE,
					&TaskUserData[DISPLAY_TIME_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR	|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[DISPLAY_XYZ_TASK_ID].TaskName, "Disp XYZ Task");
	OSTaskCreateExt(Display_XYZ_Task,
					(void *)0,
					&DisplayXYZStk[TASK_STK_SIZE*2-1],
					DISPLAY_XYZ_TASK_PRIO,
					DISPLAY_XYZ_TASK_ID,
					&DisplayXYZStk[0],
					TASK_STK_SIZE*2,
					&TaskUserData[DISPLAY_XYZ_TASK_ID],
					OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);


	strcpy(TaskUserData[DISPLAY_RUNLINE_TASK_ID].TaskName, "Disp RunLine Task");
	OSTaskCreateExt(Display_RunLine_Task,
					(void *)0,
					&DisplayRunLineStk[TASK_STK_SIZE*2-1],
					DISPLAY_RUNLINE_TASK_PRIO,
					DISPLAY_RUNLINE_TASK_ID,
					&DisplayRunLineStk[0],
					TASK_STK_SIZE*2,
					&TaskUserData[DISPLAY_RUNLINE_TASK_ID],
					OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[DISPLAY_MSG_TASK_ID].TaskName, "Disp Msg Task");
	OSTaskCreateExt(Display_Msg_Task,
					(void *)0,
					&DisplayMsgStk[TASK_STK_SIZE - 1],
					DISPLAY_MSG_TASK_PRIO,
					DISPLAY_MSG_TASK_ID,
					&DisplayMsgStk[0],
					TASK_STK_SIZE,
					&TaskUserData[DISPLAY_MSG_TASK_ID],
					OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR	|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[CHECK_PROBE_TASK_ID].TaskName, "RunCheck Probe Task");
	OSTaskCreateExt(Check_Probe_Task,
					(void *)0,
					&CheckProbeStk[TASK_STK_SIZE*8-1],
					CHECK_PROBE_TASK_PRIO,
					CHECK_PROBE_TASK_ID,
					&CheckProbeStk[0],
					TASK_STK_SIZE*8,
					&TaskUserData[CHECK_PROBE_TASK_ID],
					OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[RUN_CHECKIO_TASK_ID].TaskName, "RunCheckIO Task");
	OSTaskCreateExt(Run_CheckIO_Task,
					(void *)0,
					&RunCheckIOStk[TASK_STK_SIZE*8-1],
					RUN_CHECKIO_TASK_PRIO,
					RUN_CHECKIO_TASK_ID,
					&RunCheckIOStk[0],
					TASK_STK_SIZE*8,
					&TaskUserData[RUN_CHECKIO_TASK_ID],
					OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[DISPLAY_RUNSPEEDCOUNT_TASK_ID].TaskName, "DispSpedCunt Task");
	OSTaskCreateExt(Display_RunSpeedCount_Task,
				(void *)0,
				&DisplayRunSpeedCountStk[TASK_STK_SIZE*4-1],
				DISPLAY_RUNSPEEDCOUNT_TASK_PRIO,
				DISPLAY_RUNSPEEDCOUNT_TASK_ID,
				&DisplayRunSpeedCountStk[0],
				TASK_STK_SIZE*4,
				(void *)0,
				OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

	strcpy(TaskUserData[ASSISTANT_TASK_ID].TaskName, "Assistant_Task");
	OSTaskCreateExt(Assistant_Task,
				(void *)0,
				&AssistantStk[TASK_STK_SIZE-1],
				ASSISTANT_TASK_PRIO,
				ASSISTANT_TASK_ID,
				&AssistantStk[0],
				TASK_STK_SIZE,
				(void *)0,
				OS_TASK_OPT_STK_CHK||OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

}

