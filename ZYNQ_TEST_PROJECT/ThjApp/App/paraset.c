#include	"includes.h"

extern		BASEINI 	g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	BOOL		g_bEnglish;

/*
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TextColor6;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
*/

extern      INT32S      g_iLayNum;
extern      INT16S      g_iParameterEdit;
extern		BOOL		g_bInsertNum;
extern		BOOL		g_bClearNum;


//#define		PARACOUNT	 	13

#define		PARAMAX			20									//最大的参数个数

#define     PARAS_COL    	13									//每列参数个数
#define		PARAOBJECT		(PARAS_COL+2)						//编辑框按钮对象个数


#define		PARAWOFFSET		140									//水平偏移值	
#define		PARAHOFFSET		32									//垂直偏移值


static 		INT32S 		s_iParaCount=0;							//参数的数量
static 		INT8S		s_ParaName[PARAMAX][0x30];				//参数说明
static 		INT8S		s_ParaInfo[PARAMAX][0x50];				//参数说明

static 		void 		*p_Para[PARAMAX];						//指向数据的指针
static  	INT8U		ParaType[PARAMAX];						//数据类型
static 		INT8U		ParaDot[PARAMAX];						//小数点位数
static 		INT8U		ParaLong[PARAMAX];						//参数可设定的长度位数
static 		INT8U		ParaPas[PARAMAX];						//参数可设定的长度位数

static		INT16S		s_ParaStartLine=0;						//指示显示起始行
static		INT16S		s_ParaCurLine=0;						//指示显示当前行


void	CParaSet_Init(INT16S left,INT16S top,INT16S width,INT16S height)
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
	s_iParaCount = 0;
	
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	
	CMainFrm_Init();
	CMainFrm_SetArraySize( PARAOBJECT );
	
	i=0;
	k=0;
	j=g_iDlgTop+50-PARAHOFFSET;
	
	temp.style=CTROL_STYLE_EDIT;
	strcpy(temp.name,"");
	
	temp.x=g_iDlgLeft+400;
	temp.w=110;
	temp.h=26;
	temp.key=0;
	temp.algin=LEFT_ALGIN;

	for(m=0;m<PARAS_COL;m++)
	{
		temp.ID=0x9000+k++;
		temp.y=(j+=PARAHOFFSET);
		CMainFrm_Add(i++,&temp);
	}


	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Ok":"保存");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-130;
	temp.y=g_iDlgTop+g_iDlgHeight-50;
	temp.w=100;
	temp.h=36;
	temp.key=TSAVE;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"取消");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+10;
	temp.y=g_iDlgTop+g_iDlgHeight-50;
	temp.w=100;
	temp.h=36;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);
}


void	CParaSet_Destroy()
{
	CMainFrm_Destroy();
}


void   CParaSet_Create()
{
//	INT16S  j=g_iDlgTop+50-PARAHOFFSET;
	CTROL   *ptr;
	INT16S 	i = 0;
	INT8S ParaName[0x50];
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"Work Parameter Settings":"工作参数设置");

	{
		strcpy(ParaName,g_bEnglish?"ScrProtect(0-No,1-Yes)":"是否屏保 (0:不屏保,1:屏保)");
 		CParaSet_PutPara(ParaName," "	,&g_Ini.iScrProtect			,PARAINT,0,1,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"ProtectTime(Min)":"屏保时间 (分钟)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iScrProtectTime		,PARAINT,0,3,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"BmpProtect(0-No,1-Yes)":"图片屏保 (0:否,1:是)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iScrProtectBmp		,PARAINT,0,1,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"DispRunTime(0-No,1-Yes)":"显示剩余时间 (0:否,1:是)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iDispLeavingTime	,PARAINT,0,1,0);
	 	
 		strcpy(ParaName,g_bEnglish?"DeliverDrv(0-No,1-Yes,)":"线架驱动 (0:不驱动,1:驱动)");
 		CParaSet_PutPara(ParaName," "	,&g_Ini.iXianjiaDrive		,PARAINT,0,1,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"WarningOut(0-No,1-Yes)":"报警输出 (0无,1输出,2三色灯)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iWarningOut			,PARAINT,0,1,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"ExceedCheck(0-No,1-Yes)":"跑线检测 (0:不检测,1:检测)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iPaoXian			,PARAINT,0,1,0);
	 	
	// 	strcpy(ParaName,g_bEnglish?"EnableBackY(1-Yes,0-No)":"Y轴倒线 (1:可倒,0:不可倒)");
	//	CParaSet_PutPara(ParaName," "	,&g_Ini.iYDaoXian			,PARAINT,0,1,0);
	 	strcpy(ParaName,g_bEnglish?"SafeDoor(0-No,1-Yes)":"安全门检测 (0:不使用,1:使用)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iSafeDoor			,PARAINT,0,1,0);
	 	
	 	strcpy(ParaName,g_bEnglish?"Blocking line(0-No,>0 Yes)":"堵线计数 (0:不使用,>0 使用)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.iDuXianNum			,PARALONG,0,6,0);


	 	strcpy(ParaName,g_bEnglish?"XZeroOffset(°)":"X轴零点偏移(单位:度)");
	 	CParaSet_PutPara(ParaName," "	,&g_Ini.fStartOffset[0],PARAFLOAT,3,6,0); 	
	 	
	 	
	 	strcpy(ParaName,g_bEnglish?(g_Ini.iAxisRunMode[1]==2?"YZeroOffset(mm)":"YZeroOffset(°)")
	 		:(g_Ini.iAxisRunMode[1]==2?"Y轴零点偏移(单位:毫米)":"Y轴零点偏移(单位:度)"));
	 	CParaSet_PutPara(ParaName," ",&g_Ini.fStartOffset[1],PARAFLOAT,3,6,0);
	 	
	 	strcpy(ParaName,g_bEnglish?(g_Ini.iAxisRunMode[2]==2?"ZZeroOffset(mm)":"ZZeroOffset(°)")
	 		:(g_Ini.iAxisRunMode[2]==2?"Z轴零点偏移(单位:毫米)":"Z轴零点偏移(单位:度)"));
	 	CParaSet_PutPara(ParaName," ",&g_Ini.fStartOffset[2],PARAFLOAT,3,6,0);
		
		strcpy(ParaName,g_bEnglish?(g_Ini.iAxisRunMode[3]==2?"AZeroOffset(mm)":"AZeroOffset(°)")
			:(g_Ini.iAxisRunMode[3]==2?"A轴零点偏移(单位:毫米)":"A轴零点偏移(单位:度)"));
		CParaSet_PutPara(ParaName," ",&g_Ini.fStartOffset[3],PARAFLOAT,3,6,0);
		
		/*
		strcpy(ParaName,g_bEnglish?(g_Ini.iAxisRunMode[4]==2?"BZeroOffset(mm)":"BZeroOffset(°)")
	 		:(g_Ini.iAxisRunMode[4]==2?"B轴零点偏移(单位:毫米)":"B轴零点偏移(单位:度)"));
	 	CParaSet_PutPara(ParaName," ",&g_Ini.fStartOffset[4],PARAFLOAT,3,6,0);
		
		strcpy(ParaName,g_bEnglish?(g_Ini.iAxisRunMode[5]==2?"CZeroOffset(mm)":"CZeroOffset(°)")
			:(g_Ini.iAxisRunMode[5]==2?"C轴零点偏移(单位:毫米)":"C轴零点偏移(单位:度)"));
		CParaSet_PutPara(ParaName," ",&g_Ini.fStartOffset[5],PARAFLOAT,3,6,0);
		*/
		
	}

	CMainFrm_ShowAllControl();

	for(i=PARAS_COL;i<PARAOBJECT;i++)
	{
		ptr=CMainFrm_GetItemID(0x9000+i);
		CButton_SetCtrl(ptr);
		CButton_Create();
	}
	
	CParaSet_UpdateView(s_ParaStartLine);
}



INT32S 	CParaSet_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* SysPara,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD)
{
	if(s_iParaCount>=PARAMAX)
		return FALSE;
	
	sprintf(s_ParaName[s_iParaCount],"%03d, ",s_iParaCount+1);
	strcat(s_ParaName[s_iParaCount],ParaName);
	
	sprintf(s_ParaInfo[s_iParaCount],"%03d, ",s_iParaCount+1);
	strcat(s_ParaInfo[s_iParaCount],ParaInfo);
		
	p_Para[s_iParaCount] 	= SysPara;
	ParaType[s_iParaCount]	= Type;
	ParaDot[s_iParaCount] 	= Dot;
	ParaLong[s_iParaCount]	= DataLong;
	ParaPas[s_iParaCount] 	= uPWD;
	s_iParaCount++;
	
	return TRUE;
}


INT32S  CParaSet_UpdateView(INT16S iStartLine)
{
	INT16S j=0;
	CTROL   *ptr;

	for(j=0;j<PARAS_COL;j++)
	{

		if(iStartLine+j>= s_iParaCount)
		{	
			CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),368+120,28,DARKGRAYCOLOR,1);
			continue;
		}

		ptr=CMainFrm_GetItemID(0x9000+j);
		CEdit_SetCtrl(ptr);
		
		if(j==s_ParaCurLine)
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(LIGHTBLUECOLOR);
			CEdit_SetFirst(TRUE);
			CEdit_SetLen(ParaLong[iStartLine+j]);
			
			CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),368,26,LIGHTBLUECOLOR,1);
			CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),s_ParaName[iStartLine+j],BLACKCOLOR,LIGHTBLUECOLOR,KZK_22);
		//	CSysPara_Message(s_ParaInfo[iStartLine+j]);
		}
		else
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(WHITECOLOR);
			
		//	CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),368,26,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,1);
		//	CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),s_ParaName[iStartLine+j],SKYBLUECOLOR,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);

			CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),368,26,LIGHTGRAYCOLOR,1);
			CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(j%PARAS_COL),s_ParaName[iStartLine+j],SKYBLUECOLOR,LIGHTGRAYCOLOR,KZK_22);
		}
		
		switch(ParaType[iStartLine+j])
		{
			case PARAINT:
				CEdit_SetInt(*((INT16S*)p_Para[iStartLine+j]));
				break;
				
			case PARALONG:
				CEdit_SetLong(*((INT32S*)p_Para[iStartLine+j]));
				break;
				
			case PARAFLOAT:
				CEdit_SetFloat(*((FP32*)p_Para[iStartLine+j]),ParaDot[iStartLine+j]);
				break;
				
			case PARA8U:
				CEdit_SetInt(*((INT8U*)p_Para[iStartLine+j]));
				break;
				
			default:
				break;
		}
		
		{
			CRect rect;

			CEdit_GetSize(&rect.left, &rect.top, &rect.right,&rect.bottom);
			CDC_DrawEdit_2(rect,WHITECOLOR);
		}
		
		CEdit_SetNeedPassword(ParaPas[iStartLine+j]);

		CEdit_UpdateData(TRUE);
	}
	
	switch(ParaType[iStartLine+s_ParaCurLine])
	{
		case PARAINT:
			CEdit_SetInt(*((INT16S*)p_Para[iStartLine+s_ParaCurLine]));
			break;
			
		case PARALONG:
			CEdit_SetLong(*((INT32S*)p_Para[iStartLine+s_ParaCurLine]));
			break;
			
		case PARAFLOAT:
			CEdit_SetFloat(*((FP32*)p_Para[iStartLine+s_ParaCurLine]),ParaDot[iStartLine+s_ParaCurLine]);
			break;
			
		case PARA8U:
			CEdit_SetInt(*((INT8U*)p_Para[iStartLine+s_ParaCurLine]));
			break;
			
		default:
			break;
	}
	
	CEdit_SetNeedPassword(ParaPas[iStartLine+s_ParaCurLine]);

	return TRUE;
}



void	CParaSet_UpdateObject(INT16S dk,BOOL IsEnter)
{
	CTROL *ptr;
	
	ptr=CMainFrm_GetItemID(0x9000+s_ParaCurLine);
	
	if(s_ParaCurLine < PARAS_COL) 
	{	
		CEdit_SetCtrl(ptr);
	
		switch(ParaType[s_ParaStartLine+s_ParaCurLine])
		{
			case PARAINT:
				if(IsEnter)
				{
					INT16S iTemp = *((INT16S*)p_Para[s_ParaStartLine+s_ParaCurLine]);
					
					*((INT16S*)p_Para[s_ParaStartLine+s_ParaCurLine])=CEdit_GetInt(); 
					
					if(!SysParaCheck())	
					{
						*((INT16S*)p_Para[s_ParaStartLine+s_ParaCurLine]) = iTemp;
						return;
					}
				}
						
				CEdit_SetInt(*((INT16S*)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
				
			case PARALONG:
				if(IsEnter)
				{
					INT32S lTemp = *((INT32S*)p_Para[s_ParaStartLine+s_ParaCurLine]);
					
					*((INT32S*)p_Para[s_ParaStartLine+s_ParaCurLine])=CEdit_GetLong(); 	
			
					if(!SysParaCheck())	
					{
						*((INT32S*)p_Para[s_ParaStartLine+s_ParaCurLine]) = lTemp;
						return;
					}
				}
			
				CEdit_SetLong(*((INT32S*)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
				
			case PARAFLOAT:
				if(IsEnter)
				{
					FP32 fTemp = *((FP32*)p_Para[s_ParaStartLine+s_ParaCurLine]);
					
					*((FP32*)p_Para[s_ParaStartLine+s_ParaCurLine])=CEdit_GetFloat(); 	
					
					if(!SysParaCheck())	
					{
						*((FP32*)p_Para[s_ParaStartLine+s_ParaCurLine]) = fTemp;
						return;
					}
				
				}
				CEdit_SetFloat(*((FP32*)p_Para[s_ParaStartLine+s_ParaCurLine]),ParaDot[s_ParaStartLine+s_ParaCurLine]);
				break;
			
			case PARA8U:
				if(IsEnter)
				{
					INT8U uTemp = *((INT8U*)p_Para[s_ParaStartLine+s_ParaCurLine]);
				
					*((INT8U*)p_Para[s_ParaStartLine+s_ParaCurLine])=CEdit_GetInt(); 	
				
					if(!SysParaCheck())	
					{
						*((INT8U*)p_Para[s_ParaStartLine+s_ParaCurLine]) = uTemp;
						return;
					}
				}
			
				CEdit_SetInt(*((INT8U*)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
			
			default:
				break;
		}
	}
	
//	else
//	{
//		CButton_SetCtrl(ptr);
//		CButton_DrawItem(Normal);
//	}
	
	if(s_ParaStartLine+s_ParaCurLine+dk> s_iParaCount-1)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_DrawActive();
		return;
	}
		
	if(s_ParaStartLine+s_ParaCurLine+dk< 0)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_DrawActive();
		return;
	}
	
//	CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),368,26,s_ParaCurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,1);
//	CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),s_ParaName[s_ParaStartLine+s_ParaCurLine],SKYBLUECOLOR,s_ParaCurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);

	CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),368,26,LIGHTGRAYCOLOR,1);
	CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),s_ParaName[s_ParaStartLine+s_ParaCurLine],SKYBLUECOLOR,LIGHTGRAYCOLOR,KZK_22);
		

	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	CEdit_SetNeedPassword(ParaPas[s_ParaStartLine+s_ParaCurLine]);
	CEdit_UpdateData(FALSE);
	
	s_ParaCurLine+=dk;

	if(s_ParaCurLine >= PARAS_COL)
	{
		s_ParaStartLine += PARAS_COL;
		s_ParaCurLine=0;
	
		CParaSet_UpdateView(s_ParaStartLine);
		
		return;
	}	
		
	if(s_ParaCurLine<0 )
	{
		s_ParaStartLine -= PARAS_COL;
	
		if(s_ParaStartLine<0)
			s_ParaStartLine =0;
		
		s_ParaCurLine=PARAS_COL-1;

		CParaSet_UpdateView(s_ParaStartLine);
	
		return;
	}
		
		
	ptr = CMainFrm_GetItemID(0x9000+s_ParaCurLine);

	if(s_ParaCurLine<PARAS_COL)
	{		
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
		
		switch(ParaType[s_ParaStartLine+s_ParaCurLine])
		{
			case PARAINT:
				CEdit_SetInt(*((INT16S*)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
				
			case PARALONG:
				CEdit_SetLong(*((INT32S*)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
				
			case PARAFLOAT:
				CEdit_SetFloat(*((FP32*)p_Para[s_ParaStartLine+s_ParaCurLine]),ParaDot[s_ParaStartLine+s_ParaCurLine]);
				break;
				
			case PARA8U:
				CEdit_SetInt(*((INT8U *)p_Para[s_ParaStartLine+s_ParaCurLine]));
				break;
				
			default:
				break;
		}
		
		CEdit_SetLen(ParaLong[s_ParaStartLine+s_ParaCurLine]);
		CEdit_SetNeedPassword(ParaPas[s_ParaStartLine+s_ParaCurLine]);

		CDC_PutColorBar(g_iDlgLeft+30+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),368,26,LIGHTBLUECOLOR,1);
		CDC_OutText_UTF(g_iDlgLeft+32+ PARAWOFFSET*(s_ParaCurLine/PARAS_COL) ,g_iDlgTop+50+PARAHOFFSET*(s_ParaCurLine%PARAS_COL),s_ParaName[s_ParaStartLine+s_ParaCurLine],BLACKCOLOR,LIGHTBLUECOLOR,KZK_22);
		
	//	CSysPara_Message(s_SysParaInfo[s_StartLine+s_CurLine]);
		
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_DrawActive();
	}
//	else
//	{
//		CButton_SetCtrl(ptr);
//		CButton_DrawItem(Foucs);
//	}
}


void	CParaSet_OnButtonOk()
{
	CEdit_SetNeedPassword(FALSE);
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	Ok_Exit=TRUE;
	CEdit_SetTextColor(g_nBk);
	CEdit_SetBackColor(g_TextColor1);
	g_iDlgIDOk=1;
}


void	CParaSet_OnButtonCancel()
{
	CEdit_SetNeedPassword(FALSE);
	SysParaRead();

   	Ok_Exit=TRUE;
   	CEdit_SetTextColor(g_nBk);
   	CEdit_SetBackColor(g_TextColor1);
   	g_iDlgIDOk=0;
}


void	CParaSet_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9000+s_ParaCurLine);
	
	CEdit_SetCtrl(ptr);
	
	
	g_bInsertNum = FALSE;
	g_bClearNum = FALSE;
	
	switch(key)
	{
		case TUP:
		case TLEFT:
			CParaSet_UpdateObject(-1,FALSE);
			break;
				
		case TDOWN:
		case TRIGHT:
			CParaSet_UpdateObject(1,FALSE);
			break;
			
		case TSAVE:
			CParaSet_OnButtonOk();
			break;

		case TCANCEL:
			CParaSet_OnButtonCancel();
			break;
			
		case TENTER:
		
		//	if(g_iDlgCurObject != PARACOUNT-2 && g_iDlgCurObject != PARACOUNT-1)
			{
				CParaSet_UpdateObject(1,TRUE);
			}
		/*	else
			{
				if(g_iDlgCurObject==PARACOUNT-2)
					CParaSet_OnButtonOk();
					
				if(g_iDlgCurObject==PARACOUNT-1)
					CParaSet_OnButtonCancel();
			}*/
			break;
			
		case TZERO:case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':	
		case '8':case '9':case '.':case TMINUS:case TCLEAR:
		
		//	if(s_CurLine<s_iParaSetCount-2)
			{
				g_bInsertNum = TRUE;
				CEdit_OnKey(key,tkey);
			}
			break;
			
	/*		
		case TBACKSPACE:	
				CEdit_OnKey(key,tkey);
			break;
			*/
			
		#ifdef	PRINTBMP															//屏幕截图 归零键
		case TDELALL:
			{
				char   filename[0x20];
				INT16S i=0;
				
//				CBmp_Init();
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
				CreatBMP(filename,800,600,1);
//				CBmp_Destroy();
				OSQPost(MsgQueue,"Write Bmp End!");
			}
			break;
		#endif

		default:
			break;
	}
}


INT16S    CParaSet_DoModal()
{
	INT16U key=0,tkey=0;
	
	g_iParameterEdit = 1;
	CParaSet_Create();

	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
		//	Uart_Printf("key = %x\n",key);
			CParaSet_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		
		/*
		if(RealIO(STOPIO,0,2))
		{
			CParaSet_OnButtonCancel();
			break;
		}*/
		
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	g_iParameterEdit = 0;
	
	return g_iDlgIDOk;
}
