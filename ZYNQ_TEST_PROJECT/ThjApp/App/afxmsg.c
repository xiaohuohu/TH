#include	"includes.h"

TEST_EXT	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;

TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;

WORK_EXT	INT32S 		g_nBk;

TEST_EXT	INT8S	    g_pszDlgDispStr[0x80];
TEST_EXT	INT8S   	g_pszDlgWindowText[0x35];
TEST_EXT	INT32S   	g_lDlgTextColor;


void	CAfxMsgDlg_Init(INT16S left,INT16S top,INT16S width,INT16S height,
						INT8S *message,INT8S *title,INT32S tc)
{
	INT16S i=0;
	CTROL temp;
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	strcpy(g_pszDlgDispStr,message);
	strcpy(g_pszDlgWindowText,title);
	g_lDlgTextColor=tc;

	CMainFrm_Init();
	CMainFrm_SetArraySize( 1 );
	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001;	
	strcpy(temp.name,g_bEnglish?"OK":"È·¶¨");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-45;
	temp.y=g_iDlgTop+g_iDlgHeight-40;
	temp.w=80;
	temp.h=30;
	temp.key=TENTER;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);
}


void	CAfxMsgDlg_Destroy()
{
	CMainFrm_Destroy();
}


void   CAfxMsgDlg_Create()
{
	CTROL *ptr;
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_pszDlgWindowText);
	CDC_OutText24Ex(g_iDlgLeft+30,g_iDlgTop+50,g_pszDlgDispStr,g_lDlgTextColor,g_nBk);
	CMainFrm_ShowAllControl();
	ptr=CMainFrm_GetItemID(0x9001);
	CButton_SetCtrl(ptr);
	CButton_Create();
	CButton_DrawItem(Foucs);
}


void	CAfxMsgDlg_OnButtonOk()
{
	Ok_Exit=TRUE;
	g_iDlgIDOk=1;
}


//#pragma warn -par
void	CAfxMsgDlg_OnKey(INT16U key,INT16U tkey)
{
	switch(key)
	{
	case TENTER:
		{
			CAfxMsgDlg_OnButtonOk();
			break;
		}
		
#ifdef	PRINTBMP																//ÆÁÄ»½ØÍ¼ Æô¶¯¼ü
	case F5:
		{
			char filename[0x20];
			INT16S i=0;
		//	CBmp_Init();
			strcpy(filename,"afms");
			
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
	//		CBmp_Destroy();
	//		OSQPost(MsgQueue,"Write Bmp End!");
		}
		break;
#endif
		
	default:break;
	}
}


INT16S    CAfxMsgDlg_DoModal()
{
	INT16U key,tkey=0;
	
	CAfxMsgDlg_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			CAfxMsgDlg_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}