#include	"includes.h"

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

TEST_EXT  	OS_EVENT 	*MsgQueue;

INT8S		g_pszButton1[10]="È·¶¨";
INT8S   	g_pszButton2[10]="È¡Ïû";


void	CMsgDlg_Init(INT16S left,INT16S top,INT16S width,INT16S height,
					 INT8S *message,INT8S *title,INT32S tc,INT8S *button1,
					 INT8S *button2)
{
	INT16S i=0;
	CTROL  temp;
	
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	g_iDlgCurObject=0;
	
	strcpy(g_pszDlgDispStr,message);
	strcpy(g_pszDlgWindowText,title);
	
	g_lDlgTextColor=tc;
	
	strcpy(g_pszButton1,button1);
	strcpy(g_pszButton2,button2);

	CMainFrm_Init();
	CMainFrm_SetArraySize( 2 );
	
	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001;
	strcpy(temp.name,g_pszButton1);
	temp.x=g_iDlgLeft+g_iDlgWidth/2-60;
	temp.y=g_iDlgTop+g_iDlgHeight-40;
	temp.w=60;
	temp.h=30;
	temp.key=0;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9002;
	strcpy(temp.name,g_pszButton2);
	temp.x=g_iDlgLeft+g_iDlgWidth/2+10;
	temp.y=g_iDlgTop+g_iDlgHeight-40;
	temp.key=0;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);
}


void	CMsgDlg_Destroy()
{
	CMainFrm_Destroy();
}


void   CMsgDlg_Create()
{
	CTROL *ptr;
	CRect rc;
	
	rc.left = g_iDlgLeft;
	rc.right = g_iDlgLeft+g_iDlgWidth;
	rc.top = g_iDlgTop;
	rc.bottom = g_iDlgTop+g_iDlgHeight;
	
	//CDC_DrawWin(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_pszDlgWindowText);
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_pszDlgWindowText);
	CDC_OutCenTextKzk24(rc,g_pszDlgDispStr,g_lDlgTextColor,DARKGRAYCOLOR);
	CMainFrm_ShowAllControl();
	
	ptr=CMainFrm_GetItemID(0x9002);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9001);
	CButton_SetCtrl(ptr);
	CButton_Create();
	CButton_DrawItem(Foucs);
}


void	CMsgDlg_OnButtonOk()
{
	Ok_Exit=TRUE;
	g_iDlgIDOk=1;
}


void	CMsgDlg_OnButtonCancel()
{
	Ok_Exit=TRUE;
	g_iDlgIDOk=0;
}


void	CMsgDlg_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;

	switch(key)
	{
		case SHIFT_TAB:
		case TUP:
		case TLEFT:
			ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
			CButton_SetCtrl(ptr);
			CButton_DrawItem(Normal);
			g_iDlgCurObject--;
		
			if(g_iDlgCurObject < 0)
				g_iDlgCurObject = 1;
		
			ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
			CButton_SetCtrl(ptr);
			CButton_DrawItem(Foucs);
			break;
			
		case TAB:
		case TDOWN:
		case TRIGHT:
			ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
			CButton_SetCtrl(ptr);
			CButton_DrawItem(Normal);
			g_iDlgCurObject++;
		
			if(g_iDlgCurObject > 1)
				g_iDlgCurObject = 0;
		
			ptr=CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
			CButton_SetCtrl(ptr);
			CButton_DrawItem(Foucs);
			break;
			
		case TENTER:
			{
				if(g_iDlgCurObject==0)
					CMsgDlg_OnButtonOk();
				else
					CMsgDlg_OnButtonCancel();
				break;
			}
			
		case TSAVE:
			CMsgDlg_OnButtonOk();
			break;
			
		case TCANCEL:
			{
				CMsgDlg_OnButtonCancel();
				break;
			}
		
	#ifdef	PRINTBMP															//ÆÁÄ»½ØÍ¼ ¹éÁã¼ü
		case F5:
			{
				char   filename[0x20];
				INT16S i = 0;
				
//				CBmp_Init();
				strcpy(filename,"MsgDlg");
				
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
//				CBmp_Destroy();
			}
			
			break;
	#endif
		default:
			break;
	}
}


INT16S    CMsgDlg_DoModal()
{
	INT16U key=0,tkey=0;
	
	CMsgDlg_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{
			CMsgDlg_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}