#include	"includes.h"

extern		BASEINI 	g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	BOOL		g_bEnglish;
extern		INT16S		g_iCurEditNo;
extern		BOOL		g_bInsertNum;		





#define		PARAMAX			40								//最大的参数个数

#define     PARAS_COL    	11								//每列参数个数
#define		PARAOBJECT		(PARAS_COL*2+7)					//编辑框按钮对象个数

#define		PARAHOFFSET		32								//垂直偏移值
#define		PARAWOFFSET		399								//水平偏移值

static 		INT32S 		s_iParaSetCount=0;					//参数的数量
static 		INT8S		s_ParaSetName[PARAMAX][0x20];		//参数说明
static 		INT8S		s_ParaSetInfo[PARAMAX][0x50];		//参数说明

static 		void 		*p_ParaSet[PARAMAX];				//指向数据的指针
static  	INT8U		ParaSetType[PARAMAX];				//数据类型
static 		INT8U		ParaSetDot[PARAMAX];				//小数点位数
static 		INT8U		ParaSetLong[PARAMAX];				//参数可设定的长度位数
static 		INT8U		ParaSetPas[PARAMAX];				//参数可设定的长度位数

static		INT16S		s_StartLine=0;						//指示显示起始行
static		INT16S		s_CurLine=0;						//指示显示当前行
static 		BOOL		s_bFunction1 = FALSE;				//气缸状态
//#define     PRINTBMP

void	CParaSet_Init(INT16S left,INT16S top,INT16S width,INT16S height)
{
	INT16S i,j,k,m;
	CTROL temp;
	
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
//	s_CurLine=0;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	s_iParaSetCount =0;
	
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	
	CMainFrm_Init();
	CMainFrm_SetArraySize(PARAOBJECT);
	
	i=0;
	k=0;
	j=g_iDlgTop+40-PARAHOFFSET;
	
	temp.style=CTROL_STYLE_EDIT;
	strcpy(temp.name,"");
	
	temp.x=g_iDlgLeft+278;
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
	
	
	j=g_iDlgTop+40-PARAHOFFSET;
	temp.x=g_iDlgLeft+278+PARAWOFFSET;
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
	strcpy(temp.name,g_bEnglish?"":"");
	temp.x=g_iDlgLeft+3;
	temp.y=g_iDlgTop+g_iDlgHeight-42;
	temp.w=113;
	temp.h=40;
	temp.key=NULL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"":"");
	temp.x=g_iDlgLeft+3+temp.w*1;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?" ":" ");
	temp.x=g_iDlgLeft+3+temp.w*2;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?" ":" ");
	temp.x=g_iDlgLeft+3+temp.w*3;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?" ":" ");
	temp.x=g_iDlgLeft+3+temp.w*4;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Save":"存 储");
	temp.x=g_iDlgLeft+3+temp.w*5;
	temp.key=TSAVE;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"取 消");
	temp.x=g_iDlgLeft+3+temp.w*6;
	temp.w=114;

	temp.key=TCANCEL;
	CMainFrm_Add(i++,&temp);
}


void	CParaSet_Destroy()
{
	CMainFrm_Destroy();
	s_bFunction1 =FALSE;	
//	LedOut(CYLINDERLED,s_bFunction1?1:0);
}


void   CParaSet_Create()
{
//	INT16S	j=g_iDlgTop+45-PARAHOFFSET;
	CTROL   *ptr;
	INT16S 	i = 0;
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"System parameter":"工作参数设定");
	CParaSet_Message(NULL);
	if(g_bEnglish)
	{
		CParaSet_PutPara("Screen Pro","Screen Protect.1:Yes,0:No",			&g_Ini.iScrProtect			,PARAINT,0,1,0);
	 	CParaSet_PutPara("ScnPro Time","AutoRun mode ScreenPro Time.min,Range 1~100",&g_Ini.iScrProtectTime,PARAINT,0,3,0);
	 	CParaSet_PutPara("Pic Protect","Picture Protect mode.1:Yes,0:No",	    &g_Ini.iScrProtectBmp		,PARAINT,0,1,0);
		CParaSet_PutPara("LineRacks","LineRacks Driving.1:Yes,0:No",		&g_Ini.iXianjiaDrive		,PARAINT,0,1,0);
	 	CParaSet_PutPara("Warning Out","Warning Output.1:Yes,0:No"	,		&g_Ini.iWarningOut			,PARAINT,0,1,0);
	 	CParaSet_PutPara("PaoXianCheck","PaoXian Checking.1:Yes,0:No"	,			&g_Ini.iPaoXian				,PARAINT,0,1,0);
	 	CParaSet_PutPara("AxisY Rev","AxisY Reverse.1:Yes,0:No",				&g_Ini.iYDaoXian			,PARAINT,0,1,0);
	 	CParaSet_PutPara("IO SINGLE","0.disable,1.enable.COM:PROBE6"	,	&g_Ini.iSwhSingleIo				,PARAINT,0,1,0);
	 	CParaSet_PutPara("IO RUN","0.disable,1.enable.COM:PROBE5"	,			&g_Ini.iSwhRunIo			,PARAINT,0,1,0);
	 	CParaSet_PutPara("XStartOffset","StartOffset.Deg.Range -360~360", &g_Ini.fStartOffset[0]		,PARAFLOAT,1,6,0);
	 //	CParaSet_PutPara("Y轴零点偏移","单位:度,范围(-360~360)", &g_Ini.fStartOffset[1]		,PARAFLOAT,1,6,0);
	 	CParaSet_PutPara("ZStartOffset","StartOffset.Deg/mm.Range -1000~1000", &g_Ini.fStartOffset[2]		,PARAFLOAT,1,6,0);
	 	CParaSet_PutPara("AStartOffset","StartOffset.Deg/mm.Range -1000~1000", &g_Ini.fStartOffset[3]		,PARAFLOAT,1,6,0);
		
		//CParaSet_PutPara("SafeDoor","Safe Door Checking.0:No 1:Yes,default:0",&g_Ini.iSafeDoor		    ,PARAINT,0,1,0);
		//CParaSet_PutPara("TStop Mode","TStop Button Mode.0:Stop 1:Single,default:1", &g_Ini.iStopMode		,PARA8U,0,1,0);
		CParaSet_PutPara("Batch","0:Invalid;Not 0:Batch count,default:0", &g_Ini.iPerCount,PARALONG,0,6,0);
		CParaSet_PutPara("ProbeMode","0:Invalid 1:Valid,default:1", &g_Ini.iProbeMode		,PARAINT,0,1,0);	
	}
	else
	{
 		CParaSet_PutPara("是否屏保","1屏保,0不屏保",			&g_Ini.iScrProtect			,PARAINT,0,1,0);
	 	CParaSet_PutPara("屏保时间","自动加工模式进入屏保时间,单位:分钟,范围(1-100)",&g_Ini.iScrProtectTime,PARAINT,0,3,0);
	 	CParaSet_PutPara("图片屏保","1图片屏保,0非图片屏保",	&g_Ini.iScrProtectBmp		,PARAINT,0,1,0);
 		CParaSet_PutPara("送线架驱动","1驱动,0不驱动",			&g_Ini.iXianjiaDrive		,PARAINT,0,1,0);
	 	CParaSet_PutPara("报警输出","1输出,0不输出"	,			&g_Ini.iWarningOut			,PARAINT,0,1,0);
	 	CParaSet_PutPara("跑线检测","1检测,0不检测"	,			&g_Ini.iPaoXian				,PARAINT,0,1,0);
	 	CParaSet_PutPara("Y轴倒线","1可倒,0不可倒",				&g_Ini.iYDaoXian			,PARAINT,0,1,0);
	 	CParaSet_PutPara("IO单条使能","0关闭,1启用,2,加工中等待探针6.端口:探针6"	,	&g_Ini.iSwhSingleIo	,PARAINT,0,1,0);
	 	CParaSet_PutPara("IO启动使能","0关闭,1启用.端口:探针5"	,			&g_Ini.iSwhRunIo		,PARAINT,0,1,0);
	 	
	 	CParaSet_PutPara("X轴零点偏移","单位:度,范围-360.0~360.0", &g_Ini.fStartOffset[0]		,PARAFLOAT,1,6,0);
	 //	CParaSet_PutPara("Y轴零点偏移","单位:度,范围-360.0~360.0", &g_Ini.fStartOffset[1]		,PARAFLOAT,1,6,0);
	 	CParaSet_PutPara("Z轴零点偏移","单位:丝杆mm,旋转 度,范围-1000.0~1000.0", &g_Ini.fStartOffset[2]	,PARAFLOAT,1,6,0);
	 	CParaSet_PutPara("A轴零点偏移","单位:丝杆mm,旋转 度,范围-1000.0~1000.0", &g_Ini.fStartOffset[3]	,PARAFLOAT,1,6,0);	
		//CParaSet_PutPara("安全门检测","0:不检测 1:检测,默认值:0",&g_Ini.iSafeDoor		    ,PARAINT,0,1,0);
		//CParaSet_PutPara("停止键选择","0:停止功能 1:单条功能,默认值:1", &g_Ini.iStopMode		,PARA8U,0,1,0);
		CParaSet_PutPara("每批产量","0:功能无效 非0:目标产值,达到后停机提示,默认值:0", &g_Ini.iPerCount,PARALONG,0,6,0);
		CParaSet_PutPara("探针模式","0:无伺服清零输出 1:有伺服清零输出,默认值:1", &g_Ini.iProbeMode	,PARAINT,0,1,0);
		CParaSet_PutPara("送线编辑清零","送线列按ENT是否清零 0:不清零 1:清零,默认值:0", &g_Ini.iSendClear	,PARALONG,0,1,0);

		
	}
	
	CMainFrm_ShowAllControl();
	
	for(i=PARAS_COL*2;i<PARAOBJECT;i++)
	{
		ptr=CMainFrm_GetItemID(0x9000+i);
		CButton_SetCtrl(ptr);
		CButton_Create();
	}

	CParaSet_DrawButtonInfo(0,0);

	CParaSet_UpdateView(s_StartLine);

}


void CParaSet_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus)
{

	INT16S i=0;
	INT16S x=0;
	INT16S y=0;
	INT8S buf[0x10];
	
	x = g_iDlgLeft+10;
	y = g_iDlgTop+g_iDlgHeight-40;
	
 	if(iCliyer==0)
	{
		CDC_OutText24(x-5,y,"*",YELLOWCOLOR,DARKBLUECOLOR);
	}
	else if(iCliyer==1)
	{
		CDC_OutText24(x-5,y,"*",REDCOLOR,DARKBLUECOLOR);
	}
	
	if(NumStatus ==0)
	{
		for(i=0;i<6;i++)
		{		
			sprintf(buf,"%d",i+1);
			CDC_OutText(x+90+i*113,y,buf,YELLOWCOLOR,DARKBLUECOLOR);
		}
	}
	else if(NumStatus>0 && NumStatus<7)
	{
		sprintf(buf,"%d",NumStatus);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,REDCOLOR,DARKBLUECOLOR);
		OSTimeDly(100);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,YELLOWCOLOR,DARKBLUECOLOR);
	}
}


void	CParaSet_Message(INT8S  *ParaInfo)
{
	CRect rect;
	
	rect.left	= g_iDlgLeft+10;
	rect.top	= g_iDlgTop+g_iDlgHeight-80;
	rect.right	= g_iDlgLeft+10+780;
	rect.bottom = g_iDlgTop+g_iDlgHeight-80+30;
	
	if(ParaInfo==NULL)
	{
		INT16S x1 = rect.left-1;
		INT16S x2 = rect.right+1;
		INT16S y1 = rect.top-1;
		INT16S y2 = rect.bottom+1;

		INT16S x=x1,y=y1,xx=x2,yy=y2;

		CDC_Line(x,y,x,yy,BLACKCOLOR);
		CDC_Line(x,y,xx,y,BLACKCOLOR);

		CDC_Line(x,yy,xx,yy,WHITECOLOR);
		CDC_Line(xx,y,xx,yy,WHITECOLOR);

		CDC_Line(x+1,yy-1,xx-1,yy-1,WHITECOLOR);
		CDC_Line(xx-1,y+1,xx-1,yy-1,WHITECOLOR);
	}
	else
	{
		CDC_PutColorBar(rect.left,rect.top,780,30,GRAYCOLOR,1);
		//CDC_OutText_UTF(rect.left+3,rect.top+2,ParaInfo,BLACKCOLOR,GRAYCOLOR,MZK_24);
		CDC_OutText24(rect.left+3,rect.top+2,ParaInfo,BLACKCOLOR,-1);	
	}
}



INT32S 	CParaSet_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* ParaSet,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD)
{
	if(s_iParaSetCount>=PARAMAX)
		return FALSE;
	
	sprintf(s_ParaSetName[s_iParaSetCount],"%03d,",s_iParaSetCount+1);
	strcat(s_ParaSetName[s_iParaSetCount],ParaName);
	
	sprintf(s_ParaSetInfo[s_iParaSetCount],"%03d,",s_iParaSetCount+1);
	strcat(s_ParaSetInfo[s_iParaSetCount],ParaInfo);
		
	p_ParaSet[s_iParaSetCount] 		= ParaSet;
	ParaSetType[s_iParaSetCount]	= Type;
	ParaSetDot[s_iParaSetCount] 	= Dot;
	ParaSetLong[s_iParaSetCount]	= DataLong;
	ParaSetPas[s_iParaSetCount]		= uPWD;
	s_iParaSetCount++;
	
	return TRUE;
}


INT32S  CParaSet_UpdateView(INT16S iStartLine)
{
	INT16S j=0;
	CTROL   *ptr;

	for(j=0;j<PARAS_COL*2;j++)
	{
		
		if(iStartLine+j>= s_iParaSetCount)
		{	
			CDC_PutColorBar(g_iDlgLeft+8+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+40+PARAHOFFSET*(j%PARAS_COL),266+120,28,DARKBLUECOLOR,1);
			continue;
		}
		
		ptr=CMainFrm_GetItemID(0x9000+j);
		CEdit_SetCtrl(ptr);
		
		if(j==s_CurLine)
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(LIGHTBLUECOLOR);
			CEdit_SetFirst(TRUE);
			CEdit_SetLen(ParaSetLong[iStartLine+j]);
			
			CDC_PutColorBar(g_iDlgLeft+10+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),266,26,LIGHTBLUECOLOR,1);
			//CDC_OutText_UTF(g_iDlgLeft+12+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),s_ParaSetName[iStartLine+j],BLACKCOLOR,LIGHTBLUECOLOR,MZK_24);
			CDC_OutText24(g_iDlgLeft+12+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),s_ParaSetName[iStartLine+j],BLACKCOLOR,-1);
			CParaSet_Message(s_ParaSetInfo[iStartLine+j]);
		}
		else
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(WHITECOLOR);
			
			CDC_PutColorBar(g_iDlgLeft+10+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),266,26,LIGHTGRAYCOLOR,1);
			//CDC_OutText_UTF(g_iDlgLeft+12+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),s_ParaSetName[iStartLine+j],LIGHTBLUECOLOR,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);
			CDC_OutText24(g_iDlgLeft+12+ PARAWOFFSET*(j/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(j%PARAS_COL),s_ParaSetName[iStartLine+j],DARKBLUECOLOR,-1);
		}
		  
		switch(ParaSetType[iStartLine+j])
		{
			case PARAINT:
				CEdit_SetInt(*((INT16S*)p_ParaSet[iStartLine+j]));
				break;
				
			case PARALONG:
				CEdit_SetLong(*((INT32S*)p_ParaSet[iStartLine+j]));
				break;
				
			case PARAFLOAT:
				CEdit_SetFloat(*((FP32*)p_ParaSet[iStartLine+j]),ParaSetDot[iStartLine+j]);
				break;
				
			case PARA8U:
				CEdit_SetInt(*((INT8U*)p_ParaSet[iStartLine+j]));
				break;
				
			default:
				break;
		}
		
		{
			CRect rect;

			CEdit_GetSize(&rect.left, &rect.top, &rect.right,&rect.bottom);
			CDC_DrawEdit_2(rect,WHITECOLOR);
		}
		
		CEdit_SetNeedPassword(ParaSetPas[iStartLine+j]);

		CEdit_UpdateData(TRUE);
	}
	
	switch(ParaSetType[iStartLine+s_CurLine])
	{
		case PARAINT:
			CEdit_SetInt(*((INT16S*)p_ParaSet[iStartLine+s_CurLine]));
			break;
			
		case PARALONG:
			CEdit_SetLong(*((INT32S*)p_ParaSet[iStartLine+s_CurLine]));
			break;
			
		case PARAFLOAT:
			CEdit_SetFloat(*((FP32*)p_ParaSet[iStartLine+s_CurLine]),ParaSetDot[iStartLine+s_CurLine]);
			break;
			
		case PARA8U:
			CEdit_SetInt(*((INT8U*)p_ParaSet[iStartLine+s_CurLine]));
			break;
			
		default:
			break;
	}
	
	CEdit_SetNeedPassword(ParaSetPas[iStartLine+s_CurLine]);	
	CEdit_SetTextColor(BLACKCOLOR);										//该函数调用完后，背景颜色与字体颜色均为最后一个编辑框对应的颜色，需要重新置为焦点状态的颜色
	CEdit_SetBackColor(LIGHTBLUECOLOR);
	return TRUE;
}



void	CParaSet_UpdateObject(INT16S dk,BOOL IsEnter)
{
	CTROL *ptr;
	
	ptr = CMainFrm_GetItemID(0x9000+s_CurLine);

	if(s_CurLine < PARAS_COL*2) 
	{	
		CEdit_SetCtrl(ptr);
	
		switch(ParaSetType[s_StartLine+s_CurLine])
		{
			case PARAINT:
				if(IsEnter)
				{
					INT16S iTemp = *((INT16S*)p_ParaSet[s_StartLine+s_CurLine]);
					
					*((INT16S*)p_ParaSet[s_StartLine+s_CurLine])=CEdit_GetInt(); 
					
					if(!SysParaCheck())	
					{
						*((INT16S*)p_ParaSet[s_StartLine+s_CurLine]) = iTemp;
						return;
					}
				}
						
				CEdit_SetInt(*((INT16S*)p_ParaSet[s_StartLine+s_CurLine]));
				break;
				
			case PARALONG:
				if(IsEnter)
				{
					INT32S lTemp = *((INT32S*)p_ParaSet[s_StartLine+s_CurLine]);
					
					*((INT32S*)p_ParaSet[s_StartLine+s_CurLine])=CEdit_GetLong(); 	
			
					if(!SysParaCheck())	
					{
						*((INT32S*)p_ParaSet[s_StartLine+s_CurLine]) = lTemp;
						return;
					}
				}
			
				CEdit_SetLong(*((INT32S*)p_ParaSet[s_StartLine+s_CurLine]));
				break;
				
			case PARAFLOAT:
				if(IsEnter)
				{
					FP32 fTemp = *((FP32*)p_ParaSet[s_StartLine+s_CurLine]);
					
					*((FP32*)p_ParaSet[s_StartLine+s_CurLine])=CEdit_GetFloat(); 	
					
					if(!SysParaCheck())	
					{
						*((FP32*)p_ParaSet[s_StartLine+s_CurLine]) = fTemp;
						return;
					}
				
				}
				CEdit_SetFloat(*((FP32*)p_ParaSet[s_StartLine+s_CurLine]),ParaSetDot[s_StartLine+s_CurLine]);
				break;
			
			case PARA8U:
				if(IsEnter)
				{
					INT8U uTemp = *((INT8U*)p_ParaSet[s_StartLine+s_CurLine]);
				
					*((INT8U*)p_ParaSet[s_StartLine+s_CurLine])=CEdit_GetInt(); 	
				
					if(!SysParaCheck())	
					{
						*((INT8U*)p_ParaSet[s_StartLine+s_CurLine]) = uTemp;
						return;
					}
				}
			
				CEdit_SetInt(*((INT8U*)p_ParaSet[s_StartLine+s_CurLine]));
				break;
			
			default:
				break;
		}
	}
/*	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Normal);
	}*/
	
	if(s_StartLine+s_CurLine+dk> s_iParaSetCount-1)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_UpdateData(FALSE);
		return;
	}
		
	if(s_StartLine+s_CurLine+dk< 0)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_UpdateData(FALSE);
		return;
	}
	
	CDC_PutColorBar(g_iDlgLeft+10+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),266,26,LIGHTGRAYCOLOR,1);
	//CDC_OutText_UTF(g_iDlgLeft+12+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),s_ParaSetName[s_StartLine+s_CurLine],LIGHTBLUECOLOR,s_CurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);
	CDC_OutText24(g_iDlgLeft+12+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),s_ParaSetName[s_StartLine+s_CurLine],DARKBLUECOLOR,-1);

	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	CEdit_SetNeedPassword(ParaSetPas[s_StartLine+s_CurLine]);
	CEdit_UpdateData(FALSE);
		
	s_CurLine+=dk;

	if(s_CurLine >= PARAS_COL*2)
	{
		s_StartLine += PARAS_COL*2;
	
		s_CurLine=0;
	
		CParaSet_UpdateView(s_StartLine);
		
		return;
	}	
		
	if(s_CurLine<0 )
	{
		s_StartLine -= PARAS_COL*2;
	
		if(s_StartLine<0)
			s_StartLine =0;
		
		s_CurLine=PARAS_COL*2-1;

		CParaSet_UpdateView(s_StartLine);
	
		return;
	}
	
	ptr = CMainFrm_GetItemID(0x9000+s_CurLine);
	
	if(s_CurLine<PARAS_COL*2)
	{		
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
		
		switch(ParaSetType[s_StartLine+s_CurLine])
		{
			case PARAINT:
				CEdit_SetInt(*((INT16S*)p_ParaSet[s_StartLine+s_CurLine]));
				break;
				
			case PARALONG:
				CEdit_SetLong(*((INT32S*)p_ParaSet[s_StartLine+s_CurLine]));
				break;
				
			case PARAFLOAT:
				CEdit_SetFloat(*((FP32*)p_ParaSet[s_StartLine+s_CurLine]),ParaSetDot[s_StartLine+s_CurLine]);
				break;
				
			case PARA8U:
				CEdit_SetInt(*((INT8U *)p_ParaSet[s_StartLine+s_CurLine]));
				break;
				
			default:
				break;
		}
		
		CEdit_SetLen(ParaSetLong[s_StartLine+s_CurLine]);
		CEdit_SetNeedPassword(ParaSetPas[s_StartLine+s_CurLine]);

		CDC_PutColorBar(g_iDlgLeft+10+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),266,26,LIGHTBLUECOLOR,1);
		//CDC_OutText_UTF(g_iDlgLeft+12+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),s_ParaSetName[s_StartLine+s_CurLine],BLACKCOLOR,LIGHTBLUECOLOR,MZK_24);
		CDC_OutText24(g_iDlgLeft+12+ PARAWOFFSET*(s_CurLine/PARAS_COL) ,g_iDlgTop+41+PARAHOFFSET*(s_CurLine%PARAS_COL),s_ParaSetName[s_StartLine+s_CurLine],BLACKCOLOR,-1);
	
		CParaSet_Message(s_ParaSetInfo[s_StartLine+s_CurLine]);
		
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_UpdateData(FALSE);
	}
/*	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Foucs);
	}*/
	
}


void	CParaSet_OnButtonOk()
{
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	
	//InitBoard();																//只需要执行部分有变动的，其他可保持不变
	{
		INT16S j=0;
		
		for(j=0; j<CHMAX; j++)
		{
			SetUnitPulse(j+1,g_Ini.fMM[j],g_Ini.lPulse[j]); 					//设定各轴脉冲当量		
			set_pulse_mode(j+1,1,0,g_Ini.iRunDir[j]);
		}
	}
	
	Ok_Exit=TRUE;
	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	g_iDlgIDOk=1;
}


void	CParaSet_OnButtonCancel()
{
   SysParaRead();
   Ok_Exit=TRUE;
   CEdit_SetNeedPassword(FALSE);
   CEdit_SetTextColor(BLACKCOLOR);
   CEdit_SetBackColor(WHITECOLOR);  
   g_iDlgIDOk=0;
}

void	CParaSet_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;
	
	ptr=CMainFrm_GetItemID(0x9000+s_CurLine);
	CEdit_SetCtrl(ptr);
	
	g_bInsertNum = FALSE;
	//g_bClearNum = FALSE;
	
	switch(key)
	{
		
		case TSET:
			s_bFunction1 =s_bFunction1?FALSE:TRUE;
		//	LedOut(CYLINDERLED,s_bFunction1?1:0);
			CParaSet_DrawButtonInfo(s_bFunction1,-1);	
			break;
		
		//case TSAVE:
		case TSCREEN:													//无存储按键，用TSCREEN代替
			{
				CParaSet_OnButtonOk();
			}
			break;
			
		case TCANCEL:
			CParaSet_OnButtonCancel();
			break;
		
		case TUP:
			CParaSet_UpdateObject(-1,FALSE);
			break;
			
		case TDOWN:
			CParaSet_UpdateObject(1,FALSE);
			break;		
			
		case TLEFT:
			{
				if(s_CurLine/PARAS_COL)
					CParaSet_UpdateObject(-PARAS_COL,FALSE);
			}
			break;
			
	    case TRIGHT:
	    	{	
				if(!(s_CurLine/PARAS_COL))
					CParaSet_UpdateObject(PARAS_COL,FALSE);
			}
			break;

		case TPAGEUP:
			{
				if((s_CurLine+s_StartLine)/(PARAS_COL*2))
				{
					s_StartLine -= PARAS_COL*2;
					s_CurLine=0;
					CSysPara_UpdateView(s_StartLine);
				}
			}
			break;

		case TPAGEDOWN:
			{
				if(((s_CurLine+s_StartLine)/(PARAS_COL*2))<((s_iParaSetCount+1)/(PARAS_COL*2)))
				{
					s_StartLine += PARAS_COL*2;
					s_CurLine=0;
					CSysPara_UpdateView(s_StartLine);
				}
			}
			break;	
			
		case TENTER:
			{	
				CParaSet_UpdateObject(1,TRUE);
				break;
			}
			
			
		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':	
		case '8':case '9':case '.':case '-':
			if(s_bFunction1)
			{
				s_bFunction1 =s_bFunction1?FALSE:TRUE;
//				LedOut(CYLINDERLED,s_bFunction?1:0);
				CSysPara_DrawButtonInfo(s_bFunction1,-1);
				
				switch(key)
				{
					case '6':
						CSysPara_DrawButtonInfo(-1,6);
						CSysPara_OnButtonOk();
						break;	
					default:
						break;
				}
				
				
				
				break;
			}
			g_bInsertNum = TRUE;
			
		case TCLEAR:
		//	if(s_CurLine<s_iParaSetCount-2)
			{
				
				CEdit_OnKey(key,tkey);
			}
			
			break;
		/*	
		case TBACKSPACE:
				CEdit_OnKey(key,tkey);
			break;
		*/
	#ifdef	PRINTBMP			
		case F5:case TDELALL:

			{
				char filename[0x20];
				INT16S i=0;
				
				strcpy(filename,"Workpara");
				
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
				
				//OSQPost(MsgQueue,"Write Bmp End!");
			}

			break;
	#endif	
			
		default:break;
	}
}


INT16S    CParaSet_DoModal()
{
	INT16U key,tkey=0;
	
	//g_iParameterEdit = 1;

	{
		CParaSet_Create();
	}
	
	Ok_Exit=FALSE;
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{

			CParaSet_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}

		if(RealIO(STOPIO,0,2))
		{
			CParaSet_OnButtonCancel();
			break;
		}

		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	//g_iParameterEdit = 0;
	
	return g_iDlgIDOk;
}


