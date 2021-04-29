/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        work.c
  �����ˣ�       ��־��
  �������ڣ�		2004/11/20
  ��Ҫ��������:  �жϣ��ӹ��Լ����ݽ����һЩ������� 

  �޸ļ�¼��
   1.  �޸���:	      ��־��
       �޸�����:	      2005/08/04
       ��Ҫ�޸�����:   �ӹ����տ��ƹ���
   2. ...
 *************************************************/

#define		WORK_GLOBALS

#include	"includes.h"
#include    "StrTool.h"
//#include	"del.h"



TEST_EXT	BOOL		g_bRelease;
TEST_EXT	OS_EVENT	*RunSpeedCountSem;
TEST_EXT    OS_EVENT 	*RunLineQueue;
TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT  	OS_EVENT 	*LedOutQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		g_bPM;
//TEST_EXT	BOOL		g_bBmp;
TEST_EXT	BOOL		g_bRam;
extern 		COMMAND		Line_Data[MAXLINE+1];
extern 		INT16S		Line_Data_Size;
extern 		UNITDATA	g_UnitData;
extern 		BASEINI		g_Ini;
extern   	const 		INT16U			g_uXMS;
extern	 	const		INT16U			g_uData;
extern 		INT16U		g_uBmpXMS;
extern		INT8U		g_ucXMSArray[XMSSIZE];
extern  	INT8U		g_ucDataArray[DATASIZE];

WORK_EXT	INT32S 		g_TextColor1=LIGHTWHITECOLOR;
WORK_EXT	INT32S 		g_TextColor2=YELLOWCOLOR;
WORK_EXT	INT32S 		g_TextColor3=MAGENTACOLOR;
WORK_EXT	INT32S 		g_TextColor4=CYANCOLOR; 
WORK_EXT	INT32S 		g_TextColor5=REDCOLOR;
WORK_EXT	INT32S		g_TextColor6=WHITECOLOR;
WORK_EXT	INT32S		g_TextColor7=DARKGRAYCOLOR;
WORK_EXT	INT32S 		g_TextColor8=LIGHTGRAYCOLOR;
WORK_EXT	INT32S		g_TextColor9=BLUECOLOR;
WORK_EXT	INT32S		g_TextColor10=LIGHTGRAYCOLOR;
WORK_EXT	INT32S		g_TextColor11=GREENCOLOR;

WORK_EXT	INT32S 		g_TableLine =GRAYCOLOR;
WORK_EXT	INT32S 		g_nBk		=BLACKCOLOR;
WORK_EXT	INT32S		g_nTechfrmBk=DARKGREENCOLOR2;
WORK_EXT	INT32S 		g_nTitleBk	=DARKBLUECOLOR;//CYANCOLOR; 
WORK_EXT	INT32S 		g_nTableBk	=BLACKCOLOR; 
WORK_EXT	INT32S 		g_Color1	=LIGHTBLACKCOLOR;

WORK_EXT 	INT8S		g_pszMsgBuf[0x30]="\0";									//�����Ϣ��ʾ���ݱ����þֲ�������BUG
WORK_EXT	INT8S		g_pszLedOut[8][3]={"\0","\0","\0","\0","\0","\0","\0","\0"};
WORK_EXT	INT16S 		g_iRunLine=0;		
WORK_EXT 	INT16S		g_iStaRunLine=0;	
WORK_EXT 	BOOL 	 	g_bScreen=FALSE;	   									//ָʾ��ǰ��Ļ��ʾ�Ǹ����棬TRUE��ʾ���ӹ�����
WORK_EXT 	BOOL	  	g_bFirstDispTime=TRUE;									//ָʾ�Ƿ���Ҫȫ��ˢ������ʱ����ʾ��TRUE��Ҫ
//WORK_EXT	BOOL		g_bFirstDispRunTime=FALSE;
WORK_EXT	BOOL		g_bDispRunTime=FALSE;
WORK_EXT	BOOL		g_bCanDispRunTime=FALSE;								//��ʾ��ʱ�Ƿ�ɽ�����ʾʣ��ʱ��ˢ�� ��


WORK_EXT	INT32S		g_lRunTime=100;		  
WORK_EXT	FP32   		g_fRunSpeed=0.0;
WORK_EXT    INT32S      g_lAxisSpeed[4]={10,10,10,10};
WORK_EXT 	BOOL   		g_bStartRun=FALSE;  
WORK_EXT 	BOOL		g_bRuning=FALSE;    
WORK_EXT 	BOOL		g_bModify=TRUE;  
WORK_EXT	BOOL		g_bModifySave=TRUE;
WORK_EXT	BOOL		g_bDataCopy=FALSE; 
WORK_EXT	BOOL		g_bUnpackCorrect=FALSE;
WORK_EXT	BOOL		g_bUnpackRuning=FALSE;
WORK_EXT	INT16S		g_iMoveAxis=0;
WORK_EXT 	BOOL		g_bFirstResetX=FALSE;
WORK_EXT 	BOOL		g_bFirstResetY=FALSE;
WORK_EXT 	BOOL		g_bFirstResetZ=FALSE;
WORK_EXT 	BOOL		g_bFirstResetU=FALSE;
WORK_EXT 	BOOL		g_bFirstResetV=FALSE;
WORK_EXT 	BOOL		g_bFirstResetW=FALSE;

WORK_EXT 	INT16S		g_iWorkStatus=NORUN;
WORK_EXT 	BOOL		g_bAutoRunMode=TRUE;

WORK_EXT 	INT32S		g_lStepNode=0;										//�ò���������������

WORK_EXT 	BOOL		g_bStepIO = FALSE;  								//�ڸ�ƽ̨�ò�����ʾ���ڲ��Լӹ��������Դ��㵵��ΪFALSE,���㵵ΪTRUE
WORK_EXT 	BOOL		g_bDispRunLine=FALSE;
WORK_EXT 	BOOL		g_bIsHand=FALSE;	
WORK_EXT 	BOOL		g_bStepToSingle=FALSE;
WORK_EXT 	BOOL		g_bMode=FALSE;	   
WORK_EXT	BOOL		g_bEMSTOP=FALSE; 
WORK_EXT	INT16S		g_iADValue=-2;  
WORK_EXT	BOOL		g_bTestStart=TRUE;
WORK_EXT	BOOL		g_bHandRun = FALSE;									//�ڸ�ƽ̨�ò�����ʾ�������ּӹ�������ҡֹͣ��ΪFALSE,��ҡʱΪTRUE
WORK_EXT	INT32U		g_lHandRun=0;
WORK_EXT	FP32 		g_fSpeedBi=0.1;
WORK_EXT	FP32		g_lSpeed=10;
WORK_EXT	BOOL		g_bHandMove=FALSE;
WORK_EXT	BOOL		g_bHandset=FALSE;
WORK_EXT	FP32 		g_lWidthPos=0;
WORK_EXT	BOOL		g_bSequeBack=FALSE;
WORK_EXT	BOOL		g_bGotoStart[MAXCH]={FALSE,FALSE,FALSE,FALSE};
WORK_EXT	INT16S      g_iOutStatus=0;
WORK_EXT	BOOL		g_bPressStartIO=FALSE;
WORK_EXT	BOOL		g_bPressStartIO2=FALSE;
WORK_EXT	BOOL		g_bPressStepIO2=FALSE;
WORK_EXT	BOOL		g_bBackZero=FALSE;										//ָʾ�Ƿ�Ϊ�������
WORK_EXT	INT16S		g_iErrorNo=0; 
WORK_EXT	BOOL		g_bCylinder=FALSE; 										//ָʾ�Ƿ������ײ���״̬
WORK_EXT	BOOL		g_bScrProtectRun=FALSE;
WORK_EXT 	INT16S		g_iWorkNumber 	= 1;
WORK_EXT 	INT16S		g_iSpeedBeilv 	= 1;
WORK_EXT 	FP32		g_fStepSpeed[4]={0.1,0.5,1.0,1.0};
WORK_EXT 	INT32S  	g_lXmsSize=0;      
WORK_EXT	INT32S		g_lDataCopyXmsSize=0;
WORK_EXT 	INT32S		g_lYCountLen=0;       									//�ӹ�������Y��Ĳ�����
WORK_EXT 	INT32S		g_lZCountLen=0;      									//�ӹ�������Y��Ĳ�����
WORK_EXT	FP32 		g_fCloseDispCount=0.0;
WORK_EXT	INT32S		g_ulTiCount=0;
WORK_EXT	INT16U		g_uiSuspendTask=0xFF1E;									//suspend 1~5 task,high byte rev
//WORK_EXT	INT32S		g_lAxisValue[CHMAX]={0,0,0,0};							//��¼����ض�֮ǰ��λ��
WORK_EXT 	INT32S		g_lProbeFail=0; 										//��¼̽��ʧ��   
WORK_EXT	INT16S		g_iCheckStatusIO=0;
WORK_EXT 	INT32S  	g_lRamRunCounts=0;										//���籣�浱ǰ����  

//20130815
WORK_EXT	BOOL		g_bTestToSingle=FALSE;
WORK_EXT    BOOL 		g_bWheel    = FALSE;									//���ְ����ӹ�
WORK_EXT    BOOL      	g_bStepMode = FALSE;									//����ģʽ
WORK_EXT	BOOL		g_bFunc 	= FALSE;									//����
WORK_EXT	INT16S		g_iTCmdPos 	= 0;										//�趨����̽��
WORK_EXT	INT32S		g_lInputPos[4]={0,0,0,0};
WORK_EXT	BOOL		g_bUseTimes = FALSE;
WORK_EXT	INT16S		g_iStatus=0;
WORK_EXT    INT16S     g_iToolBarPage = 0;
WORK_EXT	INT16U 		g_uiMoveKey=0;
//WORK_EXT    BOOL        g_bScreenSaved=FALSE;
extern		INT32S 		g_bInitbmp;

OUTDATA 	OutData[100];									//�����������
INT32S  	g_lOutSize;										//��¼�����׵Ĵ���
INT32S		lOutFlag[100]={0};								//��¼��ױ��򿪹���ʶ


WORK_EXT	BOOL		g_bWheelStatus=FALSE;

extern 		int			Suspended;
int						sdq = 0;
UNITDATA        		Rundata[MAXLINE];
WORK_EXT 	BOOL		g_bModify1=FALSE;
INT32S      g_iCheckXian = 0;
BOOL        g_bCheckDiffer= FALSE;
INT32S      g_iCheckTime = 0;
INT32S      g_iCheckNum  = 0;
INT32S      g_iLayNum    = 0;
INT32S      g_iLayNum2   = 0;
BOOL        g_bCheckLay  = FALSE;
extern		BOOL		g_bLCD;
INT32S  	Hour11=-1;
INT32S		Minute11=-1;
INT32S		Hour12=-1;
INT32S		Minute12=-1;


INT32S 			lYProbelong =0;						//��̽���Ĳ�ֵ 
INT32S 			lYProbelong1 =0;					//�ܵĴ�̽���Ĳ�ֵ

INT32S			g_lRunLintCount = 0;

int 		lTagCount = 0;
int 		lTagLastCount=0;						//���浯����������
extern 	BOOL  g_bInterface;

TEMPOUTDATA   	OutDataAdd[20]={0};					//��¼һ�е����ײ������������������
INT32S  		lOutSizeAdd=0;						//��¼�����׵Ĵ���
INT32S			g_lProbeAngle = -888888;			//����̽����ʾ�Ƕ�

BOOL			g_bLagCheck = TRUE;				    //�Ƿ����ͺ�����⹦�ܣ���̽���йر��ͺ������
extern		INT16S		g_iCurEditNo;

extern      INT32S      g_iEmpNode[MAXCH];	
extern      BOOL 		g_bBackZeroProcess;
BOOL		g_bDuxianIO = FALSE;					//�����źż���־λ
INT32S		nLastDuxCount=0;						//���߼���

//INT32S			g_lYplusLong = 0;					//�Ƿ�̽��ʧ�ܣ�̽��ʧ�ܺ��¼Jָ���е���ת��֮�������н�����Y�����ܵ�����
//INT16S			g_itemplNode = 0;					//��¼̽��ʧ�ܺ���ת�����е�

INT32S SrotTempOutData(INT32S dir,INT32S axis)			
{
	INT32S  i=0,k=0;
	TEMPOUTDATA Temp;

	if(dir == 0)					//��С��������
	{
		for(i=0;i<lOutSizeAdd-1;i++)
		{
			for(k=0;k<lOutSizeAdd-1;k++)
			{
				if(OutDataAdd[k].outPos[axis]>OutDataAdd[k+1].outPos[axis])	//���ķź���
				{
					memcpy(&Temp,&OutDataAdd[k],sizeof(Temp));

					memcpy(&OutDataAdd[k],&OutDataAdd[k+1],sizeof(Temp));

					memcpy(&OutDataAdd[k+1],&Temp,sizeof(Temp));
				}
			}
		}
	}
	else 							//�Ӵ�С
	{
		for(i=0;i<lOutSizeAdd-1;i++)
		{
			for(k=0;k<lOutSizeAdd-1;k++)
			{
				if(OutDataAdd[k].outPos[axis]<OutDataAdd[k+1].outPos[axis])	//��С�ķź���
				{
					memcpy(&Temp,&OutDataAdd[k],sizeof(Temp));

					memcpy(&OutDataAdd[k],&OutDataAdd[k+1],sizeof(Temp));

					memcpy(&OutDataAdd[k+1],&Temp,sizeof(Temp));
				}
			}
		}

	}

	return 1;
}


INT32S GetYLong(INT32S index)						//���ڲ��Ҳ�����������֮ǰ�Ĳ�ͬ��Yֵ������Y������ʾ
{
	UNITDATA 	pData;
	INT16S 		i;

	INT32S 		lYTemp;
	INT32S     lYTempIo=0;

	FP32 		VAR=0;


	if(index>0)
	{
		for(i=1;i<g_lDataCopyXmsSize;i++)
		{
			XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)i*sizeof(UNITDATA),sizeof(UNITDATA));

			if(pData.nLine == index)
			{ 
				lYTemp = pData.lValue[1];
				lYTempIo = pData.nIO;

				//Uart_Printf("1=%d 2=%d #=%d\n",pData.nLine,index,i);

				if(i-1>0)
				{
					XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)(i-1)*sizeof(UNITDATA),sizeof(UNITDATA));

					if(lYTemp != pData.lValue[1] && lYTempIo==0)				//������ǰ���Yֵ��ǰһ�����Yֵ��һ��ʱ�����̽���Ĳ�ֵ
					{
						lYProbelong = 0;
					}
				}

				for(;i>0;i--)													
				{
					XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)i*sizeof(UNITDATA),sizeof(UNITDATA));

					if(lYTemp !=  pData.lValue[1])
					{
						//Uart_Printf("3=%d 4=%d @=%d\n",lYTemp,pData.lValue[1],i);
						VAR= GetMMFromPulse(2,pData.lValue[1]*100);
						goto RTN;
					}
				}

				goto RTN;
			}

		}
	}
	RTN:

	/*{
long p1;
BaseGetCommandPos(2,&p1);
	Uart_Printf("index=%ld, VAR=%ld,p1=%ld\n",index,(INT32S)VAR,p1);
}*/
	return VAR;
}




void	DisplayXYZ( BOOL cur)
{
	INT8S   buffer[0x10]="\0";

	static  INT32S curPos[6]={0,0,0,0,0,0};
	static	INT32S LastcurPos[6]={-1,-1,-1,-1,-1,-1};


	static	INT32S UQuanPos = 0;
	static  INT32S UAnglePos = 0;
	static	INT32S ZQuanPos = 0;
	static  INT32S ZAnglePos=0;
	static	INT32S VQuanPos = 0;
	static  INT32S VAnglePos = 0;
	static	INT32S WQuanPos = 0;
	static  INT32S WAnglePos = 0;

	if(cur)
	{	
		if( g_Ini.iAxisSwitch[0]==1 )
		{
			memset(buffer,NULL,sizeof(buffer));

			curPos[0] = (INT32S)(GetAngle(1,cur)*10);

			//	ltoa(curPos[0]%3600,(INT8U *)buffer);
			if(g_Ini.iAxisCa==0)
				sprintf(buffer,"%0.1f",curPos[0]/10.0);

			else
				sprintf(buffer,"%d",curPos[0]);

			/*if(strlen(buffer)<8)
			{
				for(i=strlen(buffer);i<9;i++)
					buffer[i]=' ';
			}*/

			if(LastcurPos[0] != curPos[0])
			{

				CDC_OutTextLR_UTF(75,getmaxy()-90,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

				LastcurPos[0] = curPos[0];
			}

		}


		/*
		if( g_Ini.iAxisSwitch[1]==1 )
		{		
			INT32S	AllYsendLong;

			static long s_lPrePos=0;
			long		lNowPos =0;

			get_command_pos(2,&lNowPos);

			memset(buffer,NULL,sizeof(buffer));

			curPos[1] = (INT32S)(GetMM(2,cur)*100);

			AllYsendLong=curPos[1];

			curPos[1] = curPos[1] -  GetYLong(vm_getindex()) - GetMMFromPulse(2,lYProbelong*100);


			//�� g_iWorkStatus!=RUNNING ����£��������������ʾ			
			//������g_iWorkStatus==RUNNING ����£���(lNowPos != s_lPrePos || curPos[1] == 0)����£��������������ʾ
			if(lNowPos != s_lPrePos || curPos[1] == 0 || g_iWorkStatus!=RUNNING)						
			{

				//�˴�����ֻ��Լӹ�״̬�£���Ϊ�˱���������ʾ���ִ���һ��������������ֵ������ʾΪ0
				if((curPos[1]<0 && lNowPos>s_lPrePos || curPos[1]>0 && lNowPos<s_lPrePos) && g_iWorkStatus==RUNNING)
				{
					curPos[1] =0;
				}


				s_lPrePos =lNowPos;


			//	ltoa(curPos[1],(INT8U *)buffer);

				if(g_Ini.iAxisCa==0)
					sprintf(buffer,"%0.2f",curPos[1]/100.0);

				else
					sprintf(buffer,"%d",curPos[1]);


				if(LastcurPos[1] != curPos[1])
				{
					CDC_OutTextLR_UTF(222,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

					LastcurPos[1] = curPos[1];
				}
			}
		}*/

		if( g_Ini.iAxisSwitch[1]==1 )
		{
			memset(buffer,NULL,sizeof(buffer));
			if(g_Ini.iAxisRunMode[1] == 0)
			{
				curPos[1] = (INT32S)(GetAngle(2,cur)*10);

				if(g_Ini.iAxisCa==0)
					sprintf(buffer,"%0.1f",curPos[1]/10.0);

				else
					sprintf(buffer,"%d",curPos[1]);

			}
			else if(g_Ini.iAxisRunMode[1] == 1)
			{

				curPos[1] = (INT32S)(GetMM(2,cur)*10);//ȡ��ϵͳ����

				ZQuanPos = curPos[1]/3600;
				ZAnglePos = curPos[1]%3600;


				if(g_Ini.iAxisCa==0)
				{
					if(curPos[1]>=0)
						sprintf(buffer,"%dC%0.1f",ZQuanPos,ZAnglePos/10.);
					else
						sprintf(buffer,"-%dC%0.1f",abs(ZQuanPos),abs(ZAnglePos)/10.0);
				}
				else
				{
					if(curPos[1]>=0)
						sprintf(buffer,"%dC%04d",ZQuanPos,ZAnglePos);
					else
						sprintf(buffer,"-%dC%04d",abs(ZQuanPos),abs(ZAnglePos));
				}	
				//Uart_Printf("ZQuanPos = %d  ZAnglePos = %04d\n",ZQuanPos,ZAnglePos);
			}
			else  if(g_Ini.iAxisRunMode[1] == 2)
			{
				curPos[1] = (INT32S)(GetMM(2,cur)*100);
				if(g_Ini.iAxisCa==0)
					sprintf(buffer,"%0.2f",curPos[1]/100.0);
				else
					sprintf(buffer,"%d",curPos[1]);
			}
			else if(g_Ini.iAxisRunMode[1] ==3)		//����
			{
				Uart_Printf("����...\n");	
				curPos[1] = (INT32S)(GetMM(2,cur)*100);
				curPos[1] = (curPos[1] -  GetYLong(vm_getindex()) - GetMMFromPulse(2,lYProbelong*100));
				ftoa(curPos[1]/100.0,(INT8U *)buffer,2,0);
			}

			if(LastcurPos[1] != curPos[1])
			{	
				CDC_OutTextLR_UTF(75+195,getmaxy()-90,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
				LastcurPos[1] = curPos[1];
			}
		}


		if(g_Ini.iAxisCount>2)
		{
			if( g_Ini.iAxisSwitch[2]==1 )
			{
				memset(buffer,NULL,sizeof(buffer));
				if(g_Ini.iAxisRunMode[2] == 0)
				{
					curPos[2] = (INT32S)(GetAngle(3,cur)*10);

					//ltoa(curPos[2], (INT8U *)buffer);

					if(g_Ini.iAxisCa==0)
						sprintf(buffer,"%0.1f",curPos[2]/10.0);

					else
						sprintf(buffer,"%d",curPos[2]);

				}
				else if(g_Ini.iAxisRunMode[2] == 1)
				{
					curPos[2] = (INT32S)(GetMM(3,cur)*10);

					ZQuanPos = curPos[2]/3600;
					ZAnglePos = curPos[2]%3600;

					if(g_Ini.iAxisCa==0)
					{
						if(curPos[2]>=0)
							sprintf(buffer,"%dC%0.1f",ZQuanPos,ZAnglePos/10.);
						else
							sprintf(buffer,"-%dC%0.1f",abs(ZQuanPos),abs(ZAnglePos)/10.);
					}	
					else
					{
						if(curPos[2]>=0)
							sprintf(buffer,"%dC%04d",ZQuanPos,ZAnglePos);
						else
							sprintf(buffer,"-%dC%04d",abs(ZQuanPos),abs(ZAnglePos));
					}

				}
				else  if(g_Ini.iAxisRunMode[2] == 2)
				{
					curPos[2] = (INT32S)(GetMM(3,cur)*100);

					//	ltoa(curPos[2], (INT8U *)buffer);

					if(g_Ini.iAxisCa==0)
						sprintf(buffer,"%0.2f",curPos[2]/100.0);

					else
						sprintf(buffer,"%d",curPos[2]);

				}

				/*if(strlen(buffer)<13)
				{
					for(i=strlen(buffer);i<13;i++)
						buffer[i]=' ';
				}*/

				if(LastcurPos[2] != curPos[2])
				{	
					CDC_OutTextLR_UTF(75+195*2,getmaxy()-90,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

					LastcurPos[2] = curPos[2];
				}
			}
		}
		if(g_Ini.iAxisCount>3)
		{
			if( g_Ini.iAxisSwitch[3]==1 )
			{
				memset(buffer,NULL,sizeof(buffer));

				if(g_Ini.iAxisRunMode[3] == 0)
				{
					curPos[3] = (INT32S)(GetAngle(4,cur)*10);
					//	ltoa(curPos[3], (INT8U *)buffer);
					if(g_Ini.iAxisCa==0)
						sprintf(buffer,"%0.1f",curPos[3]/10.0);
					else
						sprintf(buffer,"%d",curPos[3]);
				}
				else if(g_Ini.iAxisRunMode[3] == 1)
				{
					curPos[3] = (INT32S)(GetMM(4,cur)*10);

					UQuanPos = curPos[3]/3600;
					UAnglePos = curPos[3]%3600;

					if(g_Ini.iAxisCa==0)
					{
						if(curPos[3]>=0)
							sprintf(buffer,"%dC%0.1f",UQuanPos,UAnglePos/10.);
						else
							sprintf(buffer,"-%dC%0.1f",abs(UQuanPos),abs(UAnglePos)/10.);
					}	
					else
					{
						if(curPos[3]>=0)
							sprintf(buffer,"%dC%04d",UQuanPos,UAnglePos);
						else
							sprintf(buffer,"-%dC%04d",abs(UQuanPos),abs(UAnglePos));
					}	

				}
				else if(g_Ini.iAxisRunMode[3] == 2)
				{
					curPos[3] = (INT32S)(GetMM(4,cur)*100);

					//	ltoa(curPos[3], (INT8U *)buffer);

					if(g_Ini.iAxisCa==0)
						sprintf(buffer,"%0.2f",curPos[3]/100.0);

					else
						sprintf(buffer,"%d",curPos[3]);
				}


				if(LastcurPos[3] != curPos[3])
				{	
					CDC_OutTextLR_UTF(75+195*3,getmaxy()-90,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

					LastcurPos[3] = curPos[3];
				}


			}

		}


		if(g_Ini.iAxisCount>4)
		{
			if( g_Ini.iAxisSwitch[4]==1 )
			{
				memset(buffer,NULL,sizeof(buffer));

				if(g_Ini.iAxisRunMode[4] == 0)
				{
					curPos[4] = (INT32S)(GetAngle(5,cur)*10);
					ltoa(curPos[4], (INT8U *)buffer);
				}
				else if(g_Ini.iAxisRunMode[4] == 1)
				{
					curPos[4] = (INT32S)(GetMM(5,cur)*10);

					VQuanPos = curPos[4]/3600;
					VAnglePos = curPos[4]%3600;

					if(curPos[4]>=0)
						sprintf(buffer,"%dC%04d",VQuanPos,VAnglePos);
					else
						sprintf(buffer,"-%dC%04d",abs(VQuanPos),abs(VAnglePos));
				}
				else  if(g_Ini.iAxisRunMode[4] == 2)
				{
					curPos[4] = (INT32S)(GetMM(5,cur)*100);

					ltoa(curPos[4], (INT8U *)buffer);
				}

				//ltoa(curPos[3], (INT8U *)buffer);

				/*if(strlen(buffer)<13)
				{
					for(i=strlen(buffer);i<13;i++)
						buffer[i]=' ';
				}*/

				if(LastcurPos[4] != curPos[4]) 
				{	
					CDC_OutTextLR_UTF(560,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

					LastcurPos[4] = curPos[4];
				}

			}

		}

		if(g_Ini.iAxisCount>5)
		{
			if( g_Ini.iAxisSwitch[5]==1 )
			{
				memset(buffer,NULL,sizeof(buffer));

				if(g_Ini.iAxisRunMode[5] == 0)
				{
					curPos[5] = (INT32S)(GetAngle(6,cur)*10);
					ltoa(curPos[5], (INT8U *)buffer);
				}
				else if(g_Ini.iAxisRunMode[5] == 1)
				{
					curPos[5] = (INT32S)(GetMM(6,cur)*10);

					WQuanPos = curPos[5]/3600;
					WAnglePos = curPos[5]%3600;

					if(curPos[5]>=0)
						sprintf(buffer,"%dC%04d",WQuanPos,WAnglePos);
					else
						sprintf(buffer,"-%dC%04d",abs(WQuanPos),abs(WAnglePos));
				}
				else  if(g_Ini.iAxisRunMode[5] == 2)
				{
					curPos[5] = (INT32S)(GetMM(6,cur)*100);

					ltoa(curPos[5], (INT8U *)buffer);
				}
				//ltoa(curPos[3], (INT8U *)buffer);

				/*if(strlen(buffer)<13)
				{
					for(i=strlen(buffer);i<13;i++)
						buffer[i]=' ';
				}*/

				if(LastcurPos[5] != curPos[5])
				{	
					CDC_OutTextLR_UTF(680,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);

					LastcurPos[5] = curPos[5];
				}

			}

		}

		/*
		{															//���걣�浽����
			static 	INT32S sTempPos[MAXCH]={0,0,0,0,0,0};
			INT32S	cmdpos;  
			INT16S	i=0;	

			for(i=0;i<g_Ini.iAxisCount;i++)
			{
				if(i==1)											//�����᲻����
					continue;

				cmdpos = GetPulse(i+1,TRUE);

				if(sTempPos[i]!= cmdpos)
				{
					sTempPos[i] = cmdpos;
					WritePosToRam(i+1, sTempPos[i]);
				}
			}
		}*/

	}
	else
	{
		INT16S	i=0;	

		for(i=0;i<g_Ini.iAxisCount;i++)
		{
			LastcurPos[i]=-1;

			if( g_Ini.iAxisSwitch[i]==0 )	
				CDC_OutTextLR_UTF(75+i*195,getmaxy()-90," OFF",REDCOLOR,g_nTechfrmBk,MZK_24,8,1);
		}
	}
}



INT16S	RunAllData(void)
{
	INT32S 		lNode=0;
	INT16S 		l=0,k=0;
	INT32U		ti1,ti2;
	INT32S 		x=0,y=0,z=0,a=0,movespeed;
	INT32S 		b=0,c=0;
	UNITDATA 	pData,pDataTmp;
	INT16S		ProbeCount=0;
	BOOL	 	WorkEnd=FALSE;

	BOOL  		bProbeFirstRun =  TRUE; 								//�жϼӹ���һ������̽�뵯��ʱ��������ʱ����ֱ������AUTORUNEND
	INT16S		iHaveTag = 0;											//���ڴ���bProbeFirstRun��������չ��
	INT32S 		itempRunCount =g_Ini.lRunCount;


	INT16U		nLastLine=0;
	INT8U		nLastOut[MAXCYLINDER]={2,2,2,2,2,2,2,2};
	INT32S		lCurPulse[MAXCH] ;	
	BOOL		openpro ;//��¼��ǰ�̵������Ƿ���̽������Ϊ1

	SetQuiteRange(8000);


	////*********����ǰ�ȼ���Ƿ������  �ͱ���������*****************////
	if(g_bRelease)
	{
		if(g_bEMSTOP)
		{
			OSQPost(MsgQueue,g_bEnglish?"Start is invalid after emergency stop":"����ֹͣ�����ȹ���!");
			return 1;
		}

		if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"����ͣ��״̬!��̧��ͣ��ť!");
			return 1;
		}


		if( !g_bFirstResetX  )
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Reset X!!!":"X�ȹ���!!!");
			return 1;
		}

		switch(g_Ini.iAxisCount)
		{
		case 6:
			if(g_Ini.iAxisSwitch[5]==1)
			{
				if( !g_bFirstResetW  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset C!!!":"C���ȹ��� ...!!!");
					return 1;
				}
			}

		case 5:
			if(g_Ini.iAxisSwitch[4]==1)
			{
				if( !g_bFirstResetV  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset B!!!":"B���ȹ��� ...!!!");
					return 1;
				}
			}

		case 4:
			if(g_Ini.iAxisSwitch[3]==1)
			{
				if( !g_bFirstResetU  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset A!!!":"A�ȹ��� ...!!!");
					return 1;
				}
			}

		case 3:
			if(g_Ini.iAxisSwitch[2]==1)
			{
				if( !g_bFirstResetZ  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset Z!!!":"Z�ȹ��� ...!!!");
					return 1;
				}
			}

		case 2:
			if(g_Ini.iAxisSwitch[1]==1)
			{
				if( !g_bFirstResetY  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset Y!!!":"Y�ȹ��� ...!!!");
					return 1;
				}
			}

		default:
			break;
		}


		/*
		if(CheckWarning() || CheckLimit(TRUE,TRUE,0,0))
		{
			if(g_Ini.iWarningOut==1)
			{
				if(GetOut(O_WARNING)==0)
					WriteBit(O_WARNING,OPEN);
			}

		}
		else
		{
			if(g_Ini.iWarningOut==1)
			{
				WriteBit(O_WARNING,CLOSE);
			}
		}*/

		/*
		if(g_Ini.iAxisSwitch[1]==1)								//����λ������
		{
			if(!g_bFirstResetY)
			{
				SetMM(2,0,TRUE);
				g_bFirstResetY=TRUE;
				g_lYCountLen=0;
			}
		}*/
	}

	if( !g_bUnpackCorrect )
	{
		OSQPost(MsgQueue,g_bEnglish?"Data Invalid!":"��ǰ��������д���!");
		return 1;
	}

	Start(1);Start(2);

	switch(g_Ini.iAxisCount)
	{
	case 6:Start(6);
	case 5:Start(5);
	case 4:Start(4);
	case 3:Start(3);
	break;
	}

	UpdateSpeedCountProbe();									//ˢ��̽��

	{															//����ǰ���Թ����еԭ�㣬�����ڲ������������

		INT16S i=0,Number=0;


		for(Number=1;Number<=MAXCH;Number++)//֧�ֵ���˳��������13��������
		{
			for(i=0;i<g_Ini.iAxisCount;i++)
			{
				if((g_Ini.iBackOrder[i] == Number) && (g_Ini.iAxisSwitch[i]==1) )
				{
					if(g_Ini.iBackMode[i]!=4  || g_Ini.iAxisRunMode[i]==1)
					{
						INT32S lTemp = 0;
						lTemp = GetPulse(i+1,TRUE);

						if(lTemp!=0)
						{
							if(!WorkFindZero(i+1))
								return 1;
						}
					}
				}
			}

			OSTimeDly(10);
		}



		/*


		if(g_Ini.iBackMode[0]!=4)	//��Ҫ����
		{
			if(GetPulse(1,TRUE)!=0)
			{
				if(!WorkFindZero(1))
					return 1;
			}
		}

		OSTimeDly(10);

		if(g_Ini.iAxisSwitch[1]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(2,TRUE);

			if(g_Ini.iBackMode[1]!=4  || g_Ini.iAxisRunMode[1]==1)	//��Ȧ�ᶼҪ�Ȼ���,��ʹ�������óɲ�����
			{
				if(lTemp!=0)
				{
					if(!WorkFindZero(2))
						return 1;
				}
			}

			OSTimeDly(10);
		}


		if(g_Ini.iAxisCount>2 && g_Ini.iAxisSwitch[2]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(3,TRUE);

			if(g_Ini.iBackMode[2]!=4  || g_Ini.iAxisRunMode[2]==1)	//��Ȧ�ᶼҪ�Ȼ���,��ʹ�������óɲ�����
			{
				if(lTemp!=0)
				{
					if(!WorkFindZero(3))
						return 1;
				}
			}

			OSTimeDly(10);
		}		

		if(g_Ini.iAxisCount>3 && g_Ini.iAxisSwitch[3]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(4,TRUE);

			if(g_Ini.iBackMode[3]!=4 || g_Ini.iAxisRunMode[3]==1)	
			{
				if(lTemp!=0)
				{
					if(!WorkFindZero(4))
						return 1;
				}
			}

			OSTimeDly(10);
		}	*/	

	}	




	g_lRunLintCount = g_Ini.lRunCount;

	if(g_bStepIO)
	{
		CTeach_UpdateStatus(STATUS_TEST);
		OSQPost(MsgQueue,g_bEnglish?"Start TestRun...":"��ʼ��������...");
		LedOut(TESTLED,1);
	}
	else if(g_bHandRun)
	{
		CTeach_UpdateStatus(STATUS_TEST);
		OSQPost(MsgQueue,g_bEnglish?"Start Handwheel Run...":"��ʼ��ҡ����...");
	}
	else
	{
		CTeach_UpdateStatus(STATUS_AUTO);
		//CTeach_GotoFristline();
		OSQPost(MsgQueue,g_bEnglish?"Start AutoRun...":"��ʼ�Զ��ӹ�...");
	}

	//	OSTimeDly(100);

	//	if(g_bAutoRunMode&&!g_bMode)
	{
		if(g_Ini.iDispLeavingTime==1&&g_Ini.lDestCount>0)
		{
			//	CDC_SaveScreen();
			//	g_bScreenSaved=TRUE;
			//	g_bFirstDispRunTime=TRUE;
			g_bDispRunTime=FALSE;								//����ʱ��ʼ����δ��ʾʣ��ʱ��

			UpdateLeavingTime(TRUE,FALSE,TRUE);
		}
	}

	g_iWorkStatus=RUNNING;
	g_iErrorNo = 0;

	{
		INT16U	i = 0;

		for(i=0; i<MAXCH; i++)
		{
			SetCommandPos(i+1,0);
			SetActualPos(i+1,0);
		}

	}

	{															//���������������
		INT8S a = 0;

		for(a=0; a<5; a++)
			ResumeTasks(a);

		OSTimeDly(50);
	}

	LedOut(STOPLED,0);
	RunStatusOutCtr(2);


	//	if(g_bAutoRunMode && !g_bMode)								//���Զ��ӹ�ģʽ��
	CloseDisplay(0);

	if(!g_bMode)
		if(g_Ini.iAxisSwitch[1]==1)
			WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);	


	g_iMoveAxis = g_Ini.iAxisCount*100; 						//zhi shi wei inp

	vm_sync();													//ͬ������ջ���

	g_lProbeAngle = -888888;									//�Ƕȳ�ʼ��

	ti1 = OSTimeGet();

	for(k=0;k<MAXCH;k++)
		lCurPulse[k] =GetPulse(k+1,TRUE);

	do
	{
		//*******���ݿ������ӹ�����********//
		if(!g_bDataCopy && g_lStepNode==0)
		{
			if(!DataCopy())
			{
				OSQPost(MsgQueue,g_bEnglish?"Data analysis error!":"���ݿ����������,��ȷ�����������ȷ����!");
				g_iMoveAxis = 0;
				//return 0;
				goto   WHILEEND;
			}	
		}
		openpro = FALSE;
		lNode = 0;
		WorkEnd = FALSE;
		ProbeCount = 0;

		if(g_bStepMode==TRUE)									//����ģʽ
		{
			g_bAutoRunMode = FALSE;
		}
		for(; lNode<g_lDataCopyXmsSize;)						//�ӹ�һ�����ɵ����ݷ���
		{
			XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA));
			lNode++;			

			switch(pData.nCode)
			{
			case SPEED:										//�ٶ�ָ��
			{
				/*
						XMSToMEMEx((INT8U   *)&pData,g_uXMS,lNode*sizeof(UNITDATA),sizeof(UNITDATA));
						lNode ++; 

						if(pData.nCode!=JUMP)
							lNode--;

						if(g_bStepIO)
							g_lStepNode = lNode;
				 */
				continue;
			}
			break;

			case MOVE:
			{
				/*
						if( pData.nIO) 												//���н��͵�̽�����ڲ岹ǰ��̽��򿪷��뻺�������
						{
							g_iCheckStatusIO =  pData.nIO;
							vm_set_probe(g_iCheckStatusIO,2);
						}*/

				{

					INT8U	AxisEnable =0;					//���ڱ�ʶ�岹�Ŀ�����

					AxisEnable = 1|(g_Ini.iAxisSwitch[1]<<1) |(g_Ini.iAxisSwitch[2]<<2) |(g_Ini.iAxisSwitch[3]<<3)|(g_Ini.iAxisSwitch[4]<<4)|(g_Ini.iAxisSwitch[5]<<5);

					if(g_Ini.iAxisCount<3)					//g_Ini.iAxisCount==2
					{
						AxisEnable = AxisEnable & 0x03;
					}
					else if(g_Ini.iAxisCount<4)				//g_Ini.iAxisCount==3
					{
						AxisEnable = AxisEnable & 0x07;
					}
					else if(g_Ini.iAxisCount<5)				//g_Ini.iAxisCount==4
					{
						AxisEnable = AxisEnable & 0x0f;
					}
					else if(g_Ini.iAxisCount<6)				//g_Ini.iAxisCount==5
					{
						AxisEnable = AxisEnable & 0x1f;
					}

					//�岹����
					x = pData.lValue[0];
					y = pData.lValue[1];
					z = pData.lValue[2];
					a = pData.lValue[3];
					b = pData.lValue[4];
					c = pData.lValue[5];



					for(k=0;k<MAXCH;k++)   //������ģʽ�У��������ݵĿ���֮ǰ�����ּӹ�����ʱ����λ�ã�
					{
						if(g_Ini.iBackMode[k]==4 && lNode-1<=g_iEmpNode[k])
						{
							INT32S value=0;
							value = lCurPulse[k];

							switch(k)
							{	case 0:
								x=value;
								break;
							case 2:
								z=value;
								break;
							case 3:
								a=value;
								break;
							case 4:
								b=value;
								break;
							case 5:
								c=value;
								break;
							}
						}

					}

					movespeed = pData.lValue[6];


					if(movespeed>VM.SpdMax[pData.SpindleAxis-1]*1000)		//�������ٹ���xxl20141127
						movespeed=VM.SpdMax[pData.SpindleAxis-1]*1000;

					if(movespeed<1)
						movespeed=1;

					/*
							{														//�����ڽ��ʹ�����������Ȼ�ٶȺ��������ִ���
								if(g_Ini.iAxisSwitch[1]==0)							//���������ߵ��Ǿ���λ������Ϊ���ʾ�ߵ������ʱ�������ʱ���ӹ�ʱ���ߵ�������������
									y = 0;

								if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
									z = 0;

								if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
									a = 0;
							}
					 */
					//Uart_Printf("test\n");
					//һ���Է���෢�������� ����м���̽�����׵ȴ�,��ȴ�����ڼ�������
					if(vm_abs_inp_move(pData.nLine,AxisEnable,pData.SpindleAxis-1,x,y,z,a,b,c,movespeed) == -1)
					{
						Uart_Printf("vm_abs_inp_move error\n");
						goto WHILEEND;
					}


					for(l=0;l<MAXCYLINDER;l++)  							//���׶���  �ڿ�ʼ�ǵ�������֮ǰִ��
					{
						if(2 !=pData.nOut[l])
						{
							vm_set_io(l,pData.nOut[l]);
						}
					}

					if(pData.nReset)										//�������־
					{
						if(pData.nReset & 0x01)
						{
							vm_set_position(Ax,0);
						}

						if(pData.nReset & 0x02)
						{
							vm_set_position(Ay,0);
						}

						if(pData.nReset & 0x04)
						{
							vm_set_position(Az,0);
						}

						if(pData.nReset & 0x08)
						{
							vm_set_position(Aa,0);
						}

						if(pData.nReset & 0x10)
						{
							vm_set_position(Ab,0);
						}

						if(pData.nReset & 0x20)
						{
							vm_set_position(Ac,0);
						}
					}
				}


#if FALSE
				if( pData.nIO>0&&pData.nIO<2) 												//�����͵�̽����ʱֹͣ���ͻ�������ݣ����ȴ�̽���ź�
				{
					BOOL 	bFirstProbe = TRUE;
					openpro = TRUE;
					g_iCheckStatusIO = pData.nIO;

					vm_start();

					while(TRUE)
					{
						if(pData.nLine-1 == vm_getindex())
						{
							g_bLagCheck = FALSE;
						}

						if(pData.nLine == vm_getindex())				//�ȴ�ִ�л��浽̽���С�ע��̽���е��к��뿪ʼ���к�һ�¡�
						{
							lYProbelong = 0;							//ÿ�ν�̽����ʱҪ��ǰһ̽���ƫ������
							g_lProbeAngle = -888888.0;					//�Ƕȳ�ʼ��

							if(bFirstProbe)								//��̽��,���״μ��̽��״̬�����ź�̽�����
							{
								INT16S flag = 0;

								bFirstProbe = FALSE;

								switch(g_iCheckStatusIO)
								{
								case 1:
								{
									RealIO(TTANZHEN6,0,2);
									//vm_set_probe(1,1);						//������Ϊ1��������
								}
								break;

								default:
									break;
								}

								switch(g_iCheckStatusIO)
								{
								case 1:
								{
									flag = RealIO(TTANZHEN6,0,2);

									if(flag)
										g_lProbeAngle = -188888.0;//̽��1ǰ����������ʾ

								}
								break;

								default:
									break;
								}

								if(flag ==1)							//��ʾ������̽���Դ򿪣����״ξͼ�⵽̽���ź�
								{
									g_lProbeFail++;						//̽��ǰ����ʧ��

									vm_stop(1);							//���建���ֹͣ����
									lYProbelong = pData.lValue[1] - GetPulse(2,TRUE);
									lYProbelong1+=lYProbelong;
									SetPulse(2,pData.lValue[1],TRUE);

									vm_sync();							//ͬ������ջ���

									//CloseProbe(0);

									if(g_Ini.lProbeCount ==1)
									{
										g_iErrorNo=15;					//��̽��һֱ����ʧ��
										OSTimeDly(30);
										SetQuiteRange(0);
										WriteBit(O_SENDADD,0);

										ExitScrProtect();

										CTeach_UpdateStatus(STATUS_PAUSE);

										OSQPost(MsgQueue,g_bEnglish?"Probe keep touching!":"̽��һֱ����,��\"����\"��\"����\"����");

										while(TRUE)						//�˴������2.3.4.17�⼸������������ᴦ��whileѭ����
										{
											if(g_iErrorNo != 0 && g_iErrorNo!=15 && g_iErrorNo!=3 && g_iErrorNo != 2 && g_iErrorNo != 4)
											{
												Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
												goto   WHILEEND;
											}
											if(g_iErrorNo == 0)
												break;

											OSTimeDly(1);
										}
									}

									if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
									{
										g_bAutoRunMode=FALSE;
									}

									break;								//�˳�����ִ����һ��
								}
							}
							else										//��⵽̽���ź�
							{

								INT16S flag = 0;

								switch(g_iCheckStatusIO)
								{
								case 1:
									flag = RealIO(TTANZHEN6,0,2);;

								default:
									break;
								}

								if(flag ==1 )							//̽��ɹ�
								{
									vm_stop(1);							//���建���ֹͣ����
									lYProbelong = pData.lValue[1] - GetPulse(2,TRUE);
									Uart_Printf("g_lProbeAngle=%ld lYProbelong=%d\n ",g_lProbeAngle,lYProbelong);
									lYProbelong1+=lYProbelong;
									SetPulse(2,pData.lValue[1],TRUE);   //�����ֱֵ�����ý�ȥ,��ϵͳ��Ϊ�Ѿ��ﵽ�˽�������
									vm_sync();							//ͬ������ջ���
									//CloseProbe(0);
									break;								//�˳�����ִ����һ��?
								}
							}
						}

						if (pMQ->QueueCount==0 && ReadFifo()==0 && IsInpEnd(1))	//��̽����ִ���ꡣ
						{

							INT16S flag = 0;

							OSTimeDly(3);								//��Ϊ̽�����ʱ

							switch(g_iCheckStatusIO)
							{
							case 1:
								flag = RealIO(TTANZHEN6,0,2);

							default:
								break;
							}

							if(flag==1)									//̽��ɹ�
							{
								//	vm_stop();								//���建���ֹͣ����
								vm_sync();								//ͬ������ջ���
								//CloseProbe(0);
								break;									//�˳�����ִ����һ��?
							}
							else
							{
								g_lProbeFail++;							//̽��δ����ʧ��

								//	vm_stop();								//���建���ֹͣ����
								vm_sync();								//ͬ������ջ���

								//CloseProbe(0);

								if(g_Ini.lProbeCount ==1)
								{
									g_iErrorNo=14;						//��̽��δ����ʧ��
									OSTimeDly(30);
									SetQuiteRange(0);

									WriteBit(O_SENDADD,0);

									ExitScrProtect();

									CTeach_UpdateStatus(STATUS_PAUSE);

									OSQPost(MsgQueue,g_bEnglish?"Probe hasn't touched!":"̽��δ����,��\"����\"��\"����\"����\n");

									while(TRUE)
									{

										if(g_iErrorNo != 0 && g_iErrorNo!=14 && g_iErrorNo!=3 && g_iErrorNo != 2 && g_iErrorNo != 4)
										{
											Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
											goto   WHILEEND;
										}
										if(g_iErrorNo == 0)
											break;

										OSTimeDly(1);
									}
								}

								if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_bAutoRunMode=FALSE;
								}

								break;									//�˳�����ִ����һ��?
							}
						}

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{
							g_lProbeAngle = -888888.0;

							bProbeFirstRun = TRUE;
							iHaveTag--;

							lYProbelong = 0;
							lYProbelong1 = 0;
							g_Ini.lRunCount=vm_get_tag();
							lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

							ti2=OSTimeGet();

							g_lRunTime=ti2-ti1;

							if(g_lRunTime<0)
								g_lRunTime=1000;

							OSSemPost(RunSpeedCountSem);
							ti1=OSTimeGet();

							if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
						}

						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
						{
							if(bFirstProbe == FALSE)
							{
								//CloseProbe(0);
								Uart_Printf("vm_get_tag error! very improtion\n");
							}
							goto  AUTORUNEND;
						}

						OSTimeDly(1);

						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 4)
						{
							if(bFirstProbe == FALSE)
							{
								//CloseProbe(0);
							}


							goto   WHILEEND;
						}

					} // end while(TRUE)

					SetCommandPos(1,0);				 					//̽��������λ�ã����������ͺ���
					SetActualPos(1,0);
					g_bLagCheck = TRUE;

				} //end if( pData.nIO) ̽�봦��
#endif

																//̽��8������ȫ��
				if(pData.fDelay-0.0>0.000001 ||  ( pData.nIO>0 && pData.nIO!=8 && g_Ini.iProDelay>0))					//�������ʱ
				{
					INT32U tt2=0;
					INT32U tt1=0;

					vm_start();

					while(TRUE)					//�ȴ��������ݷ����ٿ�ʼ��ʱ,��IO�ȴ�
					{

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{
							g_lProbeAngle = -888888;

							bProbeFirstRun = TRUE;
							iHaveTag--;

							lYProbelong = 0;
							lYProbelong1 = 0;
							g_Ini.lRunCount=vm_get_tag();
							lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

							ti2=OSTimeGet();

							g_lRunTime=ti2-ti1;

							if(g_lRunTime<0)
								g_lRunTime=1000;

							OSSemPost(RunSpeedCountSem);
							ti1=OSTimeGet();

							if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
							CheckDuxian();
						}

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
						{
							//Uart_Printf("4iHaveTag=%d\n",iHaveTag);
							goto  AUTORUNEND;
						}


						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 )
						{
							Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
							goto   WHILEEND;
						}


						if(!vm_get_status())
							break;

						OSTimeDly(1);
					}

					vm_sync();								//ͬ������ջ���

					if(g_Ini.iProDelay>0 && pData.nIO)							//����ȴ�
					{
						INT16U PROIO[10]={TTANZHEN1,TTANZHEN2,TTANZHEN3,TTANZHEN4,TTANZHEN5,TTANZHEN6,TTANZHEN7};


						tt1=OSTimeGet();

						while(TRUE)
						{
							tt2=OSTimeGet();


							if(RealIO(PROIO[pData.nIO-1],0,2))
								break;

							//if((tt2-tt1) >= (INT32U)(10 *1000.0))//����ȴ�10�볬ʱ
							if((tt2-tt1)*1000.0/OS_TICKS_PER_SEC>g_Ini.iProDelay)//������ָ������������˳�
							{
								g_lProbeFail++;

								if(g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_iErrorNo = 60;//����ȴ���ʱ����
									goto WHILEEND;
								}
								else
									break;
							}

							if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 )
							{
								goto   WHILEEND;
							}

							OSTimeDly(1);
						}

					}


					if(pData.fDelay-0.0>0.000001)	//��ʱ
					{

						tt1=OSTimeGet();

						while(TRUE)
						{
							tt2=OSTimeGet();

							if((tt2-tt1) >= (INT32U)(pData.fDelay*1000.0) && GetRange()!=0)
							{
								break;
							}

							if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 )
							{
								goto   WHILEEND;
							}

							OSTimeDly(1);
						}
					}
				}// END 	if(pData.fDelay-0.0>0.000001 ||  pData.nIO>0)					//�������ʱ


				if(pMQ->QueueCount>8)												//���ӶԲ岹����ռ���Ӧ�ò��������
				{//����������ڽ��л���

					vm_start();

					while(TRUE)
					{

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{
							g_lProbeAngle = -888888;

							bProbeFirstRun = TRUE;
							iHaveTag--;
							//	Uart_Printf("3iHaveTag=%d\n",iHaveTag);

							lYProbelong = 0;
							lYProbelong1 = 0;
							g_Ini.lRunCount=vm_get_tag();
							//Uart_Printf("1lRunCount=%d\n",g_Ini.lRunCount);

							lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

							ti2=OSTimeGet();

							g_lRunTime=ti2-ti1;

							if(g_lRunTime<0)
								g_lRunTime=1000;

							OSSemPost(RunSpeedCountSem);
							ti1=OSTimeGet();


							if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}

							CheckDuxian();
						}

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  iHaveTag>0)
						{
							//Uart_Printf("4iHaveTag=%d\n",iHaveTag);

							goto  AUTORUNEND;
						}

						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 4 && g_iErrorNo != 61)
						{
							//Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
							goto   WHILEEND;
						}

						if(pMQ->QueueCount<6)
						{
							//	Uart_Printf("testp pMQ->QueueCount<600\n");
							break;
						}

						OSTimeDly(1);
					}
				} // end if(pMQ->QueueCount>120)

			}
			break;

			case JUMP:
				continue;

			case PEND: 									//����ָ��
			{
				XMSToMEMEx((INT8U *)&pDataTmp,g_uXMS,(g_lXmsSize-2)*sizeof(UNITDATA),sizeof(UNITDATA));

				for(k=0;k<MAXCH;k++)
				{
					if(g_iEmpNode[k]!=99999999)
					{
						lCurPulse[k]=pDataTmp.lValue[k];
					}
				}

				//	vm_set_tag(g_Ini.lRunCount+lTagCount+1);

				itempRunCount++;
				vm_set_tag(itempRunCount);

				bProbeFirstRun = FALSE;
				iHaveTag++;

				/*
						if(lTagCount>=TAGBUFCOUNT)
						{
							vm_start();

							while(TRUE)
							{

								if(g_bDataCopy==FALSE || g_bAutoRunMode==FALSE)
								{
								//	Uart_Printf("testpoint AUTORUNEND1\n");

									goto AUTORUNEND;
								}
								else if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
								{
									//Uart_Printf("g_Ini.lRunCount2 = %ld %d\n",g_Ini.lRunCount,vm_get_tag());
									g_Ini.lRunCount=vm_get_tag();
									lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

									//Uart_Printf("g_Ini.lRunCount2 = %ld\n",g_Ini.lRunCount);

									ti2=OSTimeGet();

									g_lRunTime=ti2-ti1;

									if(g_lRunTime<0)
										g_lRunTime=1000;

									OSSemPost(RunSpeedCountSem);
									ti1=OSTimeGet();


									//Uart_Printf("tag =%d %d\n",vm_get_tag(),lTagLastCount);

									if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
									{
										//OSQPost(MsgQueue,g_bEnglish?"Count of running arrive!":"�ӹ���������!");
										g_bAutoRunMode=FALSE;
									//	Uart_Printf("testpoint AUTORUNEND4\n");
										goto AUTORUNEND;
									}

									break;
								}

								//Uart_Printf("tag =%ld\n",vm_get_tag());


								OSTimeDly(1);

								if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4)
								{
								//	Uart_Printf("g_iErrorNo101 = %d\n",g_iErrorNo);
									goto   WHILEEND;
								}
							}
						}
						else
							lTagCount++;
				 */

				if(g_bDataCopy==FALSE ||g_bAutoRunMode==FALSE||pData.nIO==1)
				{
					//Uart_Printf("testpoint AUTORUNEND3\n");
					goto AUTORUNEND;
				}

			}
			break;

			default:
				break;
			}

		}//end for(; lNode<g_lDataCopyXmsSize;)

AUTORUNEND:
		if(g_bAutoRunMode==FALSE || g_bDataCopy==FALSE||openpro==TRUE )
		{
			vm_start();

			vm_tag_stop();//�ߵ������Eָ��

			//Uart_Printf("vm_get_tag1=%ld\n",vm_get_tag());

			while(vm_get_status())
			{
				OSTimeDly(1);

				if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 && g_iErrorNo != 61)
				{
					Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
					goto   WHILEEND;
				}

			}

			OSTimeDly(1);//THMOTIONҪ��

			//	Uart_Printf("vm_get_tag2=%ld\n",vm_get_tag());

			itempRunCount = g_Ini.lRunCount=vm_get_tag();
			lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

			bProbeFirstRun =  TRUE;
			iHaveTag=0;

			{
				INT32S temp = 0;

				temp = GetPulse(1,TRUE)%g_Ini.lPulse[0];		//x
				ps_debugout("GetPulse(1,TRUE) = %d\r\n",GetPulse(1,TRUE));
				if(temp ==0)
					SetMM(1,0,TRUE);


				//	SetMM(2,0,TRUE);								//y
				if(g_Ini.iAxisRunMode[1]!=2)
				{
					if(g_Ini.iAxisRunMode[1]==3)//������
						SetMM(2,0,TRUE);
					else
					{

						temp = GetPulse(2,TRUE)%g_Ini.lPulse[1];	//y
						/****************�и�Y����̽��1ʱ���⹤��***************/
						if((temp == 0)&&(openpro==TRUE))
						{
							Uart_Printf("testpoint AUTORUNEND3@@@\n");
							SetMM(2,0,TRUE);
							g_bBackZero=TRUE;
							if(!BackZero(2,g_Ini.iBackDir[1]))
							{
								goto   WHILEEND;
							}

							SetMM(7,0,FALSE);							//���������ֺ����壬��Ȼ��"��������"��״̬�½�����Щ���沢��ҡ���ֶ������˳�ʱ��ѡ�е�����ƶ���
							SetMM(8,0,FALSE);
						}
						/*******************************************************/
					}
				}


				if(g_Ini.iAxisRunMode[2]!=2)
				{
					temp = GetPulse(3,TRUE)%g_Ini.lPulse[2];	//z

					if(temp ==0)
						SetMM(3,0,TRUE);
				}

				//Uart_Printf("tempz=%d\n",temp);
				if(g_Ini.iAxisRunMode[3]!=2)
				{
					temp = GetPulse(4,TRUE)%g_Ini.lPulse[3];		//a

					if(temp ==0)
						SetMM(4,0,TRUE);
				}


				if(g_Ini.iAxisRunMode[4]!=2)
				{
					temp = GetPulse(5,TRUE)%g_Ini.lPulse[4];		//c

					if(temp ==0)
						SetMM(5,0,TRUE);
				}


				if(g_Ini.iAxisRunMode[5]!=2)
				{
					temp = GetPulse(6,TRUE)%g_Ini.lPulse[5];		//c

					if(temp ==0)
						SetMM(6,0,TRUE);
				}
			}

			vm_sync();					//ͬ������ջ���

			ti2=OSTimeGet();

			g_lRunTime=ti2-ti1;

			if(g_lRunTime<0)
				g_lRunTime=1000;

			OSSemPost(RunSpeedCountSem);
			ti1=OSTimeGet();

		}

	}while(g_bAutoRunMode);

WHILEEND:

	//	Uart_Printf("testpoint WHILEEND\n");

	g_iOutStatus = 0;
	g_lStepNode=0;

	g_iMoveAxis=0;

	g_bCanDispRunTime = FALSE;

	{
		INT8S a=0;

		for(a=1;a<5;a++)
			SuspendTasks(a);

		OSTimeDly(50);
	}

	RunEnd();

	return TRUE;
}



void	RunEnd()
{
	/*
	if(g_Ini.iScrProtect == 1)									//������ʾ
	{
		if(!g_bLCD || g_bScrProtectRun)
		{
			if(g_Ini.iScrProtectBmp && g_bInitbmp)
			{
				g_bScrProtectRun=FALSE;
				CDC_RestoreScreen();
			//	g_bFirstDispRunTime=TRUE;
				ResumeTasks(5);
				UpdateSpeedCountProbe();
			}
			else
			{
				//Uart_Printf("testpoint 52\n");
				LCD_Power(1);
 				Lcd_EnvidOnOff(1);
				g_bLCD = TRUE;
			}
		}
	}*/
	ExitScrProtect();


	if(g_iCurEditNo!=9)											//��ǰ�ı༭�㲻���б�ʱ  Ϊ9Ϊ���б�༭״̬
	{
		UpdateFileDestProbe(g_iCurEditNo+1,1,0);
		g_iCurEditNo = 9;

		if(g_bDispRunTime)
		{
			//CTeach_GotoNline(1);
			CTeach_InvidateView(FALSE);
			g_bDispRunTime=FALSE;
		}
	}
	else
	{
		if(g_bDispRunTime)
		{
			//CTeach_GotoNline(1);
			CTeach_InvidateView(FALSE);
			g_bDispRunTime=FALSE;
		}
		else
		{
			CTeach_Update(0,0);
			//Uart_Printf("test\n");
		}
	}

	DisplayXYZ(TRUE);


	WriteBit(O_SENDADD,0);
	CloseProbe(0);


	WriteRunCount();

	CTeach_UpdateStatus(STATUS_STOP);

	LedOut(RUNLED,0);

	if(g_bMode)
	{
		g_bMode=FALSE;
		LedOut(TESTLED,0);
	}

	if(g_Ini.iHandStatus != 0)
	{
		g_Ini.iHandStatus=0;
		LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
		CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"���ֹ�",g_TextColor5,g_nBk,KZK_22);
	}

	if(g_bHandset)
	{
		g_bHandset = FALSE;
		LedOut(BOXLED,g_bHandset?1:0);
		CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOff  ":"�ֺй�",g_TextColor5,g_nBk,KZK_22);
	}

	LedOut(STOPLED,1);

	if(g_Ini.lRunCount <= g_Ini.lDestCount)						//�ӹ�������ʾ
	{
		if(g_bStepIO)
		{
			OSQPost(MsgQueue,g_bEnglish?"TestRun Stop":"���Լӹ�����!");
		}
		else if(g_bHandRun)
		{
			OSQPost(MsgQueue,g_bEnglish?"HandRateRun Stop":"��ҡ�ӹ�����!");
		}
		else
		{
			OSQPost(MsgQueue,g_bEnglish?"AutoRun Stop":"�Զ��ӹ�����!");
		}
	}


	if(g_Ini.lRunCount>=g_Ini.lDestCount)						//�ӹ������ʾ
	{
		OSTimeDly(70);

		DisplayXYZ(TRUE);
		/*
		if(g_Ini.iWarningOut==1)
		{
			WriteBit(O_WARNING,OPEN);
		}*/
		CDC_SaveScreen();

		RunStatusOutCtr(1);

		sprintf(g_pszMsgBuf,g_bEnglish?"%dAmount fill!":"�ӹ����������:\n     %d��!",g_Ini.lRunCount);
		CAfxMsgDlg_Init(250,200,300,150,g_pszMsgBuf,g_bEnglish?"Waring":"����",REDCOLOR);
		CAfxMsgDlg_DoModal();
		CAfxMsgDlg_Destroy();

		//	WriteBit(O_WARNING,CLOSE);
		RunStatusOutCtr(0);

		g_Ini.lRunCount=0;
		WriteRunCount();
		WriteDataToFile(g_Ini.iFileName);
		SysParaWrite();

		CDC_RestoreScreen();
		UpdateSpeedCountProbe();

		OSQPost(MsgQueue,g_bEnglish?"Count of running arrive!":"�ӹ���������!");
		//	CTeach_Create();
	}

	if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)  //̽��ʧ����ʾ
	{
		OSQPost(MsgQueue,g_bEnglish?"ProbeFail Reaches Setting!":"̽��ʧ�ܴ�������!");
		g_lProbeFail = 0;
	}


	{															//������ʾ

		if(g_iErrorNo>6 && g_iErrorNo<=12)
		{
			g_bFirstResetX=FALSE;							//ΪTRUEʱ��Ҫ��λ
			g_bFirstResetY=FALSE;
			g_bFirstResetZ=FALSE;
			g_bFirstResetU=FALSE;
			g_bFirstResetV=FALSE;
			g_bFirstResetW=FALSE;

			BackZeroLed();
		}

		if(g_iErrorNo==1 || g_iErrorNo==61)
		{
			g_bEMSTOP=TRUE;
			g_lStepNode=0;

			if(g_bMode)
			{
				g_bStepIO=FALSE;
				g_bDispRunLine=FALSE;
				g_bAutoRunMode=TRUE;
				//CDC_OutText24(563,406,g_bEnglish?"Auto":"�Զ�",g_TextColor5,g_nBk);
				g_bMode=FALSE;
				LedOut(TESTLED,g_bMode?1:0);							//������ʾ�������ģʽ

			}
		}



		/*	if(g_Ini.iWarningOut==1)
			{
				if(GetOut(O_WARNING)==0)
					WriteBit(O_WARNING,OPEN);
			}*/

		switch(g_iErrorNo)
		{

		case 1:
		{
			INT16S l=0;

			for(l=0; l<15; l++)
			{
				WriteBit(l,0);
			}

			CloseProbe(0);

		}

		OSQPost(MsgQueue,g_bEnglish?"Stop Warning!":"��ͣ����!!!");
		break;

		case 2:	OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning!":"���߱���!!!");						break;
		case 3:	OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning!":"���߱���!!!");						break;
		case 4:	OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning!":"���߱���!!!");						break;
		case 5: OSQPost(MsgQueue,g_bEnglish?"Safe Door Waring":"��ȫ�Ŵ�״̬!!!");						break;

		case 7:	OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;
		case 8:	OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;
		case 9:	OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;
		case 10:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;
		case 11:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;
		case 12:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Servo Warning!":"�ŷ�����!!!",TRUE));			break;

		case 14:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for no Touch!":"̽��δ����!!!");					break;
		case 15:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for Touching Throughout!":"̽��һֱ����!!!");		break;
		case 18:OSQPost(MsgQueue,g_bEnglish?"Command J Error!":"J��תλ�ô�!!!");							break;

		case 20:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 21:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 22:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 23:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 24:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 25:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 26:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 27:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 28:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 29:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 30:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Negative Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;
		case 31:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Positive Hard Limit Warning!!!":"Ӳ������λ����!!!",TRUE));							break;

		case 32:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 33:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;
		case 34:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 35:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;
		case 36:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 37:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;
		case 38:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 39:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;
		case 40:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 41:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;
		case 42:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Negative Soft Limit Warning!!!":"����λ����!!!",TRUE));							break;
		case 43:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Positive Soft Limit Warning!!!":"������λ����!!!",TRUE));							break;

		case 44:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 45:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 46:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 47:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 48:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 49:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Lag Warning!":"�ͺ󱨾�!!!",TRUE));					break;
		case 50:OSQPost(MsgQueue,g_bEnglish?"system error! check data!":"�ٶ��쳣����,�Ƿ�λ�ò����������嵱������!"); break;

		case 60:OSQPost(MsgQueue,g_bEnglish?"IO Wait timeout alert":"����ȴ���ʱ����!"); break;
		case 61:OSQPost(MsgQueue,g_bEnglish?"Blocking line Waring":"���߱���!"); break;
		default:
			break;
		}

		if(g_iErrorNo>=1)			//��ͣ�������˳�������
			RunStatusOutCtr(1);
		else
			RunStatusOutCtr(0);


	}

	g_iErrorNo = 0;

	g_iWorkStatus=NORUN;

	vm_clear_errNo();

	if(!IsInpEnd(1))
	{
		while(TRUE)
		{
			if(IsInpEnd(1))
			{
				OSQPost(MsgQueue,"");
				break;
			}

			vm_stop(0);

			OSTimeDly(200);

			OSQPost(MsgQueue,g_bEnglish?"System Ready.":"ϵͳ׼����");
		}
	}

	OSTimeDly(200);

}


//�����ӹ�iDuXianNum�����û�м�⵽�����źžͼ�ͣ����
void	CheckDuxian()
{
	if(g_Ini.iDuXianNum)
	{
		if(g_bDuxianIO || nLastDuxCount == 0 || nLastDuxCount>g_Ini.lRunCount)
		{
			nLastDuxCount = g_Ini.lRunCount;
			g_bDuxianIO = FALSE;
			//OSQPost(MsgQueue," ");
		}
		else if(!g_bDuxianIO && (g_Ini.lRunCount+2-nLastDuxCount>g_Ini.iDuXianNum))
		{
			//			if(g_bBackZero)
			//			{
			//				switch(g_Ini.iAxisCount)
			//				{
			//					case 6:Stop1(6);
			//					case 5:Stop1(5);
			//					case 4:Stop1(4);
			//					case 3:Stop1(3);
			//					case 2:Stop1(2);
			//					default:Stop1(1);
			//				}
			//			}
			//			else
			{
				vm_stop(0);

				{
					INT16S l=0;

					for(l=0; l<15; l++)
					{
						WriteBit(l,0);
					}

					RunStatusOutCtr(1);

					CloseProbe(0);
				}

				//	g_bWheel = FALSE;
				//	LedOut(WHEELLED,0);
				g_uiMoveKey = 0;

				g_lStepNode = 0;
				g_bStepIO=FALSE;
				g_bDispRunLine=FALSE;
				g_bAutoRunMode=FALSE;
				g_bMode=FALSE;

				g_iErrorNo=61;
				nLastDuxCount = 0;
				if(g_Ini.iBackMode[0]!=4)
					SetMM(1,0,TRUE);
			}
		}
	}
}

void	UpdateLeavingTime(BOOL init,BOOL disp,BOOL DataInit)
{

	static DATETIME_T	ti;

	static INT32S day=0;
	INT8S buf[0x10];
	INT32S tt1=0,tt2=0;
	INT32S Hour1,Minute1,Second1,Hour2,Minute2,Second2;
	INT32S leftOffset=40;

	leftOffset=(g_bEnglish?30:40);

	if(DataInit)
	{
		RtcGetDateTime(&ti);

		Hour11=-1;Minute11=-1;Hour12=-1;Minute12=-1;
		day=0;
		return;
	}

	if(init)
	{
		if(g_bEnglish)
		{
			CDC_DrawWin(170+leftOffset,210,390,120,"Running...");

			CDC_OutText_UTF(180+leftOffset,252,"runTime:        Hour     Min     Sec",BLACKCOLOR,-1,KZK_22);
			CDC_OutText_UTF(186+leftOffset,282,"sprTime:        Hour     Min     Sec",BLACKCOLOR,-1,KZK_22);
		}
		else
		{
			CDC_DrawWin(180+leftOffset,210,340,120,"���ڼӹ�...");
			CDC_OutText_UTF(192+leftOffset,250,"����ʱ��:        Сʱ      ��      ��",BLACKCOLOR,-1,KZK_22);
			CDC_OutText_UTF(192+leftOffset,280,"ʣ��ʱ��:        Сʱ      ��      ��",BLACKCOLOR,-1,KZK_22);
		}

		Hour11=-1;Minute11=-1;Hour12=-1;Minute12=-1;
	}

	if(g_fRunSpeed>=0.01)
	{
		DATETIME_T	tti;

		RtcGetDateTime(&tti);

		if(tti.day!=ti.day)
		{
			day+=1;
			ti.day=tti.day;
		}

		tt1=day*(24*3600)+tti.hour*3600+tti.minute*60+tti.second-(ti.hour*3600+ti.minute*60+ti.second);
		tt2=(INT32S)(((FP32)(g_Ini.lDestCount-g_Ini.lRunCount))/(g_fRunSpeed/60.0));
	}

	Hour1=tt1/3600;
	Minute1=(tt1-Hour1*3600)/60;
	Second1=tt1%60;

	if(Hour1>9999)
		Hour1=9999;

	if(Hour1!=Hour11)
	{
		if(disp)
		{
			sprintf(buf,"%04d",Hour1);
			CDC_OutTextLR_UTF(290+leftOffset,250,buf,BLACKCOLOR,BLUECOLOR,MZK_24,3,1);


		}
		Hour11=Hour1;
	}

	if(Minute1!=Minute11)
	{
		if(disp)
		{
			sprintf(buf,"%02d",Minute1);
			CDC_OutTextLR_UTF((g_bEnglish?403:395)+leftOffset,250,buf,BLACKCOLOR,BLUECOLOR,MZK_24,1,1);
		}
		Minute11=Minute1;
	}

	if(disp)
	{
		sprintf(buf,"%02d",Second1);
		CDC_OutTextLR_UTF((g_bEnglish?480:458)+leftOffset,250,buf,BLACKCOLOR,BLUECOLOR,MZK_24,1,1);
	}

	Hour2=tt2/3600;
	Minute2=(tt2-Hour2*3600)/60;
	Second2=tt2%60;


	if(Hour2>9999)
		Hour2=9999;

	if(Hour2!=Hour12)
	{
		if(disp)
		{
			sprintf(buf,"%04d",Hour2);
			CDC_OutTextLR_UTF(290+leftOffset,280,buf,BLACKCOLOR,BLUECOLOR,MZK_24,3,1);
		}
		Hour12=Hour2;
	}

	if(Minute2!=Minute12)
	{
		if(disp)
		{
			sprintf(buf,"%02d",Minute2);
			CDC_OutTextLR_UTF((g_bEnglish?403:395)+leftOffset,280,buf,BLACKCOLOR,BLUECOLOR,MZK_24,1,1);
		}
		Minute12=Minute2;
	}

	if(disp)
	{
		sprintf(buf,"%02d",Second2);
		CDC_OutTextLR_UTF((g_bEnglish?480:458)+leftOffset,280,buf,BLACKCOLOR,BLUECOLOR,MZK_24,1,1);
	}

}


void	CloseDisplay(FP32 tt)
{
#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  							/* Prevent compiler warning                           */
#endif

	if(g_Ini.iScrProtect==1)
	{
		if(!g_bLCD || fabs(tt-0.0)<0.001 || g_bScrProtectRun || g_bMode /*|| g_bWheel*/)
		{
			OS_ENTER_CRITICAL();
			g_fCloseDispCount=0.0; 												//����ʱ�临λ
			OS_EXIT_CRITICAL();
			return;
		}

		OS_ENTER_CRITICAL();
		g_fCloseDispCount+=tt;
		OS_EXIT_CRITICAL();


		if((g_fCloseDispCount/60.0) > g_Ini.iScrProtectTime)
		{
			//			g_bScrProtectRun=TRUE;
			if(g_Ini.iScrProtectBmp && g_bInitbmp)
			{
				SuspendTasks(5);
				CDC_SaveScreen();
				OSTimeDly(150);
				g_bScrProtectRun=TRUE;
				OSTimeDly(50);
				CDC_LOCKSRC();
				DispBMP("\\PARA\\start.bmp",1,0,0);
				CDC_UNLOCKSRC();
			}
			else
			{
				//Uart_Printf("CloseDisplay2\n");

				//	LCD_Power(0);
				//	Lcd_EnvidOnOff(0);
				//				Lcd_PowerOnOff(0);

				g_bLCD = FALSE;
			}

			OS_ENTER_CRITICAL();
			g_fCloseDispCount=0.0;
			OS_EXIT_CRITICAL();
		}
	}
}


void	UpdateProbe(BOOL init)
{
	INT16S i,j;
	static INT16S oldstatus[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	INT16S status[8]={0};

	if(init)
	{
		for( j=0;j<8;j++)
			oldstatus[j]=-1;
	}

	for( i=0; i<8; i++)//(g_Ini.iAxisCount<3?4:8); i++)
	{
		switch(i)
		{
		case 0:status[i]=ReadBit(4);break;
		case 1:status[i]=ReadBit(3);break;
		case 2:status[i]=ReadBit(9);break;
		case 3:status[i]=ReadBit(8);break;
		case 4:status[i]=ReadBit(14);break;
		case 5:status[i]=ReadBit(13);break;
		case 6:status[i]=ReadBit(19);break;
		case 7:status[i]=ReadBit(18);break;
		}

		if(status[i]!=oldstatus[i])
		{
			oldstatus[i]=status[i];

			if(status[i]==0)
			{

				CDC_Fillellipse(443+i*15,getmaxy()-47,5,5,g_TextColor5);

				if(status[i])
				{
					oldstatus[i]=-1;
				}
			}
			else
			{
				CDC_Fillellipse(443+i*15,getmaxy()-47,5,5,g_nTitleBk);
			}
		}
	}
}

/*
void	UpdateProbe(BOOL init)
{
	INT16S i,j;
	CRect rc;
	static INT16S oldstatus[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	INT16S status[8]={0};
	char buf[0x20];
	static INT32S s_lProbeAngle = 0;

	if(init)
	{
		for( j=0;j<8;j++)
			oldstatus[j]=-1;
	}

	if(g_iToolBarPage==0)
	{

		if(g_lProbeAngle != -999999)
		{
			s_lProbeAngle = g_lProbeAngle;

			g_lProbeAngle = -999999;

			memset(buf,NULL,sizeof(buf));

			if(s_lProbeAngle == -188888)						//T1
			{

				strcpy(buf,"ǰ��");
				CDC_OutText_UTF(360,473,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -288888)					//T2
			{
				strcpy(buf,"ǰ��");
				CDC_OutText_UTF(360,507,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -177777)					//T1
			{

				strcpy(buf,"δ��");
				CDC_OutText_UTF(360,473,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -277777)					//T2
			{
				strcpy(buf,"δ��");
				CDC_OutText_UTF(360,507,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle > 150000)						//T2
			{
				s_lProbeAngle = (s_lProbeAngle -200000);

				sprintf(buf,"%d",s_lProbeAngle);

				CDC_OutText_UTF(360,504,buf,REDCOLOR,g_nBk,MZK_24);

			}
			else if(s_lProbeAngle>50000)						//T1
			{
				s_lProbeAngle = (s_lProbeAngle -100000);

				sprintf(buf,"%d",s_lProbeAngle);

				CDC_OutText_UTF(360,470,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -888888 )
			{
				CDC_PutColorBar(356,470,59,30,g_nBk,1);
				CDC_PutColorBar(356,504,59,30,g_nBk,1);
			}
		}


		for( i=0; i<2; i++)
		{
			switch(i)
			{
			case 0:status[i]=ReadBit(4);break;
			case 1:status[i]=ReadBit(3);break;
			case 2:status[i]=ReadBit(9);break;
			case 3:status[i]=ReadBit(8);break;
			case 4:status[i]=ReadBit(14);break;
			case 5:status[i]=ReadBit(13);break;
			case 6:status[i]=ReadBit(19);break;
			case 7:status[i]=ReadBit(18);break;
			}

			if(status[i]!=oldstatus[i])
			{
				oldstatus[i]=status[i];

				switch(i)
				{
				case 0:
					{
					    rc.left = 328;
					    rc.right = 328+29;
					    rc.top = 469;
					    rc.bottom = 499;

					    CDC_DrawRect(rc,status[i]==0?BLUECOLOR:g_nBk,status[i]==0?BLUECOLOR:g_nBk);
					    CDC_OutText_UTF(330,470,"T1",status[i]==0?REDCOLOR:g_TextColor11,status[i]==0?BLUECOLOR:g_nBk,MZK_24);



				    }
				    break;
			    case 1:
					{

					    rc.left = 328;
					    rc.right = 328+29;
					    rc.top = 503;
					    rc.bottom = 533;

					    CDC_DrawRect(rc,status[i]==0?BLUECOLOR:g_nBk,status[i]==0?BLUECOLOR:g_nBk);
					    CDC_OutText_UTF(330,504,"T2",status[i]==0?REDCOLOR:g_TextColor11,status[i]==0?BLUECOLOR:g_nBk,MZK_24);

				    }
				    break;
				default:
					break;
			    }
			}
		}
	}
}*/

void	UpdateCylinder(BOOL init)
{
	INT16S i,j;
	static INT16S oldstatus[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	INT16S status[16]={0};

	if(init)
	{
		for(j=0;j<16;j++)
			oldstatus[j]=-1;
	}

	for( i=0; i<16;i++)//(g_Ini.iAxisCount<3?4:8); i++)
	{
		status[i]=GetOut(i);

		if(status[i]!=oldstatus[i])
		{
			oldstatus[i]=status[i];

			if(status[i]==1)
			{
				if(i<8)
					CDC_Fillellipse(676+i*15,getmaxy()-52,5,5,g_TextColor5);
				else
					CDC_Fillellipse(676+(i%8)*15,getmaxy()-40,5,5,g_TextColor5);

			}
			else
			{
				//	CDC_Fillellipse(676+i*15,getmaxy()-47,5,5,g_nTitleBk);
				if(i<8)
					CDC_Fillellipse(676+i*15,getmaxy()-52,5,5,g_nTitleBk);
				else
					CDC_Fillellipse(676+(i%8)*15,getmaxy()-40,5,5,g_nTitleBk);

			}
		}
	}
}

/*void	UpdateCylinder(BOOL init)
{
	INT16S i,j;
	char buf[0x10] = "\0";
	//CRect rc[7] = {{2,454,91,479},{93,454,182,479},{184,454,273,479},{275,454,364,479},{366,454,455,479},{457,454,545,479},{547,454,637,479}};
	CRect rc[7] = {{2,568,114,599},{116,568,228,599},{230,568,342,599},{344,568,456,599},{458,568,570,599},{572,568,684,599},{686,568,797,599}};
	static INT16S oldstatus[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	INT16S status[8]={0};

	if(init)
	{
		for(j=0;j<8;j++)
			oldstatus[j]=-1;
	}

	if(g_iToolBarPage==0)
	{
		for( i=0; i<7; i++)
		{

			if(6 == i)
				i=7;

			status[i]=GetOut(i);

			if(status[i]!=oldstatus[i])
			{
				oldstatus[i]=status[i];

				if(7 == i)
				{
					strcpy(buf,"�߼ܼ���");
				}
				else
					sprintf(buf,g_bEnglish?"OUT%d":"����%d",i+1);

				if(7 == i)
				{
					CDC_DrawRect(rc[i-1],status[i]==1?WHITECOLOR:g_nBk,WHITECOLOR);
					CDC_OutText_UTF(rc[i-1].left+2,572,buf,status[i]==1?BLUECOLOR:g_TextColor11,status[i]==1?WHITECOLOR:g_nBk,KZK_26);
					}
				else
				{
					CDC_DrawRect(rc[i],status[i]==1?WHITECOLOR:g_nBk,WHITECOLOR);
					CDC_OutText_UTF(rc[i].left+26,572,buf,status[i]==1?BLUECOLOR:g_TextColor11,status[i]==1?WHITECOLOR:g_nBk,KZK_26);
				}
			}

		}
	}
}*/


/*void	UpdateProbe(BOOL init)													//ˢ����ʾ̽���ź�״̬
{

	INT16S i,j;
	CPoint point;
	INT8S  in[4]={4,3,9,8};
	static INT16S oldstatus[4]={-1,-1,-1,-1};
	INT16S status[4]={0};
	INT8S  string[2];

	memset(string,NULL,2);

	if(init)
	{
		for( j=0; j<4; j++)
			oldstatus[j] = -1;
	}

	for( i=0; i< 4; i++)
	{
		status[i] = ReadBit(in[i]);
		point.x = 95+ i*30;
		point.y = 420;

		if(status[i] != oldstatus[i])
		{
			oldstatus[i] = status[i];

			if(status[i] == 0)
			{
				string[0] = '1'+i;

				CDC_Fillellipse2(point,10,10,YELLOWCOLOR);

				CDC_OutText24(point.x-6,point.y-13,string,BLACKCOLOR,-1);

			}
			else
			{
				CDC_Fillellipse2(point,10,10,g_nBk);
				CDC_Fillellipse2(point,7,7,WHITECOLOR);
			}
		}
	}
}



void	UpdateCylinder(BOOL init)												//ˢ����ʾ����״̬
{

	INT16S 	i,j;
	CPoint 	point;
	static 	INT16S oldstatus[4]={-1,-1,-1,-1};
	INT16S 	status[4]={0};
	INT8S 	string[2];

	memset(string,NULL,2);

	if(init)
	{
		for(j=0; j<4; j++)
			oldstatus[j] = -1;
	}

	for( i=0; i<4; i++)
	{
		status[i] = GetOut(i);

		point.x = 315 + i*30;
		point.y = 420;

		if(status[i] != oldstatus[i])
		{
			oldstatus[i] = status[i];

			if(status[i] == 1)
			{
				string[0] = '1'+i;
				CDC_Fillellipse2(point,10,10,YELLOWCOLOR);
				CDC_OutText24(point.x-6,point.y-13,string,BLACKCOLOR,-1);
			}
			else
			{
				CDC_Fillellipse2(point,10,10,g_nBk);
				CDC_Fillellipse2(point,7,7,WHITECOLOR);
			}
		}
	}

}
 */

void  InitLed()
{
	AllLedOff();

	LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]==1?0:1);  							//������ر�ʱ����

	if(g_Ini.iAxisCount > 2)
		LedOut(ZSWITCHLED,g_Ini.iAxisSwitch[2]==1?0:1);

	if(g_Ini.iAxisCount > 3)
		LedOut(USWITCHLED,g_Ini.iAxisSwitch[3]==1?0:1);

	if(g_Ini.iAxisCount > 4)
		LedOut(VSWITCHLED,g_Ini.iAxisSwitch[4]==1?0:1);

	if(g_Ini.iAxisCount > 5)
		LedOut(WSWITCHLED,g_Ini.iAxisSwitch[5]==1?0:1);

	LedOut(TESTLED,g_bMode?1:0);												//������ʾ�������ģʽ
	//g_Ini.iHandStatus = 0;
	//	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);									//������ʾ�������ҡ��
	LedOut(STOPLED,1);
	BackZeroLed();

	DisplayXYZ(FALSE);
	//	AllLedOff();
}


void	BackZeroLed()															//�жϹ�����Ƿ����
{
	BOOL bXStatus=g_bFirstResetX?TRUE:(g_Ini.iAxisSwitch[0]==1?FALSE:TRUE);
	//BOOL bYStatus=g_bFirstResetY?TRUE:(g_Ini.iAxisSwitch[1]==1?FALSE:TRUE);
	BOOL bZStatus=g_bFirstResetZ?TRUE:((g_Ini.iAxisCount>2 && g_Ini.iAxisSwitch[2]==1)?FALSE:TRUE);
	BOOL bUStatus=g_bFirstResetU?TRUE:((g_Ini.iAxisCount>3 && g_Ini.iAxisSwitch[3]==1)?FALSE:TRUE);
	BOOL bVStatus=g_bFirstResetV?TRUE:((g_Ini.iAxisCount>4 && g_Ini.iAxisSwitch[4]==1)?FALSE:TRUE);
	BOOL bWStatus=g_bFirstResetW?TRUE:((g_Ini.iAxisCount>5 && g_Ini.iAxisSwitch[5]==1)?FALSE:TRUE);


	LedOut(BACKZEROLED,(bXStatus && bZStatus && bUStatus && bVStatus && bWStatus)?0:1);
	//Uart_Printf("bYStatus = %d\n",(bYStatus && bZStatus && bUStatus && bVStatus && bWStatus));
}


BOOL	WorkFindZero(INT16S ch)
{
	INT8S a=0;

	SetQuiteRange(8000);

	if(ch==2 && g_Ini.iAxisRunMode[1]==3)
	{
		SetMM(ch,0,TRUE);
		return TRUE;
	}


	for(a=1; a<5; a++)
		ResumeTasks(a);

	OSTimeDly(20);
	g_bBackZero=TRUE;
	g_iErrorNo=0;


	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset...":"�����...",TRUE));


	if( !FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
	{
		OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset Interrupted!":"������ж�,��ȷ�ϼ�ͣ��ť״̬!",TRUE));

		g_bBackZero=FALSE;

		for(a=1; a<5; a++)
			SuspendTasks(a);

		OSTimeDly(50);
		return FALSE;
	}

	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset Finish.":"�������.",TRUE));

	g_bBackZero=FALSE;

	for(a=1; a<5; a++)
		SuspendTasks(a);

	OSTimeDly(50);
	return TRUE;
}


void	Warning(INT16S x,INT16S y,INT16S w,INT16S h,char *msg,char *title,INT32S tc)
{
	INT8S a=0;

	if(g_Ini.iWarningOut==1)
	{
		WriteBit(O_WARNING,OPEN);
	}

	for(a=0;a<8;a++)
		SuspendTasks(a);

	OSTimeDly(50);

	CAfxMsgDlg_Init(x,y,w,h,msg,title,tc);
	CAfxMsgDlg_DoModal();
	CAfxMsgDlg_Destroy();

	for(a=0; a<8; a++)
		ResumeTasks(a);

	if(g_Ini.iWarningOut==1)
		WriteBit(O_WARNING,CLOSE);

}

void	UpdateHandBox(BOOL flag)
{
	CRect   rc;

	rc.left = 700;
	rc.top =  415;
	rc.right = 780;
	rc.bottom = 471;
	g_Ini.iHandStatus = flag?1:0;

	CDC_DrawButton(rc,g_nBk);
	CDC_OutText24(705,430,  g_bEnglish?"HandB":"�ֳֺ�",WHITECOLOR,g_nBk);

	if(flag==FALSE)
	{
		CDC_Circle(740,443,26,REDCOLOR);
		CDC_Circle(740,443,25,REDCOLOR);
		CDC_Circle(740,443,24,REDCOLOR);
		CDC_Circle(740,443,23,REDCOLOR);
		CDC_Circle(740,443,22,REDCOLOR);
		CDC_Line(740-18,443+18,740+18,443-18,REDCOLOR);
		CDC_Line(740-18,443+19,740+18,443-19,REDCOLOR);
		CDC_Line(740-18,443+17,740+18,443-17,REDCOLOR);
		CDC_Line(740-18,443+20,740+18,443-20,REDCOLOR);
		CDC_Line(740-18,443+16,740+18,443-16,REDCOLOR);
	}
}

#if FALSE
void FunC(INT16S page,INT16S num)
{
	//CRect rc[7] = {{2,454,91,479},{93,454,182,479},{184,454,273,479},{275,454,364,479},{366,454,455,479},{457,454,545,479},{547,454,637,479}};

	CRect rc[7] = {{2,568,114,599},{116,568,228,599},{230,568,342,599},{344,568,456,599},{458,568,570,599},{572,568,684,599},{686,568,797,599}};

	INT16S OutStatus = 0;

	switch(page)
	{
	case 0:
	{
		switch(num)
		{
		case 1:
		{

			WriteBit(0,GetOut(0)?0:1);
			OutStatus = GetOut(0);

			CDC_DrawRect(rc[0],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);
			//CDC_OutText(22,459,g_bEnglish?"OUT1":"����1",OutStatus==1?BLUECOLOR:g_TextColor2,OutStatus==1?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(28,572,g_bEnglish?"OUT1":"����1",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);

			if(OutStatus==1)
				sprintf(g_pszMsgBuf,g_bEnglish?"1 Cylinder Out":"1���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"1 Cylinder In":"1�������˻�");
			OSQPost(MsgQueue,g_pszMsgBuf);


		}
		break;
		case 2:
		{

			WriteBit(1,GetOut(1)?0:1);
			OutStatus = GetOut(1);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"2 Cylinder Out":"2���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"2 Cylinder In":"2�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);

			CDC_DrawRect(rc[1],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(113,459,g_bEnglish?"OUT2":"����2",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(142,572,g_bEnglish?"OUT2":"����2",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);


		}
		break;
		case 3:
		{
			WriteBit(2,GetOut(2)?0:1);
			OutStatus = GetOut(2);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"3 Cylinder Out":"3���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"3 Cylinder In":"3�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[2],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(204,459,g_bEnglish?"OUT3":"����3",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(256,572,g_bEnglish?"OUT3":"����3",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 4:
		{
			WriteBit(3,GetOut(3)?0:1);
			OutStatus = GetOut(3);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"4 Cylinder Out":"4���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"4 Cylinder In":"4�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[3],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(295,459,g_bEnglish?"OUT4":"����4",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(370,572,g_bEnglish?"OUT4":"����4",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 5:
		{
			WriteBit(4,GetOut(4)?0:1);
			OutStatus = GetOut(4);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"5 Cylinder Out":"5���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"5 Cylinder In":"5�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[4],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(386,459,g_bEnglish?"OUT5":"����5",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(484,572,g_bEnglish?"OUT5":"����5",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 6:
		{
			WriteBit(5,GetOut(5)?0:1);
			OutStatus = GetOut(5);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"6 Cylinder Out":"6���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"6 Cylinder In":"6�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[5],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(477,459,g_bEnglish?"OUT6":"����6",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(598,572,g_bEnglish?"OUT6":"����6",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 7:
		{
			WriteBit(6,GetOut(6)?0:1);
			OutStatus = GetOut(6);

			if(GetOut(2))
				sprintf(g_pszMsgBuf,g_bEnglish?"7 Cylinder Out":"7���������");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"7 Cylinder In":"7�������˻�");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[6],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(567,459,g_bEnglish?"OUT7":"����7",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(712,572,g_bEnglish?"OUT7":"����7",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		}
	}
	break;
	case 1:
	{
		switch(num)
		{
		case 0:
		{

		}
		break;
		case 1:
		{
			BOOL bUpdate = FALSE;
			INT16S key =0;
			Uart_Printf("ȷ������ϵͳ�ļ�?��(����)��(����)\n");
			OSQPost(MsgQueue,g_bEnglish?"Upadate system file?":"ȷ������ϵͳ�ļ�?��(����)��(����)");

			while(1)
			{
				key=KeyMBoxGetKey();
				if(key!=0xffff)
				{
					if(key == TENTER)
					{
						bUpdate = TRUE;
					}

					break;
				}
				OSTimeDly(10);
			}

			if(bUpdate)
			{

				OSQPost(MsgQueue,g_bEnglish?"Update system file...":"�ļ�������...");
				switch(SysAppUpdate())
				{
				case 1:
					OSQPost(MsgQueue,g_bEnglish?"SMC System update sucessful,please reboot system":"�����̳��������ɹ�,����ϵͳ����Ч!");
					break;
				case 2:
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update sucessful,please reboot system":"U�̳��������ɹ�,����ϵͳ����Ч!");
					break;
				case -1:
					//OSQPost(MsgQueue,g_bEnglish?"SMC System update failed!":"����������ʧ��,��ȷ�������ļ��Ƿ����!");
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update failed!":"δ������U��,��ȷ��U����������!");
					break;
				case -2:
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update failed!":"U�̳����ȡʧ��,��ȷ��U�̶�д·������!");
					break;
				case -3:
					OSQPost(MsgQueue,g_bEnglish?"System update failed!":"�ļ�����ʧ��,��ȷ�������ļ�����ȷ��!");
					break;
				}
			}
		}
		break;
		case 2:
		{
		}
		break;
		case 3:
		{
		}
		break;
		case 4:
		{
		}
		break;
		case 5:
		{
		}
		break;
		case 6:
		{
		}
		break;
		case 7:
		{
			//SysParaWrite();
			sprintf(g_pszMsgBuf,g_bEnglish?"Save OK":"��ϵͳ�����!");
			OSQPost(MsgQueue,g_pszMsgBuf);
		}
		break;
		}
	}
	break;

	case 2:
	{
		switch(num)
		{
		case 1:
		{

		}	break;
		case 2:
		{




		}
		break;
		case 3:
		{

		}
		break;
		case 4:
		{

		}
		break;
		case 5:
		{

		}
		break;
		case 6:
		{

		}
		break;
		case 7:
		{

		}
		break;
		}
	}
	break;

	}
}
#endif

BOOL	g_bStepMove=FALSE;

void	PressKeyMove(INT16S ch,INT16S col,INT16S dir)
{
	FP32	temp;
	FP32  	Speed = 10.00;
	//INT32S  pp;


	if(g_lStepNode!=0)
	{
		switch(ch)
		{
		case 1:	g_bFirstResetX=TRUE;break;
		case 2:	g_bFirstResetY=TRUE;break;
		case 3:	g_bFirstResetZ=TRUE;break;
		case 4:	g_bFirstResetU=TRUE;break;
		case 5:	g_bFirstResetV=TRUE;break;
		case 6:	g_bFirstResetW=TRUE;break;
		}
	}


	if(CheckLimit(TRUE,TRUE,ch,dir>0?1:-1))
	{
		return;	
	}								


	//if(g_iSpeedBeilv==2)
	{
		INT16S stepmode=0;
		INT32S	t1,t2;


		if(ch==2 || g_Ini.iAxisRunMode[ch-1]==2)
		{
			//	if(g_bUseTimes)
			//		temp=((FP32)g_Ini.iStepDis[ch-1]*g_Ini.iStepRate[ch-1]/100.0);
			//	else
			temp=(FP32)g_Ini.iStepDis[ch-1]/100.0;

		}
		else
		{
			//	if(g_bUseTimes)
			//		temp=(FP32)g_Ini.iStepDis[ch-1]*g_Ini.iStepRate[ch-1]/10.0;
			//	else
			temp=((FP32)g_Ini.iStepDis[ch-1]/10.0);
		}


		/*
		if(g_bUseTimes)
			Speed = g_Ini.lHandSpeed[ch-1]*g_Ini.iStepRate[ch-1];
		else
			Speed = g_Ini.lHandSpeed[ch-1];*/

		if(g_iSpeedBeilv==3)									//���� 
		{
			Speed = g_Ini.lHandSpeed[ch-1]*g_Ini.iStepRate[ch-1];				
		}
		else if(g_iSpeedBeilv==2)								//���� 
		{
			Speed = g_Ini.lHandSpeed[ch-1];
		}
		else													//����
			Speed = g_Ini.lHandSpeed[ch-1] * 0.5;


		//	Uart_Printf("g_bUseTimes=%d,iStepDis=%d,iStepRate=%d\n",(int)g_bUseTimes,g_Ini.iStepDis[ch-1],g_Ini.iStepRate[ch-1]);

		g_bStepMove=TRUE;
		t1=OSTimeGet();

		while(TRUE)
		{
			HandFunc();

			t2=OSTimeGet();

			if(t2-t1>200||t2-t1<0)
			{
				stepmode=1;
				break;
			}

			if(!g_bStepMove)
			{
				stepmode=0;
				break;
			}

			OSTimeDly(3);
		}


		if(ch ==1 || (ch ==3 && g_Ini.iAxisRunMode[2]==0) || (ch ==4 && g_Ini.iAxisRunMode[3] ==0)|| (ch ==5 && g_Ini.iAxisRunMode[4] ==0)|| (ch ==6 && g_Ini.iAxisRunMode[5] ==0))
		{
			INT32S lTempPos=0;
			lTempPos = GetPulse(ch,TRUE) % g_Ini.lPulse[ch-1];//�������к����ڶ�Ȧ��ҡʱ�����ö�Ȧ��ҡ��
			SetPulse(ch, lTempPos,TRUE);
		}

		//Uart_Printf("stepmode = %d,pp = %d\n",stepmode,pp);

		if(stepmode)
		{

			/*
			g_bStepMove=TRUE;



			FifoMoveMM(ch,dir?temp:-temp,Speed);
			SetQuiteRange(4000);
			OSTimeDly(1);
			SetQuiteRange(8000);

			while(g_bStepMove)
			{				
				if( 0 == ReadFifo())
					FifoMoveMM(ch,dir?temp:-temp,Speed);

				HandFunc();

				OSTimeDly(1);
			}

			ResetFifo();

			while(TRUE)
			{
				if(IsFree(ch))	
					break;			

				OSTimeDly(1);
			}		

			SetQuiteRange(8000);
			 */

			g_bStepMove=TRUE;

			AddMoveMM(ch,dir>0?100000:-100000,5.0,Speed,Speed*25);

			while(g_bStepMove)
			{				
				HandFunc();

				if(PulseLagCheck(ch))
				{
					Stop1(ch);
					OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
					break;	
				}				

				if(CheckLimit(TRUE,TRUE,ch,dir>0?1:-1))
				{	
					Stop1(ch);
					break;
				}

				OSTimeDly(1);
			}	

			Stop2(ch);
			//	Uart_Printf("stop2\n");
			//	Stop1(ch);

			while(TRUE)
			{
				if(IsFree(ch))	
					break;

				OSTimeDly(1);
			}	

			OSTimeDly(100);		
		}
		else
		{
			AddMoveMM(ch,dir?temp:-temp,1.0,Speed,Speed*25);

			while(TRUE)
			{
				if(IsFree(ch))	
					break;	

				if(PulseLagCheck(ch))
				{
					Stop1(ch);
					OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
					break;	
				}

				if(CheckLimit(TRUE,TRUE,ch,dir>0?1:-1))
					Stop1(ch);

				OSTimeDly(1);
			}
		}
	}

	DisplayXYZ(TRUE);
}


void	HandFunc(void)
{


	if(GetKey(TRUE)==0)
		g_bStepMove=FALSE;

	if( RealIO( STOPIO , g_Ini.StopLevel ,2)||(g_bHandset?RealIO(STOPIO2,0,3):0))
	{
		switch(g_Ini.iAxisCount)
		{
		case 6:Stop1(6);
		case 5:Stop1(5);
		case 4:Stop1(4);
		case 3:Stop1(3);
		case 2:Stop1(2);
		Stop1(1);
		break;
		}
	}

	OSTimeDly(1);
}



BOOL ExitScrProtect(void)          						//�������У����˳�����,����ֵΪTRUEʱ��ʾ�н����˳���������
{
	BOOL bDoExit=FALSE;

	if(g_Ini.iScrProtect == 1)         					//������ʾ 
	{
		if(!g_bLCD || g_bScrProtectRun)
		{
			if(g_Ini.iScrProtectBmp && g_bInitbmp)
			{
				g_bScrProtectRun=FALSE;
				CDC_RestoreScreen();
				ResumeTasks(5);    
				UpdateSpeedCountProbe();
			}
			else
			{
				g_bLCD = TRUE;
				//	LCD_Power(1);
				//	Lcd_EnvidOnOff(1);
				//				Lcd_PowerOnOff(1);
			}
			bDoExit=TRUE;
		}
	}

	return bDoExit;
}









