#include	"includes.h"
#include    "base.h"

//extern	CMouse g_mouse;
extern	BASEINI g_Ini;
extern	INT16S	SVGAmode;

TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;

TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;

TEST_EXT	INT8S	    g_pszDlgDispStr[0x80];
TEST_EXT	INT8S   	g_pszDlgWindowText[0x35];
TEST_EXT	INT32S   	g_lDlgTextColor;
WORK_EXT	INT32S 		g_nBk;
extern      BOOL       	g_bInsertNum;

INT32S		g_lPassword=0;
INT8S       g_Password[0x20];


void	CPassword_Init(INT16S left,INT16S top,INT16S width,INT16S height,
						INT8S *title,INT8S *message,INT32S tc)
{
	INT16S i,k;
	CTROL temp;
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	g_iDlgCurObject=0;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	strcpy(g_pszDlgDispStr,message);
	strcpy(g_pszDlgWindowText,title);
	g_lDlgTextColor=tc;

	g_lPassword=0;

	CMainFrm_Init();
	CMainFrm_SetArraySize( 3 );
	
	i=0;
	k=0;

	temp.style=CTROL_STYLE_EDIT;
	temp.ID=0x9001+k++;
	strcpy(temp.name,"");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-55;
	temp.y=g_iDlgTop+g_iDlgHeight-110;
	temp.w=180;
	temp.h=32;
	temp.key=0;
	temp.algin=LEFT_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"OK":"确定");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-90;
	temp.y=g_iDlgTop+g_iDlgHeight-47;
	temp.w=85;temp.h=40;
	temp.key=TSAVE;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Exit":"取消");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+20;
	temp.y=g_iDlgTop+g_iDlgHeight-47;
	temp.w=85;temp.h=40;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);
}


void	CPassword_Destroy()
{
	CMainFrm_Destroy();
}


void   CPassword_Create()
{
	CTROL *ptr;
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_pszDlgWindowText);

	CDC_OutText24Ex(g_iDlgLeft+10,g_iDlgTop+36,g_pszDlgDispStr,g_lDlgTextColor,g_nBk);
	
	CDC_OutText24Ex(g_iDlgLeft+g_iDlgWidth/2-125,g_iDlgTop+g_iDlgHeight-105,g_bEnglish?" PWD":"密码:",WHITECOLOR,g_nBk);
	CMainFrm_ShowAllControl();

	ptr=CMainFrm_GetItemID(0x9003);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9002);
	CButton_SetCtrl(ptr);
	CButton_Create();


	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetString("");
	CEdit_SetNeedPassword(TRUE);
	CEdit_UpdateData(TRUE);
	CEdit_SetFirst(TRUE);
	CEdit_SetLen(10);
}


void	CPassword_UpdateObject(INT16S dk)
{
	CTROL *ptr;
	ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject<1)
	{
		CEdit_SetCtrl(ptr);

		switch(g_iDlgCurObject)
		{
		case 0:
				if(CEdit_IsEmpty())
				{
					CEdit_SetString("");
				}
				else
				{
					g_lPassword=CEdit_GetLong();
					CEdit_String(g_Password);
					CEdit_SetString(g_Password);
				}
				break;
		default:break;
		}

		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(DARKGRAYCOLOR);
		CEdit_UpdateData(FALSE);
	}
	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Normal);
	}
	
	g_iDlgCurObject+=dk;

	if(g_iDlgCurObject>2)
		g_iDlgCurObject=0;

	if(g_iDlgCurObject<0)
		g_iDlgCurObject=2;

	ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject<1)
	{
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
	
		switch(g_iDlgCurObject)
		{
			case 0:
				CEdit_SetString("");
				CEdit_SetLen(10);
				break;
			default:break;
		}
		
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(WHITECOLOR);
		CEdit_DrawActive();
	}
	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Foucs);
	}
}


INT32S	CPassword_GetPassword()
{
	return g_lPassword;
}


void	CPassword_GetPasswordEx(INT8S *str)
{
	strcpy(str,g_Password);
}


void	CPassword_OnButtonOk()
{
	g_lPassword=CEdit_GetLong();
	Ok_Exit=TRUE;
	CEdit_String(g_Password);
	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	g_iDlgIDOk=1;
}


//取消退出
void	CPassword_OnButtonCancel()
{
   g_lPassword=0;
   Ok_Exit=TRUE;
   CEdit_String(g_Password);
   CEdit_SetTextColor(BLACKCOLOR);
   CEdit_SetBackColor(WHITECOLOR);
   g_iDlgIDOk=0;
}


void	CPassword_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;

	ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	CEdit_SetCtrl(ptr);
	g_bInsertNum=FALSE;
	
	switch(key)
	{
	case TUP:
	case TLEFT:
			CPassword_UpdateObject(-1);
			break;
			
	case TDOWN:
	case TRIGHT:
			CPassword_UpdateObject(1);
			break;
			
	case TSAVE:CPassword_OnButtonOk();break;

	case TCANCEL:
		CPassword_OnButtonCancel();break;
		
	case TENTER:
		{			
			{				
				if(g_iDlgCurObject == 2)
					CPassword_OnButtonCancel();
				else
					CPassword_OnButtonOk();
			}
			break;
		}
		
	case '0':case '1':case '2':case '3':case '4':case '5':
	case '6':case '7':case '8':case '9':case '.':case '-':
		g_bInsertNum=TRUE;
	case TCLEAR:case TBACK:
		if(g_iDlgCurObject<1)
			CEdit_OnKey(key,tkey);
		break;
	
		
#ifdef	PRINTBMP																//屏幕截图 启动键
	case F5:
		{
			char filename[0x20];
			INT16S i=0;
//			CBmp_Init();
			strcpy(filename,"pwd");
			
			for(i=0;i<strlen(filename);i++)
			{
				if(filename[i]=='.')
				{
					filename[i]='\0';
						break;
				}
			}
			
			strcat(filename,".bmp");
			CreatBMP(filename,800,480,1);
//			CBmp_Destroy();
	//		OSQPost(MsgQueue,"Write Bmp End!");
		}
		break;
#endif

	default:break;
	}
}


INT16S    CPassword_DoModal()
{
	INT16U key,tkey;
	
	CPassword_Create();

	while(TRUE)
	{
		key=KeyMBoxGetKey();
		
		if(key!=0xffff)
		{

			CPassword_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		if(RealIO(STOPIO,0,2))
		{
			CPassword_OnButtonCancel();
			break;
		}
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	CEdit_SetNeedPassword(FALSE);
	
	return g_iDlgIDOk;
}