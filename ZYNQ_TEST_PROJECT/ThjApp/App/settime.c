#include	"includes.h"

extern		BASEINI 	g_Ini;
TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	BOOL		g_bEnglish;
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;

INT16S      g_iHour;
INT16S		g_iMin;
INT16S		g_iSec;
INT16S		g_iYear;
INT16S		g_iMon;
INT16S		g_iDay;
BOOL		g_bSetTime;
extern		BOOL		g_bInsertNum;	

#define	SETTIMEWOFFSET	160
#define	SETTIMEHOFFSET	36
#define	SETTIMECOUNT	9


void	CSetTime_Init(INT16S left,INT16S top,INT16S width,INT16S height)
{
	INT16S i,j,k;
	CTROL  temp;
	
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	g_iDlgCurObject=0;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	g_bSetTime=TRUE;
	ResumeTasks(7);

	g_iYear=0;
	g_iMon=0;
	g_iDay=0;
	g_iHour=0;
	g_iMin=0;
	g_iSec=0;

	CMainFrm_Init();
	CMainFrm_SetArraySize( SETTIMECOUNT );
	
	i=0;
	k=0;
	j=g_iDlgTop+80-SETTIMEHOFFSET;
	temp.style=CTROL_STYLE_EDIT;
	temp.ID=0x9001+k++;
	strcpy(temp.name,"");
	temp.x=g_iDlgLeft+120;
	temp.y=(j+=SETTIMEHOFFSET);
	temp.w=100;
	temp.h=30;
	temp.key=0;
	temp.algin=LEFT_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9001+k++;
	temp.y=(j+=SETTIMEHOFFSET);
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9001+k++;
	temp.y=(j+=SETTIMEHOFFSET);
	CMainFrm_Add(i++,&temp);


	temp.x=g_iDlgLeft+145+SETTIMEWOFFSET;
	j=g_iDlgTop+80-SETTIMEHOFFSET;
	temp.ID=0x9001+k++;
	temp.y=(j+=SETTIMEHOFFSET);
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9001+k++;
	temp.y=(j+=SETTIMEHOFFSET);
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9001+k++;
	temp.y=(j+=SETTIMEHOFFSET);
	CMainFrm_Add(i++,&temp);


	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"SetDate":"设定日期");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-140;
	temp.y=g_iDlgTop+g_iDlgHeight-100;
	temp.w=150;temp.h=40;
	temp.key=0;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"SetTime":"设定时间");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+20;
	temp.y=g_iDlgTop+g_iDlgHeight-100;
	temp.w=150;temp.h=40;
	temp.key=0;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Exit":"退 出");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-60;
	temp.y=g_iDlgTop+g_iDlgHeight-50;
	temp.w=120;temp.h=40;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);
}


void	CSetTime_Destroy(void)
{
	g_bSetTime=FALSE;
	CMainFrm_Destroy();
	SuspendTasks(7);
}


void   CSetTime_Create(void)
{
	INT16S	j=g_iDlgTop+82-SETTIMEHOFFSET;
	CTROL   *ptr;
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"Date time set":"日期时间设定");
	CDC_OutText24Ex(g_iDlgLeft+70,j+=SETTIMEHOFFSET,g_bEnglish?"Year":"年:",WHITECOLOR,g_nBk);
	CDC_OutText24Ex(g_iDlgLeft+70,j+=SETTIMEHOFFSET,g_bEnglish?"Mon":"月:",WHITECOLOR,g_nBk);
	CDC_OutText24Ex(g_iDlgLeft+70,j+=SETTIMEHOFFSET,g_bEnglish?"Day":"日:",WHITECOLOR,g_nBk);

	j=g_iDlgTop+80-SETTIMEHOFFSET;
	CDC_OutText24Ex(g_iDlgLeft+90+SETTIMEWOFFSET,j+=SETTIMEHOFFSET,g_bEnglish?"Hour":"时:",WHITECOLOR,g_nBk);
	CDC_OutText24Ex(g_iDlgLeft+90+SETTIMEWOFFSET,j+=SETTIMEHOFFSET,g_bEnglish?"Min":"分:",WHITECOLOR,g_nBk);
	CDC_OutText24Ex(g_iDlgLeft+90+SETTIMEWOFFSET,j+=SETTIMEHOFFSET,g_bEnglish?"Sec":"秒:",WHITECOLOR,g_nBk);
	CMainFrm_ShowAllControl();

	ptr=CMainFrm_GetItemID(0x9001+SETTIMECOUNT-3);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9001+SETTIMECOUNT-2);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9001+SETTIMECOUNT-1);
	CButton_SetCtrl(ptr);
	CButton_Create();

	CEdit_SetTextColor(BLUECOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iYear);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9002);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iMon);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9003);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iDay);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9004);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iHour);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9005);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iMin);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9006);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iSec);
	CEdit_UpdateData(TRUE);

	CEdit_SetTextColor(YELLOWCOLOR);
	CEdit_SetBackColor(LIGHTBLUECOLOR);
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetInt(g_iYear);
	CEdit_UpdateData(TRUE);
	CEdit_SetFirst(TRUE);
	CEdit_SetLen(4);
}


void	CSetTime_UpdateObject(INT16S dk,BOOL isEnter)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject<SETTIMECOUNT-3)
	{
		CEdit_SetCtrl(ptr);
		
		switch(g_iDlgCurObject)
		{
			case 0:
			    if(isEnter)
					g_iYear = CEdit_GetInt();
				
				if(g_iYear>2999)
					g_iYear=2000;
					
				if(g_iYear<0)
					g_iYear=0;
					
				CEdit_SetInt(g_iYear);
				break;
				
			case 1:
				if(isEnter)
					g_iMon = CEdit_GetInt();
				
				if(g_iMon>12)
					g_iMon=1;
					
				if(g_iMon<0)
					g_iMon=0;
					
				CEdit_SetInt(g_iMon);
				break;
				
			case 2:
				if(isEnter)
					g_iDay = CEdit_GetInt();
				
				if(g_iDay>31)
					g_iDay=1;
					
				if(g_iDay<0)
					g_iDay=0;
					
				CEdit_SetInt(g_iDay);
				break;
				
			case 3:
				if(isEnter)
					g_iHour = CEdit_GetInt();
				
				if(g_iHour>23)
					g_iHour=0;
					
				if(g_iHour<0)
					g_iHour=0;
					
				CEdit_SetInt(g_iHour);
				break;
				
			case 4:
				if(isEnter)
					g_iMin = CEdit_GetInt();
				
				if(g_iMin>59)
					g_iMin=0;
					
				if(g_iMin<0)
					g_iMin=0;
					
				CEdit_SetInt(g_iMin);
				break;
				
			case 5:
				if(isEnter)
					g_iSec = CEdit_GetInt();
				
				if(g_iSec>59)
					g_iSec=0;
					
				if(g_iSec<0)
					g_iSec=0;
					
				CEdit_SetInt(g_iSec);
				break;
			
			default:
				break;
		}
		
		CEdit_SetTextColor(BLUECOLOR);
		CEdit_SetBackColor(WHITECOLOR);
		CEdit_UpdateData(FALSE);
	}
	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Normal);
	}
	
	g_iDlgCurObject += dk;
	
	if(g_iDlgCurObject >= SETTIMECOUNT)
		g_iDlgCurObject = 0;
		
	if(g_iDlgCurObject < 0)
		g_iDlgCurObject = SETTIMECOUNT-1;
		
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject < SETTIMECOUNT-3)
	{
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
		
		switch(g_iDlgCurObject)
		{
			case 0:
				CEdit_SetInt(g_iYear);
				CEdit_SetLen(4);
				break;
					
			case 1:
				CEdit_SetInt(g_iMon);
				CEdit_SetLen(2);
				break;
					
			case 2:
				CEdit_SetInt(g_iDay);
				CEdit_SetLen(2);
				break;
					
			case 3:
				CEdit_SetInt(g_iHour);
				CEdit_SetLen(2);
				break;
					
			case 4:
				CEdit_SetInt(g_iMin);
				CEdit_SetLen(2);
				break;
					
			case 5:
				CEdit_SetInt(g_iSec);
				CEdit_SetLen(2);
				break;
				
			default:
				break;
		}
		
		CEdit_SetTextColor(YELLOWCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_DrawActive();
	}
	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Foucs);
	}
}


void	CSetTime_OnButtonSetDate(void)
{
	DATETIME_T dat;
	
	RtcGetDateTime(&dat);
	
	dat.year=g_iYear;
	dat.month=g_iMon;
	dat.day=g_iDay;
	
	if(dat.hour>23)
   		dat.hour=0;
   		
   	if(dat.minute>59)
   		dat.minute=0;
   		
   	if(dat.second>59)
   		dat.second=0;
   		
	RtcSetDateTime(&dat);	
}


void	CSetTime_OnButtonSetTime(void)
{
	DATETIME_T tim;
	
	RtcGetDateTime(&tim);
	tim.hour=g_iHour;
	tim.minute=g_iMin;
	tim.second=g_iSec;
	tim.year+=2000;
	
	if(tim.year>2099)
   		tim.year=2000;
   		
   	if(tim.month>12)
   		tim.month=0;
   		
   	if(tim.day>31)
   		tim.day=0;
   		
	RtcSetDateTime(&tim);
}


void	CSetTime_OnButtonExit(void)
{
   Ok_Exit=TRUE;
   CEdit_SetTextColor(g_nBk);
   CEdit_SetBackColor(g_TextColor1);
   g_iDlgIDOk=1;
}


void	CSetTime_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	CEdit_SetCtrl(ptr);
	g_bInsertNum=FALSE;
	
	switch(key)
	{
	
	#ifdef	PRINTBMP															//屏幕截图 启动键
		case F5:
			{
				char   filename[0x20];
				INT16S i=0;
				
//				CBmp_Init();
				strcpy(filename,"timeset");
				
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
//				CBmp_Destroy();
			}
			break;
	#endif
		
		case TUP:
		case TLEFT:
			CSetTime_UpdateObject(-1,FALSE);
			break;
			
		case TDOWN:
		case TRIGHT:
			CSetTime_UpdateObject(1,FALSE);
			break;
			
		case F7:
			CSetTime_OnButtonSetDate();
			break;
			
		case F2:
			CSetTime_OnButtonSetTime();
			break;

		case TSCREEN: case TCANCEL:
			CSetTime_OnButtonExit();
			break;
			
		case TENTER:
			{
				if(g_iDlgCurObject < SETTIMECOUNT-3)
				{
					CSetTime_UpdateObject(1,TRUE);
				}
				else
				{
					if(g_iDlgCurObject == SETTIMECOUNT-1)
						CSetTime_OnButtonExit();
					
					if(g_iDlgCurObject == SETTIMECOUNT-2)
					{
						CSetTime_OnButtonSetTime();
					}
					
					if(g_iDlgCurObject == SETTIMECOUNT-3)
					{
						CSetTime_OnButtonSetDate();
					}
				}
				break;
			}
			
		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case '-':
			g_bInsertNum=TRUE;
			
		case TCLEAR:case BACK:
			if(g_iDlgCurObject < SETTIMECOUNT-3)
				CEdit_OnKey(key,tkey);
			break;
			
		default:break;
	}
}


INT16S    CSetTime_DoModal(void)
{
	INT16U key,tkey=0;
	
	CSetTime_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{
			CSetTime_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		if(RealIO(STOPIO,0,2))
		{
			CSetTime_OnButtonExit();
			break;
		}
		
		OSTimeDly(100);
	}
	
	Ok_Exit=FALSE;
	
	return g_iDlgIDOk;
}