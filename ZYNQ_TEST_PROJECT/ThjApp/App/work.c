/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        work.c
  创建人：       范志州
  创建日期：		2004/11/20
  主要功能描述:  中断，加工以及数据解码的一些处理操作 

  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/04
       主要修改内容:   加工工艺控制过程
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

WORK_EXT 	INT8S		g_pszMsgBuf[0x30]="\0";									//存放信息提示内容避免用局部变量的BUG
WORK_EXT	INT8S		g_pszLedOut[8][3]={"\0","\0","\0","\0","\0","\0","\0","\0"};
WORK_EXT	INT16S 		g_iRunLine=0;		
WORK_EXT 	INT16S		g_iStaRunLine=0;	
WORK_EXT 	BOOL 	 	g_bScreen=FALSE;	   									//指示当前屏幕显示那个界面，TRUE显示主加工界面
WORK_EXT 	BOOL	  	g_bFirstDispTime=TRUE;									//指示是否需要全部刷新日期时间显示，TRUE需要
//WORK_EXT	BOOL		g_bFirstDispRunTime=FALSE;
WORK_EXT	BOOL		g_bDispRunTime=FALSE;
WORK_EXT	BOOL		g_bCanDispRunTime=FALSE;								//表示当时是否可进行显示剩余时间刷新 。


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

WORK_EXT 	INT32S		g_lStepNode=0;										//该参数现在以无意义

WORK_EXT 	BOOL		g_bStepIO = FALSE;  								//在该平台该参数表示正在测试加工，若测试打到零档，为FALSE,非零档为TRUE
WORK_EXT 	BOOL		g_bDispRunLine=FALSE;
WORK_EXT 	BOOL		g_bIsHand=FALSE;	
WORK_EXT 	BOOL		g_bStepToSingle=FALSE;
WORK_EXT 	BOOL		g_bMode=FALSE;	   
WORK_EXT	BOOL		g_bEMSTOP=FALSE; 
WORK_EXT	INT16S		g_iADValue=-2;  
WORK_EXT	BOOL		g_bTestStart=TRUE;
WORK_EXT	BOOL		g_bHandRun = FALSE;									//在该平台该参数表示正在手轮加工，若手摇停止，为FALSE,手摇时为TRUE
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
WORK_EXT	BOOL		g_bBackZero=FALSE;										//指示是否为回零过程
WORK_EXT	INT16S		g_iErrorNo=0; 
WORK_EXT	BOOL		g_bCylinder=FALSE; 										//指示是否处于气缸测试状态
WORK_EXT	BOOL		g_bScrProtectRun=FALSE;
WORK_EXT 	INT16S		g_iWorkNumber 	= 1;
WORK_EXT 	INT16S		g_iSpeedBeilv 	= 1;
WORK_EXT 	FP32		g_fStepSpeed[4]={0.1,0.3,0.6,1.0};
WORK_EXT 	INT32S  	g_lXmsSize=0;      
WORK_EXT	INT32S		g_lDataCopyXmsSize=0;
WORK_EXT 	INT32S		g_lYCountLen=0;       									//加工过程中Y轴的补偿量
WORK_EXT 	INT32S		g_lZCountLen=0;      									//加工过程中Y轴的补偿量
WORK_EXT	FP32 		g_fCloseDispCount=0.0;
WORK_EXT	INT32S		g_ulTiCount=0;
WORK_EXT	INT16U		g_uiSuspendTask=0xFF1E;									//suspend 1~5 task,high byte rev
//WORK_EXT	INT32S		g_lAxisValue[CHMAX]={0,0,0,0};							//记录各轴关断之前的位置
WORK_EXT 	INT32S		g_lProbeFail=0; 										//记录探针失败   
WORK_EXT	INT16S		g_iCheckStatusIO=0;
WORK_EXT 	INT32S  	g_lRamRunCounts=0;										//铁电保存当前产量  

//20130815
WORK_EXT	BOOL		g_bTestToSingle=FALSE;
WORK_EXT    BOOL 		g_bWheel    = FALSE;									//手轮按键加工
WORK_EXT    BOOL      	g_bStepMode = FALSE;									//单行模式
WORK_EXT	BOOL		g_bFunc 	= FALSE;									//气缸
WORK_EXT	INT16S		g_iTCmdPos 	= 0;										//设定几号探针
WORK_EXT	INT32S		g_lInputPos[4]={0,0,0,0};
WORK_EXT	BOOL		g_bUseTimes = FALSE;
WORK_EXT	INT16S		g_iStatus=0;
WORK_EXT    INT16S      g_iToolBarPage = 0;
WORK_EXT	INT16U 		g_uiMoveKey=0;
//WORK_EXT    BOOL        g_bScreenSaved=FALSE;
WORK_EXT	INT32S 		g_bInitbmp;
WORK_EXT	INT16S 		g_ADTemp;
WORK_EXT    BOOL   		WorkErr;

OUTDATA 	OutData[100];									//存放汽缸数组
INT32S  	g_lOutSize;										//记录有汽缸的次数
INT32S		lOutFlag[100]={0};								//记录汔缸被打开过标识


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


INT32S 			lYProbelong =0;						//打到探针后的差值 
INT32S 			lYProbelong1 =0;					//总的打到探针后的差值

INT32S			g_lRunLintCount = 0;

int 		lTagCount = 0;
int 		lTagLastCount=0;						//缓存弹簧条数计数
extern 	BOOL  g_bInterface;

TEMPOUTDATA   	OutDataAdd[20]={0};					//记录一行的气缸操作次数，并排序控制
INT32S  		lOutSizeAdd=0;						//记录有汽缸的次数
INT32S			g_lProbeAngle = -888888;			//碰触探针显示角度

BOOL			g_bLagCheck = TRUE;				    //是否开启滞后量检测功能，在探针行关闭滞后量检测
extern		INT16S		g_iCurEditNo;

extern      INT32S      g_iEmpNode[MAXCH];	



//INT32S			g_lYplusLong = 0;					//是否探针失败，探针失败后记录J指令行到跳转行之间所有行解析的Y轴所跑的脉冲
//INT16S			g_itemplNode = 0;					//记录探针失败后跳转到当行点

INT32S SrotTempOutData(INT32S dir,INT32S axis)			
{
	INT32S  i=0,k=0;
	TEMPOUTDATA Temp;

	if(dir == 0)					//从小到大排序
	{
		for(i=0;i<lOutSizeAdd-1;i++)
		{
			for(k=0;k<lOutSizeAdd-1;k++)
			{
				if(OutDataAdd[k].outPos[axis]>OutDataAdd[k+1].outPos[axis])	//最大的放后面
				{
					memcpy(&Temp,&OutDataAdd[k],sizeof(Temp));

					memcpy(&OutDataAdd[k],&OutDataAdd[k+1],sizeof(Temp));

					memcpy(&OutDataAdd[k+1],&Temp,sizeof(Temp));
				}
			}
		}
	}
	else 							//从大到小
	{
		for(i=0;i<lOutSizeAdd-1;i++)
		{
			for(k=0;k<lOutSizeAdd-1;k++)
			{
				if(OutDataAdd[k].outPos[axis]<OutDataAdd[k+1].outPos[axis])	//最小的放后面
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


INT32S GetYLong(INT32S index)						//用于查找并返回索引行之前的不同的Y值，用于Y座标显示
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

					if(lYTemp != pData.lValue[1] && lYTempIo==0)				//当，当前点的Y值与前一个点的Y值不一样时，清除探针后的差值
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

			ltoa(curPos[0]%3600,(INT8U *)buffer);

			/*if(strlen(buffer)<8)
			{
				for(i=strlen(buffer);i<9;i++)
					buffer[i]=' ';
			}*/

			if(LastcurPos[0] != curPos[0])
			{

				CDC_OutTextLR_UTF(80,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
				//CDC_OutTextLR_UTF(70,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,5,1);

				LastcurPos[0] = curPos[0];
			}

		}
		else
		{
			//CDC_OutText_UTF(80,87,"OFF   ",REDCOLOR,g_nTechfrmBk,MZK_32);
		}

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


			//在 g_iWorkStatus!=RUNNING 情况下，进入坐标更新显示			
			//或者在g_iWorkStatus==RUNNING 情况下，且(lNowPos != s_lPrePos || curPos[1] == 0)情况下，进入坐标更新显示
			if(lNowPos != s_lPrePos || curPos[1] == 0 || g_iWorkStatus!=RUNNING)						
			{

				//此处处理只针对加工状态下，是为了避免坐标显示出现错误，一旦出现坐标正负值错误，显示为0
				if((curPos[1]<0 && lNowPos>s_lPrePos || curPos[1]>0 && lNowPos<s_lPrePos) && g_iWorkStatus==RUNNING)
				{
					curPos[1] =0;
				}


				s_lPrePos =lNowPos;


				ltoa(curPos[1],(INT8U *)buffer);

				if(LastcurPos[1] != curPos[1])
				{
					CDC_OutTextLR_UTF(200,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
					//CDC_OutTextLR_UTF(242,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
					LastcurPos[1] = curPos[1];
				}
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

					ltoa(curPos[2], (INT8U *)buffer);
				}
				else if(g_Ini.iAxisRunMode[2] == 1)
				{
					curPos[2] = (INT32S)(GetMM(3,cur)*10);

					ZQuanPos = curPos[2]/3600;
					ZAnglePos = curPos[2]%3600;

					if(curPos[2]>=0)
						sprintf(buffer,"%dC%04d",ZQuanPos,ZAnglePos);
					else
						sprintf(buffer,"-%dC%04d",abs(ZQuanPos),abs(ZAnglePos));
				}
				else  if(g_Ini.iAxisRunMode[2] == 2)
				{
					curPos[2] = (INT32S)(GetMM(3,cur)*100);

					ltoa(curPos[2], (INT8U *)buffer);
				}

				/*if(strlen(buffer)<13)
				{
					for(i=strlen(buffer);i<13;i++)
						buffer[i]=' ';
				}*/

				if(LastcurPos[2] != curPos[2])
				{	
					CDC_OutTextLR_UTF(320,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
					//CDC_OutTextLR_UTF(412,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
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
					ltoa(curPos[3], (INT8U *)buffer);
				}
				else if(g_Ini.iAxisRunMode[3] == 1)
				{
					curPos[3] = (INT32S)(GetMM(4,cur)*10);

					UQuanPos = curPos[3]/3600;
					UAnglePos = curPos[3]%3600;

					if(curPos[3]>=0)
						sprintf(buffer,"%dC%04d",UQuanPos,UAnglePos);
					else
						sprintf(buffer,"-%dC%04d",abs(UQuanPos),abs(UAnglePos));
				}
				else if(g_Ini.iAxisRunMode[3] == 2)
				{
					curPos[3] = (INT32S)(GetMM(4,cur)*100);

					ltoa(curPos[3], (INT8U *)buffer);
				}

				//ltoa(curPos[3], (INT8U *)buffer);

				/*if(strlen(buffer)<13)
				{
					for(i=strlen(buffer);i<13;i++)
						buffer[i]=' ';
				}*/

				if(LastcurPos[3] != curPos[3])
				{	
					CDC_OutTextLR_UTF(440,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
					//CDC_OutTextLR_UTF(582,getmaxy()-93,buffer,g_TextColor11,g_nBk,MZK_24,8,1);
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



		{															//坐标保存到铁电
			static 	INT32S sTempPos[MAXCH]={0,0,0,0,0,0};
			INT32S	cmdpos;  
			INT16S	i=0;	

			for(i=0;i<g_Ini.iAxisCount;i++)
			{
				if(i==1)											//送线轴不保存
					continue;

				cmdpos = GetPulse(i+1,TRUE);

				if(sTempPos[i]!= cmdpos)
				{
					sTempPos[i] = cmdpos;
					WritePosToRam(i+1, sTempPos[i]);
				}
			}
		}

	}
	else
	{
		INT16S	i=0;	

		for(i=0;i<g_Ini.iAxisCount;i++)
		{
			LastcurPos[i]=-1;

			if( g_Ini.iAxisSwitch[i]==0 )	
				//CDC_OutTextLR_UTF(70+i*170,getmaxy()-93," OFF",REDCOLOR,g_nTechfrmBk,MZK_24,8,1);
				CDC_OutTextLR_UTF(80+i*120,getmaxy()-93,"OFF",REDCOLOR,g_nTechfrmBk,MZK_24,8,1);
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

	BOOL  		bProbeFirstRun =  TRUE; 								//判断加工第一条带有探针弹簧时，按单条时不能直接跳到AUTORUNEND
	INT16S		iHaveTag = 0;											//用于代替bProbeFirstRun，增加扩展性
	INT32S 		itempRunCount =g_Ini.lRunCount;


	INT16U		nLastLine=0;
	INT8U		nLastOut[MAXCYLINDER]={2,2,2,2,2,2,2,2};
	INT32S		lCurPulse[MAXCH] ;	

	SetQuiteRange(8000);


	////*********启动前先检查是否归完零  和报警输入检测*****************////
	if(g_bRelease)
	{
		if(g_bEMSTOP)
		{
			OSQPost(MsgQueue,g_bEnglish?"Start is invalid after emergency stop":"紧急停止后请先归零!");
			return 1;
		}

		if( !g_bFirstResetX  )
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Reset X!!!":"X先归零!!!");
			return 1;
		}

		switch(g_Ini.iAxisCount)
		{
		case 6:
			if(g_Ini.iAxisSwitch[5]==1)
			{
				if( !g_bFirstResetW  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset C!!!":"C轴先归零 ...!!!");
					return 1;
				}
			}

		case 5:
			if(g_Ini.iAxisSwitch[4]==1)
			{
				if( !g_bFirstResetV  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset B!!!":"B轴先归零 ...!!!");
					return 1;
				}
			}

		case 4:
			if(g_Ini.iAxisSwitch[3]==1)
			{
				if( !g_bFirstResetU  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset A!!!":"A先归零 ...!!!");
					return 1;
				}
			}

		case 3:
			if(g_Ini.iAxisSwitch[2]==1)
			{
				if( !g_bFirstResetZ  )
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Reset Z!!!":"Z先归零 ...!!!");
					return 1;
				}
			}
			break;

		default:
			break;
		}



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
		}

		if(g_Ini.iAxisSwitch[1]==1)								//送线位置清零
		{
			if(!g_bFirstResetY)
			{
				SetMM(2,0,TRUE);
				g_bFirstResetY=TRUE;
				g_lYCountLen=0;
			}
		}
	}

	if( !g_bUnpackCorrect||WorkErr )
	{
		OSQPost(MsgQueue,g_bEnglish?"Data Invalid!":"当前所编程序有错误!");
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

	UpdateSpeedCountProbe();									//刷新探针

	//	if(g_lStepNode==0)															//g_lStepNode该标识现在以无用了，在因为现在手轮加工不会退出加工过程。
	{															//启动前若以归过机械原点，但现在不在零点需回零点
		//INT32S temp = (GetPulse(1,TRUE)%g_Ini.lPulse[0]+g_Ini.lPulse[0])%g_Ini.lPulse[0];

		//if(temp!=g_Ini.lPulse[0] && GetPulse(1,TRUE)!=0)

		if(g_Ini.iBackMode[0]!=4)	//需要回零
		{
			if(GetPulse(1,TRUE)!=0)
			{
				if(!WorkFindZero(1))
					return 1;
			}
		}

		OSTimeDly(10);

		SetMM(2,0,TRUE);
		g_bFirstResetY=TRUE;

		OSTimeDly(10);

		if(g_Ini.iAxisCount>2 && g_Ini.iAxisSwitch[2]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(3,TRUE);

			if(g_Ini.iBackMode[2]!=4  || g_Ini.iAxisRunMode[2]==1)	//多圈轴都要先回零,即使错误设置成不回零
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
		}		

		if(g_Ini.iAxisCount>4 && g_Ini.iAxisSwitch[4]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(5,TRUE);

			if(g_Ini.iBackMode[4]!=4 || g_Ini.iAxisRunMode[4]==1)	
			{
				if(lTemp!=0)
				{
					if(!WorkFindZero(5))
						return 1;
				}
			}

			OSTimeDly(10);
		}		

		if(g_Ini.iAxisCount>5 && g_Ini.iAxisSwitch[5]==1)
		{
			INT32S lTemp = 0;

			lTemp = GetPulse(6,TRUE);

			if(g_Ini.iBackMode[5]!=4 || g_Ini.iAxisRunMode[5]==1)	
			{
				if(lTemp!=0)
				{
					if(!WorkFindZero(6))
						return 1;
				}
			}

			OSTimeDly(10);
		}		

	}	

	g_lRunLintCount = g_Ini.lRunCount;

	if(g_bStepIO)
	{
		CTeach_UpdateStatus(STATUS_TEST);
		OSQPost(MsgQueue,g_bEnglish?"Start TestRun...":"开始测试运行...");
		LedOut(TESTLED,1);
	}
	else if(g_bHandRun)
	{
		CTeach_UpdateStatus(STATUS_TEST);
		OSQPost(MsgQueue,g_bEnglish?"Start Handwheel Run...":"开始手摇运行...");
	}
	else
	{
		CTeach_UpdateStatus(STATUS_AUTO);
		//CTeach_GotoFristline();
		OSQPost(MsgQueue,g_bEnglish?"Start AutoRun...":"开始自动加工...");
	}

	//	OSTimeDly(100);

	//	if(g_bAutoRunMode&&!g_bMode)
	{
		if(g_Ini.iDispLeavingTime==1&&g_Ini.lDestCount>0)
		{
			//	CDC_SaveScreen();
			//	g_bScreenSaved=TRUE;
			//	g_bFirstDispRunTime=TRUE;
			g_bDispRunTime=FALSE;								//启动时初始化表还未显示剩余时间

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

	{															//开启其它检测任务
		INT8S a = 0;

		for(a=0; a<5; a++)
			ResumeTasks(a);

		OSTimeDly(50);
	}

	LedOut(STOPLED,0);

	//	if(g_bAutoRunMode && !g_bMode)								//开自动加工模式下
	CloseDisplay(0);

	if(!g_bMode)
		if(g_Ini.iAxisSwitch[1]==1)
			WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);	


	g_iMoveAxis = g_Ini.iAxisCount*100; 						//zhi shi wei inp

	vm_sync();													//同步，清空缓存

	g_lProbeAngle = -888888;									//角度初始化

	ti1 = OSTimeGet();

	for(k=0;k<MAXCH;k++)
		lCurPulse[k] =GetPulse(k+1,TRUE);

	do
	{
		//*******数据拷贝到加工数组********//
		if(!g_bDataCopy && g_lStepNode==0)
		{
			if(!DataCopy())
			{
				OSQPost(MsgQueue,g_bEnglish?"Data analysis error!":"数据拷贝解码错误,请确认所编程序正确无误!");
				g_iMoveAxis = 0;
				//return 0;
				goto   WHILEEND;
			}	
		}

		lNode = 0;
		WorkEnd = FALSE;
		ProbeCount = 0;

		if(g_bStepMode==TRUE)									//单行模式
		{
			g_bAutoRunMode = FALSE;
		}

		/*{														//初始化汽缸操作标识
			INT32S k=0;

			for(k=0;k<100;k++)
				lOutFlag[k]=0;
		}*/

		for(; lNode<g_lDataCopyXmsSize;)						//加工一条弹簧的数据发送
		{
			XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA));
			lNode++;			

			switch(pData.nCode)
			{
			case SPEED:										//速度指令
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
						if( pData.nIO) 												//当行解释到探针行在插补前将探针打开放入缓存队列里
						{
							g_iCheckStatusIO =  pData.nIO;
							vm_set_probe(g_iCheckStatusIO,2);
						}*/

				{

					INT8U	AxisEnable =0;					//用于标识插补的开关轴

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

					//插补解析
					x = pData.lValue[0];
					y = pData.lValue[1];
					z = pData.lValue[2];
					a = pData.lValue[3];
					b = pData.lValue[4];
					c = pData.lValue[5];



					for(k=0;k<MAXCH;k++)   //不回零模式中，在有数据的空行之前都保持加工结束时的轴位置；
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


					if(movespeed>VM.SpdMax[pData.SpindleAxis-1]*1000)		//主轴限速功能xxl20141127
						movespeed=VM.SpdMax[pData.SpindleAxis-1]*1000;

					if(movespeed<1)
						movespeed=1;

Uart_Printf("vm_getindex()==%d  速度 ==%d\r\n",vm_getindex(),pData.lValue[6]);
					/*
							{														//必须在解释代码里做处理不然速度和主轴会出现错误，
								if(g_Ini.iAxisSwitch[1]==0)							//由于现在走的是绝对位置所以为零表示走到零关轴时不在零点时，加工时会走到零点产生误动作。
									y = 0;

								if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
									z = 0;

								if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
									a = 0;
							}
					 */

					if(vm_abs_inp_move(pData.nLine,AxisEnable,pData.SpindleAxis-1,x,y,z,a,b,c,movespeed) == -1)
					{
						Uart_Printf("vm_abs_inp_move error\n");
						goto WHILEEND;
					}



					for(l=0;l<MAXCYLINDER;l++)  							//汽缸动作  在开始角到结束角之前执行
					{
						//ps_debugout("气缸 %d  nOut = %d\r\n ",l,pData.nOut[l]);
						if(2 !=pData.nOut[l])
						{
							vm_set_io(l,pData.nOut[l]);

						}
					}

					if(pData.nReset)										//轴清零标志
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

				//ps_debugout("pData.nIO = %d g_Ini.iWaitSignal = %d\r\n",pData.nIO,g_Ini.iWaitSignal);
				if( pData.nIO&&(pData.nIO!=g_Ini.iWaitSignal)) 												//当发送到探针行时停止发送缓存的数据，并等待探针信号
				{

					BOOL 	bFirstProbe = TRUE;
					//							g_iCheckStatusIO = pData.nIO;
					g_iCheckStatusIO = g_Ini.iSetProNum[pData.nIO-1];
					vm_start();

					while(TRUE)
					{

						if(pData.nLine-1 == vm_getindex())
						{
							g_bLagCheck = FALSE;
						}

						if(pData.nLine == vm_getindex())					//等待执行缓存到探针行。注：探针行的行号与开始角行号一致。
						{
							lYProbelong = 0;								//每次进探针行时要把前一探针的偏差清零
							g_lProbeAngle = -888888;						//角度初始化

							if(bFirstProbe)									//打开探针,并首次检查探针状态若有信号探针出错
							{
								INT16S flag = 0;

								bFirstProbe = FALSE;

								OpenProbe(0,g_iCheckStatusIO,2);			//首次检测探针时打开探针

								flag = ReadProbe(0,g_iCheckStatusIO);

								if(flag ==1)								//表示缓存中探针以打开，并首次就检测到探针信号
								{
									//	Uart_Printf("testpoint  探针前触碰失败 \n");

									g_lProbeFail++;							//探针前触碰失败
									vm_stop(1);								//加清缓存和停止代码

									if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针失败需关闭同一行的气缸
									{
										for(l=0;l<MAXCYLINDER;l++)
										{
											if(1==nLastOut[l])
											{
												WriteBit(l,0);
											}
										}
									}

									lYProbelong = pData.lValue[1] - GetPulse(2,TRUE);

									lYProbelong1 +=lYProbelong;

									SetPulse(2,pData.lValue[1],TRUE);		//因为送线绝对坐标，探针失败直接设置探针行的Y轴进去

									vm_sync();								//同步，清空缓存

									CloseProbe(0);

									//if(g_Ini.lProbeCount ==1)
									if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
									{
										g_iErrorNo=15;						//表探针一直触碰失败
										OSTimeDly(30);
										SetQuiteRange(0);

										WriteBit(O_SENDADD,0);

										ExitScrProtect();
										CTeach_UpdateStatus(STATUS_PAUSE);

										OSQPost(MsgQueue,g_bEnglish?"Probe Fail for Touching Throughout!":"探针一直触碰失败!");

										while(TRUE)
										{
											if(g_iErrorNo != 0 && g_iErrorNo!=15 && g_iErrorNo!=3  && g_iErrorNo!=2 && g_iErrorNo!=4)
											{
												Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
												goto   WHILEEND;
											}

											if(g_Ini.iWarningOut==1)
											{
												if(GetOut(O_WARNING)==0)
													WriteBit(O_WARNING,OPEN);
											}

											if(g_iErrorNo == 0)
											{
												if(g_Ini.iWarningOut==1)
												{
													WriteBit(O_WARNING,CLOSE);
												}
												break;
											}

											OSTimeDly(1);
										}
									}

									//*****************条件跳转J指令*********************//

									XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA)); //检测下一行是否是条件跳转J指令
									lNode++;

									if(pData.nCode != JUMP)
									{
										lNode--;
									}
									else
									{
										INT16S temp=pData.lValue[0]-1;
										INT32S YPrelValue =0;
										INT32S PrenLine=0;

										lNode=0;
										temp = temp*2;

										while(1)
										{
											XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA));
											lNode++;

											if(lNode>=g_lDataCopyXmsSize)
											{
												g_iErrorNo = 18;
												g_iWorkStatus=NORUN;
												goto   WHILEEND;
											}

											if(temp == pData.nLine+1 || temp == pData.nLine)
											{
												lNode--;

												lYProbelong = YPrelValue - GetPulseFromMM(2,GetYLong(PrenLine))/100.0;

												SetPulse(2,YPrelValue,TRUE);		//跳转到该行后，由于Y轴是绝对表示要把Y轴的位置设定进去
												vm_sync();								//同步，清空缓存

												break;
											}
											PrenLine = pData.nLine;
											YPrelValue=pData.lValue[1];
										}
									}

									if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
									{
										g_bAutoRunMode=FALSE;
									}

									break;												//退出检测段执行下一行
								}
							}
							else														//检测到探针信号
							{
								INT16S flag = 0;

								flag = ReadProbe(0,g_iCheckStatusIO);

								if(flag ==1 )				//探针成功
								{

									//	Uart_Printf("testpoint  探针成功1 \n");

									vm_stop(1);				//加清缓存和停止代码

									if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针成功需关闭同一行的气缸
									{
										for(l=0;l<MAXCYLINDER;l++)
										{
											if(1==nLastOut[l])
											{
												WriteBit(l,0);
											}
										}
									}

									lYProbelong = pData.lValue[1] - GetPulse(2,TRUE);
									lYProbelong1+=lYProbelong;

									SetPulse(2,pData.lValue[1],TRUE);

									vm_sync();				//同步，清空缓存

									CloseProbe(0);

									//	Uart_Printf("testpoint  探针成功2 \n");

									break;					//退出检测段执行下一行?
								}
							}
						}

						if (pMQ->QueueCount==0 && ReadFifo()==0 && IsInpEnd(1))			//若探针行执行完。
						{

							INT16S flag = 0;

							OSTimeDly(3);					//作为探针后延时

							flag = ReadProbe(0,g_iCheckStatusIO);

							if(flag==1)						//探针成功
							{
								if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针成功需关闭同一行的气缸
								{
									for(l=0;l<MAXCYLINDER;l++)
									{
										if(1==nLastOut[l])
										{
											WriteBit(l,0);
										}
									}
								}

								//	vm_stop();					//加清缓存和停止代码
								vm_sync();					//同步，清空缓存
								CloseProbe(0);
								break;						//退出检测段执行下一行?
							}
							else
							{
								//Uart_Printf("testpoint  探针未触碰失败1!\n");

								if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针失败需关闭同一行的气缸
								{
									for(l=0;l<MAXCYLINDER;l++)
									{
										if(1==nLastOut[l])
										{
											WriteBit(l,0);
										}
									}
								}

								g_lProbeFail++;				//探针未触碰失败

								//	vm_stop();					//加清缓存和停止代码
								vm_sync();					//同步，清空缓存
								CloseProbe(0);

								//if(g_Ini.lProbeCount ==1)
								if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_iErrorNo=14;						//表探针未触碰失败
									OSTimeDly(30);
									SetQuiteRange(0);

									WriteBit(O_SENDADD,0);

									ExitScrProtect();
									CTeach_UpdateStatus(STATUS_PAUSE);

									OSQPost(MsgQueue,g_bEnglish?"Probe Fail for no Touch!":"探针未触碰失败!\n");

									while(TRUE)
									{
										if(g_iErrorNo != 0 && g_iErrorNo!=14 && g_iErrorNo!=3 && g_iErrorNo!=2 && g_iErrorNo!=4)
										{
											//				Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
											goto   WHILEEND;
										}

										if(g_Ini.iWarningOut==1)
										{
											if(GetOut(O_WARNING)==0)
												WriteBit(O_WARNING,OPEN);
										}

										if(g_iErrorNo == 0)
										{
											if(g_Ini.iWarningOut==1)
											{
												WriteBit(O_WARNING,CLOSE);
											}
											break;
										}

										OSTimeDly(1);
									}
								}


								//*****************条件跳转J指令*********************//

								XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA)); //检测下一行是否是条件跳转J指令
								lNode++;

								if(pData.nCode != JUMP)
								{
									lNode--;
								}
								else
								{
									INT16S temp=pData.lValue[0]-1;
									INT32S YPrelValue =0;
									INT32S PrenLine=0;

									lNode=0;
									temp = temp*2;

									while(1)
									{
										XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA));
										lNode++;

										if(lNode>=g_lDataCopyXmsSize)
										{
											g_iErrorNo = 18;
											g_iWorkStatus=NORUN;
											goto   WHILEEND;
										}

										if(temp == pData.nLine+1 || temp == pData.nLine)
										{
											lNode--;

											lYProbelong = YPrelValue - GetPulseFromMM(2,GetYLong(PrenLine))/100.0;

											SetPulse(2,YPrelValue,TRUE);		//跳转到该行后，由于Y轴是绝对表示要把Y轴的位置设定进去
											vm_sync();								//同步，清空缓存

											break;
										}

										PrenLine = pData.nLine;
										YPrelValue=pData.lValue[1];
									}
								}

								if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_bAutoRunMode=FALSE;
								}

								break;						//退出检测段执行下一行?
							}

						}

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

							if(g_Ini.lDestCount>0 && g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
						}

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
						{

							if(bFirstProbe == FALSE)
							{
								if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针失败需关闭同一行的气缸
								{
									for(l=0;l<MAXCYLINDER;l++)
									{
										if(1==nLastOut[l])
										{
											WriteBit(l,0);
										}
									}
								}

								CloseProbe(0);
								Uart_Printf("vm_get_tag error! very improtion\n");
							}
							goto  AUTORUNEND;
						}

						OSTimeDly(1);

						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4)
						{
							//	Uart_Printf("g_iErrorNo102 = %d\n",g_iErrorNo);

							if(bFirstProbe == FALSE)
								CloseProbe(0);

							goto   WHILEEND;
						}

					} // end while(TRUE)


					SetCommandPos(2,0);				 //探针检测后清除位置，便于脉冲滞后检测
					SetActualPos(2,0);
					g_bLagCheck = TRUE;

				} //end if( pData.nIO) 探针处理


				for(l=0;l<MAXCYLINDER;l++)
				{
					nLastOut[l]=pData.nOut[l];
				}

				nLastLine=pData.nLine;

				if(pData.fDelay-0.0>0.000001)					//延时
				{
					INT32U tt2=0;
					INT32U tt1=0;

					vm_start();

					while(TRUE)					//等待缓存数据发完再开始延时
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

							if(g_Ini.lDestCount>0 && g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
						}

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && bProbeFirstRun==FALSE)
							if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
							{
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

					vm_sync();								//同步，清空缓存

					tt1=OSTimeGet();

					while(TRUE)
					{
						tt2=OSTimeGet();

						if((tt2-tt1) >= (INT32U)(pData.fDelay*1000.0))
						{
							break;
						}

						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 )
						{
							goto   WHILEEND;
						}

						OSTimeDly(1);
					}
				}// END 	if(FALSE && pData.fDelay-0.0>0.000001)	//延时

				if(pData.nIO==g_Ini.iWaitSignal&&g_Ini.iWaitSignal)
				{
					INT32U tt2=0;
					INT32U tt1=0;
					INT8S  IO[8]={4,3,9,8,14,13,19,18};

					vm_start();						//启动执行缓存队列

					while(TRUE)					  //等待缓存数据发完再开始延时
					{

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{
							//	g_lProbeAngle = -888888;
							//	bProbeFirstRun = TRUE;

							CurIndex = 0;
							lYProbelong = 0;
							lYProbelong1 = 0;


							iHaveTag--;

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
							goto  AUTORUNEND;
						}


						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8 &&g_iErrorNo!=9 && g_iErrorNo!=10)
						{
							goto   WHILEEND;
						}

						if(!vm_get_status())
							break;

						OSTimeDly(1);
					}
					vm_sync();												//同步，清空缓存
					tt1=OSTimeGet();

					while(TRUE)
					{//输入等待实际执行区域
						tt2=OSTimeGet();

						if(RealIO(IO[pData.nIO-1],0,2))
						{
							break;
						}

						if((tt2-tt1)*1000.0/OS_TICKS_PER_SEC>g_Ini.iProDelay)//读不到指定输入则错误退出
								{
							//Uart_Printf("g_lProbeFail = %d!\n",g_lProbeFail);
							OSSemPost(RunSpeedCountSem);
							g_iErrorNo=51;
							goto WHILEEND;
								}
						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8 && g_iErrorNo!=9 && g_iErrorNo!=10 )
							goto WHILEEND;
						OSTimeDly(1);
					}
					break;
				}

				if(pMQ->QueueCount>8)												//增加对插补缓存空间在应用层进行限制
				{

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


							if(g_Ini.lDestCount>0 && g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
						}

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  iHaveTag>0)
						{
							Uart_Printf("4iHaveTag=%d\n",iHaveTag);
							goto  AUTORUNEND;
						}

						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 4 )
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

			case PEND: 									//结束指令
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


				if(g_bDataCopy==FALSE ||g_bAutoRunMode==FALSE)
				{

					Uart_Printf("testpoint AUTORUNEND3\n");
					goto AUTORUNEND;
				}

			}
			break;

			default:
				break;
			}

		}//end for(; lNode<g_lDataCopyXmsSize;)

		AUTORUNEND:
		if(g_bAutoRunMode==FALSE || g_bDataCopy==FALSE )
		{

			vm_start();

			vm_tag_stop();

			//	Uart_Printf("vm_get_tag1=%ld\n",vm_get_tag());

			while(vm_get_status())
			{
				OSTimeDly(1);

				if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4)
				{
					Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
					goto   WHILEEND;
				}

			}

			//	Uart_Printf("vm_get_tag2=%ld\n",vm_get_tag());

			itempRunCount = g_Ini.lRunCount=vm_get_tag();
			lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;

			bProbeFirstRun =  TRUE;
			iHaveTag=0;

			{
				INT32S temp = 0;

				temp = GetPulse(1,TRUE)%g_Ini.lPulse[0];		//x

				if(temp ==0)
					SetMM(1,0,TRUE);

				//Uart_Printf("tempx=%ld\n",temp);

				SetMM(2,0,TRUE);								//y

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

			vm_sync();					//同步，清空缓存

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
	g_ADTemp = 0;//确保每次退出  都要进行清零
	RunEnd();

	return TRUE;
}



void	RunEnd()
{

	ExitScrProtect();

	if(g_iCurEditNo!=9)											//当前的编辑点不在列表时  为9为在列表编辑状态
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

	if(g_bMode)
	{
		g_bMode=FALSE;
		LedOut(TESTLED,0);
	}

	if(g_Ini.iHandStatus != 0)
	{
		g_Ini.iHandStatus=0;
		LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
		CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
	}

	if(g_bHandset)
	{
		g_bHandset = FALSE;
		LedOut(STEPLED,g_bHandset?1:0);
		CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOff  ":"手盒关",g_TextColor5,g_nBk,KZK_22);
	}

	LedOut(STOPLED,1);

	if(g_Ini.lRunCount <= g_Ini.lDestCount)						//加工结束提示
	{
		if(g_bStepIO)
		{
			OSQPost(MsgQueue,g_bEnglish?"TestRun Stop":"测试加工结束!");
		}
		else if(g_bHandRun)
		{
			OSQPost(MsgQueue,g_bEnglish?"HandRateRun Stop":"手摇加工结束!");
		}
		else
		{
			OSQPost(MsgQueue,g_bEnglish?"AutoRun Stop":"自动加工结束!");
		}
	}


	if(g_Ini.lDestCount>0 && g_Ini.lRunCount>=g_Ini.lDestCount)						//加工完成提示
	{
		INT16U key =0;

		sprintf(g_pszMsgBuf,g_bEnglish?" %d  Amount fill!<ENTER>CLR CNT!":"加工数量已完成\n %d 个!按<输入键>产量清零!",g_Ini.lRunCount);

		OSQPost(MsgQueue,g_pszMsgBuf);

		WriteBit(O_WARNING,0);

		while(TRUE)
		{
			key = KeyMBoxGetKey();

			if(key != 0xffff)
			{
				if(key==TENTER)
				{
					g_Ini.lRunCount=0;
					UpdateSpeedCountProbe();
					break;
				}
				else
					break;
			}

			if(RealIO(STOPIO,0,2))
				break;

			OSTimeDly(1);
		}

		OSQPost(MsgQueue," ");

		//g_Ini.lRunCount=0;
		WriteRunCount();
		WriteDataToFile(g_Ini.iFileName);
		SysParaWrite();

	}

	if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)  //探针失败提示
	{
		OSQPost(MsgQueue,g_bEnglish?"ProbeFail Reaches Setting!":"探针失败次数到达!");
		g_lProbeFail = 0;
	}


	{															//报警提示
		if((g_iErrorNo>=1 && g_iErrorNo<=49))
		{
			if(g_iErrorNo>6 && g_iErrorNo<=12)
			{
				g_bFirstResetX=FALSE;							//为TRUE时需要复位
				g_bFirstResetY=FALSE;
				g_bFirstResetZ=FALSE;
				g_bFirstResetU=FALSE;
				g_bFirstResetV=FALSE;
				g_bFirstResetW=FALSE;

				BackZeroLed();
			}
			else
			{
				if(g_iErrorNo==1)
				{
					g_bEMSTOP=TRUE;
					g_lStepNode=0;

					if(g_bMode)
					{
						g_bStepIO=FALSE;
						g_bDispRunLine=FALSE;
						g_bAutoRunMode=TRUE;
						//CDC_OutText24(563,406,g_bEnglish?"Auto":"自动",g_TextColor5,g_nBk);
						g_bMode=FALSE;
						LedOut(TESTLED,g_bMode?1:0);							//灯亮表示进入测试模式

					}

					/*if(g_Ini.iAutoBackZero==0)
					{
						g_bFirstResetX=FALSE;
						g_bFirstResetY=FALSE;
						g_bFirstResetZ=FALSE;
						g_bFirstResetU=FALSE;
					}*/
				}
			}

			if(g_Ini.iWarningOut==1)
			{
				if(GetOut(O_WARNING)==0)
					WriteBit(O_WARNING,OPEN);
			}
		}

		switch(g_iErrorNo)
		{

		case 1:
		{
			INT16S l=0;

			for(l=0; l<12; l++)
			{
				if(l==O_WARNING)
				{
					if(g_Ini.iWarningOut==1)
						continue;
				}

				WriteBit(l,0);
			}

			CloseProbe(0);

		}

		OSQPost(MsgQueue,g_bEnglish?"Stop Warning!":"急停报警!!!");
		break;

		case 2:	OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning!":"断线报警!!!");			break;
		case 3:	OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning!":"缠线报警!!!");		break;
		case 4:	OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning!":"跑线报警!!!");				break;


		case 7:	OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;
		case 8:	OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;
		case 9:	OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;
		case 10:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;
		case 11:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;
		case 12:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Servo Warning!":"伺服报警!!!",TRUE));			break;


		case 14:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for no Touch!":"探针未触碰!!!");					break;
		case 15:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for Touching Throughout!":"探针一直触碰!!!");		break;
		case 18:OSQPost(MsgQueue,g_bEnglish?"Command J Error!":"J跳转位置错!!!");							break;

		case 20:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 21:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;
		case 22:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 23:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;
		case 24:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 25:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;
		case 26:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 27:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;
		case 28:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 29:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;
		case 30:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Negative Hard Limit Warning!!!":"硬件负限位报警!!!",TRUE));							break;
		case 31:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Positive Hard Limit Warning!!!":"硬件正限位报警!!!",TRUE));							break;

		case 32:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 33:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;
		case 34:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 35:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;
		case 36:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 37:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;
		case 38:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 39:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;
		case 40:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 41:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;
		case 42:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Negative Soft Limit Warning!!!":"软负限位报警!!!",TRUE));							break;
		case 43:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Positive Soft Limit Warning!!!":"软正限位报警!!!",TRUE));							break;

		case 44:OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 45:OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 46:OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 47:OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 48:OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 49:OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Lag Warning!":"滞后报警!!!",TRUE));					break;
		case 50:OSQPost(MsgQueue,g_bEnglish?"system error! check data!":"速度异常错误,是否位置差异过大或脉冲当量过大!"); break;
		case 51:OSQPost(MsgQueue,g_bEnglish?"Waiting timeout! ":"等待输入信号超时报警!"); break;
		default:
			break;
		}
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

			OSQPost(MsgQueue,g_bEnglish?"System Ready.":"系统准备中");

		}
	}

	OSTimeDly(500);

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
			CDC_DrawWin(180+leftOffset,210,340,120,"正在加工...");
			CDC_OutText_UTF(192+leftOffset,250,"已用时间:        小时      分      秒",BLACKCOLOR,-1,KZK_22);
			CDC_OutText_UTF(192+leftOffset,280,"剩余时间:        小时      分      秒",BLACKCOLOR,-1,KZK_22);
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
			g_fCloseDispCount=0.0; 												//屏保时间复位
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

				LCD_Power(0);

				//	Lcd_EnvidOnOff(0);
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

				strcpy(buf,"前碰");
				CDC_OutText_UTF(360,473,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -288888)					//T2
			{
				strcpy(buf,"前碰");
				CDC_OutText_UTF(360,507,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -177777)					//T1
			{

				strcpy(buf,"未碰");
				CDC_OutText_UTF(360,473,buf,REDCOLOR,g_nBk,MZK_24);
			}
			else if(s_lProbeAngle == -277777)					//T2
			{
				strcpy(buf,"未碰");
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
	static INT16S oldstatus[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	INT16S status[8]={0};

	if(init)
	{
		for(j=0;j<8;j++)
			oldstatus[j]=-1;
	}

	for( i=0; i<8;i++)//(g_Ini.iAxisCount<3?4:8); i++)
	{
		status[i]=GetOut(i);

		if(status[i]!=oldstatus[i])
		{
			oldstatus[i]=status[i];

			if(status[i]==1)
			{
				CDC_Fillellipse(676+i*15,getmaxy()-47,5,5,g_TextColor5);
			}
			else
			{
				CDC_Fillellipse(676+i*15,getmaxy()-47,5,5,g_nTitleBk);
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
					strcpy(buf,"线架加速");
				}
				else
					sprintf(buf,g_bEnglish?"OUT%d":"气缸%d",i+1);

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


/*void	UpdateProbe(BOOL init)													//刷新显示探针信号状态
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



void	UpdateCylinder(BOOL init)												//刷新显示汽缸状态
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

	LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]==1?0:1);  							//送线轴关闭时灯亮

	if(g_Ini.iAxisCount > 2)
		LedOut(ZSWITCHLED,g_Ini.iAxisSwitch[2]==1?0:1);

	if(g_Ini.iAxisCount > 3)
		LedOut(USWITCHLED,g_Ini.iAxisSwitch[3]==1?0:1);

	if(g_Ini.iAxisCount > 4)
		LedOut(VSWITCHLED,g_Ini.iAxisSwitch[4]==1?0:1);

	if(g_Ini.iAxisCount > 5)
		LedOut(WSWITCHLED,g_Ini.iAxisSwitch[5]==1?0:1);

	LedOut(TESTLED,g_bMode?1:0);												//灯亮表示进入测试模式
	//g_Ini.iHandStatus = 0;
	//	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);									//灯亮表示进入打开手摇轮
	LedOut(STOPLED,1);
	BackZeroLed();

	DisplayXYZ(FALSE);
	//	AllLedOff();
}


void	BackZeroLed()															//判断归零灯是否点亮
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

	for(a=1; a<5; a++)
		ResumeTasks(a);

	OSTimeDly(20);
	g_bBackZero=TRUE;
	g_iErrorNo=0;


	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset...":"回零点...",TRUE));


	if( !FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
	{
		OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset Interrupted!":"回零点中断,请确认急停按钮状态!",TRUE));

		g_bBackZero=FALSE;

		for(a=1; a<5; a++)
			SuspendTasks(a);

		OSTimeDly(50);
		return FALSE;
	}

	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" Reset Finish.":"回零完成.",TRUE));

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
	CDC_OutText24(705,430,  g_bEnglish?"HandB":"手持盒",WHITECOLOR,g_nBk);

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
			//CDC_OutText(22,459,g_bEnglish?"OUT1":"气缸1",OutStatus==1?BLUECOLOR:g_TextColor2,OutStatus==1?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(28,572,g_bEnglish?"OUT1":"气缸1",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);

			if(OutStatus==1)
				sprintf(g_pszMsgBuf,g_bEnglish?"1 Cylinder Out":"1号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"1 Cylinder In":"1号汽缸退回");
			OSQPost(MsgQueue,g_pszMsgBuf);


		}
		break;
		case 2:
		{

			WriteBit(1,GetOut(1)?0:1);
			OutStatus = GetOut(1);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"2 Cylinder Out":"2号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"2 Cylinder In":"2号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);

			CDC_DrawRect(rc[1],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(113,459,g_bEnglish?"OUT2":"气缸2",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(142,572,g_bEnglish?"OUT2":"气缸2",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);


		}
		break;
		case 3:
		{
			WriteBit(2,GetOut(2)?0:1);
			OutStatus = GetOut(2);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"3 Cylinder Out":"3号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"3 Cylinder In":"3号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[2],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(204,459,g_bEnglish?"OUT3":"气缸3",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(256,572,g_bEnglish?"OUT3":"气缸3",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 4:
		{
			WriteBit(3,GetOut(3)?0:1);
			OutStatus = GetOut(3);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"4 Cylinder Out":"4号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"4 Cylinder In":"4号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[3],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(295,459,g_bEnglish?"OUT4":"气缸4",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(370,572,g_bEnglish?"OUT4":"气缸4",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 5:
		{
			WriteBit(4,GetOut(4)?0:1);
			OutStatus = GetOut(4);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"5 Cylinder Out":"5号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"5 Cylinder In":"5号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[4],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(386,459,g_bEnglish?"OUT5":"气缸5",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(484,572,g_bEnglish?"OUT5":"气缸5",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 6:
		{
			WriteBit(5,GetOut(5)?0:1);
			OutStatus = GetOut(5);

			if(OutStatus)
				sprintf(g_pszMsgBuf,g_bEnglish?"6 Cylinder Out":"6号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"6 Cylinder In":"6号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[5],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(477,459,g_bEnglish?"OUT6":"气缸6",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(598,572,g_bEnglish?"OUT6":"气缸6",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
		}
		break;
		case 7:
		{
			WriteBit(6,GetOut(6)?0:1);
			OutStatus = GetOut(6);

			if(GetOut(2))
				sprintf(g_pszMsgBuf,g_bEnglish?"7 Cylinder Out":"7号汽缸伸出");
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"7 Cylinder In":"7号汽缸退回");

			OSQPost(MsgQueue,g_pszMsgBuf);
			CDC_DrawRect(rc[6],OutStatus?WHITECOLOR:g_nBk,WHITECOLOR);

			//CDC_OutText(567,459,g_bEnglish?"OUT7":"气缸7",OutStatus?BLUECOLOR:g_TextColor2,OutStatus?WHITECOLOR:g_nBk);
			CDC_OutText_UTF(712,572,g_bEnglish?"OUT7":"气缸7",OutStatus==1?BLUECOLOR:g_TextColor11,OutStatus==1?WHITECOLOR:g_nBk,KZK_24);
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
			Uart_Printf("确认升级系统文件?是(记忆)否(其它)\n");
			OSQPost(MsgQueue,g_bEnglish?"Upadate system file?":"确认升级系统文件?是(记忆)否(其它)");

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

				OSQPost(MsgQueue,g_bEnglish?"Update system file...":"文件升级中...");
				switch(SysAppUpdate())
				{
				case 1:
					OSQPost(MsgQueue,g_bEnglish?"SMC System update sucessful,please reboot system":"电子盘程序升级成功,重启系统后生效!");
					break;
				case 2:
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update sucessful,please reboot system":"U盘程序升级成功,重启系统后生效!");
					break;
				case -1:
					//OSQPost(MsgQueue,g_bEnglish?"SMC System update failed!":"电子盘升级失败,请确定升级文件是否存在!");
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update failed!":"未搜索到U盘,请确定U盘连接正常!");
					break;
				case -2:
					OSQPost(MsgQueue,g_bEnglish?"U Disk System update failed!":"U盘程序读取失败,请确定U盘读写路径正常!");
					break;
				case -3:
					OSQPost(MsgQueue,g_bEnglish?"System update failed!":"文件升级失败,请确定升级文件的正确性!");
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
			sprintf(g_pszMsgBuf,g_bEnglish?"Save OK":"存系统档完成!");
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

		if(g_iSpeedBeilv==3)									//高速 
		{
			Speed = g_Ini.lHandSpeed[ch-1]*g_Ini.iStepRate[ch-1];				
		}
		else if(g_iSpeedBeilv==2)								//中速 
		{
			Speed = g_Ini.lHandSpeed[ch-1];
		}
		else													//低速
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
			lTempPos = GetPulse(ch,TRUE) % g_Ini.lPulse[ch-1];//以下两行函数在多圈手摇时，不用多圈回摇。
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
					OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
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
					OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
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

	if( RealIO( STOPIO , IOPRESS ,2)||(g_bHandset?RealIO(STOPIO2,0,3):0))
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



BOOL ExitScrProtect(void)          						//若屏保中，则退出屏保,返回值为TRUE时表示有进行退出屏保动作
{
	BOOL bDoExit=FALSE;

	if(g_Ini.iScrProtect == 1)         					//屏保显示 
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
				LCD_Power(1); 
				//	Lcd_EnvidOnOff(1);
			}

			bDoExit=TRUE;
		}
	}

	return bDoExit;
}













