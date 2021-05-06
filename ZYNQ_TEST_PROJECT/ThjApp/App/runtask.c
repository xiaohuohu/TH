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
extern 		COMMAND		Line_Data[MAXLINE+1];
extern 		INT16S		Line_Data_Size;
extern 		INT16U		g_uBmpXMS;
extern		BOOL		g_bInterface;
TEST_EXT	BOOL		g_bRelease;
TEST_EXT	BOOL		g_bPM;
//TEST_EXT	BOOL		g_bBmp;
TEST_EXT	OS_EVENT	*RunSpeedCountSem;
TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT  	OS_EVENT	*UnpackDataSem;
TEST_EXT    OS_EVENT 	*RunLineQueue;
//TEST_EXT	OS_EVENT	*DispRunTimeSem;
TEST_EXT    OS_EVENT 	*MsgQueue;
TEST_EXT  	OS_EVENT 	*LedOutQueue;
TEST_EXT	BOOL		g_bEnglish;

WORK_EXT 	INT8S		g_pszMsgBuf[0x30];
WORK_EXT	INT8S		g_pszLedOut[8][3];
WORK_EXT	INT16S 		g_iRunLine;
WORK_EXT 	INT16S		g_iStaRunLine;
WORK_EXT 	BOOL 	 	g_bScreen;	   											//指示当前屏幕显示那个界面，TRUE显示主加工界面
WORK_EXT 	BOOL	  	g_bFirstDispTime;										//指示是否需要全部刷新日期时间显示，TRUE需要
//WORK_EXT	BOOL		g_bFirstDispRunTime;
WORK_EXT	BOOL		g_bDispRunTime;
WORK_EXT	BOOL		g_bCanDispRunTime;



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
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;
WORK_EXT 	INT16S		g_iWorkStatus;
WORK_EXT 	BOOL		g_bAutoRunMode;
WORK_EXT 	INT32S		g_lStepNode;	
WORK_EXT 	BOOL		g_bStepIO;  
WORK_EXT 	BOOL		g_bDispRunLine;
WORK_EXT 	BOOL		g_bIsHand;	   
WORK_EXT 	BOOL		g_bStepToSingle;
WORK_EXT 	BOOL		g_bStepToSingle2;
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

/*
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
 */

WORK_EXT	BOOL		g_bLCD;
WORK_EXT 	BOOL		g_bModify1;  
WORK_EXT	INT16S		g_iCheckStatusIO;
TEST_EXT	BOOL		g_bRam;
//20130815
WORK_EXT	BOOL		g_bTestToSingle;
WORK_EXT    BOOL 		g_bWheel;
//WORK_EXT    BOOL        g_bScreenSaved;


//WORK_EXT 	OUTDATA 	OutData[100];
//WORK_EXT 	INT32S  	g_lOutSize;
//WORK_EXT	BOOL		g_bOutInit;

WORK_EXT 	INT16S		g_iCurEditNo;
WORK_EXT	BOOL		g_bWheelStatus;
WORK_EXT	INT32S		g_TextColor11;

extern int 	g_bMode2;
INT32U      Suspended = 0;
extern     	INT32S 		g_iCheckXian;
extern 		BOOL        g_bCheckDiffer;
extern 		INT32S      g_iCheckTime;
extern 		BOOL        g_bCheckLay;
extern 		INT32S 		g_lHandPulse;
extern		BOOL		g_bTestFrm;
extern		INT16U		g_iTestAxis;
extern		INT32S		g_lRunLintCount;
extern      INT32S      g_bInitbmp;
WORK_EXT    BOOL      	g_bStepMode ;									//单行模式

extern		BOOL		g_bLagCheck;


BOOL		g_bRefresh = FALSE;
WORK_EXT	BOOL		g_bHandMove;
extern      BOOL 		g_bBackZeroProcess;
WORK_EXT    BOOL		g_bEditEnable;
WORK_EXT    BOOL		g_bNewProFile;
WORK_EXT    BOOL		g_bCylinder;
WORK_EXT    BOOL		g_bInsertNum;
WORK_EXT    BOOL		g_bEditData;

INT16S      g_ADTemp=0;

INT16S   AssisKey_Funtion(INT16S ADTemp);

void	Check_Probe_Task(void *data)
{
	INT16S  	Count=0,number=0;     
	BOOL    	predir=TRUE;
	BOOL    	curdir=TRUE;
	INT32S 		pos=0,pos1=0;
	FP32 		mv=0;
	INT32S      speed=0;


#if OS_CRITICAL_METHOD == 3                     	 						/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                 							/* Prevent compiler warning                           */
#endif

	data = data;

	for (;;)
	{
		if(g_iWorkStatus==NORUN  && g_bHandMove)
		{
			if(g_iMoveAxis<7 && g_iMoveAxis!=0)  								//借用此任务处理手摇轮
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

					pos = GetPulse(7,FALSE);
					pos1 = GetPulse(8,FALSE);

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					if(pos>0 || pos1>0  )
						curdir = TRUE;						
					else
						curdir = FALSE;

					if(pos!=0 || pos1!=0)
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
								//	pos = pos*200;  											//放大倍数可以使得手摇的反应更加灵敏
								pos = pos*50;  												//由于FPGA的反馈位置是MXC312的4倍							

								CurSpeedBi = (FP32)(labs(pos))/(tmp*g_Ini.fHandRunRate); 	//20061013更改之方法

								if(CurSpeedBi > 1.0)
									CurSpeedBi = 1.0;

								if(fabs(g_fSpeedBi-CurSpeedBi) > 0.02)
								{
									g_fSpeedBi = CurSpeedBi;

									speed = (INT32S)((FP32)g_lSpeed*g_fSpeedBi);

									if(speed<2)
										speed =2;

									OS_ENTER_CRITICAL();
									set_startv(g_iMoveAxis,speed);
									set_speed(g_iMoveAxis,speed);
									set_acc(g_iMoveAxis,speed);
									OS_EXIT_CRITICAL();
								}
							}
							else if(!IsFree(g_iMoveAxis) && pos1!=0)
							{
								//	pos1 = pos1 * 200;  										//放大倍数可以使得手摇的反应更加灵敏
								pos1 = pos1 * 50;  											//放大倍数可以使得手摇的反应更加灵敏								

								CurSpeedBi = (FP32)(labs(pos1))/(tmp*g_Ini.fHandRunRate); 	//20061013更改之方法

								if(CurSpeedBi > 1.0)
									CurSpeedBi = 1.0;

								if(fabs(g_fSpeedBi-CurSpeedBi) > 0.02)
								{
									g_fSpeedBi = CurSpeedBi;

									speed = (INT32S)((FP32)g_lSpeed*g_fSpeedBi);

									if(speed<2)
										speed =2;

									OS_ENTER_CRITICAL();
									set_startv(g_iMoveAxis,speed);
									set_speed(g_iMoveAxis,speed);
									set_acc(g_iMoveAxis,speed);
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

					if(g_iMoveAxis==2 && g_Ini.iAxisSwitch[1]==1)				//送线轴
					{
						temp = GetMM(g_iMoveAxis,TRUE)*100;

						if(mv != temp)
						{
							mv = temp;

							if( !g_bScreen && (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] == 'M' )
							{
								CEdit_SetLong((INT32S)mv);
								CEdit_UpdateData(FALSE);
							}
						}
					}
					else
					{
						FP32 	curPos =0;
						INT32S 	QuanPos = 0;
						FP32 	AnglePos=0;

						if(g_Ini.iAxisSwitch[g_iMoveAxis-1]==1)
						{
							switch(g_iMoveAxis)
							{
							case 1:
								temp=GetAngle(g_iMoveAxis,TRUE)*10;
								break;

							case 3:
								if(g_Ini.iAxisRunMode[2]==0)
									temp=GetAngle(g_iMoveAxis,TRUE)*10;
								else if(g_Ini.iAxisRunMode[2]==1)			//多圈
								{
									curPos=GetMM(g_iMoveAxis,TRUE);
									QuanPos = curPos/360;
									AnglePos = curPos - QuanPos*360;
									temp = QuanPos + AnglePos/1000.0;
								}
								else   if (g_Ini.iAxisRunMode[2]==2)
								{
									temp=GetMM(g_iMoveAxis,TRUE)*100;
								}
								break;

							case 4:
								if(g_Ini.iAxisRunMode[3]==0)
									temp=GetAngle(g_iMoveAxis,TRUE)*10;
								else if(g_Ini.iAxisRunMode[3]==1)			//多圈
								{
									curPos=GetMM(g_iMoveAxis,TRUE);
									QuanPos = curPos/360;
									AnglePos = curPos - QuanPos*360;
									temp = QuanPos + AnglePos/1000.0;
								}
								else  if (g_Ini.iAxisRunMode[3]==2)
								{
									temp=GetMM(g_iMoveAxis,TRUE)*100;
								}
								break;

							case 5:
								if(g_Ini.iAxisRunMode[4]==0)
									temp=GetAngle(g_iMoveAxis,TRUE)*10;
								else if(g_Ini.iAxisRunMode[4]==1)			//多圈
								{
									curPos=GetMM(g_iMoveAxis,TRUE);
									QuanPos = curPos/360;
									AnglePos = curPos - QuanPos*360;
									temp = QuanPos + AnglePos/1000.0;
								}
								else  if (g_Ini.iAxisRunMode[4]==2)
								{
									temp=GetMM(g_iMoveAxis,TRUE)*100;
								}
								break;

							case 6:
								if(g_Ini.iAxisRunMode[5]==0)
									temp=GetAngle(g_iMoveAxis,TRUE)*10;
								else if(g_Ini.iAxisRunMode[5]==1)			//多圈
								{
									curPos=GetMM(g_iMoveAxis,TRUE);
									QuanPos = curPos/360;
									AnglePos = curPos - QuanPos*360;
									temp = QuanPos + AnglePos/1000.0;
								}
								else  if (g_Ini.iAxisRunMode[5]==2)
								{
									temp=GetMM(g_iMoveAxis,TRUE)*100;
								}
								break;

							default:
								break;
							}


							if(fabs(temp-mv)>0.0001)
							{
								mv = temp;

								if( !g_bScreen && (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] == 'M' )
								{
									if((g_iMoveAxis == 3 &&g_Ini.iAxisRunMode[2]==1)|| (g_iMoveAxis == 4 &&g_Ini.iAxisRunMode[3]==1)|| (g_iMoveAxis == 5 &&g_Ini.iAxisRunMode[4]==1)|| (g_iMoveAxis == 6 &&g_Ini.iAxisRunMode[5]==1))
									{
										CEdit_SetFloat(mv,4);
									}
									else
										CEdit_SetLong((INT32S)mv);

									CEdit_UpdateData(FALSE);
								}
							}

						}
					}
				}
			}

			OSTimeDly(12);
		}
		else
		{
			OSTimeDly(12);
		}

		if((g_uiSuspendTask&0x0001) != 0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}



void    Run_CheckIO_Task(void *data)
{

	INT16U key=0,tkey=0;
	INT16S Count=0;
	//	static  BOOL    btimebegin=TRUE;
	//	static  BOOL    bdisptime=TRUE;

#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                 	 						/* Prevent compiler warning                           */
#endif

	data = data;

	for(;;)
	{

		if(g_bModify && !g_bUnpackRuning && !g_bBackZero && !g_bBackZeroProcess)
		{
			OSSemPost(UnpackDataSem);
		}

		key = KeyMBoxGetKey();


		if(key != 0xffff)
		{

			if(FALSE == ExitScrProtect())
			{
				OS_ENTER_CRITICAL();
				g_fCloseDispCount=0.0;	
				OS_EXIT_CRITICAL();

				if(g_Ini.iDispLeavingTime==1 && g_Ini.lDestCount>0)
				{
					if(g_bDispRunTime)
					{
						g_bCanDispRunTime = FALSE;
						OSTimeDly(40);

						if(key != TSTOP)  				//除了停机键的其他按键，只让"剩余时间"框退出显示，不执行对应功能(不然可能会出错)20141201
							key = 0xffff;

						g_bDispRunTime=FALSE;

						//CTeach_GotoFristline();
						CTeach_InvidateGrid();
					}
				}

				switch( key )
				{

				case F1:
					if(g_Ini.iTestFuction && g_bMode && !g_bHandRun&&(g_iWorkStatus==RUNNING||g_iWorkStatus==SINGLERUN) && g_iErrorNo ==0)
					{

						if(g_ADTemp<100)
						{
							g_ADTemp+=5;
						}
						else
						{
							g_ADTemp=100;
						}
					}
					break;
				case F2:
					if(g_Ini.iTestFuction && g_bMode&& !g_bHandRun&&(g_iWorkStatus==RUNNING||g_iWorkStatus==SINGLERUN) && g_iErrorNo ==0)
					{
						if(g_ADTemp>0)
						{
							g_ADTemp-=5;
						}
						else
						{
							g_ADTemp=0;
						}
					}
					break;

				case TSTOP:
				{
					if(g_bBackZero)
					{
						switch(g_Ini.iAxisCount)
						{
						case 6:Stop1(6);
						case 5:Stop1(5);
						case 4:Stop1(4);
						case 3:Stop1(3);
						case 2:Stop1(2);
						default:Stop1(1);
						}
						break;
					}

					if(g_bMode)
					{
						g_bTestToSingle=TRUE;
						OSQPost(MsgQueue,g_bEnglish?"Test last spring...":"测试最后一条...");

					}
					else
						OSQPost(MsgQueue,g_bEnglish?"The last spring...":"加工最后一条...");

					ps_debugout("check_io_task\r\n");
					g_bAutoRunMode = FALSE;
					LedOut(STOPLED,1);

				}
				break;

				case TTEST:
				{
					if(RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2) || (g_bRelease && RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2)) || (g_Ini.iPaoXian && RealIO(PAOXIANDI,0,2)))
						break;

					if(g_Ini.iSafeDoor>0)
					{
						if(RealIO(I_SAFEDOOR,g_Ini.iSafeDoor==1?0:1,2))//g_Ini.iSafeDoor=1为常开，2为常闭
						{
							break;
						}
					}

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					g_iErrorNo=0;

					if(!g_bMode)
					{
						g_bTestToSingle=FALSE;
						g_iADValue=-2;
						//	g_bHandRun = FALSE;
						g_bTestStart = TRUE;
						g_bDispRunLine=TRUE;

						//	g_bStepIO = TRUE;
						g_bMode=TRUE;
						LedOut(TESTLED,1);

						WriteBit(O_SENDADD,0);
					}

					OSQPost(MsgQueue,g_bEnglish?"Start Test Run...":"进入测试模式...");
					CTeach_UpdateStatus(STATUS_TEST);

					CTeach_Update(0,0);
				}
				break;

				case THANDSWITCH:									//各轴手轮
				case THANDBOX:										//各轴手盒
				{

					if(RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2) || (g_bRelease && RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2)) || (g_Ini.iPaoXian && RealIO(PAOXIANDI,0,2)))
						break;

					if(g_Ini.iSafeDoor>0)
					{
						if(RealIO(I_SAFEDOOR,g_Ini.iSafeDoor==1?0:1,2))//g_Ini.iSafeDoor=1为常开，2为常闭
						{
							break;
						}
					}

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					g_iErrorNo=0;

					if(!g_bHandRun  && !g_bStepIO )
					{
						OS_ENTER_CRITICAL();

						//	g_bHandRun = TRUE; 
						g_bTestStart = TRUE;
						//	g_bStepIO = TRUE;
						g_bDispRunLine=TRUE;
						g_iADValue=-2;

						OS_EXIT_CRITICAL();

						WriteBit(O_SENDADD,0);

						if(key == THANDSWITCH)
						{
							g_Ini.iHandStatus=g_Ini.iHandStatus==1?0:1;
							LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);

							if(g_bHandset)
							{
								g_bHandset = FALSE;
								LedOut(STEPLED,g_bHandset?1:0);
							}

							if(g_Ini.iHandStatus)
								CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOn ":"手轮开",g_TextColor11,g_nBk,KZK_22);
							else
								CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
						}
						else
						{

							g_bHandset=g_bHandset==1?0:1;
							LedOut(STEPLED,g_bHandset==1?1:0);

							if(g_Ini.iHandStatus)
							{
								g_Ini.iHandStatus = FALSE;
								LedOut(HANDLED,g_Ini.iHandStatus == 1?1:0);
							}

							if(g_bHandset)
								CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOn ":"手盒开",g_TextColor11,g_nBk,KZK_22);
							else
								CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手盒关",g_TextColor5,g_nBk,KZK_22);

						}
					}
				}
				break;

				case TSENDCLR:
				{
					WriteBit(CLEARWARNDO,1);						//线架报警清除开关输出
					OSTimeDly(500);
					WriteBit(CLEARWARNDO,0);
					break;
				}


				case TUP:
				case TDOWN:
				case TLEFT:
				case TRIGHT:
					//	if(!g_bMode)
					//		break;
					//case TCOUNTSET:
					//case TJCOMMAND:
					//case TSCOMMAND:
				{
					CTeach_OnKey(key,tkey);
				}
				break;

				case '0': case '1': case '2': case '3': case '4': case '5':
				case '6': case '7': case '8': case '9':
				case '-':case TDOT:
				case TINSERT:
				case TDEL:
				case TENTER:
				case TBACKSPACE:							//增加清除命令 20131031
					if(g_bBackZero)
						break;
					else if( g_Ini.iPasswordTime !=0 && g_bEditEnable==FALSE )
					{
						OSQPost(MsgQueue,"编辑超时,请退出加工,输入编辑密码!");
						break;
					}
					else
					{
						if(g_iCurEditNo==9)
						{
							CTeach_OnKey(key,tkey);
						}
					}
					break;

				default:break;

				}
			}
		}
		if(RealIO(EXT_SINGLEDI,0,3))
		{
			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 6:Stop1(6);
				case 5:Stop1(5);
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
				break;
			}

			if(g_bMode)
			{

				g_bTestToSingle=TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Test last spring...":"测试最后一条...");

			}
			else
				OSQPost(MsgQueue,g_bEnglish?"The last spring...":"加工最后一条...");

			g_bAutoRunMode = FALSE;
			LedOut(STOPLED,1);
		}
		/*	else
		{	

            if(!btimebegin)
            {
               	CTimer_Begin_1();
                btimebegin=TRUE;
            }                            
    		else
    		{
	    		if(!bdisptime)
	    		{
	        		if(CTimer_IsTimeOut_1())
	        		{    
	        		    if(g_bAutoRunMode&&!g_bMode)
	        			{		
	        				if(g_Ini.iDispLeavingTime==1&&g_Ini.lDestCount>0 &&!g_bScreenSaved)
	        				{
	        					CDC_SaveScreen();
	        					g_bFirstDispRunTime=TRUE;
	        					g_bDispRunTime=TRUE;
	        				}
	        			}

	        			bdisptime=TRUE;
	    			}
				}
			}
		}*/

		//********************手持盒***********************//
		if (g_bHandset?RealIO(SINGLEIO,0,3):0)
		{
			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 6:Stop1(6);
				case 5:Stop1(5);
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);break;
				}
			}

			if(g_bMode)
			{
				g_bTestToSingle=TRUE;						
				OSQPost(MsgQueue,g_bEnglish?"Test last spring...":"测试最后一条...");
			}
			else
				OSQPost(MsgQueue,g_bEnglish?"The last spring...":"加工最后一条...");

			g_bAutoRunMode = FALSE;
		}


		//*******************手轮加工***********************//
		//	if(g_bWheel)
		if(g_bMode && !g_bStepIO &&( g_Ini.iHandStatus ==1 || g_bHandset))
		{

			if( !g_bBackZero && g_iErrorNo ==0)
			{
				Count++;

				if(Count > g_Ini.iHandRunSpeed*3)
				{
					INT32U t2=0;
					INT32S tmp=0;
					static INT32S pos=0;
					//static INT32S pos1=0;

					Count= 0;

					t2   = OSTimeGet();
					tmp  = t2-g_lHandRun;

					if(tmp <= 0)
						tmp = 1;

					g_lHandRun = t2;

					if(g_bHandset)
						pos = GetPulse(7,FALSE);
					else 
						pos = GetPulse(8,FALSE);
					//pos = (INT32S)pos+pos1;								

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
					//Uart_Printf("test4=%d %d\n",pos1,pos);

					if(pos>0 )										
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
					else if(GetRange()!=0)
					{
						g_fSpeedBi = 0.0;
						SetRange(0);

						if(g_bHandRun == TRUE)
						{
							g_bHandRun = FALSE;
							OSQPost(MsgQueue,g_bEnglish?"HandRateRun Stop":"手摇加工停止!");
						}	

						g_iRunLine  = (((vm_getindex()%500)+1)/2);//(((vm_getindex()%500)+1)/2)-1;
						OSQFlush(RunLineQueue);								// Clear queue
						OSQPost(RunLineQueue,(void *)&g_iRunLine);
					}
				}																//end if(Count>g_Ini.iHandRunSpeed)
			}

		}

		//if(!g_bPressStartIO && RealIO(RUNDI,0,2))											//手动运行时按手执盒自动加工键轮廓
		if(!g_bPressStartIO&&(RealIO(STARTIO,0,2)||RealIO(EXT_STARTIO,0,2)||(RealIO(RUNIO,0,2)&&g_bHandset)))		//按键面板上的启动按键，和手持盒上的启动键
		{
			g_bPressStartIO=TRUE;


			if(RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2) || (g_bRelease && RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2)) || (g_Ini.iPaoXian && RealIO(PAOXIANDI,0,2)))
				continue;

			if(g_Ini.iSafeDoor>0)
			{
				if(RealIO(I_SAFEDOOR,g_Ini.iSafeDoor==1?0:1,2))//g_Ini.iSafeDoor=1为常开，2为常闭
				{
					continue;
				}
			}

			g_iErrorNo=0;	

			if(g_bMode)
			{

				//CTeach_GotoFristline();

				OS_ENTER_CRITICAL();

				g_bStepIO=FALSE;
				g_bDispRunLine=FALSE;
				//	g_bAutoRunMode=TRUE;
				g_bMode=FALSE;
				g_bHandRun = FALSE;
				//	g_bWheel = FALSE;
				//	LedOut(WHEELLED,0);

				OS_EXIT_CRITICAL();

				LedOut(TESTLED,g_bMode?1:0);

				g_fCloseDispCount=0.0;	
			}

			if(g_bAutoRunMode ==TRUE)
				g_bCanDispRunTime=TRUE;



			CTeach_UpdateStatus(STATUS_AUTO);
			OSQPost(MsgQueue,g_bEnglish?"Start Auto Run...":"开始自动加工...");

			SetRange(8000);

			if(g_Ini.iAxisSwitch[1]==1)
				WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);	

		}

		if(g_bPressStartIO&& (RealIO(STARTIO,1,3)||RealIO(EXT_STARTIO,0,2)) && (g_bHandset?RealIO(RUNIO,1,2):1))
		{
			g_bPressStartIO=FALSE;
		}


		if(RealIO(STOPIO,0,2)||(g_bHandset?RealIO(STOPIO2,0,3):0))			//停止
		{
			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 6:Stop1(6);
				case 5:Stop1(5);
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
			}
			else
			{
				/*if(g_iErrorNo==3 &&  RealIO(SONGXIANIOCONST,1,2))
				{
					OSQPost(MsgQueue,"停止-复位!");
					OSTimeDly(1000);
					OSQPost(MsgQueue," ");
				}
				else*/
				{			
					g_ADTemp=0;
					g_iErrorNo=1;
					g_bStepIO=FALSE;
					g_bDispRunLine=FALSE;
					//	g_bAutoRunMode=FALSE;


					g_bMode=FALSE;
					LedOut(TESTLED,0);	

					//					Uart_Printf("g_iErrorNo201=%d\n",g_iErrorNo);

					vm_stop(0);

					//					Uart_Printf("g_iErrorNo202=%d\n",g_iErrorNo);
				}
			}
		}

		if(g_bRelease)
		{
			if( RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2)  && g_iErrorNo!=2)
			{
				g_iErrorNo=2;

				SetRange(0);

				WriteBit(O_SENDADD,0);
				ExitScrProtect();
				CTeach_UpdateStatus(STATUS_PAUSE);

				if(g_Ini.iWarningOut==1)
				{
					if(GetOut(O_WARNING)==0)
						WriteBit(O_WARNING,OPEN);
				}

				OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning!":"断线报警!");

			}
		}

		if( RealIO(SONGXIANIOCONST,g_Ini.iIOKinkConfig_Level,2)&& g_iErrorNo!=3)
		{
			g_iErrorNo=3;
			SetRange(0);


			WriteBit(O_SENDADD,0);
			ExitScrProtect();
			CTeach_UpdateStatus(STATUS_PAUSE);

			if(g_Ini.iWarningOut==1)
			{
				if(GetOut(O_WARNING)==0)
					WriteBit(O_WARNING,OPEN);
			}

			OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning!":"缠线报警");
		}	

		if(g_Ini.iPaoXian)
		{
			if( RealIO(PAOXIANDI,IOPRESS,2) && g_iErrorNo!=4 )
			{
				g_iErrorNo=4;
				SetRange(0);

				WriteBit(O_SENDADD,0);
				ExitScrProtect();
				CTeach_UpdateStatus(STATUS_PAUSE);


				if(g_Ini.iWarningOut==1)
				{
					if(GetOut(O_WARNING)==0)
						WriteBit(O_WARNING,OPEN);
				}

				OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning!":"跑线报警");

			}

		}


		if(g_Ini.iSafeDoor>0)
		{
			if(RealIO(I_SAFEDOOR,g_Ini.iSafeDoor==1?0:1,2) && g_iErrorNo != 13)
			{
				g_iErrorNo=13;
				SetRange(0);

				WriteBit(O_SENDADD,0);
				ExitScrProtect();
				CTeach_UpdateStatus(STATUS_PAUSE);


				if(g_Ini.iWarningOut==1)
				{
					if(GetOut(O_WARNING)==0)
						WriteBit(O_WARNING,OPEN);
				}
				OSQPost(MsgQueue,g_bEnglish?"Safe Door Warning!":"安全门报警!加工暂停...");

			}

		}

		switch(g_Ini.iAxisCount)
		{
		case 6:
		{
			if(g_Ini.iAxisSwitch[5]==1)
			{
				if( RealIO(WSERVOWARNING,g_Ini.iServoLogic[5],2))			//伺服报警检测
					g_iErrorNo=12;

				if(PulseLagCheck(6))										//脉冲滞后检测
					g_iErrorNo = 49;

				if(g_Ini.iAxisRunMode[5]>=0 || g_Ini.iAxisRunMode[5]<=2)	//限位报警检测
				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(6,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(6,0);

					if(res!=0)
						g_iErrorNo=res;
				}

			}
		}

		case 5:
		{
			if(g_Ini.iAxisSwitch[4]==1)
			{
				if( RealIO(VSERVOWARNING,g_Ini.iServoLogic[4],2))
					g_iErrorNo=11;

				if(PulseLagCheck(5))
					g_iErrorNo = 48;

				if(g_Ini.iAxisRunMode[4]>=0 || g_Ini.iAxisRunMode[4]<=2)
				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(5,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(5,0);

					if(res!=0)
						g_iErrorNo=res;
				}
			}
		}
		case 4:
		{
			if(g_Ini.iAxisSwitch[3]==1)
			{
				if( RealIO(USERVOWARNING,g_Ini.iServoLogic[3],2))			//伺服报警检测
					g_iErrorNo=10;

				if(PulseLagCheck(4))										//脉冲滞后检测
					g_iErrorNo = 47;

				if(g_Ini.iAxisRunMode[3]>=0 || g_Ini.iAxisRunMode[3]<=2)	//限位报警检测
				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(4,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(4,0);

					if(res!=0)
						g_iErrorNo=res;
				}

			}
		}

		case 3:
		{
			if(g_Ini.iAxisSwitch[2]==1)
			{
				if( RealIO(ZSERVOWARNING,g_Ini.iServoLogic[2],2))
					g_iErrorNo=9;

				if(PulseLagCheck(3))
					g_iErrorNo = 46;

				if(g_Ini.iAxisRunMode[2]>=0 || g_Ini.iAxisRunMode[2]<=2)
				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(3,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(3,0);

					if(res!=0)
						g_iErrorNo=res;
				}
			}
		}

		case 2:
		{
			if(g_Ini.iAxisSwitch[1]==1)
			{
				if( RealIO(YSERVOWARNING,g_Ini.iServoLogic[1],2))
					g_iErrorNo=8;

				if(g_bLagCheck)
				{
					if(PulseLagCheck(2))
						g_iErrorNo = 45;
				}

				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(2,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(2,0);

					if(res!=0)
						g_iErrorNo=res;
				}
			}
		}

		case 1:
		{
			if(g_Ini.iAxisSwitch[0]==1)
			{
				if( RealIO(XSERVOWARNING,g_Ini.iServoLogic[0],2))
					g_iErrorNo=7;

				if(PulseLagCheck(1))
					g_iErrorNo = 44;

				if(g_Ini.iAxisRunMode[0]>=0 || g_Ini.iAxisRunMode[0]<=2)
				{
					INT16S res=0;

					if(!g_bBackZero)
					{
						res=CheckSoftLimit(1,0);

						if(res!=0)
							g_iErrorNo=res;
					}

					res=CheckHardLimit(1,0);

					if(res!=0)
						g_iErrorNo=res;
				}
			}
		}

		default:break;
		}

		//if(g_iErrorNo==3 || g_iErrorNo==2 ||  g_iErrorNo==4 ||  g_iErrorNo==14 ||  g_iErrorNo==15)
		{
			//ExitScrProtect();
		}

		if(vm_get_errNo())
		{
			g_iErrorNo=50;
		}

		if(g_iErrorNo>1 && g_iErrorNo<=50 && (g_iErrorNo!=3 && g_iErrorNo!=2 &&  g_iErrorNo!=4&&  g_iErrorNo!=14&&  g_iErrorNo!=15) )
		{
			OS_ENTER_CRITICAL();

			if(g_bBackZero)
			{
				switch(g_Ini.iAxisCount)
				{
				case 6:Stop1(6);
				case 5:Stop1(5);
				case 4:Stop1(4);
				case 3:Stop1(3);
				case 2:Stop1(2);
				default:Stop1(1);
				}
			}


			vm_stop(0);

			g_iWorkStatus = NORUN;

			//Uart_Printf("g_iErrorNo203=%d\n",g_iErrorNo);

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
				if(g_lRunTime/OS_TICKS_PER_SEC > 20)
					g_fRunSpeed=(60.0/((FP32)g_lRunTime/OS_TICKS_PER_SEC));	
				else
					g_fRunSpeed=(60.0/((FP32)g_lRunTime/OS_TICKS_PER_SEC))*0.7+g_fRunSpeed*0.3;
			}
			else
				g_fRunSpeed=100.0;

			if(g_fRunSpeed>999.0)
				g_fRunSpeed=999.0;

			if(!g_bScrProtectRun)
			{

				UpdateSpeedCountProbe();
				//UpdateLeavingTime(FALSE,FALSE,TRUE);

				if(g_bCanDispRunTime==TRUE && g_bAutoRunMode && !g_bMode && g_Ini.iDispLeavingTime==1 && g_Ini.lDestCount>0 && g_iWorkStatus==RUNNING)
				{		

					//OSSemPost(DispRunTimeSem);
					if(g_bDispRunTime==FALSE)
					{
						g_bDispRunTime=TRUE;

						UpdateLeavingTime(TRUE,TRUE,FALSE);			//显示对话框，更新时间
					}
					else
					{
						UpdateLeavingTime(FALSE,TRUE,FALSE);		//更新时间
					}
				}
			}


			if(g_bAutoRunMode && !g_bMode && g_iWorkStatus==RUNNING)			//防止自动转向单条屏保时，只能先按启动键再按其他键才能退出屏保的问题
			{
				CloseDisplay(((FP32)g_lRunTime)/OS_TICKS_PER_SEC);
			}

			if(g_bRam)
				WriteRunCount();
		}
		else
		{

			switch(err)
			{
			case OS_ERR_TIMEOUT:
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

/*
void    DispRunTime_Task(void *data)
{
	INT8U err;

	#if OS_CRITICAL_METHOD == 3                      							// Allocate storage for CPU status register           
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  							// Prevent compiler warning                           
	#endif  

	data=data;

	for(;;)
	{
		OSSemPend(DispRunTimeSem,0,&err);


		if(err==OS_NO_ERR)
		{

			if(!g_bMode && g_bAutoRunMode)
			{
				if(g_bDispRunTime==FALSE)
				{
					g_bDispRunTime=TRUE;

					UpdateLeavingTime(TRUE,TRUE,FALSE);			//显示对话框，更新时间
				}
				else
				{
					UpdateLeavingTime(FALSE,TRUE,FALSE);		//更新时间
				}
			}

		}
	}
}
 */

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
			if(!g_bScrProtectRun)
			{
				RunGotoLine(runline,FALSE);
			}
		}
		else
		{
			switch(err)
			{
			case OS_ERR_TIMEOUT:
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


void    Display_XYZ_Task(void *data)
{
#if OS_CRITICAL_METHOD == 3                      						/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  						/* Prevent compiler warning                           */
#endif

	data=data;	

	for(;;)
	{	

		static	INT32S 	tm = 0;
		static	BOOL	cnt = FALSE;	//计数标识

		if (g_bEditEnable && !cnt)
		{
			tm=OSTimeGet();
			cnt = TRUE;
		}

		if (g_bEditEnable && cnt)
		{
			if ((OSTimeGet()-tm) > 60*1000*g_Ini.iPasswordTime)
			{
				tm = 0;
				cnt = FALSE;
				g_bEditEnable = FALSE;
			}
		}


		if(!g_bScrProtectRun && !g_bTestFrm && g_bInterface)
		{
			DisplayXYZ(TRUE);		
			UpdateProbe(FALSE);
			UpdateCylinder(FALSE);

		}

		OSTimeDly(70);

		if((g_uiSuspendTask&0x0020)!=0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}


/*********************************************
函数名:       	RunGotoLine
主要功能:    	显示行跳到指定的N行
输入参数:       n 	------指定的行号
				update -------跳行方式，update == TRUE 为走表式，update == FALSE为走行式。
输出参数:        无
返回值:         	无
 *********************************************/
BOOL  g_bRunGotoLine = FALSE;

//#pragma	warn -par
void	RunGotoLine( INT16S n ,BOOL update)										//指定标号
{
	if(g_bScreen)
	{
		return;
	}
	else
	{
		INT16S i=0;

		if(g_iCurEditNo!=9)											//当前的编辑点不在列表时  为9为在列表编辑状态
		{ 
			UpdateFileDestProbe(g_iCurEditNo+1,1,0);
			g_iCurEditNo = 9;
		}


		if(n==(CList_StartRow()+CList_CurRow()) || n==0)
			return;

		if(update==FALSE &&  n>=CList_StartRow() && n < CList_StartRow()+ROWS)
		{
			g_bRunGotoLine = TRUE;
			CTeach_Update(n-CList_StartRow()-CList_CurRow(),0);
			g_bRunGotoLine = FALSE;
		}
		else
		{
			CTeach_DrawGrid();
			CTeach_InitData();

			if(n>MAXLINE-ROWS+1)						//最大行保护
				n = MAXLINE-ROWS+1;	

			if(n>=1)
			{
				CList_SetStartRow(n-1);
				CList_SetCurRow(1);
				CList_SetCurCol(2);
			}
			else if(n<1)
			{
				CList_SetStartRow(0);
				CList_SetCurRow(0);
				CList_SetCurCol(2);
			}


			for(i=0; i<ROWS; i++)
				CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);

			g_bRunGotoLine = TRUE;  
			CTeach_Update(0,0);
			g_bRunGotoLine = FALSE;
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
			g_bModify = FALSE;
		}		

	}
}

//串口接收任务 此处有延时  2020/9/10 cj  原本的OSTimeDly(5) 修改为 OSTimeDly(2)
void    Get_ComData_Task(void *data)
{//另外做一套按键
	INT16S temp=0;
	INT32S AD = 0;
	INT32S LasADTemp = 0;
	//INT16S ADTemp=0;
	INT8U buffer[0x10];
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  /* Prevent compiler warning                           */
#endif

	data=data;

	for(;;)
	{
		if(!g_Ini.iTestFuction)
		{
			temp=CTelCom_GetChar(1);

			if(g_bMode && !g_bHandRun&&(g_iWorkStatus==RUNNING||g_iWorkStatus==SINGLERUN) && g_iErrorNo ==0)
			{
				if(temp!=2000)
					temp=temp/2;

				if(temp!=2000 && abs(temp-g_iADValue)>1)
				{
					g_iADValue=temp;
					g_fCloseDispCount=0.0;

					//	Uart_Printf("ComData_Taskg_iADValue=%d\n",g_iADValue);

					if(temp<3)
					{
						SetRange(0);

						if(g_bStepIO == TRUE)
						{
							g_bStepIO = FALSE;

							OSQPost(MsgQueue,g_bEnglish?"TestRun Stop":"测试加工停止!");
							CTeach_UpdateStatus(STATUS_PAUSE);
							g_iRunLine = (((vm_getindex()%500)+1)/2);
							OSQFlush(RunLineQueue);								// Clear queue
							OSQPost(RunLineQueue,(void *)&g_iRunLine);
						}
					}
					else
					{
						if(temp<127)
						{
							AD = (INT32S)(8000*(FP32)temp/126.0);
							SetRange(AD);
							CTeach_UpdateStatus(STATUS_TEST);
							if(g_bStepIO == FALSE)
							{
								g_bStepIO =TRUE;
								OSQPost(MsgQueue,g_bEnglish?"Start Test Run...":"开始测试运行...");
							}
							//	if(g_Ini.iAxisSwitch[1]==1)WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);

						}
					}
				}
			}
		}
		else
		{
			if(g_bMode&& !g_bHandRun&&(g_iWorkStatus==RUNNING||g_iWorkStatus==SINGLERUN) && g_iErrorNo ==0)
			{
				//ADTemp = AssisKey_Funtion(ADTemp);
				//Uart_Printf(" ADTemp =%d \r\n",ADTemp);
				if(g_ADTemp==0)
				{
					SetRange(0);
					if(LasADTemp!=g_ADTemp)
					{
						g_bStepIO = FALSE;
						if(g_bEnglish)
							sprintf(buffer,"TestRun Stop test speed ");
						else
							sprintf(buffer,"测试加工停止!加工速度百分比  0 ");
						OSQPost(MsgQueue,buffer);
						CTeach_UpdateStatus(STATUS_PAUSE);
						g_iRunLine = (((vm_getindex()%500)+1)/2);
						OSQFlush(RunLineQueue);								// Clear queue
						OSQPost(RunLineQueue,(void *)&g_iRunLine);
					}
				}
				else
				{
					AD = (INT32S)(8000*(FP32)g_ADTemp/100.0);
					SetRange(AD);
					CTeach_UpdateStatus(STATUS_TEST);
					if(LasADTemp!=g_ADTemp)
					{
						g_bStepIO=TRUE;
						if(g_bEnglish)
							sprintf(buffer,"Start Test Run... test speed %d  ",g_ADTemp);
						else
							sprintf(buffer,"开始测试运行...加工速度百分比 %d  ",g_ADTemp);
						OSQPost(MsgQueue,buffer);
					}
					//	if(g_Ini.iAxisSwitch[1]==1)WriteBit(O_SENDADD,g_Ini.iXianjiaDrive?1:0);
				}
				LasADTemp=g_ADTemp;
			}
		}
		OSTimeDly(2);
	}
}

INT16S   AssisKey_Funtion(INT16S ADTemp)
{//特殊测试模式下响应按钮
	INT16S temp=0,tkey=0;
	INT16U key=0;
	INT16S  row=0;
	COMMAND *ptr;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                  /* Prevent compiler warning                           */
#endif
	key = KeyMBoxGetKey();//把测试单条加上

	if(key != 0xffff)
	{
//		Uart_Printf("key =%d ",key);
		switch(key)
		{
		case TSTOP:
		{
			CTeach_OnKey(TSTOP,0);
		}
		break;

		case TTEST:
		{
			if(ADTemp>0)
				break;
Uart_Printf(" ##################TTEST from AssisKey_Funtion################# \n");
			CTeach_OnKey(TTEST,0);
		}
		break;

		case THANDSWITCH:									//各轴手轮
		{
			CTeach_OnKey(THANDSWITCH,0);
			break;
		}
		case THANDBOX:										//各轴手盒
		{
			CTeach_OnKey(THANDBOX,0);
		}
		break;
		case TXPLUS:
		{
			if(ADTemp<100)
			{
				ADTemp+=5;
			}
			else
			{
				ADTemp=100;
			}
//			Uart_Printf("增加  key =%d ",key);
			break;
		}
		case TXMINUS:
		{
			if(ADTemp>0)
			{
				ADTemp-=5;
			}
			else
			{
				ADTemp=0;
			}
//			Uart_Printf("减少 key =%d ",key);
			break;
		}
		case TUP:												//向上键
		{
			CTeach_OnKey(TUP,0);
		}
		break;

		case TDOWN:													//向下键
		{
			CTeach_OnKey(TDOWN,0);
		}
		break;

		case TRIGHT:												//向右键
		{
			CTeach_OnKey(TRIGHT,0);
		}
		break;

		case TLEFT:													//向左键
		{
			CTeach_OnKey(TLEFT,0);
		}
		break;
		case TENTER:											//记忆键
		{
Uart_Printf(" ##################Enter from AssisKey_Funtion################# \n");
			CTeach_OnKey(TENTER,0);
		} break;

		case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8':					//数字键

			if(g_bCylinder)
			{
				INT16S temp=key-'1';

				WriteBit(temp,GetOut(temp)?0:1);

				if(GetOut(temp))
					sprintf(g_pszMsgBuf,g_bEnglish?"%ld Cylinder Out":"%1d号汽缸伸出",temp+1);
				else
					sprintf(g_pszMsgBuf,g_bEnglish?"%ld Cylinder In":"%1d号汽缸退回",temp+1);

				OSQPost(MsgQueue,g_pszMsgBuf);
				UpdateCylinder(FALSE);
				break;
			}													//转到下一个CASE

		case '0':												//0.-清楚键输入
		case TDOT:												//'.'   835B设定G定点送线
		case '9':case TMINUS:

			g_bInsertNum = TRUE;

			CEdit_OnKey( key, tkey );

			if(g_iCurEditNo == 9)
			{
				g_bEditData=TRUE;
			}
			break;

		case TBACKSPACE:										//清除命令按键
Uart_Printf(" ***************Clean from AssisKey_Funtion***************\n");
			CTeach_OnKey(TBACKSPACE,0);
			break;
		case TINSERT:											//插入
		{
Uart_Printf(" =========Insert from AssisKey_Funtion=========== \n");
			CTeach_OnKey(TINSERT,0);
		}
			break;

		case TDEL:												//删除单行
		{
Uart_Printf(" -----------Delete from AssisKey_Funtion------------ \n");
			CTeach_OnKey(TDEL,0);
		}
			break;

		default:
			break;
		}

	}
	return ADTemp;
}
/*void    Assistant_Task(void *data)
{
	//INT16S step = 0;

	#if OS_CRITICAL_METHOD == 3                      							// Allocate storage for CPU status register           
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                 	 						// Prevent compiler warning                           
	#endif  

	data=data;

	for(;;)
	{
		OSTimeDly(10);		
	}
}*/

