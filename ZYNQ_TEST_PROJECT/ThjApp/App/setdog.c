#include	"includes.h"

extern		BASEINI 	g_Ini;
extern 		NEWDOG		NewDog;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
WORK_EXT	BOOL		g_bHandset;
extern		INT16S		SVGAmode;
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
extern		BOOL		g_bInsertNum;	

#define		SETDOGHOFFSET	40
#define		SETDOGWOFFSET	300
#define		SETDOGCOUNT		4


void	CSetDog_Init(INT16S left,INT16S top,INT16S width,INT16S height)
{
	INT16S i,j,k,m;
	CTROL  temp;
	
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	g_iDlgCurObject=0;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;

	CMainFrm_Init();
	CMainFrm_SetArraySize( SETDOGCOUNT );
	i=0;
	k=0;
	j=g_iDlgTop+20;
	temp.style=CTROL_STYLE_EDIT;
	temp.x=g_iDlgLeft+205;
	temp.w=140;
	temp.h=30;
	temp.key=0;
	temp.algin=LEFT_ALGIN;

	for(m=0; m<2; m++)
	{
		temp.ID=0x9001+k++;
		temp.y=(j+=SETDOGHOFFSET);
		CMainFrm_Add(i++,&temp);
	}

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Ok":"确定");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-100;
	temp.y=g_iDlgTop+g_iDlgHeight-45;
	temp.w=90;temp.h=36;
	temp.key=TSAVE;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"取消");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+10;
	temp.y=g_iDlgTop+g_iDlgHeight-45;
	temp.w=90;temp.h=36;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

}


void	CSetDog_Destroy(void)
{
	CMainFrm_Destroy();
}


void   CSetDog_Create(void)
{
	INT16S	j=g_iDlgTop+20;
	CTROL   *ptr;

	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"Adtech password set":"公司级软件狗设定");
	
	{
		CDC_OutText24Ex(g_iDlgLeft+30,j+=SETDOGHOFFSET,g_bEnglish?"Factory ID:":"公司级电脑编号:",WHITECOLOR,g_nBk);
		CDC_OutText24Ex(g_iDlgLeft+30,j+=SETDOGHOFFSET,g_bEnglish?"Use days":"使用天数:",WHITECOLOR,g_nBk);
		//CDC_OutText24Ex(g_iDlgLeft+30,j+=SETDOGHOFFSET,g_bEnglish?"Login Password":"登录密码:",WHITECOLOR,g_nBk);
	}
	
	CMainFrm_ShowAllControl();

	ptr=CMainFrm_GetItemID(0x9001+SETDOGCOUNT-2);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9001+SETDOGCOUNT-1);
	CButton_SetCtrl(ptr);
	CButton_Create();

 	CEdit_SetTextColor(BLUECOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	

	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.Adtech_ComputerNo);
	CEdit_UpdateData(TRUE);
	
	ptr=CMainFrm_GetItemID(0x9002);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.Adtech_Days);
	CEdit_UpdateData(TRUE);
	
/*	ptr=CMainFrm_GetItemID(0x9003);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.Adtech_Password);
	CEdit_UpdateData(TRUE);*/
	

	CEdit_SetTextColor(YELLOWCOLOR);
	CEdit_SetBackColor(LIGHTBLUECOLOR);
	
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.Adtech_ComputerNo);
	CEdit_SetFirst(TRUE);
	CEdit_UpdateData(TRUE);
	CEdit_SetLen(8);

}


void	CSetDog_UpdateObject(INT16S dk,BOOL isEnter)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject<SETDOGCOUNT-2)
	{
		CEdit_SetCtrl(ptr);
		
		switch(g_iDlgCurObject)
		{
			case 0:
				if(isEnter)
					NewDog.Adtech_ComputerNo=CEdit_GetLong();
				
				CEdit_SetLong(NewDog.Adtech_ComputerNo);
				break;
				
			case 1:
				if(isEnter)
					NewDog.Adtech_Days=CEdit_GetLong();
				
				CEdit_SetLong(NewDog.Adtech_Days);
				break;
				
		/*	case 2:
				NewDog.Adtech_Password=CEdit_GetLong();
				CEdit_SetLong(NewDog.Adtech_Password);
				break;*/
				  
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
	
	if(g_iDlgCurObject >= SETDOGCOUNT)
		g_iDlgCurObject=0;
		
	if(g_iDlgCurObject < 0)
		g_iDlgCurObject = SETDOGCOUNT-1;
		
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject < SETDOGCOUNT-2)
	{
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
		
		switch(g_iDlgCurObject)
		{
			case 0:
				CEdit_SetLong(NewDog.Adtech_ComputerNo);
				CEdit_SetLen(8);
				break;
				
			case 1:
				CEdit_SetLong(NewDog.Adtech_Days);
				CEdit_SetLen(3);
				break;
				
		/*	case 2:
				CEdit_SetLong(NewDog.Adtech_Password);
				CEdit_SetLen(6);
				break;*/

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


void	CSetDog_OnButtonOk(void)
{
	if(NewDog.Adtech_Days==0)
		NewDog.Adtech_Active=12345678;
	else
		NewDog.Adtech_Active=0;

	NewDog.First_Use = 0;

	WriteDog();
	Ok_Exit=TRUE;
	CEdit_SetTextColor(g_nBk);
    CEdit_SetBackColor(g_TextColor1); 
	g_iDlgIDOk=1;
}



void	CSetDog_OnButtonCancel(void)											//取消退出
{
   ReadDog();
   Ok_Exit=TRUE;
   CEdit_SetTextColor(g_nBk);
   CEdit_SetBackColor(g_TextColor1); 
   g_iDlgIDOk=0;
}


void	CSetDog_OnKey(INT16U key,INT16U tkey)
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
				strcpy(filename,"setdog");
				
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
			CSetDog_UpdateObject(-1,FALSE);
			break;
				
		case TDOWN:
		case TRIGHT:
			CSetDog_UpdateObject(1,FALSE);
			break;
			
		case TSAVE:
			CSetDog_OnButtonOk();
			break;
		
		case TCANCEL:
			CSetDog_OnButtonCancel();
			break;
			
		case TENTER:
			if(g_iDlgCurObject != SETDOGCOUNT-2 && g_iDlgCurObject != SETDOGCOUNT-1)
			{
				CSetDog_UpdateObject(1,TRUE);
			}
			else
			{
				if(g_iDlgCurObject==SETDOGCOUNT-2)
					CSetDog_OnButtonOk();
					
				if(g_iDlgCurObject==SETDOGCOUNT-1)
					CSetDog_OnButtonCancel();
			}
			break;

		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case '-':
			g_bInsertNum=TRUE;
			
		case TCLEAR:case BACK:
			
			if(g_iDlgCurObject < SETDOGCOUNT-2)
				CEdit_OnKey(key,tkey);
				
			break;
			
		default:
			break;
		
	}
}


INT16S    CSetDog_DoModal(void)
{
	INT16U key,tkey=0;
	
	CSetDog_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{
			CSetDog_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		if(RealIO(STOPIO,0,2) || (g_bHandset?RealIO(STOPIO2,0,3):0))
		{
			CSetDog_OnButtonCancel();
			break;
		}
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	
	return g_iDlgIDOk;
}