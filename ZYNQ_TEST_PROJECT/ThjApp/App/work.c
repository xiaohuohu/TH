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
extern     	FP64		g_fPulseOfMM[CHMAX];
WORK_EXT	INT32S 		g_TextColor1=LIGHTWHITECOLOR;
WORK_EXT	INT32S 		g_TextColor2=YELLOWCOLOR;
WORK_EXT	INT32S 		g_TextColor3=MAGENTACOLOR;
WORK_EXT	INT32S 		g_TextColor4=CYANCOLOR; 
WORK_EXT	INT32S 		g_TextColor5=YELLOWCOLOR;
WORK_EXT	INT32S 		g_TableLine =WHITECOLOR;
WORK_EXT	INT32S 		g_nBk		=DARKBLUECOLOR;
WORK_EXT	INT32S		g_nTechfrmBk=BLACKCOLOR;								//DARKGREENCOLOR2;
WORK_EXT	INT32S 		g_nTitleBk	=DARKGREENCOLOR2;							//LIGHTREDCOLOR;  
WORK_EXT	INT32S 		g_nTableBk	=BLACKCOLOR;								//DARKBLUECOLOR; 
WORK_EXT	INT32S 		g_nBackColor=BLACKCOLOR; 
WORK_EXT	INT32S 		g_nRunTextColor=WHITECOLOR; 
WORK_EXT	INT32S 		g_Color1	=LIGHTBLACKCOLOR;
WORK_EXT	INT32S		g_TextColor6=WHITECOLOR;
WORK_EXT	INT32S		g_TextColor7=DARKGRAYCOLOR;
WORK_EXT 	INT8S		g_pszMsgBuf[0x30]="\0";									//存放信息提示内容避免用局部变量的BUG
WORK_EXT	INT8S		g_pszLedOut[8][3]={"\0","\0","\0","\0","\0","\0","\0","\0"};
WORK_EXT	INT16S 		g_iRunLine=0;		
WORK_EXT 	INT16S		g_iStaRunLine=0;	
WORK_EXT 	BOOL 	 	g_bScreen=FALSE;	   									//指示当前屏幕显示那个界面，TRUE显示主加工界面
WORK_EXT 	BOOL	  	g_bFirstDispTime=TRUE;									//指示是否需要全部刷新日期时间显示，TRUE需要
WORK_EXT	BOOL		g_bFirstDispRunTime=FALSE;
WORK_EXT	BOOL		g_bDispRunTime=FALSE;
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
WORK_EXT 	INT16S		g_iWorkStatus=NORUN;
WORK_EXT 	BOOL		g_bAutoRunMode=TRUE;
WORK_EXT 	INT32S		g_lStepNode=0;	   
WORK_EXT 	BOOL		g_bStepIO = FALSE;  
WORK_EXT 	BOOL		g_bDispRunLine=FALSE;
WORK_EXT 	BOOL		g_bIsHand=FALSE;	
WORK_EXT 	BOOL		g_bStepToSingle=FALSE;
WORK_EXT 	BOOL		g_bMode=FALSE;	   
WORK_EXT	BOOL		g_bEMSTOP=FALSE; 
WORK_EXT	INT16S		g_iADValue=-1;  
WORK_EXT	BOOL		g_bTestStart=TRUE;
WORK_EXT	BOOL		g_bHandRun = FALSE;
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
WORK_EXT	BOOL		g_bPressStepIO2=TRUE;                                    // 高电平触发IO口单条
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
WORK_EXT 	INT32S		g_lProbeFail=0; 										//记录探针失败   
WORK_EXT	INT16S		g_iCheckStatusIO=0;
//WORK_EXT 	INT32S  	g_lRamRunCounts=0;										//铁电保存当前产量  
WORK_EXT 	INT32S  	g_lCurLine=0;											//用于显示当前行号 
WORK_EXT	INT32S		lYProbelong;
WORK_EXT	INT32S		lYProbelong1;
extern		INT32S		g_iProbe[4];
int 		lTagLastCount=0;							//缓存弹簧条数计数

extern		INT32S      g_iEmpNode[MAXCH];
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
extern      BOOL		g_bSingleRun ;	
//INT32S  	Hour11=-1;
//INT32S	Minute11=-1;
//INT32S	Hour12=-1;
//INT32S	Minute12=-1;
extern		BOOL		g_bInterface;
BOOL		g_bPerCountFinish = FALSE;
extern		BOOL		g_bIsRun;												//是否在运行中，主要是停机屏保时间清零
BOOL		g_bProbeLagCheck = TRUE;											//有探针动作时，是否进行逻辑脉冲与实际脉冲滞后检测
extern		INT32S 		g_bInitbmp;
BOOL        g_bRunPause=FALSE;						//是否加工中暂停过
BOOL		g_bPressWaitIO=FALSE;
//---------------------硬件限位检测----------------------//
BOOL  CheckHardLimit(INT16S ch, INT32S pos)
{
	if(g_Ini.iHardLimitSwitch[ch-1])
	{
		switch(ch)
		{
		/*
			case 1:
				if(RealIO(XLMTMINUS,0,2) && pos<0)
				{
					OSQPost(MsgQueue,g_bEnglish?"X-Axis negative limit!":"X轴硬件负限位报警!!!");
					return TRUE;
				}
				else if(RealIO(XLMTPLUS,0,2) && pos>0)
				{
					OSQPost(MsgQueue,g_bEnglish?"X-Axis positive limit!":"X轴硬件正限位报警!!!");
					return TRUE;
				}
				break;

			case 2:
				if(RealIO(YLMTMINUS,0,2) && pos<0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Y-Axis negative limit!":"Y轴硬件负限位报警!!!");
					return TRUE;
				}
				else if(RealIO(YLMTPLUS,0,2) && pos>0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Y-Axis positive limit!":"Y轴硬件正限位报警!!!");
					return TRUE;
				}
				break;
		 */
		case 3:
			if(RealIO(g_Ini.iZeroReplaceLimit[2]==1?ZZERO:ZLMTMINUS,0,2) && pos<0)
			{
				OSQPost(MsgQueue,g_bEnglish?"Z-Axis negative limit!":"Z轴硬件负限位报警!!!");
				//g_iErrorNo=14;
				g_iErrorNo=19;
				return TRUE;
			}
			else if(RealIO(g_Ini.iZeroReplaceLimit[2]==2?ZZERO:ZLMTPLUS,0,2) && pos>0)
			{
				OSQPost(MsgQueue,g_bEnglish?"Z-Axis positive limit!":"Z轴硬件正限位报警!!!");
				//g_iErrorNo=15;
				g_iErrorNo=20;
				return TRUE;
			}
			else if(pos==0)
			{
				if(RealIO(g_Ini.iZeroReplaceLimit[2]==1?ZZERO:ZLMTMINUS,0,2))
				{
					//g_iErrorNo=14;
					g_iErrorNo=19;
					OSQPost(MsgQueue,g_bEnglish?"Z-Axis negative limit Warming!":"Z轴负限位报警,请将轴回转到限位范围内!!!");
					return TRUE;
				}
				else if(RealIO(g_Ini.iZeroReplaceLimit[2]==2?ZZERO:ZLMTPLUS,0,2))
				{
					//g_iErrorNo=15;
					g_iErrorNo=20;
					OSQPost(MsgQueue,g_bEnglish?"Z-Axis positive limit Warming!":"Z轴正限位报警,请将轴回转到限位范围内!!!");
					return TRUE;
				}
			}
			break;

		case 4:
			if(RealIO(ULMTMINUS,0,2) && pos<0)
			{
				OSQPost(MsgQueue,g_bEnglish?"U-Axis negative limit!":"A轴硬件负限位报警!!!");
				g_iErrorNo=16;
				return TRUE;
			}
			else if(RealIO(ULMTPLUS,0,2) && pos>0)
			{
				OSQPost(MsgQueue,g_bEnglish?"U-Axis positive limit!":"A轴硬件正限位报警!!!");
				g_iErrorNo=17;
				return TRUE;
			}
			else if(pos==0)
			{
				if(RealIO(ULMTMINUS,0,2))
				{
					g_iErrorNo=16;
					OSQPost(MsgQueue,g_bEnglish?"A-Axis negative limit Warming!":"A轴负限位报警,请将轴回转到限位范围内!!!");
					return TRUE;
				}
				else if(RealIO(ULMTPLUS,0,2))
				{
					g_iErrorNo=17;
					OSQPost(MsgQueue,g_bEnglish?"A-Axis positive limit Warming!":"A轴正限位报警,请将轴回转到限位范围内!!!");
					return TRUE;
				}
			}
			break;

		default:
			break;
		}
	}
	return FALSE;
}


//---------------------软件限位检测----------------------//
BOOL  CheckSoftLimit(INT16S ch,INT32S pos)
{
	if(g_Ini.fMaxLimit[ch-1]-g_Ini.fMinLimit[ch-1]>0.01)
	{
		switch(ch)
		{
		case 1:
			break;

		case 2:
			break;

		case 3:
			//if(g_Ini.iZSoftLimit==1)
		{
			if(pos<0 && (GetMM(3,TRUE)-g_Ini.fMinLimit[2] < 0.01))
			{
				OSQPost(MsgQueue,g_bEnglish?"Z-Axis negative limit!":"Z轴软件负限位报警!!!");
				//g_iErrorNo=14;
				g_iErrorNo=22;
				return TRUE;

			}
			else if(pos>0 && (g_Ini.fMaxLimit[2]-GetMM(3,TRUE)< 0.01))
			{
				OSQPost(MsgQueue,g_bEnglish?"Z-Axis positive limit!":"Z轴软件正限位报警!!!");
				//g_iErrorNo=15;
				g_iErrorNo=21;
				return TRUE;

			}
			else if(pos==0)
			{
				if(GetMM(3,TRUE)-g_Ini.fMinLimit[2] < 0.01)
				{
					//g_iErrorNo=14;
					g_iErrorNo=22;
					OSQPost(MsgQueue,g_bEnglish?"Z-Axis negative limit Warming!":"Z轴负限位报警,请将轴回转到限位范围内!!!");

					return TRUE;
				}
				else if(g_Ini.fMaxLimit[2]-GetMM(3,TRUE)< 0.01)
				{
					//g_iErrorNo=15;
					g_iErrorNo=21;
					OSQPost(MsgQueue,g_bEnglish?"Z-Axis positive limit Warming!":"Z轴正限位报警,请将轴回转到限位范围内!!!");

					return TRUE;
				}
			}
		}
		break;

		case 4:
			if(pos<0 && (GetMM(4,TRUE)-g_Ini.fMinLimit[3] < 0.01))
			{
				OSQPost(MsgQueue,g_bEnglish?"A-Axis negative limit!":"A轴软件负限位报警!!!");
				g_iErrorNo=16;
				return TRUE;
			}
			else if(pos>0 && (g_Ini.fMaxLimit[3]-GetMM(4,TRUE)< 0.01))
			{
				OSQPost(MsgQueue,g_bEnglish?"A-Axis positive limit!":"A轴软件正限位报警!!!");
				g_iErrorNo=17;
				return TRUE;
			}
			else if(pos==0)
			{
				if(GetMM(4,TRUE)-g_Ini.fMinLimit[3] < 0.01)
				{
					g_iErrorNo=16;
					OSQPost(MsgQueue,g_bEnglish?"A-Axis negative limit Warming!":"A轴负限位报警,请将轴回转到限位范围内!!!");

					return TRUE;
				}
				else if(g_Ini.fMaxLimit[3]-GetMM(4,TRUE)< 0.01)
				{
					g_iErrorNo=17;
					OSQPost(MsgQueue,g_bEnglish?"A-Axis positive limit Warming!":"A轴正限位报警,请将轴回转到限位范围内!!!");

					return TRUE;
				}
			}
			break;

		default:
			break;
		}
	}
	return FALSE;
}


void	DisplayXYZ( BOOL cur)
{
	INT8S   buffer[0x10]="\0";
	INT32S	speed;
	INT16U	i = 0;

	static	FP32	temppos1 = -999999.0;
	static	FP32	temppos2 = -999999.0;
	static	FP32	temppos3 = -999999.0;

	FP32	temp1 = 0.0;
	FP32	temp2 = 0.0;
	FP32	temp3 = 0.0;

	if(cur)
	{
		if(!g_bInterface)
		{	
			memset(buffer,NULL,sizeof(buffer));
			ftoa(GetAngle(1,cur),(INT8U *)buffer,1,8);
			CDC_OutTextLR_UTF(110,367,buffer,REDCOLOR,g_nBackColor,MZK_32,8,1);

			if( g_Ini.iAxisSwitch[1]==1)
			{		
				memset(buffer,NULL,sizeof(buffer));					
				ftoa(GetMM(2,cur) - GetYLong(vm_getindex()),(INT8U *)buffer,1,9);

				CDC_OutTextLR_UTF(340,367,buffer,REDCOLOR,g_nBackColor,MZK_32,9,1);
			}
			else
			{
				CDC_OutTextLR_UTF(340,367,"OFF",REDCOLOR,g_nBackColor,MZK_32,6,1);
			}		

			if(g_Ini.iAxisCount>2)
			{
				if( g_Ini.iAxisSwitch[2]==1 && g_Ini.iAxisRunMode[2]==0)		//Z轴单圈模式
				{

					memset(buffer,NULL,sizeof(buffer));

					ftoa(GetAngle(3,cur),(INT8U *)buffer,1,8);
					//Uart_Printf("GetAngle(3,cur)=%f\n",GetAngle(3,cur));

					CDC_OutTextLR_UTF(580,367,buffer,REDCOLOR,g_nBackColor,MZK_32,8,1);

				}
				else if( g_Ini.iAxisSwitch[2]==1 && g_Ini.iAxisRunMode[2]==1)//Z轴多圈模式
				{
					INT16S  tempi1;
					FP32 	tempi2;
					FP32	z;			
					INT8S	buf1[0x10]="\0";
					INT8S   buf2[0x10]="\0";

					memset(buffer,NULL,sizeof(buffer));
					z=GetMM(3,cur);

					if(z>=0)
					{
						tempi1=z/360;
						tempi2=z-tempi1*360;
						itoa(tempi1,(INT8U *)buf2,10);
						strcpy(buffer,buf2);
					}
					else
					{				
						tempi1=(INT16S)(fabs(z)/360);
						tempi2=fabs(z)-tempi1*360;
						itoa(tempi1,(INT8U *)buf2,10);
						buffer[0]='-';
						strcat(buffer,buf2);
					}

					itoa((INT16S)(tempi2),(INT8U *)buf1,10);
					strcat(buffer,"C");
					strcat(buffer,buf1);
					//tempi1=((INT16S)(tempi2*10+0.01))%10;					//用于显示小数位
					//strcat(buffer,".");									
					//memset(buf1,NULL,0x10);		
					//itoa(tempi1,(INT8U *)buf1,10);
					//strcat(buffer,buf1);

					{
						INT16S i=0;

						for(i=strlen(buffer);i<11;i++)
						{
							buffer[i]=' ';
						}

						buffer[i]=NULL;
					}

					CDC_OutTextLR_UTF(580,367,buffer,REDCOLOR,g_nBackColor,MZK_32,8,1);	
				}
				else if(g_Ini.iAxisSwitch[2]==1 && g_Ini.iAxisRunMode[2]==2)//Z轴丝杆
				{
					memset(buffer,NULL,sizeof(buffer));					
					ftoa(GetMM(3,cur),(INT8U *)buffer,2,8);

					CDC_OutTextLR_UTF(580,367,buffer,REDCOLOR,g_nBackColor,MZK_32,8,1);	
				}
				else
				{
					CDC_OutTextLR_UTF(580,367,"OFF",REDCOLOR,g_nBackColor,MZK_32,6,1);
				}	
			}
		}
		else																	//当轴的坐标位置发生改变时，才进行刷新显示轴坐标，提高效率。
		{
			temp1 = GetAngle(1,cur);
			temp2 = GetMM(2,cur);

			if(g_Ini.iAxisRunMode[2]==0)
				temp3 = GetAngle(3,cur);
			else if(g_Ini.iAxisRunMode[2]==1)
				temp3 = GetMM(3,cur);
			else
				temp3 = GetMM(3,cur);

			if(g_Ini.iAxisCount<3)
			{
				if(temppos1!=temp1)
				{
					memset(buffer,NULL,sizeof(buffer));

					ftoa(GetAngle(1,cur),(INT8U *)buffer,1,9);

					CDC_OutText_UTF(220,165,buffer,REDCOLOR,g_nBackColor,YH_32);

					temppos1 = temp1;
				}

				if(g_Ini.iAxisSwitch[1]) 
				{
					if(temppos2!=temp2)
					{	
						FP32 Ylong=0.0;

						Ylong = GetMM(2,cur) - GetYLong(vm_getindex());

						memset(buffer,NULL,sizeof(buffer));

						if(fabs(Ylong-0.0)<0.1)
						{
							CDC_PutColorBar(220,235,180,35,g_nBackColor,100);
						}

						ftoa(Ylong,(INT8U *)buffer,1,8);

						CDC_OutText_UTF(220,235,buffer,REDCOLOR,g_nBackColor,YH_32);
						temppos2 = temp2;
					}
				}
				else if(((INT32S)(temppos2)-99999) != 0)
				{
					CDC_PutColorBar(220,235,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,235,"OFF",REDCOLOR,g_nBackColor,YH_32);
					temppos2 = 99999.0;
				}

			}

			else if(g_Ini.iAxisCount>2)
			{
				if(temppos1!=temp1)												//
				{
					memset(buffer,NULL,sizeof(buffer));

					ftoa(GetAngle(1,cur),(INT8U *)buffer,1,9);

					CDC_OutText_UTF(220,143,buffer,REDCOLOR,g_nBackColor,YH_32);

					temppos1 = temp1;
				}

				if(g_Ini.iAxisSwitch[1])
				{
					if(temppos2!=temp2)
					{	
						FP32 Ylong=0.0;

						Ylong = GetMM(2,cur) - GetYLong(vm_getindex());
						memset(buffer,NULL,sizeof(buffer));

						if(fabs(Ylong-0.0)<0.1)
						{
							CDC_PutColorBar(220,193,180,35,g_nBackColor,100);
						}

						ftoa(GetMM(2,cur) - GetYLong(vm_getindex()),(INT8U *)buffer,1,8);

						CDC_OutText_UTF(220,193,buffer,REDCOLOR,g_nBackColor,YH_32);
						temppos2 = temp2;
					}

				}
				else if(((INT32S)(temppos2)-99999) != 0) 
				{
					CDC_PutColorBar(220,193,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,193,"OFF",REDCOLOR,g_nBackColor,YH_32);
					temppos2 = 99999.0;
				}

				if(g_Ini.iAxisSwitch[2])
				{
					if(temppos3!=temp3)
					{
						if(g_Ini.iAxisRunMode[2]==0)
							ftoa(GetAngle(3,TRUE),(INT8U *)buffer,1,8);
						else if(g_Ini.iAxisRunMode[2]==1)
						{
							INT16S  tempi1;
							FP32 	tempi2;
							FP32	z;			
							INT8S	buf1[0x10]="\0";
							INT8S   buf2[0x10]="\0";

							z=GetMM(3,TRUE);

							if(z>=0)
							{
								tempi1=z/360;
								tempi2=z-tempi1*360;
								itoa(tempi1,(INT8U *)buf2,10);
								strcpy(buffer,buf2);
							}
							else
							{				
								tempi1=(INT16S)(fabs(z)/360);
								tempi2=fabs(z)-tempi1*360;
								itoa(tempi1,(INT8U *)buf2,10);
								buffer[0]='-';
								strcat(buffer,buf2);
							}

							itoa((INT16S)(tempi2),(INT8U *)buf1,10);
							strcat(buffer,"C");
							strcat(buffer,buf1);
							{
								INT16S i=0;

								for(i=strlen(buffer);i<10;i++)
								{
									buffer[i]=' ';
								}

								buffer[i]=NULL;
							}			
						}
						else if(g_Ini.iAxisRunMode[2]==2)
							ftoa(GetMM(3,TRUE),(INT8U *)buffer,2,8);

						CDC_OutText_UTF(220,243,buffer,REDCOLOR,g_nBackColor,YH_32);
						temppos3=temp3;
					}
				}			
				else if(((INT32S)(temppos3)-99999) != 0)
				{
					CDC_PutColorBar(220,243,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,243,"OFF",REDCOLOR,g_nBackColor,YH_32);
					temppos3 = 99999.0;
				}

			}

			UpdateProbe(FALSE);
			//UpdateCylinder(FALSE);
		}

		{															//坐标保存到铁电
			static 	INT32S sTempPos[MAXCH]={0,0,0,0};
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
}


INT16S	RunAllData()
{
	INT32S 		lNode=0;
	INT16S 		l=0,k=0;
	INT32U		ti1,ti2;
	INT32S 		x=0,y=0,z=0,a=0,movespeed;
	UNITDATA 	pData,pDataTmp;
	INT16S		ProbeCount=0;
	BOOL	 	WorkEnd=FALSE;

	BOOL  		bProbeFirstRun =  TRUE; 						//判断加工第一条带有探针弹簧时，按单条时不能直接跳到AUTORUNEND
	INT16S		iHaveTag = 0;													//用于代替bProbeFirstRun，增加扩展性
	INT32S 		itempRunCount =g_Ini.lRunCount;


	INT32S		lCurPulse[MAXCH] ;
	INT16U		nLastLine=0;
	INT8U		nLastOut[8]={2,2,2,2,2,2,2,2};
	INT32S	    iCountPer=0;									//保存每批产量


	SetQuiteRange(8000);


	////*********启动前先检查是否归完零  和报警输入检测*****************////
	if(g_bRelease)
	{
		if(g_bEMSTOP)
		{
			OSQPost(MsgQueue,g_bEnglish?"StartRun is Invalid after Emergency Stop!":"紧急停止后请先归零!");
			return 1;
		}

		if( !g_bFirstResetX  )
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Reset X!!!":"X先归零!!!");
			return 1;
		}

		switch(g_Ini.iAxisCount)
		{

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

		if(CheckWarning(TRUE))
		{
			if(g_Ini.iWarningOut==1)
			{
				if(GetOut(O_WARNING)==0)
					WriteBit(O_WARNING,OPEN);
			}

			return 1;

		}

		if(g_Ini.iWarningOut==1)
		{
			WriteBit(O_WARNING,CLOSE);
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

	if( !g_bUnpackCorrect )
	{
		OSQPost(MsgQueue,g_bEnglish?"Data Invalid!":"当前所编程序有错误!");
		return 1;
	}

	Start(1);Start(2);

	switch(g_Ini.iAxisCount)
	{
	case 4:Start(4);
	case 3:Start(3);
	break;
	}


	{															//启动前若以归过机械原点，但现在不在零点需回零点

		OSTimeDly(10);

		SetMM(2,0,TRUE);
		g_bFirstResetY=TRUE;

		OSTimeDly(10);

		if(g_Ini.iAxisCount>3 && g_Ini.iAxisSwitch[3]==1 && (g_Ini.iBackMode[3]!=3 || g_Ini.iAxisRunMode[3]==1))
		{
			if(!WorkFindZero(4))
				return 1;
		}

		if(g_Ini.iAxisCount>2 && g_Ini.iAxisSwitch[2]==1 && (g_Ini.iBackMode[2]!=3 || g_Ini.iAxisRunMode[2]==1))
		{		
			if(!WorkFindZero(3))
				return 1;
		}		


		if(GetPulse(1,TRUE)!=0 && g_Ini.iBackMode[0]!=3)
		{
			if(!WorkFindZero(1))
				return 1;
		}	

	}	

	key_set_led(STOPLED,LEDCLOSE);	

	if(g_bStepIO)
	{
		OSQPost(MsgQueue,g_bEnglish?"Start TestRun...":"开始寸动运行...");
	}
	else if(g_bHandRun)
	{
		OSQPost(MsgQueue,g_bEnglish?"Start Handwheel Run...":"开始手摇运行...");
	}
	else
	{
		if(g_bAutoRunMode ==FALSE)
			OSQPost(MsgQueue,g_bEnglish?"Test Single...":"单条加工...");
		else
			OSQPost(MsgQueue,g_bEnglish?"Start AutoRun...":"开始自动加工...");

		if(g_bAutoRunMode)
			key_set_led(RUNLED,LEDOPEN);
		else
			key_set_led(STEPLED,LEDOPEN);
	}

	g_iWorkStatus=RUNNING;
	g_iErrorNo = 0;

	{															//开启其它检测任务
		INT8S a = 0;

		for(a=0; a<5; a++)
			ResumeTasks(a);

		OSTimeDly(50);
	}

	//	if(g_bAutoRunMode && !g_bMode)								//开自动加工模式下
	CloseDisplay(0);

	if(!g_bMode)
	{
		if(g_Ini.iXianjiaDrive==1)
			WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]?1:0);							
	}

	g_iMoveAxis = g_Ini.iAxisCount*100; 						//zhi shi wei inp

	vm_sync();											//同步，清空缓存

	//g_lProbeAngle = -888888;								//角度初始化

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
				OSQPost(MsgQueue,g_bEnglish?"Data Analysis Error!":"数据拷贝解码错误,请确认所编程序正确无误!");
				g_iMoveAxis = 0;
				//return 0;
				goto   WHILEEND;
			}	
		}

		lNode = 0;
		WorkEnd = FALSE;
		ProbeCount = 0;


		for(; lNode<g_lDataCopyXmsSize;)						//加工一条弹簧的数据发送
		{
			XMSToMEMEx((INT8U *)&pData,g_uXMS,(INT32S)lNode*sizeof(UNITDATA),sizeof(UNITDATA));
			lNode++;			

			//Uart_Printf("pData.nLine=%d,pData.lValue[0]=%d,pData.lValue[1]=%d,pData.lValue[2]=%d,pData.lValue[3]=%d,pData.lValue[4]=%d,pData.nReset=%d\n",pData.nLine,pData.lValue[0],pData.lValue[1],pData.lValue[2],pData.lValue[3],pData.lValue[4],pData.nReset);

			switch(pData.nCode)
			{
			case SPEED:										//速度指令
			{
				continue;
			}
			break;

			case MOVE:
			{
				/*
						if( pData.nIO) 												//当行解释到探针行在插补前将探针打开放入缓存队列里
						{
							g_iCheckStatusIO =  pData.nIO;

							if(g_Ini.iProbeMode==1)
								vm_set_probe(g_iCheckStatusIO,2);
						}
				 */

				{

					INT8U	AxisEnable =0;					//用于标识插补的开关轴

					AxisEnable = 1|(g_Ini.iAxisSwitch[1]<<1) |(g_Ini.iAxisSwitch[2]<<2) |(g_Ini.iAxisSwitch[3]<<3);

					if(g_Ini.iAxisCount<3)					//g_Ini.iAxisCount==2
					{
						AxisEnable = AxisEnable & 0x03;
					}
					else if(g_Ini.iAxisCount<4)				//g_Ini.iAxisCount==3
					{
						AxisEnable = AxisEnable & 0x07;
					}

					//插补解析
					x = pData.lValue[0];
					y = pData.lValue[1];
					z = pData.lValue[2];
					a = pData.lValue[3];
					movespeed = pData.lValue[4];

					for(k=0;k<MAXCH;k++)
					{
						if(g_Ini.iBackMode[k]==3 && lNode-1<=g_iEmpNode[k] && g_Ini.iAxisRunMode[k]!=1)
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
							}
						}
					}


					if(movespeed>VM.SpdMax[pData.SpindleAxis-1]*1000)
						movespeed=VM.SpdMax[pData.SpindleAxis-1]*1000;

					if(movespeed<1)
						movespeed=1;


					//Uart_Printf("pData.nLine=%d  AxisEnable=%d pData.SpindleAxis-1=%d  x=%d y=%d z=%d a=%d movespeed =%d\n",pData.nLine,AxisEnable,pData.SpindleAxis-1,x,y,z,a,movespeed);

					if(vm_abs_inp_move(pData.nLine,AxisEnable,pData.SpindleAxis-1,x,y,z,a,0,0,movespeed) == -1)
					{
						Uart_Printf("vm_abs_inp_move error\n");
						goto WHILEEND;
					}


					for(l=0;l<MAXCYLINDER;l++)  							//汽缸动作
					{
						//Uart_Printf("pData.nOut[%d]=%d\n",l,pData.nOut[l]);

						if(2 !=pData.nOut[l])
						{
							vm_set_io(l,pData.nOut[l]);
						}
					}

					//Uart_Printf("pData.nReset=%d\n",pData.nReset);
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
					}
				}

				//	Uart_Printf("pData.nIO=%d\n",pData.nIO);


				//if(pData.nIO<=4&&pData.nIO!=0) 	//当发送到探针行时停止发送缓存的数据，并等待探针信号
				if( pData.nIO == 6 && g_Ini.iSwhSingleIo==2)
				{

					INT32U tt2=0;
					INT32U tt1=0;

					vm_start();

					while(TRUE)					//等待缓存数据发完再开始延时
					{

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{

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

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
						{
							goto  AUTORUNEND;
						}


						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 8 )
						{
							Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
							goto   WHILEEND;
						}

						if(!vm_get_status())
							break;

						OSTimeDly(1);
					}

					vm_sync();								//同步，清空缓存

					tt1= OSTimeGet();
					while(TRUE)
					{
						tt2 = OSTimeGet();

						if(!g_bPressStepIO2 && RealIO(SINGLEDI,0,2))
						{
							g_bPressStepIO2=TRUE;
							break;
						}

						if(tt2-tt1>g_Ini.iProDelay)
						{
							g_iErrorNo = 32;
							goto   WHILEEND;
						}

						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 4 )
							goto   WHILEEND;

						OSTimeDly(1);
					}
				}//当发送到探针行时停止发送缓存的数据，并等待探针信号
				else if(pData.nIO && g_Ini.iProDelay == 0)
				{
					BOOL 	bFirstProbe = TRUE;

					g_iCheckStatusIO = pData.nIO;

					vm_start();												//执行缓存插补

					while(TRUE)
					{
						/*if(pData.nLine-1 == vm_getindex())
							{
								g_bLagCheck = FALSE;
							}*/

						if(pData.nLine == vm_getindex())					//等待执行缓存到探针行。注：探针行的行号与开始角行号一致。
						{
							lYProbelong = 0;								//每次进探针行时要把前一探针的偏差清零
							//g_lProbeAngle = -888888;						//角度初始化

							if(bFirstProbe)									//打开探针,并首次检查探针状态若有信号探针出错
							{
								INT16S flag = 0;

								bFirstProbe = FALSE;


								//if(g_Ini.iProbeMode==1)
								//OpenProbe(0,g_iCheckStatusIO,2);		//首次读探针时打开探针

								OpenProbe(0,g_iCheckStatusIO,g_Ini.iProbeMode==1?2:6);//伺服清零输出点6没有接 可以看做为无私服清零

								flag = ReadProbe(0,g_iCheckStatusIO);

								/*
									if(g_Ini.iProbeMode==1)
										flag = ReadProbe(0,g_iCheckStatusIO);
									else
										flag = RealIO(g_iProbe[g_iCheckStatusIO-1],0,2);
								 */

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


									/*
										for(l=0;l<MAXCYLINDER;l++)  			//探针失败后探针行汽缸动作关
										{
											if(1==pData.nOut[l])
											{
												WriteBit(l,0);
											}
										}
									 */

									lYProbelong = pData.lValue[1] - GetPulse(2,TRUE);

									lYProbelong1 +=lYProbelong;

									SetPulse(2,pData.lValue[1],TRUE);		//因为送线绝对坐标，探针失败直接设置探针行的Y轴进去

									vm_sync();								//同步，清空缓存

									//if(g_Ini.iProbeMode==1)
									CloseProbe(0);

									//if(g_Ini.lProbeCount ==1)

									if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
									{
										g_iErrorNo=23;						//表探针一直触碰失败
										OSTimeDly(30);
										SetQuiteRange(0);


										g_bRunPause=TRUE;

										if(g_Ini.iXianjiaDrive)
											WriteBit(O_SENDADD,0);

										OSQPost(MsgQueue,g_bEnglish?"Probe Fail for Touching Throughout!":"探针一直触碰失败!加工暂停...");

										while(TRUE)
										{
											if(g_iErrorNo != 0 && g_iErrorNo!=23 && g_iErrorNo!=3  && g_iErrorNo!=2 && g_iErrorNo!=8)
											{
												//Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
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

									break;												//退出检测段执行下一行
								}
							}
							else														//检测到探针信号
							{
								INT16S flag = 0;

								//if(g_Ini.iProbeMode==1)
								flag = ReadProbe(0,g_iCheckStatusIO);
								//else
								//flag=RealIO(g_iProbe[g_iCheckStatusIO-1],0,2);


								if(flag ==1 )				//探针成功
								{

									//	Uart_Printf("testpoint  探针成功1 \n");

									vm_stop(1);				//加清缓存和停止代码


									if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针成功需关闭同一行的气缸
									{
										for(l=0;l<MAXCYLINDER;l++)  						//探针成功后探针行汽缸动作关
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

									//if(g_Ini.iProbeMode==1)
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

							//if(g_Ini.iProbeMode==1)
							flag = ReadProbe(0,g_iCheckStatusIO);
							//else
							//flag=RealIO(g_iProbe[g_iCheckStatusIO-1],0,2);

							if(flag==1)						//探针成功
							{

								if(nLastLine==pData.nLine-1)								//气缸解析在开始角,探针成功需关闭同一行的气缸
								{
									for(l=0;l<MAXCYLINDER;l++)  							//探针成功后探针行汽缸动作关
									{
										if(1==nLastOut[l])
										{
											WriteBit(l,0);
										}
									}
								}

								//	vm_stop();					//加清缓存和停止代码
								vm_sync();					//同步，清空缓存

								//if(g_Ini.iProbeMode==1)
								CloseProbe(0);

								break;						//退出检测段执行下一行?
							}
							else
							{
								//Uart_Printf("testpoint  探针未触碰失败1!\n");

								if(nLastLine==pData.nLine-1)			//气缸解析在开始角,探针失败需关闭同一行的气缸
								{
									for(l=0;l<MAXCYLINDER;l++)  							//探针失败后探针行汽缸动作关
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

								//if(g_Ini.iProbeMode==1)
								CloseProbe(0);

								//if(g_Ini.lProbeCount ==1)
								if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_iErrorNo=24;						//表探针未触碰失败
									OSTimeDly(30);
									SetQuiteRange(0);

									g_bRunPause=TRUE;

									if(g_Ini.iXianjiaDrive)
										WriteBit(O_SENDADD,0);

									OSQPost(MsgQueue,g_bEnglish?"Probe Fail for no Touch!":"探针未触碰失败!加工暂停...\n");

									while(TRUE)
									{
										if(g_iErrorNo != 0 && g_iErrorNo!=24 && g_iErrorNo!=3 && g_iErrorNo!=2 && g_iErrorNo!=8)
										{
											//				Uart_Printf("g_iErrorNo = %d\n",g_iErrorNo);
											goto   WHILEEND;
										}

										if(g_iErrorNo == 0)
										{
											//				Uart_Printf("testpoint g_iErrorNo==0\n");
											break;
										}

										OSTimeDly(1);
									}
								}


								if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
								{
									g_bAutoRunMode=FALSE;
								}

								break;						//退出检测段执行下一行?
							}

						}

						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)			//是否执行下一个新的弹簧
						{
							//g_lProbeAngle = -888888;

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


							if(g_Ini.iPerCount>0)								//到达批量设置则退出加工
							{
								if(!g_bMode)
								{
									iCountPer++;

									if(iCountPer==g_Ini.iPerCount)
									{
										iCountPer=0;
										g_bPerCountFinish = TRUE;
										//	vm_sync();								//同步，清空缓存
										g_bAutoRunMode=FALSE;
										goto AUTORUNEND;
									}
								}
								else
									iCountPer=0;
							}

							if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
							{
								g_bAutoRunMode=FALSE;
							}
						}

						//	if((g_bDataCopy==FALSE|| g_bAutoRunMode==FALSE) && bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) && iHaveTag>0)
						{

							if(bFirstProbe == FALSE)
							{
								if(nLastLine==pData.nLine-1)			//气缸解析在开始角
								{
									for(l=0;l<MAXCYLINDER;l++)
									{
										if(1==pData.nOut[l])
										{
											WriteBit(l,0);
										}
									}
								}

								if(g_Ini.iProbeMode==1)
									CloseProbe(0);

								//Uart_Printf("vm_get_tag error! very improtion\n");
							}
							goto  AUTORUNEND;
						}

						OSTimeDly(1);

						if(g_iErrorNo != 0 && g_iErrorNo != 3  && g_iErrorNo != 2  && g_iErrorNo != 8)
						{
							//	Uart_Printf("g_iErrorNo102 = %d\n",g_iErrorNo);

							if(bFirstProbe == FALSE)
							{
								//if(g_Ini.iProbeMode==1)
								CloseProbe(0);
							}

							goto   WHILEEND;
						}

					} // end while(TRUE)


					//SetCommandPos(2,0);				 //探针检测后清除位置，便于脉冲滞后检测
					//SetActualPos(2,0);
					//g_bLagCheck = TRUE;

				} //end if( pData.nIO) 探针处理

				for(l=0;l<MAXCYLINDER;l++)
				{
					nLastOut[l]=pData.nOut[l];
				}

				//				if(pData.fDelay-0.0>0.000001)					//延时


				if(g_Ini.iProDelay>0&&pData.nIO>0) //输入等待只有探针为5才起作用
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
							Uart_Printf(" g_Ini.lRunCount==%d\r\n",g_Ini.lRunCount);

							ti2=OSTimeGet();

							if(g_Ini.iPerCount>0)								//到达批量设置则退出加工
							{
								if(!g_bMode)
								{
									iCountPer++;
									Uart_Printf(" iCountPer==%d\r\n",iCountPer);
									if(iCountPer+TAGBUFCOUNT>g_Ini.iPerCount)
									{
										iCountPer=0;
										g_bPerCountFinish = TRUE;
										//	vm_sync();								//同步，清空缓存
										g_bAutoRunMode=FALSE;
										//goto AUTORUNEND;
										pData.nCode = PEND;
										continue;
									}
								}
								//										else
								//											iCountPer=0;
							}

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


						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8 )//&&g_iErrorNo!=9 && g_iErrorNo!=10
						{
							goto   WHILEEND;
						}

						if(!vm_get_status())
							break;

						OSTimeDly(1);
					}

					vm_sync();												//同步，清空缓存
					tt1=OSTimeGet();


					if(g_Ini.iPbFnSl==0)//探针为输入等待模式
					{
						while(TRUE)
						{
							//等待输入
							tt2=OSTimeGet();

							if(!g_bPressWaitIO&&RealIO(IO[pData.nIO-1],0,2))
							{
								g_bPressWaitIO=TRUE;
								break;
							}
							if(g_bPressWaitIO&&RealIO(IO[pData.nIO-1],1,2))
							{
								g_bPressWaitIO=FALSE;
								//break;
							}
							if((tt2-tt1)*1000.0/OS_TICKS_PER_SEC>g_Ini.iProDelay)//读不到指定输入则错误退出
							{
								//Uart_Printf("g_lProbeFail = %d!\n",g_lProbeFail);
								//								OSSemPost(RunSpeedCountSem);
								//								SetRange(0);
								//								g_bRunPause=TRUE;
								//								Uart_Printf(" g_Ini.lRunCount2222222==%d\r\n",g_Ini.lRunCount);
								g_iErrorNo=31;
								//break;
								goto WHILEEND;
							}

							if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8 && g_iErrorNo!=9 && g_iErrorNo!=10 )
								goto WHILEEND;

							OSTimeDly(1);
						}
					}
					break;
				}// if(pData.fDelay-0.0>0.000001)
				nLastLine=pData.nLine;

				if(pMQ->QueueCount>8)												//增加对插补缓存空间在应用层进行限制
				{
					vm_start();

					while(TRUE)
					{
						if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
						{

							bProbeFirstRun = TRUE;
							iHaveTag--;

							lYProbelong = 0;
							lYProbelong1 = 0;
							g_Ini.lRunCount=vm_get_tag();

							lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;
							Uart_Printf(" g_Ini.lRunCount333333==%d\r\n",g_Ini.lRunCount);
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

						//	if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  bProbeFirstRun==FALSE)
						if((g_bDataCopy==FALSE || g_bAutoRunMode==FALSE) &&  iHaveTag>0)
						{
							//	Uart_Printf("4iHaveTag=%d\n",iHaveTag);

							goto  AUTORUNEND;
						}

						if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8 )
						{
							//Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
							goto   WHILEEND;
						}

						if(pMQ->QueueCount<6)
						{
							//Uart_Printf("test pMQ->QueueCount<6 \n");
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


				//	vm_set_tag(g_Ini.lRunCount+lTagCount+1);		//此处对加工的弹簧数进行累加，但是不会直接改变g_Ini.lRunCount，需要等到执行完当前条全部缓存g_Ini.lRunCount才能累加

				itempRunCount++;
				vm_set_tag(itempRunCount);

				bProbeFirstRun = FALSE;
				iHaveTag++;

				//Uart_Printf("lTagCount=%d,TAGBUFCOUNT=%d\n",lTagCount,TAGBUFCOUNT);
				/*
						if(lTagCount>=TAGBUFCOUNT)						//如果缓存中存储的内容达到了预设的弹簧条数
						{
							vm_start();

							while(TRUE)
							{

								if( g_bDataCopy==FALSE || g_bAutoRunMode==FALSE)
								{
									goto AUTORUNEND;
								}
								else if((vm_get_tag()+TAGBUFCOUNT)>lTagLastCount)
								{	
									g_Ini.lRunCount=vm_get_tag();
									lTagLastCount=g_Ini.lRunCount+TAGBUFCOUNT;


									ti2=OSTimeGet();

									g_lRunTime=ti2-ti1;

									if(g_lRunTime<0)
										g_lRunTime=1000;

									OSSemPost(RunSpeedCountSem); 
									ti1=OSTimeGet();

									if(g_Ini.iPerCount>0)								//到达批量设置则退出加工
									{	
										if(!g_bMode)
										{
											iCountPer++;									

											if(iCountPer==g_Ini.iPerCount)
											{
												iCountPer=0;
												g_bPerCountFinish = TRUE;
												vm_sync();								//同步，清空缓存
												g_bAutoRunMode=FALSE;
												goto AUTORUNEND;
											}
										}
										else
											iCountPer=0;
									}					

									if(g_Ini.lRunCount>=g_Ini.lDestCount-1)
									{
										g_bAutoRunMode=FALSE;
										goto AUTORUNEND;
									}

									break;
								}

								OSTimeDly(1);

								if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8)
								{
									goto   WHILEEND;
								}
							}
						}
						else
							lTagCount++;
				 */


				if(g_bDataCopy==FALSE || g_bAutoRunMode==FALSE)
				{
					//	Uart_Printf("testpoint AUTORUNEND3\n");
					goto AUTORUNEND;
				}

			}
			break;

			default:
				break;
			}

		}//end for(; lNode<g_lDataCopyXmsSize;)

		AUTORUNEND:
		if(g_bDataCopy==FALSE || g_bAutoRunMode==FALSE)
		{
			vm_start();
			vm_tag_stop();											//此库函数调用会打印vm_tag_stop	
			//	Uart_Printf("vm_get_tag1=%ld\n",vm_get_tag());

			while(vm_get_status())
			{
				OSTimeDly(1);	

				if(g_iErrorNo != 0 && g_iErrorNo != 3 && g_iErrorNo != 2 && g_iErrorNo != 8)
				{
					//Uart_Printf("g_iErrorNo100 = %d\n",g_iErrorNo);
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

				if(g_Ini.iAxisRunMode[0]!=2)
				{
					temp = GetPulse(1,TRUE)%g_Ini.lPulse[0];	//x

					if(temp == 0)
					{
						SetMM(1,0,TRUE);
					}
				}

				SetMM(2,0,TRUE);								//y

				if(g_Ini.iAxisRunMode[2]!=2)
				{
					temp = GetPulse(3,TRUE)%g_Ini.lPulse[2];	//z

					if(temp ==0)
						SetMM(3,0,TRUE);
				}


				if(g_Ini.iAxisRunMode[3]!=2)
				{
					temp = GetPulse(4,TRUE)%g_Ini.lPulse[3];	//a

					if(temp ==0)
						SetMM(4,0,TRUE);
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

	/*{
		long pos1,po2;
		static INT8S buf[100];

		memset(buf,NULL,sizeof(buf));

		get_command_pos(1,&pos1);

		sprintf(buf,"pos1=%d,MOD=%d\n",pos1,pos1%16384);

		OSQPost(MsgQueue,buf);

		OSTimeDly(5000);
	}*/

	//g_bCanDispRunTime = FALSE;

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
	ExitScrProtect();

	g_bMode=FALSE;
	g_bStepIO = FALSE;

	SetMM(7,0,FALSE);
	SetMM(8,0,FALSE);

	CloseProbe(0);

	WriteRunCount();

	if(g_Ini.iXianjiaDrive)	
		WriteBit(O_SENDADD,0);


	if(g_Ini.lProbeCount>0 && g_lProbeFail>=g_Ini.lProbeCount)
	{
		if(g_bInterface)
			SuspendTasks(5);

		OSTimeDly(100);													//为了避免出现消息框与坐标值覆盖

		CAfxMsgDlg_Init(250,130,300,200,g_bEnglish?"Probe fail count arrive!":"  探针失败次数到达!","系统警告",REDCOLOR);
		CAfxMsgDlg_DoModal();
		CAfxMsgDlg_Destroy();

		ResumeTasks(5);
		g_lProbeFail = 0;
		CTeach_Create();		
	}

	key_set_led(STEPLED,LEDCLOSE);
	key_set_led(RUNLED,LEDCLOSE);
	key_set_led(STOPLED,LEDOPEN);
	key_set_led(HANDLELED,LEDCLOSE);

	if(g_Ini.lRunCount>=g_Ini.lDestCount)
	{
		if(g_bInterface)
		{
			SuspendTasks(5);
		}

		sprintf(g_pszMsgBuf,g_bEnglish?"%dAmount fill!":"加工数量%d个完成!",g_Ini.lRunCount);

		CAfxMsgDlg_Init(250,130,300,200,g_pszMsgBuf,g_bEnglish?"Waring":"警告",REDCOLOR);
		CAfxMsgDlg_DoModal();
		CAfxMsgDlg_Destroy();

		ResumeTasks(5);

		g_Ini.lRunCount=0;
		//g_lRamRunCounts=g_Ini.lRunCount;
		WriteBit(O_WARNING,0);
		WriteRunCount();
		WriteDataToFile(g_Ini.iFileName);
		SysParaWrite();
		CTeach_Create();
	}

	if(g_Ini.lRunCount <= g_Ini.lDestCount)
	{
		if(g_bMode)
		{
			OSQPost(MsgQueue,g_bEnglish?"TestRun Stop":"测试加工停止");
		}
		else
		{
			if(g_bPerCountFinish)
			{
				g_bPerCountFinish = FALSE;
				WriteBit(2,1);				
				OSQPost(MsgQueue,g_bEnglish?"Count of running arrive!":"一批次产量到达!");
			}
			//else if(g_bAutoRunMode ==FALSE)
			//	OSQPost(MsgQueue,g_bEnglish?"SingleRun stop...":"单条加工停止");
			else //if(g_bAutoRunMode ==TRUE)
				OSQPost(MsgQueue,g_bEnglish?"AutoRun Stop":"自动加工停止");

			if(!g_bInterface)
				CTeach_Update(0,0);
		}
	}

	{
		if((g_iErrorNo>=1 && g_iErrorNo<=7) || g_iErrorNo==15)
		{

			if(g_iErrorNo>3)
			{
				g_bFirstResetX=FALSE;											//为TRUE时需要复位
				g_bFirstResetY=FALSE;
				g_bFirstResetZ=FALSE;
				g_bFirstResetU=FALSE;
			}
			else
			{
				if(g_iErrorNo==1)
				{
					g_bEMSTOP=TRUE;
					g_lStepNode=0;

					if(g_bMode)
					{
						//g_bStepIO=FALSE;
						g_bDispRunLine=FALSE;					
						g_bAutoRunMode=TRUE;
						g_bMode=FALSE;
						CTeach_ModeDisp();
					}
				}
			}
		}

		if(g_iErrorNo)
		{
			RunWarningDisplay(g_iErrorNo);
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

			OSQPost(MsgQueue,g_bEnglish?"System Ready.":"系统准备中");	
			OSTimeDly(200);
		}
	}
}

void	RunWarningDisplay(INT16S no)
{
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
	}

	OSQPost(MsgQueue,g_bEnglish?"Stop Warning!":"急停报警!!!");
	break;

	case 2:	OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning!":"断线报警!!!");			break;
	case 3:	OSQPost(MsgQueue,g_bEnglish?"Low temperature Warning!":"缠线报警!!!");		break;
	case 4:	OSQPost(MsgQueue,g_bEnglish?"X Servo Warning!":"X伺服报警!!!");				break;
	case 5:	OSQPost(MsgQueue,g_bEnglish?"Y Servo Warning!":"Y伺服报警!!!");				break;
	case 6:	OSQPost(MsgQueue,g_bEnglish?"Z Servo Warning!":"Z伺服报警!!!");				break;
	case 7:	OSQPost(MsgQueue,g_bEnglish?"U Servo Warning!":"U伺服报警!!!");				break;
	case 8:	OSQPost(MsgQueue,g_bEnglish?"Wire Running Warning!":"跑线报警!!!");			break;

	case 10:OSQPost(MsgQueue,g_bEnglish?"X Lag Warning!":"X脉冲滞后报警!!!");			break;
	case 11:OSQPost(MsgQueue,g_bEnglish?"Y Lag Warning!":"Y脉冲滞后报警!!!");			break;
	case 12:OSQPost(MsgQueue,g_bEnglish?"Z Lag Warning!":"Z脉冲滞后报警!!!");			break;
	case 13:OSQPost(MsgQueue,g_bEnglish?"U Lag Warning!":"U脉冲滞后报警!!!");			break;

	case 19:OSQPost(MsgQueue,g_bEnglish?"-Z_LIMIT Warning!":"Z轴硬件负限位报警!!!");	break;		//enxuan
	case 20:OSQPost(MsgQueue,g_bEnglish?"+Z_LIMIT Warning!":"Z轴硬件正限位报警!!!");	break;

	case 21:OSQPost(MsgQueue,g_bEnglish?"-Z_LIMIT_S Warning!":"Z轴软件正限位报警!!!");	break;		//enxuan
	case 22:OSQPost(MsgQueue,g_bEnglish?"+Z_LIMIT_S Warning!":"Z轴软件负限位报警!!!");	break;

	case 23:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for Touching Throughout!":"探针一直触碰!!!");		break;
	case 24:OSQPost(MsgQueue,g_bEnglish?"Probe Fail for no Touch!":"探针未触碰!!!");					break;

	case 30:OSQPost(MsgQueue,g_bEnglish?"system error! check data!":"速度异常错误,是否位置差异过大或脉冲当量过大!"); break;
	case 31:OSQPost(MsgQueue,g_bEnglish?"move time err! ":"等待输入信号超时报警!"); break;
	case 32:OSQPost(MsgQueue,g_bEnglish?"error 32!":"等待超时退出!"); break;
	default:
	{
		WriteDataToFile( g_Ini.iFileName );
		WriteRunCount();
		g_bModifySave=TRUE;
	}
	break;
	}
}


void	UpdateLeavingTime(BOOL init,BOOL initRunCount,BOOL disp)
{
	static INT32S  	Hour11=-1;
	static INT32S	Minute11=-1;
	static INT32S	Hour12=-1;
	static INT32S	Minute12=-1;
	static INT32S   Day12=-1;
	static INT32S   lRunCount=0;												//记录本次加工开始时已完成加工数量
	static INT32S   lProbeFailCount=0;											//记录本次加工开始时已有探针失败次数
	INT8S  buf[0x10];
	INT32S tt1=0,tt2=0;
	INT32S Hour1=0,Minute1=0,Second1=0,Day2=0,Hour2=0,Minute2=0,Second2=0;

	if(g_bScrProtectRun)
		return;

	if(init)
	{
		//		CDC_DrawWin(180,180,240,120,"正在加工...");
		//		CDC_OutText24Ex(168,120,g_bEnglish?"Elapse T:":"已用时间:",g_TextColor1,g_nBk);
		//		CDC_OutText24Ex(EDITLEFT,EDITTOP+i*(EDITHEIGHT+10),g_bEnglish?"Remain T:":"剩余时间:",g_TextColor1,g_nTechfrmBk);
		//		Day11   = -1;
		//		Hour11  = -1;
		//		Minute11= -1;

		CDC_OutText_32(227,352,g_bEnglish?"D":"天",g_nRunTextColor,g_nTechfrmBk);
		CDC_OutText_32(285,352,g_bEnglish?"H":"时",g_nRunTextColor,g_nTechfrmBk);
		CDC_OutText_32(342,352,g_bEnglish?"M":"分",g_nRunTextColor,g_nTechfrmBk);
		Day12   = -1;
		Hour12  = -1;
		Minute12= -1;

		//		return;
	}

	if(initRunCount)
	{
		lRunCount=g_Ini.lRunCount;
		lProbeFailCount=g_lProbeFail;
		//		return;
	}


	if(g_fRunSpeed>=1)
	{
		tt1=(INT32S)(((FP32)(g_Ini.lRunCount-lRunCount+g_lProbeFail-lProbeFailCount))/(g_fRunSpeed/60.0));
		tt2=(INT32S)(((FP32)(g_Ini.lDestCount-g_Ini.lRunCount))/(g_fRunSpeed/60.0));
	}
	/*
	Hour1=tt1/3600;
	Minute1=(tt1-Hour1*3600)/60;
	Second1=tt1%60;

	if(Hour1!=Hour11)
	{
		if(disp)
		{
			sprintf(buf,"%03ld:",Hour1);
			CDC_OutText(240,120,buf,g_TextColor1,g_nBk);
			Hour11=Hour1;

		}
	}

	if(Minute1!=Minute11)
	{
		if(disp)
		{
			sprintf(buf,"%02ld:",Minute1);
			CDC_OutText(274,120,buf,g_TextColor1,g_nBk);
			Minute11=Minute1;
		}
	}

	if(disp)
	{
		sprintf(buf,"%02ld",Second1);
		CDC_OutText(298,120,buf,g_TextColor1,g_nBk);
	}
	 */
	Day2   = tt2/(3600*24);
	Hour2  = (tt2-Day2*24*3600)/3600;
	Minute2= (tt2-Day2*24*3600-Hour2*3600)/60;//+((tt2%60)>0?1:0);
	//	Second2= tt2%60;

	if(Day2>999)
		Day2 = 999;

	if(Day2!=Day12)
	{
		if(disp)
		{
			sprintf(buf,"%03ld",Day2);
			CDC_OutText_32(183,352,buf,g_nRunTextColor,g_nTechfrmBk);
			//			CDC_OutText24Ex(234,352,"天",g_nRunTextColor,g_nTechfrmBk);
			Day12=Day2;
		}
	}
	//	if(Hour2>999)
	//		Hour2 = 999;

	if(Hour2!=Hour12)
	{
		if(disp)
		{
			sprintf(buf,"%02ld",Hour2);
			CDC_OutText_32(256,352,buf,g_nRunTextColor,g_nTechfrmBk);
			//			CDC_OutText24Ex(288,352,"时",g_nRunTextColor,g_nTechfrmBk);
			Hour12=Hour2;
		}
	}

	if(Minute2!=Minute12)
	{
		if(disp)
		{
			sprintf(buf,"%02ld",Minute2);
			CDC_OutText_32(313,352,buf,g_nRunTextColor,g_nTechfrmBk);
			//			CDC_OutText24Ex(344,352,"分",g_nRunTextColor,g_nTechfrmBk);
			Minute12=Minute2;
		}
	}
	/*
	if(disp)
	{
		sprintf(buf,"%02ld",Second2);
		CDC_OutText24Ex(683,408,buf,YELLOWCOLOR,g_nTechfrmBk);
	}
	 */
}


void	CloseDisplay(FP32 tt)
{
#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  							/* Prevent compiler warning                           */
#endif

	if(g_Ini.iScrProtect==1)
	{		
		if(!g_bLCD || fabs(tt-0.0)<0.001 || g_bScrProtectRun || g_bMode)
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
			if(g_Ini.iScrProtectBmp  && g_bInitbmp)//修正图片打开失败时，则关闭屏幕20140807
			{
				SuspendTasks(5);
				OSTimeDly(150);
				g_bScrProtectRun=TRUE;						
				OSTimeDly(50);
				//				g_bFirstDispRunTime=TRUE;

				DispBMP("\\PARA\\start.bmp",1,0,0);

			}
			else
			{
				LCD_Power(0);	
				g_bLCD = FALSE;		
			}

			OS_ENTER_CRITICAL();
			g_fCloseDispCount=0.0;
			OS_EXIT_CRITICAL();
		}
	}
}


//#pragma warn -par
void	UpdateProbe(BOOL init)													//刷新显示探针信号状态
{

	INT16S i,j;
	//	INT8S  in[4]={4,3,9,8};
	//	static INT16S oldstatus[4]={-1,-1,-1,-1};
	static INT16S oldstatus=-1;
	static INT16S oldpaoxian=-1;

	//	INT16S status[4]={0};
	INT16S status=0;
	INT16S statuspx=0;

	if(init)
	{
		//		for( j=0; j<4; j++)
		//			oldstatus[j] = -1;
		oldstatus = -1;
		oldpaoxian=-1;
	}

	//	for( i=0; i< 4; i++)
	{
		//		status[i] = ReadBit(in[i]);

		status = ReadBit(4);

		if(status != oldstatus)
		{
			oldstatus = status;

			if(status == 0)
			{
				CDC_PutColorBar(641, 430, 156, 47,GREENCOLOR,100);
				CDC_TextCen_32(640, 428, 156, 49,g_bEnglish?"Probe 1":"探针1",BLACKCOLOR,GREENCOLOR);
			}
			else
			{
				CDC_PutColorBar(641, 429, 156, 48,g_nBackColor,100);
				CDC_TextCen_32(640, 428, 156, 49,g_bEnglish?"Probe 1":"探针1",GREENCOLOR,g_nBackColor);
			}
		}
	}

	statuspx = ReadBit(PAOXIANDI);

	if(statuspx != oldpaoxian)
	{
		oldpaoxian = statuspx;

		if(statuspx == 0)
		{
			CDC_PutColorBar(3*160+1, 430, 158, 47,GREENCOLOR,100);
			CDC_TextCen_32(3*160 , 428, 160, 49,g_bEnglish?"Wire fly":"跑线",BLACKCOLOR,GREENCOLOR);
		}
		else
		{	
			CDC_PutColorBar(3*160+1, 429, 158, 48,g_nBackColor,100);
			CDC_TextCen_32(3*160 , 428, 160, 49,g_bEnglish?"Wire fly":"跑线",GREENCOLOR,g_nBackColor);
		}
	}

}



void	UpdateCylinder(BOOL init)												//刷新显示汽缸状态
{

	INT16S 	i,j;
	static 	INT16S oldstatus[2]={-1,-1};
	static	INT16S oldsongxian = -1;

	INT16S 	status[2]={0};
	INT8S 	string[10]="气缸1";
	INT8S 	stringE[10]="Air 1";

	INT16S 	statussx=0;
	INT8S 	stringsx[10]="送线架";
	INT8S 	stringsxE[10]="Drive";

	INT32S	x=0;

	if(init)
	{
		for(j=0; j<2; j++)
			oldstatus[j] = -1;

		oldsongxian = -1;
	}

	for( i=0; i<2; i++)
	{	
		status[i] = GetOut(i);

		x = i*160;

		if(status[i] != oldstatus[i])
		{
			string[4] = '1'+i;
			stringE[4] = '1'+i;

			oldstatus[i] = status[i];

			if(status[i] == 1)
			{
				CDC_PutColorBar(x+1, 430, 158, 47,GREENCOLOR,100);
				CDC_TextCen_32(x , 428, 160, 49,g_bEnglish?stringE:string,BLACKCOLOR,GREENCOLOR);
			}
			else
			{	
				CDC_PutColorBar(x+1, 429, 158, 48,g_nBackColor,100);
				CDC_TextCen_32(x , 428, 160, 49,g_bEnglish?stringE:string,GREENCOLOR,g_nBackColor);
			}
		}
	}

	statussx = GetOut(SXDRIVEDO);

	if(statussx != oldsongxian)
	{	
		oldsongxian = statussx;

		if(statussx == 1)
		{
			CDC_PutColorBar(2*160+1, 430, 158, 47,GREENCOLOR,100);
			CDC_TextCen_32(2*160 , 428, 160, 49,g_bEnglish?stringsxE:stringsx,BLACKCOLOR,GREENCOLOR);
		}
		else
		{	
			CDC_PutColorBar(2*160+1, 429, 158, 48,g_nBackColor,100);
			CDC_TextCen_32(2*160 , 428, 160, 49,g_bEnglish?stringsxE:stringsx,GREENCOLOR,g_nBackColor);
		}
	}


}


/*void  InitLed()
{
	AllLedOff();

	LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]==1?0:1);  							//送线轴关闭时灯亮	
	LedOut(USWITCHLED,g_Ini.iAxisSwitch[3]==1?0:1);
	LedOut(ZSWITCHLED,g_Ini.iAxisSwitch[2]==1?0:1);

	LedOut(TESTLED,g_bMode?1:0);												//灯亮表示进入测试模式
//	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);									//灯亮表示进入打开手摇轮
	LedOut(STOPLED,1);
	BackZeroLed();
//	AllLedOff();

}
 */

/*void	BackZeroLed()															//判断归零灯是否点亮
{
	BOOL bXStatus=g_bFirstResetX?TRUE:(g_Ini.iAxisSwitch[0]==1?FALSE:TRUE);
	BOOL bYStatus=g_bFirstResetY?TRUE:(g_Ini.iAxisSwitch[1]==1?FALSE:TRUE);
	BOOL bZStatus=g_bFirstResetZ?TRUE:(g_Ini.iAxisSwitch[2]==1?FALSE:TRUE);
	BOOL bUStatus=g_bFirstResetU?TRUE:(g_Ini.iAxisSwitch[3]==1?FALSE:TRUE);

	g_pszLedOut[4][0] = BACKZEROLED;
	g_pszLedOut[4][1] = (bXStatus&&bYStatus&&bZStatus&&bUStatus)?0:1;
	OSQPost(LedOutQueue,&g_pszLedOut[4]);		
}
 */

BOOL	WorkFindZero(INT16S ch)
{
	INT8S a=0;

	for(a=1; a<5; a++)
		ResumeTasks(a);

	OSTimeDly(20);
	g_bBackZero=TRUE;
	g_iErrorNo=0;

	switch(ch)
	{
	case 1:
		OSQPost(MsgQueue,g_bEnglish?"X Reset...":"X回零点...");
		break;
	case 3:
		OSQPost(MsgQueue,g_bEnglish?"Z Reset...":"Z回零点...");
		break;
	case 4:
		OSQPost(MsgQueue,g_bEnglish?"A Reset...":"A回零点...");
		break;
	default:
		break;
	}

	if( !FindZero(ch,g_Ini.iBackMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
	{
		switch(ch)
		{
		case 1:
			OSQPost(MsgQueue,g_bEnglish?"X Reset Error!":"X回零点中断!");
			break;
		case 3:
			OSQPost(MsgQueue,g_bEnglish?"Z Reset Error!":"Z回零点中断!");
			break;
		case 4:
			OSQPost(MsgQueue,g_bEnglish?"A Reset Error!":"A回零点中断!");
			break;
		default:
			break;
		}

		g_bBackZero=FALSE;

		for(a=1; a<5; a++)
			SuspendTasks(a);

		OSTimeDly(50);

		return FALSE;
	}

	switch(ch)
	{
	case 1:
		OSQPost(MsgQueue,g_bEnglish?"X Reset End!":"X回零完成!");
		break;
	case 3:
		OSQPost(MsgQueue,g_bEnglish?"Z Reset End!":"Z回零完成!");
		break;
	case 4:
		OSQPost(MsgQueue,g_bEnglish?"A Reset End!":"A回零完成!");
		break;
	default:
		break;
	}

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

FP32 GetYLong(INT32S index)						//用于查找并返回索引行之前的不同的Y值，用于Y座标显示
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
						VAR= GetMMFromPulse(2,pData.lValue[1]);
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

BOOL ExitScrProtect(void)				//若屏保中，则退出屏保,返回值为TRUE时表示有进行退出屏保动作
{	
	BOOL bDoExit=FALSE;

	if(g_Ini.iScrProtect == 1)
	{

		if(!g_bLCD || g_bScrProtectRun)
		{
			if(g_Ini.iScrProtectBmp  && g_bInitbmp)
			{
				g_bScrProtectRun=FALSE;
				CTeach_Create();
				ResumeTasks(5);
			}
			else
			{

				LCD_Power(1);	
				g_bLCD = TRUE;		
			}

			bDoExit=TRUE;
		}
	}

	return bDoExit;
}
void	UpdateHandBox(BOOL flag)
{/*	
	CRect   rc;	

	rc.left = 700;
	rc.top =  415;
	rc.right = 780;
	rc.bottom = 471;
	g_Ini.iHandStatus = flag?1:0;

	CDC_DrawButton(rc,g_nBk);
	CDC_OutText24Ex(705,430,  g_bEnglish?"HandB":"手持盒",WHITECOLOR,g_nBk);

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
	}*/
}
