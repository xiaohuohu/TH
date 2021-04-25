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
//# include	"usb.h"
# include 	"strtool.h"
//# include	"part.h"
# include	"usb_host.h"
# include   "base.h"

extern 		int 		usb_max_devs;
//int			VER=110;															//版本号指示

extern 		NEWDOG		NewDog;
extern 		COMMAND		Line_Data[MAXLINE];
extern 		INT16S		Line_Data_Size;
extern 		UNITDATA	g_UnitData;
extern 		BASEINI		g_Ini;
extern 		INT16U		g_uBmpXMS;
extern      INT16S      g_iCardStopFlag[CHMAX];	

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
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TextColor6;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S		g_nTechfrmBk;
WORK_EXT	INT32S 		g_nBackColor;
WORK_EXT	INT32S 		g_nRunTextColor; 
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;
//WORK_EXT 	INT32S  	g_lRamRunCounts;
WORK_EXT 	INT32S		g_lProbeFail; 											//记录探针失败   
WORK_EXT 	FP32		g_fStepSpeed[4];

INT32S		g_lAxisValue[4] = {0,0,0,0};										//记录各轴关断时的位置
INT32S      g_lHandPulse  = 0;
extern 		INT32S      g_iLayNum;
WORK_EXT	FP32   		g_fRunSpeed;
extern		BOOL		g_bSysReadBackup;								

#define		LISTHEIGHT	40
#define		LISTLEFT	4
#define		LISTTOP		4

#define		EDITHEIGHT  40														//主界面显示框高度
#define		EDITWIDTH	345														//主界面显示框宽度
#define		EDITLEFT	43														//主界面显示框X起点
#define		EDITTOP		93														//主界面显示框Y起点

INT16S		Widths2[COLS2]={ 70,140,140,140,100,90,110};
INT16S		Widths3[COLS3]={ 70,110,110,110,110,100,85,95};	
INT16S		Widths4[COLS4]={ 70,110,110,110,95,95,60,65,75};

INT16S		g_iCurEditNo=9;
BOOL		g_bEditData=FALSE; 
//CRect		g_EditRect[3]={{85,9,85+58,9+28},{315,9,315+115,9+28},{575,9,575+58,9+28}};		//程序号，目标数量，探针设定
CRect		g_EditRect[3]={{236,94,370,133},{600,144,734,183},{600,244,734,283}};		
BOOL		g_bDispServoWaring=TRUE;
CTROL		*m_ptrCtrl;	
CTROL		CtrlTmp;
extern      INT8U   	g_ukeyscan;   
INT16S		g_iCommandPos = 0;
INT8S		g_cCommand[7]={'M','S','E','L','N','J','G'};
BOOL		g_bInterface = TRUE;												//界面识别，(TRUE：主界面,FALSE:教导界面)
BOOL		g_bInsertNum = FALSE;												//是否有数字输入

INT32S		g_iStopScrTime = 0;													//停机屏保计时
extern		INT32S 		g_bInitbmp;
BOOL		g_bSingleRun = FALSE;		

void	CTeach_Init()
{
	m_ptrCtrl = &CtrlTmp;
	g_iRunLine = 0;
	Ok_Exit = FALSE;	
	CList_Init();
	CList_MoveTo( LISTLEFT,LISTTOP );											//列表框的位置

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

	default:
	{
		CList_SetMaxRowCol( ROWS, COLS3, MAXLINE,LISTHEIGHT, Widths3);
		m_ptrCtrl->w=Widths3[1]-2;
	}
	break;
	}

	m_ptrCtrl->x = CList_CurCol()+2;
	m_ptrCtrl->y = LISTTOP+CList_CurRow()*LISTHEIGHT+2;
	m_ptrCtrl->h = LISTHEIGHT-2;
	CEdit_SetCtrl(m_ptrCtrl);
	CEdit_SetTextColor(WHITECOLOR);
	CEdit_SetBackColor(BLUECOLOR);
	CEdit_Create();
	//	InstallDriverInterrupt();

	ReadDataFromFile(g_Ini.iFileName);
	ReadRunCount();
	//g_Ini.lRunCount = g_lRamRunCounts;
	//WriteDataToFile(g_Ini.iFileName);

	g_bModifySave=TRUE;
}


void	CTeach_Destroy()
{
	CMsg_Destroy();
	CList_Destroy();
	Ok_Exit=FALSE;
}


void	CTeach_Create()
{
	CRect   rc;

	SetMM(7,0,FALSE);
	SetMM(8,0,FALSE);

	CEdit_SetTextColor(WHITECOLOR);
	CEdit_SetBackColor(BLUECOLOR);

	g_iCurEditNo = (g_bInterface?0:9);

	g_bFirstDispTime = TRUE;
	g_iCommandPos = 0;

	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),g_nBackColor,100);					//画外框
	CDC_Rectangle(0,0,getmaxx(),getmaxy(),SILVERCOLOR);
	CDC_Rectangle(1,1,getmaxx()-1,getmaxy()-1,SILVERCOLOR);
	CDC_Rectangle(2,2,getmaxx()-2,getmaxy()-2,SILVERCOLOR);
	CDC_Rectangle(3,3,getmaxx()-3,getmaxy()-3,SILVERCOLOR);



	if(g_bInterface)
	{
		INT16U i=0;
		CRect rect;
		INT16U  NUM[6]={8,2,0,8,0,0};
		INT16U  startPosX,startPosY;
		INT16U	YMzk,SZzk,HZzk;


		DispBMP("\\PARA\\main.bmp",1,3,3);	
		//DispBMP("\\PARA\\main1.bmp",1,123,60);
		//DispBMPL("\\PARA\\main1.bmp",123,3,60,0,100,48);
		//CDC_OutText_48(223,23,g_bEnglish?"YF8208 Spring Control":"YF8208弹簧机控制器",PINKCOLOR, g_nBackColor);

		//DispBMPL("\\PARA\\Number.bmp",123,3,40,0,32,48);


		startPosX=228;
		startPosY=6;
		YMzk=32;
		SZzk=23;
		HZzk=48;

		if(g_Ini.iSystemName==9999)
		{
			DispBMP("\\PARA\\main1.bmp",1,123,3);
		}
		else
		{
			if(g_Ini.iSystemName!=0)
			{

				NUM[0] = (g_Ini.iSystemName%10000)/1000;
				NUM[1] = (g_Ini.iSystemName%1000)/100;
				NUM[2] = (g_Ini.iSystemName%100)/10;
				NUM[3] = g_Ini.iSystemName%10;
			}

			//显示英文YF--位宽为32
			for(i=0;i<2;i++)
				DispWords(i,startPosX+i*YMzk,startPosY);
			//显示数字8208--位宽为24
			for(i=0;i<4;i++)
				DispWordsS(NUM[i]+2,startPosX+2*YMzk+i*SZzk-2,startPosY);
			//显示汉字--位宽为48
			for(i=0;i<6;i++)
				DispWordsH(i,startPosX+2*YMzk+4*SZzk+i*HZzk,startPosY);
		}

		CDC_PutColorBar(5,393,659,36,REDCOLOR,100);

		CMsg_Init();
		CMsg_SetWH(115,397,550,24,WHITECOLOR,REDCOLOR,WHITECOLOR);

		CDC_Line(0,63,getmaxx(),63,WHITECOLOR);
		CDC_Line(0,64,getmaxx(),64,WHITECOLOR);

		for(i=0; i<6; i++)														//左边显示框
		{
			if(i==1 || i==2 || i==3)
				continue;

			CDC_DrawArcEdit(EDITLEFT,EDITTOP+i*(EDITHEIGHT+10),EDITWIDTH,EDITHEIGHT,YELLOWCOLOR);
		}

		for(i=0; i<6; i++)														//右边
		{
			CDC_DrawArcEdit(EDITLEFT+EDITWIDTH+24,EDITTOP+i*(EDITHEIGHT+10),EDITWIDTH,EDITHEIGHT,YELLOWCOLOR);
		}

		CDC_OutText_32(47,101+5*(EDITHEIGHT+10),g_bEnglish?"Left Time":"剩余时间",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47,101+4*(EDITHEIGHT+10),g_bEnglish?"Current Line":"当前行号",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47,101,g_bEnglish?"Filename":"程序编号",YELLOWCOLOR, g_nBackColor);

		CDC_OutText_32(47+EDITWIDTH+24,101,g_bEnglish?"Mode":"当前模式",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+EDITHEIGHT+10,g_bEnglish?"Counts":"目标数量",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+2*(EDITHEIGHT+10),g_bEnglish?"Total":"当前数量",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+3*(EDITHEIGHT+10),g_bEnglish?"Set Probe":"探针设定",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+4*(EDITHEIGHT+10),g_bEnglish?"Fail Probe":"探针失败",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+5*(EDITHEIGHT+10),g_bEnglish?"Speed":"生产速度",YELLOWCOLOR, g_nBackColor);


		CDC_Line(0,390,getmaxx(),390,WHITECOLOR);								//消息框28
		CDC_Line(0,428,getmaxx(),428,WHITECOLOR);
		CDC_OutText24Ex(5,397,g_bEnglish?"Message:":"消息提示:",WHITECOLOR, REDCOLOR);


		CDC_Line(160,428,160,getmaxy(),WHITECOLOR);								//气缸探针框
		CDC_Line(161,428,161,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(0, 428, 160, 49,g_bEnglish?"Air 1":"气缸1",GREENCOLOR,g_nBackColor);

		CDC_Line(320,428,320,getmaxy(),WHITECOLOR);
		CDC_Line(321,428,321,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(160, 428, 160, 49,g_bEnglish?"Air 2":"气缸2",GREENCOLOR,g_nBackColor);

		CDC_Line(480,428,480,getmaxy(),WHITECOLOR);
		CDC_Line(481,428,481,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(320, 428, 160, 49,g_bEnglish?"Drive":"送线架",GREENCOLOR,g_nBackColor);

		CDC_Line(640,428,640,getmaxy(),WHITECOLOR);
		CDC_Line(641,428,641,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(480, 428, 160, 49,g_bEnglish?"Wire fly":"跑线",GREENCOLOR,g_nBackColor);
		CDC_TextCen_32(640, 428, 160, 49,g_bEnglish?"Probe 1":"探针1",GREENCOLOR,g_nBackColor);

		if(g_Ini.iAxisCount<3)
		{
			rect.left = 73;															//坐标显示背景框
			rect.top  = 159;
			rect.right= 133;
			rect.bottom = 202;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);

			rect.left = 73;
			rect.top  = 229;
			rect.right= 133;
			rect.bottom = 272;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);
		}
		else if(g_Ini.iAxisCount>2)
		{
			rect.left = 73;															//坐标显示背景框
			rect.top  = 138;
			rect.right= 133;
			rect.bottom = 183;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);

			rect.left = 73;
			rect.top  = 188;
			rect.right= 133;
			rect.bottom = 233;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);

			rect.left = 73;
			rect.top  = 238;
			rect.right= 133;
			rect.bottom = 283;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);
		}

		switch(g_Ini.iAxisCount)
		{

		case 4:
		case 3:
			CDC_TextCen32(74, 138, 60, 45,"X",BLACKCOLOR,DARKGREENCOLOR2);
			CDC_TextCen32(74, 188, 60, 45,"Y",BLACKCOLOR,DARKGREENCOLOR2);
			CDC_TextCen32(74, 238, 60, 45,"Z",BLACKCOLOR,DARKGREENCOLOR2);

			{
				INT8S   buffer[0x10]="\0";

				memset(buffer,NULL,0x10);
				ftoa(GetAngle(1,TRUE),(INT8U *)buffer,1,8);
				CDC_OutText_UTF(220,143,buffer,REDCOLOR,g_nBackColor,YH_32);

				if(g_Ini.iAxisSwitch[1])
				{
					memset(buffer,NULL,0x10);
					ftoa(GetMM(2,TRUE) - GetYLong(vm_getindex()),(INT8U *)buffer,1,8);
					CDC_OutText_UTF(220,193,buffer,REDCOLOR,g_nBackColor,YH_32);
				}
				else
				{
					CDC_PutColorBar(220,193,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,193,"OFF",REDCOLOR,g_nBackColor,YH_32);
				}

				if(g_Ini.iAxisSwitch[2])
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
					}
					else if(g_Ini.iAxisRunMode[2]==2)
						ftoa(GetMM(3,TRUE),(INT8U *)buffer,2,8);

					CDC_OutText_UTF(220,243,buffer,REDCOLOR,g_nBackColor,YH_32);
				}
				else
				{
					CDC_PutColorBar(220,243,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,243,"OFF",REDCOLOR,g_nBackColor,YH_32);
				}
			}
			break;

		case 2:
			CDC_TextCen32(74, 161, 60, 40,"X",BLACKCOLOR,DARKGREENCOLOR2);
			CDC_TextCen32(74, 231, 60, 40,"Y",BLACKCOLOR,DARKGREENCOLOR2);

			{
				INT8S   buffer[0x10]="\0";

				memset(buffer,NULL,0x10);
				ftoa(GetAngle(1,TRUE),(INT8U *)buffer,1,8);
				CDC_OutText_UTF(220,165,buffer,REDCOLOR,g_nBackColor,YH_32);

				if(g_Ini.iAxisSwitch[1])
				{
					memset(buffer,NULL,0x10);
					ftoa(GetMM(2,TRUE) - GetYLong(vm_getindex()),(INT8U *)buffer,1,8);
					CDC_OutText_UTF(220,235,buffer,REDCOLOR,g_nBackColor,YH_32);
				}
				else
				{
					CDC_PutColorBar(220,235,150,34,g_nBackColor,100);
					CDC_OutText_UTF(220,235,"OFF",REDCOLOR,g_nBackColor,YH_32);
				}
			}

			break;
		}

		UpdateFileDestProbe(0,0,0);												//程序编号，产量设定，探针设定	
		UpdateSpeedCountProbe();												//运行速度，产量，探针失败次数,显示更新	
		UpdateProbe(TRUE);														//刷新显示探针信号状态
		UpdateCurAxis(g_iWorkNumber);
		UpdateCylinder(TRUE);													//刷新显示汽缸状态
		UpdateLeavingTime(TRUE,TRUE,TRUE);

	}
	else	
	{
		CRect rect;

		CDC_PutColorBar(5,445,656,30,REDCOLOR,100);

		CMsg_Init();
		CMsg_SetWH(115,448,550,24,WHITECOLOR,REDCOLOR,WHITECOLOR);

		rc.left = 2;															//画信息提示框
		rc.top =  444;
		rc.right = getmaxx()-3;
		rc.bottom = getmaxy()-3;
		CDC_Rectangle2(rc,WHITECOLOR);

		rc.left   += 1;
		rc.top    -= 1;
		rc.right  -= 1;
		rc.bottom += 1; 
		CDC_Rectangle2(rc,WHITECOLOR);
		CDC_OutText24Ex(5,447,g_bEnglish?"Message:":"消息提示:",WHITECOLOR,REDCOLOR);	

		if(g_iWorkNumber!=0)
		{
			switch(g_iWorkNumber)
			{
			case 1:CList_SetCurCol(2);break;
			case 2:CList_SetCurCol(3);break;
			case 3:CList_SetCurCol(4);break;
			case 4:CList_SetCurCol(5);break;
			}
		}

		CTeach_InvidateView(TRUE);												//列表显示

		rect.left = 10;															//坐标显示背景框
		rect.top  = 368;
		rect.right= 70;
		rect.bottom = 402;
		CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);

		rect.left = 240;
		rect.top  = 368;
		rect.right= 300;
		rect.bottom = 402;
		CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);

		if(g_Ini.iAxisCount>2)
		{
			rect.left = 480;
			rect.top  = 368;
			rect.right= 540;
			rect.bottom = 402;
			CDC_DrawRect(rect,DARKGREENCOLOR2,g_nBackColor);
		}

		switch(g_Ini.iAxisCount)
		{

		case 4:
		case 3:
			CDC_TextCen_32(480,370,60,32,"Z",YELLOWCOLOR,DARKGREENCOLOR2);
		case 2:
			CDC_TextCen_32(10,370,60,32,"X",YELLOWCOLOR,DARKGREENCOLOR2);
			CDC_TextCen_32(240,370,60,32,"Y",YELLOWCOLOR,DARKGREENCOLOR2);
			break;
		}

		CDC_OutText_32(10,409,g_bEnglish?"Mode:":"当前模式:",YELLOWCOLOR,g_nBackColor);
		CDC_OutText_32(550,409,g_bEnglish?"S":"S",YELLOWCOLOR,g_nBackColor);

		UpdateAxisStatus();	
	}

	CTeach_ModeDisp();
	//----------------------------显示各个灯的亮灭-------------------------------//
	if(g_Ini.iAxisSwitch[1]==1)	
		key_set_led(YSWITCHLED,LEDCLOSE);
	else
		key_set_led(YSWITCHLED,LEDOPEN);

	if(g_Ini.iAxisSwitch[2]==0 && g_Ini.iAxisCount==3)	
		key_set_led(ZSWITCHLED,LEDOPEN);
	else
		key_set_led(ZSWITCHLED,LEDCLOSE);

	if(g_bMode)	
		key_set_led(HANDLELED,LEDOPEN);	
	else
		key_set_led(HANDLELED,LEDCLOSE);

	if(GetOut(0))
		key_set_led(AIRALED,LEDOPEN);
	else
		key_set_led(AIRALED,LEDCLOSE);

	if(GetOut(1))
		key_set_led(AIRBLED,LEDOPEN);
	else
		key_set_led(AIRBLED,LEDCLOSE);

	if(GetOut(2))
		key_set_led(AIRCLED,LEDOPEN);
	else
		key_set_led(AIRCLED,LEDCLOSE);

	if(GetOut(3))
		key_set_led(AIRDLED,LEDOPEN);
	else
		key_set_led(AIRDLED,LEDCLOSE);	

	if(g_iSpeedBeilv==3)
	{
		key_set_led(HANDBOXLED,LEDOPEN);
	}
	else
	{
		key_set_led(HANDBOXLED,LEDCLOSE);
	}

	//key_set_led(STOPLED,LEDOPEN);	
	//---------------------------------------------------------------------------//

	if(g_bInterface)
	{
		//CDC_TextSelect_32(236,294,132,39,DISPSTYLE,"0",g_nRunTextColor, g_nBackColor);
		CDC_OutTextLR_UTF(236,294,"0",g_nRunTextColor,g_nBackColor,MZK_32,7,0);
	}
	else
	{
		//CDC_OutText_32(680,409,"0",g_nRunTextColor, g_nBackColor);
		CDC_OutTextLR_UTF(630,405,"0",g_nRunTextColor,g_nBackColor,MZK_32,3,0);
	}

}


void	CTeach_InvidateView(BOOL init)
{
	INT16S size=0,i=0;

	if(init)
	{
		INT8S *title2[COLS2] ={"N","开始","结束","送线开","速比","探针","气缸"};
		INT8S *title21[COLS2] ={"N","Start","End","Y(ON)","SpdRate","Probe","Out"};

		INT8S *title3[COLS3] ={"N","开始","结束","送线开","Z轴开","速比","探针","气缸"};	
		INT8S *title31[COLS3] ={"N","Start","End","Y(ON)","Z(ON)","SpdRate","Probe","Out"};	

		INT8S *title4[COLS4] ={"N","开始","结束","送线","Z轴","A轴","速比","探针","气缸"};	
		INT8S *title41[COLS4] ={"N","Start","End","Y(ON)","Z(ON)","A(ON)","SpdRate","Probe","Out"};

		switch(g_Ini.iAxisCount)
		{	
		case 2:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title2[3]="送线关";
				title21[3]="Y(OFF)";
			}
			CList_DrawTitle( g_bEnglish?title21:title2 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;

		case 3:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title3[3]="送线关";
				title31[3]="Y(OFF)";
			}

			if(g_Ini.iAxisSwitch[2]==0)
			{
				title3[4]="Z轴关";
				title31[4]="Z(OFF)";
			}

			CList_DrawTitle( g_bEnglish?title31:title3 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;

		case 4:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title4[3]="送线关";
				title41[3]="Y(OFF)";
			}

			if(g_Ini.iAxisSwitch[2]==0)
			{
				title4[4]="Z轴关";
				title41[4]="Z(OFF)";
			}
			if(g_Ini.iAxisSwitch[3]==0)
			{
				title4[5]="A轴关";
				title41[5]="A(OFF)";
			}
			CList_DrawTitle( g_bEnglish?title41:title4 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;

		default:
		{
			CList_DrawTitle( g_bEnglish?title21:title2 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;
		}

		CTeach_DrawGrid();
		CTeach_InitData();
	}

	size = Line_Data_Size;

	if( size < 1 )
	{
		return;
	}

	size = size - CList_StartRow();												//size为当前页面的行数

	if( size > ROWS ) 
		size = ROWS;

	for(i=0; i<ROWS; i++)
		CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);

	CTeach_Update(0,0);
	UpdateCurAxis(g_iWorkNumber);
}


void	CTeach_DrawGrid()
{
	INT16S x,i;
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

	default:
		CList_GetItemRect(ROWS-1,COLS2-1,&rt2);
		break;
	}

	//	CDC_PutColorBar(rt1.left,rt1.top,rt2.right-rt1.left,rt2.bottom-rt1.top,g_nTechfrmBk,1);
	CDC_Rectangle(rt1.left,rt1.top,rt2.right,rt2.bottom+1,g_TableLine);

	x = rt1.left;

	switch(g_Ini.iAxisCount)
	{

	case 2:
		for(i=0; i<COLS2+1; i++)
		{
			CDC_Line(x, rt1.top , x, rt2.bottom,g_TableLine);
			x += Widths2[i];
		}
		break;

	case 3:
		for(i=0; i<COLS3+1; i++)
		{
			CDC_Line(x, rt1.top , x, rt2.bottom,g_TableLine);
			x += Widths3[i];
		}
		break;

	case 4:
		for(i=0; i<COLS4+1; i++)
		{
			CDC_Line(x, rt1.top , x, rt2.bottom,g_TableLine);
			x += Widths4[i];
		}
		break;

	default:
		for(i=0; i<COLS2+1; i++)
		{
			CDC_Line(x, rt1.top , x, rt2.bottom,g_TableLine);
			x += Widths2[i];
		}
		break;
	}
}


void	UpdateSpeedCountProbe()
{
	char buffer[0x09];

	sprintf(buffer,"%d",g_Ini.lRunCount);										//产量
	CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top+EDITHEIGHT+10, g_EditRect[1].right-g_EditRect[1].left,
			g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

	sprintf(buffer,"%d",g_lProbeFail);											//探针失败
	CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top+EDITHEIGHT+10, g_EditRect[2].right-g_EditRect[2].left,
			g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

	sprintf(buffer,"%d",(INT32S)g_fRunSpeed);								//运行速度
	CDC_PutColorBar(g_EditRect[2].left, g_EditRect[2].top+2*(EDITHEIGHT+10), g_EditRect[2].right-g_EditRect[2].left,
			g_EditRect[2].bottom-g_EditRect[2].top, g_nTechfrmBk,0);
	CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top+2*(EDITHEIGHT+10), g_EditRect[2].right-g_EditRect[2].left,
			g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
}


void	UpdateFileDestProbe(INT16S no,INT16S dir,INT16S IsEnter)
{
	if( no==0 )
	{
		char buffer[0x09];

		sprintf(buffer,"%d",g_Ini.iFileName);
		CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
				g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

		sprintf(buffer,"%d",g_Ini.lDestCount);

		CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
				g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

		sprintf(buffer,"%d",g_Ini.lProbeCount);
		CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
				g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

		//		g_iCurEditNo = 9;

		if(g_bInterface)
		{

			CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);

			CEdit_SetFirst(TRUE);

			switch(g_iCurEditNo)
			{
			case 0:
				sprintf(buffer,"%d",g_Ini.iFileName);
				CEdit_SetString(buffer);
				CEdit_SetLen(3);
				break;

			case 1:
				CEdit_SetLong(g_Ini.lDestCount);
				CEdit_SetLen(7);
				break;

			case 2:
				CEdit_SetLong(g_Ini.lProbeCount);
				CEdit_SetLen(3);
				break;

			default:
				break;
			}

			CEdit_UpdateData(TRUE);
		}

	}
	else
	{
		INT8S buffer[0x10];

		switch(no)
		{
		case 1:
		{
			if(dir<0)
			{
				if(g_iCurEditNo==9)
				{
					/*							CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
											CList_CurRow(),Reset,g_TextColor6,
											g_nTableBk);
					 */
				}
				else
				{
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);

					switch(g_iCurEditNo)
					{
					case 0:
						sprintf(buffer,"%d",g_Ini.iFileName);
						CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
								g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 1:
						sprintf(buffer,"%d",g_Ini.lDestCount);
						CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
								g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 2:
						sprintf(buffer,"%d",g_Ini.lProbeCount);
						CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
								g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					default:
						break;
					}
				}

				g_iCurEditNo = 0;

				CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
				CEdit_SetFirst(TRUE);

				switch(g_iCurEditNo)
				{
				case 0:
					sprintf(buffer,"%d",g_Ini.iFileName);
					CEdit_SetString(buffer);
					CEdit_SetLen(3);
					break;

				case 1:
					CEdit_SetLong(g_Ini.lDestCount);
					CEdit_SetLen(7);
					break;

				case 2:
					CEdit_SetLong(g_Ini.lProbeCount);
					CEdit_SetLen(3);
					break;

				default:
					break;
				}

				CEdit_UpdateData(FALSE);
			}
			else
			{
				INT16S temp;

				if(IsEnter > 0)
				{
					temp = g_Ini.iFileName;

					if(g_iWorkStatus==NORUN)
						g_Ini.iFileName=CEdit_GetInt();
					else
						OSQPost(MsgQueue,g_bEnglish?"Process can not load the file!":"正在加工不能装入文件!");

					if(temp != g_Ini.iFileName)
					{
						/*
								if(!g_bModifySave)
								{
									INT8S a=0;

									for(a=5; a<8; a++)
										SuspendTasks(a);

									OSTimeDly(50);
									CMsgDlg_Init(200,140,200,120,g_bEnglish?" save file?":"文件已更改要保存吗?",g_bEnglish?"Warning":"警告",REDCOLOR,
												g_bEnglish?"Yes":"是",g_bEnglish?"No":"否");

									if(CMsgDlg_DoModal())
									{
										WriteDataToFile(temp);
										SysParaWrite();
										WriteRunCount();

										g_bModifySave=TRUE;
										CMsgDlg_Destroy();

										for(a=5; a<8; a++)
											ResumeTasks(a);
									}
									else
									{
										g_bModifySave=TRUE;
										CMsgDlg_Destroy();

										for(a=5; a<8; a++)
											ResumeTasks(a);
									}
								}
						 */

						WriteDataToFile(temp);//修正切换文件时，未保存加工数量未保存到加工文件中的问题20140815

						ReadDataFromFile(g_Ini.iFileName);

						if(Line_Data_Size<2)
						{
							g_Ini.lRunCount = 0;
							g_Ini.lDestCount = 1000000;
							g_fRunSpeed = 0;
						}

						WriteRunCount();
						g_bModify=TRUE;

						{
							INT32S t1,t2;

							OSSemPost(UnpackDataSem);

							t1 = OSTimeGet();

							g_bUnpackCorrect=FALSE;

							while(TRUE)
							{
								OSTimeDly(2);

								if(g_bUnpackCorrect)
									break;

								t2 = OSTimeGet();

								if((t2-t1)>200 || (t2-t1<-200))
									break;
							}

							if(!g_bUnpackCorrect)
							{
								CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);
								sprintf(buffer,"%d",g_Ini.iFileName);
								CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
										g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
								SysParaWrite();
								break;
							}
						}

						sprintf(g_pszMsgBuf,g_bEnglish?"Load%0dfile!":"载入%0d号文件!",g_Ini.iFileName);
						OSQPost(MsgQueue,g_pszMsgBuf);
					}
					else
					{
						CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);
						sprintf(buffer,"%d",g_Ini.iFileName);
						CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
								g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

					}
				}

				SysParaWrite();
				WriteRunCount();
				CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);
				sprintf(buffer,"%d",g_Ini.iFileName);
				CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
						g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
				g_iRunLine=0;
				g_iStaRunLine=0;
				g_lStepNode=0;
				CList_SetStartRow(0);
				CList_SetCurRow(0);
				CList_SetCurCol(0);
				//						CTeach_Update(0,0);
			}
		}
		break;

		case 2:
		{
			if(dir<0)
			{
				if(g_iCurEditNo==9)
				{
					/*							CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
											CList_CurRow(),Reset,g_TextColor6,
											g_nTableBk);
					 */
				}
				else
				{
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);

					switch(g_iCurEditNo)
					{
					case 0:
						sprintf(buffer,"%d",g_Ini.iFileName);
						CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
								g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 1:
						sprintf(buffer,"%d",g_Ini.lDestCount);
						CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
								g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 2:
						sprintf(buffer,"%d",g_Ini.lProbeCount);
						CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
								g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					default:
						break;
					}
				}

				g_iCurEditNo=1;
				CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
				CEdit_SetFirst(TRUE);

				switch(g_iCurEditNo)
				{
				case 0:
					sprintf(buffer,"%d",g_Ini.iFileName);
					CEdit_SetString(buffer);
					CEdit_SetLen(3);
					break;

				case 1:
					CEdit_SetLong(g_Ini.lDestCount);
					CEdit_SetLen(7);
					break;

				case 2:
					CEdit_SetLong(g_Ini.lProbeCount);
					CEdit_SetLen(3);
					break;

				default:
					break;
				}

				CEdit_UpdateData(FALSE);
			}
			else
			{

				CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);

				if(IsEnter>0)
				{
					g_Ini.lDestCount = CEdit_GetLong();

					if(g_Ini.lDestCount<1)
						g_Ini.lDestCount = 1;

					sprintf(buffer,"%d",g_Ini.lDestCount);
					CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
							g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
					SysParaWrite();
					WriteDataToFile(g_Ini.iFileName);
				}
				else
				{
					sprintf(buffer,"%d",g_Ini.lDestCount);
					CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
							g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
				}

				//						CTeach_Update(0,0);
			}
		}
		break;

		case 3:
		{
			if(dir<0)
			{
				if(g_iCurEditNo==9)
				{
					//							CTeach_DrawItem(CList_CurRow()+CList_StartRow(),
					//											CList_CurRow(),Reset,g_TextColor6,
					//											g_nTableBk);

				}
				else
				{
					CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);

					switch(g_iCurEditNo)
					{
					case 0:
						sprintf(buffer,"%d",g_Ini.iFileName);
						CDC_TextSelect_32(g_EditRect[0].left, g_EditRect[0].top, g_EditRect[0].right-g_EditRect[0].left,
								g_EditRect[0].bottom-g_EditRect[0].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 1:
						sprintf(buffer,"%d",g_Ini.lDestCount);
						CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top, g_EditRect[1].right-g_EditRect[1].left,
								g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					case 2:
						sprintf(buffer,"%d",g_Ini.lProbeCount);
						CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
								g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
						break;

					default:
						break;
					}
				}

				g_iCurEditNo = 2;
				CEdit_MoveWindowRect(g_EditRect[g_iCurEditNo]);
				CEdit_SetFirst(TRUE);

				switch(g_iCurEditNo)
				{
				case 0:
					sprintf(buffer,"%d",g_Ini.iFileName);
					CEdit_SetString(buffer);
					CEdit_SetLen(3);
					break;

				case 1:
					CEdit_SetLong(g_Ini.lDestCount);
					CEdit_SetLen(7);
					break;

				case 2:
					CEdit_SetLong(g_Ini.lProbeCount);
					CEdit_SetLen(3);
					break;

				default:
					break;
				}

				CEdit_UpdateData(FALSE);
			}
			else
			{

				CDC_PutColorBar2(g_EditRect[g_iCurEditNo],g_nTechfrmBk,1);

				if(IsEnter>0)
				{
					g_Ini.lProbeCount = CEdit_GetLong();

					if(g_Ini.lProbeCount<0)
						g_Ini.lProbeCount = 0;

					sprintf(buffer,"%d",g_Ini.lProbeCount);
					CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
							g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

					SysParaWrite();
					WriteDataToFile(g_Ini.iFileName);
				}
				else
				{
					sprintf(buffer,"%d",g_Ini.lProbeCount);
					CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top, g_EditRect[2].right-g_EditRect[2].left,
							g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);
				}

				//						CTeach_Update(0,0);
			}
		}
		break;

		default:
			break;
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
		for(i=0;i<3;i++)
			x+=Widths2[i];

		w=Widths2[3];
		rect.left=x+2;														//+2与-2是为了修复覆盖于错位问题。
		rect.top=CList_Top();
		rect.right=x+w;
		rect.bottom=CList_Top()+CList_Height();

		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"Y(ON)":"Y(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"送线开":"送线关",BLACKCOLOR,g_nTitleBk);

		break;

	case 3:
		for(i=0;i<3;i++)
			x+=Widths3[i];

		w=Widths3[3];
		rect.left=x+2;
		rect.top=CList_Top();
		rect.right=x+w;
		rect.bottom=CList_Top()+CList_Height();

		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"Y(ON)":"Y(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"送线开":"送线关",BLACKCOLOR,g_nTitleBk);


		x+=w;
		w=Widths3[4];
		rect.left=x+2;
		rect.right=x+w;

		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z(ON)":"Z(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z轴开":"Z轴关",BLACKCOLOR,g_nTitleBk);


		break;

	case 4:
		for(i=0;i<3;i++)
			x+=Widths4[i];

		w=Widths4[3];
		rect.left=x+2;
		rect.top=CList_Top();
		rect.right=x+w;
		rect.bottom=CList_Top()+CList_Height();
		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"Y(ON)":"Y(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"送线开":"送线关",BLACKCOLOR,g_nTitleBk);

		x+=w;
		w=Widths4[4];
		rect.left=x+2;
		rect.right=x+w;
		CDC_PutColorBar2( rect,g_nTitleBk,1);
		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z(ON)":"Z(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z轴开":"Z轴关",BLACKCOLOR,g_nTitleBk);

		x+=w;
		w=Widths4[5];
		rect.left=x+2;
		rect.right=x+w;
		CDC_PutColorBar2( rect,g_nTitleBk,1);
		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[3]==1?"A(ON)":"A(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[3]==1?"A轴开":"A轴关",BLACKCOLOR,g_nTitleBk);
		break;

	default:return;
	}

}


void	UpdateCurAxis(INT32S AxisNum)
{
	switch(AxisNum)
	{
	case 1:
		key_set_led(XLED,LEDOPEN);
		key_set_led(YLED,LEDCLOSE);

		if(g_Ini.iAxisCount>2)
			key_set_led(ZLED,LEDCLOSE);

		break;
	case 2:
		key_set_led(XLED,LEDCLOSE);
		key_set_led(YLED,LEDOPEN);

		if(g_Ini.iAxisCount>2)
			key_set_led(ZLED,LEDCLOSE);

		break;
	case 3:
		if(g_Ini.iAxisCount<3)
			break;

		key_set_led(XLED,LEDCLOSE);
		key_set_led(YLED,LEDCLOSE);
		key_set_led(ZLED,LEDOPEN);
		break;
		/*
		case 4:
			if(g_Ini.iAxisCount<4)
				break;
			key_set_led(XLED,LEDCLOSE);
			key_set_led(YLED,LEDCLOSE);
			key_set_led(ZLED,LEDCLOSE);
			key_set_led(ULED,LEDOPEN);
			break;
		 */
	default:
		key_set_led(XLED,LEDCLOSE);
		key_set_led(YLED,LEDCLOSE);
		key_set_led(ZLED,LEDCLOSE);
		break;
	}
}


BOOL	UpdateCommand(INT8S *str)
{
	COMMAND *ptr=NULL;
	CRect	rect;

	if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
	{
		return FALSE;
	}

	g_bModify = TRUE;
	g_bModifySave = FALSE;
	ptr = (Line_Data+CList_StartRow()+CList_CurRow());

	strcpy(ptr->value[0], str );
	CDC_PutColorBar(Widths4[0]+CList_Left()+1,CList_Top()+CList_CurRow()*CList_Height()+3,Widths4[1]-4,CList_Height()-4,g_nTableBk,1);

	rect.left=Widths4[0]+CList_Left();
	rect.top=CList_Top()+CList_CurRow()*CList_Height();
	rect.right=rect.left+Widths4[1];
	rect.bottom=rect.top+LISTHEIGHT;

	CDC_OutCenText24(rect,ptr->value[0],DARKGREENCOLOR,g_nTableBk);

	return TRUE;
}


void	CTeach_Update( INT16S row,INT16S col )
{
	INT16S 	maxSize;
	INT16S 	oldrow,newrow,oldcol,newcol;
	CRect 	rect;
	COMMAND *ptr;
	INT16S 	t = CList_CurCol() + col;
	//CList_CurCol()实际决定了g_iWorkNumber
	if( t < 3 )
		g_iWorkNumber = 1;
	else
	{
		if( t< CList_MaxCol()-3 )
			g_iWorkNumber= t-1;
		else
			g_iWorkNumber=0;													//没有选择任何一个轴
	}

	//	CTeach_UpdateSpeed(0);

	if(col<0)
	{
		if(CList_CurCol()==1)
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
			oldcol = CList_CurCol();

			CList_ColAdd( col );

			if( CList_CurCol() <= 1 )
				CList_SetCurCol(1);

			if(CList_CurCol()>=CList_MaxCol())
				CList_SetCurCol(CList_MaxCol()-1);
		}

		if( CList_CurRow()==0 && CList_StartRow()!=0)
		{
			CTeach_UpdatePage( row );
			return;
		}

		if(CList_StartRow()==0 && CList_CurRow()==0)
			return;
	}

	if( row > 0 )
	{
		if(col!=0)
		{
			oldcol = CList_CurCol();

			CList_ColAdd( col );

			if( CList_CurCol() <= 1 )
				CList_SetCurCol(1);

			if(CList_CurCol()>=CList_MaxCol())
				CList_SetCurCol(CList_MaxCol()-1);

		}

		maxSize = Line_Data_Size - CList_StartRow();

		if( maxSize > ROWS )
			maxSize = ROWS;

		if( CList_CurRow() + row >= maxSize)
		{
			CTeach_UpdatePage( row );
			return;
		}
	}

	//	CTeach_DrawItem( CList_CurRow()+CList_StartRow(), CList_CurRow() ,Reset,g_TextColor1,g_nTableBk);

	oldrow = CList_CurRow();
	oldcol = CList_CurCol();

	CList_ColAdd( col );
	CList_RowAdd( row );

	if(CList_CurRow()<0)
		CList_SetCurRow(0);

	if( CList_CurCol() <= 1 )
		CList_SetCurCol(1);

	if(CList_CurCol() >= CList_MaxCol())
		CList_SetCurCol(CList_MaxCol()-1);

	if(CList_CurRow() >= ROWS)
		CList_SetCurRow(ROWS-1);

	//	CTeach_DrawItem( CList_CurRow()+CList_StartRow(), CList_CurRow() ,Reset,g_TextColor1,g_nTableBk);

	newrow = CList_CurRow();
	newcol = CList_CurCol();

	//	if(oldrow!=newrow)
	{
		CTeach_DrawItem( CList_StartRow()+oldrow, oldrow ,Reset, g_TextColor6,g_nTableBk);
		CTeach_DrawItem( CList_StartRow()+newrow, newrow ,Active,g_TextColor6,g_nTableBk);
		//		CTeach_DrawGrid();
	}
	/*	else
	{
		CList_GetItemRect(CList_CurRow(),CList_CurCol(),&rect);
		CDC_Rectangle(3,rect.top,getmaxx()-3,rect.bottom,PINKCOLOR);
		CDC_Rectangle(4,rect.top+1,getmaxx()-4,rect.bottom-1,PINKCOLOR);
	}
	 */

	CList_GetItemRect(CList_CurRow(),CList_CurCol(),&rect);
	//	CEdit_SetTextColor(BLACKCOLOR);
	//	CEdit_SetBackColor(WHITECOLOR);

	CEdit_SetTextColor(WHITECOLOR);
	CEdit_SetBackColor(BLUECOLOR);

	CEdit_MoveWindowRect(rect);

	if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
		return;

	ptr = ( Line_Data+CList_StartRow()+CList_CurRow());

	if(CList_CurCol() == CList_MaxCol()-1)
	{
		CEdit_SetLen(4);    													//气缸列设定字符个数
	}
	else
	{
		if(CList_CurCol() == CList_MaxCol()-2)
		{
			CEdit_SetLen(1);    												//探针列设定字符个数
		}
		else
		{
			if(CList_CurCol()==CList_MaxCol()-3)
			{
				CEdit_SetLen(4);												//速比列设定字符个数
			}
			else
			{
				if(CList_CurCol()<3)
					CEdit_SetLen(6); 											//X列设定字符个数为6
				else
				{
					if(CList_CurCol() == 3)
						CEdit_SetLen(7); 										//Y列设定字符个数为7

					if(CList_CurCol() == 4)
						CEdit_SetLen(6); 										//Z列设定字符个数为6

					if(CList_CurCol() == 5)
						CEdit_SetLen(6); 										//A列设定字符个数为6
				}
			}
		}
	}

	switch(g_Ini.iAxisCount)
	{
	case 2:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_SetString( ptr->value[CList_CurCol()+1] );
		else
		{
			if(ptr->value[0][0]=='N' && CList_CurCol()==3)
			{
				if(ptr->value[2][0]=='1')
					CEdit_SetString(g_bEnglish?"Open":"送线开");
				else
					CEdit_SetString(g_bEnglish?"Close":"送线关");
			}
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
		}
		break;

	case 3:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_SetString( ptr->value[CList_CurCol()+0] );
		else
		{
			if(ptr->value[0][0]=='N' && CList_CurCol()==3)
			{
				if(ptr->value[2][0]=='1')
					CEdit_SetString(g_bEnglish?"Open":"送线开");
				else
					CEdit_SetString(g_bEnglish?"Close":"送线关");
			}
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
		}
		break;

	case 4:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_SetString( ptr->value[CList_CurCol()-1] );
		else
		{
			if(ptr->value[0][0]=='N' && CList_CurCol()==3)
			{
				if(ptr->value[2][0]=='1')
					CEdit_SetString(g_bEnglish?"Open":"送线开");
				else
					CEdit_SetString(g_bEnglish?"Close":"送线关");
			}
			else
				CEdit_SetString( ptr->value[CList_CurCol()-1] );
		}
		break;

	default:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_SetString( ptr->value[CList_CurCol()+0] );
		else
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
				return;

			CList_SetStartRow(0);
			CTeach_InvidateView(FALSE);
			return;
		}
	}

	{
		INT16S tmprow = CList_CurRow();

		if(tmprow < 0)
			tmprow = 0;

		if(tmprow > ROWS-1)
			tmprow = ROWS-1;

		CTeach_DrawItem(  CList_CurRow()+CList_StartRow(), tmprow ,Reset,g_TextColor6,g_nTableBk);
	}

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
void	CTeach_DisplayLine( INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor)//qColor白色，bColor黑色
{
	INT16S 		col;
	CRect 		rect;
	INT8S 		m_Buffer[0x12];
	COMMAND 	*ptr = Line_Data+data;

	for(col=0; col<CList_MaxCol(); col++)
	{
		if(col == 0)
		{
			if(ptr->value[0][0]!=NULL )
			{
				if( CList_DataRow(row) >= CList_MaxData())
				{
					return;
				}
			}

			CList_GetItemRect( row, col, &rect );
			sprintf(m_Buffer,"%03d",data+col);
			//			CDC_OutCenText24Ex(rect,m_Buffer,YELLOWCOLOR,g_nBackColor);
			CDC_TextSelect_32(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,0,m_Buffer,YELLOWCOLOR,g_nBackColor);
			//			CDC_TextCen24(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,
			//			m_Buffer,g_TextColor2,g_nBackColor);
			continue;
		}

		switch( status )
		{
		case Reset:
		{
			CList_GetItemRect( row, col, &rect );

			rect.left += 1;
			rect.right -= 1;
			rect.top+=1;
			CDC_PutColorBar2(rect,bColor,1);

			if(CList_DataRow(row) >= CList_MaxData())
				return;

			CTeach_ViewConvert(row, col, m_Buffer);				//将ptr->value[]中的值赋给m_buffer，在下面代码中执行后显示为白字黑底

			if(ptr->value[0][0]=='N' && col==3)
			{
				if(m_Buffer[0]=='1')
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"送线开",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"送线关",qColor,bColor);
				}
			}
			else
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,m_Buffer,qColor,bColor);

			memset(m_Buffer,NULL,sizeof(m_Buffer));
		}
		break;

		case Active:
		{
			CList_GetItemRect( row, col, &rect );

			rect.left += 1;
			rect.right -= 1;
			rect.top+=1;
			CDC_PutColorBar2( rect,bColor,1);

			if(CList_DataRow(row) >= CList_MaxData())
				break;

			CTeach_ViewConvert( row, col, m_Buffer );

			if(ptr->value[0][0]=='N' && col==3)
			{
				if(m_Buffer[0]=='1')
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"送线开",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"送线关",qColor,bColor);
				}
			}
			else
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,m_Buffer,qColor,bColor);
		}
		break;

		case MoveActive:											//手动模式正行跳动时，显示为黑字白底
		{
			CList_GetItemRect( row, col, &rect );

			rect.left += 1;
			rect.right -= 1;
			rect.top+=1;

			CDC_PutColorBar2( rect,qColor,1);
			CDC_Rectangle2( rect ,bColor);

			if(CList_DataRow(row) >= CList_MaxData())
				break;

			CTeach_ViewConvert( row, col, m_Buffer );

			if(ptr->value[0][0]=='N' && col==3)
			{
				if(m_Buffer[0]=='1')
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"送线开",bColor,qColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"送线关",bColor,qColor);
				}
			}
			else
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,m_Buffer,bColor,qColor );
		}
		break;

		case Draw:
		{
			CList_GetItemRect( row, col, &rect );

			rect.left += 1;
			rect.right -= 1;
			rect.top+=1;
			CDC_PutColorBar2( rect,bColor,1);

			if(CList_DataRow(row) >= CList_MaxData())
				break;

			CTeach_ViewConvert( row, col, m_Buffer );

			if(ptr->value[0][0]=='N' && col==3)
			{
				if(m_Buffer[0]=='1')
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"送线开",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"送线关",qColor,bColor);
				}
			}
			else
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,m_Buffer,qColor,bColor);

			memset(m_Buffer,NULL,sizeof(m_Buffer));
		}
		break;

		default:
			break;
		}


		/*		if( col == 0 && ptr->value[0][0]!=NULL )
		{
			CList_GetItemRect( row, col, &rect );
			CDC_PutColorBar2( rect,g_nTitleBk,1);
			CDC_Rectangle2( rect,g_TableLine);

			if( CList_DataRow(row) >= CList_MaxData())
			{
				return;
			}

			sprintf(m_Buffer,"%03d",data+1);
			CDC_OutCenText24Ex(rect,m_Buffer,g_TextColor2,g_nBackColor);
			continue;
		}
		else
		{
			if(col==0 && ptr->value[0][0]==NULL)
			{
				CList_GetItemRect( row, col, &rect );
				CDC_PutColorBar2(rect,g_nTitleBk,1);
				CDC_Rectangle2( rect,g_TableLine);
				continue;
			}
		}

		switch( status )
		{
			case Reset:
				{
					CList_GetItemRect( row, col, &rect );
					CDC_PutColorBar2(rect,bColor,1);
					CDC_Rectangle2( rect, g_TableLine);

					if(CList_DataRow(row) >= CList_MaxData())
						return;

					CTeach_ViewConvert(row, col, m_Buffer);
					CDC_OutCenText24Ex(rect,m_Buffer,qColor,bColor);
					memset(m_Buffer,NULL,sizeof(m_Buffer));
				}
				break;

			case Active:
				{
					CList_GetItemRect( row, col, &rect );
					CDC_PutColorBar2( rect,bColor,1);
					CDC_Rectangle2( rect ,g_TableLine);

					if(CList_DataRow(row) >= CList_MaxData())
						break;

					CTeach_ViewConvert( row, col, m_Buffer );
					CDC_OutCenText24Ex( rect, m_Buffer, qColor,bColor );

					rect.left=5;
					rect.right=getmaxx()-5;
					CDC_Rectangle2( rect ,PINKCOLOR);
					rect.left=6;
					rect.right=getmaxx()-6;
					rect.top=rect.top+1;
					rect.bottom=rect.bottom-1;
					CDC_Rectangle2( rect ,PINKCOLOR);
					memset(m_Buffer,NULL,sizeof(m_Buffer));
				}
				break;

			case MoveActive:
				{
					CList_GetItemRect( row, col, &rect );
					CDC_PutColorBar2( rect,qColor,1);
					CDC_Rectangle2( rect ,bColor);

					if(CList_DataRow(row) >= CList_MaxData())
						break;

					CTeach_ViewConvert( row, col, m_Buffer );
					CDC_OutCenText24Ex( rect, m_Buffer, bColor,qColor );

					rect.left=5;
					rect.right=getmaxx()-5;
					CDC_Rectangle2( rect ,PINKCOLOR);
					rect.left=6;
					rect.right=getmaxx()-6;
					rect.top=rect.top+1;
					rect.bottom=rect.bottom-1;
					CDC_Rectangle2( rect ,PINKCOLOR);
					memset(m_Buffer,NULL,sizeof(m_Buffer));
				}
				break;

			case Draw:
				{
					CList_GetItemRect( row, col, &rect );
					CDC_PutColorBar2( rect,bColor,1);
					CDC_Rectangle2( rect,g_TableLine);

					if(CList_DataRow(row) >= CList_MaxData())
						break;

					CTeach_ViewConvert( row, col, m_Buffer );
					CDC_OutCenText24Ex(rect,m_Buffer,qColor,bColor);
					memset(m_Buffer,NULL,sizeof(m_Buffer));
				}
				break;

			default:
				break;
		}
		 */
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

	switch(g_Ini.iAxisCount)
	{
	case 2:
		if(col>=CList_MaxCol()-3)
			strcpy(buffer,ptr->value[col+1]);
		else
			strcpy(buffer,ptr->value[col-1]);
		break;

	case 3:
		if(col>=CList_MaxCol()-3)
			strcpy(buffer,ptr->value[col+0]);
		else
			strcpy(buffer,ptr->value[col-1]);
		break;

	case 4:
		if(col>=CList_MaxCol()-3)
			strcpy(buffer,ptr->value[col-1]);
		else
			strcpy(buffer,ptr->value[col-1]);
		break;

	default:
		if(col>=CList_MaxCol()-3)
			strcpy(buffer,ptr->value[col+0]);
		else
			strcpy(buffer,ptr->value[col-1]);
		break;
	}
}


void	CTeach_InitData()
{
	if( Line_Data_Size == 0 )
	{
		Line_Data_Size = 1;

		DataCls(1);
		DataPut(0,0,"YF");
		DataPut(0,1,"50");
		DataPut(0,2,"50");

		if(g_Ini.iAxisCount>2)
		{
			DataPut(0,3,"50");
		}
		/*
		DataPut(0,0,"YF");
		DataPut(0,1,"50");
		DataPut(0,3,"50");
		DataPut(0,4,"50");
		DataPut(0,5,"50");
		DataCls(1);
		DataPut(1,0,"M");
		 */
		CList_SetStartRow(0);
		CList_SetCurCol(0);
		CList_SetCurRow(0);
		//		CList_SetCurRow(1);
	}
}


void	CTeach_AddData()
{
	INT16S size;

	if( Line_Data_Size >= MAXLINE )
		return;

	g_bModify = TRUE;
	g_bModifySave = FALSE;
	size = Line_Data_Size;

	//	DataPut(size,0,"0.0");

	DataCls(size);

	Line_Data_Size ++;
}


void	CTeach_Insert()
{
	INT16S 	i=0,index=0;
	INT8S 	buf[9]="\0";

	COMMAND *ptr = Line_Data;

	if( Line_Data_Size >= MAXLINE  )
		return;

	if( Line_Data_Size == 1 )
	{
		CTeach_AddData();
		return;
	}

	index = CList_StartRow() + CList_CurRow();

	if( index == 0 )
		return;

	g_bModify = TRUE;
	g_bModifySave=FALSE;

	for( i=Line_Data_Size; i>index; i--)
		*(Line_Data+i) = *(Line_Data+i-1);

	DataCls(index);
	//	DataPut(index,0,"0.0");
	Line_Data_Size ++;

	for(i=0; i<Line_Data_Size; i++)												//有N指令则其对应的行号需发生变化以保证指向的位置正确
	{
		ptr = (Line_Data+i);

		if(ptr->value[0][0]=='N')
		{
			INT16S temp = 0;
			INT8S  buf[9];
			INT8S  buf1[9]="\0";
			INT16S i=0;

			buf1[0]='N';

			for(i=0; ptr->value[0][i+1]!='\0'; i++)
			{
				buf[i] = ptr->value[0][i+1];
			}

			buf[i] = '\0';

			temp=stringtoi((INT8U *)buf);

			if(temp>index)
			{
				temp++;

				ltoa((INT32U)temp,(INT8U *)buf);

				strcat(buf1,buf);
				strcpy(ptr->value[0],buf1);
			}
		}
	}

	/*
	for(i=0; i<Line_Data_Size; i++)												//有J或G指令则其对应的行号需发生变化以保证指向的位置正确
	{
		ptr = (Line_Data+i);

		if(ptr->value[0][0]=='J'|| ptr->value[0][0]=='G')
		{
			INT16S temp = stringtoi((INT8U *)ptr->value[1]);

			if(temp>index)
			{
				temp++;
				ltoa((INT32U)temp,(INT8U *)buf);
				strcpy(ptr->value[1],buf);
			}
		}
	}
	 */

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

	for(i=0; i<Line_Data_Size; i++)												//有N指令则其对应的行号需发生变化以保证指向的位置正确
	{
		ptr=(Line_Data+i);

		if(ptr->value[0][0]=='N')
		{
			INT16S temp = 0;
			INT8S  buf[9];
			INT8S  buf1[9]="\0";
			INT16S i=0;

			buf1[0]='N';

			for(i=0; ptr->value[0][i+1]!='\0'; i++)
			{
				buf[i] = ptr->value[0][i+1];
			}

			buf[i] = '\0';

			temp=stringtoi((INT8U *)buf);

			if(temp>index)
			{
				temp--;
				ltoa((INT32U)temp,(INT8U *)buf);
				strcat(buf1,buf);
				strcpy(ptr->value[0],buf1);
			}
		}
	}
	/*
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
	 */
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
	INT16U key,tkey;

	data = data;

	if(!g_bSysParaCorrect)
	{
		g_iCurEditNo = 3;//修正第一次初始化参数时，系统参数测试界面显示异常的问题20140724

		CSysPara_Init(0,0,798,478);
		CSysPara_DoModal();
		CSysPara_Destroy();
	}

	g_bEnglish=g_Ini.iLanguage==1?TRUE:FALSE;
	CTeach_Init();
	CTeach_Create();
	TaskStartCreateTasks();														//创建其它任务

	SuspendTasks(1);
	SuspendTasks(2);
	SuspendTasks(3);
	SuspendTasks(4);
	OSTimeDly(50);

	g_iStopScrTime = OSTimeGet();

	for(;;)
	{
		CTeach_LoopCheck();

		key = KeyMBoxGetKey();

		if(key != 0xffff)
		{
			if(!ExitScrProtect())
			{
				CTeach_OnKey(key,tkey);
			}
		}

		OSTimeDly(3);
	}
}


void	CTeach_LoopCheck()
{
	INT32S pos=0;
	INT16S	i=0;

	{																			//急停按键处理
		static INT16S stopio=-1, stopio1=-1;
		INT16S stopbit, stopbit1;

		stopbit = ReadBit(STOPIO);
		stopbit1= ReadBit(STOPIO2);

		if(stopbit!=stopio || stopbit1!=stopio1)
		{
			stopio = stopbit;
			stopio1= stopbit1;

			if(stopbit==0 || stopbit1==0)
			{
				g_bCylinder = FALSE;

				{
					INT16S l=0;

					for(l=0; l<(g_Ini.iXianjiaDrive?(MAXCYLINDER-1):MAXCYLINDER); l++)
					{
						WriteBit(l,0);
					}
				}

				if(g_bMode)
				{
					g_lStepNode   = 0;
					//g_bStepIO     = FALSE;
					g_bDispRunLine= FALSE;
					g_bAutoRunMode= TRUE;

					CTeach_ModeDisp();
					g_bMode = FALSE;
					CTeach_ModeDisp();//修正手摇急停后，界面显示状态切换的BUG（退出手动加工模式） 20140812
					key_set_led(HANDLELED,LEDCLOSE);
				}
				else
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!操作前请抬起急停按钮!");

				g_bEMSTOP = TRUE;
			}

			return;
		}

		if(stopbit==0 || stopbit1==0)
		{
			OSTimeDly(5);
			return;
		}
	}

	{																			  //伺服报警功能
		INT16S XYZIO[4]={XSERVOWARNING,YSERVOWARNING,ZSERVOWARNING,USERVOWARNING};//XYZ伺服报警信号输入端口

		for( i=0; i<g_Ini.iAxisCount; i++)
		{
			if( g_Ini.iAxisSwitch[i]==0 )
				continue;

			if( RealIO(XYZIO[i],g_Ini.iServoLogic[i],2) )
			{
				OSTimeDly(3);

				if( RealIO(XYZIO[i],g_Ini.iServoLogic[i],2))
				{
					WriteBit(O_WARNING,OPEN);

					g_bFirstResetX = FALSE;
					g_bFirstResetY = FALSE;
					g_bFirstResetZ = FALSE;
					g_bFirstResetU = FALSE;

					if(g_bDispServoWaring)
					{
						switch(i)
						{
						case 0:
							OSQPost(MsgQueue,g_bEnglish?"X Servo Warning!Please Power off Check!":"X伺服故障,请断电检查!");
							break;

						case 1:
							OSQPost(MsgQueue,g_bEnglish?"Y Servo Warning!Please Power off Check!":"Y伺服故障,请断电检查!");
							break;

						case 2:
							OSQPost(MsgQueue,g_bEnglish?"Z Servo Warning!Please Power off Check!":"Z伺服故障,请断电检查!");
							break;

						case 3:
							OSQPost(MsgQueue,g_bEnglish?"U Servo Warning!Please Power off Check!":"U伺服故障,请断电检查!");
							break;

						default:
							break;
						}

						g_bDispServoWaring=FALSE;
					}

					return;
				}
			}
		}
	}


	{//断线，缠线报警
		INT16S 	bit;
		static 	INT16S OtherIOStatus[3]={0,1,1};
		//INT16S 	OtherIO[2]={DUANXIAN,SONGXIANIOCONST};
		static	BOOL bAlarm = TRUE;
		static	BOOL bDoorAlarm = TRUE;
		OtherIOStatus[0]=!g_Ini.iIOBreakLineConfig_Level;
		OtherIOStatus[1]=!g_Ini.iIOKinkConfig_Level;
		//if(g_Ini.iAxisCount<3)
		//OtherIO[1] = SONGXIANIOCONST;	//当为两轴卡时Y原点为缠线报警


		if(g_bRelease)
		{
			if(RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2) && bAlarm)
			{

				bAlarm = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Broken Wire Waring":"断线报警!");

				if(g_Ini.iWarningOut==1)
					WriteBit(O_WARNING,1);
			}

			else if(RealIO(DUANXIAN,!g_Ini.iIOBreakLineConfig_Level,2) && !bAlarm)
			{
				bAlarm = TRUE;
				OSQPost(MsgQueue,g_bEnglish?" ":" ");

				if(g_Ini.iWarningOut==1)
					WriteBit(O_WARNING,0);
			}
		}


		bit = ReadBit(SONGXIANIOCONST);

		if(bit!=OtherIOStatus[1])
		{
			OtherIOStatus[1] = bit;

			if(bit==g_Ini.iIOKinkConfig_Level)
			{
				g_iErrorNo=3;
				OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"缠线报警!");

				if(g_Ini.iWarningOut==1)
				{
					WriteBit(O_WARNING,OPEN);
				}
			}
			else
			{
				g_iErrorNo=0;
				OSQPost(MsgQueue,g_bEnglish?" ":" ");

				if(g_Ini.iWarningOut==1)
				{
					WriteBit(O_WARNING,CLOSE);
				}
			}
		}


		if(g_Ini.iPaoXian)
		{
			bit = ReadBit(PAOXIANDI);		//跑线报警

			if(bit!=OtherIOStatus[2])
			{
				OtherIOStatus[2] = bit;

				if( bit==0 )
				{
					g_iErrorNo=8;
					OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"跑线报警!");

					if(g_Ini.iWarningOut==1)
					{
						WriteBit(O_WARNING,OPEN);
					}
				}
				else
				{
					g_iErrorNo=0;
					OSQPost(MsgQueue,g_bEnglish?" ":" ");

					if(g_Ini.iWarningOut==1)
					{
						WriteBit(O_WARNING,CLOSE);
					}
				}
			}
		}
	}

	if(g_bMode)																	//寸动功能
	{
		//	g_bStepIO = FALSE;

		if(RealIO(STEPDI,IOPRESS,3))
		{
			if(CheckWarning(TRUE))
			{
				OSTimeDly(100);
				return;
			}

			if(g_lStepNode==0 && g_bModify)
			{
				INT32S t1,t2;

				OSSemPost(UnpackDataSem);
				t1 = OSTimeGet();
				g_bUnpackCorrect=FALSE;

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

			g_bAutoRunMode= TRUE;
			g_bTestStart  = TRUE;
			//g_bStepIO     = TRUE;
			g_bDispRunLine= TRUE;

			SetRange((INT32S)(g_fStepSpeed[g_iSpeedBeilv-1]*8000));
			RunAllData();

			return;
		}

	}

	if(g_Ini.iSwhRunIo==1)
	{																			//IO口启动
		if(!g_bPressStartIO && RealIO(RUNDI,0,2))
		{
			g_bPressStartIO = TRUE;
			CTeach_OnKey(TRUN,0);
		}

		if(g_bPressStartIO && RealIO(RUNDI,1,2))
		{
			g_bPressStartIO = FALSE;
		}
	}

	if(g_Ini.iSwhSingleIo==1 )		 //IO口单条
	{
		if(!g_bPressStepIO2 && RealIO(SINGLEDI,0,2) /*&&RealIO(SINGLEDI2,0,2)*/)                         //IO口单条
		{
			g_bPressStepIO2= TRUE;
			CTeach_OnKey(TSTEP,0);

		}

		if(g_bPressStepIO2 && RealIO(SINGLEDI,1,2) /*&& RealIO(SINGLEDI2,1,2)*/)
		{
			g_bPressStepIO2 = FALSE;
		}
	}
	else if ( g_Ini.iSwhSingleIo==2)						//配合冲压机信号做启动功能
	{
		/*
			 if(RealIO(SINGLEDI1,0,2))                         //IO口单条
			 {
				CTeach_OnKey(TRUN,0);							//特别工艺注意是TRUN
			 }*/

		if(!g_bPressStepIO2 && RealIO(SINGLEDI,0,2) /*&&RealIO(SINGLEDI2,0,2)*/)                         //IO口单条
		{
			g_bPressStepIO2= TRUE;
			CTeach_OnKey(TRUN,0);

		}

		if(g_bPressStepIO2 && RealIO(SINGLEDI,1,2) /*&& RealIO(SINGLEDI2,1,2)*/)
		{
			g_bPressStepIO2 = FALSE;
		}
	}




	//if(g_Ini.iHandStatus)														// 手轮功能
	{
		pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);

		if(labs(pos)>1)
		{
			if(g_bRelease)
			{
				if(RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level, 2))
				{
					OSQPost(MsgQueue,g_bEnglish?"Broken Wire Waring":"断线报警!");

					if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
					return ;
				}

				if( RealIO(SONGXIANIOCONST, g_Ini.iIOKinkConfig_Level, 2 ))
				{
					OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"缠线报警!");

					if(g_Ini.iWarningOut==1)
					{
						if(GetOut(O_WARNING)==0)
							WriteBit(O_WARNING,OPEN);
					}

					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);

					return ;
				}
			}

			{
				if(labs(pos)>1)
				{
					ExitScrProtect();
				}
			}

			if(!g_bMode)														//自动模式下手轮移动各个轴
			{
				if(g_iWorkNumber<g_Ini.iAxisCount+1 && g_iWorkNumber!=0)
				{
					FP32 	mv = -10000.0;
					FP32 	pL[4] = {0.2,0.8,2.0,3.0};
					static  INT16S s_iLagWarning[4]={0};
					INT32S  lTempPos =0;

					if(s_iLagWarning[g_iWorkNumber-1]==1)
					{
						SetMM(7,0,FALSE);
						SetMM(8,0,FALSE);
						return;
					}

					OSQPost(MsgQueue,"");

					{
						INT16S i=0;

						for(i=0; i<4; i++)
						{
							s_iLagWarning[i] = 0;
						}
					}

					if(g_lStepNode!=0)
					{
						g_lStepNode = 0;
						SetMM(2,0,TRUE);
					}

					g_lHandRun = OSTimeGet();

					switch(g_iWorkNumber)
					{
					INT32S curPos =0;
					INT32S QuanPos = 0;
					INT32S AnglePos=0;

					case 1:
						g_iMoveAxis  = 1;
						g_fSpeedBi   = 0.05 ;
						g_bHandMove  = TRUE;
						g_iWorkStatus= NORUN;

						lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];//以下两行函数在多圈手摇时，不用多圈回摇。
						SetPulse(g_iWorkNumber, lTempPos,TRUE);

						ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber));

						while (1)
						{
							if(IsFree(g_iWorkNumber))
								break;

							if(g_Ini.iLag[g_iWorkNumber-1]!=0 && (labs(GetPulse(g_iWorkNumber,TRUE)*g_Ini.iNumerator[g_iWorkNumber-1]-GetPulse(g_iWorkNumber,FALSE)*g_Ini.iDenominator[g_iWorkNumber-1]) >= g_Ini.iLag[g_iWorkNumber-1]))
							{
								Stop1(g_iWorkNumber);
								s_iLagWarning[g_iWorkNumber-1] = 1;
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
								break;
							}

							OSTimeDly(1);
						}

						mv = GetAngle(g_iWorkNumber,TRUE);
						g_iMoveAxis = 0;
						g_bHandMove = FALSE;
						break;

					case 2:

						if(g_Ini.iAxisSwitch[1] == 0)
						{
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							break;
						}

						if(g_Ini.iYDaoXian)
						{
							g_iMoveAxis  = 2;
							g_iWorkStatus= NORUN;
							g_fSpeedBi   = 0.05;
							g_bHandMove  = TRUE;

							ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber));

							while (1)
							{
								if (IsFree(g_iWorkNumber))
									break;

								if(g_Ini.iLag[g_iWorkNumber-1]!=0 && (labs(GetPulse(g_iWorkNumber,TRUE)*g_Ini.iNumerator[g_iWorkNumber-1]-GetPulse(g_iWorkNumber,FALSE)*g_Ini.iDenominator[g_iWorkNumber-1]) >= g_Ini.iLag[g_iWorkNumber-1]))
								{
									Stop1(g_iWorkNumber);
									s_iLagWarning[g_iWorkNumber-1] = 1;
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;
								}

								OSTimeDly(1);
							}

							mv = GetMM(g_iWorkNumber,TRUE);
							g_iMoveAxis = 0;
							g_bHandMove = FALSE;
						}
						else
						{

							if(pos>0)
							{
								g_iMoveAxis   = 2;
								g_iWorkStatus = NORUN;
								g_fSpeedBi    = 0.05;
								g_bHandMove   = TRUE;

								ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber));

								while (1)
								{
									if (IsFree(g_iWorkNumber))
										break;

									if(g_Ini.iLag[g_iWorkNumber-1]!=0 && (labs(GetPulse(g_iWorkNumber,TRUE)*g_Ini.iNumerator[g_iWorkNumber-1]-GetPulse(g_iWorkNumber,FALSE)*g_Ini.iDenominator[g_iWorkNumber-1]) >= g_Ini.iLag[g_iWorkNumber-1]))
									{
										Stop1(g_iWorkNumber);
										s_iLagWarning[g_iWorkNumber-1] = 1;
										OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
										break;
									}

									OSTimeDly(1);
								}

								g_bHandMove = FALSE;
								mv = GetMM(g_iWorkNumber,TRUE);
								g_iMoveAxis = 0;

							}
							else
							{
								SetMM(7,0,FALSE);
								SetMM(8,0,FALSE);
							}
						}
						break;

					case 3:
						if(g_Ini.iAxisCount<3)
							break;

						if(g_Ini.iAxisSwitch[2] == 0)
						{
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							break;
						}
						//检测硬限位和软限位，如果限位报警，则提示，并且将手轮置0，跳出loopcheck函数。
						if(CheckHardLimit(g_iWorkNumber,pos) || CheckSoftLimit(g_iWorkNumber,pos))
						{
							OSTimeDly(100);
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							g_bHandMove = FALSE;
							g_iMoveAxis = 0;
							return;
						}

						g_iMoveAxis  = 3;
						g_iWorkStatus= NORUN;
						g_fSpeedBi   = 0.05 ;
						g_bHandMove  = TRUE;

						if(g_Ini.iAxisRunMode[2] == 0)
						{
							lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
							SetPulse(g_iWorkNumber, lTempPos,TRUE);
						}

						ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber));

						while (1)
						{
							if (IsFree(g_iWorkNumber))
								break;

							if(g_Ini.iLag[g_iWorkNumber-1]!=0 && (labs(GetPulse(g_iWorkNumber,TRUE)*g_Ini.iNumerator[g_iWorkNumber-1]-GetPulse(g_iWorkNumber,FALSE)*g_Ini.iDenominator[g_iWorkNumber-1]) >= g_Ini.iLag[g_iWorkNumber-1]))
							{
								Stop1(g_iWorkNumber);
								s_iLagWarning[g_iWorkNumber-1] = 1;
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
								break;
							}

							if(CheckHardLimit(g_iWorkNumber,pos) || CheckSoftLimit(g_iWorkNumber,pos))
							{
								Stop1(g_iWorkNumber);
								break;
							}

							OSTimeDly(1);
						}

						if(g_Ini.iAxisRunMode[2]==0)
							mv=GetAngle(g_iMoveAxis,TRUE);
						else if(g_Ini.iAxisRunMode[2]==1)			//多圈
						{
							curPos=(INT32S)(GetMM(g_iMoveAxis,TRUE));
							QuanPos = curPos/360;
							AnglePos = curPos%360;

							mv = QuanPos + AnglePos/1000.0;
						}
						else if(g_Ini.iAxisRunMode[2]==2)
							mv=GetMM(g_iMoveAxis,TRUE);

						g_iMoveAxis = 0;
						g_bHandMove = FALSE;
						break;

					case 4:
						if(g_Ini.iAxisCount<4)
							break;

						if(g_Ini.iAxisSwitch[3] == 0)
						{
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							break;
						}

						g_iMoveAxis   = 4;
						g_iWorkStatus = NORUN;
						g_fSpeedBi    = 0.05 ;
						g_bHandMove   = TRUE;

						if(CheckHardLimit(g_iWorkNumber,pos) || CheckSoftLimit(g_iWorkNumber,pos))
						{
							OSTimeDly(100);
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
							g_bHandMove = FALSE;
							g_iMoveAxis = 0;
							return;
						}

						if(g_Ini.iAxisRunMode[3] == 0)
						{
							lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//以下两行函数在多圈手摇时，不用多圈回摇。
							SetPulse(g_iWorkNumber, lTempPos,TRUE);
						}

						ShortlyPmove(g_iWorkNumber,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[g_iWorkNumber-1]*GetUnitPulse(g_iWorkNumber));

						while (1)
						{
							if (IsFree(g_iWorkNumber))
								break;

							if(g_Ini.iLag[g_iWorkNumber-1]!=0 && (labs(GetPulse(g_iWorkNumber,TRUE)*g_Ini.iNumerator[g_iWorkNumber-1]-GetPulse(g_iWorkNumber,FALSE)*g_Ini.iDenominator[g_iWorkNumber-1]) >= g_Ini.iLag[g_iWorkNumber-1]))
							{
								Stop1(g_iWorkNumber);
								s_iLagWarning[g_iWorkNumber-1] = 1;
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
								break;
							}

							if(CheckHardLimit(g_iWorkNumber,pos) || CheckSoftLimit(g_iWorkNumber,pos))
							{
								Stop1(g_iWorkNumber);
								break;
							}

							OSTimeDly(1);
						}

						if(g_Ini.iAxisRunMode[3]==0)
							mv=GetAngle(g_iMoveAxis,TRUE);
						else if(g_Ini.iAxisRunMode[3]==1)
						{
							curPos=(INT32S)(GetMM(g_iMoveAxis,TRUE));
							QuanPos = curPos/360;
							AnglePos = curPos%360;

							mv = QuanPos + AnglePos/1000.0;
						}
						else if(g_Ini.iAxisRunMode[3]==2)
							mv=GetMM(g_iMoveAxis,TRUE);

						break;

					default:
						break;
					}

				}
				else
				{
					SetMM(7,0,FALSE);
					SetMM(8,0,FALSE);
				}
			}
			else																//手动模式下手轮加工
			{
				if(pos>0)
				{
					if(g_lStepNode==0 && g_bModify)
					{
						INT32S t1,t2;

						OSSemPost(UnpackDataSem);

						t1 = OSTimeGet();

						g_bUnpackCorrect=FALSE;

						while(TRUE)
						{
							OSTimeDly(2);

							if(g_bUnpackCorrect)
								break;

							t2 = OSTimeGet();

							if((t2-t1)>100 || (t2-t1<-100))
							{
								SetMM(7,0,FALSE);
								SetMM(8,0,FALSE);

								return;
							}
						}
					}

					if(!g_bUnpackCorrect)
					{
						OSQPost(MsgQueue,g_bEnglish?"The data exist error,Can't run!":"编程数据错误，不能执行手轮加工操作!");

						SetMM(7,0,FALSE);
						SetMM(8,0,FALSE);
					}
					else
					{
						SetMM(7,0,FALSE);
						SetMM(8,0,FALSE);

						g_lHandRun     = -1;
						//g_bStepIO      = TRUE;
						g_bDispRunLine = TRUE;
						g_bAutoRunMode = TRUE;

						g_bTestStart   = TRUE;
						g_fSpeedBi     = 0.2;

						RunAllData();

						//g_bHandRun = FALSE;
						//g_bStepIO  = FALSE;
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
}


void	CTeach_OnKey(INT16S key,INT16S tkey)
{
	COMMAND *ptr;
	INT16S  row=0;
	static  char disp[0x05]="\0";
	static  int  dispjs=0;
	INT16S temp;

	if(key=='.'|| key=='7'|| key=='9' || key=='6')
	{
		if(key == '.')
			disp[dispjs++] = '.';
		else
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
		char buf[0x60] = "项目号:";
		char bufE[0x60]= "No.:";
		INT16U value,libver;

		value=Har_Ver1();														//获取FPGA硬件版本号

		libver = Lib_Ver();

		g_bInsertNum = FALSE;

		g_iCurEditNo = 3;

		if(g_bEnglish)
		{
			memset(disp,NULL,5);
			strcat(bufE,PROJECT_ID);
			strcat(bufE,"\nSW.Ver:V");

			{
				INT8U buf1[6]="\0";

				if(!ftoa((FP32)VER*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}

				strcat(bufE,(INT8S *)buf1);
			}

			{
				char buf2[0x20]="\0";

				sprintf(buf2,"%s%0x","\nHW.Ver:",value);

				strcat(bufE,(INT8S *)buf2);
			}

			strcat(bufE,"\nData:");
			strcat(bufE,__DATE__);
			strcat(bufE,"\nTime:");
			strcat(bufE,__TIME__);

			strcat(bufE,"\nLib Ver:V");

			{
				INT8U buf1[6]="\0";

				if(!ftoa((FP32)libver*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}

				strcat(bufE,(INT8S *)buf1);
			}
		}
		else
		{
			memset(disp,NULL,5);
			strcat(buf,"DB016A027B-TH08HB-A01");
			strcat(buf,"\n软件版本:V");

			{
				INT8U buf1[6]="\0";

				if(!ftoa((FP32)VER*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}

				strcat(buf,(INT8S *)buf1);
			}

			{
				char buf2[0x20]="\0";

				sprintf(buf2,"%s%0x","\n硬件版本:",value);

				strcat(buf,(INT8S *)buf2);
			}

			strcat(buf,"\n日期:");
			strcat(buf,__DATE__);
			strcat(buf,"\n时间:");
			strcat(buf,__TIME__);

			strcat(buf,"\n库版本:V");

			{
				INT8U buf1[6]="\0";

				if(!ftoa((FP32)libver*0.01,buf1,2,0))
				{
					Uart_Printf("BfToStr OverFlow!\n");
				}

				strcat(buf,(INT8S *)buf1);
			}
		}

		for(a=5; a<8; a++)
			SuspendTasks(a);

		SuspendTasks(0);

		OSTimeDly(50);
		CAfxMsgDlg_Init(200,120,400,280,g_bEnglish?bufE:buf,g_bEnglish?"MESSAGE":"信息提示",g_TextColor1);
		CAfxMsgDlg_DoModal();
		CAfxMsgDlg_Destroy();

		for(a=5; a<8; a++)
			ResumeTasks(a);

		ResumeTasks(0);

		g_iCurEditNo = (g_bInterface?0:9);

		if(g_bInterface)
			UpdateFileDestProbe(0,0,0);

		CTeach_Create();

		return;
	}
	//将该参数设置开放
	/*else if(strcmp(disp,"76..") == 0)
	{
		INT8S a = 0;
		INT8U iZeroX = 0;
		INT8U iZeroZ = 0;
		INT8U iZeroA = 0;

		g_bInsertNum = FALSE;

		memset(disp,NULL,5);

		for(a=5; a<8; a++)
			SuspendTasks(a);

		SuspendTasks(0);

		OSTimeDly(50);

		iZeroX = g_Ini.iServoZero[0];
		iZeroA = g_Ini.iServoZero[3];
		iZeroZ = g_Ini.iServoZero[2];

		g_iCurEditNo = 3;

		MachPara_Init(0,0,798,478);
		MachPara_DoModal();
		MachPara_Destroy();

		if(iZeroX!=g_Ini.iServoZero[0] || iZeroZ!=g_Ini.iServoZero[2] || iZeroA!=g_Ini.iServoZero[3])
		{
			g_bFirstResetX=FALSE;
			g_bFirstResetY=FALSE;
			g_bFirstResetZ=FALSE;
			g_bFirstResetU=FALSE;
		}

		for(a=5; a<8; a++)
			ResumeTasks(a);

		ResumeTasks(0);

		g_iCurEditNo = (g_bInterface?0:9);

		if(g_bInterface)
			UpdateFileDestProbe(0,0,0);

		CTeach_Create();
		return;
	}*/

	if(g_bEMSTOP)
	{
		//	if(key!=TBACKZERO && key!=TX && key!=TY && key!=TZ && key!=TSET )

		if(key== TRUN || key==TSTEP || key==THANDMOVE)//启动，单条，手轮加工
		{
			OSQPost(MsgQueue,g_bEnglish?"key is invalid after emergency stop":"紧急停止后该按键无效,请先归零!");
			return;
		}
	}

	if(!g_bDispServoWaring)
		g_bDispServoWaring = TRUE;

	if(g_bInterface && g_iCurEditNo==9 && (key>='0' && key<='9' || key==TMINUS || key=='.' || key==TCLEAR))
	{
		return;
	}

	g_bInsertNum = FALSE;

	switch(key)
	{
#ifdef	PRINTBMP														//屏幕截图 启动键

	case F5:
	{
		char   filename[0x20];
		INT16S i=0;

		strcpy(filename,"thfrm");

		for(i=0; i<strlen(filename); i++)
		{
			if(filename[i] == '.')
			{
				filename[i] = '\0';
				break;
			}
		}

		strcat(filename,".bmp");

		CreatBMP(filename,800,480,1);

		OSQPost(MsgQueue,"Write Bmp End!");
	}
	break;

#endif

	case TTEACH:															//教导

		/*	if(PressKeyTime(2000) && g_bInterface)
			{
				INT16S a=0;

				for(a=5; a<8; a++)
					SuspendTasks(a);

				UsbMain();

				for(a=5; a<8; a++)
					ResumeTasks(a);

				CTeach_Create();
			}
			else */
	{
		if(!g_bInterface  && (CList_StartRow()+CList_CurRow())>0 && g_iWorkNumber!=0)
		{
			if( (Line_Data+CList_StartRow() + CList_CurRow() )->value[0][0] != 'N' )
			{
				FP32 mv = 0.0;
				INT32S curPos =0;
				INT32S QuanPos = 0;
				INT32S AnglePos=0;

				switch(g_iWorkNumber)
				{
				case 1:
					mv = GetAngle(1,TRUE);
					break;

				case 2:
					mv = GetMM(2,TRUE);
					break;

				case 3:
					if(g_Ini.iAxisRunMode[2]==0)
					{
						mv = GetAngle(3,TRUE);
					}
					else if(g_Ini.iAxisRunMode[2]==1)
					{
						curPos=(INT32S)(GetMM(3,TRUE));
						QuanPos = curPos/360;
						AnglePos = curPos%360;
						Uart_Printf("curPos=%d,QuanPos=%d,AnglePos=%d",curPos,QuanPos,AnglePos);
						mv = (FP32)QuanPos + AnglePos/1000.0;
					}
					else if(g_Ini.iAxisRunMode[2]==2)
					{
						mv = GetMM(3,TRUE);
					}
					break;

				case 4:
					if(g_Ini.iAxisRunMode[3]==0)
					{
						mv = GetAngle(4,TRUE);
					}
					else if(g_Ini.iAxisRunMode[3]==1)
					{
						curPos=(INT32S)(GetMM(4,TRUE));
						QuanPos = curPos/360;
						AnglePos = curPos%360;

						mv = QuanPos + AnglePos/1000.0;
					}
					else if(g_Ini.iAxisRunMode[3]==2)
					{
						mv = GetMM(4,TRUE);
					}
					break;

				default:
					break;
				}

				if( g_iWorkNumber==3||g_iWorkNumber==4)
				{
					if(g_Ini.iAxisRunMode[g_iWorkNumber-1]==0)
						CEdit_SetFloat(mv,1);
					else if(g_Ini.iAxisRunMode[g_iWorkNumber-1]==1)
						CEdit_SetFloat(mv,3);
					else if(g_Ini.iAxisRunMode[g_iWorkNumber-1]==2)
						CEdit_SetFloat(mv,2);
				}
				else
				{
					CEdit_SetFloat(mv,1);
				}

				CEdit_UpdateData(FALSE);

				if(!CTeach_DataOk())
					break;

				CTeach_Update(0,1);

				SetMM(2,0,TRUE);

				WriteDataToFile( g_Ini.iFileName );	//自动保存加工文件

			}
		}
	}
	break;

	case TAIR:
		if(g_lStepNode != 0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't enter state of cylinder test at running":"停机模式下进入气缸测试状态");
			break;
		}

		g_bCylinder = g_bCylinder?FALSE:TRUE;

		if(g_bCylinder)
		{
			OSQPost(MsgQueue,g_bEnglish?"Enter state of cylinder test":"进入汽缸测试状态!");
		}
		else
		{

			INT16S i=0;

			for(i=0; i<MAXCYLINDER; i++)
				WriteBit(i,0);


			key_set_led(AIRALED,LEDCLOSE);
			key_set_led(AIRBLED,LEDCLOSE);
			key_set_led(AIRCLED,LEDCLOSE);
			key_set_led(AIRDLED,LEDCLOSE);

			if(g_bInterface)
				UpdateCylinder(FALSE);

			OSQPost(MsgQueue,g_bEnglish?"Exit state of cylinder test":"退出汽缸测试状态!");
		}

		break;

	case TAIRA:

	{
		//static BOOL IsOpen = FALSE;

		//IsOpen = IsOpen?FALSE:TRUE;

		if(!GetOut(0))
		{
			WriteBit(0,1);

			key_set_led(AIRALED,LEDOPEN);

			OSQPost(MsgQueue,g_bEnglish?"Open Air 1":"打开气缸1");
		}
		else
		{
			WriteBit(0,0);

			key_set_led(AIRALED,LEDCLOSE);

			OSQPost(MsgQueue,g_bEnglish?"Close Air 1":"关闭气缸1");
		}
		if(g_bInterface)
			UpdateCylinder(FALSE);
	}
	break;

	case TAIRB:

	{
		//static BOOL IsOpen = FALSE;

		//IsOpen = IsOpen?FALSE:TRUE;

		if(!GetOut(1))
		{
			WriteBit(1,1);

			key_set_led(AIRBLED,LEDOPEN);

			OSQPost(MsgQueue,g_bEnglish?"Open Air 2":"打开气缸2");
		}
		else
		{
			WriteBit(1,0);

			key_set_led(AIRBLED,LEDCLOSE);

			OSQPost(MsgQueue,g_bEnglish?"Close Air 2":"关闭气缸2");
		}
		if(g_bInterface)
			UpdateCylinder(FALSE);
	}

	break;

	case TAIRC:

	{
		//static BOOL IsOpen = FALSE;

		//IsOpen = IsOpen?FALSE:TRUE;

		if(!GetOut(2))
		{
			WriteBit(2,1);
			key_set_led(AIRCLED,LEDOPEN);
			OSQPost(MsgQueue,g_bEnglish?"Open Air 3":"打开气缸3");
		}
		else
		{
			WriteBit(2,0);
			key_set_led(AIRCLED,LEDCLOSE);
			OSQPost(MsgQueue,g_bEnglish?"Close Air 3":"关闭气缸3");
		}
		if(g_bInterface)
			UpdateCylinder(FALSE);
	}
	break;

	case TAIRD:

	{
		//static BOOL IsOpen = FALSE;

		//IsOpen = IsOpen?FALSE:TRUE;

		if(!GetOut(3))
		{
			WriteBit(3,1);
			key_set_led(AIRDLED,LEDOPEN);
			OSQPost(MsgQueue,g_bEnglish?"Open Air 4":"打开气缸4");
		}
		else
		{
			WriteBit(3,0);
			key_set_led(AIRDLED,LEDCLOSE);
			OSQPost(MsgQueue,g_bEnglish?"Close Air 4":"关闭气缸4");
		}
		if(g_bInterface)
			UpdateCylinder(FALSE);
	}
	break;

	case THANDBOX: 															//手轮，倍率设置
		CTeach_UpdateSpeed(1);

		if(g_iSpeedBeilv==3)
		{
			key_set_led(HANDBOXLED,LEDOPEN);
		}
		else
		{
			key_set_led(HANDBOXLED,LEDCLOSE);
		}

		break;

	case THANDMOVE:															//手动，模式键
	{
		//if(CheckWarning(TRUE))
		//return;

		g_bMode = g_bMode?FALSE:TRUE;

		if(g_bMode)
		{
			key_set_led(HANDLELED,LEDOPEN);
			OSQPost(MsgQueue,g_bEnglish?"Goto HandMode":"进入手动模式!");
		}
		else
		{
			key_set_led(HANDLELED,LEDCLOSE);
			SetMM(7,0,FALSE);
			SetMM(8,0,FALSE);
			OSQPost(MsgQueue,g_bEnglish?"Goto AutoMode":"进入自动模式!");
		}

		CTeach_ModeDisp();
	}
	break;

	case TLOOP:																//循环指令
		if(!g_bInterface)
		{
			INT32S index=0;
			INT32S i=0;
			COMMAND *ptr;
			static BOOL LoopStyle = FALSE;

			LoopStyle = LoopStyle?FALSE:TRUE;

			index = CList_StartRow() + CList_CurRow();

			if( index == 0 )
				break;

			g_bModify = TRUE;
			g_bModifySave=FALSE;

			ptr = Line_Data + index;

			if(ptr->value[0][0] != 'N')
			{
				for( i=Line_Data_Size; i>index; i--)
					*(Line_Data+i) = *(Line_Data+i-1);

				DataCls(index);
				DataPut(index,0,"N");
				DataPut(index,1,"次");

				Line_Data_Size ++;
				LoopStyle = TRUE;
			}

			DataPut(index,2,LoopStyle?"1":"0");								//循环模式选择（0:顺序循环123，1:倒序循环123--321--123）

			CTeach_InvidateView(FALSE);
			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
		}
		break;

	case TSET:																//参数选择界面
		temp=g_iCurEditNo;
		g_iCurEditNo = 3;
		CTeach_ParaSelect();

		if(g_bInterface && g_iCurEditNo!=0)									//如果处于主界面，进入设置界面然后退出到主界面，并且IsCreate没有置为TRUE，即不会重新CTeach_Create时
		{
			g_iCurEditNo = (g_bInterface?temp:9);							//保持为temp不变
			UpdateFileDestProbe(0,0,0);
		}																	//如果IsCreate被置为TRUE，则g_iCurEditNo被置为0

		SetMM(7,0,FALSE);
		SetMM(8,0,FALSE);
		break;

	case TPROBESET:															//探针设置

		if(g_bInterface)
		{
			OSQPost(MsgQueue,g_bEnglish?"Probe settings":"设定探针失败次数");

			UpdateFileDestProbe(3,-1,0);
		}
		break;

	case TPROBECLR:															//探针清零

		if(g_bInterface)
		{
			g_lProbeFail=0;

			if(g_bInterface)
				UpdateSpeedCountProbe();

			OSQPost(MsgQueue,g_bEnglish?"Probe clear!":"探针清零!");
		}
		break;

	case TCOUNTCLR:															//计数清零
		if(g_bInterface)
		{
			INT8S a=0;

			for(a=5; a<8; a++)
				SuspendTasks(a);

			OSTimeDly(50);
			CMsgDlg_Init(250,130,300,200,g_bEnglish?"Clear processing Count?":"清除加工数量?",g_bEnglish?"Warning":"警告",REDCOLOR,g_bEnglish?"Yes":"是",g_bEnglish?"No":"否");

			if(CMsgDlg_DoModal())
			{
				g_Ini.lRunCount = 0;
				WriteBit(O_WARNING,0);
				SysParaWrite();
				WriteRunCount();
				WriteDataToFile(g_Ini.iFileName);

				CMsgDlg_Destroy();
				CTeach_Create();
				OSQPost(MsgQueue,g_bEnglish?"Reset Count":"加工数量清零!");

				for(a=5; a<8; a++)
					ResumeTasks(a);

				break;
			}
			else
			{
				CMsgDlg_Destroy();
				CTeach_Create();

				for(a=5; a<8; a++)
					ResumeTasks(a);
			}
		}
		break;

	case TDELALL:																//全删
	{
		INT8S a=0;

		if(g_bInterface)
			break;

		if(g_bMode || g_lStepNode!=0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't delete the file in processing ":"手动加工模式下或加工未结束时不能全删!");
			break;
		}

		for(a=5; a<8; a++)
			SuspendTasks(a);
		OSTimeDly(50);

		CMsgDlg_Init(250,130,300,200,g_bEnglish?"Delete all of data?":"删除所有数据吗?",g_bEnglish?"Warning":"警告",REDCOLOR,g_bEnglish?"Yes":"是",g_bEnglish?"No":"否");

		if(CMsgDlg_DoModal())
		{
			CTeach_DeleteAll();
			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
			CMsgDlg_Destroy();

			for(a=5; a<8; a++)
				ResumeTasks(a);
		}
		else
		{
			CMsgDlg_Destroy();

			for(a=5; a<8; a++)
				ResumeTasks(a);
		}

		CTeach_Create();
	}
	break;

	case TSAVE:
		if(g_iCurEditNo==0) 									//另存为功能
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

		if(g_lStepNode==0)
		{
			INT32S t1,t2;

			OSSemPost(UnpackDataSem);

			t1 = OSTimeGet();

			g_bUnpackCorrect=FALSE;

			while(TRUE)
			{
				OSTimeDly(2);

				if(g_bUnpackCorrect)
					break;

				t2 = OSTimeGet();

				if((t2-t1)>200 || (t2-t1<-200))
					return;
			}
		}

		OSQPost(MsgQueue,g_bEnglish?"Save...":"保存...");

		WriteDataToFile(g_Ini.iFileName);
		WriteRunCount();
		g_bModifySave = TRUE;
		g_bModify = TRUE;

		OSQPost(MsgQueue,g_bEnglish?"Saved":"文件保存!");
		break;

	case TX:																//X轴选择

		if(!g_bInterface)
		{
			if(CList_CurCol()==1 || CList_CurCol()==2)
				break;

			if( CList_CurCol() > 1 )
				CTeach_Update(0,1-CList_CurCol());
		}
		else
			g_iWorkNumber=1;
		UpdateCurAxis(g_iWorkNumber);
		/*
			key_set_led(XLED,LEDOPEN);
			key_set_led(YLED,LEDCLOSE);
			key_set_led(ZLED,LEDCLOSE);
		 */
		break;

	case TY:																//Y轴选择
		if(!g_bInterface)
		{
			if( CList_CurCol()==3 )
				break;

			CTeach_Update(0,3-CList_CurCol());
		}
		else
			g_iWorkNumber=2;

		UpdateCurAxis(g_iWorkNumber);
		/*
			key_set_led(XLED,LEDCLOSE);
			key_set_led(ZLED,LEDCLOSE);
			key_set_led(YLED,LEDOPEN);
		 */
		break;

	case TZ:
		if(g_Ini.iAxisCount > 2)											//Z轴选择
		{
			if(!g_bInterface)
			{
				if(CList_CurCol()==4)
					break;

				CTeach_Update(0,4-CList_CurCol());
			}
			else
				g_iWorkNumber = 3;

			UpdateCurAxis(g_iWorkNumber);
			/*
				key_set_led(YLED,LEDCLOSE);
				key_set_led(XLED,LEDCLOSE);
				key_set_led(ZLED,LEDOPEN);
			 */
		}
		break;

	case TRATE:
		if(!g_bInterface)
		{
			if(CList_CurCol()==CList_MaxCol()-3)
				break;

			CTeach_Update(0,CList_MaxCol()-3-CList_CurCol());
		}
		break;

	case TSCREEN:															//界面切换
	{
		g_bInterface = g_bInterface?FALSE:TRUE;
		OSTimeDly(20);
		//Uart_Printf();
		CTeach_Create();
		//Uart_Printf();
	}
	break;

	case TINSERT:															//列表插入编辑
		if(!g_bInterface)
		{
			CTeach_Insert();
			CTeach_InvidateView(FALSE);

			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
		}
		break;

	case TDEL:																//列表删除编辑
		if(!g_bInterface)
		{
			INT16S  row,size,i;
			COMMAND *ptr1;

			row = CList_CurRow()+CList_StartRow();
			ptr1= Line_Data+row;

			CTeach_Delete();

			if( row >= Line_Data_Size )
			{
				CTeach_Update(-1,0);
				break;
			}

			size = Line_Data_Size;

			if( size < 1 )
				return;

			size = size - CList_StartRow();									//显示个数

			if( size > ROWS )
				size = ROWS;												//显示个数调整

			for(i=0; i<size; i++)
				CTeach_DisplayLine(i+CList_StartRow(),i,Draw,g_TextColor6,g_nTableBk);

			if( size<ROWS )
			{
				CTeach_DisplayLine(size+CList_StartRow(),size,Reset,g_TextColor6,g_nTableBk);
			}

			CTeach_Update(0,0);

			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;

		}
		break;

	case TUP:																//编辑框向上移动
		if(!g_bInterface)
		{
			INT16S row = CList_CurRow()+CList_StartRow();
			COMMAND *ptr;

			ptr = Line_Data+row;

			if((g_Ini.iAxisCount<3 && ptr->value[0][0]==NULL && ptr->value[1][0]==NULL && ptr->value[2][0]==NULL) ||
					(g_Ini.iAxisCount>2 && ptr->value[0][0]==NULL && ptr->value[1][0]==NULL && ptr->value[2][0]==NULL && ptr->value[3][0]==NULL))
			{
				CTeach_OnKey(TDEL,0);
			}
			else
			{
				CTeach_Update(-1,0);
			}
		}
		else
		{
			INT16S tmp = g_iCurEditNo;

			switch(g_iCurEditNo)
			{
			case 0:
			case 1:
			case 2:
				tmp--;

				if( tmp<0 )
					tmp = 2;

				UpdateFileDestProbe(tmp+1,-1,0);
				break;
			default:
				break;
			}
		}

		break;

	case TDOWN:																//编辑框向下移动
		if(!g_bInterface)
		{
			INT16S row = CList_CurRow()+CList_StartRow();

			ptr = Line_Data+row;

			if( row == Line_Data_Size -1 )									//增加一个
			{
				INT16S newcol,oldcol=CList_CurCol();

				CTeach_AddData();
				CList_SetCurCol(1);

				newcol = CList_CurCol();

				if(oldcol<=5 && newcol>5)
				{
					//	Lcd_MoveViewPort(320,0,0);
				}
				else
				{
					if(oldcol>5 && newcol<=5)
						;//	Lcd_MoveViewPort(0,0,0);
				}

				CTeach_Update(1,0);
				UpdateCurAxis(g_iWorkNumber);

				ptr = Line_Data+CList_CurRow()+CList_StartRow();

				if(ptr->value[0][0]=='M')
				{
					CEdit_SetFont(FONT24);
					CEdit_SetFloat(GetAngle(1,TRUE),1);
					CEdit_UpdateData( FALSE );
				}
			}
			else
			{
				CTeach_Update(1,0);
				UpdateCurAxis(g_iWorkNumber);
			}
		}
		else
		{
			INT16S tmp = g_iCurEditNo;

			switch(g_iCurEditNo)
			{
			case 0:
			case 1:
			case 2:
				tmp++;

				if( tmp>2 )
					tmp = 0;

				UpdateFileDestProbe(tmp+1,-1,0);
				break;
			default:
				break;
			}
		}
		break;

	case TPAGEUP:															//向上翻页
		if(!g_bInterface)
		{
			CTeach_UpdatePage(-ROWS);
		}
		break;

	case TPAGEDOWN:															//向下翻页
		if(!g_bInterface)
		{
			CTeach_UpdatePage(ROWS);
		}
		break;

	case TRIGHT:															//编辑框向右移动
	{
		INT16S tmp = g_iCurEditNo;

		switch(g_iCurEditNo)
		{
		case 0:
		case 1:
		case 2:
			tmp++;

			if( tmp>2 )
				tmp = 0;

			UpdateFileDestProbe(tmp+1,-1,0);
			break;

		case 9:
			if(CList_CurRow()+CList_StartRow() >= Line_Data_Size-1 && CList_CurCol() >= CList_MaxCol()-1)
				break;

			if(CList_CurCol() == CList_MaxCol()-1)
			{
				CTeach_Update(1,1-CList_CurCol());
			}
			else
				CTeach_Update(0,1);

			UpdateCurAxis(g_iWorkNumber);
			break;

		default:
			break;
		}
	}
	break;

	case TLEFT:																//编辑框向右移动
	{
		INT16S tmp = g_iCurEditNo;

		switch(g_iCurEditNo)
		{
		case 0:
		case 1:
		case 2:
			tmp--;

			if( tmp<0 )
				tmp = 2;

			UpdateFileDestProbe(tmp+1,-1,0);
			break;

		case 9:
			if(CList_CurCol() == 1 && CList_CurRow() == 0 && CList_StartRow() ==0)
				break;

			if(CList_CurCol() == 1 )
			{
				CTeach_Update(-1,CList_MaxCol()-1-CList_CurCol());
			}
			else
				CTeach_Update(0,-1);

			UpdateCurAxis(g_iWorkNumber);
			break;

		default:
			break;
		}
	}
	break;

	case TENTER:															//确定键输入
	{
		switch(g_iCurEditNo)
		{
		case 0:
		{
			INT16S itemp = g_Ini.iFileName;

			UpdateFileDestProbe(1,1,1);

			g_iCurEditNo = 1;

			if(itemp != g_Ini.iFileName)
			{

				g_bInterface = FALSE;

				OSTimeDly(10);

				CTeach_Create();
			}
			else
				UpdateFileDestProbe(2,-1,0);

		}
		return;

		case 1:
			UpdateFileDestProbe(2,1,1);
			g_iCurEditNo = 2;
			UpdateFileDestProbe(3,-1,0);
			return;

		case 2:
			UpdateFileDestProbe(3,1,1);
			g_iCurEditNo = 0;
			UpdateFileDestProbe(1,-1,0);
			return;

		default:
			break;
		}

		if(!g_bInterface)
		{
			if(!CTeach_DataOk())								//该函数校验输入的数据的正确性，同时将m_bString值存储到ptr->value[]中
				break;

			row = CList_CurRow()+CList_StartRow();

			if( row >= Line_Data_Size -1 && CList_CurCol() == CList_MaxCol()-1)
			{
				CTeach_AddData();
				CList_SetCurCol(1);
				//CTeach_InvidateView(FALSE);
				CTeach_Update(1,0);
				UpdateCurAxis(g_iWorkNumber);

				ptr = Line_Data+CList_CurRow()+CList_StartRow();

				if(CList_StartRow()+CList_CurRow()>=Line_Data_Size)
				{
					return;
				}

				if(ptr->value[0][0]=='M')
				{
					CEdit_SetFont(FONT24);
					CEdit_SetFloat(GetAngle(1,TRUE),1);
					CEdit_UpdateData( FALSE );
				}

				WriteDataToFile( g_Ini.iFileName );
				g_bModifySave = TRUE;
				g_bModify = TRUE;
				break;
			}

			else if( CList_CurCol() == 3 )
			{

				if(g_Ini.iSendClear>0 && g_iWorkStatus==NORUN && g_lStepNode==0)   //确认键不具备将当前数值存入编辑框功能，因此此处注释掉清空Y轴的代码
				{
					SetMM(2,0,TRUE);
					g_bFirstResetY=FALSE;
					DisplayXYZ(TRUE);
				}
			}

			if( CList_CurCol() == CList_MaxCol()-1 )
			{
				CTeach_Update(1,-CList_CurCol());
				UpdateCurAxis(g_iWorkNumber);
				WriteDataToFile( g_Ini.iFileName );
				g_bModifySave = TRUE;
				g_bModify = TRUE;
				break;
			}

			CTeach_Update(0,1);
			UpdateCurAxis(g_iWorkNumber);
			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
		}
	}
	break;

	case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8':						//1-8数字键输入,5-8号气缸没有被显示

		if(g_bCylinder)
		{
			INT16S temp = key-'1';

			WriteBit(temp,GetOut(temp)?0:1);

			if(GetOut(temp))
				sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT ON":"%1d号汽缸伸出",temp+1);
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT OFF":"%1d号汽缸退回",temp+1);

			OSQPost(MsgQueue,g_pszMsgBuf);

			if(g_bInterface)
				UpdateCylinder(FALSE);


			switch(temp)
			{
			case 0:key_set_led(AIRALED,GetOut(temp)?LEDOPEN:LEDCLOSE);break;
			case 1:key_set_led(AIRBLED,GetOut(temp)?LEDOPEN:LEDCLOSE);break;
			case 2:key_set_led(AIRCLED,GetOut(temp)?LEDOPEN:LEDCLOSE);break;
			case 3:key_set_led(AIRDLED,GetOut(temp)?LEDOPEN:LEDCLOSE);break;
			default:break;
			}
			break;
		}																	//转到下一个CASE

	case '9':case '0':case '.':case TMINUS:											//0.-清楚键输入

		g_bInsertNum = TRUE;

	case TCLEAR:
		if(key == TMINUS)
			key = '-';

		CEdit_OnKey( key, tkey );

		if(g_iCurEditNo == 9)
		{
			g_bEditData=TRUE;
		}

		break;

	case TYSWITCH:
	{
		g_Ini.iAxisSwitch[1] = g_Ini.iAxisSwitch[1]?0:1;

		g_bModify = TRUE;
		g_bModifySave = FALSE;

		if(g_Ini.iAxisSwitch[1]==1)
		{
			key_set_led(YSWITCHLED,LEDCLOSE);

			g_lAxisValue[1] = GetPulse(2,TRUE);
			//g_bFirstResetY=FALSE;
		}
		else
		{
			key_set_led(YSWITCHLED,LEDOPEN);

			SetPulse(2,g_lAxisValue[1],TRUE);
		}

		if(!g_bInterface)
			UpdateAxisStatus();

		SysParaWrite();

		break;
	}

	case TZSWITCH:
	{
		if(g_Ini.iAxisCount<3)
			break;

		g_bModify = TRUE;
		g_bModifySave = FALSE;

		g_Ini.iAxisSwitch[2] = g_Ini.iAxisSwitch[2]?0:1;

		if(g_Ini.iAxisSwitch[2]==1)
		{
			key_set_led(ZSWITCHLED,LEDCLOSE);

			g_lAxisValue[2] = GetPulse(3,TRUE);
			g_bFirstResetZ=FALSE;
		}
		else
		{
			key_set_led(ZSWITCHLED,LEDOPEN);

			SetPulse(3,g_lAxisValue[2],TRUE);
		}

		if(!g_bInterface)
			UpdateAxisStatus();

		SysParaWrite();

		break;
	}


	case TSTEP:																//单条，停止键
		if(CheckWarning(TRUE))
			return;
		//			if(g_Ini.iStopMode==1)
		{
			INT16S i=0;

			if(g_bCylinder)
			{
				g_bCylinder = FALSE;

				for(i=0; i<MAXCYLINDER; i++)
					WriteBit(i,0);
			}

			if(g_lStepNode==0 && g_bModify)
			{
				INT32S t1,t2;

				OSSemPost(UnpackDataSem);

				t1 = OSTimeGet();

				g_bUnpackCorrect=FALSE;

				while(TRUE)
				{
					OSTimeDly(2);

					if(g_bUnpackCorrect)
						break;

					t2 = OSTimeGet();

					if((t2-t1)>200 || (t2-t1<-200))
						return;
				}
			}

			if(g_bMode)
			{
				g_bMode = FALSE;
				CTeach_ModeDisp();
				key_set_led(HANDLELED,LEDCLOSE);
			}


			g_bAutoRunMode = FALSE;
			g_iWorkStatus=SINGLERUN;

			if(g_lStepNode!=0)
			{
				g_bStepToSingle = TRUE;
			}

			//g_bStepIO     = FALSE;
			g_bDispRunLine= FALSE;
			g_bTestStart  = FALSE;
			g_bSingleRun=TRUE;

			RunAllData();

		}
		break;

	case TRUN:																//启动键
	{
		if(CheckWarning(TRUE))
			return;

		if(g_bCylinder)
		{
			g_bCylinder = FALSE;
		}

		if(!g_bInterface)
		{
			if(g_iCurEditNo!=9)
			{
				UpdateFileDestProbe(g_iCurEditNo+1,1,0);
				g_iCurEditNo=9;
			}
		}

		if(g_lStepNode!=0)
		{
			g_bStepToSingle = TRUE;
		}


		if(g_lStepNode==0 && g_bModify)
		{
			INT32S t1,t2;

			OSSemPost(UnpackDataSem);

			t1 = OSTimeGet();

			g_bUnpackCorrect=FALSE;

			while(TRUE)
			{
				OSTimeDly(2);

				if(g_bUnpackCorrect)
					break;

				t2 = OSTimeGet();

				if((t2-t1)>200 || (t2-t1<-200))
				{
					//Uart_Printf("test\n");
					return;
				}
			}
		}

		if(g_bMode)
		{
			g_bMode = FALSE;

			CTeach_ModeDisp();

			key_set_led(HANDLELED,LEDCLOSE);
		}


		g_bEMSTOP     = FALSE;
		g_bAutoRunMode= TRUE;
		g_bTestStart  = FALSE;
		g_bStepIO     = FALSE;
		g_bHandRun =  FALSE;
		g_bDispRunLine= FALSE;
		g_bSingleRun=FALSE;
		RunAllData();

	}
	break;

	case TBACKZERO:															//归零键
	{
		INT16S i=0;

		if(CheckWarning(FALSE))
			return;

		SuspendTasks(0);
		OSTimeDly(50);
		ResumeTasks(1);
		//g_bStepIO=FALSE;
		if(CTeach_BackZero(1))
		{
			g_lStepNode   = 0;
			g_iStaRunLine = 0;
			g_iRunLine    = 0;

			SetMM(2,0,TRUE);

			if(g_Ini.iAxisCount>2)
			{
				if(g_Ini.iAxisSwitch[2]==1)
				{
					if(CTeach_BackZero(3))
					{
						if(g_Ini.iAxisCount<4)
						{
							g_bEMSTOP = FALSE;
							DisplayXYZ(TRUE);
							SuspendTasks(1);
							OSTimeDly(50);
							ResumeTasks(0);
							break;
						}
					}
					else
					{

						SuspendTasks(1);
						OSTimeDly(50);
						ResumeTasks(0);
						break;
					}
				}
				else
				{
					if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
					{
						g_bEMSTOP=FALSE;
						SuspendTasks(1);
						OSTimeDly(50);
						ResumeTasks(0);
						break;
					}
				}

				if(g_Ini.iAxisSwitch[3]==1 && g_Ini.iAxisCount>3)
				{
					if(CTeach_BackZero(4))
					{
						g_bEMSTOP = FALSE;
					}
					else
					{
						SuspendTasks(1);
						OSTimeDly(50);
						ResumeTasks(0);
						break;
					}
				}
				else
				{
					g_bEMSTOP = FALSE;
				}

			}
			else
			{
				g_bEMSTOP = FALSE;
			}
		}

		DisplayXYZ(TRUE);
		SuspendTasks(1);
		OSTimeDly(50);
		ResumeTasks(0);
		//g_bEMSTOP = FALSE;
	}
	break;

	default:
		break;
	}
}


BOOL	CTeach_BackZero(int ch)
{
	BOOL  tmpflag=FALSE;

	if( ch==2 )
	{
		SetMM(2,0,TRUE);
		return TRUE;
	}
	else
	{
		if(ReadBit(STOPIO)==0 || ReadBit(STOPIO2)==0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"紧急停机状态!进行操作前请抬起急停按钮!");
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
				if(FindZero(ch,g_Ini.iBackMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
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
				if(FindZero(ch,g_Ini.iBackMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
					tmpflag=TRUE;
				else
					tmpflag=FALSE;
			}
			break;

		case 4:
			OSQPost(MsgQueue,g_bEnglish?"U Reset...":"U归零...");

			if(!g_bFirstResetU)
			{
				if( BackZero(ch,g_Ini.iBackDir[ch-1]))
					tmpflag=TRUE;
				else
					tmpflag=FALSE;
			}
			else
			{
				if(FindZero(ch,g_Ini.iBackMode[ch-1],g_Ini.fBackSpeed[ch-1]*GetUnitPulse(ch)))
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
		SetMM(2,0,TRUE);
		g_bFirstResetY=TRUE;
		SetMM(ch,0,TRUE);

		switch(ch)
		{
		case 1:
			g_bFirstResetX = TRUE;
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"X归零结束!");
			break;

		case 3:
			g_bFirstResetZ = TRUE;
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"Z归零结束!");
			break;

		case 4:
			g_bFirstResetU = TRUE;
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"U归零结束!");
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
		/*	switch(ch)
		{
			case 1:
				g_bFirstResetX = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"X归零故障,请确认原点信号是否正常!");
				break;

			case 3:
				g_bFirstResetZ = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Z归零故障,请确认原点信号是否正常!");
				break;

			case 4:
				g_bFirstResetU = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"U归零故障,请确认原点信号是否正常!");
				break;

			default:
				break;
		}
		 */
		if(g_iErrorNo)
		{
			RunWarningDisplay(g_iErrorNo);
		}
		else
		{
			switch(ch)
			{
			case 1:
				g_bFirstResetX = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"X归零故障,请确认原点信号是否正常!");
				break;

			case 3:
				g_bFirstResetZ = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Z归零故障,请确认原点信号是否正常!");
				break;

			case 4:
				g_bFirstResetU = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"U归零故障,请确认原点信号是否正常!");
				break;

			default:
				break;
			}
		}

		return FALSE;
	}
}


BOOL	CTeach_GotoStart(void)
{
	if(!g_bSequeBack)
	{
		g_bBackZero=TRUE;
		g_iErrorNo=0;

		//	    if(!g_bFirstResetX ||!g_bFirstResetY||!g_bFirstResetZ||!g_bFirstResetU) //add 12.17
		{
			if(! CTeach_BackZero(44))
			{
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"回原点失败!");
				g_bBackZero=FALSE;
				g_bFirstResetX=FALSE;
				g_bFirstResetY=FALSE;
				g_bFirstResetZ=FALSE;
				g_bFirstResetU=FALSE;
				//    			BackZeroLed();
				return  FALSE;
			}

			SetAngle(1,0,TRUE);
			g_bFirstResetX=TRUE;
			g_bFirstResetY=TRUE;
			g_bFirstResetZ=TRUE;
			g_bFirstResetU=TRUE;
			//   	    BackZeroLed();
		}

		//	    if(!g_bGotoStart[0] ||!g_bGotoStart[1]||!g_bGotoStart[2]||!g_bGotoStart[3])
		{
			if(! GotoMM4(1,2,3,4,g_Ini.fStartOffset[0],g_Ini.fStartOffset[1],g_Ini.fStartOffset[2],g_Ini.fStartOffset[3],g_Ini.fBackSpeed[1],FALSE))
			{
				OSQPost(MsgQueue,g_bEnglish?"Goto StartPos Error!":"回起点错误!");
				g_bBackZero=FALSE;
				g_bGotoStart[0]=FALSE;
				g_bGotoStart[1]=FALSE;
				g_bGotoStart[2]=FALSE;
				g_bGotoStart[3]=FALSE;
				//    		    BackZeroLed();
				return  FALSE;
			}

			SetAngle(1,0,TRUE);
			SetMM(2,0,TRUE);
			SetMM(3,0,TRUE);
			SetMM(4,0,TRUE);
			g_bGotoStart[0]=TRUE;
			g_bGotoStart[1]=TRUE;
			g_bGotoStart[2]=TRUE;
			g_bGotoStart[3]=TRUE;
			OSTimeDly(100);
			DisplayXYZ(TRUE);
		}

		OSQPost(MsgQueue,g_bEnglish?"Finish!":"完成!");
		g_lStepNode = 0;

		if(g_bMode)
		{
			g_bMode=FALSE;
			CTeach_ModeDisp();
			//		    CDC_OutText24Ex(563,406,g_bEnglish?"Auto":"自动",g_TextColor5,g_nBk);
			//			LedOut(TESTLED,g_bMode?1:0);										//灯亮表示进入测试模式
		}

		g_bBackZero=FALSE;
		//	    BackZeroLed();
		return  TRUE;
	} 																			//end if g_bSequeBack
	else
	{
		INT16S tempch=g_iWorkNumber;

		switch(tempch)
		{
		case 1:
			OSQPost(MsgQueue,g_bEnglish?"Goto X-StartPos...":"X轴回起点中...");
			break;

		case 2:
			OSQPost(MsgQueue,g_bEnglish?"Goto Y-StartPos...":"Y轴回起点中...");
			break;

		case 3:
			OSQPost(MsgQueue,g_bEnglish?"Goto Z-StartPos...":"Z轴回起点中...");
			break;

		case 4:
			OSQPost(MsgQueue,g_bEnglish?"Goto U-StartPos...":"U轴回起点中...");
			break;

		default:
			break;
		}

		g_bBackZero=TRUE;
		g_iErrorNo=0;

		switch(tempch)
		{
		case 1:
			SetAngle(1,0,TRUE);

			g_bFirstResetX = TRUE;

			if(!g_bGotoStart[0] )
			{
				if(! GotoMM(1,g_Ini.fStartOffset[0],g_Ini.fBackSpeed[0],FALSE))
				{
					g_bBackZero=FALSE;
					g_bGotoStart[0]=FALSE;
					//    				   BackZeroLed();
					OSQPost(MsgQueue,g_bEnglish?"Goto to X-StartPos Error":"X轴回起点错误!");
					return   FALSE;
				}

				SetAngle(tempch,0,TRUE);
				g_bGotoStart[0]=TRUE; 										//X轴回起点结束
			}
			break;

		case 2:
			if(! Home(tempch,g_Ini.iBackDir[tempch-1]))
			{
				OSQPost(MsgQueue,g_bEnglish?"Y Reset Error":"Y轴回原点失败!");
				g_bBackZero=FALSE;
				g_bFirstResetY=FALSE;
				//    				BackZeroLed();
				return   FALSE;
			}

			g_bFirstResetY=TRUE;

			if(!g_bGotoStart[1] )
			{
				if(! GotoMM(2,g_Ini.fStartOffset[1],g_Ini.fBackSpeed[1],FALSE))
				{
					OSQPost(MsgQueue,g_bEnglish?"Goto to Y-StartPos Error":"Y轴回起点错误!");
					g_bBackZero=FALSE;
					g_bGotoStart[1]=FALSE;
					//        				BackZeroLed();
					return FALSE;
				}

				SetMM(tempch,0,TRUE);
				g_bGotoStart[1]=TRUE;
			}
			break;

		case 3:
			if(! Home(tempch,g_Ini.iBackDir[tempch-1]))
			{
				OSQPost(MsgQueue,g_bEnglish?"Z Reset Error":"Z轴回原点失败!");
				g_bBackZero=FALSE;
				g_bFirstResetZ=FALSE;
				//					BackZeroLed();
				return FALSE;
			}

			g_bFirstResetZ=TRUE;

			if(!g_bGotoStart[2] )
			{
				if(! GotoMM(3,g_Ini.fStartOffset[2],g_Ini.fBackSpeed[2],FALSE))
				{
					OSQPost(MsgQueue,g_bEnglish?"Goto to Z-StartPos Error":"Z轴回起点错误!");
					g_bBackZero=FALSE;
					g_bGotoStart[2]=FALSE;
					//    				    BackZeroLed();
					return FALSE;
				}

				SetAngle(tempch,0,TRUE);
				g_bGotoStart[2]=TRUE;
			}
			break;

		case 4:
			if(!Home(tempch,g_Ini.iBackDir[tempch-1]))
			{
				OSQPost(MsgQueue,g_bEnglish?"U Reset Error!":"U轴回原点失败!");
				g_bBackZero=FALSE;
				g_bFirstResetU=FALSE;
				//                	BackZeroLed();
				return FALSE;
			}

			g_bFirstResetU=TRUE;

			if(!g_bGotoStart[3] )
			{
				if(!GotoMM(4,g_Ini.fStartOffset[3],g_Ini.fBackSpeed[3],FALSE))
				{
					OSQPost(MsgQueue,g_bEnglish?"Goto to U-StartPos Error":"U轴回起点错误!");
					g_bBackZero=FALSE;
					g_bGotoStart[3]=FALSE;
					//                		BackZeroLed();
					return FALSE;
				}

				SetAngle(tempch,0,TRUE);
				g_bGotoStart[3]=TRUE;
			}
			break;

		default:
			break;
		}

		DisplayXYZ(TRUE);
		OSQPost(MsgQueue,g_bEnglish?"Finish":"完成!");
		g_lStepNode = 0;

		if(g_bMode)
		{
			g_bMode=FALSE;
			CTeach_ModeDisp();
			//		    CDC_OutText24Ex(563,406,g_bEnglish?"auto":"自动",g_TextColor5,g_nBk);
			//			LedOut(TESTLED,g_bMode?1:0);										//灯亮表示进入测试模式
		}

		g_bBackZero=FALSE;
		//    	BackZeroLed();
	}

	return  TRUE;
}


BOOL	CTeach_ProcessError( COMMAND *ptr, INT16S col ,INT8S *string)
{
	if(strcmp(ptr->value[0],"YF")==0)
	{
		if((col == CList_MaxCol()-2 || col == CList_MaxCol()-1) && string[0])
		{
			OSQPost(MsgQueue,g_bEnglish?"Settings of speed is invalid":"速度数据设定无效");
			return FALSE;
		}

		if( col == 2 )
		{
			if( string[0] )
			{
				FP32 n = (FP32)stringtof((INT8U *)string);

				if( n < 0.01 || n >g_Ini.fXMaxSpeed )
				{
					OSQPost(MsgQueue,g_bEnglish?"Speed of X is too large":"X速度设定超出范围");
					return FALSE;
				}
			}
		}

		if( col == 1 )
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't change":"速度指令不能改变");
			return FALSE;
		}

	}

	else if(ptr->value[0][0] == 'N')
	{

		if(/*col == 3 ||*/ col == CList_MaxCol()-2 || col == CList_MaxCol()-1 || col == CList_MaxCol()-3)
		{
			OSQPost(MsgQueue,g_bEnglish?"Settings of Loop is invalid":"编辑无效");
			return FALSE;
		}
	}

	else
	{
		FP32 n = (FP32)stringtof( (INT8U *)string );

		if( col == 1 || col == 2 )
		{
			if( n>360.01 || n < -360.01) 
			{
				OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");									
				return FALSE;
			}
		}

		if( col == 3 && string[0] )
		{   
			if(n<-9999.99 || n > 99999.99)  
			{
				OSQPost(MsgQueue,g_bEnglish?"-9999.99<y<99999.99":"-9999.99<送线长度<99999.99!");		
				return FALSE;
			}

			if(strcmp(string,"+")==0||strcmp(string,"-")==0)
			{
				OSQPost(MsgQueue,g_bEnglish?"Err Code ":"错误符号");		
				return FALSE;
			}
			//SetMM(2,0,TRUE);
		}

		if(col==CList_MaxCol()-3 && string[0]!=NULL)
		{
			if( n>5.001 || n < 0.0099)
			{
				OSQPost(MsgQueue,g_bEnglish?"5.0>=Ratio>=0.01":"5.0>=速度比例>=0.01");
				return FALSE;
			}
		}

		if(g_Ini.iAxisCount>2)
		{
			if(col==CList_MaxCol()-4)
			{
				if(g_Ini.iAxisRunMode[2]==0)
				{
					if( n>360.01 || n < -360.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");					
						return FALSE;
					}
				}
				else if(g_Ini.iAxisRunMode[2]==1)
				{
					INT16S 	temp;
					FP32	tempf;
					FP64 	tt=n;

					temp=(INT16S)tt;						
					tempf=n-temp;

					if((tempf-0.359>0.001)||(tempf+0.359)<-0.001)	
					{
						OSQPost(MsgQueue,g_bEnglish?"Angle Err":"角度输入错误,小数部分应小于0.360");		
						return FALSE;
					}

					if(n<-49.359 || n > 49.359)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-49.359<y<49.359":"-49.359<角度<49.359!");		
						return FALSE;
					}
				}				
				//else
				{
					//if(g_Ini.iZSoftLimit)
					{
						if( n>=g_Ini.fMaxLimit[2] && (g_Ini.fMaxLimit[2]-g_Ini.fMinLimit[2])>0.01 )
						{
							OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"正限位越界");					
							return FALSE;
						}
						else if(n <= g_Ini.fMinLimit[2] && (g_Ini.fMaxLimit[2]-g_Ini.fMinLimit[2])>0.01)
						{
							OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"负限位越界");					
							return FALSE;
						}
					}
					if((strcmp(string,"+")==0||strcmp(string,"-")==0)&&(g_Ini.iAxisRunMode[2]==2))
					{
						OSQPost(MsgQueue,g_bEnglish?"Err Code ":"错误符号");		
						return FALSE;
					}	
				}
			}
		}

		if(g_Ini.iAxisCount>3)
		{
			if(col==5)
			{
				if(g_Ini.iAxisRunMode[3]==0)							//A单圈模式
				{					
					if( n>360.01 || n < -360.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");						
						return FALSE;
					}						
				}
				else if(g_Ini.iAxisRunMode[3]==1)						//A多圈模式
				{
					INT16S 	temp;
					FP32	tempf;
					FP64 	tt=n;

					temp=(INT16S)tt;						
					tempf=n-temp;

					if((tempf-0.359>0.001)||(tempf+0.359)<-0.001)	
					{
						OSQPost(MsgQueue,g_bEnglish?"Angle Err":"角度输入错误,小数部分应小于0.360");		
						return FALSE;
					}

					if(n<-49.359 || n > 49.359)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-49.359<y<49.359":"-49.359<角度<49.359!");		
						return FALSE;
					}													
				}
				//else if(g_Ini.iAxisRunMode[3]==2)						//A丝杆模式
				{
					if( n>=g_Ini.fMaxLimit[3] && (g_Ini.fMaxLimit[3]-g_Ini.fMinLimit[3])>0.01 )
					{
						OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"正限位越界");					
						return FALSE;
					}
					else if(n <= g_Ini.fMinLimit[3] && (g_Ini.fMaxLimit[3]-g_Ini.fMinLimit[3])>0.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"负限位越界");					
						return FALSE;
					}						

					if((strcmp(string,"+")==0||strcmp(string,"-")==0)&&(g_Ini.iAxisRunMode[3]==2))
					{
						OSQPost(MsgQueue,g_bEnglish?"Err Code ":"错误符号");		
						return FALSE;
					}								
				}	
			}
		}

		if(col==CList_MaxCol()-2)
		{
			if(n>6+0.01)
			{
				OSQPost(MsgQueue,g_bEnglish?"Probe <=6":"探针设定<=6");
				return FALSE;
			}				
		}

		if(col==CList_MaxCol()-1)
		{
			INT16S i=0;

			for(i=0;i<strlen(string);i++)
			{
				if(g_Ini.iCyldMode==0)
				{
					if((string[i]<'0'||string[i]>'1')&&string[i]!=NULL)
					{					
						OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"汽缸设定无效,确保为01序列");	
						return FALSE;
					}
				}
				else
					break;	
			}
		}

	}

	OSQPost(MsgQueue," ");
	return TRUE;

	/*	switch( ptr->value[0][0] )
	{
		case 'S':
			 {
				if( (col == 3 || col == CList_MaxCol()-2 || col == CList_MaxCol()-1 ) && string[0] )
				{
					OSQPost(MsgQueue,g_bEnglish?"Settings of speed is invalid":"速度数据设定无效");
					return FALSE;
				}

				if( col == 2 )
				{
					if( string[0] )
					{
						FP32 n = (FP32)stringtof((INT8U *)string);

						if( n < 0.01 || n >g_Ini.fXMaxSpeed )
						{
							OSQPost(MsgQueue,g_bEnglish?"Speed of X is too large":"X速度设定超出范围");
							return FALSE;
						}
					}
				}
			}
			break;

		case 'M':
			{
				FP32 n = (FP32)stringtof( (INT8U *)string );

				if( col == 2 || col == 3 )
				{
					if( n>360.01 || n < -360.01) 
					{
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");									
						return FALSE;
					}
				}

				if( col == 4 && string[0] )
				{   
					if(n<-9999.99 || n > 79999.99)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-9999.99<y<79999.99":"-9999.99<送线长度<79999.99!");		
						return FALSE;
					}
				}

				if(col==CList_MaxCol()-3 && string[0]!=NULL)
				{
					if( n>5.001 || n < 0.0099)
					{
						OSQPost(MsgQueue,g_bEnglish?"5.0>=Ratio>=0.01":"5.0>=速度比例>=0.01");
						return FALSE;
					}
				}

				if(g_Ini.iAxisCount==3)
				{
					if(col==CList_MaxCol()-4)
					{
						if(g_Ini.iAxisRunMode[2])
						{
							if( n>360.01 || n < -360.01)
							{
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");					
								return FALSE;
							}
						}
						else
						{
							if( n>79999.99 || n < -79999.99)
							{
								OSQPost(MsgQueue,g_bEnglish?"79999.99>=Z>=-79999.99":"79999.99>=Z轴长度>=-79999.99");					
								return FALSE;
							}
						}
					}
				}

				if(g_Ini.iAxisCount==4)
				{
					if(col==CList_MaxCol()-4 || col==CList_MaxCol()-5)
					{
						if(g_Ini.iAxisRunMode[3])
						{
							if( n>360.01 || n < -360.01)
							{
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=角度>=-360");						
								return FALSE;
							}
						}
						else
						{
							if( n>79999.99 || n < -79999.99)
							{
								OSQPost(MsgQueue,g_bEnglish?"79999.99>=A>=-79999.99":"79999.99>=A轴长度>=-79999.99");					
								return FALSE;
							}
						}
					}
				}

				if(col==CList_MaxCol()-2)
				{
					if(g_Ini.iAxisCount>2)
					{
						if(g_Ini.iPaoXian&&n>(MAXCYLINDER-1+0.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"probe <=3 at checking of wire out":"有跑线检测时,探针设定<=3");
							return FALSE;
						}
						else
						{
							if(!g_Ini.iPaoXian&&n>(MAXCYLINDER+0.01))
							{
								OSQPost(MsgQueue,g_bEnglish?"Probe <=4":"探针设定<=4");
								return FALSE;
							}
						}					
					}
					else
					{
						if(g_Ini.iPaoXian&&n>(MAXCYLINDER-1+0.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"probe <=3 when checking of wire out":"有跑线检测时,探针设定<=3");
							return FALSE;
						}
						else
						{

							if(g_Ini.iSafeDoor&&n>(MAXCYLINDER-2+0.01))
							{

								OSQPost(MsgQueue,g_bEnglish?"Probe<=2 when checking of safedoor":"有安全门检测时,探针设定<=2");
								return FALSE;
							}
							else
							{	
								if(!g_Ini.iPaoXian&&n>(MAXCYLINDER+0.01))
								{
									OSQPost(MsgQueue,g_bEnglish?"Probe <=4":"探针设定<=4");
									return FALSE;
								}
							}
						}					
					}
				}

				if(col==CList_MaxCol()-1)
				{
					INT16S i=0;

					for(i=0;i<strlen(string);i++)
					{
						if((string[i]<'0'||string[i]>'1')&&string[i]!=NULL)
						{					
							OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"汽缸设定无效,确保为01序列");	
							return FALSE;
						}
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
				else if( string[0] )
				{
					OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"数据无效");
					return FALSE;
				}
			}
			break;


		case 'N':case 'E':
			if( string[0] )
			{
				OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"数据无效");
				return FALSE;
			}
			break;
	}

	OSQPost(MsgQueue," ");
	return TRUE;
	 */
}


void	CTeach_ParaSelect()
{
	INT16U key,len;
	INT8S  a=0;
	BOOL   IsCreate=FALSE;
	char   buf[0x20]="\0";

	len = CEdit_GetLen();															//记录当前编辑框的可输入字符长度

	for(a=5; a<8; a++)
		SuspendTasks(a);

	SuspendTasks(0);

	OSTimeDly(80);

	CDC_SaveScreen();
	CDC_PutColorBar(3,90,400,252,LIGHTBLUECOLOR,1);
	CDC_Rectangle(3,90,400,340,g_TextColor1);
	CDC_Rectangle(4,90,399,341,g_TextColor1);

	if(g_bEnglish)
	{
		CDC_OutText24(40,100,"0---File manage",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,140,"1---System parameter",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,180,"2---Work parameter",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,220,"3---Input/Output check",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,260,"4---Key check",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,300,"5---Date&Time settings",WHITECOLOR,LIGHTBLUECOLOR);

	}
	else
	{
		CDC_OutText24(40,100,"0---文件管理系统",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,140,"1---更改系统参数",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,180,"2---更改工作参数",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,220,"3---进入输入输出检测界面",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,260,"4---进入按键检测界面",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,300,"5---日期时间设定",WHITECOLOR,LIGHTBLUECOLOR);
	}

	while(TRUE)
	{
		OSTimeDly(10);

		key = KeyMBoxGetKey();

		if(key != 0xffff)
		{
			switch(key)
			{
#ifdef	PRINTBMP
			case F5:
			{
				char   filename[0x20];
				INT16S i=0;

				//						CBmp_Init();
				strcpy(filename,"paraset");

				for(i=0; i<strlen(filename); i++)
				{
					if(filename[i]=='.')
					{
						filename[i]='\0';
						break;
					}
				}

				strcat(filename,".bmp");
				CreatBMP(filename,800,480,1);
				//						CBmp_Destroy();
				OSQPost(MsgQueue,"Write Bmp End!");
			}
			break;
#endif

			case '0':
			{
				SuspendTasks(0);
				SuspendTasks(5);
				SuspendTasks(6);
				SuspendTasks(7);

				WriteDataToFile(g_Ini.iFileName);

				CDispDir_Init(0,0,800,480,-1);
				CDispDir_DoModal();
				CDispDir_Destroy();

				ReadDataFromFile(g_Ini.iFileName);
				WriteDataToFile(g_Ini.iFileName);
				OSSemPost(UnpackDataSem);
				OSTimeDly(200);

				//CTeach_Init();
				//CTeach_Create();

				IsCreate = TRUE;

				ResumeTasks(0);
				ResumeTasks(5);
				ResumeTasks(6);
				ResumeTasks(7);
			}
			break;

			case '1':
			{
				CPassword_Init(240,140,340,180,g_bEnglish?"Password ":"密码验证",buf,g_TextColor2);

				if(CPassword_DoModal())
				{
					INT32S pass;
					INT16S iLanguage;
					INT16S iAxis;
					INT16S iAxisID;
					INT16S iSystemName;
					INT16S iAxisRunMode[4];

					iLanguage = g_Ini.iLanguage;
					iAxis = g_Ini.iAxisCount;
					pass=CPassword_GetPassword();
					iSystemName = g_Ini.iSystemName;

					for(iAxisID=0;iAxisID<4;iAxisID++)
					{
						iAxisRunMode[iAxisID]=g_Ini.iAxisRunMode[iAxisID];
					}

					CPassword_Destroy();

					if(pass == g_Ini.lPassword || pass == 790909L)
					{
						CSysPara_Init(0,0,798,478);

						if(CSysPara_DoModal())
						{
							if(iLanguage != g_Ini.iLanguage || iAxis != g_Ini.iAxisCount || g_bSysReadBackup||iSystemName != g_Ini.iSystemName)
							{
								g_bEnglish=g_Ini.iLanguage==1?TRUE:FALSE;
								IsCreate = TRUE;
							}

							for(iAxisID=0;iAxisID<4;iAxisID++)
							{
								if(g_Ini.iAxisRunMode[iAxisID]!=iAxisRunMode[iAxisID])//轴类型发生变化，触发CTeach_Create
								{
									IsCreate = TRUE;
									break;
								}
							}

							g_bFirstResetX=FALSE;
							g_bFirstResetY=FALSE;
							g_bFirstResetZ=FALSE;
							g_bFirstResetU=FALSE;

							if(g_bMode)
							{
								g_bMode=FALSE;
								key_set_led(HANDLELED,LEDCLOSE);
							}

							{
								INT32S t1,t2;

								OSSemPost(UnpackDataSem);

								t1 = OSTimeGet();

								g_bUnpackCorrect=FALSE;

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
				FP32	tmpx=g_Ini.fStartOffset[0];
				FP32	tmpy=g_Ini.fStartOffset[1];
				FP32	tmpz=g_Ini.fStartOffset[2];
				FP32    tmpu=g_Ini.fStartOffset[3];

				OSTimeDly(20);
				CParaSet_Init(0,0,798,478); 							//需更改

				if(CParaSet_DoModal())
				{
					if(fabs(tmpu-g_Ini.fStartOffset[3])>0.001)
						g_bFirstResetU=FALSE;  							//此处不用break;

					if(fabs(tmpz-g_Ini.fStartOffset[2])>0.001)
						g_bFirstResetZ=FALSE;

					if(fabs(tmpy-g_Ini.fStartOffset[1])>0.001)
						g_bFirstResetY=FALSE;
					if(fabs(tmpx-g_Ini.fStartOffset[0])>0.001)
						g_bFirstResetX=FALSE;
				}

				CParaSet_Destroy();
			}
			break;

			case '3':
				ResumeTasks(6);
				OSTimeDly(10);
				CTestFrm_Init();
				CTestFrm_DoModal();
				CTestFrm_Destroy();
				OSTimeDly(50);
				IsCreate = TRUE;
				break;

			case '4':
				OSTimeDly(10);
				CTestKey_Init();
				CTestKey_DoModal();
				CTestKey_Destroy();
				OSQPost(MsgQueue," ");
				break;

			case '5':
			{
				CSetTime_Init(180,80,480,300);
				CSetTime_DoModal();
				CSetTime_Destroy();
			}
			break;

			case '6':														//用户级系统管理密码设定
			{
				sprintf(buf,g_bEnglish?"\n Mach Serial:%d\n":"\n  用户级机器编号:%d\n",NewDog.User_ComputerNo);
				CPassword_Init(200,140,360,220,g_bEnglish?"User's System Pwd":"用户级系统管理密码验证",buf,g_TextColor2);

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
						CUserDog_Init(200,100,420,270);
						CUserDog_DoModal();
						CUserDog_Destroy();
					}
				}
				else
				{
					CPassword_Destroy();
				}
			}
			break;
			/*
				case '7':  														//公司级系统管理密码设定
					{
						sprintf(buf,g_bEnglish?"\nCompany Mach Code:%d\n":"\n  公司级电脑编号:%d",NewDog.Adtech_ComputerNo);

						CPassword_Init(200,140,400,220,g_bEnglish?"Company System Pwd":"公司级系统管理密码验证",buf,g_TextColor2);

						if(CPassword_DoModal())
						{
							INT32S pass;

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

							if(password == NewDog.Adtech_ComputerNo ||pass==116001)
							{

								CSetDog_Init(200,100,380,230);
								CSetDog_DoModal();
								CSetDog_Destroy();
							}
						}
						else
						{
							CPassword_Destroy();
						} 	
					}
					break;
			 */
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
			}

			break; 

		}																		//end if(key!=0xffff)
	}																			//end while(TRUE);

	for(a=5;a<8;a++)
		ResumeTasks(a);

	ResumeTasks(0);

	//	if(!g_bScreen)
	if(!g_bInterface)
	{
		g_iCurEditNo = 9;
		CTeach_Update(0,0);
	}

	CEdit_SetTextColor(WHITECOLOR);
	CEdit_SetBackColor(BLUECOLOR);	
	CEdit_SetLen(len);
}


void	CTeach_UpdateSpeed(int speed )
{
	g_iSpeedBeilv += speed;

	if( g_iSpeedBeilv > 3 ) 
		g_iSpeedBeilv = 1;

	switch(g_iSpeedBeilv)
	{
	case 1:
		OSQPost(MsgQueue,g_bEnglish?"Low":"低速");
		break;

	case 2:
		OSQPost(MsgQueue,g_bEnglish?"Mid":"中速");
		break;

	case 3:
		OSQPost(MsgQueue,g_bEnglish?"High":"高速");
		break;

	default:
		break;
	}	
	/*
	switch(g_iSpeedBeilv)
	{
		case 1:
			CDC_OutText24Ex(735,445,g_bEnglish?"Low":"低速",g_TextColor2,g_nTechfrmBk);
			break;

		case 2:
			CDC_OutText24Ex(735,445,g_bEnglish?"Mid":"中速",WHITECOLOR,g_nTechfrmBk);
			break;

		case 3:
			CDC_OutText24Ex(735,445,g_bEnglish?"High":"高速",REDCOLOR,g_nTechfrmBk);
			break;

		default:
			break;
	}
	 */
}


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
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_String( ptr->value[CList_CurCol()+1] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;

	case 3:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_String( ptr->value[CList_CurCol()+0] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
		break;

	case 4:
		CEdit_String( ptr->value[CList_CurCol()-1] );
		break;

	default:
		if(CList_CurCol()>=CList_MaxCol()-3)
			CEdit_String( ptr->value[CList_CurCol()+0] );
		else
			CEdit_String( ptr->value[CList_CurCol()-1] );
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


void CTeach_ModeDisp(void)
{
	if(g_bInterface)
	{
		if(!g_bMode)
			CDC_OutText_32(672,100,g_bEnglish?"Auto ":"自动",WHITECOLOR, g_nBackColor);
		//CDC_TextSelect_32(600,94,134,39,DISPSTYLE,g_bEnglish?"Auto ":"自动",WHITECOLOR, g_nBackColor);
		else
			CDC_OutText_32(672,100,g_bEnglish?"Hand ":"手动",WHITECOLOR, g_nBackColor);
		//CDC_TextSelect_32(600,94,134,39,DISPSTYLE,g_bEnglish?"Hand ":"手动",WHITECOLOR, g_nBackColor);
	}
	else
	{
		if(!g_bMode)
			CDC_OutText_32(150,409,g_bEnglish?"Auto ":"自动",WHITECOLOR, g_nBackColor);
		else
			CDC_OutText_32(150,409,g_bEnglish?"Hand ":"手动",WHITECOLOR, g_nBackColor);
	}
}
