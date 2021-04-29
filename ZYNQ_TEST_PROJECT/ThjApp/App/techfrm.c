/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        techfrm.c
  创建人：       范志州
  创建日期：	 2004/11/20
  主要功能描述:  数据编辑界面

  修改记录：
   1.  修改者:	      范志州
	   修改日期:	  2005/08/04
	   主要修改内容:  界面编辑的各种BUG
   2. ...
*************************************************/
# include   "includes.h"



	
extern 		int 		usb_max_devs;																																									
extern 		NEWDOG		NewDog;
extern 		COMMAND		Line_Data[MAXLINE+1];
extern 		INT16S		Line_Data_Size;
extern 		UNITDATA	g_UnitData;
extern 		BASEINI		g_Ini;
extern 		INT16U		g_uBmpXMS;
WORK_EXT	FP32   		g_fRunSpeed;		

TEST_EXT	BOOL		g_bRelease;
TEST_EXT	OS_EVENT	*RunSpeedCountSem;
TEST_EXT  	OS_EVENT 	*KeyMBox;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT  	OS_EVENT 	*LedOutQueue;
TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT  	OS_EVENT	*UnpackDataSem;
TEST_EXT	INT32S		g_lBmpStartTime;
TEST_EXT	BOOL		g_bSysParaCorrect;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT    INT8S		g_Color;
TEST_EXT	BOOL		Ok_Exit;
//TEST_EXT	BOOL		g_bBmp;
WORK_EXT 	INT8S		g_pszMsgBuf[0x30];
WORK_EXT	INT8S		g_pszLedOut[8][3];
WORK_EXT	INT16S 		g_iRunLine;
WORK_EXT 	INT16S		g_iStaRunLine;
WORK_EXT    BOOL 		g_bScreen;
WORK_EXT 	BOOL	  	g_bFirstDispTime;
WORK_EXT	BOOL		g_bUnpackCorrect;
WORK_EXT	INT32S		g_lRunTime;
WORK_EXT 	BOOL   		g_bStartRun;  
WORK_EXT 	BOOL		g_bRuning;    
WORK_EXT 	BOOL		g_bModify;  
WORK_EXT	BOOL		g_bModifySave;
WORK_EXT	BOOL		g_bUnpackRuning;
WORK_EXT	FP32		g_lSpeed;
WORK_EXT	BOOL		g_bHandMove;
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
WORK_EXT 	BOOL		g_bMode,g_bMode2;	  
WORK_EXT	BOOL		g_bEMSTOP; 
WORK_EXT	INT16S		g_iADValue;
WORK_EXT	BOOL		g_bTestStart;
WORK_EXT	BOOL		g_bHandRun;  
WORK_EXT	INT32U		g_lHandRun;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	FP32 		g_lWidthPos;
WORK_EXT	BOOL		g_bSequeBack;
WORK_EXT	BOOL		g_bGotoStart[MAXCH];
WORK_EXT	BOOL		g_bPressStartIO;
WORK_EXT	BOOL		g_bPressStartIO2;
WORK_EXT	BOOL		g_bPressStepIO2;
WORK_EXT	BOOL		g_bBackZero;
WORK_EXT	INT16S		g_iErrorNo;
WORK_EXT	BOOL		g_bCylinder; 
WORK_EXT 	INT16S		g_iWorkNumber;
WORK_EXT 	INT16S		g_iSpeedBeilv;
WORK_EXT 	INT32S  	g_lXmsSize;
WORK_EXT 	INT32S		g_lYCountLen;       
WORK_EXT 	INT32S		g_lZCountLen;  
  
 /*   
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TextColor6;
WORK_EXT	INT32S 		g_TextColor7;
WORK_EXT	INT32S 		g_TextColor8;
WORK_EXT	INT32S		g_TextColor9;
WORK_EXT	INT32S		g_TextColor10;
WORK_EXT	INT32S		g_TextColor11;

WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S		g_nTechfrmBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;
*/


//WORK_EXT	INT32S		g_lAxisValue[CHMAX];
WORK_EXT 	INT32S  	g_lRamRunCounts;
WORK_EXT 	INT32S		g_lProbeFail; 											//记录探针失败   

//20130815
WORK_EXT    BOOL      	g_bStepMode;
WORK_EXT    BOOL 		g_bWheel;
WORK_EXT	BOOL		g_bFunc;
WORK_EXT	INT16S		g_iTCmdPos;
WORK_EXT	BOOL		g_bTestToSingle;
WORK_EXT	INT32S		g_lInputPos[CHMAX];
WORK_EXT	BOOL		g_bUseTimes;
WORK_EXT	INT16S		g_iStatus;
WORK_EXT    INT16S      g_iToolBarPage;
WORK_EXT	INT16U 		g_uiMoveKey;
WORK_EXT	INT16U		g_uiSuspendTask;
TEST_EXT    OS_EVENT 	*RunLineQueue;

BOOL		g_bEditTimes=FALSE;
BOOL		g_bEndCom = FALSE;
BOOL		g_bJump = FALSE;
BOOL   		g_gridStyle=FALSE;								//网格式列表框
INT16S		g_iStepEditNo=0;
INT16S		g_iStepTimesEditNo=0;
INT16S		g_iParameterEdit=0;
INT32S      g_lHandPulse  = 0;


extern 		INT32S      g_iLayNum;
WORK_EXT	FP32   		g_fRunSpeed;
extern		BOOL		g_bSysReadBackup;

//#define		LISTHEIGHT	35
//#define		LISTLEFT	4
//#define		LISTTOP		154

//20130822

#define		LISTLEFT	1
#define		LISTTOP		130
#define		LISTHEIGHT	28
//#define		ROWS		

//列表框和为
/*
INT16S		Widths2[COLS2]={ 56,56,125,125,138,80,78,138};
INT16S		Widths3[COLS3]={ 56,56,100,100,129,100,70,56,129};
INT16S		Widths4[COLS4]={ 56,56,84,84,104,90,90,54,54,124};
*/

INT16S  Widths2[COLS2]={ 52,55,110,110,121,80,76,121,71};
INT16S  Widths3[COLS3]={ 52,55,100,100,112,100,70,50,101,56};
INT16S  Widths4[COLS4]={ 52,47,77,77,96,96,96,50,48,101,56};



INT16S		Widths5[COLS5]={ 53,53,80,80,91,80,80,80,54,54,91};
INT16S		Widths6[COLS6]={ 48,25,78,78,90,78,78,78,78,50,25,90};


INT16S		g_iCurEditNo=1;
BOOL		g_bEditData=FALSE; 
BOOL		g_bTest = FALSE;

///*************************文件料号***************目标数量*******************探针设定**********//
//CRect		g_EditRect[5]={{112,40,112+42,40+20},{302,40,302+100,40+20},{502,40,502+66,40+20},{588,472,605+30,472+20},{6,504,6+112,504+21}};
CRect		g_EditRect[5]={{110,42,120+42,42+28},{305,42,310+86,42+28},{517,42,517+43,42+28},{118,474,118+24,474+29}};

//20130819 
CRect		g_StepEditRect[MAXCH]={{118,474,118+31,474+28},{240,474,240+31,474+28},{362,474,362+31,474+28},{484,474,484+31,474+28},{606,474,606+31,474+28},{728,474,728+31,474+28}};


CRect		g_StepEditRectEx[MAXCH]={{166,474,166+19,474+28},{288,474,288+19,474+28},{410,474,410+19,474+28},{532,474,532+19,474+28},{654,474,654+19,474+28},{774,474,774+19,474+28}};
				

BOOL		g_bDispServoWaring=TRUE;
CTROL		*m_ptrCtrl;	
CTROL		CtrlTmp;

INT16S		g_iCommandPos = 0;
INT8S		g_cCommand[7]={'M','S','E','L','N','J','G'};
BOOL		g_bInterface = TRUE;
BOOL		g_bCopy = FALSE;
BOOL		g_bInsertNum = FALSE;								//是否数字输入
BOOL		g_bNewProFile = FALSE;								//建立新的文件，探针g_iTCmdPos重新 设置为0
BOOL		g_bClearNum = FALSE;								//在编辑数据若数据缩短了，覆盖超出长度的数据（在按"-"时会出现这种情况）
BOOL        g_bBackZeroProcess	= FALSE;						//表示归零按键整个过程，与g_bBackZero有所区别


WORK_EXT	BOOL		g_bCanDispRunTime;
extern      BOOL		g_bRunGotoLine;
extern      INT8U   	g_ukeyscan;   
extern		INT8S		m_bString[MAXCHAR];
extern		BOOL		g_bTestKey;
extern		BOOL		g_bRefresh;
extern		INT32S 		g_bInitbmp;


BOOL 	g_bPressRunIO = TRUE;
BOOL	g_bPressStopIO = TRUE;
WORK_EXT    BOOL		g_bEditEnable = FALSE;



void	CTeach_Init()
{
	m_ptrCtrl 	= &CtrlTmp;
	g_bMode 	= FALSE;
	g_iRunLine 	= 0;
	Ok_Exit 	= FALSE;
	g_bRefresh 	= FALSE;
	
	CList_Init();
	
	CList_MoveTo( LISTLEFT,LISTTOP );							//列表框的位置
	
	g_iCurEditNo=9;

	Uart_Printf("CTeach Init ...\n");

	switch(g_Ini.iAxisCount)
	{
		case 2:
			{
				CList_SetMaxRowCol( ROWS, COLS2, MAXLINE,LISTHEIGHT, Widths2);
				m_ptrCtrl->w=Widths2[1]-2;
			}
			break;
			
		case 3:
			{
				CList_SetMaxRowCol( ROWS, COLS3, MAXLINE,LISTHEIGHT, Widths3);
				m_ptrCtrl->w=Widths3[1]-2;
			}
			break;
			
		case 4:
			{
				CList_SetMaxRowCol( ROWS, COLS4, MAXLINE,LISTHEIGHT, Widths4);
				m_ptrCtrl->w=Widths4[1]-2;
			}
			break;
		
		case 5:
			{
				CList_SetMaxRowCol( ROWS, COLS5, MAXLINE,LISTHEIGHT, Widths5);
				m_ptrCtrl->w=Widths5[1]-2;
			}
			break;
			
		case 6:
			{
				CList_SetMaxRowCol( ROWS, COLS6, MAXLINE,LISTHEIGHT, Widths6);
				m_ptrCtrl->w=Widths6[1]-2;
			}
			break;
			
		default:
			{
				CList_SetMaxRowCol( ROWS, COLS6, MAXLINE,LISTHEIGHT, Widths6);
				m_ptrCtrl->w=Widths6[1]-2;
			}
			break;
	}
	
	m_ptrCtrl->x = CList_CurCol()+2;
	m_ptrCtrl->y = LISTTOP+CList_CurRow()*LISTHEIGHT+2;
	m_ptrCtrl->h = LISTHEIGHT-2;
	
	CEdit_SetCtrl(m_ptrCtrl);
	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	CEdit_Create();
	
	CTelCom_Init(COM2,14400L);  					//测试旋钮

//	InstallDriverInterrupt();
	
	ReadDataFromFile(g_Ini.iFileName);
	ReadRunCount();
	
//	WriteDataToFile(g_Ini.iFileName);
		
	g_bModifySave=TRUE;
	
	//CTimer_SetTimes_1(20000);

}


void	CTeach_Destroy()
{
	CMsg_Destroy();
	CList_Destroy();
	Ok_Exit=FALSE;
}


void	CTeach_Create()
{
	INT8S	buf[0x10]="";
	BOOL	bLogo = FALSE;	
	INT8S   systemBuf[0x40]="";
	INT8S   *systemName[10]={"TH08R","TH412","TH416","CNC428","CNC820",""};
	
	g_bFirstDispTime = TRUE;
	g_iCommandPos = 0;
	g_bInterface = TRUE;

	
	if(g_Ini.iMachineType>=0 && g_Ini.iMachineType<7)
		strcpy(systemBuf,systemName[g_Ini.iMachineType]);
	else
		strcpy(systemBuf,"TH08R");	
		
	CDC_ClearScreen(g_nBk);
	
	bLogo = InitDispBMP("\\PARA\\LOGO.BMP");
	
	if(bLogo == TRUE)
	{
		CDC_LOCKSRC();
		DispBMP("\\PARA\\LOGO.BMP", 1, 220, 2);
		CDC_UNLOCKSRC();
		
		//g_bInitbmp=InitDispBMP("\\PARA\\start.bmp");
	}
	else
	{
		if(g_bEnglish)
		{
			sprintf(buf,"v%0.2f\n",VER/100.0);		
			strcat(systemBuf, " Spring System");	
			CDC_OutText_UTF(250,8,systemBuf,g_TextColor1,g_nBk,KZK_32);
		//	CDC_OutText(610,17,buf,REDCOLOR,g_nBk);
		}
		else
		{
			sprintf(buf,"v%0.2f\n",VER/100.0);
			strcat(systemBuf, " 弹簧机控制系统");
			CDC_OutText_UTF(240,8,systemBuf,g_TextColor1,g_nBk,KZK_32);
		//	CDC_OutText_UTF(540,12,buf,REDCOLOR,g_nBk,MZK_24);		
		}
	}

	
	if(!g_bEnglish)
	{
		//sprintf(buf,"v%0.2f\n",VER/100.0);
		//CDC_OutText_UTF(260,8,"弹簧机控制系统",g_TextColor1,g_nBk,KZK_32);
		//CDC_OutText_UTF(470,12,buf,REDCOLOR,g_nBk,MZK_24);


		CDC_OutText_UTF(8,45,"程序编号:          ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(210,45,"目标数量:             个",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(422,45,"探针设定:      次",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(584,45,"当前日期:",g_TextColor1,g_nBk,KZK_22);
		
		CDC_OutText_UTF(8,70,"加工速度:       个/分",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(210,70,"成品数量:             个",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(422,70,"探针失败:      次",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(584,70,"当前时间:",g_TextColor1,g_nBk,KZK_22);
	
	}
	else 
	{
		//sprintf(buf,"v%0.2f\n",VER/100.0);
		//CDC_OutText_UTF(160,8,"CNC820 Spring Control System",g_TextColor1,g_nBk,KZK_32);
		//CDC_OutText(610,17,buf,REDCOLOR,g_nBk);
		CDC_OutText_UTF(8,47,"PrgrmN:          ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(210,47,"CntSet:            ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(410,47,"PrbSet:      ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(574,47,"CurDate:",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(8,71,"RunSpd:       PPM",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(210,71,"CurCnt:           ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(410,71,"PrbFail:       ",g_TextColor1,g_nBk,KZK_22);
		CDC_OutText_UTF(574,71,"CurTime:",g_TextColor1,g_nBk,KZK_22);
	}
	
	
	CDC_Line(0,0,getmaxx(),0,g_TextColor8);
	CDC_Line(0,1,getmaxx(),1,g_TextColor8);
	CDC_Line(0,2,getmaxx(),2,g_TextColor8);
	
	CDC_Line(0,getmaxy(),getmaxx(),getmaxy(),g_TextColor8);
	CDC_Line(0,getmaxy()-1,getmaxx(),getmaxy()-1,g_TextColor8);
	CDC_Line(0,getmaxy()-2,getmaxx(),getmaxy()-2,g_TextColor8);
	
	CDC_Line(0,0,0,getmaxy(),g_TextColor8);
	CDC_Line(1,0,1,getmaxy(),g_TextColor8);
	CDC_Line(2,0,2,getmaxy(),g_TextColor8);
	
	CDC_Line(getmaxx(),0,getmaxx(),getmaxy(),g_TextColor8);
	CDC_Line(getmaxx()-1,0,getmaxx()-1,getmaxy(),g_TextColor8);
	CDC_Line(getmaxx()-2,0,getmaxx()-2,getmaxy(),g_TextColor8);
	
	
	
	CDC_Line(CList_Left()-3,CList_Top()-CList_Height()-3,getmaxx(),CList_Top()-CList_Height()-3,g_TextColor8);
	CDC_Line(CList_Left()-3,CList_Top()-CList_Height()-2,getmaxx(),CList_Top()-CList_Height()-2,g_TextColor8);
	CDC_Line(CList_Left()-3,CList_Top()-CList_Height()-1,getmaxx(),CList_Top()-CList_Height()-1,g_TextColor8);
	
	CDC_Line(CList_Left()-3,CList_Top()+CList_MaxRow()*CList_Height()+3,getmaxx(),CList_Top()+CList_MaxRow()*CList_Height()+3,g_TextColor8);
	CDC_Line(CList_Left()-3,CList_Top()+CList_MaxRow()*CList_Height()+2,getmaxx(),CList_Top()+CList_MaxRow()*CList_Height()+2,g_TextColor8);
	CDC_Line(CList_Left()-3,CList_Top()+CList_MaxRow()*CList_Height()+1,getmaxx(),CList_Top()+CList_MaxRow()*CList_Height()+1,g_TextColor8);
	/*
	CDC_Line(0,getmaxy()-62,getmaxx(),getmaxy()-62,g_TextColor8);
	CDC_Line(0,getmaxy()-61,getmaxx(),getmaxy()-61,g_TextColor8);
	CDC_Line(0,getmaxy()-60,getmaxx(),getmaxy()-60,g_TextColor8);*/
	
	
	
	CDC_OutText24(g_bEnglish?35:10 ,510,g_bEnglish?"X:":axisMsg(1,":",FALSE),g_TextColor1,g_nBk);								//四个轴的坐标显示位置							
	CDC_OutText24(g_bEnglish?230:205,510,g_bEnglish?"Y:":axisMsg(2,":",FALSE),g_TextColor1,g_nBk);
	switch(g_Ini.iAxisCount)
	{
		case 4:CDC_OutText24(g_bEnglish?620:595,510,g_bEnglish?"A:":axisMsg(4,":",FALSE),g_TextColor1,g_nBk);
		case 3:CDC_OutText24(g_bEnglish?425:400,510,g_bEnglish?"Z:":axisMsg(3,":",FALSE),g_TextColor1,g_nBk);
			break;
		default:break;
	}
	
	
	CDC_OutText_UTF(6,getmaxy()-58,g_bEnglish?"CurRate:":"当前倍率:",g_TextColor1,g_nBk,KZK_22);
	CDC_OutText_UTF(165,getmaxy()-58,g_bEnglish?"CurMod:":"当前模式:",g_TextColor1,g_nBk,KZK_22);
	CDC_OutText_UTF(330,getmaxy()-58,g_bEnglish?"PrbState:":"探针状态:",g_TextColor1,g_nBk,KZK_22);
	CDC_OutText_UTF(564,getmaxy()-58,g_bEnglish?"CylState:":"气缸状态:",g_TextColor1,g_nBk,KZK_22);
	CDC_OutText_UTF(6,getmaxy()-31,g_bEnglish?"Msg:":"讯息:",g_TextColor1,g_nBk,KZK_24);
	
	if(g_bMode)
	{
		CDC_OutText_UTF(268,getmaxy()-58,g_bEnglish?"Test:":"测试",g_TextColor5,g_nBk,KZK_22);
	}
	else
		CDC_OutText_UTF(268,getmaxy()-58,g_bEnglish?"Auto":"自动",g_TextColor11,g_nBk,KZK_22);
		
	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
	LedOut(BOXLED,g_bHandset?1:0);
	
	//if(g_Ini.iHaveHandRate)
	{
		if(g_Ini.iHandStatus)
			CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOn ":"手轮开",g_TextColor11,g_nBk,KZK_22);
		else
			CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
	}
	
	CList_SetStartRow(0);												//创建主界面时让光标落在第2行的"开始角"列	
	CList_SetCurRow(1);
	CList_SetCurCol(2);
	
	
	CTeach_InvidateView(TRUE);											//列表显示
	

	
	if(g_iWorkStatus != RUNNING)
		InitLed();

	CTeach_UpdateSpeed(0);												//倍数速度更新
	UpdateFileDestProbe(0,0,0);											//程序编号，产量设定，探针设定
	

	
	UpdateAxisStatus();
	UpdateCurAxis();
	

	UpdateProbe(TRUE);
	UpdateCylinder(TRUE);
	UpdateSpeedCountProbe();											//运行速度，产量，探针失败次数,显示更新						
	//
	g_bInitbmp=InitDispBMP("\\PARA\\start.bmp");	
	g_iCurEditNo=9;
	

}


void	CTeach_InvidateView(BOOL init)
{
	INT16S size=0,i=0;
	
	if(init)
	{
		INT8S *title2[COLS2] ={"行号","指令","开始角","结束角","送线","速比","探针","气缸","延时"};
		INT8S *title3[COLS3] ={"行号","指令","开始角","结束角","送线","转芯","速比","探针","气缸","延时"};
		INT8S *title4[COLS4] ={"行号","指令","开始角","结束角","送线","转芯","卷取","速比","探针","气缸","延时"};
		
		
		INT8S *title5[COLS5] ={"行号","指令","开始角","结束角","送线","转芯","卷取","卷取","速比","探针","气缸"};
		INT8S *title6[COLS6] ={"行号","C","开始角","结束角","送线","转芯","卷取","卷取","夹耳","速比","T","气缸"};
		
		INT8S *title2e[COLS2] ={"No","C","BgnX","EndX","","Rat","Prb","Cylinder","Dly"};
		INT8S *title3e[COLS3] ={"No","C","BgnX","EndX","","","Rat","Prb","Cylinder","Dly"};
		INT8S *title4e[COLS4] ={"No","C","BgX","EdX","","","","Rat","Prb","Cylinder","Dly"};	
		
		
		INT8S *title5e[COLS5] ={"No","C","BgX","EdX","","","","","Rat","Prb","Cylind"};
		INT8S *title6e[COLS6] ={"No","C","BgX","EdX","","","","","","Rat","P","Cylind"};


		/*strcpy(title2[4],axisMsg(2,"",FALSE));
		strcpy(title3[4],axisMsg(2,"",FALSE));
		strcpy(title4[4],axisMsg(2,"",FALSE));
		strcpy(title5[4],axisMsg(2,"",FALSE));
		strcpy(title6[4],axisMsg(2,"",FALSE));
			
		strcpy(title3[5],axisMsg(3,"",FALSE));
		strcpy(title4[5],axisMsg(3,"",FALSE));
		strcpy(title5[5],axisMsg(3,"",FALSE));
		strcpy(title6[5],axisMsg(3,"",FALSE));
		
		strcpy(title4[6],axisMsg(4,"",FALSE));
		strcpy(title5[6],axisMsg(4,"",FALSE));
		strcpy(title6[6],axisMsg(4,"",FALSE));
		
		strcpy(title5[7],axisMsg(5,"",FALSE));
		strcpy(title6[7],axisMsg(5,"",FALSE));

		strcpy(title6[8],axisMsg(6,"",FALSE));*/


		switch(g_Ini.iAxisCount)
		{	
			case 2:
				{
					CList_DrawTitle( g_bEnglish?title2e:title2 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
				
			case 3:
				{
					CList_DrawTitle( g_bEnglish?title3e:title3 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
				
			case 4:
				{
					CList_DrawTitle( g_bEnglish?title4e:title4 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
				
			case 5:
				{
					CList_DrawTitle( g_bEnglish?title5e:title5 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
				
			case 6:
				{
					CList_DrawTitle( g_bEnglish?title6e:title6 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
				
			default:
				{
					CList_DrawTitle( g_bEnglish?title6e:title6 ,g_TextColor11,g_nTitleBk,1);
				}
				break;
		}
		
	
	}
	
	CTeach_DrawGrid();
	CTeach_InitData();
	
	size = Line_Data_Size;
	
	if( size < 1 )
	{
		return;
	}
	
	size = size - CList_StartRow();
	
	if( size > ROWS ) size = ROWS;
	
	//Uart_Printf("row=%d\n",CList_StartRow());
	
	
	for(i=0; i<ROWS; i++)
	   CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);
	
	CTeach_Update(0,0);

		
}

//清空列表
void	CTeach_DrawGrid()
{

	CRect  rt1,rt2;
	
	CList_GetItemRect(0,0,&rt1);
	
	switch(g_Ini.iAxisCount)
	{
		case 2:	
			CList_GetItemRect(ROWS-1,COLS2-1,&rt2);
			break;
			
		case 3:	
			CList_GetItemRect(ROWS-1,COLS3-1,&rt2);
			break;
			
		case 4:	
			CList_GetItemRect(ROWS-1,COLS4-1,&rt2);
			break;
			
		case 5:	
			CList_GetItemRect(ROWS-1,COLS5-1,&rt2);
			break;
			
		case 6:	
			CList_GetItemRect(ROWS-1,COLS6-1,&rt2);
			break;
			
		default:
			CList_GetItemRect(ROWS-1,COLS6-1,&rt2);
			break;
	}

	CDC_PutColorBar(rt1.left,rt1.top,rt2.right-rt1.left,rt2.bottom-rt1.top,g_nTableBk,100);
	CDC_Rectangle(rt1.left,rt1.top,rt2.right,rt2.bottom,g_nTableBk);
	
	
}


void	UpdateSpeedCountProbe()
{
	char buffer[0x10];
	
	
	sprintf(buffer,"%d",g_lProbeFail);
	CDC_OutTextLR_UTF(519,67,buffer,g_TextColor11,g_nBk,MZK_24,2,1);		//探针失败
	

	sprintf(buffer,"%d",(INT32S)(g_fRunSpeed+0.5));
	CDC_OutTextLR_UTF(110,67,buffer,g_TextColor11,g_nBk,MZK_24,2,1);		//运行速度
	
	sprintf(buffer,"%d",g_Ini.lRunCount);
	CDC_OutTextLR_UTF(307,67,buffer,g_TextColor11,g_nBk,MZK_24,6,1);		//产量

}



void	UpdateFileDestProbe(INT16S no,INT16S dir,INT16S IsEnter)
{


	if( no==0 )
	{
		char buffer[0x09];
		
		INT16S i = 0;
		
		sprintf(buffer,"%d",g_Ini.iFileName);
		CDC_OutTextLR_UTF(g_EditRect[0].left,g_EditRect[0].top,buffer,g_TextColor11,g_nBk,MZK_24,3,1);
		
		sprintf(buffer,"%d",g_Ini.lDestCount);
		CDC_OutTextLR_UTF(g_EditRect[1].left+2,g_EditRect[1].top,buffer,g_TextColor11,g_nBk,MZK_24,6,1);
		
		sprintf(buffer,"%d",g_Ini.lProbeCount);
		CDC_OutTextLR_UTF(g_EditRect[2].left+2,g_EditRect[2].top,buffer,g_TextColor11,g_nBk,MZK_24,2,1);
		
	/*	for(i=0;i<g_Ini.iAxisCount;i++)
		{
			
			{
				sprintf(buffer,"%-1d",g_Ini.iStepRate[i]);
				
				CDC_OutText_UTF(g_StepEditRectEx[i].left+2,g_StepEditRectEx[i].top,buffer,g_TextColor11,g_nBk,MZK_24);
			}
			
			{
				sprintf(buffer,"%-2d",g_Ini.iStepDis[i]);
				
				CDC_OutText_UTF(g_StepEditRect[i].left+2,g_StepEditRect[i].top,buffer,g_TextColor11,g_nBk,MZK_24);
			}
		}*/
		
		g_iCurEditNo=9;
		
	}
	else
	{

		INT8S buffer[0x10];
		
		switch(no)
		{
		case 1://文件
			{
				if(dir<0)
				{
					if(g_iCurEditNo==9)
					{
						//CEdit_SetBackColor(YELLOWCOLOR);
						//CEdit_UpdateData(TRUE);
						CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
										CList_CurRow(),Reset,g_TextColor8,
										g_nTableBk);
					}
					else
					{
						CTeach_DrawEditRect();
					}
				
					g_iCurEditNo=0;
				    
					//CEdit_SetBackColor(YELLOWCOLOR);
					CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
					CEdit_SetFirst(TRUE);
					
					switch(g_iCurEditNo)
					{
						case 0:	CEdit_SetInt(g_Ini.iFileName);CEdit_SetLen(3);break;
						case 1:	CEdit_SetLong(g_Ini.lDestCount);CEdit_SetLen(7);break;
						case 2:	CEdit_SetLong(g_Ini.lProbeCount);CEdit_SetLen(3);break;					
						case 3:	CEdit_SetLong(g_Ini.lRunSpeed[0]);CEdit_SetLen(3);break;
						
						default:break;
					}
					
					CEdit_UpdateData(FALSE);
				}
				else
				{	
					INT16S temp;
				
					if(IsEnter>0)
					{
						temp=g_Ini.iFileName;
						
						if(g_iWorkStatus==NORUN)
							g_Ini.iFileName=CEdit_GetInt();
						else
							OSQPost(MsgQueue,g_bEnglish?"System is runnig,cannot load file!":"正在加工不能装入文件!");
							
						if(temp!=g_Ini.iFileName)
						{	
							INT32S	a,pulse;
							FP32	tmpf[6],offset;
						
							tmpf[0]=g_Ini.fStartOffset[0];
							tmpf[2]=g_Ini.fStartOffset[2];
							tmpf[3]=g_Ini.fStartOffset[3];
							tmpf[4]=g_Ini.fStartOffset[4];
							tmpf[5]=g_Ini.fStartOffset[5];
							

							WriteDataToFile(temp);

							g_Ini.lRunCount = 0;
							g_Ini.lRunSpeed[0]=30;	
							g_Ini.lProbeCount=1;
							g_Ini.lDestCount=100000;
							g_fRunSpeed = 0.0;
							g_lProbeFail = 0;
						
							ReadDataFromFile(g_Ini.iFileName);
							WriteRunCount();
							
							g_bModify=TRUE;
							g_bModifySave=TRUE;

							for(a = 1;a <= g_Ini.iAxisCount;a++ )
							{
								offset = tmpf[a-1]-g_Ini.fStartOffset[a-1];
								
								if(a==2 || fabs(offset)<0.000001)
									continue;
								if(g_Ini.iAxisRunMode[a-1]==2)
									pulse = GetPulse(a,TRUE)+GetPulseFromMM(a,offset);
								else
									pulse = GetPulse(a,TRUE)+GetPulseFromAngle(a,offset);
								
								BaseSetCommandPos(a,pulse);
						
							}
							

							CDC_PutColorBar(306,67,75,24,g_nBk,1);
							UpdateSpeedCountProbe();

							{
								INT32S t1,t2;
								
								OSSemPost(UnpackDataSem);
								g_bUnpackCorrect = FALSE;
								
								t1=OSTimeGet();
								
								while(TRUE)
								{
									OSTimeDly(2);
									
									if(g_bUnpackCorrect)
										break;
										
									t2=OSTimeGet();
									
									if((t2-t1)>200||(t2-t1<-200))
										break;
								}
								
								if(!g_bUnpackCorrect)
								{
									CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nBk,1);
									
									sprintf(buffer,"%-3d ",g_Ini.iFileName);
									
									CDC_OutText_UTF(g_EditRect[0].left,g_EditRect[0].top,buffer,g_TextColor11,g_nBk,MZK_24);
									
									SysParaWrite();
									
									
									break;
								}								
							}
							
							sprintf(g_pszMsgBuf,g_bEnglish?"Load No.%d file!":"载入%d号文件!",g_Ini.iFileName);
							OSQPost(MsgQueue,g_pszMsgBuf);
						}
						else
						{
							CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nBk,1);
							sprintf(buffer,"%-3d ",g_Ini.iFileName);
							CDC_OutText_UTF(g_EditRect[0].left,g_EditRect[0].top,buffer,g_TextColor11,g_nBk,MZK_24);
						
						}
					}
					
					SysParaWrite();
					WriteRunCount();
					
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nBk,1);
					sprintf(buffer,"%-3d ",g_Ini.iFileName);
					CDC_OutText_UTF(g_EditRect[0].left,g_EditRect[0].top,buffer,g_TextColor11,g_nBk,MZK_24);
					
					
					g_iCurEditNo=9;
					CTeach_Update(0,0);
					
					UpdateAxisStatus();
					UpdateCurAxis();
				
				}
			}
			break;
		case 2://目标产量
			{
				if(dir<0)
				{
					
					if(g_iCurEditNo==9)
					{
						//CEdit_SetBackColor(YELLOWCOLOR);
						//CEdit_UpdateData(TRUE);
						CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
										CList_CurRow(),Reset,g_TextColor8,
										g_nTableBk);
					}
					else
					{
						
						CTeach_DrawEditRect();
					}
					
					g_iCurEditNo=1;
					//CEdit_SetBackColor(YELLOWCOLOR);
					CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
					CEdit_SetFirst(TRUE);
					
					switch(g_iCurEditNo)
					{
						case 0:	CEdit_SetInt(g_Ini.iFileName);CEdit_SetLen(3);break;
						case 1:	CEdit_SetLong(g_Ini.lDestCount);CEdit_SetLen(7);break;
						case 2:	CEdit_SetLong(g_Ini.lProbeCount);CEdit_SetLen(3);break;
						
						case 3:	CEdit_SetLong(g_Ini.lRunSpeed[0]);CEdit_SetLen(3);break;
						
						default:break;
					}
					
					CEdit_UpdateData(FALSE);
				}
				else
				{
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nBk,1);
					
					if(IsEnter>0)
					{
						g_Ini.lDestCount=CEdit_GetLong();
						
						if(g_Ini.lDestCount<1)
							g_Ini.lDestCount=1;
							
						sprintf(buffer,"%-7d ",g_Ini.lDestCount);
						CDC_OutText_UTF(g_EditRect[1].left+2,g_EditRect[1].top,buffer,g_TextColor11,g_nBk,MZK_24);
						
						SysParaWrite();
						WriteDataToFile(g_Ini.iFileName);
					}
					else
					{
						sprintf(buffer,"%-7d ",g_Ini.lDestCount);
						CDC_OutText_UTF(g_EditRect[1].left+2,g_EditRect[1].top,buffer,g_TextColor11,g_nBk,MZK_24);
					}
					
					g_iCurEditNo=9;
					CTeach_Update(0,0);
				}
			}
			break;	
			
		case 3://探针次数
			{
				if(dir<0)
				{
					
					if(g_iCurEditNo==9)
					{
						//CEdit_SetBackColor(YELLOWCOLOR);
						//CEdit_UpdateData(TRUE);
						CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
										CList_CurRow(),Reset,g_TextColor8,
										g_nTableBk);
					}
					else
					{
						CTeach_DrawEditRect();
					}
					
				    
					g_iCurEditNo=2;
					
					//CEdit_SetBackColor(YELLOWCOLOR);
					CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
					CEdit_SetFirst(TRUE);
					
					switch(g_iCurEditNo)
					{
						case 0:	CEdit_SetInt(g_Ini.iFileName);CEdit_SetLen(3);break;
						case 1:	CEdit_SetLong(g_Ini.lDestCount);CEdit_SetLen(7);break;
						case 2:	CEdit_SetLong(g_Ini.lProbeCount);CEdit_SetLen(3);break;				
						case 3:	CEdit_SetLong(g_Ini.lRunSpeed[0]);CEdit_SetLen(3);break;
						default:break;
					}
					
					CEdit_UpdateData(FALSE);
				}
				else
				{	
					
					CDC_PutColorBar2(g_EditRect[2],g_nBk,1);
					
					if(IsEnter>0)
					{
						g_Ini.lProbeCount=CEdit_GetLong();
						
						if(g_Ini.lProbeCount<0) 
							g_Ini.lProbeCount=0;
					
						sprintf(buffer,"%-3d ",g_Ini.lProbeCount);
						
						CDC_OutText_UTF(g_EditRect[2].left+2,g_EditRect[2].top,buffer,g_TextColor11,g_nBk,MZK_24);
						
						SysParaWrite();
						WriteDataToFile(g_Ini.iFileName);
					}
					else
					{
						sprintf(buffer,"%-3d ",g_Ini.lProbeCount);
						CDC_OutText_UTF(g_EditRect[2].left+2,g_EditRect[2].top,buffer,g_TextColor11,g_nBk,MZK_24);
					}
					
					g_iCurEditNo=9;
					CTeach_Update(0,0);
				}
			}
			break;
		/*
		case 4:													//寸动
			{		
				if(dir<=0)
				{
					if(g_iCurEditNo==9)
						CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
										CList_CurRow(),Reset,g_TextColor6,
										g_nTableBk);
					else
					{
					
						CTeach_DrawEditRect();
						
					}
					
					g_iCurEditNo=3;
					UpdateStep(dir);
					
					if((g_iStepEditNo%2)==1)
					{
						g_EditRect[3].left=g_StepEditRectEx[g_iStepEditNo/2].left;
						g_EditRect[3].top=g_StepEditRectEx[g_iStepEditNo/2].top;
						g_EditRect[3].right=g_StepEditRectEx[g_iStepEditNo/2].right;
						g_EditRect[3].bottom=g_StepEditRectEx[g_iStepEditNo/2].bottom;
					}
					else
					{
						g_EditRect[3].left=g_StepEditRect[g_iStepEditNo/2].left;
						g_EditRect[3].top=g_StepEditRect[g_iStepEditNo/2].top;
						g_EditRect[3].right=g_StepEditRect[g_iStepEditNo/2].right;
						g_EditRect[3].bottom=g_StepEditRect[g_iStepEditNo/2].bottom;
					}
					
					CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
					CEdit_SetFirst(TRUE);
					
					switch(g_iCurEditNo)
					{
					case 0:	CEdit_SetInt(g_Ini.iFileName);CEdit_SetLen(3);break;
					case 1:	CEdit_SetLong(g_Ini.lDestCount);CEdit_SetLen(6);break;
					case 2:	CEdit_SetLong(g_Ini.lProbeCount);CEdit_SetLen(5);break;
					case 3:
							if(g_iStepEditNo%2==1)
							{
								CEdit_SetInt(g_Ini.iStepRate[g_iStepEditNo/2]);
								CEdit_SetLen(1);
							}
							else
							{
								CEdit_SetInt(g_Ini.iStepDis[g_iStepEditNo/2]);
								CEdit_SetLen(2);
							}break;
					default:break;
					}					
					CEdit_UpdateData(TRUE);
				}
				else
				{	
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nBk,1);
					
					
					if(IsEnter>0)
					{
						if(g_iStepEditNo%2==1)
						{
							g_Ini.iStepRate[g_iStepEditNo/2]=CEdit_GetInt();
							
							if(g_Ini.iStepRate[g_iStepEditNo/2]<=0)
								g_Ini.iStepRate[g_iStepEditNo/2]=1;
							
							sprintf(buffer,"%-1d ",g_Ini.iStepRate[g_iStepEditNo/2]);
							
							CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
						}
						else
						{
							g_Ini.iStepDis[g_iStepEditNo/2]=CEdit_GetInt();
							
							if(g_Ini.iStepDis[g_iStepEditNo/2]<=0)
								g_Ini.iStepDis[g_iStepEditNo/2]=1;
							
							if(g_Ini.iStepDis[g_iStepEditNo/2]>999)
								g_Ini.iStepDis[g_iStepEditNo/2]=999;
							
							sprintf(buffer,"%-2d ",g_Ini.iStepDis[g_iStepEditNo/2]);
							
							CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
						}
						
						UpdateStep(1);
						
						if(g_iStepEditNo%2==1)
						{
							g_EditRect[3].left=g_StepEditRectEx[g_iStepEditNo/2].left;
							g_EditRect[3].top=g_StepEditRectEx[g_iStepEditNo/2].top;
							g_EditRect[3].right=g_StepEditRectEx[g_iStepEditNo/2].right;
							g_EditRect[3].bottom=g_StepEditRectEx[g_iStepEditNo/2].bottom;
						}
						else
						{
							g_EditRect[3].left=g_StepEditRect[g_iStepEditNo/2].left;
							g_EditRect[3].top=g_StepEditRect[g_iStepEditNo/2].top;
							g_EditRect[3].right=g_StepEditRect[g_iStepEditNo/2].right;
							g_EditRect[3].bottom=g_StepEditRect[g_iStepEditNo/2].bottom;
						}
						
						CEdit_MoveWindowRect(g_EditRect[3]);
						CEdit_SetFirst(TRUE);
						
						if(g_iStepEditNo%2==1)
						{
							CEdit_SetInt(g_Ini.iStepRate[g_iStepEditNo/2]);
							CEdit_SetLen(1);
						}
						else
						{
							CEdit_SetInt(g_Ini.iStepDis[g_iStepEditNo/2]);
							CEdit_SetLen(2);
						}
						
						SysParaWrite();
						
						CEdit_UpdateData(TRUE);
					}
					else
					{
						if(IsEnter==0)
						{
							if(g_iStepEditNo%2==1)
							{
								sprintf(buffer,"%-1d ",g_Ini.iStepRate[g_iStepEditNo/2]);
								CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
							}
							else
							{
								sprintf(buffer,"%-2d ",g_Ini.iStepDis[g_iStepEditNo/2]);
								CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
							}
							
							OSQPost(MsgQueue," ");
							CTeach_Update(0,0);
						}
						else
						{
							if(g_iStepEditNo%2==1)
							{							
								sprintf(buffer,"%-1d ",g_Ini.iStepRate[g_iStepEditNo/2]);
								CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
							}
							else
							{				
								sprintf(buffer,"%-1d ",g_Ini.iStepDis[g_iStepEditNo/2]);
								CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
							}
							
							UpdateStep(1);
							
														
							if(g_iStepEditNo%2==1)
							{
								g_EditRect[3].left=g_StepEditRectEx[g_iStepEditNo/2].left;
								g_EditRect[3].top=g_StepEditRectEx[g_iStepEditNo/2].top;
								g_EditRect[3].right=g_StepEditRectEx[g_iStepEditNo/2].right;
								g_EditRect[3].bottom=g_StepEditRectEx[g_iStepEditNo/2].bottom;
							}
							else
							{
								g_EditRect[3].left=g_StepEditRect[g_iStepEditNo/2].left;
								g_EditRect[3].top=g_StepEditRect[g_iStepEditNo/2].top;
								g_EditRect[3].right=g_StepEditRect[g_iStepEditNo/2].right;
								g_EditRect[3].bottom=g_StepEditRect[g_iStepEditNo/2].bottom;
							}
							
							CEdit_MoveWindowRect(g_EditRect[3]);
							CEdit_SetFirst(TRUE);
							
							if(g_iStepEditNo%2==1)
							{
								CEdit_SetInt(g_Ini.iStepRate[g_iStepEditNo/2]);
								CEdit_SetLen(1);
							}
							else
							{
								CEdit_SetInt(g_Ini.iStepDis[g_iStepEditNo/2]);
								CEdit_SetLen(2);
							}
							
							CEdit_UpdateData(TRUE);
						}
					}
				}
			}
			break;*/
		}
	}
}


void	UpdateAxisStatus()
{
	INT16S x=CList_Left(),w;
	INT16S i;
	CRect rect;
	
	
	switch(g_Ini.iAxisCount)
	{
		case 2:
			for(i=0;i<4;i++)
				x+=Widths2[i];
				
			w=Widths2[4];
			rect.left=x+2;
			rect.top=CList_Top()-CList_Height();
			rect.right=x+w-1;
			rect.bottom=CList_Top();
			
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
			
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			}
				
			break;
	
		case 3:
			for(i=0;i<4;i++)
				x+=Widths3[i];
				
			w=Widths3[4];
			rect.left=x+2;
			rect.top=CList_Top()-CList_Height();
			rect.right=x+w-1;
			rect.bottom=CList_Top();
			
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
			
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths3[5];
			rect.left=x+2;
			rect.right=x+w-1;
			
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[2]==1?"Z":"Z",g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk,1);
				CDC_OutCenTextKzk24(rect,axisMsg(3,"",FALSE),g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			}
				
			break;
	
		case 4:
			for(i=0;i<4;i++)
				x+=Widths4[i];
				
			w=Widths4[4];
			rect.left=x+2;
			rect.top=CList_Top()-CList_Height();
			rect.right=x+w-1;
			rect.bottom=CList_Top();
			
			
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
			
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths4[5];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[2]==1?"Z":"Z",g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(3,"",FALSE),g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths4[6];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[3]==1?"A":"A",g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(4,"",FALSE),g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			}
				
			break;
			
		case 5:
			for(i=0;i<4;i++)
				x+=Widths5[i];
				
			w=Widths5[4];
			rect.left=x+2;
			rect.top=CList_Top()-CList_Height();
			rect.right=x+w-1;
			rect.bottom=CList_Top();
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths5[5];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[2]==1?"Z":"Z",g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(3,"",FALSE),g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths5[6];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[3]==1?"A":"A",g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(4,"",FALSE),g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			}
				
			x+=w;
			w=Widths5[7];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[4]==1?"B":"B",g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(5,"",FALSE),g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			break;
			
		case 6:
			for(i=0;i<4;i++)
				x+=Widths6[i];
				
			w=Widths6[4];
			rect.left=x+2;
			rect.top=CList_Top()-CList_Height();
			rect.right=x+w-1;
			rect.bottom=CList_Top();
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths6[5];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[2]==1?"Z":"Z",g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(3,"",FALSE),g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			x+=w;
			w=Widths6[6];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[3]==1?"A":"A",g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(4,"",FALSE),g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
			}
			
				x+=w;
			w=Widths6[7];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[4]==1?"B":"B",g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(5,"",FALSE),g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
			}
			
				x+=w;
			w=Widths6[8];
			rect.left=x+2;
			rect.right=x+w-1;
			CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk,1);
			if(g_bEnglish)
				CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[5]==1?"C":"C",g_Ini.iAxisSwitch[5]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk);
			else
			{
				
				CDC_OutCenTextKzk24(rect,axisMsg(6,"",FALSE),g_Ini.iAxisSwitch[5]==0?DARKGRAYCOLOR2:g_TextColor11,g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk);
			}
			
			break;
			
	default:return;
	}
	
}

void   UpdateLed()
{
	switch(g_iWorkNumber)
	{
	case 1:
	
		LedOut(XLED,1);
		LedOut(YLED,0);
		
		if(g_Ini.iAxisCount>2)
		{
			LedOut(ZLED,0);
		
		}
		
		if(g_Ini.iAxisCount>3)
		{
			LedOut(ULED,0);
		}
		
		if(g_Ini.iAxisCount>4)
		{
			LedOut(VLED,0);
		}
		
		if(g_Ini.iAxisCount>5)
		{
			LedOut(WLED,0);
		}
		break;
		
	case 2:
		LedOut(XLED,0);
		LedOut(YLED,1);

		if(g_Ini.iAxisCount>2)
		{
			LedOut(ZLED,0);
		}
		
		if(g_Ini.iAxisCount>3)
		{
			LedOut(ULED,0);
		}
		
		if(g_Ini.iAxisCount>4)
		{
			LedOut(VLED,0);
		}
		
		if(g_Ini.iAxisCount>5)
		{
			LedOut(WLED,0);
		}
		break;
		
	case 3:
		if(g_Ini.iAxisCount<3)
			break;
		LedOut(XLED,0);
		LedOut(YLED,0);
		LedOut(ZLED,1);

		if(g_Ini.iAxisCount>3)
		{
			LedOut(ULED,0);
		}
		
		
		if(g_Ini.iAxisCount>4)
		{
			LedOut(VLED,0);
		}
		
		if(g_Ini.iAxisCount>5)
		{
			LedOut(WLED,0);
		}
		break;
	case 4:
		if(g_Ini.iAxisCount<4)
			break;
			
		LedOut(XLED,0);
		LedOut(YLED,0);
		LedOut(ZLED,0);
		LedOut(ULED,1);
		if(g_Ini.iAxisCount>4)
		{
			LedOut(VLED,0);
		}
		
		if(g_Ini.iAxisCount>5)
		{
			LedOut(WLED,0);
		}
		break;	
		
		
	case 5:
		if(g_Ini.iAxisCount<5)
			break;
			
		LedOut(XLED,0);
		LedOut(YLED,0);
		LedOut(ZLED,0);
		LedOut(ULED,0);
		LedOut(VLED,1);
		if(g_Ini.iAxisCount>5)
		{
			LedOut(WLED,0);
		}
		break;
		
	
	case 6:
		if(g_Ini.iAxisCount<6)
			break;
			
		LedOut(XLED,0);
		LedOut(YLED,0);
		LedOut(ZLED,0);
		LedOut(ULED,0);
		LedOut(VLED,0);
		LedOut(WLED,1);
		break;
		
	
	default:
		LedOut(XLED,0);
		LedOut(YLED,0);
		LedOut(ZLED,0);
		LedOut(ULED,0);
		LedOut(VLED,0);
		LedOut(WLED,0);
		break;
	}

}


void	UpdateCurAxis()
{
	INT16S x1,xw1,x2,xw2,y,yw,z=0,zw=0;
	INT16S u=0,uw=0;
	INT16S v=0,vw=0;
	INT16S w=0,ww=0;
	INT16S ytop=0;
 	CRect rect;
 	
	switch(g_Ini.iAxisCount)
	{
	case 2:
		x1=Widths2[0]+Widths2[1]+CList_Left();
		xw1=Widths2[2];
		x2=x1+xw1;
		xw2=Widths2[3];
		y=x2+xw2;
		yw=Widths2[4];
		break;
	case 3:
		x1=Widths3[0]+Widths3[1]+CList_Left();
		xw1=Widths3[2];
		x2=x1+xw1;
		xw2=Widths3[3];
		y=x2+xw2;
		yw=Widths3[4];
		z=y+yw;
		zw=Widths3[5];
		break;
	case 4:
		x1=Widths4[0]+Widths4[1]+CList_Left();
		xw1=Widths4[2];
		x2=x1+xw1;
		xw2=Widths4[3];
		y=x2+xw2;
		yw=Widths4[4];
		z=y+yw;
		zw=Widths4[5];
		u=z+zw;
		uw=Widths4[6];
		break;
		
	case 5:
		x1=Widths5[0]+Widths5[1]+CList_Left();
		xw1=Widths5[2];
		x2=x1+xw1;
		xw2=Widths5[3];
		y=x2+xw2;
		yw=Widths5[4];
		z=y+yw;
		zw=Widths5[5];
		u=z+zw;
		uw=Widths5[6];
		v=u+uw;
		vw=Widths5[7];
		break;
		
	case 6:
		x1=Widths6[0]+Widths6[1]+CList_Left();
		xw1=Widths6[2];
		x2=x1+xw1;
		xw2=Widths6[3];
		y=x2+xw2;
		yw=Widths6[4];
		z=y+yw;
		zw=Widths6[5];
		u=z+zw;
		uw=Widths6[6];
		v=u+uw;
		vw=Widths6[7];
		w=v+vw;
		ww=Widths6[8];
		break;
	}
	
	
	UpdateLed();

	ytop=(CList_Height()-TEXT_HEIGHT)/2+CList_Top()-CList_Height();
	
	

	rect.left=x1+2;
	rect.top=CList_Top()-CList_Height();
	rect.right=x1+xw1-1;
	rect.bottom=CList_Top();
   	CDC_OutCenTextKzk24(rect,g_bEnglish?(g_Ini.iAxisCount>3?"BgX":"BgnX"):"开始角",(g_iWorkNumber == 1) ? g_TextColor5:g_TextColor11,g_nTitleBk);

	rect.left = x2+2;
	rect.right = x2+xw2-1;
   	CDC_OutCenTextKzk24(rect,g_bEnglish?(g_Ini.iAxisCount>3?"EdX":"EndX"):"结束角",(g_iWorkNumber == 1) ? g_TextColor5:g_TextColor11,g_nTitleBk);

	rect.left = y+2;
	rect.right = y+yw-1;
	
	CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk,1);
	if(g_bEnglish)
		CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[1]==1?"Y":"Y",g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 2) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
	else
	{
		
		CDC_OutCenTextKzk24(rect,axisMsg(2,"",FALSE),g_Ini.iAxisSwitch[1]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 2) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[1]==0?BLACKCOLOR:g_nTitleBk);
	}
	
   	switch(g_Ini.iAxisCount)
   	{
   	case 6: 
		rect.left = w+2;
		rect.right = w+ww-1;
		CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk,1);
		if(g_bEnglish)
			CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[5]==1?"C":"C",g_Ini.iAxisSwitch[5]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 6) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk);
		else
		{
			
			CDC_OutCenTextKzk24(rect,axisMsg(6,"",FALSE),g_Ini.iAxisSwitch[5]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 6) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[5]==0?BLACKCOLOR:g_nTitleBk);
		}
   	
   	case 5: 
		rect.left = v+2;
		rect.right = v+vw-1;
		CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk,1);
		if(g_bEnglish)
			CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[4]==1?"B":"B",g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 5) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
		else
		{
			
			CDC_OutCenTextKzk24(rect,axisMsg(5,"",FALSE),g_Ini.iAxisSwitch[4]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 5) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[4]==0?BLACKCOLOR:g_nTitleBk);
		}
   	
   	
   	case 4: 
		rect.left = u+2;
		rect.right = u+uw-1;
		CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk,1);
		if(g_bEnglish)
			CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[3]==1?"A":"A",g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 4) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
		else
		{
			
			CDC_OutCenTextKzk24(rect,axisMsg(4,"",FALSE),g_Ini.iAxisSwitch[3]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 4) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[3]==0?BLACKCOLOR:g_nTitleBk);
		}
		
   	case 3:
   		rect.left = z+2;
		rect.right = z+zw-1;
		CDC_PutColorBar2(rect,g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk,1);
   		if(g_bEnglish)
			CDC_OutCenTextKzk24(rect,g_Ini.iAxisSwitch[2]==1?"Z":"Z",g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 3) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
		else
		{
			
			CDC_OutCenTextKzk24(rect,axisMsg(3,"",FALSE),g_Ini.iAxisSwitch[2]==0?DARKGRAYCOLOR2:((g_iWorkNumber == 3) ? g_TextColor5:g_TextColor11),g_Ini.iAxisSwitch[2]==0?BLACKCOLOR:g_nTitleBk);
		}
		
		break;
 	}
			   
		
	
	
}


BOOL UpdateCommand(INT8S *str)
{
	 COMMAND *ptr=NULL;
	 //CRect rect;
	 
	 //INT16S  *pWidths;  
	 
	 if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
	 {
	  	return FALSE;
	 }  
	 
	 g_bModify = TRUE;
	 g_bModifySave = FALSE;
	 
	 ptr = (Line_Data+CList_StartRow()+CList_CurRow());
	 
	 strcpy(ptr->value[0], str );
	 
	/* switch(g_Ini.iAxisCount)
	 {
	  case 2:
	   	pWidths=Widths2;
	   	break;
	  case 3:
	   	pWidths=Widths3;
	   	break;
	  case 4:
	   	pWidths=Widths4;
	   	break;
	  
	  case 5:
	   	pWidths=Widths5;
	   	break;
	  case 6:
	   	pWidths=Widths6;
	   	break;
	   	
	  default:
	   	pWidths=Widths6;
	   	break; 
	 }
	 
	 CDC_PutColorBar(pWidths[0]+CList_Left()+2,CList_Top()+CList_CurRow()*CList_Height()+2,pWidths[1]-4,CList_Height()-4,g_nTableBk,1);
	 
	 rect.left=pWidths[0]+CList_Left();
	 rect.top=CList_Top()+CList_CurRow()*CList_Height();
	 rect.right=rect.left+pWidths[1];
	 rect.bottom=rect.top+LISTHEIGHT;
	 
	 CDC_OutCenTextSzk24(rect,ptr->value[0],GREENCOLOR,g_nTableBk);*/
	 
	 return TRUE;
}


BOOL	UpdateProbeCommand(INT8S *str)//暂时无用
{
	COMMAND *ptr=NULL;
	CRect	rect;
	
	if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
	{
		return FALSE;
	}
	
	g_bModify = TRUE;
	g_bModifySave=FALSE;
	ptr = (Line_Data+CList_StartRow()+CList_CurRow());
	strcpy(ptr->value[0], str );
	
	switch(g_Ini.iAxisCount)
	{
	case 2:
		CDC_PutColorBar(Widths2[0]+CList_Left()+1,CList_Top()+CList_CurRow()*CList_Height()+1,Widths2[1]-4,CList_Height()-4,BLUECOLOR,1);
		
		rect.left=Widths2[0]+CList_Left();
		rect.top=CList_Top()+CList_CurRow()*CList_Height();
		rect.right=rect.left+Widths2[1];
		rect.bottom=rect.top+LISTHEIGHT;
		
		//CDC_OutText_UTF(rect.left+8,rect.top+6,ptr->value[0],GREENCOLOR,BLUECOLOR,MZK_24);
		CDC_OutCenTextSzk24(rect,ptr->value[0],GREENCOLOR,BLUECOLOR);
		break;
		
	case 3:
		CDC_PutColorBar(Widths3[0]+CList_Left()+1,CList_Top()+CList_CurRow()*CList_Height()+1,Widths3[1]-4,CList_Height()-4,BLUECOLOR,1);
		
		rect.left=Widths3[0]+CList_Left()-1;
		rect.top=CList_Top()+CList_CurRow()*CList_Height();
		rect.right=rect.left+Widths3[1];
		rect.bottom=rect.top+LISTHEIGHT;
		
		//CDC_OutText_UTF(rect.left+8,rect.top+6,ptr->value[0],GREENCOLOR,BLUECOLOR,MZK_24);
		CDC_OutCenTextSzk24(rect,ptr->value[0],GREENCOLOR,BLUECOLOR);
		break;
		
	case 4:
		CDC_PutColorBar(Widths4[0]+CList_Left()+1,CList_Top()+CList_CurRow()*CList_Height()+1,Widths4[1]-4,CList_Height()-4,BLUECOLOR,1);
		
		rect.left=Widths4[0]+CList_Left()-1;
		rect.top=CList_Top()+CList_CurRow()*CList_Height();
		rect.right=rect.left+Widths4[1];
		rect.bottom=rect.top+LISTHEIGHT;
		CDC_OutCenTextSzk24(rect,ptr->value[0],GREENCOLOR,BLUECOLOR);
		//CDC_OutText_UTF(rect.left+8,rect.top+6,ptr->value[0],GREENCOLOR,BLUECOLOR,MZK_24);
		break;
	}
	return TRUE;
}




void	CTeach_Update( INT16S row,INT16S col )
{
	INT16S 	maxSize;
	INT16S 	oldrow,newrow;//,oldcol,newcol;	
	CRect 	rect;
	COMMAND *ptr;
	INT16S 	t = CList_CurCol() + col;
	
	if( t < 4 )
		g_iWorkNumber = 1;
	else
	{

		if( t< CList_MaxCol()-4)
			g_iWorkNumber= t-2;
		else
			g_iWorkNumber=0;													//没有选择任何一个轴
	}

	if(col<0)
	{
		if(CList_CurCol()==2)
			return;
	}

	if(col>0)
	{
		if(CList_CurCol()==CList_MaxCol()-1)
			return;
	}
	
	if( row < 0 )
	{
		if(col!=0)
		{
			//oldcol = CList_CurCol();
			
			CList_ColAdd( col );
		
			if( CList_CurCol() <= 1 )
				CList_SetCurCol(2);
		
			if(CList_CurCol()>=CList_MaxCol())
				CList_SetCurCol(CList_MaxCol()-1);
		}
		
		if( CList_CurRow()==0 && CList_StartRow()!=0)
		{
			//CTeach_UpdatePage( row );
			CList_SetCurRow(ROWS-2);
			CTeach_UpdatePage( -(ROWS-1) );
			return;
		}
		
		if(CList_StartRow()==0 && CList_CurRow()==0)
			return;
	}
	
	if( row > 0 )
	{
		if(col!=0)
		{
			//oldcol = CList_CurCol();
			
			CList_ColAdd( col );
			
			if( CList_CurCol() <= 1 )
				CList_SetCurCol(2);
			
			if(CList_CurCol()>=CList_MaxCol())
				CList_SetCurCol(CList_MaxCol()-1);
			
		}
		
		
		
		maxSize = Line_Data_Size - CList_StartRow();
	
		if( maxSize > ROWS ) 
			maxSize = ROWS;
		
		
		if( CList_CurRow() + row >= maxSize)
		{
			CList_SetCurRow(1);					
			CTeach_UpdatePage( maxSize-1 );
			return;
		}
	}
	
//	CTeach_DrawItem( CList_CurRow()+CList_StartRow(), CList_CurRow() ,Reset,g_TextColor1,g_nTableBk);
    
	oldrow = CList_CurRow();
    
	CList_ColAdd( col );
	CList_RowAdd( row );

	if(CList_CurRow()<0)
		CList_SetCurRow(0);
	
	if( CList_CurCol() <= 1 )
		CList_SetCurCol(2);
	
	if(CList_CurCol() >= CList_MaxCol())
		CList_SetCurCol(CList_MaxCol()-1);
	
	if(CList_CurRow() >= ROWS)
		CList_SetCurRow(ROWS-1);
		

	newrow = CList_CurRow();
	
	if(g_bRefresh)
	{
		CEdit_SetTextColor(WHITECOLOR);
		CEdit_SetBackColor(BLACKCOLOR);
	}
	else
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(WHITECOLOR);
	}
	

	{
		CTeach_DrawItem( CList_StartRow()+oldrow, oldrow ,Reset, g_TextColor6,g_nTableBk);
		
		 if(g_bRunGotoLine==FALSE)
	  		CTeach_DrawItem( CList_StartRow()+newrow, newrow ,Active,g_TextColor6,g_nTableBk);
	 	 else
	   		CTeach_DrawItem( CList_StartRow()+newrow, newrow ,MoveActive,WHITECOLOR,g_nTableBk);

	}	

	OSTimeDly(1);
	
	CList_GetItemRect(CList_CurRow(),CList_CurCol(),&rect);
	
	//CEdit_SetTextColor(BLACKCOLOR);
	//CEdit_SetBackColor(WHITECOLOR);
	CEdit_MoveWindowRect(rect);
	
	if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
		return;
	
	ptr = ( Line_Data+CList_StartRow()+CList_CurRow());
	
	
	
	
	if(g_Ini.iAxisCount == 2)
	{
		switch(CList_CurCol())
		{
			case 2:
			case 3:
				{
					CEdit_SetLen(6); 
					//CTeach_UpdateRange(1);
				}
				break;
			case 4:
				{
					CEdit_SetLen(7); 
					//CTeach_UpdateRange(2);
				}
				break;
			case 5:
				{
					CEdit_SetLen(4);	
					//CTeach_UpdateRange(5);				
				}
				break;
			case 6:
				{
					CEdit_SetLen(2);	
					//CTeach_UpdateRange(6);			
				}
				break;
			case 7:
				{
					CEdit_SetLen(7);		
					//CTeach_UpdateRange(4);			
				}
				break;
			case 8:
				{
					CEdit_SetLen(4);	
					//CTeach_UpdateRange(4);			
				}
				break;
			
		}
	}
	else if(g_Ini.iAxisCount == 3)
	{
		
		switch(CList_CurCol())
		{
			case 2:
			case 3:
			case 5:
				{
					CEdit_SetLen(6); 
				}
				break;
			case 4:
				{
					CEdit_SetLen(7); 
					//CTeach_UpdateRange(2);
				}
				break;
			case 6:
				{
					CEdit_SetLen(4);	
					//CTeach_UpdateRange(5);				
				}
				break;
			case 7:
				{
					CEdit_SetLen(2);	
					//CTeach_UpdateRange(6);			
				}
				break;
			case 8:
				{
					CEdit_SetLen(7);	
					//CTeach_UpdateRange(4);			
				}
				break;
				
			case 9:
				{
					CEdit_SetLen(4);	
				}
				break;
			
		}
	}
	else if(g_Ini.iAxisCount == 4)
	{
		switch(CList_CurCol())
		{
			case 2:
			case 3:
			case 5:
			case 6:
				{
					
					CEdit_SetLen(6); 
					//CTeach_UpdateRange(1);
				}
				break;
			case 4:											//送线
				{
					CEdit_SetLen(7); 
					//CTeach_UpdateRange(8);
				}
				break;
			case 7:											//速比
				{
					CEdit_SetLen(4); 
					//CTeach_UpdateRange(7);
				}
				break;
			case 8:											//探针
				{
					CEdit_SetLen(2); 
					//CTeach_UpdateRange(2);
					
				}
				break;
			case 9:											//气缸
				{
					CEdit_SetLen(7);	
					//CTeach_UpdateRange(5);				
				}
				break;
			case 10:										//延时
				{
					CEdit_SetLen(4);	
					//CTeach_UpdateRange(6);			
				}
				break;
			
		}
	}
	else if(g_Ini.iAxisCount == 5)
	{
		switch(CList_CurCol())
		{
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
				{
					
					CEdit_SetLen(6); 
					//CTeach_UpdateRange(1);
				}
				break;
			case 4:											//送线
				{
					CEdit_SetLen(7); 
					//CTeach_UpdateRange(8);
				}
				break;
			case 8:											//速比
				{
					CEdit_SetLen(4); 
					//CTeach_UpdateRange(7);
				}
				break;
			case 9:											//探针
				{
					CEdit_SetLen(2); 
					//CTeach_UpdateRange(2);
					
				}
				break;
			case 10:											//气缸
				{
					CEdit_SetLen(7);	
					//CTeach_UpdateRange(5);				
				}
				break;
			
		}
	}
	else if(g_Ini.iAxisCount == 6)
	{
		switch(CList_CurCol())
		{
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 8:
				{
					
					CEdit_SetLen(6); 
					//CTeach_UpdateRange(1);
				}
				break;
			case 4:											//送线
				{
					CEdit_SetLen(7); 
					//CTeach_UpdateRange(8);
				}
				break;
			case 9:											//速比
				{
					CEdit_SetLen(4); 
					//CTeach_UpdateRange(7);
				}
				break;
			case 10:											//探针
				{
					CEdit_SetLen(2); 
					//CTeach_UpdateRange(2);
					
				}
				break;
			case 11:											//气缸
				{
					CEdit_SetLen(7);	
					//CTeach_UpdateRange(5);				
				}
				break;
		
			
		}
	}

/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/


	//Uart_Printf("CList_MaxCol() =%d,CList_CurCol() = %d\n",CList_MaxCol(),CList_CurCol());
	switch(g_Ini.iAxisCount)
	{
		case 2:
			if(CList_CurCol()>=CList_MaxCol()-4)
				CEdit_SetString( ptr->value[CList_CurCol()+3] );
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
			break;
			
		case 3:
			if(CList_CurCol()>=CList_MaxCol()-4)
				CEdit_SetString( ptr->value[CList_CurCol()+2] );
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
			break;
			
		case 4:
			if(CList_CurCol()>=CList_MaxCol()-4)
				CEdit_SetString( ptr->value[CList_CurCol()+1] );
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
			break;
			
		case 5:
			if(CList_CurCol()>=CList_MaxCol()-3)
				CEdit_SetString( ptr->value[CList_CurCol()+0] );
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
			break;
			
		case 6:
			CEdit_SetString( ptr->value[CList_CurCol()-1] );
			break;
			
			
		default:
			CEdit_SetString( ptr->value[CList_CurCol()-1] );
			
			break;
	}
	
	
	
	CEdit_SetFont(FONT16);
	CEdit_UpdateData( FALSE );
	CEdit_SetFirst(TRUE);
	
	
}




void	CTeach_UpdatePage( INT16S page )
{
	INT16S maxSize = Line_Data_Size-CList_StartRow();
	
	if(page > 0)	
	{
		if(maxSize <= ROWS) 
			return;
	}

	if(page < 0)
	{
		if(CList_StartRow()+page < 0)
		{
			if(CList_StartRow() ==0)
			{
				return;
			}
			
			CList_SetStartRow(0);
			CTeach_InvidateView(FALSE);
			return;
		}
	}
	
	/*{
		INT16S tmprow = CList_CurRow();
		
		if(tmprow < 0)
			tmprow = 0;
			
		if(tmprow > ROWS-1)
			tmprow = ROWS-1;
			
		CTeach_DrawItem(  CList_CurRow()+CList_StartRow(), tmprow ,Reset,g_TextColor6,g_nTableBk);
	}*/
	
	CList_SetStartRow(CList_StartRow()+ page);
	maxSize = Line_Data_Size-CList_StartRow();
	
	if( page > 0 )
	{
		if( CList_CurRow() > maxSize -1 )
			CList_SetCurRow( maxSize -1);
	}
	
	CTeach_InvidateView(FALSE);
}


//#pragma	warn -par
void	CTeach_DisplayLine( INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor)
{
	INT16S 		col;
	CRect 		rect;
	INT8S 		m_Buffer[0x12];
	COMMAND 	*ptr = Line_Data+data;
	INT32S 		textColor = qColor;
	
	for(col=0; col<CList_MaxCol(); col++)
	{
		if( col == 0 && ptr->value[0][0]!=NULL )
		{
			CList_GetItemRect( row, col, &rect );
			
			if (g_gridStyle)
			{
			
				CDC_PutColorBar2( rect,g_TextColor4,1);
				CDC_Rectangle2( rect,g_TableLine );
			}
			else
			{
				rect.right=rect.right-1;
				rect.bottom+=1;
				
				CDC_PutColorBar2( rect,g_TextColor4,1);
				
				rect.right=rect.right+1;
				
				CDC_Line(rect.left+1,rect.top,rect.left+1,rect.bottom,g_TableLine);
				CDC_Line(rect.left+2,rect.top,rect.left+2,rect.bottom,g_TableLine);	
				CDC_Line(rect.right-0,rect.top,rect.right-0,rect.bottom,g_TableLine);
				CDC_Line(rect.right-1,rect.top,rect.right-1,rect.bottom,g_TableLine);
			
				rect.bottom-=1;
			}
			
		
			if( CList_DataRow(row) >= CList_MaxData())
			{
				return;
			}
			
			sprintf(m_Buffer,"%03d",data+1);
			
			CDC_OutText_UTF(rect.left+6,rect.top+2,m_Buffer,g_TextColor2,g_TextColor4,MZK_24);
			
			continue;
		}
		else
		{
			if(col==0&&ptr->value[0][0]==NULL)
			{
				CList_GetItemRect( row, col, &rect );
				
				if(g_gridStyle)
				{
					CDC_PutColorBar2( rect,g_TextColor4,1);
					CDC_Rectangle2( rect,g_TableLine );	
				}
				else
				{				
					rect.right=rect.right-1;
					rect.bottom+=1;
					
					CDC_PutColorBar2( rect,g_TextColor4,1);
					
					rect.right=rect.right+1;
					
					//CDC_Line(rect.left+0,rect.top,rect.left+0,rect.bottom,g_TableLine);
					CDC_Line(rect.left+1,rect.top,rect.left+1,rect.bottom,g_TableLine);
					CDC_Line(rect.left+2,rect.top,rect.left+2,rect.bottom,g_TableLine);

					CDC_Line(rect.right-0,rect.top,rect.right-0,rect.bottom,g_TableLine);
					CDC_Line(rect.right-1,rect.top,rect.right-1,rect.bottom,g_TableLine);
					//CDC_Line(rect.right-2,rect.top,rect.right-2,rect.bottom,g_TableLine);
					
					rect.bottom-=1;
				}
					
				continue;
			}
		}
		
		
		switch( status )
		{
			case Reset:
				{
					CList_GetItemRect( row, col, &rect );
					
					//CDC_PutColorBar2(rect,bColor,1);
					//CDC_Rectangle3( rect, BLACKCOLOR);
					
					if (g_gridStyle)
					{
						CDC_PutColorBar2(rect,bColor,1);
						CDC_Rectangle2( rect, g_TableLine );
					}
					else
					{
						rect.right=rect.right-1;
						rect.left+=1;
						rect.bottom+=1;
						
						CDC_PutColorBar2(rect,bColor,1);
						
						rect.right=rect.right-1;
						
						//CDC_Line(rect.right+0,rect.top,rect.right+0,rect.bottom,g_TableLine);
						CDC_Line(rect.right+1,rect.top,rect.right+1,rect.bottom,g_TableLine);
						CDC_Line(rect.right+2,rect.top,rect.right+2,rect.bottom,g_TableLine);
						
						rect.right+=2;
						rect.left-=1;
						rect.bottom-=1;
					}
				
					if(CList_DataRow(row) >= CList_MaxData()) 
					{
						return;
					}
						
					
					
					CTeach_ViewConvert(row, col, m_Buffer);
					
					if(col==1)
						CDC_OutCenTextSzk24(rect,m_Buffer,textColor,bColor);
					else
						CDC_TextSelect_24(rect.left,rect.top,rect.right-rect.left-2,rect.bottom-rect.top,1,m_Buffer,textColor,bColor);
			
					memset(m_Buffer,NULL,0x12);
				}
				break;
				
			case Active:
				{
					CList_GetItemRect( row, col, &rect );								
			
					//CDC_Rectangle3( rect, BLACKCOLOR );
					//CDC_PutColorBar2(rect,BLUECOLOR,1);
					if (g_gridStyle)
					{
						CDC_Rectangle2( rect, g_TableLine );
					}
					else
					{
						rect.right=rect.right-2;	
						rect.left+=1;
						rect.bottom+=1;				
						
						//CDC_Line(rect.right+0,rect.top,rect.right+0,rect.bottom,g_TableLine);
						CDC_Line(rect.right+1,rect.top,rect.right+1,rect.bottom,g_TableLine);
						CDC_Line(rect.right+2,rect.top,rect.right+2,rect.bottom,g_TableLine);
					
						rect.right+=2;
						rect.left-=1;
						rect.bottom-=1;
					}
							
					if( CList_DataRow(row) >= CList_MaxData() ) 
					{
						break;
					}
						
					CTeach_ViewConvert( row, col, m_Buffer );	
					
					if(col==1)
						CDC_OutCenTextSzk24(rect,m_Buffer,textColor,bColor);
					else
						CDC_TextSelect_24(rect.left,rect.top,rect.right-rect.left-2,rect.bottom-rect.top,1,m_Buffer,textColor,bColor);
					
					if(!g_bRefresh)
					{
						rect.left=3;
						rect.right=getmaxx()-3;
						
						CDC_Rectangle2( rect ,PINKCOLOR);
						
						rect.left=4;
						rect.right=getmaxx()-4;
						rect.top=rect.top+1;
						rect.bottom=rect.bottom-1;
						
						CDC_Rectangle2( rect ,PINKCOLOR);
					}
					
					memset(m_Buffer,NULL,0x12);
				
				} 
				break;
				
			case MoveActive:
				{
					CList_GetItemRect( row, col, &rect );
					//CDC_PutColorBar2( rect,bColor,1);
					if (g_gridStyle)
					{
						CDC_PutColorBar2(rect,qColor,1);
						CDC_Rectangle2( rect, bColor );
					}
					else
					{
						rect.right=rect.right-1;
						rect.left+=1;
						rect.bottom+=1;
						
						CDC_PutColorBar2(rect,qColor,1);
						
						rect.right=rect.right-1;
						//CDC_Line(rect.right+0,rect.top,rect.right+0,rect.bottom,bColor);
						CDC_Line(rect.right+1,rect.top,rect.right+1,rect.bottom,bColor);
						CDC_Line(rect.right+2,rect.top,rect.right+2,rect.bottom,bColor);
						
						rect.right+=1;
						rect.left-=1;
						rect.bottom-=1;
					}
					
					if(CList_DataRow(row) >= CList_MaxData()) 
						break;
							
					CTeach_ViewConvert( row, col, m_Buffer );	
					
					if(col==1)
						CDC_OutCenTextSzk24(rect,m_Buffer,bColor,textColor);
					else			
						CDC_TextSelect_24(rect.left,rect.top,rect.right-rect.left-2,rect.bottom-rect.top,1,m_Buffer,bColor,textColor);
								
					if(!g_bRefresh)
					{
						rect.left=3;
						rect.right=getmaxx()-3;
						CDC_Rectangle2( rect ,PINKCOLOR);
						rect.left=4;
						rect.right=getmaxx()-4;
						rect.top=rect.top+1;
						rect.bottom=rect.bottom-1;
						CDC_Rectangle2( rect ,PINKCOLOR);
					}
						
					memset(m_Buffer,NULL,0x12);
				} 
				break;
				
			case Draw:
				{
					CList_GetItemRect( row, col, &rect );
					
					//CDC_Rectangle3( rect,g_nTableBk);
					if (g_gridStyle)
					{
						CDC_Rectangle2( rect, g_TableLine );
					}
					else
					{
						rect.right=rect.right-2;	
						rect.left+=1;
						rect.bottom+=1;	
									
						//CDC_Line(rect.right+0,rect.top,rect.right+0,rect.bottom,g_TableLine);
						CDC_Line(rect.right+1,rect.top,rect.right+1,rect.bottom,g_TableLine);
						CDC_Line(rect.right+2,rect.top,rect.right+2,rect.bottom,g_TableLine);
						
						rect.right+=2;
						rect.left-=1;
						rect.bottom-=1;
					}
					
					if(CList_DataRow(row) >= CList_MaxData()) 
						break;
										
					CTeach_ViewConvert( row, col, m_Buffer );


					if(col==1)
						CDC_OutCenTextSzk24(rect,m_Buffer,textColor,bColor);
					else
						CDC_TextSelect_24(rect.left,rect.top,rect.right-rect.left-2,rect.bottom-rect.top,1,m_Buffer,textColor,bColor);
					
					memset(m_Buffer,NULL,0x12);
				}
				break;
				
			default:
				break;
		}
		
	}
}


void	CTeach_DrawItem(INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor)
{
	CTeach_DisplayLine(data,row,status,qColor,bColor);
}


void	CTeach_ViewConvert(INT16S row,INT16S col,INT8S *buffer)
{
	COMMAND *ptr;

	ptr = Line_Data+CList_StartRow()+row;

	if(CList_StartRow()+row>=Line_Data_Size)
	{
		strcpy(buffer,"\0");
		return;
	}
	
/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/


	switch(g_Ini.iAxisCount)
	{
		case 2:
			if(col>=CList_MaxCol()-4)
				strcpy(buffer,ptr->value[col+3]);
			else
				strcpy(buffer,ptr->value[col-1]);
			break;
			
		case 3:
			if(col>=CList_MaxCol()-4)
				strcpy(buffer,ptr->value[col+2]);
			else
				strcpy(buffer,ptr->value[col-1]);
			break;
			
		case 4:
			
			if(col>=CList_MaxCol()-4)
				strcpy(buffer,ptr->value[col+1]);
			else
				strcpy(buffer,ptr->value[col-1]);
				
			break;
			
			
			
		case 5:
			if(col>=CList_MaxCol()-3)
				strcpy(buffer,ptr->value[col+0]);
			else
				strcpy(buffer,ptr->value[col-1]);
				
			break;
			
		case 6:
			
			strcpy(buffer,ptr->value[col-1]);
			break;
			
			
		default:
			strcpy(buffer,ptr->value[col-1]);
				
			break;
	}
}


void	CTeach_InitData()
{
	
	switch(Line_Data_Size)
	{
		case 0:
			{
				DataPut(0,0,"S");
				DataPut(0,1,"30");
				DataPut(0,3,"10");
				
				if(g_Ini.iAxisCount>2)
					DataPut(0,4,"30");
				if(g_Ini.iAxisCount>3)
					DataPut(0,5,"30");
				if(g_Ini.iAxisCount>4)
					DataPut(0,6,"30");
				if(g_Ini.iAxisCount>5)
					DataPut(0,7,"30");
			}
		
		case 1:
			{
				DataCls(1);
				DataPut(1,0,"M");
				
				Line_Data_Size = 2;
				CList_SetStartRow(0);
				CList_SetCurRow(1);
				CList_SetCurCol(2);
			}
			break;
		default:
			break;
	}
}


void	CTeach_AddData()
{
	INT16S size;
	
	if( Line_Data_Size >= MAXLINE) 
		return;
	
	g_bModify = TRUE;
	g_bModifySave = FALSE;
	size = Line_Data_Size;
	
	DataPut(size,0,"M");
	
	Line_Data_Size ++;
}


void	CTeach_Insert()
{
	INT16S 	i=0,index=0;
	INT8S 	buf[9]="\0";
	
	COMMAND *ptr = Line_Data;
	
	if( Line_Data_Size >= MAXLINE) 
	{
		OSQPost(MsgQueue,g_bEnglish?"Can not Insert more Data!":"行数以满，无法插入!");
		return;
	}
	
	if( Line_Data_Size == 1 )
	{ 
		CTeach_AddData(); 
		return; 
	}
	
	index = CList_StartRow() + CList_CurRow();
	
	if(index==0)    //不可在第一行插入数据
		return;
	
	g_bModify = TRUE;
	g_bModifySave=FALSE;
	
	for( i=Line_Data_Size; i>index; i--)
		*(Line_Data+i) = *(Line_Data+i-1);
		
	DataCls(index);
	DataPut(index,0,"M");
	//DataPut(index,0,"");

	DisplayXYZ(TRUE);
	
	DataPut(0,0,"S");

	Line_Data_Size ++;
	
	for(i=0;i<Line_Data_Size;i++)
	{
		ptr=(Line_Data+i);
		
		if( ptr->value[0][0]=='J'|| ptr->value[0][0]=='G' )
		{
			INT16S temp= stringtoi( (INT8U *)ptr->value[1] );
			
			if(temp>index)
			{
				temp++;
				ltoa((INT32U)temp,(INT8U *)buf);
				strcpy(ptr->value[1],buf);
			}
		}
		
	}
	
}


void	CTeach_Delete()
{
	INT16S 		i=0,index=0;
	INT8S 		buf[9]="\0";
	COMMAND 	*ptr;
	
	if(index>=Line_Data_Size)
	{
		return;
	}
	
	if( Line_Data_Size < 1 ) 
		return;
	
	index = CList_StartRow() + CList_CurRow();
	
	
	if( index ==0) 
		return;
	
	g_bModify = TRUE;
	g_bModifySave = FALSE;
	ptr = Line_Data+index;
	
	for(i=index; i<Line_Data_Size; i++,ptr++)
		*ptr = *(ptr+1);
	
	DataCls(i);
	Line_Data_Size --;
	
	for(i=0; i<Line_Data_Size; i++)												//有J或G指令则其对应的行号需发生变化以保证指向的位置正确
	{
		ptr=(Line_Data+i);
	
		if( ptr->value[0][0]=='J'|| ptr->value[0][0]=='G' )
		{
			INT16S temp= stringtoi((INT8U *)ptr->value[1]);
		
			if(temp>index)
			{
				temp--;
				ltoa((INT32U)temp,(INT8U *)buf);
				strcpy(ptr->value[1],buf);
			}
		}
	}
}


void	CTeach_DeleteAll()
{
	INT16S i=0;
	
	g_bModify = TRUE;
	g_bModifySave = FALSE;
	
	if( Line_Data_Size )
	{
		for(i=0; i<Line_Data_Size; i++)
			DataCls(i);
	
		Line_Data_Size = 0;
	}
	
	CTeach_InitData();
}


void  Teach_Task(void *data)
{
	INT16U key=0,tkey=0;
	
	data = data;

			
	if(!g_bSysParaCorrect)
	{
	
		g_iCurEditNo=1;

		CSysPara_Init(0,0,798,599);
	
		CSysPara_DoModal();
		CSysPara_Destroy();
		
		g_iCurEditNo=1;
		g_iParameterEdit = 0;
		
		g_bEnglish=g_Ini.iLanguage;

	}

	CTeach_Init();
	CTeach_Create();	

	TaskStartCreateTasks();														//创建其它任务

	SuspendTasks(1);
	SuspendTasks(2);
	//SuspendTasks(3);
	SuspendTasks(4);
	
	
	OSTimeDly(50);	

	RunStatusOutCtr(0);

	for(;;)
	{
		
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{

			CTeach_OnKey(key,tkey);
		}
		
		CTeach_LoopCheck();

		OSTimeDly(3);
	}
}


void	CTeach_LoopCheck()
{
	INT32S pos=0;
	INT16S	i;
	
	
	//********检测急停按钮状态**************//
	{
		static INT16S stopio=-1,stopio1 = -1;
		INT16S stopbit,stopbit1;	
		
		stopbit=ReadBit(STOPIO);
		stopbit1=ReadBit(STOPIO2);
		
		if(stopbit!=stopio || stopbit1!=stopio1)
		{
			stopio=stopbit;
			stopio1=stopbit1;
			//Uart_Printf("stopio %d stopio1 %d...\n",stopio,stopio1);
			if( CheckWarning() || CheckLimit(TRUE,TRUE,0,0))
			{
				if(/*g_bWheel ||*/ g_bMode)
				{
				//	g_bWheel = FALSE;					
				//	LedOut(WHEELLED,0);	
					g_bMode=FALSE;
					LedOut(TESTLED,0);
				}
				
				return;
			}
			
		
		//	if(stopbit==0 || stopbit1==0)
			if(stopbit==g_Ini.StopLevel || stopbit1==0)// 改为电平可配置
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
				g_iErrorNo = 0;
				
				g_bMode=FALSE;
				LedOut(TESTLED,0);
				
				OSQPost(MsgQueue,g_bEnglish?"Stop state,Please raise stopIO!":"停机复位状态!");
						
			//	g_bEMSTOP=TRUE;	
			}
			else
			{
				OSQPost(MsgQueue," ");
			}
			
			if(g_bMode==TRUE)//急停锁死如果按测试，锁开后，测试状态要去除，不然会直接走起来
			{
				g_bMode=FALSE;
				LedOut(TESTLED,0);
			}
			
			SetMM(7,0,FALSE);	
			SetMM(8,0,FALSE);
			
			return;
		}
		
	//	if(stopbit==0 || stopbit1==0)
		if(stopbit==g_Ini.StopLevel || stopbit1==0)//改为电平可配置
		{
			OSTimeDly(5);   
			return;
		}
	}
	
	//********************手持盒**********************************//
	
	if(g_bHandset)
	{
		{
			static INT16S AxisStatus[6]={-1,-1,-1,-1,-1,-1};			
			INT16S AxisIO[6]={TXIO,TYIO,TZIO,TAIO/*,TBIO,TCIO*/};
			
			for(i=0; i<g_Ini.iAxisCount; i++)
			{
				INT16S bit=ReadBit(AxisIO[i]);
								
				if(bit==0 && AxisStatus[i]!=0)
				{					
					switch(i)
					{
						case 0:
							if(CList_CurCol()!=2 && CList_CurCol()!=3)
							{
								CTeach_Update(0,2-CList_CurCol());	
								
							}
							break;
						case 1:
							if(CList_CurCol()!=4)
							{
								CTeach_Update(0,4-CList_CurCol());
								
							}
							break;
						case 2:
							if(CList_CurCol()!=5)
								CTeach_Update(0,5-CList_CurCol());				
							break;
							
						case 3:
							if(CList_CurCol()!=6 && g_Ini.iAxisCount>3)
								CTeach_Update(0,6-CList_CurCol());
							break;		
							
						case 4:
							if(CList_CurCol()!=7 && g_Ini.iAxisCount>4)
								CTeach_Update(0,7-CList_CurCol());
							break;			
						
						case 5:
							if(CList_CurCol()!=8 && g_Ini.iAxisCount>5)
								CTeach_Update(0,8-CList_CurCol());
							break;				
					}		
					
					AxisStatus[i]=bit;		//add by xxl 20141127	
					UpdateCurAxis(); 		//add by xxl 20141127			
					break;										
				}
				
				AxisStatus[i]=bit;						
			}			
		}
		
		{
			static INT16S SpeedStatus[3]={-1,-1,-1};
			INT16S bit[3] = {-1,-1,-1};
			
			INT16S SpeedIO[3]={LOWSPEEDIO,MIDDLESPEEDIO,HIGHSPEEDIO};
			
			for(i=0; i<3; i++)
			{
				 bit[i] = ReadBit(SpeedIO[i]);
				
				if(bit[i] != SpeedStatus[i])
				{
				
					SpeedStatus[i]=bit[i];
					
					if(SpeedStatus[i] == 0)
					{
					
						//SpeedStatus[i]=bit;
						
						g_iSpeedBeilv=i+1;
											
						switch(g_iSpeedBeilv)
						{
							case 1:
								//CDC_OutText24(735,445,g_bEnglish?" Low":"低速",g_TextColor2,g_nTechfrmBk);
								CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Low":"低速",g_TextColor5,g_nBk,KZK_22);
								break;
								
							case 2:
								CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Mid":"中速",g_TextColor5,g_nBk,KZK_22);
								break;
								
							case 3:
								CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Hgh":"高速",g_TextColor5,g_nBk,KZK_22);
								break;
								
							default:
								break;
						}
						break;
					}
				}
			}
			
		}
	
	}
		
	//*********************伺服报警状态检测************************//	
	if(g_bRelease == TRUE)
	{
		static INT16S XYZStatus[6]={0,0,0,0,0,0};
		INT16S XYZIO[6]={XSERVOWARNING,YSERVOWARNING,ZSERVOWARNING,USERVOWARNING,VSERVOWARNING,WSERVOWARNING};	
		
		for( i=0; i<g_Ini.iAxisCount; i++)
		{
			
			char str[0x40] = "";//"凸轮伺服故障,请断电检查!";
			
			INT16S bit = ReadBit(XYZIO[i]);
			
			
			{
				XYZStatus[i] = bit;
				
				if(g_Ini.iAxisSwitch[i]==0)continue;
				
				if( XYZStatus[i]== g_Ini.iServoLogic[i] )
				{					
					g_bFirstResetX=FALSE;
					g_bFirstResetY=FALSE;
					g_bFirstResetZ=FALSE;
					g_bFirstResetU=FALSE;
					g_bFirstResetV=FALSE;
					g_bFirstResetW=FALSE;
					
					
					strcpy(str,axisMsg(1+i,g_bEnglish?" Servo Warning! Please Power off Check!":"伺服故障,请断电检查!",FALSE));
					
				
					if(g_bDispServoWaring)
					{					
						sprintf(g_pszMsgBuf,"%s",str);
						OSQPost(MsgQueue,g_pszMsgBuf);	
						g_bDispServoWaring=FALSE;
						
						/*
						if(g_Ini.iWarningOut==1)
						{
							if(GetOut(O_WARNING)==0)
								WriteBit(O_WARNING,OPEN);
						}*/
					}
					
					
					
					return;
				}
			}
		}
	}
	
	
	//************************断线、缠线、跑线检测***********************//
	{
		INT16S bit;
		static INT16S OtherIOStatus[4]={0,1,1,0};
		INT16S OtherIO[2]={DUANXIAN,SONGXIANIOCONST};
		
		{
			bit = ReadBit(OtherIO[0]);

			if(g_bRelease)
			{
				if(bit!=OtherIOStatus[0])
				{
					OtherIOStatus[0]=bit;
					if(bit==g_Ini.iDuanXianLogic)
					{
						OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning":"断线报警!");
						
						/*
						if(g_Ini.iWarningOut==1)
						{
							if(GetOut(O_WARNING)==0)
								WriteBit(O_WARNING,OPEN);
						}*/
					}
					else
					{
						OSQPost(MsgQueue,g_bEnglish?" ":" ");
						/*
						if(g_Ini.iWarningOut==1)
						{						
							WriteBit(O_WARNING,CLOSE);
						}*/
					}
				}
			}

			bit=ReadBit(OtherIO[1]);
			
			if(bit!=OtherIOStatus[1])
			{
				OtherIOStatus[1]=bit;
				if(bit==0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning":"缠线报警!");
					
				/*	if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}*/
				}
				else
				{
					OSQPost(MsgQueue,g_bEnglish?" ":" ");
				
				/*	if(g_Ini.iWarningOut==1)
					{						
						WriteBit(O_WARNING,CLOSE);
					}*/
				}
			}
			
			if(g_Ini.iPaoXian)
			{
				bit=ReadBit(PAOXIANDI);
				
				if(bit!=OtherIOStatus[2])
				{
					OtherIOStatus[2]=bit;
					
					if(bit==0)
					{
						OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning":"跑线报警!");
						
						/*
						if(g_Ini.iWarningOut==1)
						{
							if(GetOut(O_WARNING)==0)
								WriteBit(O_WARNING,OPEN);
						}	*/		
					}
					else
					{
						OSQPost(MsgQueue,g_bEnglish?" ":" ");
						/*
						if(g_Ini.iWarningOut==1)
						{						
							WriteBit(O_WARNING,CLOSE);
						}*/
					}
				}
			}
									//手动测试下可以不用检测安全门信号
			if(g_Ini.iSafeDoor && !((g_Ini.iHandStatus||g_bHandset) && g_bMode && !g_bStepIO))
			{
				bit=ReadBit(SAFEDOOR);
				if(bit!=OtherIOStatus[3])
				{
					OtherIOStatus[3]=bit;
					if(bit==1)
					{
						OSQPost(MsgQueue,g_bEnglish?"Safe Door Waring":"安全门打开状态!!!");
						/*
						if(g_Ini.iWarningOut==1)
						{
							if(GetOut(O_WARNING)==0)
								WriteBit(O_WARNING,OPEN);
						}*/
					}
					else
					{
						OSQPost(MsgQueue,g_bEnglish?" ":" ");
						/*
						if(g_Ini.iWarningOut==1)
						{
							WriteBit(O_WARNING,CLOSE);
						}*/
					}
				}
			}
		}
	}


	//*******************外部启动加工***********************//	
	
	if(!g_bPressRunIO && RealIO(RUNID,0,3))                                                         //外部启动
	{
		 g_bPressRunIO=TRUE;
		 CTeach_OnKey(TRUN,0);
	}
	
	if(g_bPressRunIO &&  RealIO(RUNID,1,3) )
	{
		g_bPressRunIO=FALSE;
	}
	
	//*******************外部单条加工***********************//	

	
	if(!g_bPressStopIO && RealIO(STOPID1,0,3) )
	{
		 g_bPressStopIO=TRUE;
   	  	 CTeach_OnKey(TSTOP,0);	
	}
	
	if(g_bPressStopIO && RealIO(STOPID1,1,3) )
	{
		g_bPressStopIO=FALSE;
	}
		
	//**********************测试加工*********************//
	if(g_bMode)												
	{
		INT16S temp=-1;		
		
		temp=CTelCom_GetChar(1);
		
		if(temp!=2000)
			temp=temp/2;
			
		if(temp>2&&temp<127)
		{
			if(g_iCurEditNo!=9) 
			{
				UpdateFileDestProbe(g_iCurEditNo+1,1,0);
				g_iCurEditNo=9;
			}
			
			if(g_bRelease)
			{
			
				if( CheckWarning() || CheckLimit(TRUE,TRUE,0,0))
				{
					/*
					if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}*/
					
				//	g_bWheel = FALSE;					
				//	LedOut(WHEELLED,0);		
								
					g_bMode=FALSE;
					LedOut(TESTLED,0);
					
					return ;
				}
			
			}
			
			
			if(g_lStepNode==0&&g_bModify)
			{
				INT32S t1,t2;
				
				OSSemPost(UnpackDataSem);
				
				g_bUnpackCorrect = FALSE;
				
				t1=OSTimeGet();
				
				while(TRUE)
				{
					OSTimeDly(2);
					
					if(g_bUnpackCorrect)
						break;
						
					t2=OSTimeGet();
					
					if((t2-t1)>200||(t2-t1<-200))
					{
						g_bMode=FALSE;
						LedOut(TESTLED,0);
						return;
					}
				}				
			}
			
		//	g_iADValue=temp;
			g_iADValue=-2;

			g_bStepIO = TRUE;
			g_bDispRunLine=TRUE;
		//	g_bAutoRunMode = FALSE;
			g_bAutoRunMode = TRUE;
			g_bTestStart=TRUE;
			g_bHandRun = FALSE;
			
			RunAllData();
			
			g_bMode=FALSE;
			LedOut(TESTLED,0);
					
			g_iADValue=-2;
			return;
		}
	}
	
	
	//********************手轮加工***************************//			
	//Uart_Printf("g_Ini.iHandStatus = %d\n",g_Ini.iHandStatus);
	if((g_Ini.iHandStatus||g_bHandset) && g_bMode)
	{
		if(g_bHandset)
			pos = GetPulse(7,FALSE);
		else
			pos= GetPulse(8,FALSE);					//面板上的手轮
		
		
		if (g_bHandset)
		{
			if (ReadBit(TXIO)&&ReadBit(TYIO)&&ReadBit(TZIO)&&ReadBit(TAIO)/*&&ReadBit(TBIO)&&ReadBit(TCIO)*/)
			{
				SetMM(7,0,FALSE);	
				SetMM(8,0,FALSE);				
				pos=0;
			}
		}
		
		if(labs(pos)>1 )
		{						
			if(g_bRelease)
			{
				//手动测试下可以不用检测安全门信号
				if( (CheckWarning() && 5!=CheckWarning())|| CheckLimit(TRUE,TRUE,0,0))
				{
				/*
					if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}*/
					
					//g_bWheel = FALSE;					
					//LedOut(WHEELLED,0);		
					
					
								
					g_bMode=FALSE;
					LedOut(TESTLED,0);
					
					if(g_Ini.iHandStatus != 0)
					{
						g_Ini.iHandStatus=0;
						LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
						CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
					}
					
					if(g_bHandset == TRUE)
					{
						g_bHandset=FALSE;
						LedOut(BOXLED,g_bHandset?1:0);
						CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOff  ":"手盒关",g_TextColor5,g_nBk,KZK_22);
					}
					
					
					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
					
					return ;
				}
			}
			
			if(g_iCurEditNo!=9)	
			{
				OSQPost(MsgQueue,g_bEnglish?"Can't jog handwheel at cur state!":"参数编辑状态不可移动手轮!!");
				
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);

				return;
			}	
			
			//Uart_Printf("pos=%d,pos1=%d\n",pos,pos1);
			
			if(pos>0 )
			{
				if(g_lStepNode==0 && g_bModify)
				{
					INT32S t1,t2;
					
					OSSemPost(UnpackDataSem);
					
					g_bUnpackCorrect= FALSE;
					
					t1 = OSTimeGet();
					
					while(TRUE)
					{
						OSTimeDly(2);
					
						if(g_bUnpackCorrect)
							break;
						
						t2 = OSTimeGet();
						
						if((t2-t1)>100 || (t2-t1<-100))
							return;
					}						
				}
				
				if(!g_bUnpackCorrect)
				{
					OSQPost(MsgQueue,g_bEnglish?"Data Exist Error,Can't Run!":"编程数据错误，不能执行手轮加工操作!");

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
				}
				else
				{
					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					g_lHandRun     = -1;
				//	g_bStepIO      = TRUE;
					g_bStepIO      = FALSE;

					g_bDispRunLine = TRUE;
					
				//	g_bAutoRunMode = FALSE;
					g_bAutoRunMode = TRUE;

					g_bHandRun     = TRUE;
					g_bTestStart   = TRUE;
					g_fSpeedBi     = 0.2;
					
					//Uart_Printf("test3\n");
					RunAllData();
					
					g_bHandRun = FALSE;
					g_bStepIO  = FALSE;
				}
			}
			else
			{
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);
			}
		}

	}
	
	
	
// ****************手轮手动功能*************************************//
	
	if((g_Ini.iHandStatus || g_bHandset) && !g_bMode )									
	{
		if(g_bHandset)
			pos = GetPulse(7,FALSE);
		else
			pos= GetPulse(8,FALSE);
		
		if(labs(pos)>1 )
		{						
			if(g_bRelease)
			{
				if( CheckWarning())
				{
				/*
					if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}*/
					
					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
					
					
					if(g_Ini.iHandStatus != 0)
					{
						g_Ini.iHandStatus=0;
						LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
						CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
					}
					
					if(g_bHandset == TRUE)
					{
						g_bHandset=FALSE;
						LedOut(BOXLED,g_bHandset?1:0);
						CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOff  ":"手盒关",g_TextColor5,g_nBk,KZK_22);
					}
					
					return ;
				}
		
			}
			
			if(g_iCurEditNo!=9 )//&& g_iCurEditNo != 6 && g_iCurEditNo != 5)	//20140220在寸动速度编辑下可以手轮摇动
			{
				OSQPost(MsgQueue,g_bEnglish?"Can't Jog Handwheel at Para-Edit State!":"参数编辑状态不可移动手轮!!");
				
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);

				return;
			}
					
			if(g_iWorkNumber >0 && g_Ini.iAxisSwitch[g_iWorkNumber-1]==0)						//20141128在寸动速度编辑下可以手轮摇动
			{
				OSQPost(MsgQueue,g_bEnglish?"Current Axis is Turned off,Can't Jog Handwheel!":"轴关闭状态不可移动手轮!!");
				
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);

				return;
			}	
			
			if(g_iWorkNumber == 2 && g_Ini.iYDaoXian == 0 && pos < 0)						//
			{
				OSQPost(MsgQueue,g_bEnglish?"Can't Hand backY!":"不可倒线!");
				
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);

				return;
			}	
													
			{
				if(g_iWorkNumber<g_Ini.iAxisCount+1 && g_iWorkNumber!=0)//自动模式下手轮移动各个轴
				{
					FP32 	mv = -10000.0;
					FP32 	pL[4] = {0.5,1.0,2.0,3.0};					//1号值为没带手盒，2号值为低速档，3号值为中速档，4号值为高速档。
					INT32S  lTempPos =0;
					FP32	speed = 1.0;
					INT32S  Ypos = 0,Apos = 0;
					//INT32S  pp;
					

					
					if(g_lStepNode!=0)
					{
						g_lStepNode = 0;
						SetMM(2,0,TRUE);
					}
					
					//OSQPost(MsgQueue," ");
										
					g_lHandRun = OSTimeGet();
					
					if(g_iSpeedBeilv==3)
						pL[2] = g_Ini.iStepRate[g_iWorkNumber-1];
					
					if(pL[2]>3.5)
						pL[2] = 3.5;
					
					speed = pL[g_iSpeedBeilv-1]*g_Ini.lHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber);
					
					//Uart_Printf("g_iWorkNumber=%d,g_Ini.iAxisCount=%d\n",g_iWorkNumber,g_Ini.iAxisCount);
				
					switch(g_iWorkNumber)
					{
						case 1:
							g_iMoveAxis  = 1;
							g_fSpeedBi   = 0.05 ;
							g_bHandMove  = TRUE;
							g_iWorkStatus= NORUN;
							
							//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
							{										
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
								{
									SetMM(8,0,FALSE);
									SetMM(7,0,FALSE);

									g_iMoveAxis=0;
									g_bHandMove=FALSE;
									
									return;	
								}
								
								OSQPost(MsgQueue," ");	
							}
							
							lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];//以下两行函数在多圈手摇时，不用多圈回摇。
							SetPulse(g_iWorkNumber, lTempPos,TRUE);
							
							ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);

							while (1)
							{
								if(IsFree(g_iWorkNumber)) 
									break;
								
								if(PulseLagCheck(g_iWorkNumber))
								{
									Stop1(g_iWorkNumber);
									g_Ini.iHandStatus = 0;
									LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
									CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									Stop1(g_iWorkNumber);
									
								OSTimeDly(1);
							}

							mv = GetAngle(g_iWorkNumber,TRUE)*10;
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
							break;

						case 2:
							if(g_Ini.iAxisSwitch[1]==0)
								break;
								
							g_iMoveAxis  = 2;
							g_iWorkStatus= NORUN;
							g_fSpeedBi   = 0.05 ;
							g_bHandMove  = TRUE;
							
							
							if(g_Ini.iAxisRunMode[1]==3)						//送线模式								
							{
								if(g_Ini.iYDaoXian)
								{	
									ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,speed);

									while (1)
									{
										if (IsFree(g_iWorkNumber)) 
											break;
										
										if(PulseLagCheck(g_iWorkNumber))
										{
											Stop1(g_iWorkNumber);
											g_Ini.iHandStatus = 0;
											LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
											CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
											OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
											break;	
										}
										
										OSTimeDly(1);
									}
								}
								else
								{			
									if(pos>0 )
									{	
										ShortlyPmove(g_iWorkNumber,(pos>0 )?10000000L:-10000000L,speed);
			
										while (1)
										{
											if (IsFree(g_iWorkNumber)) 
												break;
											
											if(PulseLagCheck(g_iWorkNumber))
											{
												Stop1(g_iWorkNumber);
												g_Ini.iHandStatus = 0;
												LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
												CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
												OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
												break;	
											}
											
											OSTimeDly(1);
										}
									}
									else
									{									
										SetMM(7,0,FALSE);
										SetMM(8,0,FALSE);
									}
								}
								
								mv=GetMM(g_iMoveAxis,TRUE)*100;
							}
							else
							{
								//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
								{										
									if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									{
										SetMM(8,0,FALSE);
										SetMM(7,0,FALSE);

										g_iMoveAxis=0;
										g_bHandMove=FALSE;
										
										return;	
									}
									
									OSQPost(MsgQueue," ");	
								}
									
								if(g_Ini.iAxisRunMode[1] == 0)
								{
									lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
									SetPulse(g_iWorkNumber, lTempPos,TRUE);
								}
								
								
								ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);
					
								while (1)
								{
									if (IsFree(g_iWorkNumber)) 
										break;
										
									if(PulseLagCheck(g_iWorkNumber))
									{
										Stop1(g_iWorkNumber);
										g_Ini.iHandStatus = 0;
										LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
										CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
										OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
										break;	
									}
									
									if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
										Stop1(g_iWorkNumber);
										
									OSTimeDly(1);
								}
								
								if (g_Ini.iAxisRunMode[1]==0)
								{
									mv = GetAngle(2,TRUE)*10;
								}
								else if (g_Ini.iAxisRunMode[1]==1)
								{
									INT16S  tempi1;
									FP32 	tempi2;
									FP32	y;
									
									y = GetMM(2,TRUE);
									tempi1 = y/360;
									tempi2 = y-tempi1*360.0;									
									mv = tempi1+tempi2/1000.0;
								}
								else if (g_Ini.iAxisRunMode[1]==2)
								{
								
									mv = GetMM(2,TRUE)*100;
								}
							}	
							
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
								
							
							
							break;
								
				
							
							
						case 3:
							if(g_Ini.iAxisCount<3)
								break;
						
							if(g_Ini.iAxisSwitch[2]==0)
							 	break;
			
							g_iMoveAxis  = 3;
							g_iWorkStatus= NORUN;
							g_fSpeedBi   = 0.05 ;
							g_bHandMove  = TRUE;
							
							
							//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
							{										
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
								{
									SetMM(8,0,FALSE);
									SetMM(7,0,FALSE);

									g_iMoveAxis=0;
									g_bHandMove=FALSE;
									
									return;	
								}
								
								OSQPost(MsgQueue," ");	
							}
								
							if(g_Ini.iAxisRunMode[2] == 0)
							{
								lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
								SetPulse(g_iWorkNumber, lTempPos,TRUE);
							}
							
							
							ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);
				
							while (1)
							{
								if (IsFree(g_iWorkNumber)) 
									break;
									
								if(PulseLagCheck(g_iWorkNumber))
								{
									Stop1(g_iWorkNumber);
									g_Ini.iHandStatus = 0;
									LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
									CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									Stop1(g_iWorkNumber);
									
								OSTimeDly(1);
							}
							
							if (g_Ini.iAxisRunMode[2]==0)
							{
								mv = GetAngle(3,TRUE)*10;
							}
							else if (g_Ini.iAxisRunMode[2]==1)
							{
								INT16S  tempi1;
								FP32 	tempi2;
								FP32	y;
								
								y = GetMM(3,TRUE);
								tempi1 = y/360;
								tempi2 = y-tempi1*360.0;									
								mv = tempi1+tempi2/1000.0;
							}
							else if (g_Ini.iAxisRunMode[2]==2)
							{
							
								mv = GetMM(3,TRUE)*100;
							}
							
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
							break;
						
						case 4:
							if(g_Ini.iAxisCount<4)
								break;
							
							if(g_Ini.iAxisSwitch[3]==0)
							 	break;
			
							g_iMoveAxis   = 4;
							g_iWorkStatus = NORUN;
							g_fSpeedBi    = 0.05 ;
							g_bHandMove   = TRUE;
							
							//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
							{										
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
								{
									SetMM(8,0,FALSE);
									SetMM(7,0,FALSE);

									g_iMoveAxis=0;
									g_bHandMove=FALSE;
									
									return;	
								}
								
								OSQPost(MsgQueue," ");	
							}
							
							
							if(g_Ini.iAxisRunMode[3] == 0)
							{
								lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
								SetPulse(g_iWorkNumber, lTempPos,TRUE);
							}
							
							Apos = (pos>0 )?10000000L:-10000000L;
							

							ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);
								
							while (1)
							{
								if (IsFree(g_iWorkNumber)) 
									break;
								
								if(PulseLagCheck(g_iWorkNumber))
								{
									Stop1(g_iWorkNumber);
									g_Ini.iHandStatus = 0;
									LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
									CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									Stop1(g_iWorkNumber);
								
								OSTimeDly(1);
							}
							
							if (g_Ini.iAxisRunMode[3]==0)
							{
								mv = GetAngle(4,TRUE)*10;
							}
							else if (g_Ini.iAxisRunMode[3]==1)
							{
								INT16S  tempi1;
								FP32 	tempi2;
								FP32	y;
								
								y = GetMM(4,TRUE);
								tempi1 = y/360;
								tempi2 = y-tempi1*360.0;									
								mv = tempi1+tempi2/1000.0;
							}
							else  if (g_Ini.iAxisRunMode[3]==2)
							{
							
								mv = GetMM(4,TRUE)*100;
							}
							
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
							break;
							
						case 5:
							if(g_Ini.iAxisCount<5)
								break;
							
							if(g_Ini.iAxisSwitch[4]==0)
							 	break;
			
							g_iMoveAxis   = 5;
							g_iWorkStatus = NORUN;
							g_fSpeedBi    = 0.05 ;
							g_bHandMove   = TRUE;
							
							
							//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
							{										
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
								{
									SetMM(8,0,FALSE);
									SetMM(7,0,FALSE);

									g_iMoveAxis=0;
									g_bHandMove=FALSE;
									
									return;	
								}
								
								OSQPost(MsgQueue," ");	
							}
							
							
							if(g_Ini.iAxisRunMode[4] == 0)
							{
								lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
								SetPulse(g_iWorkNumber, lTempPos,TRUE);
							}
							
							
							ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);
								
							while (1)
							{
								if (IsFree(g_iWorkNumber)) 
									break;
								
								if(PulseLagCheck(g_iWorkNumber))
								{
									Stop1(g_iWorkNumber);
									g_Ini.iHandStatus = 0;
									LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
									CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									Stop1(g_iWorkNumber);
									
								OSTimeDly(1);
							}
							
							if (g_Ini.iAxisRunMode[4]==0)
							{
								mv = GetAngle(5,TRUE)*10;
							}
							else if (g_Ini.iAxisRunMode[4]==1)
							{
								INT16S  tempi1;
								FP32 	tempi2;
								FP32	y;
								
								y = GetMM(5,TRUE);
								tempi1 = y/360;
								tempi2 = y-tempi1*360.0;									
								mv = tempi1+tempi2/1000.0;
							}
							else   if (g_Ini.iAxisRunMode[4]==2)
							{
							
								mv = GetMM(5,TRUE)*100;
							}
							
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
							break;
							
						case 6:
							if(g_Ini.iAxisCount<6)
								break;
							
							if(g_Ini.iAxisSwitch[5]==0)
							 	break;
			
							g_iMoveAxis   = 6;
							g_iWorkStatus = NORUN;
							g_fSpeedBi    = 0.05 ;
							g_bHandMove   = TRUE;
							
							//if(g_Ini.iAxisRunMode[g_iWorkNumber-1]>=0 && g_Ini.iAxisRunMode[g_iWorkNumber-1]<=2)	
							{										
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
								{
									SetMM(8,0,FALSE);
									SetMM(7,0,FALSE);

									g_iMoveAxis=0;
									g_bHandMove=FALSE;
									
									return;	
								}
								
								OSQPost(MsgQueue," ");	
							}
							
							
							if(g_Ini.iAxisRunMode[5] == 0)
							{
								lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
								SetPulse(g_iWorkNumber, lTempPos,TRUE);
							}
							
							ShortlyPmove(g_iWorkNumber,pos>0?10000000L:-10000000L,speed);
								
							while (1)
							{
								if (IsFree(g_iWorkNumber)) 
									break;
								
								if(PulseLagCheck(g_iWorkNumber))
								{
									Stop1(g_iWorkNumber);
									g_Ini.iHandStatus = 0;
									LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
									CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								if(CheckLimit(TRUE,TRUE,g_iWorkNumber,pos))
									Stop1(g_iWorkNumber);
								
								OSTimeDly(1);
							}
							
							if (g_Ini.iAxisRunMode[5]==0)
							{
								mv = GetAngle(6,TRUE)*10;
							}
							else if (g_Ini.iAxisRunMode[5]==1)
							{
								INT16S  tempi1;
								FP32 	tempi2;
								FP32	y;
								
								y = GetMM(6,TRUE);
								tempi1 = y/360;
								tempi2 = y-tempi1*360.0;									
								mv = tempi1+tempi2/1000.0;
							}
							else  if (g_Ini.iAxisRunMode[5]==2)
							{
							
								mv = GetMM(6,TRUE)*100;
							}
							
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
							break;
							
						default:
							break;
					}		

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					OSTimeDly(2);

					if( !g_bScreen && (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] == 'M' && g_iWorkNumber>0)
					{
						if((g_iWorkNumber == 2 &&g_Ini.iAxisRunMode[1]==1)||(g_iWorkNumber == 3 &&g_Ini.iAxisRunMode[2]==1)|| (g_iWorkNumber == 4 &&g_Ini.iAxisRunMode[3]==1)|| (g_iWorkNumber == 5 &&g_Ini.iAxisRunMode[4]==1)|| (g_iWorkNumber == 6 &&g_Ini.iAxisRunMode[5]==1))
						{
							CEdit_SetFloat(mv,4);
						}
						else
						{
							if(g_Ini.iAxisCa==0)
							{	
								if((g_iWorkNumber==2 && (g_Ini.iAxisRunMode[1]==2 || g_Ini.iAxisRunMode[1]==3)) || (g_iWorkNumber==3 &&g_Ini.iAxisRunMode[2]==2) || (g_iWorkNumber==4 &&g_Ini.iAxisRunMode[3]==2))
									CEdit_SetFloat(mv/100.0,2);
								else
									CEdit_SetFloat(mv/10.0,1);
							}
							else
								CEdit_SetLong((INT32S)mv);
						}
						CEdit_UpdateData(FALSE);
					}
				}
				else
				{
					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
				}
			}			
		}
	}
	
	
/********************  寸动功能    ***********************/
	
//	if(g_uiMoveKey !=0 && !g_bMode /*&& !g_bWheel*/ && g_Ini.iHandStatus==0 && !g_bHandset)
//	{}
	
	
	//************************送线、退线*********************************//
	
	
	
}


void	CTeach_OnKey(INT16U key,INT16U tkey)
{
	COMMAND *ptr;
	INT16S  row=0;
	static  char disp[0x05]="\0";
	static  int  dispjs=0;
	INT8S   buf[20] = {0};


	
	if (!g_bEditEnable && g_Ini.iPasswordTime!=0)  //编辑数据需要密码
	{
		if ((key>='1' && key<='9') || key=='.' || key==TSAVE || key==TDEL
			 || key==TINSERT || key==TDELALL || key==TZERO || key==TCLEAR)
		{
			CPassword_Init(260,180,320,160,g_bEnglish?"Password verify":"密码验证",buf,g_TextColor2);
			
			if(CPassword_DoModal())
			{
				INT32S pass;						
				
				pass=CPassword_GetPassword();
				
				CPassword_Destroy();
				
				CTeach_Create();
				
				if(pass == g_Ini.lPassword || pass == 790909L)
				{
					INT8S buffer[0x100]="\0";							//如果申请的是INT8S buffer[0x30]="\0";	会出现显示自复位乱码，何解？
					sprintf(buffer,g_bEnglish?"Allow editing in %d minute":"%d分钟内允许编辑",g_Ini.iPasswordTime);
					OSQPost(MsgQueue,buffer);
					g_bEditEnable = TRUE;		
					return;		
				}
				else 
				{
					OSQPost(MsgQueue, g_bEnglish?"Password Wrong":"编辑密码不正确");
					return;
				}
			}
			else
			{
				CPassword_Destroy();
				CTeach_Create();
				return;
			}
		}
	}

	if(key=='.'|| key=='7'|| key=='9')// || key=='6')
	{
		//if(key == '-')
		//	disp[dispjs++] = '-';
		//else
			disp[dispjs++] = key;
			
		dispjs = dispjs%4;
	}
	else
	{
		dispjs = 0;
		memset(disp,NULL,5);
	}
	
	if(strcmp(disp,"..79")==0)
	{
		INT8S a = 0;
		char buf[0x70] = "项目号:";
		char bufE[0x70]= "PROJECT :";
		INT16U value,libver;
		
		g_bInsertNum = FALSE;
		g_bInterface = FALSE;
		
		value = Har_Ver1();														//获取FPGA硬件版本号
		libver = Lib_Ver();
				

		{
			memset(disp,NULL,5);
			strcat(buf,PROJECT_ID);
			strcat(buf,"\n软件版本:V");
			
			strcat(bufE,PROJECT_ID);
			strcat(bufE,"\nSOFTWARE:V ");
			
			{
				INT8U buf1[6]="\0";
				
				if(!ftoa((FP32)VER*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}
				
				strcat(buf,(INT8S *)buf1);
				strcat(bufE,(INT8S *)buf1);
			}
			
			{
				char buf2[0x20]="\0";
				
				sprintf(buf2,"%s%0x","\n硬件版本:",value);	
				strcat(buf,(INT8S *)buf2);
				sprintf(buf2,"%s%0x","\nHARDWARE:V ",value);
				strcat(bufE,(INT8S *)buf2);
			}
			
			strcat(buf,"\n日期:");
			strcat(buf,__DATE__);
			strcat(buf,"\n时间:");
			strcat(buf,__TIME__);
			strcat(buf,"\n库版本:V");
			
			strcat(bufE,"\nDATE:");
			strcat(bufE,__DATE__);
			strcat(bufE,"\nTIME:");
			strcat(bufE,__TIME__);
			strcat(bufE,"\nLIB :V ");
			
			{
				INT8U buf1[6]="\0";
				
				if(!ftoa((FP32)libver*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}
				
				strcat(buf,(INT8S *)buf1);
				strcat(bufE,(INT8S *)buf1);
			}		
		}	
		
		for(a=5; a<8; a++)
			SuspendTasks(a);
			
		OSTimeDly(50);
		CAfxMsgDlg_Init(190,130,410,280,g_bEnglish?bufE:buf,g_bEnglish?"MESSAGE":"信息提示",g_nBk);
		CAfxMsgDlg_DoModal();
		CAfxMsgDlg_Destroy();
		
		for(a=5; a<8; a++)
			ResumeTasks(a);
			
		CTeach_Create();
		SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
		SetMM(8,0,FALSE);	
		
		DisplayXYZ(FALSE);
		return;
	}
	
	
	if(!g_bDispServoWaring)
		g_bDispServoWaring=TRUE;

	if(g_iCurEditNo!=9)
	{ 
	//	if(key != TSYSMANGE && key != TREAD && key != TSCOMMAND &&  key != TENTER  &&  key != '.' && (key<'0'||key>'9')&&key!=TBACKSPACE
	//	&&key!=TLEFT&&key!=TRIGHT&&key!=TUP&&key!=TDOWN )
		if(TBACKZERO == key || TSTOP ==key || TTEST ==key || TCMD ==key 
		 ||TPAGEDOWN ==key ||TPAGEUP ==key ||TDEL ==key ||TINSERT ==key || TDELALL ==key
		 || (key>=F1 && key<=F8) )
		{
			UpdateFileDestProbe(g_iCurEditNo+1,1,0);
			g_iCurEditNo = 9;
			CEdit_UpdateData(TRUE);

			return;
		}
	}
	
	g_bClearNum = FALSE;
	g_bInsertNum = FALSE;

	switch(key)
	{
	
		case TBACKZERO:										//归零键
			{
				INT16S i=0,Number=0;
				
				if( CheckWarning() || CheckLimit(FALSE,TRUE,0,0))	
				{
					break;
				}
				
				if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!请抬起急停按钮!");
					break;
				}
				
				
				
				RunStatusOutCtr(0);
				
				g_bBackZeroProcess=TRUE;				
				
				LedOut(BACKZEROLED,1);
				g_iWorkNumber = 0 ;
				UpdateCurAxis();
				
				
				g_bBackZero=TRUE;
				g_iErrorNo=0;
				
				
				SuspendTasks(0);
				ResumeTasks(1);
				
				OSTimeDly(50); 
				
				LedOut(STOPLED,0);
				LedOut(BACKZEROLED,1);
				
				if(g_Ini.iBackStyle==1)
				{
					g_bFirstResetX = FALSE;			//有钢特殊需求，要求每次都搜索原点
					g_bFirstResetY = FALSE;
					g_bFirstResetZ = FALSE;
					g_bFirstResetU = FALSE;
				}
				
			//	g_bBackZero=TRUE;
			//	g_iErrorNo=0;
				
				if(g_Ini.iLimitStopAxis!=0)
				{
					if(!CheckLimitStop(g_Ini.iLimitStopAxis,g_Ini.iBackDir[g_Ini.iLimitStopAxis-1]))
					{	
						OSQPost(MsgQueue,g_bEnglish?"Find limit- is failed":"未读到到限位信号");
				        
				        SuspendTasks(1);
				        OSTimeDly(50);
				        ResumeTasks(0); 
				        
				        LedOut(STOPLED,1);
				        g_bBackZeroProcess=FALSE;
				        
						g_bBackZero=FALSE;
						
					//	OSTimeDly(500);
						return;
					}
				}
				
				
				for(Number=1;Number<=MAXCH;Number++)//支持单轴顺序归零兼容13个归零轴
			    {
			     	for(i=0;i<g_Ini.iAxisCount;i++)
			     	{ 
			      		if((g_Ini.iBackOrder[i] == Number) && (g_Ini.iAxisSwitch[i]==1))
			      		{
			       			if(!CTeach_BackZero(i+1))    //第N轴归零失败
						    {
						        SuspendTasks(1);
						        OSTimeDly(50);
						        ResumeTasks(0); 
						        
						        g_iWorkNumber = 0 ;
						        UpdateCurAxis();
						        
						        LedOut(STOPLED,1);
						        g_bBackZeroProcess=FALSE;
						        return;
						     }
			    		}
			     	}
			     
			     	OSTimeDly(30);
				}


				BackZeroLed();
				g_bEMSTOP=FALSE;		
				DisplayXYZ(TRUE);
				SuspendTasks(1);
				OSTimeDly(50);
				ResumeTasks(0);
				g_iWorkNumber = 0 ;
				LedOut(STOPLED,1);
				g_bBackZeroProcess=FALSE;

				SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
				SetMM(8,0,FALSE);	

		}
		break;
	
		case TYSWITCH:										//停线键
			{
				g_Ini.iAxisSwitch[1]=g_Ini.iAxisSwitch[1]==1?0:1;
			
				LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]?0:1);
				
				g_bFirstResetY = FALSE;
				g_bModify = TRUE; 
				g_bModifySave=FALSE;
				
				UpdateAxisStatus();
				SysParaWrite();
				DisplayXYZ(FALSE);
				BackZeroLed();
			}
			break;
		
		case TZSWITCH:										//转芯
			{
				if(g_Ini.iAxisCount<3)
					break;
				
				g_Ini.iAxisSwitch[2]=g_Ini.iAxisSwitch[2]==1?0:1;
			
				LedOut(ZSWITCHLED,g_Ini.iAxisSwitch[2]?0:1);;
				
				g_bFirstResetZ = FALSE;
		 		g_bModify = TRUE; 
				g_bModifySave=FALSE;
				
				UpdateAxisStatus();
				SysParaWrite();
				DisplayXYZ(FALSE);
				BackZeroLed();
			}
			break;
			
		case TUSWITCH:										//卷取
			{
				if(g_Ini.iAxisCount<4)
					break;
					
				g_Ini.iAxisSwitch[3]=g_Ini.iAxisSwitch[3]==1?0:1;
				
				LedOut(USWITCHLED,g_Ini.iAxisSwitch[3]?0:1);;
			
				g_bFirstResetU = FALSE;
				g_bModify = TRUE; 
				g_bModifySave=FALSE;
				
				UpdateAxisStatus();
				SysParaWrite();
				DisplayXYZ(FALSE);
				BackZeroLed();
			}
			break;
			
		case TF1:
			
			if(g_Ini.iF1==1)
			{
					if(g_iWorkNumber == 0 /*|| g_iWorkNumber == 2 */ || g_iWorkNumber > g_Ini.iAxisCount)
						break;
					
					switch(g_iWorkNumber)
					{
						case 6:
						{
							if(g_bFirstResetW == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset C":"请C轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为C轴起点?");
							}
							break;
						}
						
						case 5:
						{
							if(g_bFirstResetV == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset B":"请B轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为B轴起点?");
							}
							break;
						}
						
						case 4:
						{
							if(g_bFirstResetU == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset A":"请A轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为A轴起点?");
							}
							break;
						}
						
						case 3:
						{
							if(g_bFirstResetZ == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset Z":"请Z轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为Z轴起点?");
							}
							break;
						}
						
						
						case 2:
						{
							if(g_bFirstResetY == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset Y":"请Y轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为Y轴起点?");
							}
							break;
						}
						
						
						
						case 1:
						{
							if(g_bFirstResetX == FALSE)
							{
								OSQPost(MsgQueue,g_bEnglish?"Please Reset X":"请X轴先归零!");
								return;
							}
							else
							{
								OSQPost(MsgQueue,g_bEnglish?"Set the current positiom as a starting point":"是否设置当前点为X轴起点?");
							}
							break;
						}
						
						default:
						{
							Uart_Printf("g_iWorkNumber = %d\n",g_iWorkNumber);	
							return;
						}
						
					}
					
					LedOut(F1LED,1);
					
					{
						INT16U   offsetkey =0;
						
						while(TRUE)
						{
							offsetkey = KeyMBoxGetKey();
							
							//Uart_Printf("offsetkey  = %x\n",offsetkey);
							
							if(offsetkey!=0xffff)
							{
								if(offsetkey == TENTER)
								{
										
									if(g_Ini.iAxisRunMode[g_iWorkNumber-1] == 2)
									{
										g_Ini.fStartOffset[g_iWorkNumber-1]=GetMM(g_iWorkNumber,TRUE)+g_Ini.fStartOffset[g_iWorkNumber-1];
										BaseSetCommandPos(g_iWorkNumber,0);
									}
									else
									{
										g_Ini.fStartOffset[g_iWorkNumber-1]=GetAngle(g_iWorkNumber,TRUE)+g_Ini.fStartOffset[g_iWorkNumber-1];
										BaseSetCommandPos(g_iWorkNumber,0);
									}
							
									WriteDataToFile(g_Ini.iFileName);
									SysParaWrite();
									
									OSQPost(MsgQueue,g_bEnglish?"Set oringin success":"设置起点成功!");
								}
								else
								{
									OSQPost(MsgQueue,g_bEnglish?"Cancle":"取消设置起点!");
								}
								
								break;
							}
							
							if(RealIO(STOPIO,g_Ini.StopLevel,2))
							{
								break;
							}

							OSTimeDly(10);
						}
					}
					
					LedOut(F1LED,0);
			}
		
			break;	
	
		case TCYLINDER :									//气缸按键           835B的手动TWHEEL
			{
				if( CheckWarning()	|| CheckLimit(TRUE,TRUE,0,0))
				{
					break;
				}
				
				if(g_lStepNode!=0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Can't Enter State of Cylinder Test at Running!":"停机模式下才能进入气缸测试状态!");
					break;
				}
				
				
				{
					g_bCylinder=g_bCylinder?FALSE:TRUE;
					
					LedOut(CYLINDERLED,g_bCylinder?1:0);
					
					if(g_bCylinder)
						OSQPost(MsgQueue,g_bEnglish?"Enter State of Cylinder Test":"进入汽缸测试状态!");
					else
					{
						//INT16S i=0;
						
						//for(i=0;i<(g_Ini.iAxisCount<3?MAXCYLINDER-4:MAXCYLINDER);i++)
							//WriteBit(i,0);
						
						OSQPost(MsgQueue,g_bEnglish?"Exit State of Cylinder Test":"退出汽缸测试状态!");
					}
				}

			}							
			break;
			
		case TRUN:
			{
				if(g_iCurEditNo!=9)											//当前的编辑点不在列表时  为9为在列表编辑状态
				{ 
					UpdateFileDestProbe(g_iCurEditNo+1,1,0);
					g_iCurEditNo = 9;	
				}
			
				if(g_bRelease)
				{
					if( CheckWarning() || CheckLimit(TRUE,TRUE,0,0))
					{
						/*
						if(g_Ini.iWarningOut==1)
						{
							if(GetOut(O_WARNING)==0)
								WriteBit(O_WARNING,OPEN);
						}*/
						
				//		g_bWheel = FALSE;					
				//		LedOut(WHEELLED,0);		
									
						g_bMode=FALSE;
						LedOut(TESTLED,0);
						
						LedOut(STOPLED,1);
						
						return ;
					}
					
				}
				
				if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!请抬起急停按钮!");
					break;
				}
				
				
				

				if(g_bCylinder)
				{
					INT16S i=0;
					
					g_bCylinder = FALSE;				
					LedOut(CYLINDERLED,g_bCylinder?1:0);							//指示灯亮表示处于汽缸测试状态

				//	for(i=0; i<MAXCYLINDER; i++)
				//		WriteBit(i,0);
				}
										
						
			/*	if( g_lStepNode )
				{
					CTeach_OnKey(TSTOP,0);
					
					if(g_iWorkStatus == RUNSTOP) 
					{
						return;
					}
					
					if( g_lStepNode!=0 )
						return;
				}	*/
					
				if(g_lStepNode==0 && g_bModify)
				{
					INT32S t1,t2;
					
					OSSemPost(UnpackDataSem);
					g_bUnpackCorrect = FALSE;
				
					t1 = OSTimeGet();
					
					while(TRUE)
					{
						OSTimeDly(2);
						
						if(g_bUnpackCorrect)
							break;
							
						t2 = OSTimeGet();

						if((t2-t1)>200 || (t2-t1<-200))
						{
							return;
						}
					}
				}
				
				
				g_bDispRunLine = FALSE;  
			//	g_bEMSTOP      = FALSE;
				g_bAutoRunMode = TRUE;
				g_bStepToSingle= FALSE;
				g_bTestStart   = FALSE;
				g_bStepIO      = FALSE;
				g_bHandRun	   = FALSE;
				g_bCanDispRunTime = TRUE;

				if(g_bMode)
				{
					g_bMode = FALSE;
					LedOut(TESTLED,g_bMode?1:0);									//灯亮表示进入测试模式
				}
				
				LedOut(RUNLED,1);									//灯亮表示进入测试模式

				
				SetRange(8000);
				RunAllData();
				
				return;
			}
			break;
			
			
	
		case TSTOP:											//单条，停止键
			{
				int i = 0;
							
				if(CheckWarning()	|| CheckLimit(TRUE,TRUE,0,0))
					break;
					
				if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!请抬起急停按钮!");
					break;
				}
					
				
				if(g_iCurEditNo!=9)
				{
					UpdateFileDestProbe(g_iCurEditNo+1,1,0);
					g_iCurEditNo=9;
				}
				
				if(g_bCylinder)
				{
					g_bCylinder=FALSE;
					g_pszLedOut[0][0]=CYLINDERLED;
					g_pszLedOut[0][1]=g_bCylinder?1:0;
					OSQPost(LedOutQueue,&g_pszLedOut[0]);
				//	for(i=0;i<MAXCYLINDER;i++)
				//		WriteBit(i,0);
				}
				
				if(g_lStepNode==0 && g_bModify)
				{
					INT32S t1,t2;
					
					OSSemPost(UnpackDataSem);
					g_bUnpackCorrect = FALSE;
				
					t1 = OSTimeGet();
					
					while(TRUE)
					{
						OSTimeDly(2);
						
						if(g_bUnpackCorrect)
							break;
							
						t2 = OSTimeGet();

						if((t2-t1)>200 || (t2-t1<-200))
						{
							return;
						}
					}
				}
				
				g_bAutoRunMode = FALSE;
				g_bStepIO=FALSE; 
				g_bDispRunLine=FALSE;
				g_bTestStart=FALSE;
				RunAllData();
				
			} break;
			
		
		case TTEST:											//测试按键
			{
			
				if( CheckWarning()	|| CheckLimit(TRUE,TRUE,0,0))
				{
					break;
				}
				
				if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
				{
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!请抬起急停按钮!");
					break;
				}
					
				if(g_bCylinder)  							//汽缸测试状态下把汽缸关闭
				{
					INT16S i;
					
					g_bCylinder=FALSE;
					g_pszLedOut[0][0]=CYLINDERLED;
					g_pszLedOut[0][1]=g_bCylinder?1:0;
					OSQPost(LedOutQueue,&g_pszLedOut[0]);
					
				//	for(i=0;i<MAXCYLINDER;i++)
				//		WriteBit(i,0);
				}
			
				g_bMode=g_bMode?FALSE:TRUE;
				LedOut(TESTLED,g_bMode?1:0);

				if(g_bMode)
				{
					OSQPost(MsgQueue,g_bEnglish?"Goto TestMode":"进入测试模式!");
					g_bTestToSingle=FALSE; 
				}
				else
				{
					OSQPost(MsgQueue,g_bEnglish?"Out TestMode":"退出测试模式!");
				}

			//	g_bWheel = FALSE;					
			//	LedOut(WHEELLED,0);

				if(!g_bMode)
				{
					g_iWorkNumber=0;
					UpdateCurAxis();
				}		
			}
			break;
			
	

		case TREAD:												//料号
			OSQPost(MsgQueue,g_bEnglish?"Select file":"选取料号");
			UpdateFileDestProbe(1,-1,0);
			break;
			
			
		case TFILE:												//目录
			{
				INT16S len;
				len = CEdit_GetLen();														//记录当前编辑框的可输入字符长度
				

				SuspendTasks(5);															//如果不挂起任务编号5，当某个轴被关闭时，坐标位置显示为Off，切换界面时出现覆盖														
				SuspendTasks(7);
				SuspendTasks(0);
				
				OSTimeDly(10);

				WriteDataToFile(g_Ini.iFileName);
				
				CFileFrm_Init(0,0,800,600);
				
				CFileFrm_DoModal();

				CFileFrm_Destroy();
				
				ReadDataFromFile(g_Ini.iFileName);										//从加工文件读取数据到各个相关全局变量
				WriteRunCount();														//加工数
				
				g_bModifySave=TRUE;
				g_bModify = TRUE;															//由于可能切料号，加工文件要重新解析
				g_lStepNode=0;																//测试加工重置					
		
				SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
				SetMM(8,0,FALSE);	
		
				OSTimeDly(30);
				CTeach_Create();				

				ResumeTasks(5);
				ResumeTasks(7);
				ResumeTasks(0);
				
			}
			break;
			
			
		case THELP:
			{
				INT8S a = 0;
				char buf[0x70] = "项目号:";
				char bufE[0x70]= "PROJECT :";
				INT16U value,libver;
				
				g_bInsertNum = FALSE;
				g_bInterface = FALSE;
				
				value = Har_Ver1();														//获取FPGA硬件版本号
				libver = Lib_Ver();
						

				{
					memset(disp,NULL,5);
					strcat(buf,PROJECT_ID);
					strcat(buf,"\n软件版本:V");
					
					strcat(bufE,PROJECT_ID);
					strcat(bufE,"\nSOFTWARE:V ");
					
					{
						INT8U buf1[6]="\0";
						
						if(!ftoa((FP32)VER*0.01,buf1,2,0))
						{
							Uart_Printf("BfToStr OverFlow!\n");
						}
						
						strcat(buf,(INT8S *)buf1);
						strcat(bufE,(INT8S *)buf1);
					}
					
					{
						char buf2[0x20]="\0";
						
						sprintf(buf2,"%s%0x","\n硬件版本:",value);	
						strcat(buf,(INT8S *)buf2);
						sprintf(buf2,"%s%0x","\nHARDWARE:V ",value);
						strcat(bufE,(INT8S *)buf2);
					}
					
					strcat(buf,"\n日期:");
					strcat(buf,__DATE__);
					strcat(buf,"\n时间:");
					strcat(buf,__TIME__);
					strcat(buf,"\n库版本:V");
					
					strcat(bufE,"\nDATE:");
					strcat(bufE,__DATE__);
					strcat(bufE,"\nTIME:");
					strcat(bufE,__TIME__);
					strcat(bufE,"\nLIB :V ");
					
					{
						INT8U buf1[6]="\0";
						
						if(!ftoa((FP32)libver*0.01,buf1,2,0))
						{
							Uart_Printf("BfToStr OverFlow!\n");
						}
						
						strcat(buf,(INT8S *)buf1);
						strcat(bufE,(INT8S *)buf1);
					}		
				}	
				
				for(a=5; a<8; a++)
					SuspendTasks(a);
					
				OSTimeDly(50);
				CAfxMsgDlg_Init(190,130,410,280,g_bEnglish?bufE:buf,g_bEnglish?"MESSAGE":"信息提示",g_nBk);
				CAfxMsgDlg_DoModal();
				CAfxMsgDlg_Destroy();
				
				for(a=5; a<8; a++)
					ResumeTasks(a);
					
				CTeach_Create();
				SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
				SetMM(8,0,FALSE);	
				
				DisplayXYZ(FALSE);
				return;
			}
			break;	
			
		case TPROBESET:											//探针设定			835B汽缸设定
			{
				OSQPost(MsgQueue,g_bEnglish?"Probe settings":"设定探针失败次数");
				UpdateFileDestProbe(3,-1,0);
			}
			break;
			

		case TPROBECLR:											//探针失败清零			835B 探针设定次数
			{
				g_lProbeFail=0;
				UpdateSpeedCountProbe();
				OSQPost(MsgQueue,g_bEnglish?"Probe clear!":"探针清零!");
			}
			break;
			
		case TCOUNTSET:											//产量设定
			OSQPost(MsgQueue,g_bEnglish?"Count settings":"设定加工数量");
			UpdateFileDestProbe(2,-1,0);
			break;
			
		case TCOUNTCLR:											//产量清零
			{
				INT8S a=0;
				
				for(a=5;a<8;a++)
					SuspendTasks(a);
					
				OSTimeDly(50);
				CDC_SaveScreen();
				CMsgDlg_Init(230,140,260,170,g_bEnglish?"Clear these Count?":"清除加工数量?",g_bEnglish?"Warning":"警告",REDCOLOR,g_bEnglish?"Yes":"是",g_bEnglish?"No":"否");
				
				if(CMsgDlg_DoModal())
				{
					g_Ini.lRunCount=0;
					SysParaWrite();
					WriteRunCount();
					WriteDataToFile(g_Ini.iFileName);
					CMsgDlg_Destroy();
					CDC_RestoreScreen();

					CDC_PutColorBar(306,67,75,24,g_nBk,1);

					UpdateSpeedCountProbe();
					OSQPost(MsgQueue,g_bEnglish?"Count is cleared!":"加工数量清零!");
					
					for(a=5;a<8;a++)
						ResumeTasks(a);
						
					break;
				}
				else
				{
					CMsgDlg_Destroy();
					CDC_RestoreScreen();
					UpdateSpeedCountProbe();
					
					for(a=5;a<8;a++)
						ResumeTasks(a);
				}
			}
			break;

			/*
		case TSENDCLR:											//送线清零
			{
				SetMM(2,0,TRUE);
			}
			break; 
			*/
			
		case TBEILV:											//倍率					835B 复制
			{	
				if(g_bHandset)
				{
					OSQPost(MsgQueue,g_bEnglish?"This key is invalid at state of handset":"手持盒操作状态该键无效!");
					break;
				}
				CTeach_UpdateSpeed(1);
			}
			break;

		/*	
		case TSTEPSPEED:										//寸动速度
			
			UpdateFileDestProbe(4,0,0);
			OSQPost(MsgQueue,g_bEnglish?"Speed settings":"设定寸动速度");
			break;
			*/
			
			
		case TCMD:
			{																		//指令切换
			 	char buf[0x02];
			 	
			 	memset(buf,'\0',2);
					
				g_iCommandPos += 1;
				
				if(g_iCommandPos>6)
			 	    g_iCommandPos = 0;
			 	    
			 	buf[0] = g_cCommand[g_iCommandPos];
			 	
			    UpdateCommand(buf);
			
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				
				WriteDataToFile(g_Ini.iFileName);
				g_bModifySave = TRUE;
				g_bModify = TRUE;				    
			}
			break;
			
	/*	
		case TSCOMMAND:											//S指令					835B 产速设定
			{
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[2][0]||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					OSQPost(MsgQueue,g_bEnglish?"command S format error!":"S指令数据格式错误!");
					break;
				}
				else
				{
					FP32 tmp;
					
					tmp=(FP32)stringtof((INT8U *)ptr->value[1]);
					
					if(tmp>g_Ini.fXMaxSpeed)
					{
						OSQPost(MsgQueue,g_bEnglish?"X-Speed too Large!":"S指令设定速度太大!");
						break;
					}		 	
				}
				
				UpdateCommand("S");
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );
			}
			break;
				
		case TGCOMMAND:											//G指令					835B的归零模式THOMEMODE
			{		
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[2][0]||ptr->value[3][0]||(g_Ini.iAxisCount>2?ptr->value[4][0]:0)||
				(g_Ini.iAxisCount>3?ptr->value[5][0]:0)||(g_Ini.iAxisCount>4?ptr->value[6][0]:0)||(g_Ini.iAxisCount>5?ptr->value[7][0]:0)
				||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					OSQPost(MsgQueue,g_bEnglish?"command G format error!":"跳转指令数据格式错误!");
					break;
				}
				else
				{
					INT16S tmp;
					tmp=stringtoi((INT8U *)ptr->value[1]);
					
					if(tmp>MAXLINE)
					{
						OSQPost(MsgQueue,g_bEnglish?"G parameter value<=200!":"跳转指令设定值<=200!");
						break;
					}		 	
				}
				UpdateCommand("G");
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );

			}
			break;
			
		case TMCOMMAND:											//M指令  				835B 目录键
			{
				UpdateCommand("M");
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );

			}
			break;	
			
		case TJCOMMAND:											//J指令		835B的触棒按键
			{
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[2][0]||ptr->value[3][0]||(g_Ini.iAxisCount>2?ptr->value[4][0]:0)||
				(g_Ini.iAxisCount>3?ptr->value[5][0]:0)||(g_Ini.iAxisCount>4?ptr->value[6][0]:0)||(g_Ini.iAxisCount>5?ptr->value[7][0]:0)
				||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					OSQPost(MsgQueue,g_bEnglish?"command J format error!":"跳转指令数据格式错误!");
					break;
				}
				else
				{
					INT16S tmp;
					tmp=stringtoi((INT8U *)ptr->value[1]);
					
					if(tmp>MAXLINE)
					{
						OSQPost(MsgQueue,g_bEnglish?"J parameter value <=200!":"跳转指令设定值<=200!");
						break;
					}		 	
				}
				
				UpdateCommand("J");
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );

			}
			break;
			
		case TECOMMAND:										//结束指令				835B的汽缸按键
			{
		
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[1][0]||ptr->value[2][0]||ptr->value[3][0]||(g_Ini.iAxisCount>2?ptr->value[4][0]:0)||
				(g_Ini.iAxisCount>3?ptr->value[5][0]:0)||(g_Ini.iAxisCount>4?ptr->value[6][0]:0)||(g_Ini.iAxisCount>5?ptr->value[7][0]:0)
				||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					if(CList_CurCol()==2)
					{
						if(!CEdit_IsEmpty())
					 	{
							OSQPost(MsgQueue,g_bEnglish?"command E format error!":"E指令数据格式错误!");
							break;
						}
						else
						{
							if(!UpdateCommand("E"))
								break;
								
							 CEdit_String( ptr->value[1]);
							 CTeach_Update(0,2-CList_CurCol());
							 UpdateCurAxis();
							 OSQPost(MsgQueue," ");
							 break;
						}
					}
					else
					{
						OSQPost(MsgQueue,g_bEnglish?"command E format error!":"E指令数据格式错误!");
						break;
					}		
				}
				
				if(!UpdateCommand("E"))
					break;
					
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				OSQPost(MsgQueue," ");
				WriteDataToFile( g_Ini.iFileName );

			}
			break;
			
		case TLCOMMAND:
			{
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[2][0]||ptr->value[3][0]||(g_Ini.iAxisCount>2?ptr->value[4][0]:0)||
				(g_Ini.iAxisCount>3?ptr->value[5][0]:0)||(g_Ini.iAxisCount>4?ptr->value[6][0]:0)||(g_Ini.iAxisCount>5?ptr->value[7][0]:0)
				||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					OSQPost(MsgQueue,g_bEnglish?"command L format error!":"循环指令数据格式错误!");
					break;
				}
				else
				{
					INT16S tmp;
					
					tmp=stringtoi((INT8U *)ptr->value[1]);
					
					if(tmp>1000)
					{
						OSQPost(MsgQueue,g_bEnglish?"L parameter value <=1000!":"循环指令设定值<=1000!");
						break;
					}		 	
				}
				
				UpdateCommand("L");
				CTeach_Update(0,2-CList_CurCol());
				UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );

			}
			break;
			
		case TNCOMMAND:
			{
				ptr=(Line_Data+CList_StartRow()+CList_CurRow());
				
				if(ptr->value[1][0]||ptr->value[2][0]||ptr->value[3][0]||(g_Ini.iAxisCount>2?ptr->value[4][0]:0)||
				(g_Ini.iAxisCount>3?ptr->value[5][0]:0)||(g_Ini.iAxisCount>4?ptr->value[6][0]:0)||(g_Ini.iAxisCount>5?ptr->value[7][0]:0)
				||ptr->value[8][0]||ptr->value[9][0]||ptr->value[10][0])
				{
					OSQPost(MsgQueue,g_bEnglish?"command N format error!":"循环结束指令数据格式错误!");
					break;
				}
			
				UpdateCommand("N");
		 		CTeach_Update(0,2-CList_CurCol());
		 		UpdateCurAxis();
				WriteDataToFile( g_Ini.iFileName );

			}
			break;
			*/
	
		case TINSERT:											//插入												
			
			{
				CTeach_Insert();
				CTeach_InvidateView(FALSE);	
				
				WriteDataToFile( g_Ini.iFileName );

			} 
			break;
			
		case TDEL:												//删除单行
			{
				INT16S row,size,i;
				COMMAND *ptr1;
				
				row=CList_CurRow()+CList_StartRow();
				ptr1= Line_Data+row;
				
				if(g_iWorkStatus==RUNNING && ptr1->value[0][0]!='M')
				{
					OSQPost(MsgQueue,g_bEnglish?"Don't Delete Command except 'M' at Running!":"加工运行时不可删除M指令之外的指令!");
					break;
				}
				
				if(g_bMode&&g_lStepNode!=0 && ptr1->value[0][0]!='M')
				{
					OSQPost(MsgQueue,g_bEnglish?"One spring don't run complete,not delete command at test mode":"测试模式下,一条弹簧未加工完之前不允许删除M指令之外的指令!");
					break;
				}
				
				CTeach_Delete();
				
				if( row >= Line_Data_Size ) 
				{
					CTeach_Update(-1,0);
					break;
				}
				
				size = Line_Data_Size;
				
				if( size < 1 )
					return;
				
				size = size - CList_StartRow();
				
				if( size > ROWS ) size = ROWS;
				
				for(i=0; i<size; i++)
				   CTeach_DisplayLine(i+CList_StartRow(),i,Reset,g_TextColor1,g_nTableBk);
				
				if(size<ROWS)
				{
					CTeach_DisplayLine(size+CList_StartRow(),size,Reset,g_TextColor1,g_nTableBk);
				}
				
				CTeach_Update(0,0);
				WriteDataToFile( g_Ini.iFileName );

			} break;
		
		case TDELALL:											//全删				835B的变速度
			{
			
				#ifdef	PRINTBMP
				{
					char filename[0x20];
					INT16S i=0;
					
					strcpy(filename,"mainFrm");
					
					for(i=0;i<strlen(filename);i++)
					{
						if(filename[i]=='.')
						{
							filename[i]='\0';
							break;
						}
					}
					
					strcat(filename,".bmp");
					
					CreatBMP(filename,800,600,1);
					
					OSQPost(MsgQueue,"Write Bmp End!");
				}
				
				break;
				#endif
				
				
				{
					INT8S a=0;

					if(g_bMode||g_lStepNode!=0)
					{
						OSQPost(MsgQueue,g_bEnglish?"Can't delete all program at testmode or running":"测试加工模式下或加工未结束时不能全部删除程序!");
						break;
					}
					
					for(a=5;a<8;a++)
						SuspendTasks(a);
						
					OSTimeDly(50);
					
					if(g_bEnglish)
						CMsgDlg_Init(250,140,220,150,"Delete all data?","warnning",REDCOLOR,"Yes","No");
					else
						CMsgDlg_Init(250,140,220,150,"删除所有数据吗?","警告",REDCOLOR,"是","否");
					
					if(CMsgDlg_DoModal())
					{
						CTeach_DeleteAll();
						
					}
					
					CMsgDlg_Destroy();
					
					for(a=5;a<8;a++)
						ResumeTasks(a);
						
					CTeach_Create();
					
					SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
					SetMM(8,0,FALSE);	
					
					DisplayXYZ(FALSE);
				}
			}
			break;
			
			
		case TSYSMANGE:											//系统管理  			835B的倍率设置TTIMES
			{
				if(g_iCurEditNo!=9) 
				{			
					OSQPost(MsgQueue,"");
					UpdateFileDestProbe(g_iCurEditNo+1,1,0);
					g_iCurEditNo=9;						
				}
				
				CTeach_ParaSelect();	
			}
			break;
			
		case TWORKPARA:											//工作参数			835B的重复按键 循环指令
			{
				INT32S	pulse;
				INT16U 	len=CEdit_GetLen();
				INT8S 	a=0;
				FP32	tmpf[8];
				
				tmpf[0]=g_Ini.fStartOffset[0];
				tmpf[2]=g_Ini.fStartOffset[2];
				tmpf[3]=g_Ini.fStartOffset[3];
				tmpf[4]=g_Ini.fStartOffset[4];
				tmpf[5]=g_Ini.fStartOffset[5];
				
				
				if(g_iCurEditNo!=9) 
				{			
					OSQPost(MsgQueue,"");
					UpdateFileDestProbe(g_iCurEditNo+1,1,0);
					g_iCurEditNo=9;						
				}
				
				for(a=5;a<8;a++)
					SuspendTasks(a);
					
				OSTimeDly(50);			
				CDC_SaveScreen();
				
				g_iCurEditNo = 5;
				
				CParaSet_Init(100,45,550,520);
				
				CParaSet_DoModal();
				CParaSet_Destroy();
				
				/*switch(g_Ini.iAxisCount)
				{
					case 6:
						//if(tmpw!=g_Ini.fStartOffset[5])
							//g_bFirstResetW=FALSE;  
							
						BaseSetCommandPos(6,g_Ini.iAxisRunMode[2] ==2?GetPulseFromMM(6,g_Ini.fStartOffset[5])
						:GetPulseFromAngle(6,g_Ini.fStartOffset[5]));
					case 5:
						//if(tmpv!=g_Ini.fStartOffset[4])
							//g_bFirstResetV=FALSE;
						BaseSetCommandPos(6,g_Ini.iAxisRunMode[2] ==2?GetPulseFromMM(6,g_Ini.fStartOffset[5]):GetPulseFromAngle(6,g_Ini.fStartOffset[5]));	
					case 4:
						//if(tmpu!=g_Ini.fStartOffset[3])
							//g_bFirstResetU=FALSE;  
					case 3:
						//if(tmpz!=g_Ini.fStartOffset[2])
							//g_bFirstResetZ=FALSE;
						break;
				}*/
				
				
				for(a = 1;a <= g_Ini.iAxisCount;a++ )
				{
					if(a==2)
						continue;
						
					if(g_Ini.iAxisRunMode[a-1]==2)
						pulse = GetPulse(a,TRUE)+GetPulseFromMM(a,tmpf[a-1]-g_Ini.fStartOffset[a-1]);
					else
					{
						if(g_Ini.fStartOffset[a-1] >359.99)
						{
							g_Ini.fStartOffset[a-1] = tmpf[a-1];
							WriteDataToFile(g_Ini.iFileName);
							continue;
						}
						else
							pulse = GetPulse(a,TRUE)+GetPulseFromAngle(a,tmpf[a-1]-g_Ini.fStartOffset[a-1]);
					}
					
					BaseSetCommandPos(a,pulse);
			
				}
				
				//if(tmpx!=g_Ini.fStartOffset[0])
					//g_bFirstResetX=FALSE;
				
				SysParaWrite();
				
				g_iCurEditNo = 9;
				
				CDC_RestoreScreen();
							
				for(a=5;a<8;a++)
					ResumeTasks(a);
					
				CTeach_Update(0,0);
				CEdit_SetLen(len);
				SetMM(1,0,FALSE);
				
				SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
				SetMM(8,0,FALSE);	
			}
			break;
		
		case TSAVE:	
			
			if(g_iCurEditNo==0) 
			{
				INT16S tmpfile=g_Ini.iFileName;
				INT16S tmpfile2;
				INT16S res=0;
				
				tmpfile2=CEdit_GetInt();
				
				if(tmpfile!=tmpfile2)
				{						
					if(FileExist(tmpfile2))
					{
						INT8U a;
						
						for(a=5;a<8;a++)
							SuspendTasks(a);
							
						OSTimeDly(50);
						
						if(g_bEnglish)
							CMsgDlg_Init(200,140,350,180,"This alreay exist,Replace it?","Warning",REDCOLOR,"Yes","No");
						else
							CMsgDlg_Init(200,140,350,180,"文件已经存在,覆盖该文件?","警告",REDCOLOR,"是","否");
						
						if(CMsgDlg_DoModal())
						{
							CMsgDlg_Destroy();
							
							for(a=5;a<8;a++)
								ResumeTasks(a);
							
							CTeach_Create();
						
						}
						else
						{
							CMsgDlg_Destroy();
							
							for(a=5;a<8;a++)
								ResumeTasks(a);
								
							OSQPost(MsgQueue,g_bEnglish?"Copy file is canceled!":"取消文件复制!");
							
							CTeach_Create();							

							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							return;
						}

					}
					
					if(tmpfile2>=0 && tmpfile2<=999)
					{
						WriteDataToFile(tmpfile2);
						OSQPost(MsgQueue,g_bEnglish?"Copy succeed!":"复制成功!");
					}
				}
				else
				{
					OSQPost(MsgQueue,g_bEnglish?"The same file can't copy!":"同一文件,不能复制!");
				}
				
				return;
			}	
			
																//存储  TN  
			{	
				if(g_lStepNode==0)
				{
					INT32S t1,t2;
					
					g_bUnpackCorrect = FALSE;
					
					OSSemPost(UnpackDataSem);
					
					t1=OSTimeGet();
					
					while(TRUE)
					{
						OSTimeDly(2);
						
						if(g_bUnpackCorrect)
							break;
							
						t2=OSTimeGet();
						
						if((t2-t1)>200 || ((t2-t1)<-200))
							return;
					}
				}
				
				WriteDataToFile( g_Ini.iFileName );
				OSTimeDly(2);
				SysParaWrite();
				WriteRunCount();
				g_bModifySave=TRUE;
				OSQPost(MsgQueue,g_bEnglish?"File Save!":"文件保存!");
			} 
			break;
			
		case TCANCEL:											//取消 
			{/*
				INT8S a;
				
				if(!g_bRelease)
				{
					for(a=5;a<8;a++)
						SuspendTasks(a);
						
					CDC_SaveScreen();
					
					UsbMain();
					
					for(a=5;a<8;a++)
						ResumeTasks(a);
					
					CDC_RestoreScreen();
				}
				*/
			}
			break;
			
	
		case TUP:												//向上键
		{
			BOOL    IsEnd=FALSE;
			INT16S  i=0;
			INT16S  row=CList_CurRow()+CList_StartRow();
			COMMAND *ptr1= Line_Data+row+1;
			INT8S   tmp[9]="\0";
			
			if(g_iCurEditNo!=9) 
			{
				switch(g_iCurEditNo)
				{
					case 0:OSQPost(MsgQueue,g_bEnglish?"Exit the state of file select":"退出料号选取状态!");break;
					case 1:OSQPost(MsgQueue,g_bEnglish?"Exit the settings of running count":"退出加工数量设定状态!");break;
					case 2:OSQPost(MsgQueue,g_bEnglish?"Exit the state of probe settings":"退出探针设定状态!");break;
					case 3:OSQPost(MsgQueue,g_bEnglish?"Exit state of parameter settings":"退出寸动参数编辑状态!");break;
					default:OSQPost(MsgQueue,g_bEnglish?"Exit the state of parameter settings":"退出参数编辑状态!");break;
				}
				
				UpdateFileDestProbe(g_iCurEditNo+1,1,0);
				
				g_iCurEditNo=9;
								
				return;
			}
			
			ptr = Line_Data+row;
						
			for(i=row;i<Line_Data_Size;i++)
			{
				ptr1=Line_Data+i;
				if(ptr1->value[0][0]=='E')
					IsEnd=TRUE;
			}
			
			strcpy(tmp,ptr->value[1]);
			
			//if(ptr->value[0][0]=='M' && (stringtoi((INT8U *) tmp))== 0 && ptr->value[2][0]==NULL && !IsEnd)
			if(ptr->value[0][0]=='M' && ptr->value[1][0]==NULL && ptr->value[2][0]==NULL
				&& (g_Ini.iAxisCount>=2?ptr->value[3][0]==NULL:1)
				&& (g_Ini.iAxisCount>=3?ptr->value[4][0]==NULL:1)
				&& (g_Ini.iAxisCount>=4?ptr->value[5][0]==NULL:1)
				&& (g_Ini.iAxisCount>=5?ptr->value[6][0]==NULL:1)
				&& (g_Ini.iAxisCount>=6?ptr->value[7][0]==NULL:1)
				 && !IsEnd)
			{
				CTeach_OnKey(TDEL,0);
				
				if(CList_CurRow()>0 && row != Line_Data_Size)	//删除空M指令行后编辑框跳到上一行
				{
					CTeach_Update(-1,0);
				}
			}
			else
			{				
				CTeach_Update(-1,0);
				UpdateCurAxis();
			}
			
		}		
		break;
		
	case TDOWN:													//向下键
		{
			INT16S row=CList_CurRow()+CList_StartRow();
			
			if(g_iCurEditNo!=9) //20061229 modify add
			{
				
				//UpdateFileDestProbe(g_iCurEditNo+1,1,0);
				
				switch(g_iCurEditNo)
				{
					case 0:OSQPost(MsgQueue,g_bEnglish?"Exit the state of file select":"退出料号选取状态!");break;
					case 1:OSQPost(MsgQueue,g_bEnglish?"Exit the settings of running count":"退出加工数量设定状态!");break;
					case 2:OSQPost(MsgQueue,g_bEnglish?"Exit the state of probe settings":"退出探针设定状态!");break;
					case 3:OSQPost(MsgQueue,g_bEnglish?"Exit state of parameter settings":"退出寸动参数编辑状态!");break;
					default:OSQPost(MsgQueue,g_bEnglish?"Exit the state of parameter settings":"退出参数编辑状态!");break;
				}
				
				UpdateFileDestProbe(g_iCurEditNo+1,1,0);
				
				g_iCurEditNo=9;
								
				return;
			}
			
			ptr = Line_Data+row;
			
			if( row == Line_Data_Size -1 )
			{	
				CTeach_AddData();
									
				CList_SetCurCol(2);
			
				if(row>=MAXLINE-1)							//在最大行了，不移动编辑框
					CTeach_Update(0,0);	
				else
					CTeach_Update(1,0);
				
				ptr=Line_Data+CList_CurRow()+CList_StartRow();
				
				//Uart_Printf("\nCList_CurRow()=%d,CList_StartRow()=%d",CList_CurRow(),CList_StartRow());
						
				if(ptr->value[0][0]=='M')
				{						
					CEdit_SetLong((INT32S)(GetAngle(1,TRUE)*10));
					CEdit_UpdateData( FALSE );
				}

				break;
				
			}
			else
			{
				CTeach_Update(1,0);
			}
			
			//Uart_Printf("Currow=%d Start=%d\n",CList_CurRow(),CList_StartRow());
		}
		break;

	case TRIGHT:												//向右键
			{	
				INT16S tmp=g_iCurEditNo;
				
				switch(g_iCurEditNo)
				{
					case 0:
					case 1:
					case 2:
						tmp++;
						
						if(tmp>2)
							tmp = 0;
							
						if(tmp==0)
						{
							OSQPost(MsgQueue,g_bEnglish?"Select File":"选取料号");
						}
						else if(tmp==1)
						{
							OSQPost(MsgQueue,g_bEnglish?"Count Setting":"设定加工数量");
						}
						else if(tmp==2)
						{
							OSQPost(MsgQueue,g_bEnglish?"Probe Setting":"设定探针失败次数");
						}
							
						UpdateFileDestProbe(tmp+1,-1,0);
						break;
						
					case 3:
						UpdateFileDestProbe(4,1,-1);
						break;
					
					case 9:
						if(CList_CurRow()+CList_StartRow() >= Line_Data_Size-1&&CList_CurCol() >= CList_MaxCol()-1)
							break;
							
						if(CList_CurCol() == CList_MaxCol()-1)
						{
							CTeach_Update(1,1-CList_CurCol());
						}
						else
							CTeach_Update(0,1);
							
						UpdateCurAxis();
						break;
				}
				
			}
			break;
			
	case TLEFT:													//向左键
			{	
				INT16S tmp=g_iCurEditNo;
				
				//Uart_Printf("tstt=%d\n",g_iCurEditNo);
				switch(g_iCurEditNo)
				{
				case 0:
				case 1:
				case 2:
					tmp--;
					
					if(tmp<0)
						tmp = 2;
						
					if(tmp==0)
						{
							OSQPost(MsgQueue,g_bEnglish?"Select File":"选取料号");
						}
						else if(tmp==1)
						{
							OSQPost(MsgQueue,g_bEnglish?"Count Setting":"设定加工数量");
						}
						else if(tmp==2)
						{
							OSQPost(MsgQueue,g_bEnglish?"Probe Setting":"设定探针失败次数");
						}
						
					UpdateFileDestProbe(tmp+1,-1,0);					
					break;
					
				case 3:
					UpdateFileDestProbe(4,-1,-1);
					break;
					
				case 9:
					if(CList_CurCol() == 2 && CList_CurRow() == 0 && CList_StartRow() ==0)
						break;
					if(CList_CurCol() == 2 )
					{
						CTeach_Update(-1,CList_MaxCol()-1-CList_CurCol());
					}
					else
						CTeach_Update(0,-1);
					UpdateCurAxis();
					break;
				}
			}
			break;
			
		case TENTER:											//记忆键
		{
			
			{
			
				char str[0x10]="\0";
					
				switch(g_iCurEditNo)
				{
					case 0:
						{
							
							INT16S itemp=g_Ini.iFileName;
							
							
							
							UpdateFileDestProbe(1,1,1);
							g_iCurEditNo = 9;
							
							if(itemp!=g_Ini.iFileName)
							{
								
								//g_bResetFirstX
								g_bNewProFile=TRUE;
								
								CList_SetStartRow(0);            //创建主界面时让光标落在第2行的"开始角"列 
	       						CList_SetCurRow(1);
	       						CList_SetCurCol(2);
								
								CTeach_InvidateView(FALSE);	
								
								UpdateFileDestProbe(0,0,0);
							}
							
						}
						return;
					case 1:
						UpdateFileDestProbe(2,1,1);	
						g_iCurEditNo=9;			
						return;
					case 2:
						UpdateFileDestProbe(3,1,1);	
						g_iCurEditNo=9;			
						return;
					case 3:					
						UpdateFileDestProbe(4,1,1);	
						//CEdit_SetBackColor(YELLOWCOLOR);
						CEdit_UpdateData(TRUE);
						
						return;			
					
					default:break;
				}
				
				CEdit_StringIn( str );
		
				{
					CEdit_OnKey( key, tkey );
					
					g_bEditData=TRUE; 
					 
					if(!CTeach_DataOk())
					{
						break;
					}	
						
					row=CList_CurRow()+CList_StartRow();
					
					
					if( row >= Line_Data_Size -1 && CList_CurCol() == CList_MaxCol()-1)//当在最后一行的最后一列按ENTER键时
					{
					
						CTeach_AddData();	
						CList_SetCurCol(2);
						
					//	CDC_LOCKSRC();
						
						if(row>=MAXLINE-1)						//在最大行了，不移动编辑框
							CTeach_Update(0,0);
						else
							CTeach_Update(1,0);
						
						UpdateCurAxis();
					//	CDC_UNLOCKSRC();
						/*
						if(g_lStepNode==0)
						{
							SetMM(2,0,TRUE);
							g_bFirstResetY=FALSE;
							DisplayXYZ(TRUE);
						}*/
						
						ptr=Line_Data+CList_CurRow()+CList_StartRow();
						
						if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
						{
							return;
						}

						WriteDataToFile( g_Ini.iFileName );
						
						break;
					}
					else if( CList_CurCol() == 4 )					//当在送线轴编辑框时按ENTER键时 该系统没有送线轴
					{
						if(g_Ini.iAxisRunMode[1] == 3 && g_iWorkStatus==NORUN && g_lStepNode==0)
						{
							SetMM(2,0,TRUE);
							//g_bFirstResetY=FALSE;
							DisplayXYZ(TRUE);
						}
						
					}
					
					if( CList_CurCol() == CList_MaxCol()-1 )		//当在最后一列时按ENTER键时的处理
					{

					//	CDC_LOCKSRC();				
						CTeach_Update(1,-CList_CurCol());		
						UpdateCurAxis();		
					//	CDC_UNLOCKSRC();

						WriteDataToFile( g_Ini.iFileName );

						break;
					}
					
					
					if( CList_CurCol() == (g_Ini.iAxisCount>5?8:(g_Ini.iAxisCount>4?7:(g_Ini.iAxisCount>3?6:(g_Ini.iAxisCount>2?5:4)))) && row >= Line_Data_Size -1)	//当在最后一行的转心轴编辑框按ENTER时
					{

						CTeach_AddData();
						CList_SetCurCol(2);
						
					//	CDC_LOCKSRC();
						if(row>=MAXLINE-1)						//在最大行了，不向下一行移动编辑框
							CTeach_Update(0,1);
						else
							CTeach_Update(1,0);
							
						UpdateCurAxis();
					//	CDC_UNLOCKSRC();
						
						if(g_lStepNode==0)
						{
							SetMM(2,0,TRUE);
							g_bFirstResetY=FALSE;
							DisplayXYZ(TRUE);
						}
						
						ptr=Line_Data+CList_CurRow()+CList_StartRow();
						
						if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
						{
							return;
						}
						

						
						WriteDataToFile( g_Ini.iFileName );

						break;
					}
				    else if(CList_CurCol() == (g_Ini.iAxisCount>5?8:(g_Ini.iAxisCount>4?7:(g_Ini.iAxisCount>3?6:(g_Ini.iAxisCount>2?5:4)))))						//当在转心轴编辑框按ENTER时								
					{
					//	CDC_LOCKSRC();	
						CTeach_Update(1,1-CList_CurCol());
						UpdateCurAxis();	
					//	CDC_UNLOCKSRC();
						
						WriteDataToFile( g_Ini.iFileName );
						
						break;
					}
									
				//	CDC_LOCKSRC();
					CTeach_Update(0,1);
					UpdateCurAxis();	
				//	CDC_UNLOCKSRC();
					
					WriteDataToFile( g_Ini.iFileName );
				}
			}	
		} break;
		
		
	
			
		case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8':
		case TZERO:												//0.-清楚键输入
		case '.':												//'.'   835B设定G定点送线
		case '9':
			if(g_bCylinder)
			{			
				INT16S temp =-1;
				static char disp[0x03]="\0";
        		static int  dispjs=0;
        		
        		if(key ==TZERO)
        		 key='0';
        	
        		if(key >='1' && key <='9'&& dispjs==0  )
			    {
				    temp= key-'1';
				}
        		else
        		{
        		    disp[dispjs++]=key;
        		    dispjs=dispjs%2;
        		   	if(strcmp(disp,".0")==0) temp=9;
	        		if(strcmp(disp,".1")==0) temp=10;
	        		if(strcmp(disp,".2")==0) temp=11;
	        		if(strcmp(disp,".3")==0) temp=12;
	        		if(strcmp(disp,".4")==0) temp=13;
	        		if(strcmp(disp,".5")==0) temp=14;
	        		if(strcmp(disp,".6")==0) temp=15;
	        	
	        		if(dispjs==0)memset(disp,NULL,3);
	        		 
        		}

        	    if(temp>=0 && temp <=15)
        	    {                
	        	    WriteBit(temp,GetOut(temp)?0:1);
	        	    
					if(GetOut(temp))
						sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT ON":"%1d号汽缸伸出",temp+1);
					else
						sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT OFF":"%1d号汽缸退回",temp+1);
				}	
				
				OSQPost(MsgQueue,g_pszMsgBuf);
					
				break;
			}													//转到下一个CASE

	case TMINUS: case TCLEAR:
			
			g_bInsertNum = TRUE;
			
			CEdit_OnKey( key, tkey );
			
			if(g_iCurEditNo == 9)
			{
				g_bEditData=TRUE;
			}
			break;
			
			
		case TX:
			if(CList_CurCol()!=2&&CList_CurCol()!=3)
				CTeach_Update(0,2-CList_CurCol());
				
			g_iWorkNumber = 1;
			UpdateCurAxis();
			break;
			
		case TY:
			if(CList_CurCol()!=4)
				CTeach_Update(0,4-CList_CurCol());
			
			g_iWorkNumber = 2;
			UpdateCurAxis();
			break;
			
		case TZ:
			if(g_Ini.iAxisCount<3)
				break;
				
			if(CList_CurCol()!=5)
				CTeach_Update(0,5-CList_CurCol());
			
			g_iWorkNumber = 3;
			UpdateCurAxis();
			break;
			
		case TU:
			if(g_Ini.iAxisCount<4)
				break;
				
			if(CList_CurCol()!=6)
				CTeach_Update(0,6-CList_CurCol());
			
			g_iWorkNumber = 4;
			UpdateCurAxis();
			break;
				
			
		case TPAGEDOWN:											//下页                 	835B的线架加速
			{
				CTeach_UpdatePage(ROWS);
			}
			break;
			
		case TPAGEUP:											//上页
		    {
		        CTeach_UpdatePage(-ROWS);
		    }
		    break;

			
		case THANDBOX:											//手持盒  				TSTEP
			{
				if(g_bCylinder && !g_bRelease)
				{
					g_bCylinder = FALSE;
					LedOut(CYLINDERLED,0);
					
					g_Ini.iColorStyle++;
					
					if(g_Ini.iColorStyle>2)
						g_Ini.iColorStyle=0;
					
					if(g_Ini.iColorStyle==0)	
						OSQPost(MsgQueue,g_bEnglish?"Change default color mode,Please restart!":"切换到默认颜色样式,重启生效!");
					else if(g_Ini.iColorStyle==1)
						OSQPost(MsgQueue,g_bEnglish?"Change Frist color mode,Please restart!":"切换到颜色样式一,重启生效!");
					else if(g_Ini.iColorStyle==2)
						OSQPost(MsgQueue,g_bEnglish?"Change Second color mode,Please restart!":"切换到颜色样式二,重启生效!");
				
					SysParaWrite();
					break;
				}
				
				if( CheckWarning())
				{
					break;
				}
				
				g_bHandset=(g_bHandset?FALSE:TRUE);
				LedOut(BOXLED,g_bHandset?1:0);

				if (g_bHandset)
				{
					g_Ini.iHandStatus=0;
					LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
					SysParaWrite();
				}
				
				if(g_bHandset)
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOn   ":"手盒开",g_TextColor11,g_nBk,KZK_22);
				else
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"BoxOff  ":"手盒关",g_TextColor5,g_nBk,KZK_22);
			
					
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);
				
			}
			break;
		
		case THANDSWITCH:										//各轴手轮		
			{
			
				if( CheckWarning())
				{
					break;
				}
			
				g_Ini.iHandStatus=g_Ini.iHandStatus==1?0:1;
				LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);

				if(g_Ini.iHandStatus) 
				{
					g_bHandset = FALSE;
					LedOut(BOXLED,g_bHandset?1:0);
				}
		
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);
				
				if(g_Ini.iHandStatus)
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOn ":"手轮开",g_TextColor11,g_nBk,KZK_22);
				else
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff":"手轮关",g_TextColor5,g_nBk,KZK_22);
				
				//if(g_bEnglish)
				//	OSQPost(MsgQueue,g_Ini.iHandStatus==1?"HandOn ":"HandOff");
				//else
				//	OSQPost(MsgQueue,g_Ini.iHandStatus==1?"各轴手轮开":"各轴手轮关");
				
				SysParaWrite();
			}
			break;
		
		default: 
			break;
	}
}



BOOL	CTeach_BackZero(int ch)
{
	BOOL  tmpflag=FALSE;
	
	SetQuiteRange(8000);
	
	
	if( ch==2 && g_Ini.iAxisRunMode[1]==3 )//送线轴
	{
		g_bFirstResetY = TRUE;
		SetMM(2,0,TRUE);
	//	Uart_Printf("backzero1\n");
		return TRUE;
	}
	else
	{
		if(ReadBit(STOPIO)==g_Ini.StopLevel || ReadBit(STOPIO2)==0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!请抬起急停按钮!");
			OSTimeDly(100);
			return FALSE;
		}
		
		Start(ch);
		g_bBackZero=TRUE;
		g_iErrorNo=0;
		
		switch(ch)
		{
			case 1:	
				OSQPost(MsgQueue,g_bEnglish?"X Reset...":"X归零...");
				
				if(!g_bFirstResetX)
				{
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
				
			case 2:	
				OSQPost(MsgQueue,g_bEnglish?"Y Reset...":"Y归零...");
				
				if(!g_bFirstResetY)
				{//机械零点
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
				
				
					
			case 3: 
				OSQPost(MsgQueue,g_bEnglish?"Z Reset...":"Z归零...");
				
				if(!g_bFirstResetZ)
				{
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
						
			case 4: 
				OSQPost(MsgQueue,g_bEnglish?"A Reset...":"A归零...");
				
				if(!g_bFirstResetU)
				{
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
				
			case 5: 
				OSQPost(MsgQueue,g_bEnglish?"B Reset...":"B归零...");
				
				if(!g_bFirstResetV)
				{
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
				
			case 6: 
				OSQPost(MsgQueue,g_bEnglish?"C Reset...":"C归零...");
				
				if(!g_bFirstResetW)
				{
					if( BackZero(ch,g_Ini.iBackDir[ch-1]))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				else
				{
					if(FindZero(ch,g_Ini.iAxisRunMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
						tmpflag=TRUE;
					else
						tmpflag=FALSE;
				}
				break;
			
			default:
				break;
		}
		
		g_bBackZero=FALSE;
	}
	
	if(tmpflag)
	{		
	//	SetMM(2,0,TRUE);
	//	g_bFirstResetY=TRUE;
	
		SetMM(ch,0,TRUE);
		
		switch(ch)
		{
			case 1:
				g_bFirstResetX = TRUE;	
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"X归零结束!");
				break;
				
			case 2:
				g_bFirstResetY = TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"Y归零结束!");
				break;
				
				   
			case 3:
				g_bFirstResetZ = TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"Z归零结束!");
				break;
					
			case 4:
				g_bFirstResetU = TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"A归零结束!");
				break;
				
			case 5:
				g_bFirstResetV = TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"B归零结束!");
				break;
					
			case 6:
				g_bFirstResetW = TRUE;
				OSQPost(MsgQueue,g_bEnglish?"Finish!":"C归零结束!");
				break;
			
			default:
				break;
		}
		
		g_lYCountLen = 0;
		DisplayXYZ(TRUE);
		
		if( Line_Data_Size < 1 )
		{
			g_lStepNode = 0;
			return TRUE;
		}		
		
		g_lStepNode = 0;
		return TRUE;
	}
	else
	{
		switch(ch)
		{
			case 1: 
				g_bFirstResetX = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"X归零故障,确认原点信号是否正常!");
				break;
				
				
			case 2:
				g_bFirstResetY = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Y归零故障,确认原点信号是否正常!");
				break;
					
			case 3:
				g_bFirstResetZ = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Z归零故障,确认原点信号是否正常!");
				break;
					
			case 4:
				g_bFirstResetU = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"A归零故障,确认原点信号是否正常!");
				break;
			
			case 5:
				g_bFirstResetV = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"B归零故障,确认原点信号是否正常!");
				break;
					
			case 6:
				g_bFirstResetW = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"C归零故障,确认原点信号是否正常!");
				break; 
				
			default:
				break;
		}
		
		CheckWarning();
		CheckLimit(FALSE,TRUE,0,0);
				
		return FALSE;
	}
}



BOOL	CTeach_ProcessError( COMMAND *ptr, INT16S col ,INT8S *string)
{
	switch( ptr->value[0][0] )
	{
	case 'S':
		 {
			if( (col == 3 || (col>= CList_MaxCol()-3 && col <= CList_MaxCol()-1)) && string[0] )
			{
				OSQPost(MsgQueue,g_bEnglish?"Settings of speed is invalid":"速度数据设定无效");				
				return FALSE;
			}
			
			if( col == 2 )
			{
				if( string[0] )
				{
					FP32 n = (FP32)stringtof((INT8U *)string);
					
					if( n >g_Ini.fXMaxSpeed )
					{
						OSQPost(MsgQueue,g_bEnglish?"Speed of X is too large":"X速度设定太大");
						return FALSE;
					}
					else  if(n < 0.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"Speed of X is too little":"X速度设定太小");
						return FALSE;
					}
				}
			}
			else if(col>3 && col <CList_MaxCol()-3)
			{
				if( string[0] )
				{
					FP32 n = (FP32)stringtof((INT8U *)string);
					
					if(n < 0.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"Speed is too little":"速度设定太小");
						return FALSE;
					}
				}
			}
		}
		
		break;
		
	case 'M':
		{
			//FP32 n = atof( string );
			FP32 n = (FP32)stringtof( (INT8U *)string );
			
			if( col == 2 || col == 3 )
			{
				if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
				{
					if(ptr->value[9][0]!=NULL)
					{
						OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
						return FALSE;
					}
				}
				
				if(g_Ini.iAxisCa==0)
				{
					if( n>360.001 || n < -360.001) 
					{
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");					
						return FALSE;
					}
				}
				else
				{
					if( n>3600.01 || n < -3600.01) 
					{
						OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");					
						return FALSE;
					}
				}
			}
			
			if( col == 4 &&string[0]!=NULL)
			{   
			
				if(g_Ini.iAxisRunMode[1]==0 || g_Ini.iAxisRunMode[1]==1)
				{
					if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
					{		
						if(ptr->value[9][0]!=NULL)
						{
							OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
							return FALSE;
						}
					}
				}	
				
				
				
				if(g_Ini.iAxisRunMode[1]==1)
				{
					INT16S 	tempi;
					FP32 	tempf;
					
					tempi=(INT16S)fabs(n);
					tempf=fabs(n)-tempi;
					
					

					if(tempf>0.360)
					{
						OSQPost(MsgQueue,g_bEnglish?"Data Set Invalid!":"数据设定错误,小数点后数据必须<0.3600!");
						return FALSE;
					}
					
					if((n<-100.01 || n > 100.01))
					{
						OSQPost(MsgQueue,g_bEnglish?"-100<Y Cir<100!":"-100<卷线圈数<100");
						return FALSE;
					}
				}
				else if(g_Ini.iAxisRunMode[1]==0)
				{
				
					if(g_Ini.iAxisCa==0)
					{
						if( n>360.001 || n < -360.001) 
						{
							OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");						
							return FALSE;
						}
					}
					else
					{
						if( n>3600.01 || n < -3600.01)
						{
							OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");						
							return FALSE;
						}
					}				
				}
				else if(g_Ini.iAxisRunMode[1]==2)    //20141127  xxl
				{
					if(strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S')
					{
						OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Screw Col!":"丝杆轴不能使用加工中回零");						
						return FALSE;
					}
				}	
			
				if(g_Ini.iAxisRunMode[1]==3)
				{
					if(  (strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S'))
					{
						OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Col Y-axis!":"送线轴不能使用加工中回零");						
						return FALSE;
					}
					
					
					if(g_Ini.iAxisCa==0)
					{
						if(n<-9999.99 || n > 19999.99)  
						{
							OSQPost(MsgQueue,g_bEnglish?"-9999.99<Y<19999.99":"-9999.99<送线长度<19999.99!");					
							return FALSE;
						}
					}
					else 
					{
						if(n<-999999 || n > 1999999)  
						{
							OSQPost(MsgQueue,g_bEnglish?"-999999<Y<1999999":"-999999<送线长度<1999999!");					
							return FALSE;
						}
					}
				}
			}
			
			
			if(g_Ini.iAxisCount>=3)
			{
				if(col==5)
				{
					if(g_Ini.iAxisRunMode[2]==0 || g_Ini.iAxisRunMode[2]==1)
					{
						if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
						{		
							if(ptr->value[9][0]!=NULL)
							{
								OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
								return FALSE;
							}
						}
					}
						
					if(g_Ini.iAxisRunMode[2]==1)
					{
						INT16S 	tempi;
						FP32 	tempf;
						
						tempi=(INT16S)fabs(n);
						tempf=fabs(n)-tempi;
						
						
	
						if(tempf>0.360)
						{
							OSQPost(MsgQueue,g_bEnglish?"Data Set Invalid!":"数据设定错误,小数点后数据必须<0.3600!");
							return FALSE;
						}
						
						if((n<-100.01 || n > 100.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"-100<Y Cir<100!":"-100<卷线圈数<100");
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[2]==0)
					{
					
						if(g_Ini.iAxisCa==0)
						{
							if( n>360.001 || n < -360.001) 
							{
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");						
								return FALSE;
							}
						}
						else
						{
							if( n>3600.01 || n < -3600.01)
							{
								OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");						
								return FALSE;
							}
						}				
					}
					else if(g_Ini.iAxisRunMode[2]==2)    //20141127  xxl
					{
						if(strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S')
						{
							OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Screw Col!":"丝杆轴不能使用加工中回零");						
							return FALSE;
						}
					}				
				}
			}
			
			if(g_Ini.iAxisCount>=4)
			{	
				if(col==6)
				{
				
					if(g_Ini.iAxisRunMode[3]==0 || g_Ini.iAxisRunMode[3]==1)
					{
						if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
						{
							if(ptr->value[9][0]!=NULL)
							{
								OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
								return FALSE;
							}
						}
					}
						
					if(g_Ini.iAxisRunMode[3]==1)
					{
						INT16S 	tempi;
						FP32 	tempf;
						
						tempi=(INT16S)fabs(n);
						tempf=fabs(n)-tempi;			
					
						if(tempf>0.360)
						{
							OSQPost(MsgQueue,g_bEnglish?"Data Set Invalid!":"数据设定错误,小数点后数据必须<0.3600!");
							return FALSE;
						}
						
						if((n<-100.01 || n > 100.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"-100<Circles<100!":"-100<卷线圈数<100");
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[3]==0)
					{
					
						if(g_Ini.iAxisCa==0)
						{
							if( n>360.001 || n < -360.001) 
							{
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");						
								return FALSE;
							}
						}
						else
						{
							if( n>3600.01 || n < -3600.01) 
							{
								OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");						
								return FALSE;
							}
						}
					}
					else if(g_Ini.iAxisRunMode[3]==2)  
					{
						if(strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S')
						{
							OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Screw Col!":"丝杆轴不能使用加工中回零");						
							return FALSE;
						}
					}
				}
					
			}
		
			if(g_Ini.iAxisCount>=5)
			{	
				if(col==7)
				{
					if(g_Ini.iAxisRunMode[4]==0 || g_Ini.iAxisRunMode[4]==1)
					{
						if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
						{
							if(ptr->value[9][0]!=NULL)
							{
								OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
								return FALSE;
							}
						}
					}
						
					if(g_Ini.iAxisRunMode[4]==1)
					{
						INT16S 	tempi;
						FP32 	tempf;
						
						tempi=(INT16S)fabs(n);
						tempf=fabs(n)-tempi;			
					
						if(tempf>0.360)
						{
							OSQPost(MsgQueue,g_bEnglish?"Data Set Invalid!":"数据设定错误,小数点后数据必须<0.3600!");
							return FALSE;
						}
						
						if((n<-100.01 || n > 100.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"-100<Circles<100!":"-100<卷线圈数<100");
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[4]==0)
					{
						if( n>3600.01 || n < -3600.01) 
						{
							OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");						
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[4]==2)  
					{
						if(strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S')
						{
							OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Screw Col!":"丝杆轴不能使用加工中回零");						
							return FALSE;
						}
					}
				}
			}
			
			
			if(g_Ini.iAxisCount>=6)
			{
					
				if(col==8)
				{
					if(g_Ini.iAxisRunMode[5]==0 || g_Ini.iAxisRunMode[5]==1)
					{
						if(strcmp(string,"+")==0 || strcmp(string,"-")==0)
						{
							if(ptr->value[9][0]!=NULL)
							{
								OSQPost(MsgQueue,g_bEnglish?"Can't Insert ZeroMode \"+\" or \"-\" in Probe Row!":"在探针行中不可插入回零归式\"+\"或\"-\"");					
								return FALSE;
							}
						}
					}
						
					if(g_Ini.iAxisRunMode[5]==1)
					{
						INT16S 	tempi;
						FP32 	tempf;
						
						tempi=(INT16S)fabs(n);
						tempf=fabs(n)-tempi;			
					
						if(tempf>0.360)
						{
							OSQPost(MsgQueue,g_bEnglish?"Data Set Invalid!":"数据设定错误,小数点后数据必须<0.3600!");
							return FALSE;
						}
						
						if((n<-100.01 || n > 100.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"-100<Circles<100!":"-100<卷线圈数<100");
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[5]==0)
					{
						if( n>3600.01 || n < -3600.01) 
						{
							OSQPost(MsgQueue,g_bEnglish?"3600>=angle>=-3600":"3600>=角度>=-3600");						
							return FALSE;
						}
					}
					else if(g_Ini.iAxisRunMode[5]==2)  
					{
						if(strcmp("+",string)== 0 || strcmp("-",string)== 0 || string[0]=='S')
						{
							OSQPost(MsgQueue,g_bEnglish?"Can't Use ZeroMode \"+\" or \"-\" in Screw Col!":"丝杆轴不能使用加工中回零");						
							return FALSE;
						}
					}
				}
			}
			
			
			if(col==CList_MaxCol()-4 && string[0]!=NULL)
			{
				if( n>5.001 || n < 0.0099)
				{
					OSQPost(MsgQueue,g_bEnglish?"5.0>=Ratio>=0.01":"5.0>=速度比例>=0.01");
					return FALSE;
				}
			}
						
			if(col==CList_MaxCol()-3)   //探针
			{
				INT16S k=0;
				
			//	if(g_Ini.iPaoXian&&n>7.01)
				if(g_Ini.iPaoXian&&n>99.01)
				{
					OSQPost(MsgQueue,g_bEnglish?"probe <=7 if Exceed Wire Check.":"有跑线检测时,探针设定<=7,延时无效时1-99用于标志");
					return FALSE;
				}
				else
				{
				//	if(!g_Ini.iPaoXian&&n>8.01)
					if(!g_Ini.iPaoXian&&n>99.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"Probe <=8":"探针设定<=8,延时无效时1-99用于标志");
						return FALSE;
					}
				}					
				
				for(k=1;k<col;k++)					//有"+"或"-"回零时不能设置探针。
				{				
					if((strcmp(ptr->value[k],"+")==0 || strcmp(ptr->value[k],"-")==0) && string[0]!=NULL)
					{
						OSQPost(MsgQueue,g_bEnglish?"Can't Use Probe in Row with ZeroMode \"+\" or \"-\"!":"有回零归式\"+\"或\"-\"的程式行不可插入探针");
						return FALSE;
					}
				}
			}
			
			if(col==CList_MaxCol()-2)//气缸
			{
				INT16S i=0;
				
				if(g_Ini.iCyldMode==0)
				{	
					for(i=0;i<strlen(string);i++)
					{
						if((string[i]<'0'||string[i]>'1') && string[i]!=NULL)
						{
							OSQPost(MsgQueue,g_bEnglish?"Data Invalid,Make Sure it's 01 Sequence.":"气缸设定无效,确保为01序列");						
							return FALSE;
						}
					}
				}
				else
				{
					if(n<-16.001||n>16.001)
					{
						OSQPost(MsgQueue,g_bEnglish?"Data Invalid,-16 ~ +16.":"气缸范围,-16~+16");						
						return FALSE;
					}
					/*
					for(i=0;i<strlen(string);i++)
					{

						if(string[i]==string[i+1])
						{
							OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"汽缸设定无效,格式错误");						
							return FALSE;
						}
						
						if(string[i] != '.' && string[i] != '-' && (string[i]<'0'|| string[i]>MAXCYLINDER+'0'))
						{
							OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"汽缸设定无效,超界或设定字符错误");						
							return FALSE;
						}
					}*/

				}		
			}
			
			if(col==CList_MaxCol()-1 && string[0]!=NULL )
			{
				if( n>10.000 || n<0.01)
				{
					OSQPost(MsgQueue,g_bEnglish?"10.0>Delay>0.01":"10.0>=延时>=0.01");
					return FALSE;
				}
			}
			
		}
		break;
		
	case 'L':case 'J':case 'G':
		{
			INT16S n = stringtoi( (INT8U *)string );
			
			if( col == 2 )
			{
				if( n < 1 || n > 1000 )
				{
					OSQPost(MsgQueue,g_bEnglish?"1000>=value>=1":"1000>=数据>=1");					
					return FALSE;
				}
			}
			else if( col == 4)
			{
			
			}
			else if( string[0] )
			{
				OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"数据无效");				
				return FALSE;
			}
		} break;
		
	case 'N':case 'E':
		if( string[0] )
		{
			OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"数据无效");			
			return FALSE;
		} break;
	}
	
	OSQPost(MsgQueue," ");
	return TRUE;
}


void	CTeach_ParaSelect()
{
	INT16U key,len;
	INT8S  a=0;
	BOOL   IsCreate=FALSE;
	char   buf[0x20]="\0";	
	
	len = CEdit_GetLen();															//记录当前编辑框的可输入字符长度

	//for(a=5; a<8; a++)
	SuspendTasks(7);
		
	SuspendTasks(0);
	
	OSTimeDly(80);
	
	CDC_SaveScreen();
	
	CDC_PutColorBar(3,122,400,158,g_nTableBk,1);
	CDC_Rectangle(3,122,400,280,g_TextColor5);
	//CDC_Rectangle(4,90,399,381,g_TextColor1);

	if(g_bEnglish)
	{
		CDC_OutText24(40,130,"0---File Manage",g_TextColor11,g_nTableBk);
		CDC_OutText24(40,160,"1---System Parameter",g_TextColor11,g_nTableBk);		
		CDC_OutText24(40,190,"2---Input/Output Check",g_TextColor11,g_nTableBk);
		CDC_OutText24(40,220,"3---Key Check",g_TextColor11,g_nTableBk);
		CDC_OutText24(40,250,"4---Date&Time Settings",g_TextColor11,g_nTableBk);	
	}
	else
	{
		CDC_OutText_UTF(40,130,"0---加工数据文件管理",g_TextColor11,g_nTableBk,KZK_24);
		CDC_OutText_UTF(40,160,"1---更改系统参数",g_TextColor11,g_nTableBk,KZK_24);		
		CDC_OutText_UTF(40,190,"2---进入输入输出检测界面",g_TextColor11,g_nTableBk,KZK_24);
		CDC_OutText_UTF(40,220,"3---进入按键检测界面",g_TextColor11,g_nTableBk,KZK_24);
		CDC_OutText_UTF(40,250,"4---日期时间设定",g_TextColor11,g_nTableBk,KZK_24);	
	}

	while(TRUE)
	{
		OSTimeDly(10);
		
		key = KeyMBoxGetKey();
	
		if(key != 0xffff)
		{
			switch(key)
			{
				#ifdef PRINTBMP
				case F5:case TDELALL:
					{
						char   filename[0x20];
						INT16S i=0;
						
//						CBmp_Init();
						strcpy(filename,"SysMnge");
						
						for(i=0; i<strlen(filename); i++)
						{
							if(filename[i]=='.')
							{
								filename[i]='\0';
									break;
							}
						}
						
						strcat(filename,".bmp");
						CreatBMP(filename,800,600,1);
//						CBmp_Destroy();
						OSQPost(MsgQueue,"Write Bmp End!");
					}
					break;
				#endif
				case TZERO: 
					{
					//	SuspendTasks(0);			//0和7以在前面挂起
						SuspendTasks(5);
						SuspendTasks(6);
					//	SuspendTasks(7);					
					
						CPassword_Init(240,140,340,180,g_bEnglish?"Password ":"文件密码验证",buf,g_TextColor2);
						
						if(CPassword_DoModal())
						{
							INT32S pass;
						
							pass=CPassword_GetPassword();
							CPassword_Destroy();
						
							if(pass == g_Ini.lPassword || pass == 790909L)
							{
								WriteDataToFile(g_Ini.iFileName);
								
								CDispDir_Init(0,0,799,599,-1);
								CDispDir_DoModal();					
								CDispDir_Destroy();
								
								ReadDataFromFile(g_Ini.iFileName);	//当前文件可能被覆盖,内容变化,读取出来
								
								g_bModify = TRUE;											//重新解析
								IsCreate = TRUE;	
							}
						}
						else
						{
							CPassword_Destroy();
						}
								
	          		//	ResumeTasks(0);
	         			ResumeTasks(5);
	         			ResumeTasks(6);
	         		//	ResumeTasks(7); 
					}
					break;
					
				case '1':
					{
						CPassword_Init(200,140,340,180,g_bEnglish?"Password ":"系统密码验证",buf,g_TextColor2);
						
						if(CPassword_DoModal())
						{
							INT32S pass;
							INT16S iLanguage;
							INT16S iAxis;
							INT16S iNameNo[6];
							INT16S iLeftOrRight;
							INT16S iMacType;
							
							iMacType = g_Ini.iMachineType;
							iLanguage = g_Ini.iLanguage;
							iAxis = g_Ini.iAxisCount;
							iLeftOrRight = g_Ini.iLeftOrRight;
							iNameNo[0]=g_Ini.iAxisNameNo[0];
							iNameNo[1]=g_Ini.iAxisNameNo[1];
							iNameNo[2]=g_Ini.iAxisNameNo[2];
							iNameNo[3]=g_Ini.iAxisNameNo[3];
							iNameNo[4]=g_Ini.iAxisNameNo[4];
							iNameNo[5]=g_Ini.iAxisNameNo[5];
							
							pass=CPassword_GetPassword();
							CPassword_Destroy();
						
							if(pass == g_Ini.lPassword || pass == 790909L)
							{
								g_iCurEditNo = 4;
								
								CSysPara_Init(0,0,799,599);
								
								SuspendTasks(5);
								
								
								if(CSysPara_DoModal())
								{  
									if(iLanguage != g_Ini.iLanguage || iAxis != g_Ini.iAxisCount || g_bSysReadBackup
									|| iNameNo[0]!=g_Ini.iAxisNameNo[0] || iNameNo[1]!=g_Ini.iAxisNameNo[1] 
									|| iNameNo[2]!=g_Ini.iAxisNameNo[2] || iNameNo[3]!=g_Ini.iAxisNameNo[3]
									|| iNameNo[4]!=g_Ini.iAxisNameNo[4] || iNameNo[5]!=g_Ini.iAxisNameNo[5]
									|| iLeftOrRight != g_Ini.iLeftOrRight || iMacType!=g_Ini.iMachineType
									)
									{
										g_bEnglish=g_Ini.iLanguage==1?TRUE:FALSE;
										IsCreate = TRUE;
									}
									
									g_bFirstResetX=FALSE;
									g_bFirstResetY=FALSE;
									g_bFirstResetZ=FALSE;
									g_bFirstResetU=FALSE;
									g_bFirstResetV=FALSE;
									g_bFirstResetW=FALSE;
									
									if(g_bMode)
									{
										g_bMode=FALSE;
										LedOut(TESTLED,g_bMode?1:0);
									}
									
									{
										INT32S t1,t2;
									
										OSSemPost(UnpackDataSem);
										g_bUnpackCorrect = FALSE;
										
										t1 = OSTimeGet();
										
										while(TRUE)
										{
											OSTimeDly(2);
											
											if(g_bUnpackCorrect)
												break;
												
											t2 = OSTimeGet();
											
											if((t2-t1)>200 || (t2-t1<-200))
												break;
										}
									}	
								}
								
								ResumeTasks(5);
								
								g_iCurEditNo=9;
								
								CSysPara_Destroy();
								OSQPost(MsgQueue," ");
							}
						}
						else
						{
							CPassword_Destroy();
						}
					}
					break;
				case '2':
				{

					g_bHandset=FALSE;						
					
					LedOut(HANDLED,0);	
					OSQPost(MsgQueue,g_bEnglish?"Enter Spring(Input/Output/Key)Check...":"进入输入输出诊断界面...");
			
					g_bInterface = FALSE;
						
					CTestFrm_Init();
					CTestFrm_DoModal();
					CTestFrm_Destroy();
					
					OSTimeDly(40);
					
					g_iCurEditNo=9;
					
					CEdit_SetTextColor(g_nTableBk);
					CEdit_SetBackColor(g_TextColor6);
					
					g_bFirstResetX=FALSE;
					g_bFirstResetY=FALSE;							
					g_bFirstResetZ=FALSE;								
					g_bFirstResetU=FALSE;
					g_bFirstResetV=FALSE;								
					g_bFirstResetW=FALSE;

				}
				break;			
				case '3':
					{
						CTestKey_Init();
						CTestKey_DoModal();
						CTestKey_Destroy();
						
						OSTimeDly(10);
						
						OSQPost(MsgQueue," ");
					}
					break;
					

				case '4':
			   		{
						CSetTime_Init(160,80,480,300);
						CSetTime_DoModal();
						CSetTime_Destroy();
					}
					break;
					
				case '9':														//用户级系统管理密码设定
			   		{
			   			
			   			for(a=2;a<5;a++)
								SuspendTasks(a);
						
						OSTimeDly(5);
			   			
						sprintf(buf,g_bEnglish?"\nUser MachCode:%d\n":"\n  用户级机器编号:%d\n",NewDog.User_ComputerNo);
						CPassword_Init(200,140,400,220,g_bEnglish?"User System Pwd":"用户级系统管理密码验证",buf,g_TextColor7);
						
						if(CPassword_DoModal())
						{
							INT32S pass=0;
							INT32S password=0;
							INT32S	isetl = 0;					
							INT32S	iseth = 0;					
							
							pass = CPassword_GetPassword();
							
							password = pass;
							
							isetl = password & 0x3ff;
							isetl = isetl<<10;
							
							iseth = password>>10;
							iseth = iseth & 0x3ff;
							
							password =password>>20;
							password =password<<20;
							
							password = password | isetl | iseth;
							
							password = password - 130509;
							
							CPassword_Destroy();
							
							if(pass == NewDog.User_Password || password ==NewDog.User_ComputerNo ||pass==116001)
							{
								CUserDog_Init(180,100,420,270);
								CUserDog_DoModal();
								CUserDog_Destroy();
							}
						}
						else
						{
							CPassword_Destroy();
						}
						
						g_bFirstResetX=FALSE;
						g_bFirstResetY=FALSE;							
						g_bFirstResetZ=FALSE;								
						g_bFirstResetU=FALSE;	
						g_bFirstResetV=FALSE;								
						g_bFirstResetW=FALSE;
						
						for(a=2;a<5;a++)
							ResumeTasks(a);
						
						OSTimeDly(5);
							   		
					}
					break;
					
				case '8':  														//公司级系统管理密码设定
					{}
					break;
				
				default:
					break;
			}																	//end switch
			
			CEdit_SetNeedPassword(FALSE);

			CDC_RestoreScreen();
			
			if(IsCreate)
			{
				g_bSysReadBackup = FALSE;
				CTeach_Init();
				CTeach_Create();
				
				DisplayXYZ(FALSE);
			}
				
			break; 
			
		}																		//end if(key!=0xffff)
	}																			//end while(TRUE);
	
	g_bInterface = TRUE;
	
	//for(a=5;a<8;a++)
		ResumeTasks(7);
		
	ResumeTasks(0);
	
	if(!g_bScreen)
		CTeach_Update(0,0);
		
	CEdit_SetLen(len);
	
	g_bCylinder=FALSE;
	LedOut(CYLINDERLED,g_bCylinder?1:0);
	
	SetMM(7,0,FALSE);							//清零手轮手盒脉冲，不然在"各轴手轮"开状态下进入这些界面并有摇手轮动作，退出时候选中的轴会移动。
	SetMM(8,0,FALSE);	
}


void	CTeach_UpdateSpeed(int speed )
{
	g_iSpeedBeilv += speed;
	
	if( g_iSpeedBeilv > 3 ) 
		g_iSpeedBeilv = 1;	
	
	switch(g_iSpeedBeilv)
	{
		case 1:
			CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Low":"低速",g_TextColor5,g_nBk,KZK_22);
			break;
			
		case 2:
			CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Mid":"中速",g_TextColor5,g_nBk,KZK_22);
			break;
			
		case 3:
			CDC_OutText_UTF(110,getmaxy()-58,g_bEnglish?" Hgh":"高速",g_TextColor5,g_nBk,KZK_22);

			break;
			
		default:
			break;
	}
}

/*
BOOL	CTeach_DataOk()
{
	INT16S row;
	COMMAND *ptr;
	COMMAND *ptr1;
	//INT16S  len = -1;
	BOOL bSingleChk=FALSE;			//转芯、卷曲单圈模式时上下行数据相同时，下行数据不显示
	
	char str[0x10]="\0";
//	char buf[0x10]="\0";
	
	row = CList_CurRow()+CList_StartRow();
	ptr = Line_Data+row;
	CEdit_String( str );
	
	{
	
		if( !CTeach_ProcessError( ptr, CList_CurCol(), str) )
			return FALSE;	
		
		switch(g_Ini.iAxisCount)
		{
		case 2:
			if(CList_CurCol()>=CList_MaxCol()-3)
				CEdit_String( ptr->value[CList_CurCol()+3] );
			else
				CEdit_String( ptr->value[CList_CurCol()-1] );
			break;
		case 3:
			if(CList_CurCol()>=CList_MaxCol()-3)
				CEdit_String( ptr->value[CList_CurCol()+2] );
			else
				CEdit_String( ptr->value[CList_CurCol()-1] );
			break;
		case 4:
			if(CList_CurCol()>=CList_MaxCol()-3)
				CEdit_String( ptr->value[CList_CurCol()+1] );
			else
				CEdit_String( ptr->value[CList_CurCol()-1] );
			break;
			
		
		case 5:
			if(CList_CurCol()>=CList_MaxCol()-3)
				CEdit_String( ptr->value[CList_CurCol()+0] );
			else
				CEdit_String( ptr->value[CList_CurCol()-1] );
			break;
			
		case 6:
				CEdit_String( ptr->value[CList_CurCol()-1] );		
		
		default:break;
		}
	}

	
	switch(g_Ini.iAxisCount)
	{
		case 6:
			if(g_Ini.iAxisRunMode[5]==0 && CList_CurCol()==8)
				bSingleChk=TRUE;
			else
				bSingleChk=FALSE;
		case 5:
			if(g_Ini.iAxisRunMode[4]==0 && CList_CurCol()==7)
				bSingleChk|=TRUE;
			else
				bSingleChk|=FALSE;
				
		case 4:
			if(g_Ini.iAxisRunMode[3]==0 && CList_CurCol()==6)
				bSingleChk=TRUE;
			else
				bSingleChk=FALSE;
		case 3:
			if(g_Ini.iAxisRunMode[2]==0 && CList_CurCol()==5)
				bSingleChk|=TRUE;
			else
				bSingleChk|=FALSE;
				
			if(bSingleChk)
			{
				INT16S j = 0;
				INT16S col=CList_CurCol()-1;
			
				if(row>0)
				{
					for(j=row-1;j>=0;j--)
					{
						ptr1 = Line_Data+j;
						if(ptr1->value[col][0])
							break;
							
					}
					
					{
						if(strcmp(ptr1->value[col],ptr->value[col])==0 && strcmp(ptr1->value[0],"M")==0)
							strcpy(ptr->value[col],"");	
					}
				}
			}
			
			break;
		default:
			break;
	}
		
		
	
	if(g_bEditData)
	{
		g_bEditData=FALSE;
		g_bModify = TRUE; 
		g_bModifySave=FALSE;
	}
	return TRUE;
}

*/


BOOL	CTeach_DataOk()
{
	INT16S  row;
	COMMAND *ptr;
	char    str[0x10]="\0";
	
	row = CList_CurRow()+CList_StartRow();
	ptr = Line_Data+row;
	
	CEdit_String( str );
	
	if( !CTeach_ProcessError( ptr, CList_CurCol(), str) )
		return FALSE;	
	
	switch(g_Ini.iAxisCount)
	{
	case 2:
		if(CList_CurCol()>=CList_MaxCol()-4)
			CEdit_String( ptr->value[CList_CurCol()+3] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;
	case 3:
		if(CList_CurCol()>=CList_MaxCol()-4)
			CEdit_String( ptr->value[CList_CurCol()+2] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;
	case 4:
		if(CList_CurCol()>=CList_MaxCol()-4)
			CEdit_String( ptr->value[CList_CurCol()+1] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;
		
	
	case 5:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_String( ptr->value[CList_CurCol()+0] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;
	case 6:
			CEdit_String( ptr->value[CList_CurCol()-1] );		
		break;
	
	default:break;
	}
	
	if(g_bEditData)
	{
		g_bEditData=FALSE;
		g_bModify = TRUE; 
		g_bModifySave=FALSE;
	}
	
	return TRUE;
}


void  CTeach_UpdateTimes(BOOL bDis)
{
	CRect rc;
	char buffer[0x09];
	//INT16S i = 0;
	
	if(bDis)
	{
		rc.left = 152;//140;
		rc.right = 198;//176;
		rc.top = 1;
		rc.bottom = 30;
		CDC_DrawRect(rc,BLUECOLOR,WHITECOLOR);
		
		sprintf(buffer,"*%d",g_Ini.iStepRate[0]);
		CDC_OutText_UTF(154,4,buffer,GREENCOLOR,BLUECOLOR,MZK_24);
		
		rc.left = 292;//260;
		rc.right = 338;//296;
		rc.top = 1;
		rc.bottom = 30;
		CDC_DrawRect(rc,BLUECOLOR,WHITECOLOR);
		
		sprintf(buffer,"*%d",g_Ini.iStepRate[1]);
		CDC_OutText_UTF(294,4,buffer,GREENCOLOR,BLUECOLOR,MZK_24);
		
		
	  	if(g_Ini.iAxisCount	>2)
	  	{   
		  	rc.left = 432;
			rc.right = 478;
			rc.top = 1;
			rc.bottom = 30;
			CDC_DrawRect(rc,BLUECOLOR,WHITECOLOR);
			
			sprintf(buffer,"*%d",g_Ini.iStepRate[2]);
			CDC_OutText_UTF(434,4,buffer,GREENCOLOR,BLUECOLOR,MZK_24);	
		}
		
		
		if(g_Ini.iAxisCount	>3)
		{
			rc.left = 572;
			rc.right = 618;
			rc.top = 1;
			rc.bottom = 30;
			CDC_DrawRect(rc,BLUECOLOR,WHITECOLOR);
			
			sprintf(buffer,"*%d",g_Ini.iStepRate[3]);
			CDC_OutText_UTF(574,4,buffer,GREENCOLOR,BLUECOLOR,MZK_24);
		}
		
		/*for(i=0;i<g_Ini.iAxisCount;i++)
		{

			sprintf(buffer,"*%3d",g_Ini.iStepRate[i]);
			CDC_OutText(g_StepEditRectEx[i].left+2,g_StepEditRectEx[i].top,buffer,GREENCOLOR,BLUECOLOR);			
		}*/
		
		g_bUseTimes = TRUE;
		
	 }
	 else
	 {
	 	rc.left = 152;//140;
		rc.right = 198;//176;
		rc.top = 1;
		rc.bottom = 30;
		
		CDC_DrawRect(rc,g_nBk,g_nBk);
		CDC_Line(rc.left,rc.top,rc.left,rc.bottom,g_nBk);
		CDC_Line(rc.right,rc.top,rc.right,rc.bottom,g_nBk);
		
		rc.left = 292;
		rc.right = 338;//296;
		rc.top = 1;
		rc.bottom = 30;
		CDC_DrawRect(rc,g_nBk,g_nBk);
		
		CDC_Line(rc.left,rc.top,rc.left,rc.bottom,g_nBk);
		CDC_Line(rc.right,rc.top,rc.right,rc.bottom,g_nBk);
		
		/*rc.left = 499;
		rc.right = 537;
		rc.top = 1;
		rc.bottom = 26;
		CDC_DrawRect(rc,g_nBk,g_nBk);	
	  	*/	
	  	if(g_Ini.iAxisCount	>2)
	  	{   				 	  		   
		  	rc.left = 432;
			rc.right = 478;
			rc.top = 1;
			rc.bottom = 30;
			
			CDC_DrawRect(rc,g_nBk,g_nBk);	
			CDC_Line(rc.left,rc.top,rc.left,rc.bottom,g_nBk);
			CDC_Line(rc.right,rc.top,rc.right,rc.bottom,g_nBk);
		}
		
		if(g_Ini.iAxisCount	>3)
		{
			rc.left = 572;
			rc.right = 618;
			rc.top = 1;
			rc.bottom = 30;
			
			CDC_DrawRect(rc,g_nBk,g_nBk);	
			CDC_Line(rc.left,rc.top,rc.left,rc.bottom,g_nBk);
			CDC_Line(rc.right,rc.top,rc.right,rc.bottom,g_nBk);
		}
		
		g_bUseTimes = FALSE;
	 }
}


void	CTeach_UpdateStatus(INT16S status)
{
	//CRect rc;
	
	//if(status!=0 && status == g_iStatus)
	//	return;
	
	
	g_iStatus = status;
	
	switch(status)
	{
	//case 0://停止
	//	{
	//		CDC_OutText_UTF(745,33,"停止",g_TextColor5,BLUECOLOR,KZK_26);	
	//	}
	//	break;
	case 1://自动
		{
			CDC_OutText_UTF(268,getmaxy()-58,g_bEnglish?"Auto":"自动",g_TextColor11,g_nBk,KZK_22);	
		}
		break;	
	//case 2://暂停
	//	{
	//		CDC_OutText_UTF(745,33,"暂停",g_TextColor5,BLUECOLOR,KZK_26);
	//	}
	//	break;
	case 3://测试
		{
			CDC_OutText_UTF(268,getmaxy()-58,g_bEnglish?"Test":"测试",g_TextColor5,g_nBk,KZK_22);
		}
		break;
	//case 4://手轮
	//	{
	//		CDC_OutText_UTF(745,33,"手轮",g_TextColor5,BLUECOLOR,KZK_26);
	//	}
	//	break;
		
	}
}



void  CTeach_UpdateRange(INT32S NO)
{
	
	char str[0x20]="\0";
	CRect rc;

	rc.left = 408;
    rc.right = 796;
    rc.top = 537;
    rc.bottom = 565;
   
  //  CDC_PutColorBar2(rc,g_nBk,1);
    
	switch(NO)
	{
	case 0:
		{
			sprintf(str,"%s","   0   3600");
		}
		break;
	case 1:
		{
			sprintf(str,"%s","   -3600   3600");
		}
		break;
	case 2:
		{
			sprintf(str,"%s","   -99999   9999999");
		}
		break;
	case 3:
		{
			sprintf(str,"%s","   0   1");
		}
		break;
	case 4:
		{
			sprintf(str,"%s","   1   4");
		}
		break;
	case 5:
		{
			sprintf(str,"%s","     1   300          ");
		}
		break;
	case 6:
		{
			sprintf(str,"%s","     -8   8          ");
		}	
		break;
	case 7:
		{
			if(g_Ini.iAxisRunMode[3] == 1)
				sprintf(str,"%s","   -9360   9360");
			else if(g_Ini.iAxisRunMode[3] == 0)
				sprintf(str,"%s","   -3600   3600");
		}
		break;
	case 8:
		{
			if(g_Ini.iAxisRunMode[2] == 1)
				sprintf(str,"%s","   -9360   9360");
			else if(g_Ini.iAxisRunMode[2] == 0)
				sprintf(str,"%s","   -3600   3600");
		}
		break;
	default:
		{
			sprintf(str,"%s","                       ");
		}
		break;
	}

   // CDC_OutCenTextSzk24(rc,str,REDCOLOR,g_nBk);	
}


void	CTeach_DrawEditRect()
{
//	CRect rc;
	INT8S buffer[0x10];
	//INT16S i = 0;

	switch(g_iCurEditNo)
	{
	case 0: 
			{
				CDC_PutColorBar2(g_EditRect[0],g_nBk,1);
				sprintf(buffer,"%-3d ",g_Ini.iFileName);
				CDC_OutText_UTF(g_EditRect[0].left,g_EditRect[0].top,buffer,g_TextColor11,g_nBk,MZK_24);
			}
			break;
	case 2:
			{
				CDC_PutColorBar2(g_EditRect[2],g_nBk,1);
				sprintf(buffer,"%-3d ",g_Ini.lProbeCount);
				CDC_OutText_UTF(g_EditRect[2].left+2,g_EditRect[2].top,buffer,g_TextColor11,g_nBk,MZK_24);
			}
			break;
	case 1:
			{
				CDC_PutColorBar2(g_EditRect[1],g_nBk,1);
				sprintf(buffer,"%-7d ",g_Ini.lDestCount);
				CDC_OutText_UTF(g_EditRect[1].left+2,g_EditRect[1].top,buffer,g_TextColor11,g_nBk,MZK_24);
				//CDC_OutText(g_EditRect[2].left+2,g_EditRect[2].top,buffer,g_TextColor2,BLUECOLOR);
			}
			break;
			 
	case 3:
			//OSTimeDly(3000);
			CDC_PutColorBar2(g_EditRect[3],g_nBk,1);
			//OSTimeDly(3000);
			if((g_iStepEditNo%2)==1)
			{
				sprintf(buffer,"%-2d",g_Ini.iStepRate[g_iStepEditNo/2]);
			}
			else
				sprintf(buffer,"%-1d",g_Ini.iStepDis[g_iStepEditNo/2]);
				
			CDC_OutText_UTF(g_EditRect[3].left+2,g_EditRect[3].top,buffer,g_TextColor11,g_nBk,MZK_24);
			//Uart_Printf("g_EditRect[3].left =%d,g_EditRect[3].top =%d\n",g_EditRect[3].left,g_EditRect[3].top);
			break;
	}
}


void	UpdateStep(INT16S dk)
{
	g_iStepEditNo+=dk;
	
	if(g_iStepEditNo>=g_Ini.iAxisCount*2)
		g_iStepEditNo=0;
		
	if(g_iStepEditNo<0)
		g_iStepEditNo=g_Ini.iAxisCount*2-1;
}


void	UpdateStepTimes(INT16S dk)
{
	g_iStepTimesEditNo+=dk;
	
	if(g_iStepTimesEditNo>=g_Ini.iAxisCount)
		g_iStepTimesEditNo=0;
		
	if(g_iStepTimesEditNo<0)
		g_iStepTimesEditNo=g_Ini.iAxisCount-1;
}


void	CTeach_GotoNline(INT32S line)
{
	INT16S i=0;
	
	CTeach_DrawGrid();
	CTeach_InitData();
	
	if(line > Line_Data_Size)
	{
		line = Line_Data_Size;
	}
	
	if(line>MAXLINE-ROWS+1)											//最大行保护
		line = MAXLINE-ROWS+1;

	if(line<1)													//最小行保护
		line=1;

	CList_SetStartRow(line-1);
	CList_SetCurRow(0);
	
	for(i=0; i<ROWS; i++)
	   CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);
	   
	CTeach_Update(0,0);
}


void	CTeach_InvidateGrid()
{
	INT16S i=0;
	
	CDC_LOCKSRC();
	CTeach_DrawGrid();
	CTeach_InitData();

	//CList_SetStartRow(0);
	//CList_SetCurRow(0);
	//CList_SetCurCol(2);
	
	for(i=0; i<ROWS; i++)
	   CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);
	   
	/*switch(g_Ini.iAxisCount)
	{
		case 2:
			CDC_PutColorBar(2,LISTTOP,Widths2[0]-1,LISTHEIGHT,GREENCOLOR,1);
			break;
		case 3:
			CDC_PutColorBar(2,LISTTOP,Widths3[0]-1,LISTHEIGHT,GREENCOLOR,1);
			break;
		case 4:
			CDC_PutColorBar(2,LISTTOP,Widths4[0]-1,LISTHEIGHT,GREENCOLOR,1);
			break;
		case 5:
			CDC_PutColorBar(2,LISTTOP,Widths5[0]-1,LISTHEIGHT,GREENCOLOR,1);
			break;
		case 6:
			CDC_PutColorBar(2,LISTTOP,Widths6[0]-1,LISTHEIGHT,GREENCOLOR,1);
			break;
		
	}
	
	CDC_OutText_UTF(7,LISTTOP+2,"001",LIGHTBLACKCOLOR,GREENCOLOR,MZK_24);;*/
	
	CDC_UNLOCKSRC();
	
	CTeach_Update(0,0);

	   
}
