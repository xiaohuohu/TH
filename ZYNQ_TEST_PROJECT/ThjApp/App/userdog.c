#include	"includes.h"

extern		BASEINI 	g_Ini;
extern 		NEWDOG		NewDog;
extern		BOOL		g_bInsertNum;
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

/*
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TextColor7;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
*/


#define	USERDOGHOFFSET	32
#define	USERDOGWOFFSET	300
#define	USERDOGCOUNT	6

void	CUserDog_Init(INT16S left,INT16S top,INT16S width,INT16S height)
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
	CMainFrm_SetArraySize( USERDOGCOUNT );
	
	i=0;
	k=0;
	j=g_iDlgTop+60-USERDOGHOFFSET;
	temp.style=CTROL_STYLE_EDIT;
	strcpy(temp.name,"");
	temp.x=g_iDlgLeft+230;
	temp.w=140;
	temp.h=30;
	temp.key=0;
	temp.algin=LEFT_ALGIN;
	
	for(m=0; m<4; m++)
	{
		temp.ID=0x9001+k++;
		temp.y=(j+=USERDOGHOFFSET);
		CMainFrm_Add(i++,&temp);
	}
	
	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Ok":"确定");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-100;
	temp.y=g_iDlgTop+g_iDlgHeight-45;
	temp.w=100;temp.h=36;
	temp.key=0;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9001+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"取消");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+10;
	temp.y=g_iDlgTop+g_iDlgHeight-45;
	temp.w=100;temp.h=36;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

}


void	CUserDog_Destroy(void)
{
	CMainFrm_Destroy();
}


void   CUserDog_Create(void)
{
	INT16S	j=g_iDlgTop+60-USERDOGHOFFSET;
	CTROL   *ptr;	
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"User Password Set":"用户级软件狗设定");
	
	{
		CDC_OutText_UTF(g_iDlgLeft+35,j+=USERDOGHOFFSET+(g_bEnglish?2:0),g_bEnglish?"Machine serial":"软件狗识别码:",g_nTableBk,g_TextColor7,KZK_24);
		CDC_OutText_UTF(g_iDlgLeft+35,j+=USERDOGHOFFSET+(g_bEnglish?2:0),g_bEnglish?"User Identifier":"客户机器编号:",g_nTableBk,g_TextColor7,KZK_24);
		CDC_OutText_UTF(g_iDlgLeft+35,j+=USERDOGHOFFSET+(g_bEnglish?2:0),g_bEnglish?"Valid Days":"使用天数:",g_nTableBk,g_TextColor7,KZK_24);
		CDC_OutText_UTF(g_iDlgLeft+35,j+=USERDOGHOFFSET+(g_bEnglish?2:0),g_bEnglish?"Login Password":"登录密码:",g_nTableBk,g_TextColor7,KZK_24);
	}
	
	CMainFrm_ShowAllControl();

	ptr=CMainFrm_GetItemID(0x9001+USERDOGCOUNT-2);
	CButton_SetCtrl(ptr);
	CButton_Create();

	ptr=CMainFrm_GetItemID(0x9001+USERDOGCOUNT-1);
	CButton_SetCtrl(ptr);
	CButton_Create();
	
	CEdit_SetTextColor(BLUECOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.UserDogIdentifier);
	CEdit_UpdateData(TRUE);

	ptr=CMainFrm_GetItemID(0x9002);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.User_ComputerNo);
	CEdit_UpdateData(TRUE);


	ptr=CMainFrm_GetItemID(0x9003);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.User_Days);
	CEdit_UpdateData(TRUE);

	
	ptr=CMainFrm_GetItemID(0x9004);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.User_Password);
	CEdit_UpdateData(TRUE);


	CEdit_SetTextColor(YELLOWCOLOR);
	CEdit_SetBackColor(LIGHTBLUECOLOR);
	
	ptr=CMainFrm_GetItemID(0x9001);
	CEdit_SetCtrl(ptr);
	CEdit_SetLong(NewDog.UserDogIdentifier);
	CEdit_SetFirst(TRUE);
	CEdit_UpdateData(TRUE);
	CEdit_SetLen(4);
}


void	CUserDog_UpdateObject(INT16S dk,BOOL isEnter)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject < USERDOGCOUNT-2)
	{
		CEdit_SetCtrl(ptr);
		
		switch(g_iDlgCurObject)
		{
			case 0:
				if(isEnter)
				{
					NewDog.UserDogIdentifier=CEdit_GetLong();
				}
				
				CEdit_SetLong(NewDog.UserDogIdentifier);
				break;
				
			case 1:
				if(isEnter)
			 	{
			 		NewDog.User_ComputerNo=CEdit_GetLong();
			 	}
			 	
				CEdit_SetLong(NewDog.User_ComputerNo);
				break;
				
			case 2:
				if(isEnter)
				{	
					NewDog.User_Days=CEdit_GetLong();
				}
				
				CEdit_SetLong(NewDog.User_Days);
				break;
				
			case 3:
				if(isEnter)
				{
					NewDog.User_Password=CEdit_GetLong();
				}
				
				CEdit_SetLong(NewDog.User_Password);
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
	
	if(g_iDlgCurObject >= USERDOGCOUNT)
		g_iDlgCurObject = 0;
		
	if(g_iDlgCurObject < 0)
		g_iDlgCurObject = USERDOGCOUNT-1;
		
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	if(g_iDlgCurObject < USERDOGCOUNT-2)
	{
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
		
		switch(g_iDlgCurObject)
		{
			case 0:
				CEdit_SetLong(NewDog.UserDogIdentifier);
				CEdit_SetLen(4);
				break;
				
			case 1:
				CEdit_SetLong(NewDog.User_ComputerNo);
				CEdit_SetLen(8);
				break;
				
			case 2:
				CEdit_SetLong(NewDog.User_Days);
				CEdit_SetLen(3);
				break;
				
			case 3:
				CEdit_SetLong(NewDog.User_Password);
				CEdit_SetLen(8);
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


void	CUserDog_OnButtonOk(void)
{
	
	if(NewDog.User_Days==0)
		NewDog.User_Active=12345678;
	else
		NewDog.User_Active=0;
	
	NewDog.First_Use = 0;

	WriteDog();
	
	Ok_Exit=TRUE;
	CEdit_SetTextColor(g_nBk);
   	CEdit_SetBackColor(g_TextColor1); 
	g_iDlgIDOk=1;
}


void	CUserDog_OnButtonCancel(void)
{
   ReadDog();
   Ok_Exit=TRUE;
   CEdit_SetTextColor(g_nBk);
   CEdit_SetBackColor(g_TextColor1); 
   g_iDlgIDOk=0;
}


void	CUserDog_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9001+g_iDlgCurObject);
	
	CEdit_SetCtrl(ptr);
	
	g_bInsertNum=FALSE;
	
	switch(key)
	{	
	
	#ifdef	PRINTBMP															//屏幕截图 归零键
		case F5:
			{
				char   filename[0x20];
				INT16S i=0;
				
//				CBmp_Init();
				strcpy(filename,"usrdog");
				
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
		
			
		case TUP:
		case TLEFT:
			CUserDog_UpdateObject(-1,FALSE);
			break;
				
		case TDOWN:
		case TRIGHT:
			CUserDog_UpdateObject(1,FALSE);
			break;
			
		case TSAVE:
			CUserDog_OnButtonOk();
			break;
		
		case TCANCEL:
			CUserDog_OnButtonCancel();
			break;
			
		case TENTER:
			{
				if(g_iDlgCurObject != USERDOGCOUNT-2 && g_iDlgCurObject != USERDOGCOUNT-1)
				{
					CUserDog_UpdateObject(1,TRUE);
				}
				else
				{
					if(g_iDlgCurObject == USERDOGCOUNT-2)
						CUserDog_OnButtonOk();
						
					if(g_iDlgCurObject==USERDOGCOUNT-1)
						CUserDog_OnButtonCancel();
				}
				break;
			}


		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case '-':
		case TBACKSPACE:
		
			if(g_iDlgCurObject < USERDOGCOUNT-2)
			{	
				if(key!=TBACKSPACE)
					g_bInsertNum=TRUE;
					
				CEdit_OnKey(key,tkey);
			}
			break;
			
		default:
			break;
	}
}


INT16S    CUserDog_DoModal(void)
{
	INT16U key=0,tkey=0;
	
	CUserDog_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{
			CUserDog_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		if(RealIO(STOPIO,0,2) || (g_bHandset?RealIO(STOPIO2,0,3):0))
		{
			CUserDog_OnButtonCancel();
			break;
		}
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	
	return g_iDlgIDOk;
}