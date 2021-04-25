/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        techfrm.c
  �����ˣ�       ��־��
  �������ڣ�	 2004/11/20
  ��Ҫ��������:  ���ݱ༭����

  �޸ļ�¼��
   1.  �޸���:	      ��־��
	   �޸�����:	  2005/08/04
	   ��Ҫ�޸�����:  ����༭�ĸ���BUG
   2. ...
 *************************************************/
# include   "includes.h"
//# include	"usb.h"
# include 	"strtool.h"
//# include	"part.h"
# include	"usb_host.h"
# include   "base.h"

extern 		int 		usb_max_devs;
//int			VER=110;															//�汾��ָʾ

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
WORK_EXT 	INT32S		g_lProbeFail; 											//��¼̽��ʧ��   
WORK_EXT 	FP32		g_fStepSpeed[4];

INT32S		g_lAxisValue[4] = {0,0,0,0};										//��¼����ض�ʱ��λ��
INT32S      g_lHandPulse  = 0;
extern 		INT32S      g_iLayNum;
WORK_EXT	FP32   		g_fRunSpeed;
extern		BOOL		g_bSysReadBackup;								

#define		LISTHEIGHT	40
#define		LISTLEFT	4
#define		LISTTOP		4

#define		EDITHEIGHT  40														//��������ʾ��߶�
#define		EDITWIDTH	345														//��������ʾ����
#define		EDITLEFT	43														//��������ʾ��X���
#define		EDITTOP		93														//��������ʾ��Y���

INT16S		Widths2[COLS2]={ 70,140,140,140,100,90,110};
INT16S		Widths3[COLS3]={ 70,110,110,110,110,100,85,95};	
INT16S		Widths4[COLS4]={ 70,110,110,110,95,95,60,65,75};

INT16S		g_iCurEditNo=9;
BOOL		g_bEditData=FALSE; 
//CRect		g_EditRect[3]={{85,9,85+58,9+28},{315,9,315+115,9+28},{575,9,575+58,9+28}};		//����ţ�Ŀ��������̽���趨
CRect		g_EditRect[3]={{236,94,370,133},{600,144,734,183},{600,244,734,283}};		
BOOL		g_bDispServoWaring=TRUE;
CTROL		*m_ptrCtrl;	
CTROL		CtrlTmp;
extern      INT8U   	g_ukeyscan;   
INT16S		g_iCommandPos = 0;
INT8S		g_cCommand[7]={'M','S','E','L','N','J','G'};
BOOL		g_bInterface = TRUE;												//����ʶ��(TRUE��������,FALSE:�̵�����)
BOOL		g_bInsertNum = FALSE;												//�Ƿ�����������

INT32S		g_iStopScrTime = 0;													//ͣ��������ʱ
extern		INT32S 		g_bInitbmp;
BOOL		g_bSingleRun = FALSE;		

void	CTeach_Init()
{
	m_ptrCtrl = &CtrlTmp;
	g_iRunLine = 0;
	Ok_Exit = FALSE;	
	CList_Init();
	CList_MoveTo( LISTLEFT,LISTTOP );											//�б���λ��

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

	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),g_nBackColor,100);					//�����
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
		//CDC_OutText_48(223,23,g_bEnglish?"YF8208 Spring Control":"YF8208���ɻ�������",PINKCOLOR, g_nBackColor);

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

			//��ʾӢ��YF--λ��Ϊ32
			for(i=0;i<2;i++)
				DispWords(i,startPosX+i*YMzk,startPosY);
			//��ʾ����8208--λ��Ϊ24
			for(i=0;i<4;i++)
				DispWordsS(NUM[i]+2,startPosX+2*YMzk+i*SZzk-2,startPosY);
			//��ʾ����--λ��Ϊ48
			for(i=0;i<6;i++)
				DispWordsH(i,startPosX+2*YMzk+4*SZzk+i*HZzk,startPosY);
		}

		CDC_PutColorBar(5,393,659,36,REDCOLOR,100);

		CMsg_Init();
		CMsg_SetWH(115,397,550,24,WHITECOLOR,REDCOLOR,WHITECOLOR);

		CDC_Line(0,63,getmaxx(),63,WHITECOLOR);
		CDC_Line(0,64,getmaxx(),64,WHITECOLOR);

		for(i=0; i<6; i++)														//�����ʾ��
		{
			if(i==1 || i==2 || i==3)
				continue;

			CDC_DrawArcEdit(EDITLEFT,EDITTOP+i*(EDITHEIGHT+10),EDITWIDTH,EDITHEIGHT,YELLOWCOLOR);
		}

		for(i=0; i<6; i++)														//�ұ�
		{
			CDC_DrawArcEdit(EDITLEFT+EDITWIDTH+24,EDITTOP+i*(EDITHEIGHT+10),EDITWIDTH,EDITHEIGHT,YELLOWCOLOR);
		}

		CDC_OutText_32(47,101+5*(EDITHEIGHT+10),g_bEnglish?"Left Time":"ʣ��ʱ��",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47,101+4*(EDITHEIGHT+10),g_bEnglish?"Current Line":"��ǰ�к�",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47,101,g_bEnglish?"Filename":"������",YELLOWCOLOR, g_nBackColor);

		CDC_OutText_32(47+EDITWIDTH+24,101,g_bEnglish?"Mode":"��ǰģʽ",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+EDITHEIGHT+10,g_bEnglish?"Counts":"Ŀ������",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+2*(EDITHEIGHT+10),g_bEnglish?"Total":"��ǰ����",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+3*(EDITHEIGHT+10),g_bEnglish?"Set Probe":"̽���趨",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+4*(EDITHEIGHT+10),g_bEnglish?"Fail Probe":"̽��ʧ��",YELLOWCOLOR, g_nBackColor);
		CDC_OutText_32(47+EDITWIDTH+24,101+5*(EDITHEIGHT+10),g_bEnglish?"Speed":"�����ٶ�",YELLOWCOLOR, g_nBackColor);


		CDC_Line(0,390,getmaxx(),390,WHITECOLOR);								//��Ϣ��28
		CDC_Line(0,428,getmaxx(),428,WHITECOLOR);
		CDC_OutText24Ex(5,397,g_bEnglish?"Message:":"��Ϣ��ʾ:",WHITECOLOR, REDCOLOR);


		CDC_Line(160,428,160,getmaxy(),WHITECOLOR);								//����̽���
		CDC_Line(161,428,161,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(0, 428, 160, 49,g_bEnglish?"Air 1":"����1",GREENCOLOR,g_nBackColor);

		CDC_Line(320,428,320,getmaxy(),WHITECOLOR);
		CDC_Line(321,428,321,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(160, 428, 160, 49,g_bEnglish?"Air 2":"����2",GREENCOLOR,g_nBackColor);

		CDC_Line(480,428,480,getmaxy(),WHITECOLOR);
		CDC_Line(481,428,481,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(320, 428, 160, 49,g_bEnglish?"Drive":"���߼�",GREENCOLOR,g_nBackColor);

		CDC_Line(640,428,640,getmaxy(),WHITECOLOR);
		CDC_Line(641,428,641,getmaxy(),WHITECOLOR);
		CDC_TextCen_32(480, 428, 160, 49,g_bEnglish?"Wire fly":"����",GREENCOLOR,g_nBackColor);
		CDC_TextCen_32(640, 428, 160, 49,g_bEnglish?"Probe 1":"̽��1",GREENCOLOR,g_nBackColor);

		if(g_Ini.iAxisCount<3)
		{
			rect.left = 73;															//������ʾ������
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
			rect.left = 73;															//������ʾ������
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

		UpdateFileDestProbe(0,0,0);												//�����ţ������趨��̽���趨	
		UpdateSpeedCountProbe();												//�����ٶȣ�������̽��ʧ�ܴ���,��ʾ����	
		UpdateProbe(TRUE);														//ˢ����ʾ̽���ź�״̬
		UpdateCurAxis(g_iWorkNumber);
		UpdateCylinder(TRUE);													//ˢ����ʾ����״̬
		UpdateLeavingTime(TRUE,TRUE,TRUE);

	}
	else	
	{
		CRect rect;

		CDC_PutColorBar(5,445,656,30,REDCOLOR,100);

		CMsg_Init();
		CMsg_SetWH(115,448,550,24,WHITECOLOR,REDCOLOR,WHITECOLOR);

		rc.left = 2;															//����Ϣ��ʾ��
		rc.top =  444;
		rc.right = getmaxx()-3;
		rc.bottom = getmaxy()-3;
		CDC_Rectangle2(rc,WHITECOLOR);

		rc.left   += 1;
		rc.top    -= 1;
		rc.right  -= 1;
		rc.bottom += 1; 
		CDC_Rectangle2(rc,WHITECOLOR);
		CDC_OutText24Ex(5,447,g_bEnglish?"Message:":"��Ϣ��ʾ:",WHITECOLOR,REDCOLOR);	

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

		CTeach_InvidateView(TRUE);												//�б���ʾ

		rect.left = 10;															//������ʾ������
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

		CDC_OutText_32(10,409,g_bEnglish?"Mode:":"��ǰģʽ:",YELLOWCOLOR,g_nBackColor);
		CDC_OutText_32(550,409,g_bEnglish?"S":"S",YELLOWCOLOR,g_nBackColor);

		UpdateAxisStatus();	
	}

	CTeach_ModeDisp();
	//----------------------------��ʾ�����Ƶ�����-------------------------------//
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
		INT8S *title2[COLS2] ={"N","��ʼ","����","���߿�","�ٱ�","̽��","����"};
		INT8S *title21[COLS2] ={"N","Start","End","Y(ON)","SpdRate","Probe","Out"};

		INT8S *title3[COLS3] ={"N","��ʼ","����","���߿�","Z�Ὺ","�ٱ�","̽��","����"};	
		INT8S *title31[COLS3] ={"N","Start","End","Y(ON)","Z(ON)","SpdRate","Probe","Out"};	

		INT8S *title4[COLS4] ={"N","��ʼ","����","����","Z��","A��","�ٱ�","̽��","����"};	
		INT8S *title41[COLS4] ={"N","Start","End","Y(ON)","Z(ON)","A(ON)","SpdRate","Probe","Out"};

		switch(g_Ini.iAxisCount)
		{	
		case 2:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title2[3]="���߹�";
				title21[3]="Y(OFF)";
			}
			CList_DrawTitle( g_bEnglish?title21:title2 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;

		case 3:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title3[3]="���߹�";
				title31[3]="Y(OFF)";
			}

			if(g_Ini.iAxisSwitch[2]==0)
			{
				title3[4]="Z���";
				title31[4]="Z(OFF)";
			}

			CList_DrawTitle( g_bEnglish?title31:title3 ,BLACKCOLOR,g_nTitleBk,1);
		}
		break;

		case 4:
		{
			if(g_Ini.iAxisSwitch[1]==0)
			{
				title4[3]="���߹�";
				title41[3]="Y(OFF)";
			}

			if(g_Ini.iAxisSwitch[2]==0)
			{
				title4[4]="Z���";
				title41[4]="Z(OFF)";
			}
			if(g_Ini.iAxisSwitch[3]==0)
			{
				title4[5]="A���";
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

	size = size - CList_StartRow();												//sizeΪ��ǰҳ�������

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

	sprintf(buffer,"%d",g_Ini.lRunCount);										//����
	CDC_TextSelect_32(g_EditRect[1].left, g_EditRect[1].top+EDITHEIGHT+10, g_EditRect[1].right-g_EditRect[1].left,
			g_EditRect[1].bottom-g_EditRect[1].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

	sprintf(buffer,"%d",g_lProbeFail);											//̽��ʧ��
	CDC_TextSelect_32(g_EditRect[2].left, g_EditRect[2].top+EDITHEIGHT+10, g_EditRect[2].right-g_EditRect[2].left,
			g_EditRect[2].bottom-g_EditRect[2].top,DISPSTYLE,buffer,g_nRunTextColor, g_nTechfrmBk);

	sprintf(buffer,"%d",(INT32S)g_fRunSpeed);								//�����ٶ�
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
						OSQPost(MsgQueue,g_bEnglish?"Process can not load the file!":"���ڼӹ�����װ���ļ�!");

					if(temp != g_Ini.iFileName)
					{
						/*
								if(!g_bModifySave)
								{
									INT8S a=0;

									for(a=5; a<8; a++)
										SuspendTasks(a);

									OSTimeDly(50);
									CMsgDlg_Init(200,140,200,120,g_bEnglish?" save file?":"�ļ��Ѹ���Ҫ������?",g_bEnglish?"Warning":"����",REDCOLOR,
												g_bEnglish?"Yes":"��",g_bEnglish?"No":"��");

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

						WriteDataToFile(temp);//�����л��ļ�ʱ��δ����ӹ�����δ���浽�ӹ��ļ��е�����20140815

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

						sprintf(g_pszMsgBuf,g_bEnglish?"Load%0dfile!":"����%0d���ļ�!",g_Ini.iFileName);
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
		rect.left=x+2;														//+2��-2��Ϊ���޸������ڴ�λ���⡣
		rect.top=CList_Top();
		rect.right=x+w;
		rect.bottom=CList_Top()+CList_Height();

		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"Y(ON)":"Y(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"���߿�":"���߹�",BLACKCOLOR,g_nTitleBk);

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
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"���߿�":"���߹�",BLACKCOLOR,g_nTitleBk);


		x+=w;
		w=Widths3[4];
		rect.left=x+2;
		rect.right=x+w;

		CDC_PutColorBar2( rect,g_nTitleBk,1);

		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z(ON)":"Z(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z�Ὺ":"Z���",BLACKCOLOR,g_nTitleBk);


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
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[1]==1?"���߿�":"���߹�",BLACKCOLOR,g_nTitleBk);

		x+=w;
		w=Widths4[4];
		rect.left=x+2;
		rect.right=x+w;
		CDC_PutColorBar2( rect,g_nTitleBk,1);
		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z(ON)":"Z(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[2]==1?"Z�Ὺ":"Z���",BLACKCOLOR,g_nTitleBk);

		x+=w;
		w=Widths4[5];
		rect.left=x+2;
		rect.right=x+w;
		CDC_PutColorBar2( rect,g_nTitleBk,1);
		if(g_bEnglish)
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[3]==1?"A(ON)":"A(OFF)",BLACKCOLOR,g_nTitleBk);
		else
			CDC_TextSelect_32(rect.left,rect.top,w,CList_Height(),0,g_Ini.iAxisSwitch[3]==1?"A�Ὺ":"A���",BLACKCOLOR,g_nTitleBk);
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
	//CList_CurCol()ʵ�ʾ�����g_iWorkNumber
	if( t < 3 )
		g_iWorkNumber = 1;
	else
	{
		if( t< CList_MaxCol()-3 )
			g_iWorkNumber= t-1;
		else
			g_iWorkNumber=0;													//û��ѡ���κ�һ����
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
		CEdit_SetLen(4);    													//�������趨�ַ�����
	}
	else
	{
		if(CList_CurCol() == CList_MaxCol()-2)
		{
			CEdit_SetLen(1);    												//̽�����趨�ַ�����
		}
		else
		{
			if(CList_CurCol()==CList_MaxCol()-3)
			{
				CEdit_SetLen(4);												//�ٱ����趨�ַ�����
			}
			else
			{
				if(CList_CurCol()<3)
					CEdit_SetLen(6); 											//X���趨�ַ�����Ϊ6
				else
				{
					if(CList_CurCol() == 3)
						CEdit_SetLen(7); 										//Y���趨�ַ�����Ϊ7

					if(CList_CurCol() == 4)
						CEdit_SetLen(6); 										//Z���趨�ַ�����Ϊ6

					if(CList_CurCol() == 5)
						CEdit_SetLen(6); 										//A���趨�ַ�����Ϊ6
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
					CEdit_SetString(g_bEnglish?"Open":"���߿�");
				else
					CEdit_SetString(g_bEnglish?"Close":"���߹�");
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
					CEdit_SetString(g_bEnglish?"Open":"���߿�");
				else
					CEdit_SetString(g_bEnglish?"Close":"���߹�");
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
					CEdit_SetString(g_bEnglish?"Open":"���߿�");
				else
					CEdit_SetString(g_bEnglish?"Close":"���߹�");
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
void	CTeach_DisplayLine( INT16S data,INT16S row,DRAWSTATUS status,INT32S qColor,INT32S bColor)//qColor��ɫ��bColor��ɫ
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

			CTeach_ViewConvert(row, col, m_Buffer);				//��ptr->value[]�е�ֵ����m_buffer�������������ִ�к���ʾΪ���ֺڵ�

			if(ptr->value[0][0]=='N' && col==3)
			{
				if(m_Buffer[0]=='1')
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"���߿�",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"���߹�",qColor,bColor);
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
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"���߿�",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"���߹�",qColor,bColor);
				}
			}
			else
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,m_Buffer,qColor,bColor);
		}
		break;

		case MoveActive:											//�ֶ�ģʽ��������ʱ����ʾΪ���ְ׵�
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
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"���߿�",bColor,qColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"���߹�",bColor,qColor);
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
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Open":"���߿�",qColor,bColor);
				}
				else
				{
					CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,g_bEnglish?"Close":"���߹�",qColor,bColor);
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

	for(i=0; i<Line_Data_Size; i++)												//��Nָ�������Ӧ���к��跢���仯�Ա�ָ֤���λ����ȷ
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
	for(i=0; i<Line_Data_Size; i++)												//��J��Gָ�������Ӧ���к��跢���仯�Ա�ָ֤���λ����ȷ
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

	for(i=0; i<Line_Data_Size; i++)												//��Nָ�������Ӧ���к��跢���仯�Ա�ָ֤���λ����ȷ
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
	for(i=0; i<Line_Data_Size; i++)												//��J��Gָ�������Ӧ���к��跢���仯�Ա�ָ֤���λ����ȷ
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
		g_iCurEditNo = 3;//������һ�γ�ʼ������ʱ��ϵͳ�������Խ�����ʾ�쳣������20140724

		CSysPara_Init(0,0,798,478);
		CSysPara_DoModal();
		CSysPara_Destroy();
	}

	g_bEnglish=g_Ini.iLanguage==1?TRUE:FALSE;
	CTeach_Init();
	CTeach_Create();
	TaskStartCreateTasks();														//������������

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

	{																			//��ͣ��������
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
					CTeach_ModeDisp();//������ҡ��ͣ�󣬽�����ʾ״̬�л���BUG���˳��ֶ��ӹ�ģʽ�� 20140812
					key_set_led(HANDLELED,LEDCLOSE);
				}
				else
					OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"����ͣ��״̬!����ǰ��̧��ͣ��ť!");

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

	{																			  //�ŷ���������
		INT16S XYZIO[4]={XSERVOWARNING,YSERVOWARNING,ZSERVOWARNING,USERVOWARNING};//XYZ�ŷ������ź�����˿�

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
							OSQPost(MsgQueue,g_bEnglish?"X Servo Warning!Please Power off Check!":"X�ŷ�����,��ϵ���!");
							break;

						case 1:
							OSQPost(MsgQueue,g_bEnglish?"Y Servo Warning!Please Power off Check!":"Y�ŷ�����,��ϵ���!");
							break;

						case 2:
							OSQPost(MsgQueue,g_bEnglish?"Z Servo Warning!Please Power off Check!":"Z�ŷ�����,��ϵ���!");
							break;

						case 3:
							OSQPost(MsgQueue,g_bEnglish?"U Servo Warning!Please Power off Check!":"U�ŷ�����,��ϵ���!");
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


	{//���ߣ����߱���
		INT16S 	bit;
		static 	INT16S OtherIOStatus[3]={0,1,1};
		//INT16S 	OtherIO[2]={DUANXIAN,SONGXIANIOCONST};
		static	BOOL bAlarm = TRUE;
		static	BOOL bDoorAlarm = TRUE;
		OtherIOStatus[0]=!g_Ini.iIOBreakLineConfig_Level;
		OtherIOStatus[1]=!g_Ini.iIOKinkConfig_Level;
		//if(g_Ini.iAxisCount<3)
		//OtherIO[1] = SONGXIANIOCONST;	//��Ϊ���ῨʱYԭ��Ϊ���߱���


		if(g_bRelease)
		{
			if(RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level,2) && bAlarm)
			{

				bAlarm = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Broken Wire Waring":"���߱���!");

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
				OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"���߱���!");

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
			bit = ReadBit(PAOXIANDI);		//���߱���

			if(bit!=OtherIOStatus[2])
			{
				OtherIOStatus[2] = bit;

				if( bit==0 )
				{
					g_iErrorNo=8;
					OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"���߱���!");

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

	if(g_bMode)																	//�綯����
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
	{																			//IO������
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

	if(g_Ini.iSwhSingleIo==1 )		 //IO�ڵ���
	{
		if(!g_bPressStepIO2 && RealIO(SINGLEDI,0,2) /*&&RealIO(SINGLEDI2,0,2)*/)                         //IO�ڵ���
		{
			g_bPressStepIO2= TRUE;
			CTeach_OnKey(TSTEP,0);

		}

		if(g_bPressStepIO2 && RealIO(SINGLEDI,1,2) /*&& RealIO(SINGLEDI2,1,2)*/)
		{
			g_bPressStepIO2 = FALSE;
		}
	}
	else if ( g_Ini.iSwhSingleIo==2)						//��ϳ�ѹ���ź�����������
	{
		/*
			 if(RealIO(SINGLEDI1,0,2))                         //IO�ڵ���
			 {
				CTeach_OnKey(TRUN,0);							//�ر���ע����TRUN
			 }*/

		if(!g_bPressStepIO2 && RealIO(SINGLEDI,0,2) /*&&RealIO(SINGLEDI2,0,2)*/)                         //IO�ڵ���
		{
			g_bPressStepIO2= TRUE;
			CTeach_OnKey(TRUN,0);

		}

		if(g_bPressStepIO2 && RealIO(SINGLEDI,1,2) /*&& RealIO(SINGLEDI2,1,2)*/)
		{
			g_bPressStepIO2 = FALSE;
		}
	}




	//if(g_Ini.iHandStatus)														// ���ֹ���
	{
		pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);

		if(labs(pos)>1)
		{
			if(g_bRelease)
			{
				if(RealIO(DUANXIAN,g_Ini.iIOBreakLineConfig_Level, 2))
				{
					OSQPost(MsgQueue,g_bEnglish?"Broken Wire Waring":"���߱���!");

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
					OSQPost(MsgQueue,g_bEnglish?"Foul Waring":"���߱���!");

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

			if(!g_bMode)														//�Զ�ģʽ�������ƶ�������
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

						lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];//�������к����ڶ�Ȧ��ҡʱ�����ö�Ȧ��ҡ��
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
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
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
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
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
										OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
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
						//���Ӳ��λ������λ�������λ����������ʾ�����ҽ�������0������loopcheck������
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
							lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//�������к����ڶ�Ȧ��ҡʱ�����ö�Ȧ��ҡ��
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
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
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
						else if(g_Ini.iAxisRunMode[2]==1)			//��Ȧ
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
							lTempPos = GetPulse(g_iWorkNumber,TRUE) % g_Ini.lPulse[g_iWorkNumber-1];		//�������к����ڶ�Ȧ��ҡʱ�����ö�Ȧ��ҡ��
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
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"��ǰ�������ͺ󱨾�!!!");
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
			else																//�ֶ�ģʽ�����ּӹ�
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
						OSQPost(MsgQueue,g_bEnglish?"The data exist error,Can't run!":"������ݴ��󣬲���ִ�����ּӹ�����!");

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
		char buf[0x60] = "��Ŀ��:";
		char bufE[0x60]= "No.:";
		INT16U value,libver;

		value=Har_Ver1();														//��ȡFPGAӲ���汾��

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
			strcat(buf,"\n����汾:V");

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

				sprintf(buf2,"%s%0x","\nӲ���汾:",value);

				strcat(buf,(INT8S *)buf2);
			}

			strcat(buf,"\n����:");
			strcat(buf,__DATE__);
			strcat(buf,"\nʱ��:");
			strcat(buf,__TIME__);

			strcat(buf,"\n��汾:V");

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
		CAfxMsgDlg_Init(200,120,400,280,g_bEnglish?bufE:buf,g_bEnglish?"MESSAGE":"��Ϣ��ʾ",g_TextColor1);
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
	//���ò������ÿ���
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

		if(key== TRUN || key==TSTEP || key==THANDMOVE)//���������������ּӹ�
		{
			OSQPost(MsgQueue,g_bEnglish?"key is invalid after emergency stop":"����ֹͣ��ð�����Ч,���ȹ���!");
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
#ifdef	PRINTBMP														//��Ļ��ͼ ������

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

	case TTEACH:															//�̵�

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

				WriteDataToFile( g_Ini.iFileName );	//�Զ�����ӹ��ļ�

			}
		}
	}
	break;

	case TAIR:
		if(g_lStepNode != 0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't enter state of cylinder test at running":"ͣ��ģʽ�½������ײ���״̬");
			break;
		}

		g_bCylinder = g_bCylinder?FALSE:TRUE;

		if(g_bCylinder)
		{
			OSQPost(MsgQueue,g_bEnglish?"Enter state of cylinder test":"�������ײ���״̬!");
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

			OSQPost(MsgQueue,g_bEnglish?"Exit state of cylinder test":"�˳����ײ���״̬!");
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

			OSQPost(MsgQueue,g_bEnglish?"Open Air 1":"������1");
		}
		else
		{
			WriteBit(0,0);

			key_set_led(AIRALED,LEDCLOSE);

			OSQPost(MsgQueue,g_bEnglish?"Close Air 1":"�ر�����1");
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

			OSQPost(MsgQueue,g_bEnglish?"Open Air 2":"������2");
		}
		else
		{
			WriteBit(1,0);

			key_set_led(AIRBLED,LEDCLOSE);

			OSQPost(MsgQueue,g_bEnglish?"Close Air 2":"�ر�����2");
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
			OSQPost(MsgQueue,g_bEnglish?"Open Air 3":"������3");
		}
		else
		{
			WriteBit(2,0);
			key_set_led(AIRCLED,LEDCLOSE);
			OSQPost(MsgQueue,g_bEnglish?"Close Air 3":"�ر�����3");
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
			OSQPost(MsgQueue,g_bEnglish?"Open Air 4":"������4");
		}
		else
		{
			WriteBit(3,0);
			key_set_led(AIRDLED,LEDCLOSE);
			OSQPost(MsgQueue,g_bEnglish?"Close Air 4":"�ر�����4");
		}
		if(g_bInterface)
			UpdateCylinder(FALSE);
	}
	break;

	case THANDBOX: 															//���֣���������
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

	case THANDMOVE:															//�ֶ���ģʽ��
	{
		//if(CheckWarning(TRUE))
		//return;

		g_bMode = g_bMode?FALSE:TRUE;

		if(g_bMode)
		{
			key_set_led(HANDLELED,LEDOPEN);
			OSQPost(MsgQueue,g_bEnglish?"Goto HandMode":"�����ֶ�ģʽ!");
		}
		else
		{
			key_set_led(HANDLELED,LEDCLOSE);
			SetMM(7,0,FALSE);
			SetMM(8,0,FALSE);
			OSQPost(MsgQueue,g_bEnglish?"Goto AutoMode":"�����Զ�ģʽ!");
		}

		CTeach_ModeDisp();
	}
	break;

	case TLOOP:																//ѭ��ָ��
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
				DataPut(index,1,"��");

				Line_Data_Size ++;
				LoopStyle = TRUE;
			}

			DataPut(index,2,LoopStyle?"1":"0");								//ѭ��ģʽѡ��0:˳��ѭ��123��1:����ѭ��123--321--123��

			CTeach_InvidateView(FALSE);
			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
		}
		break;

	case TSET:																//����ѡ�����
		temp=g_iCurEditNo;
		g_iCurEditNo = 3;
		CTeach_ParaSelect();

		if(g_bInterface && g_iCurEditNo!=0)									//������������棬�������ý���Ȼ���˳��������棬����IsCreateû����ΪTRUE������������CTeach_Createʱ
		{
			g_iCurEditNo = (g_bInterface?temp:9);							//����Ϊtemp����
			UpdateFileDestProbe(0,0,0);
		}																	//���IsCreate����ΪTRUE����g_iCurEditNo����Ϊ0

		SetMM(7,0,FALSE);
		SetMM(8,0,FALSE);
		break;

	case TPROBESET:															//̽������

		if(g_bInterface)
		{
			OSQPost(MsgQueue,g_bEnglish?"Probe settings":"�趨̽��ʧ�ܴ���");

			UpdateFileDestProbe(3,-1,0);
		}
		break;

	case TPROBECLR:															//̽������

		if(g_bInterface)
		{
			g_lProbeFail=0;

			if(g_bInterface)
				UpdateSpeedCountProbe();

			OSQPost(MsgQueue,g_bEnglish?"Probe clear!":"̽������!");
		}
		break;

	case TCOUNTCLR:															//��������
		if(g_bInterface)
		{
			INT8S a=0;

			for(a=5; a<8; a++)
				SuspendTasks(a);

			OSTimeDly(50);
			CMsgDlg_Init(250,130,300,200,g_bEnglish?"Clear processing Count?":"����ӹ�����?",g_bEnglish?"Warning":"����",REDCOLOR,g_bEnglish?"Yes":"��",g_bEnglish?"No":"��");

			if(CMsgDlg_DoModal())
			{
				g_Ini.lRunCount = 0;
				WriteBit(O_WARNING,0);
				SysParaWrite();
				WriteRunCount();
				WriteDataToFile(g_Ini.iFileName);

				CMsgDlg_Destroy();
				CTeach_Create();
				OSQPost(MsgQueue,g_bEnglish?"Reset Count":"�ӹ���������!");

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

	case TDELALL:																//ȫɾ
	{
		INT8S a=0;

		if(g_bInterface)
			break;

		if(g_bMode || g_lStepNode!=0)
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't delete the file in processing ":"�ֶ��ӹ�ģʽ�»�ӹ�δ����ʱ����ȫɾ!");
			break;
		}

		for(a=5; a<8; a++)
			SuspendTasks(a);
		OSTimeDly(50);

		CMsgDlg_Init(250,130,300,200,g_bEnglish?"Delete all of data?":"ɾ������������?",g_bEnglish?"Warning":"����",REDCOLOR,g_bEnglish?"Yes":"��",g_bEnglish?"No":"��");

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
		if(g_iCurEditNo==0) 									//���Ϊ����
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
						CMsgDlg_Init(200,140,350,180,"�ļ��Ѿ�����,���Ǹ��ļ�?","����",REDCOLOR,"��","��");

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

						OSQPost(MsgQueue,g_bEnglish?"Copy file is canceled!":"ȡ���ļ�����!");

						CTeach_Create();

						SetMM(7,0,FALSE);
						SetMM(8,0,FALSE);
						return;
					}

				}

				if(tmpfile2>=0 && tmpfile2<=999)
				{
					WriteDataToFile(tmpfile2);
					OSQPost(MsgQueue,g_bEnglish?"Copy succeed!":"���Ƴɹ�!");
				}
			}
			else
			{
				OSQPost(MsgQueue,g_bEnglish?"The same file can't copy!":"ͬһ�ļ�,���ܸ���!");
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

		OSQPost(MsgQueue,g_bEnglish?"Save...":"����...");

		WriteDataToFile(g_Ini.iFileName);
		WriteRunCount();
		g_bModifySave = TRUE;
		g_bModify = TRUE;

		OSQPost(MsgQueue,g_bEnglish?"Saved":"�ļ�����!");
		break;

	case TX:																//X��ѡ��

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

	case TY:																//Y��ѡ��
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
		if(g_Ini.iAxisCount > 2)											//Z��ѡ��
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

	case TSCREEN:															//�����л�
	{
		g_bInterface = g_bInterface?FALSE:TRUE;
		OSTimeDly(20);
		//Uart_Printf();
		CTeach_Create();
		//Uart_Printf();
	}
	break;

	case TINSERT:															//�б����༭
		if(!g_bInterface)
		{
			CTeach_Insert();
			CTeach_InvidateView(FALSE);

			WriteDataToFile( g_Ini.iFileName );
			g_bModifySave = TRUE;
			g_bModify = TRUE;
		}
		break;

	case TDEL:																//�б�ɾ���༭
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

			size = size - CList_StartRow();									//��ʾ����

			if( size > ROWS )
				size = ROWS;												//��ʾ��������

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

	case TUP:																//�༭�������ƶ�
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

	case TDOWN:																//�༭�������ƶ�
		if(!g_bInterface)
		{
			INT16S row = CList_CurRow()+CList_StartRow();

			ptr = Line_Data+row;

			if( row == Line_Data_Size -1 )									//����һ��
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

	case TPAGEUP:															//���Ϸ�ҳ
		if(!g_bInterface)
		{
			CTeach_UpdatePage(-ROWS);
		}
		break;

	case TPAGEDOWN:															//���·�ҳ
		if(!g_bInterface)
		{
			CTeach_UpdatePage(ROWS);
		}
		break;

	case TRIGHT:															//�༭�������ƶ�
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

	case TLEFT:																//�༭�������ƶ�
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

	case TENTER:															//ȷ��������
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
			if(!CTeach_DataOk())								//�ú���У����������ݵ���ȷ�ԣ�ͬʱ��m_bStringֵ�洢��ptr->value[]��
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

				if(g_Ini.iSendClear>0 && g_iWorkStatus==NORUN && g_lStepNode==0)   //ȷ�ϼ����߱�����ǰ��ֵ����༭���ܣ���˴˴�ע�͵����Y��Ĵ���
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
	case '5': case '6': case '7': case '8':						//1-8���ּ�����,5-8������û�б���ʾ

		if(g_bCylinder)
		{
			INT16S temp = key-'1';

			WriteBit(temp,GetOut(temp)?0:1);

			if(GetOut(temp))
				sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT ON":"%1d���������",temp+1);
			else
				sprintf(g_pszMsgBuf,g_bEnglish?"%ld OUT OFF":"%1d�������˻�",temp+1);

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
		}																	//ת����һ��CASE

	case '9':case '0':case '.':case TMINUS:											//0.-���������

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


	case TSTEP:																//������ֹͣ��
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

	case TRUN:																//������
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

	case TBACKZERO:															//�����
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
			OSQPost(MsgQueue,g_bEnglish?"Please Raise StopIO!":"����ͣ��״̬!���в���ǰ��̧��ͣ��ť!");
			OSTimeDly(100);
			return FALSE;
		}

		Start(ch);
		g_bBackZero=TRUE;
		g_iErrorNo=0;

		switch(ch)
		{
		case 1:
			OSQPost(MsgQueue,g_bEnglish?"X Reset...":"X����...");

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
			OSQPost(MsgQueue,g_bEnglish?"Z Reset...":"Z����...");

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
			OSQPost(MsgQueue,g_bEnglish?"U Reset...":"U����...");

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
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"X�������!");
			break;

		case 3:
			g_bFirstResetZ = TRUE;
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"Z�������!");
			break;

		case 4:
			g_bFirstResetU = TRUE;
			OSQPost(MsgQueue,g_bEnglish?"Finish!":"U�������!");
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
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"X�������,��ȷ��ԭ���ź��Ƿ�����!");
				break;

			case 3:
				g_bFirstResetZ = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Z�������,��ȷ��ԭ���ź��Ƿ�����!");
				break;

			case 4:
				g_bFirstResetU = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"U�������,��ȷ��ԭ���ź��Ƿ�����!");
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
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"X�������,��ȷ��ԭ���ź��Ƿ�����!");
				break;

			case 3:
				g_bFirstResetZ = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"Z�������,��ȷ��ԭ���ź��Ƿ�����!");
				break;

			case 4:
				g_bFirstResetU = FALSE;
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"U�������,��ȷ��ԭ���ź��Ƿ�����!");
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
				OSQPost(MsgQueue,g_bEnglish?"Reset Error!":"��ԭ��ʧ��!");
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
				OSQPost(MsgQueue,g_bEnglish?"Goto StartPos Error!":"��������!");
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

		OSQPost(MsgQueue,g_bEnglish?"Finish!":"���!");
		g_lStepNode = 0;

		if(g_bMode)
		{
			g_bMode=FALSE;
			CTeach_ModeDisp();
			//		    CDC_OutText24Ex(563,406,g_bEnglish?"Auto":"�Զ�",g_TextColor5,g_nBk);
			//			LedOut(TESTLED,g_bMode?1:0);										//������ʾ�������ģʽ
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
			OSQPost(MsgQueue,g_bEnglish?"Goto X-StartPos...":"X��������...");
			break;

		case 2:
			OSQPost(MsgQueue,g_bEnglish?"Goto Y-StartPos...":"Y��������...");
			break;

		case 3:
			OSQPost(MsgQueue,g_bEnglish?"Goto Z-StartPos...":"Z��������...");
			break;

		case 4:
			OSQPost(MsgQueue,g_bEnglish?"Goto U-StartPos...":"U��������...");
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
					OSQPost(MsgQueue,g_bEnglish?"Goto to X-StartPos Error":"X���������!");
					return   FALSE;
				}

				SetAngle(tempch,0,TRUE);
				g_bGotoStart[0]=TRUE; 										//X���������
			}
			break;

		case 2:
			if(! Home(tempch,g_Ini.iBackDir[tempch-1]))
			{
				OSQPost(MsgQueue,g_bEnglish?"Y Reset Error":"Y���ԭ��ʧ��!");
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
					OSQPost(MsgQueue,g_bEnglish?"Goto to Y-StartPos Error":"Y���������!");
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
				OSQPost(MsgQueue,g_bEnglish?"Z Reset Error":"Z���ԭ��ʧ��!");
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
					OSQPost(MsgQueue,g_bEnglish?"Goto to Z-StartPos Error":"Z���������!");
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
				OSQPost(MsgQueue,g_bEnglish?"U Reset Error!":"U���ԭ��ʧ��!");
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
					OSQPost(MsgQueue,g_bEnglish?"Goto to U-StartPos Error":"U���������!");
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
		OSQPost(MsgQueue,g_bEnglish?"Finish":"���!");
		g_lStepNode = 0;

		if(g_bMode)
		{
			g_bMode=FALSE;
			CTeach_ModeDisp();
			//		    CDC_OutText24Ex(563,406,g_bEnglish?"auto":"�Զ�",g_TextColor5,g_nBk);
			//			LedOut(TESTLED,g_bMode?1:0);										//������ʾ�������ģʽ
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
			OSQPost(MsgQueue,g_bEnglish?"Settings of speed is invalid":"�ٶ������趨��Ч");
			return FALSE;
		}

		if( col == 2 )
		{
			if( string[0] )
			{
				FP32 n = (FP32)stringtof((INT8U *)string);

				if( n < 0.01 || n >g_Ini.fXMaxSpeed )
				{
					OSQPost(MsgQueue,g_bEnglish?"Speed of X is too large":"X�ٶ��趨������Χ");
					return FALSE;
				}
			}
		}

		if( col == 1 )
		{
			OSQPost(MsgQueue,g_bEnglish?"Can't change":"�ٶ�ָ��ܸı�");
			return FALSE;
		}

	}

	else if(ptr->value[0][0] == 'N')
	{

		if(/*col == 3 ||*/ col == CList_MaxCol()-2 || col == CList_MaxCol()-1 || col == CList_MaxCol()-3)
		{
			OSQPost(MsgQueue,g_bEnglish?"Settings of Loop is invalid":"�༭��Ч");
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
				OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");									
				return FALSE;
			}
		}

		if( col == 3 && string[0] )
		{   
			if(n<-9999.99 || n > 99999.99)  
			{
				OSQPost(MsgQueue,g_bEnglish?"-9999.99<y<99999.99":"-9999.99<���߳���<99999.99!");		
				return FALSE;
			}

			if(strcmp(string,"+")==0||strcmp(string,"-")==0)
			{
				OSQPost(MsgQueue,g_bEnglish?"Err Code ":"�������");		
				return FALSE;
			}
			//SetMM(2,0,TRUE);
		}

		if(col==CList_MaxCol()-3 && string[0]!=NULL)
		{
			if( n>5.001 || n < 0.0099)
			{
				OSQPost(MsgQueue,g_bEnglish?"5.0>=Ratio>=0.01":"5.0>=�ٶȱ���>=0.01");
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
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");					
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
						OSQPost(MsgQueue,g_bEnglish?"Angle Err":"�Ƕ��������,С������ӦС��0.360");		
						return FALSE;
					}

					if(n<-49.359 || n > 49.359)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-49.359<y<49.359":"-49.359<�Ƕ�<49.359!");		
						return FALSE;
					}
				}				
				//else
				{
					//if(g_Ini.iZSoftLimit)
					{
						if( n>=g_Ini.fMaxLimit[2] && (g_Ini.fMaxLimit[2]-g_Ini.fMinLimit[2])>0.01 )
						{
							OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"����λԽ��");					
							return FALSE;
						}
						else if(n <= g_Ini.fMinLimit[2] && (g_Ini.fMaxLimit[2]-g_Ini.fMinLimit[2])>0.01)
						{
							OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"����λԽ��");					
							return FALSE;
						}
					}
					if((strcmp(string,"+")==0||strcmp(string,"-")==0)&&(g_Ini.iAxisRunMode[2]==2))
					{
						OSQPost(MsgQueue,g_bEnglish?"Err Code ":"�������");		
						return FALSE;
					}	
				}
			}
		}

		if(g_Ini.iAxisCount>3)
		{
			if(col==5)
			{
				if(g_Ini.iAxisRunMode[3]==0)							//A��Ȧģʽ
				{					
					if( n>360.01 || n < -360.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");						
						return FALSE;
					}						
				}
				else if(g_Ini.iAxisRunMode[3]==1)						//A��Ȧģʽ
				{
					INT16S 	temp;
					FP32	tempf;
					FP64 	tt=n;

					temp=(INT16S)tt;						
					tempf=n-temp;

					if((tempf-0.359>0.001)||(tempf+0.359)<-0.001)	
					{
						OSQPost(MsgQueue,g_bEnglish?"Angle Err":"�Ƕ��������,С������ӦС��0.360");		
						return FALSE;
					}

					if(n<-49.359 || n > 49.359)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-49.359<y<49.359":"-49.359<�Ƕ�<49.359!");		
						return FALSE;
					}													
				}
				//else if(g_Ini.iAxisRunMode[3]==2)						//A˿��ģʽ
				{
					if( n>=g_Ini.fMaxLimit[3] && (g_Ini.fMaxLimit[3]-g_Ini.fMinLimit[3])>0.01 )
					{
						OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"����λԽ��");					
						return FALSE;
					}
					else if(n <= g_Ini.fMinLimit[3] && (g_Ini.fMaxLimit[3]-g_Ini.fMinLimit[3])>0.01)
					{
						OSQPost(MsgQueue,g_bEnglish?"Cross the Border":"����λԽ��");					
						return FALSE;
					}						

					if((strcmp(string,"+")==0||strcmp(string,"-")==0)&&(g_Ini.iAxisRunMode[3]==2))
					{
						OSQPost(MsgQueue,g_bEnglish?"Err Code ":"�������");		
						return FALSE;
					}								
				}	
			}
		}

		if(col==CList_MaxCol()-2)
		{
			if(n>6+0.01)
			{
				OSQPost(MsgQueue,g_bEnglish?"Probe <=6":"̽���趨<=6");
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
						OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"�����趨��Ч,ȷ��Ϊ01����");	
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
					OSQPost(MsgQueue,g_bEnglish?"Settings of speed is invalid":"�ٶ������趨��Ч");
					return FALSE;
				}

				if( col == 2 )
				{
					if( string[0] )
					{
						FP32 n = (FP32)stringtof((INT8U *)string);

						if( n < 0.01 || n >g_Ini.fXMaxSpeed )
						{
							OSQPost(MsgQueue,g_bEnglish?"Speed of X is too large":"X�ٶ��趨������Χ");
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
						OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");									
						return FALSE;
					}
				}

				if( col == 4 && string[0] )
				{   
					if(n<-9999.99 || n > 79999.99)  
					{
						OSQPost(MsgQueue,g_bEnglish?"-9999.99<y<79999.99":"-9999.99<���߳���<79999.99!");		
						return FALSE;
					}
				}

				if(col==CList_MaxCol()-3 && string[0]!=NULL)
				{
					if( n>5.001 || n < 0.0099)
					{
						OSQPost(MsgQueue,g_bEnglish?"5.0>=Ratio>=0.01":"5.0>=�ٶȱ���>=0.01");
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
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");					
								return FALSE;
							}
						}
						else
						{
							if( n>79999.99 || n < -79999.99)
							{
								OSQPost(MsgQueue,g_bEnglish?"79999.99>=Z>=-79999.99":"79999.99>=Z�᳤��>=-79999.99");					
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
								OSQPost(MsgQueue,g_bEnglish?"360>=angle>=-360":"360>=�Ƕ�>=-360");						
								return FALSE;
							}
						}
						else
						{
							if( n>79999.99 || n < -79999.99)
							{
								OSQPost(MsgQueue,g_bEnglish?"79999.99>=A>=-79999.99":"79999.99>=A�᳤��>=-79999.99");					
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
							OSQPost(MsgQueue,g_bEnglish?"probe <=3 at checking of wire out":"�����߼��ʱ,̽���趨<=3");
							return FALSE;
						}
						else
						{
							if(!g_Ini.iPaoXian&&n>(MAXCYLINDER+0.01))
							{
								OSQPost(MsgQueue,g_bEnglish?"Probe <=4":"̽���趨<=4");
								return FALSE;
							}
						}					
					}
					else
					{
						if(g_Ini.iPaoXian&&n>(MAXCYLINDER-1+0.01))
						{
							OSQPost(MsgQueue,g_bEnglish?"probe <=3 when checking of wire out":"�����߼��ʱ,̽���趨<=3");
							return FALSE;
						}
						else
						{

							if(g_Ini.iSafeDoor&&n>(MAXCYLINDER-2+0.01))
							{

								OSQPost(MsgQueue,g_bEnglish?"Probe<=2 when checking of safedoor":"�а�ȫ�ż��ʱ,̽���趨<=2");
								return FALSE;
							}
							else
							{	
								if(!g_Ini.iPaoXian&&n>(MAXCYLINDER+0.01))
								{
									OSQPost(MsgQueue,g_bEnglish?"Probe <=4":"̽���趨<=4");
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
							OSQPost(MsgQueue,g_bEnglish?"Settings of cylinder is invalid":"�����趨��Ч,ȷ��Ϊ01����");	
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
						OSQPost(MsgQueue,g_bEnglish?"1000>=value>=1":"1000>=����>=1");
						return FALSE;
					}
				}
				else if( string[0] )
				{
					OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"������Ч");
					return FALSE;
				}
			}
			break;


		case 'N':case 'E':
			if( string[0] )
			{
				OSQPost(MsgQueue,g_bEnglish?"Data is invalid":"������Ч");
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

	len = CEdit_GetLen();															//��¼��ǰ�༭��Ŀ������ַ�����

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
		CDC_OutText24(40,100,"0---�ļ�����ϵͳ",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,140,"1---����ϵͳ����",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,180,"2---���Ĺ�������",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,220,"3---�����������������",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,260,"4---���밴��������",WHITECOLOR,LIGHTBLUECOLOR);
		CDC_OutText24(40,300,"5---����ʱ���趨",WHITECOLOR,LIGHTBLUECOLOR);
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
				CPassword_Init(240,140,340,180,g_bEnglish?"Password ":"������֤",buf,g_TextColor2);

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
								if(g_Ini.iAxisRunMode[iAxisID]!=iAxisRunMode[iAxisID])//�����ͷ����仯������CTeach_Create
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
				CParaSet_Init(0,0,798,478); 							//�����

				if(CParaSet_DoModal())
				{
					if(fabs(tmpu-g_Ini.fStartOffset[3])>0.001)
						g_bFirstResetU=FALSE;  							//�˴�����break;

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

			case '6':														//�û���ϵͳ���������趨
			{
				sprintf(buf,g_bEnglish?"\n Mach Serial:%d\n":"\n  �û����������:%d\n",NewDog.User_ComputerNo);
				CPassword_Init(200,140,360,220,g_bEnglish?"User's System Pwd":"�û���ϵͳ����������֤",buf,g_TextColor2);

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
				case '7':  														//��˾��ϵͳ���������趨
					{
						sprintf(buf,g_bEnglish?"\nCompany Mach Code:%d\n":"\n  ��˾�����Ա��:%d",NewDog.Adtech_ComputerNo);

						CPassword_Init(200,140,400,220,g_bEnglish?"Company System Pwd":"��˾��ϵͳ����������֤",buf,g_TextColor2);

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
		OSQPost(MsgQueue,g_bEnglish?"Low":"����");
		break;

	case 2:
		OSQPost(MsgQueue,g_bEnglish?"Mid":"����");
		break;

	case 3:
		OSQPost(MsgQueue,g_bEnglish?"High":"����");
		break;

	default:
		break;
	}	
	/*
	switch(g_iSpeedBeilv)
	{
		case 1:
			CDC_OutText24Ex(735,445,g_bEnglish?"Low":"����",g_TextColor2,g_nTechfrmBk);
			break;

		case 2:
			CDC_OutText24Ex(735,445,g_bEnglish?"Mid":"����",WHITECOLOR,g_nTechfrmBk);
			break;

		case 3:
			CDC_OutText24Ex(735,445,g_bEnglish?"High":"����",REDCOLOR,g_nTechfrmBk);
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
			CDC_OutText_32(672,100,g_bEnglish?"Auto ":"�Զ�",WHITECOLOR, g_nBackColor);
		//CDC_TextSelect_32(600,94,134,39,DISPSTYLE,g_bEnglish?"Auto ":"�Զ�",WHITECOLOR, g_nBackColor);
		else
			CDC_OutText_32(672,100,g_bEnglish?"Hand ":"�ֶ�",WHITECOLOR, g_nBackColor);
		//CDC_TextSelect_32(600,94,134,39,DISPSTYLE,g_bEnglish?"Hand ":"�ֶ�",WHITECOLOR, g_nBackColor);
	}
	else
	{
		if(!g_bMode)
			CDC_OutText_32(150,409,g_bEnglish?"Auto ":"�Զ�",WHITECOLOR, g_nBackColor);
		else
			CDC_OutText_32(150,409,g_bEnglish?"Hand ":"�ֶ�",WHITECOLOR, g_nBackColor);
	}
}
