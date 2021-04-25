/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        runtask.c
  创建人：       范志州
  创建日期：		2004/11/19
  主要功能描述:  加工过程中执行之任务 

  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/04
       主要修改内容:   
   2. ...
 *************************************************/

# include   "includes.h"
# include   "StrTool.h"
# include 	"base.h"

extern 		BASEINI		g_Ini;
extern 		COMMAND		Line_Data[MAXLINE];
extern 		INT16S		Line_Data_Size;
extern 		INT16U		g_uBmpXMS;
TEST_EXT	BOOL		g_bRelease;
TEST_EXT	BOOL		g_bPM;
//TEST_EXT	BOOL		g_bBmp;
TEST_EXT	OS_EVENT	*RunSpeedCountSem;
TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT  	OS_EVENT	*UnpackDataSem;
TEST_EXT    OS_EVENT 	*RunLineQueue;
TEST_EXT	OS_EVENT	*DispRunTimeSem;
TEST_EXT    OS_EVENT 	*MsgQueue;
TEST_EXT  	OS_EVENT 	*LedOutQueue;
TEST_EXT	BOOL		g_bEnglish;

WORK_EXT 	INT8S		g_pszMsgBuf[0x30];
WORK_EXT	INT8S		g_pszLedOut[8][3];
WORK_EXT	INT16S 		g_iRunLine;
WORK_EXT 	INT16S		g_iStaRunLine;
WORK_EXT 	BOOL 	 	g_bScreen;	   											//指示当前屏幕显示那个界面，TRUE显示主加工界面
WORK_EXT 	BOOL	  	g_bFirstDispTime;										//指示是否需要全部刷新日期时间显示，TRUE需要
WORK_EXT	BOOL		g_bFirstDispRunTime;
WORK_EXT	BOOL		g_bDispRunTime;
WORK_EXT	INT32S		g_lRunTime;
WORK_EXT	FP32   		g_fRunSpeed;
WORK_EXT 	BOOL   		g_bStartRun;  
WORK_EXT 	BOOL		g_bRuning;    
WORK_EXT 	BOOL		g_bModify;  
WORK_EXT	BOOL		g_bModifySave;
WORK_EXT	BOOL		g_bUnpackRuning;
WORK_EXT	BOOL		g_bHandset;
WORK_EXT	INT16S		g_iMoveAxis;
WORK_EXT 	BOOL		g_bFirstResetX;
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT 	INT16S		g_iWorkStatus;
WORK_EXT 	BOOL		g_bAutoRunMode;
WORK_EXT 	INT32S		g_lStepNode;	
WORK_EXT 	BOOL		g_bStepIO;  
WORK_EXT 	BOOL		g_bDispRunLine;
WORK_EXT 	BOOL		g_bIsHand;	   
WORK_EXT 	BOOL		g_bStepToSingle;
WORK_EXT 	BOOL		g_bMode;	   
WORK_EXT	BOOL		g_bEMSTOP; 
WORK_EXT	INT16S		g_iADValue;
WORK_EXT	BOOL		g_bTestStart;
WORK_EXT	BOOL		g_bHandRun;
WORK_EXT	INT32U		g_lHandRun;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	INT16S      g_iOutStatus;
WORK_EXT	BOOL		g_bPressStartIO;
WORK_EXT	BOOL		g_bPressStartIO2;
WORK_EXT	BOOL		g_bPressStepIO2;
WORK_EXT	BOOL		g_bBackZero;											//指示是否为回零过程
WORK_EXT	INT16S		g_iErrorNo; 
WORK_EXT	FP32		g_lSpeed;
WORK_EXT 	INT16S		g_iWorkNumber;
WORK_EXT 	INT16S		g_iSpeedBeilv;
WORK_EXT	BOOL		g_bScrProtectRun;
WORK_EXT 	INT32S  	g_lXmsSize;      
WORK_EXT 	INT32S		g_lYCountLen;       									//加工过程中Y轴的补偿量
WORK_EXT 	INT32S		g_lZCountLen;       									//加工过程中Y轴的补偿量
WORK_EXT	FP32 		g_fCloseDispCount;
WORK_EXT	INT16U		g_uiSuspendTask;										//suspend 0~5 task,high byte rev
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;
WORK_EXT	INT32S		g_nTechfrmBk;
WORK_EXT	BOOL		g_bLCD;
WORK_EXT 	BOOL		g_bModify1;  
WORK_EXT	INT16S		g_iCheckStatusIO;
WORK_EXT	INT32S 		g_nBackColor;
WORK_EXT 	INT32S  	g_lCurLine;
WORK_EXT	INT32S 		g_nRunTextColor;
INT32S		g_iProbe[4]={TTANZHEN1,TTANZHEN2,TTANZHEN3,TTANZHEN4};

extern int 	g_bMode2;
INT32U      Suspended = 0;
extern     	INT32S 		g_iCheckXian;
extern 		BOOL        g_bCheckDiffer;
extern 		INT32S      g_iCheckTime;
extern 		BOOL        g_bCheckLay;
extern 		INT32S 		g_lHandPulse;
extern		BOOL		g_bTestFrm;
extern		INT16U		g_iTestAxis;
extern		BOOL		g_bInterface;
extern		INT32S		g_iStopScrTime;												//停机屏保计时
BOOL		g_bIsRun = FALSE;														//是否在运行中，主要是停机屏保时间清零
extern		BOOL		g_bProbeLagCheck;											//有探针动作时，是否进行逻辑脉冲与实际脉冲滞后检测
extern      INT32S      g_bInitbmp;
extern      BOOL		g_bSingleRun ;	
extern 		BOOL        g_bRunPause;
WORK_EXT 	FP32		g_fStepSpeed[4];



void	Check_Probe_Task(void *data)
{
	INT16S  	Count=0,number=0;     
	BOOL    	predir=TRUE;
	BOOL    	curdir=TRUE;
	INT32S 		pos=0;
	FP32 		mv=0;

#if OS_CRITICAL_METHOD == 3                     	 						/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                 							/* Prevent compiler warning                           */
#endif

	data = data;

	for (;;)
	{

		if(g_iWorkStatus==NORUN && g_Ini.iScrProtect==1 && !g_bIsRun)
		{
			CloseDisplayStop();
		}

		else if(g_bIsRun)
		{
			g_iStopScrTime = OSTimeGet();
		}

		if(g_iWorkStatus==NORUN && !g_bMode)
		{
			if(g_iMoveAxis<5 && g_iMoveAxis!=0)  								//借用此任务处理手摇轮
			{
				Count++;

				if(Count > g_Ini.iHandRunSpeed)
				{
					INT32U t2=0;
					INT32S tmp=0;

					Count= 0;
					t2   = OSTimeGet();
					tmp  = t2-g_lHandRun;

					if(tmp <= 0)
						tmp = 1;

					g_lHandRun = t2;

					pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					if(pos>0)
						curdir = TRUE;						
					else
						curdir = FALSE;

					if(pos!=0)
					{
						if(predir != curdir)
						{
							predir = curdir;

							OS_ENTER_CRITICAL();
							Stop1(g_iMoveAxis);
							OS_EXIT_CRITICAL();
						}
						else
						{

							FP32 CurSpeedBi;

							if(!IsFree(g_iMoveAxis) && pos!=0)
							{
								pos = pos*50;  												//放大倍数可以使得手摇的反应更加灵敏								
								CurSpeedBi = (FP32)(labs(pos))/(tmp*g_Ini.fHandRunRate); 	//20061013更改之方法

								if(CurSpeedBi > 1.0)
									CurSpeedBi = 1.0;

								if(fabs(g_fSpeedBi-CurSpeedBi) > 0.02)
								{
									INT32S speed = 0;

									g_fSpeedBi = CurSpeedBi;
									speed = (INT32S)((FP32)g_lSpeed*g_fSpeedBi);

									if(speed<2)
										speed = 2;

									OS_ENTER_CRITICAL();
									set_startv(g_iMoveAxis,speed);
									set_speed(g_iMoveAxis,speed);
									OS_EXIT_CRITICAL();
								}
							}

						}
					}
					else
					{
						OS_ENTER_CRITICAL();
						Stop1(g_iMoveAxis);
						OS_EXIT_CRITICAL();
					}
				}	

				number++;

				if(number>=3)																		//end if(Count>g_Ini.iHandRunSpeed)
				{
					FP32 temp;				
					number=0;

					if(g_iMoveAxis==2 && g_Ini.iAxisSwitch[1]==1)
					{
						temp = GetMM(g_iMoveAxis,TRUE);

						if(mv != temp)
						{
							mv = temp;

							if( !g_bScreen && (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] == 'M' )
							{
								CEdit_SetFloat(mv,2);
								CEdit_UpdateData(FALSE);
							}
						}
					}
					else
					{
						INT32S curPos =0;
						INT32S QuanPos = 0;
						INT32S AnglePos=0;	

						if(g_Ini.iAxisSwitch[g_iMoveAxis-1]==1)
						{
							switch(g_iMoveAxis)
							{
							case 1:
								temp=GetAngle(g_iMoveAxis,TRUE);
								break;

							case 3:
								if(g_Ini.iAxisRunMode[2]==0)
									temp=GetAngle(g_iMoveAxis,TRUE);
								else if(g_Ini.iAxisRunMode[2]==1)			//多圈
								{
									curPos=(INT32S)(GetMM(g_iMoveAxis,TRUE));//不能用GetAngle，因为GetAngle获取的值小于等于360。当一圈对应360°时，GetAngle与GetMM的分度单位对应的脉冲数是一样的
									QuanPos = curPos/360;
									AnglePos = curPos%360;

									temp = QuanPos + AnglePos/1000.0;
								}
								else if(g_Ini.iAxisRunMode[2]==2)
									temp=GetMM(g_iMoveAxis,TRUE);
								break;

							case 4:
								if(g_Ini.iAxisRunMode[3]==0)
									temp=GetAngle(g_iMoveAxis,TRUE);
								else if(g_Ini.iAxisRunMode[3]==1)
								{
									curPos=(INT32S)(GetMM(g_iMoveAxis,TRUE));
									QuanPos = curPos/360;
									AnglePos = curPos%360;

									temp = QuanPos + AnglePos/1000.0;
								}
								else if(g_Ini.iAxisRunMode[3]==2)
									temp=GetMM(g_iMoveAxis,TRUE);
								break;

							default:
								break;
							}

							if(mv != temp)
							{
								mv = temp;

								if( !g_bScreen && (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] == 'M' )
								{
									CEdit_SetFloat(mv,1);
									CEdit_UpdateData(FALSE);
								}
							}
						}
					}
				}
			}

			OSTimeDly(10);
		}
		else
		{
			OSTimeDly(5);
		}


#ifdef	DEBUGDISP
		CDC_PutColorBar(56,10,16,20,BLACKCOLOR,1);
		CDC_OutText(56,10,"T5",YELLOWCOLOR,BLACKCOLOR);
#endif

		if((g_uiSuspendTask&0x0001) != 0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}



void    Run_CheckIO_Task(void *data)
{

	INT16U key,tkey=0;
	INT16S Count=0;

#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                 	 						/* Prevent compiler warning                           */
#endif

	data = data;

	for(;;)
	{		
		if(g_bModify && !g_bUnpackRuning && !g_bBackZero)
		{
			OSSemPost(UnpackDataSem);
		}

		key = KeyMBoxGetKey();

		if(key != 0xffff)
		{ 	 		    		   
			if(!ExitScrProtect())				//ExitScrProtect()假时，表明之前不在屏保状态，可以响应各个按键的具体功能
			{
				OS_ENTER_CRITICAL();
				g_fCloseDispCount=0.0;
				OS_EXIT_CRITICAL();

				switch(key)
				{	
				case TSAVE:
				{
					OSQPost(MsgQueue,g_bEnglish?"Save...":"保存...");
					WriteDataToFile(g_Ini.iFileName);
					g_bModify = TRUE;
					OSQPost(MsgQueue,g_bEnglish?"Saved":"文件保存!");
				}
				break;

				case TSTEP:
				{
					if(g_bBackZero)
					{
						switch(g_Ini.iAxisCount)
						{
						case 4:Stop1(4);
						case 3:Stop1(3);
						case 2:Stop1(2);
						default:Stop1(1);
						}
						break;
					}

					if (WireWarning()) break;

					g_bMode=FALSE;

					key_set_led(STEPLED,LEDOPEN);
					key_set_led(RUNLED,LEDCLOSE);
					key_set_led(HANDLELED,LEDCLOSE);

					//g_bStepIO=FALSE;
					g_bDispRunLine=FALSE;

					if(g_Ini.iXianjiaDrive)
					{
						WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]==1?1:0);
					}

					g_iErrorNo=0;

					SetRange(8000);
					OSQPost(MsgQueue,g_bEnglish?"The Last Spring...":"加工最后一条...");
					g_bAutoRunMode=FALSE;
					CTeach_ModeDisp();
				}

				case TSTOP:
				{
					if(g_bBackZero)		//20081201解决多次按单条键死机问题
					{
						Stop1(4);
						Stop1(3);
						Stop1(2);
						Stop1(1);
						break;
					}

					if(!g_bAutoRunMode)
					{
						break;
					}

					if (WireWarning()) break;

					g_bMode=FALSE;

					key_set_led(STEPLED,LEDOPEN);
					key_set_led(RUNLED,LEDCLOSE);
					key_set_led(HANDLELED,LEDCLOSE);

					g_bDispRunLine=FALSE;

					if(g_Ini.iXianjiaDrive)
					{
						WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]==1?1:0);
					}

					g_iErrorNo=0;

					SetRange(8000);
					OSQPost(MsgQueue,g_bEnglish?"The Last Spring...":"加工最后一条...");
					g_bAutoRunMode=FALSE;
					CTeach_ModeDisp();

					break;
				}

				case THANDMOVE:
				{
					if(WireWarning())
						break;

					g_iErrorNo=0;

					if(!g_bMode)              //手动，模式键
					{
						g_bMode=TRUE;
						g_bTestStart = TRUE;
						//g_bStepIO = TRUE;
						g_bDispRunLine=TRUE;
						key_set_led(HANDLELED,LEDOPEN);
						OSQPost(MsgQueue,g_bEnglish?"Goto HandMode":"进入手动模式!");
						CTeach_ModeDisp();
					}

					if(g_Ini.iXianjiaDrive)
					{
						WriteBit(O_SENDADD,0);
					}

				}
				break;

				case TRUN:
				{
					if(WireWarning())
						break;

					OS_ENTER_CRITICAL();
					//g_bStepIO=FALSE;
					g_bDispRunLine=FALSE;
					g_bAutoRunMode=TRUE;
					g_bSingleRun=FALSE;
					g_bMode=FALSE;
					OS_EXIT_CRITICAL();
					CTeach_ModeDisp();
					g_iErrorNo=0;

					OSQPost(MsgQueue,g_bEnglish?"Start AutoRun...":"开始自动加工...");

					key_set_led(STEPLED,LEDCLOSE);
					key_set_led(RUNLED,LEDOPEN);
					key_set_led(HANDLELED,LEDCLOSE);

					if(g_Ini.iXianjiaDrive)
					{
						WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]==1?1:0);
					}

					SetRange(8000);

				}
				break;

				case TCANCEL:
				case TSCREEN:
				case TPAGEUP:
				case TPAGEDOWN:
				case TUP:
				case TDOWN:
				case TLEFT:
				case TRIGHT:
				case TENTER:
				case TINSERT:
				case TDEL:
				case TPROBESET:
				case TX:case TY:case TZ:case TRATE:
				case '0': case '1': case '2': case '3': case '4': case '5':
				case '6': case '7': case '8': case '9':
				case TCLEAR:case '-':case '.':

					if(g_bBackZero)
						break;
					else
					{
						if(!g_bDispRunTime)
							CTeach_OnKey(key,tkey);
					}
					break;

				default:
					break;

				}
			}
		}

		if(g_iWorkStatus==RUNNING && RealIO(SINGLEIO,IOPRESS,3))				//手执盒IO单条
		{
			g_iWorkStatus=SINGLERUN;

			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
				continue;
			}

			if (WireWarning()) 
				continue;

			g_bMode=FALSE;

			g_bDispRunLine=FALSE;

			if(g_Ini.iXianjiaDrive)
			{
				WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]==1?1:0);
			} 

			g_iErrorNo=0;

			SetRange(8000);
			OSQPost(MsgQueue,g_bEnglish?"The Last Spring...":"加工最后一条...");							
			g_bAutoRunMode=FALSE;							
			CTeach_ModeDisp();	

			key_set_led(STEPLED,LEDOPEN);
			key_set_led(RUNLED,LEDCLOSE);

		}

		if(g_Ini.iSwhSingleIo==1)
		{
			if(!g_bPressStepIO2 && g_iWorkStatus==RUNNING && RealIO(SINGLEDI,1,3))	 			//IO口单条
			{
				g_bPressStepIO2=TRUE;
				g_iWorkStatus=SINGLERUN;

				if(g_bBackZero)
				{
					switch(g_Ini.iAxisCount)
					{
					case 4:Stop1(4);
					case 3:Stop1(3);
					case 2:Stop1(2);
					default:Stop1(1);
					}
					continue;
				}

				if (WireWarning())
					continue;

				g_bMode=FALSE;

				g_bDispRunLine=FALSE;

				if(g_Ini.iXianjiaDrive)
				{
					WriteBit(O_SENDADD,g_Ini.iAxisSwitch[1]==1?1:0);
				}

				g_iErrorNo=0;

				SetRange(8000);
				OSQPost(MsgQueue,g_bEnglish?"The Last Spring...":"加工最后一条...");
				g_bAutoRunMode=FALSE;
				CTeach_ModeDisp();

				key_set_led(STEPLED,LEDOPEN);
				key_set_led(RUNLED,LEDCLOSE);
			}
		}

		if(g_bPressStepIO2 && g_iWorkStatus==RUNNING && RealIO(SINGLEDI,0,3))      
		{
			g_bPressStepIO2=FALSE;
		}

		if(g_bMode && !g_bStepIO)
		{		
			if(WireWarning())			//手摇加工若缠/断/跑线报警，则需手摇无反应
			{
				g_fSpeedBi = 0.0;
				SetRange(0);
				g_bRunPause=TRUE;
				g_bHandRun = FALSE;	
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);
			}
			else if(!g_bBackZero)
			{
				g_iErrorNo=0;

				Count++;

				if(Count > g_Ini.iHandRunSpeed*3)
				{
					INT32U t2=0;
					INT32S tmp=0;
					static INT32S pos=0;

					Count= 0;
					t2   = OSTimeGet();
					tmp  = t2-g_lHandRun;

					if(tmp <= 0)
						tmp = 1;

					g_lHandRun = t2;

					pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);



					if(pos>0)										
					{
						FP32 CurSpeedBi=0.01;

						if(g_bHandRun==FALSE)
						{
							g_bHandRun = TRUE;
							OSQPost(MsgQueue,g_bEnglish?"Start Handwheel Run...":"开始手摇运行...");
						}

						pos = pos*50;  										//防CNC820 836的响应过程因为MXC312手轮手一格是1，而FPGA是4

						CurSpeedBi=(FP32)(pos)/(tmp*g_Ini.fHandRunRate);

						if(CurSpeedBi>1.0)
							CurSpeedBi=1.0;

						if(fabs(g_fSpeedBi-CurSpeedBi)>0.04)
						{
							g_fSpeedBi=CurSpeedBi;
							SetRange((INT32S)(g_fSpeedBi*8000));
						}			
					}
					else
					{		
						g_fSpeedBi = 0.0;
						SetRange(0);
						g_bRunPause=TRUE;

						if(g_bHandRun == TRUE)
						{
							g_bHandRun = FALSE;
						}					
					}
				}																//end if(Count>g_Ini.iHandRunSpeed)
			}			
		}

		if(g_bMode && !g_bHandRun)											//寸动键松开停止
		{
			if(RealIO(STEPDI,IOREALSE,3))
			{
				g_bStepIO=FALSE;
				SetRange(0);
				g_bRunPause=TRUE;
			}
			else if(RealIO(STEPDI,IOPRESS,3))
			{				
				if(!g_bStepIO)
				{
					g_bStepIO=TRUE;

					if(WireWarning()==0)
					{
						g_iErrorNo=0;
						OSQPost(MsgQueue,g_bEnglish?"Start JOG Run...":"开始寸动运行...");
						SetRange((INT32S)(g_fStepSpeed[g_iSpeedBeilv-1]*8000));
					}
				}	
			}
		}

		if(g_bMode && g_iWorkStatus==RUNNING)
		{
			INT16S itempRunLine  = (((vm_getindex()%500)+1)/2);//(((vm_getindex()%500)+1)/2)-1;

			if(g_iRunLine != itempRunLine)
			{
				g_iRunLine = itempRunLine;
				OSQFlush(RunLineQueue);								// Clear queue
				OSQPost(RunLineQueue,(void *)&g_iRunLine);
			}
		}

		if(g_Ini.iSwhRunIo ==1)
		{
			if(!g_bPressStartIO && RealIO(RUNDI,0,2))								//IO口启动
			{
				g_bPressStartIO=TRUE;

				if(RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2))
					continue;

				g_iErrorNo=0;

				if(g_bStepIO)
				{
					OS_ENTER_CRITICAL();

					g_bStepIO=FALSE;
					g_bDispRunLine=FALSE;
					g_bMode=FALSE;
					g_bHandRun = FALSE;

					OS_EXIT_CRITICAL();

					OS_ENTER_CRITICAL();
					g_fCloseDispCount=0.0;
					OS_EXIT_CRITICAL();
				}

				OSQPost(MsgQueue,g_bEnglish?"Start Auto Run...":"开始自动加工...");

				key_set_led(STEPLED,LEDOPEN);
				key_set_led(RUNLED,LEDCLOSE);

				SetRange(8000);

				if(g_Ini.iAxisSwitch[1]==1)
					WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);
			}
		}

		if(g_bPressStartIO && RealIO(RUNDI,1,2))
		{
			g_bPressStartIO=FALSE;
		}

		if(RealIO(STOPIO,0,2) || RealIO(STOPIO2,0,3))							//紧急停止
		{
			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
			}
			else
			{
				g_iErrorNo=1;
				g_bDispRunLine=FALSE;

				g_bMode=FALSE;
				vm_stop(0);

				//CTeach_ModeDisp();	
			}	

		}

		{			
			if(g_Ini.iLag[0]!=0 && (labs(GetPulse(1,TRUE)*g_Ini.iNumerator[0]-GetPulse(1,FALSE)*g_Ini.iDenominator[0]) >= g_Ini.iLag[0]))
			{
				g_iErrorNo=10;
			}

			if(g_Ini.iLag[1]!=0 && g_bProbeLagCheck && (labs(GetPulse(2,TRUE)*g_Ini.iNumerator[1]-GetPulse(2,FALSE)*g_Ini.iDenominator[0]) >= g_Ini.iLag[1]))
			{
				g_iErrorNo=11;
			}

			if(g_Ini.iAxisCount>2)
			{
				if(g_Ini.iLag[2]!=0 && (labs(GetPulse(3,TRUE)*g_Ini.iNumerator[2]-GetPulse(3,FALSE)*g_Ini.iDenominator[2]) >= g_Ini.iLag[2]))
				{
					g_iErrorNo=12;
				}
			}

			if(g_Ini.iAxisCount>3)
			{
				if(g_Ini.iLag[3]!=0 && (labs(GetPulse(4,TRUE)*g_Ini.iNumerator[3]-GetPulse(4,FALSE)*g_Ini.iDenominator[3]) >= g_Ini.iLag[3]))
				{
					g_iErrorNo=13;
				}
			}
		}

		{
			//if(g_Ini.iAxisCount>2&&(g_Ini.iAxisRunMode[2]==2))					//在RunAllData函数的回零动作中，将g_bBackZero置为TRUE，并且将g_iErrorNo置为0
			if(g_Ini.iAxisCount>2)
			{																			
				if(!g_bBackZero)													//非归零模式检测硬限位和软限位		
					CheckSoftLimit(3,0);                                  		

				if(g_Ini.iZeroReplaceLimit[2]==0)									//如果不是原点当限位，始终检测硬限位
				{
					CheckHardLimit(3,0);
				}
				else if(g_Ini.iAxisRunMode[2]==2)									//如果是原点当限位，则归零过程不检测硬限位
				{
					if(!g_bBackZero)
					{
						CheckHardLimit(3,0);
					}

				}				

			}

			//if(g_Ini.iAxisCount>3&&(g_Ini.iAxisRunMode[3]==2))
			if(g_Ini.iAxisCount>3)
			{				
				CheckHardLimit(4,0);

				if(!g_bBackZero)		
					CheckSoftLimit(4,0);		
			}		
		}

		if(g_bRelease)
		{
			if( RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2))
			{
				g_iErrorNo=2;
				SetRange(0);
				g_bRunPause=TRUE;

				if(g_Ini.iXianjiaDrive)	
					WriteBit(O_SENDADD,0);

				OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning!":"断线报警!加工暂停...");
			}
		}

		if( RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2) && g_iErrorNo!=3)
		{
			g_iErrorNo=3;
			SetRange(0);
			g_bRunPause=TRUE;

			if(g_Ini.iXianjiaDrive)	
				WriteBit(O_SENDADD,0);

			OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning!":"缠线报警!加工暂停...");
		}

		if(g_Ini.iPaoXian)
		{
			if( RealIO(PAOXIANDI,IOPRESS,2) && g_iErrorNo!=8)
			{
				g_iErrorNo=8;
				SetRange(0);
				g_bRunPause=TRUE;


				if(g_Ini.iXianjiaDrive)	 
					WriteBit(O_SENDADD,0);

				OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning!":"跑线报警!加工暂停...");
			}
		}

		if( g_Ini.iAxisSwitch[0]==1 && RealIO(XSERVOWARNING,g_Ini.iServoLogic[0],2))
			g_iErrorNo=4;

		if( g_Ini.iAxisSwitch[1]==1 && RealIO(YSERVOWARNING,g_Ini.iServoLogic[1],2))
			g_iErrorNo=5;

		if(g_Ini.iAxisCount>2)
		{
			if( g_Ini.iAxisSwitch[2]==1 && RealIO(ZSERVOWARNING,g_Ini.iServoLogic[2],2))
				g_iErrorNo=6;
		}

		if(g_Ini.iAxisCount>3)
		{
			if( g_Ini.iAxisSwitch[3]==1 && RealIO(USERVOWARNING,g_Ini.iServoLogic[3],2))
				g_iErrorNo=7;
		}

		if(vm_get_errNo())
		{
			g_iErrorNo=30;

		}

		if((g_iErrorNo>1 && g_iErrorNo<=13 && g_iErrorNo!=3 && g_iErrorNo!=2 &&  g_iErrorNo!=8)|| (g_iErrorNo>=19 && g_iErrorNo<=22) || g_iErrorNo == 30)
		{
			OS_ENTER_CRITICAL();

			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
			}	

			Uart_Printf("g_iErrorNo=%d !!!!!\n",g_iErrorNo);

			vm_stop(0);

			g_iWorkStatus = NORUN;

			OS_EXIT_CRITICAL();
		}

#ifdef	DEBUGDISP
		CDC_PutColorBar(32,10,16,20,g_nBk,1);
		CDC_OutText(32,10,"T4",g_TextColor2,g_nBk);
#endif

		OSTimeDly(6);

		if((g_uiSuspendTask&0x0002)!=0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}


void    Display_RunSpeedCount_Task(void *data)
{
	INT8U  err;

	data = data;

	for(;;)
	{
		OSSemPend(RunSpeedCountSem,0,&err);

		if(err==OS_NO_ERR)
		{

			if(g_lRunTime!=0)
			{
				if(g_lRunTime > 20*OS_TICKS_PER_SEC)
					g_fRunSpeed=(INT32S)((FP32)(60*OS_TICKS_PER_SEC)/g_lRunTime);	
				else
					g_fRunSpeed=(INT32S)(((FP32)(60*OS_TICKS_PER_SEC)/g_lRunTime)*0.7+g_fRunSpeed*0.3);
			}
			else
				g_fRunSpeed=100.0;

			if(g_fRunSpeed>999.0)
				g_fRunSpeed=999.0;

			if(!g_bScrProtectRun && g_bInterface)
			{
				UpdateSpeedCountProbe();
				UpdateLeavingTime(FALSE,FALSE,TRUE);
			}

			if(g_bAutoRunMode && !g_bMode && g_iWorkStatus==RUNNING)			//防止自动转向单条屏保时，只能先按启动键再按其他键才能退出屏保的问题
				CloseDisplay(((FP32)g_lRunTime)/OS_TICKS_PER_SEC);


		}
		else
		{

			switch(err)
			{
			case OS_TIMEOUT:
				OSQPost(MsgQueue,"Timeout!");
				break;

			default	:
				OSQPost(MsgQueue,"undefine error!");
				break;
			}
		}

		if((g_uiSuspendTask&0x0004)!=0)
			OSTaskSuspend(OS_PRIO_SELF);

	}
}


void    DispRunTime_Task(void *data)
{
	INT8U err;

#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  							/* Prevent compiler warning                           */
#endif

	data=data;

	for(;;)
	{
		OSSemPend(DispRunTimeSem,0,&err);

		if(err==OS_NO_ERR)
		{
			/*		if(g_bFirstDispRunTime)
			{
				g_bFirstDispRunTime=FALSE;
				UpdateLeavingTime(TRUE,TRUE,FALSE);
			}
			else
			{
				//UpdateLeavingTime(FALSE,TRUE,FALSE);	
			}
			 */
		}
	}
}


void    Display_RunLine_Task(void *data)
{
	INT16S  runline;
	INT8U	err;

	data = data;

	for(;;)
	{
		runline = *(INT16S *)OSQPend(RunLineQueue,0,&err);

		if(err == OS_NO_ERR)
		{
			if(!g_bScrProtectRun && !g_bInterface)
			{
				RunGotoLine(runline,FALSE);
			}
		}
		else
		{
			switch(err)
			{
			case OS_TIMEOUT:
				OSQPost(MsgQueue,"Timeout!");
				break;

			case OS_ERR_EVENT_TYPE:
				OSQPost(MsgQueue,"Event Type Err!");
				break;

			case OS_ERR_PEND_ISR:
				OSQPost(MsgQueue,"Pend Isr Err!");
				break;

			case OS_ERR_PEVENT_NULL:
				OSQPost(MsgQueue,"PEvent Null Err!");
				break;

			default:
				break;
			}
		}

		{
			OS_Q_DATA qdata;

			err = OSQQuery(RunLineQueue,&qdata);

			if(err == OS_NO_ERR)
			{
				if(qdata.OSNMsgs == 0)  										//队列为NULL时才能挂起该任务
				{
					if((g_uiSuspendTask&0x0010)!=0)
						OSTaskSuspend(OS_PRIO_SELF);
				}
				else
				{
					if(qdata.OSNMsgs>18)
						OSQPost(MsgQueue,"RunLineQueue Overflow!");
				}
			}
		}
	}
}

void	CloseDisplayStop(void)
{
	if(g_bScrProtectRun || !g_bLCD || g_iErrorNo!=0)
	{
		g_iStopScrTime = OSTimeGet();
		return;
	}

	if((OSTimeGet()-g_iStopScrTime)/1000.0/60.0 > g_Ini.iScrProtectTime)
	{
		if(g_Ini.iScrProtectBmp  && g_bInitbmp)
		{
			g_bScrProtectRun=TRUE;
			SuspendTasks(5);
			OSTimeDly(50);					
			DispBMP("\\PARA\\start.bmp",1,0,0);
		}
		else
		{
			LCD_Power(0);	
			//	g_bScrProtectRun =TRUE;
			g_bLCD = FALSE;	
		}
	}
}

void    Display_XYZ_Task(void *data)
{
	INT16S	Count=0; 
	INT32S  pos=0;
	BOOL	predir=TRUE;
	BOOL	curdir=TRUE;
	BOOL	IsRun=FALSE;
	INT16U	key=0;

#if OS_CRITICAL_METHOD == 3                      						/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  						/* Prevent compiler warning                           */
#endif

	data=data;	

	for(;;)
	{		
		if(!g_bScrProtectRun && !g_bTestFrm)
		{
			if(g_bInterface)
			{
				UpdateProbe(FALSE);
				UpdateCylinder(FALSE);
			}


			DisplayXYZ(TRUE);

			if(g_iWorkStatus==RUNNING || g_iWorkStatus==SINGLERUN)
			{
				INT8S buf[0x10];
				INT32S lCurLine=0;

				lCurLine=((vm_getindex()%500)+1)/2;

				if(g_lCurLine!=lCurLine)								//行号有变化才更新显示
				{
					g_lCurLine=lCurLine;
					ltoa(g_lCurLine,(INT8U *)buf);
				}

				if(g_bInterface)
				{
					CDC_OutTextLR_UTF(236,294,buf,g_nRunTextColor,g_nBackColor,MZK_32,7,0);
				}
				else
				{
					CDC_OutTextLR_UTF(630,405,buf,g_nRunTextColor,g_nBackColor,MZK_32,3,0);
					//CDC_OutText_32(680,409,buf,g_nRunTextColor, g_nBackColor);
				}
			}

			OSTimeDly(70);
		}

		else if(g_bTestFrm)
		{

			char   buf[0x10];
			INT32S p = 0;

			p = GetPulse(3,TRUE)*g_Ini.iNumerator[2];
			memset(buf,NULL,sizeof(buf));
			sprintf(buf,"%-8d",p);
			CDC_OutText24Ex(530,253,buf,YELLOWCOLOR,g_nBk);	


			p = GetPulse(2,TRUE)*g_Ini.iNumerator[1];
			memset(buf,NULL,sizeof(buf));
			sprintf(buf,"%-8d",p);				
			CDC_OutText24Ex(530,193,buf,YELLOWCOLOR,g_nBk);

			p = GetPulse(1,TRUE)*g_Ini.iNumerator[0];
			memset(buf,NULL,sizeof(buf));											
			sprintf(buf,"%-8d",p);				
			CDC_OutText24Ex(530,133,buf,YELLOWCOLOR,g_nBk);

			if(g_iTestAxis==0)
			{
				p = GetPulse(8,FALSE);	
				memset(buf,NULL,sizeof(buf));										//手轮脉冲值
				sprintf(buf,"%-8d",p);				
				CDC_OutText24Ex(660,75,buf,YELLOWCOLOR,g_nBk);
			}
			else
			{
				if(g_Ini.iLag[g_iTestAxis-1]!=0)
				{
					p = labs(GetPulse(g_iTestAxis,TRUE)*g_Ini.iNumerator[g_iTestAxis-1]-GetPulse(g_iTestAxis,FALSE)*g_Ini.iDenominator[g_iTestAxis-1]);
				}
				else
					p = 0;
				memset(buf,NULL,sizeof(buf));	
				sprintf(buf,"%-8d",p);				
				CDC_OutText24Ex(680,447,buf,YELLOWCOLOR,g_nBk);
			}

			p = GetPulse(1,FALSE)*g_Ini.iDenominator[0];						//X轴编码器值
			memset(buf,NULL,sizeof(buf));
			sprintf(buf,"%-8d",p);				
			CDC_OutText24Ex(660,133,buf,YELLOWCOLOR,g_nBk);

			p = GetPulse(2,FALSE)*g_Ini.iDenominator[1];						//Y轴编码器值
			memset(buf,NULL,sizeof(buf));
			sprintf(buf,"%-8d",p);				
			CDC_OutText24Ex(660,193,buf,YELLOWCOLOR,g_nBk);

			p = GetPulse(3,FALSE)*g_Ini.iDenominator[2];						//Z轴编码器值
			memset(buf,NULL,sizeof(buf));
			sprintf(buf,"%-8d",p);				
			CDC_OutText24Ex(660,253,buf,YELLOWCOLOR,g_nBk);

			if(g_iTestAxis && !g_bBackZero)
			{
				Count++;

				if(Count>g_Ini.iHandRunSpeed)									//指示手摇加工滞后之时间
				{
					Count=0;				
					pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					if(pos>0)
						curdir=TRUE;
					else
						curdir=FALSE;

					if(abs(pos)<3)								//add by xiaomeidi,20080418避免干扰采用滤波
						IsRun=TRUE;

					if(abs(pos)>2)								//if(pos!=0)modify by xiaomeidi,20080418避免干扰采用滤波
					{
						IsRun=TRUE;

						if(predir!=curdir)
						{
							predir=curdir;

							OS_ENTER_CRITICAL();							
							Stop1(g_iTestAxis);							
							OS_EXIT_CRITICAL();
						}
					}
					else
					{					
						if( IsRun)
						{
							OS_ENTER_CRITICAL();
							Stop1(g_iTestAxis);
							OS_EXIT_CRITICAL();

							IsRun=FALSE;
						}
					}
				}
			}

			OSTimeDly(10);		
		}

		else
			OSTimeDly(70);

		/*{
			static int aa =0 ;
			aa++;
			OSTimeDly(10);
			if(aa>10)
			{
				WriteBit(2,1);
				OSTimeDly(100);
				WriteBit(2,0);
			}
		}*/

		if((g_uiSuspendTask&0x0020)!=0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}


//#pragma	warn -par
void	RunGotoLine( INT16S n ,BOOL update)										//指定标号
{
	if(g_bInterface)
	{
		char buf[6];
		sprintf(buf,"%-3d",n+1);
		CDC_OutText(96,180,buf,g_TextColor2,g_nBk);	

		return;
	}
	else
	{
		if(g_bDispRunLine)
		{
			INT16S row;
			static INT16S d = 0; 												//省掉堆栈带来的成本

			d = n - g_iStaRunLine;

			if( !update && !d )
			{
				d = n - CList_StartRow() - CList_CurRow();

				if( d )
					CTeach_Update(d,0);

				return;
			}

			row = g_iStaRunLine - CList_StartRow();

			if(row < 0)
				row = 0;

			if(row > ROWS-1)
				row = ROWS-1;

			RunDisplayLine(g_iStaRunLine, row ,Reset,WHITECOLOR,g_nTableBk);


			d = n - CList_StartRow() - ROWS + 1;

			if( d )
			{
				CTeach_UpdatePage( d );
			}

			d = n - CList_StartRow() - CList_CurRow();

			if( d )
				CTeach_Update(d,0);

			g_iStaRunLine = n;

			row = g_iStaRunLine -CList_StartRow();

			if(row<0)
				row=0;

			if(row>ROWS-1)
				row=ROWS-1;

			RunDisplayLine(g_iStaRunLine, row ,MoveActive,WHITECOLOR,g_nTableBk);
		}
	}
}


//#pragma	warn -par
void	RunDisplayLine( INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor)
{
	CTeach_DisplayLine(data,row,status,qColor,bColor);	
}


void	DataUpack_Task(void *data)
{
	INT8U err;

#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  							/* Prevent compiler warning                           */
#endif

	data = data;

	for(;;)
	{
		OSSemPend(UnpackDataSem,0,&err);

		if(err==OS_NO_ERR)
		{
			INT16S tmp;

			g_bUnpackRuning=TRUE;

			if((tmp=UnpackData()) != 0)
			{
				UnpackErrDisp(tmp);
			}

			g_bUnpackRuning=FALSE;
		}		

	}
}


void    Assistant_Task(void *data)
{
	INT16S step = 0;

#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                 	 						/* Prevent compiler warning                           */
#endif

	data=data;

	for(;;)
	{
		OSTimeDly(10);		
	}
}

BOOL	WireWarning(void)
{
	if(g_bRelease)
	{
		if( RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2))
			return TRUE;	

		if( RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2))
			return TRUE;

		if(g_Ini.iPaoXian)
		{
			if( RealIO(PAOXIANDI,IOPRESS,2))
				return TRUE;
		}
	}

	return FALSE;	 	
}

