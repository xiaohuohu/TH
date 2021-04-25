#include	"includes.h"

extern		BASEINI 	g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;
//TEST_EXT	INT16S		g_iDlgCurObject;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	BOOL		g_bEnglish;

/*
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TextColor7;
WORK_EXT	INT32S 		g_TableLine;

WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
 */

BOOL		g_bSysReadBackup = FALSE;							//主要防止导出备份参数，由于系统轴数的变化，而导致教导界面显示不正确

extern		INT16S		g_iCurEditNo;
//extern		INT16S		g_iParameterEdit;
extern		BOOL		g_bInsertNum;		
//extern		BOOL		g_bClearNum;




#define		SYSPARAMAX			120								//最大的参数个数

#define     SYSPARAS_COL    	11								//每列参数个数
#define		SYSOBJECT			(SYSPARAS_COL*2+7)				//编辑框按钮对象个数

#define		SYSHOFFSET			32								//垂直偏移值
#define		SYSWOFFSET			399								//水平偏移值

static 		INT32S 		s_iSysParaCount=0;						//参数的数量
static 		INT8S		s_SysParaName[SYSPARAMAX][0x20];		//参数说明
static 		INT8S		s_SysParaInfo[SYSPARAMAX][0x50];		//参数说明

static 		void 		*p_SysPara[SYSPARAMAX];					//指向数据的指针
static  	INT8U		SysParaType[SYSPARAMAX];				//数据类型
static 		INT8U		SysParaDot[SYSPARAMAX];					//小数点位数
static 		INT8U		SysParaLong[SYSPARAMAX];				//参数可设定的长度位数
static 		INT8U		SysParaPas[SYSPARAMAX];					//参数可设定的长度位数

static		INT16S		s_StartLine=0;							//指示显示起始行
static		INT16S		s_CurLine=0;							//指示显示当前行
static 		BOOL		s_bFunction = FALSE;					//这里作为是否按下"*"的标志量（新平台是表示是否按下"气缸"按键的标志量）
//#define     PRINTBMP

void	CSysPara_Init(INT16S left,INT16S top,INT16S width,INT16S height)
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
	s_iSysParaCount =0;

	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);

	CMainFrm_Init();
	CMainFrm_SetArraySize(SYSOBJECT);

	i=0;
	k=0;
	j=g_iDlgTop+41-SYSHOFFSET;

	temp.style=CTROL_STYLE_EDIT;
	strcpy(temp.name,"");

	temp.x=g_iDlgLeft+278;
	temp.w=110;
	temp.h=26;
	temp.key=0;
	temp.algin=LEFT_ALGIN;

	for(m=0;m<SYSPARAS_COL;m++)
	{
		temp.ID=0x9000+k++;
		temp.y=(j+=SYSHOFFSET);
		CMainFrm_Add(i++,&temp);
	}

	j=g_iDlgTop+41-SYSHOFFSET;
	temp.x=g_iDlgLeft+278+SYSWOFFSET;
	temp.w=110;
	temp.h=26;
	temp.key=0;
	temp.algin=LEFT_ALGIN;

	for(m=0;m<SYSPARAS_COL;m++)
	{
		temp.ID=0x9000+k++;
		temp.y=(j+=SYSHOFFSET);
		CMainFrm_Add(i++,&temp);
	}

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;

	strcpy(temp.name,g_bEnglish?"PrePage":"上 页");
	temp.x=g_iDlgLeft+3;
	temp.y=g_iDlgTop+g_iDlgHeight-42;
	temp.w=113;
	temp.h=40;
	temp.key=NULL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"NxtPage ":"下 页");
	temp.x=g_iDlgLeft+3+temp.w*1;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"BackUp ":"备 份");
	temp.x=g_iDlgLeft+3+temp.w*2;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"ReBack":"还 原");
	temp.x=g_iDlgLeft+3+temp.w*3;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Init":"初始化");
	temp.x=g_iDlgLeft+3+temp.w*4;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"取 消");
	temp.x=g_iDlgLeft+3+temp.w*5;
	temp.key=TSAVE;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Ok":"存 储");
	temp.x=g_iDlgLeft+3+temp.w*6;
	temp.w=114;

	temp.key=TCANCEL;
	CMainFrm_Add(i++,&temp);
}


void	CSysPara_Destroy()
{
	CMainFrm_Destroy();
	s_bFunction =FALSE;	
	//	LedOut(CYLINDERLED,s_bFunction?1:0);
}


void   CSysPara_Create()
{
	//	INT16S	j=g_iDlgTop+45-SYSHOFFSET;
	CTROL   *ptr;
	INT16S 	i = 0;

	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"System parameter":"系统参数设定");
	CSysPara_Message(NULL);

	if(g_bEnglish)
	{
		CSysPara_PutPara("X Pulse","X Axis:Pulse of a Circle. Range 200~40000",&g_Ini.lPulse[0],PARALONG,0,6,0);
		CSysPara_PutPara("Y Pulse","Y Axis:Pulse of a Circle. Range 200~40000",&g_Ini.lPulse[1],PARALONG,0,6,0);
		CSysPara_PutPara("Z Pulse","Z Axis:Pulse of a Circle. Range 200~40000",&g_Ini.lPulse[2],PARALONG,0,6,0);
		CSysPara_PutPara("A Pulse","A Axis:Pulse of a Circle. Range 200~40000",&g_Ini.lPulse[3],PARALONG,0,6,0);

		CSysPara_PutPara("X Angle","X Angle of a Circle.Range 0.5~2000.0",&g_Ini.fMM[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y Angle","Y Angle of a Circle.Range 0.5~2000.0",&g_Ini.fMM[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z Angle","Z Angle of a Circle.Range 0.5~2000.0",&g_Ini.fMM[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A Angle","A Angle of a Circle.Range 0.5~2000.0",&g_Ini.fMM[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X Dir","X Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iRunDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y Dir","Y Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iRunDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z Dir","Z Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iRunDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A Dir","A Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iRunDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X Back Dir","X Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iBackDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y Back Dir","Y Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iBackDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z Back Dir","Z Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iBackDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A Back Dir","A Dir logic.0:Foreward 1:backward,default 0",&g_Ini.iBackDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X Ratio","Reduction ratio.Range 0.1~100.0,default 1.0",&g_Ini.fGearRate[0],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Y Ratio","Reduction ratio.Range 0.1~100.0,default 1.0",&g_Ini.fGearRate[1],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Z Ratio","Reduction ratio.Range 0.1~100.0,default 1.0",&g_Ini.fGearRate[2],PARAFLOAT,1,4,0);
		CSysPara_PutPara("A Ratio","Reduction ratio.Range 0.1~100.0,default 1.0",&g_Ini.fGearRate[3],PARAFLOAT,1,4,0);

		CSysPara_PutPara("X Speed","Rated Spd.r/min.Range 100~8000,default 2000",&g_Ini.lMaxSpeed[0],PARALONG,0,4,0);
		CSysPara_PutPara("Y Speed","Rated Spd.r/min.Range 100~8000,default 2000",&g_Ini.lMaxSpeed[1],PARALONG,0,4,0);
		CSysPara_PutPara("Z Speed","Rated Spd.r/min.Range 100~8000,default 2000",&g_Ini.lMaxSpeed[2],PARALONG,0,4,0);
		CSysPara_PutPara("A Speed","Rated Spd.r/min.Range 100~8000,default 2000",&g_Ini.lMaxSpeed[3],PARALONG,0,4,0);

		CSysPara_PutPara("X Acc","Acc of X-axis.UNIT:KPP/s.Range 50~10000",&g_Ini.lChAddSpeed[0],PARALONG,0,5,0);
		CSysPara_PutPara("Y Acc","Acc of Y-axis.UNIT:KPP/s.Range 50~10000",&g_Ini.lChAddSpeed[1],PARALONG,0,5,0);
		CSysPara_PutPara("Z Acc","Acc of Z-axis.UNIT:KPP/s.Range 50~10000",&g_Ini.lChAddSpeed[2],PARALONG,0,5,0);
		CSysPara_PutPara("A Acc","Acc of A-axis.UNIT:KPP/s.Range 50~10000",&g_Ini.lChAddSpeed[3],PARALONG,0,5,0);

		CSysPara_PutPara("X Handspeed","HandSpd.Deg/sec.Range 0.1~1000.0,default60.0",&g_Ini.fHandSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y Handspeed","HandSpd.mm/sec.Range 0.1~1000.0,default60.0",&g_Ini.fHandSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z Handspeed","HandSpd.Deg/sec.Range 0.1~1000.0,default60.0",&g_Ini.fHandSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A Handspeed","HandSpd.Deg/sec.Range 0.1~1000.0,default60.0",&g_Ini.fHandSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X Backspeed","BackSpd.Deg/sec.Range 0.1~1000.0,default90.0",&g_Ini.fBackSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y Backspeed","BackSpd.Deg/sec.Range 0.1~1000.0,default90.0",&g_Ini.fBackSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z Backspeed","BackSpd.Deg/sec.Range 0.1~1000.0,default90.0",&g_Ini.fBackSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A Backspeed","BackSpd.Deg/sec.Range 0.1~1000.0,default90.0",&g_Ini.fBackSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X WarnLog","X SevWarning Logic.0:Open 1:Close,default 1",&g_Ini.iServoLogic[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y WarnLog","Y SevWarning Logic.0:Open 1:Close,default 1",&g_Ini.iServoLogic[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z WarnLog","Z SevWarning Logic.0:Open 1:Close,default 1",&g_Ini.iServoLogic[2],PARA8U,0,1,0);
		CSysPara_PutPara("A WarnLog","A SevWarning Logic.0:Open 1:Close,default 1",&g_Ini.iServoLogic[3],PARA8U,0,1,0);

		CSysPara_PutPara("X SevZero","X Go Back to SevZero 0:No 1:Yes,default 0",&g_Ini.iServoZero[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y SevZero","Y Go Back to SevZero 0:No 1:Yes,default 0",&g_Ini.iServoZero[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z SevZero","Z Go Back to SevZero 0:No 1:Yes,default 0",&g_Ini.iServoZero[2],PARA8U,0,1,0);
		CSysPara_PutPara("A SevZero","A Go Back to SevZero 0:No 1:Yes,default 0",&g_Ini.iServoZero[3],PARA8U,0,1,0);

		CSysPara_PutPara("Z RunMode","RunMode.0-Circle,1-Circles,2-Screw,default 0",&g_Ini.iAxisRunMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A RunMode","RunMode.0-Circle,1-Circles,2-Screw,default 0",&g_Ini.iAxisRunMode[3],PARAINT,0,1,0);

		CSysPara_PutPara("X BackMode","BackMode.0:Nearby 1:Foreward 2:Backward 3:Never",&g_Ini.iBackMode[0],PARAINT,0,1,0);
		CSysPara_PutPara("Z BackMode","BackMode.0:Nearby 1:Foreward 2:Backward 3:Never",&g_Ini.iBackMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A BackMode","BackMode.0:Nearby 1:Foreward 2:Backward 3:Never",&g_Ini.iBackMode[3],PARAINT,0,1,0);
		//CSysPara_PutPara("Z BackZero","BackZero Cyclically.0:No,1:Yes,default 1",&g_Ini.iZBackZero,PARA8U,0,1,0);

		CSysPara_PutPara("Z LimitPlus","Soft LimitPlus.Range -1000~1000,default 0.0",&g_Ini.fMaxLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z LimitDec", "Soft LimitDec.Range -1000~1000,default 0.0",&g_Ini.fMinLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A LimitPlus","Soft LimitPlus.Range -1000~1000,default 0.0",&g_Ini.fMaxLimit[3],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A LimitDec", "Soft LimitDec.Range -1000~1000,default 0.0",&g_Ini.fMinLimit[3],PARAFLOAT,1,6,0);

		//CSysPara_PutPara("Z Softlimit","Switch of ZSoftLimit.0:Close,1:Open,defalut0",&g_Ini.iZSoftLimit,PARA8U,0,1,0);
		CSysPara_PutPara("XHard Switch","X HardLimit Check Switch.0:No,1:Yes,default0",&g_Ini.iHardLimitSwitch[0],PARA8U,0,1,0);
		CSysPara_PutPara("YHard	Switch","Y HardLimit Check Switch.0:No,1:Yes,default0",&g_Ini.iHardLimitSwitch[1],PARA8U,0,1,0);
		CSysPara_PutPara("ZHard Switch","Z HardLimit Check Switch.0:No,1:Yes,default0",&g_Ini.iHardLimitSwitch[2],PARA8U,0,1,0);
		CSysPara_PutPara("AHard Switch","A HardLimit Check Switch.0:No,1:Yes,default0",&g_Ini.iHardLimitSwitch[3],PARA8U,0,1,0);

		CSysPara_PutPara("ZZeroForLimt","ZeroPoint replace Limit?0:No,1:-Limit,2:+Limit",&g_Ini.iZeroReplaceLimit[2],PARA8U,0,1,0);
		//CSysPara_PutPara("Probe Ahead",	"Ahead Delay Time,ms,Range 0-9999",				&g_Ini.iProbeAheadTime			,PARAINT,0,4,0);
		//CSysPara_PutPara("Probe Behind","Behind Delay Time,ms,Range 0-9999",			&g_Ini.iProbeLagTime			,PARAINT,0,4,0);

		CSysPara_PutPara("SystemName","System Name,Range 0~9999", &g_Ini.iSystemName,PARALONG,0,4,0);
		CSysPara_PutPara("Axis Count","System Axis Count.Range 2~3",&g_Ini.iAxisCount,PARAINT,0,1,0);
		CSysPara_PutPara("CylinderMode",".Cylinder mode 0:01sequence.1:+/-sequence",&g_Ini.iCyldMode,PARA8U,0,1,0);
		CSysPara_PutPara("Probe mode"," 0: wait for input, 1: is probe mode.",&g_Ini.iPbFnSl,PARAINT,0,1,0);
		CSysPara_PutPara("Probe Delay"," Delay time s.",&g_Ini.iProDelay,PARAFLOAT,0,6,0);


		//CSysPara_PutPara("Start Speed","StartSpd.m/min.Range 1.0~1000.0,default 2.0",&g_Ini.fStartSpeed,PARAFLOAT,1,6,0);
		//CSysPara_PutPara("Add Speed","AddSpd.m/(min2).Range 1~100000,default 2000",&g_Ini.lAddSpeed,PARALONG,0,6,0);
		CSysPara_PutPara("X Max Speed","MaxSpdX.m/min.Range 1.0~3000.0,default 500.0",&g_Ini.fXMaxSpeed,PARAFLOAT,1,6,0);

		CSysPara_PutPara("ControlMode","Control Mode of Sec-axis.0:Smooth,1:Acc/Dec",&g_Ini.iSecondAxisStyle,PARAINT,0,1,0);
		//CSysPara_PutPara("ReverseDly",	"ReverseDelay Ratio .Range 0~30000,default 20",&g_Ini.iReverseDly,PARAINT,0,5,0);
		//CSysPara_PutPara("速度延迟比","速度变化时的延迟比例.范围(0--30000),默认2",&g_Ini.iSpeedDly,PARAINT,0,5,0);

		//CSysPara_PutPara("SafeDoor","SafeDoor Number,Range 1~50",&g_Ini.iSafeDoorNum,PARAINT,0,2,0);
		CSysPara_PutPara("HandTest Spd","Hand wheel test speed.Range 1~10,default 2",&g_Ini.iHandRunSpeed,PARAINT,0,2,0);
		CSysPara_PutPara("Hand Ratio","HandWheel Ratio.Range 1.0~1000.0,default50.0",&g_Ini.fHandRunRate,PARAFLOAT,1,6,0);
		CSysPara_PutPara("ParaSet Psw","Para setting password.default 123",&g_Ini.lPassword,PARALONG,0,6,1);
		CSysPara_PutPara("Language","iLanguage 0:Chinese 1:English,default 0",&g_Ini.iLanguage,PARAINT,0,1,0);
		//add 2020/4/7
		CSysPara_PutPara("Kink IN Port Config","default 21 0-24,255-Disable.",&g_Ini.iIOKinkConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("Kink IN Level Config","default 0 0-Low 1-High.",&g_Ini.iIOKinkConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("BreakLine IN Port Config","default 20 0-24,255-Disable.",&g_Ini.iIOBreakLineConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("BreakLine IN Level Config","default 1 0-Low 1-High.",&g_Ini.iIOBreakLineConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("RunLine IN Port Config","default 18 0-24,255-Disable.",&g_Ini.iIORunLineConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("WireAcc OUT Port Config","default 7 0-24,255-Disable.",&g_Ini.iIOWireAccConfig_OUT,PARA8U,0,3,0);
		CSysPara_PutPara("Sart IN Port Config","default 14 0-24,255-Disable.",&g_Ini.iIOSart_IN,PARA8U,0,3,0);
		CSysPara_PutPara("Single IN Port Config","default 13 0-24,255-Disable.",&g_Ini.iIOSingleConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("Alarm OUT Port Config","default 9 0-24,255-Disable.",&g_Ini.iIOAlarmConfig_OUT,PARA8U,0,3,0);
		//CSysPara_PutPara("SendLine+ IN Port Config","default 14 0-24,255-Disable.",&g_Ini.iIOSendLinePConfig_IN,PARA8U,0,3,0);
		//CSysPara_PutPara("SendLine- IN Port Config","default 13 0-24,255-Disable.",&g_Ini.iIOSendLineMConfig_IN,PARA8U,0,3,0);
	}
	else
	{
		CSysPara_PutPara("X轴脉冲","X轴旋转一圈的脉冲数.单位:个.范围(200-40000)",&g_Ini.lPulse[0],PARALONG,0,6,0);
		CSysPara_PutPara("Y轴脉冲","Y轴旋转一圈的脉冲数.单位:个.范围(200-40000)",&g_Ini.lPulse[1],PARALONG,0,6,0);
		CSysPara_PutPara("Z轴脉冲","Z轴旋转一圈的脉冲数.单位:个.范围(200-40000)",&g_Ini.lPulse[2],PARALONG,0,6,0);
		CSysPara_PutPara("A轴脉冲","A轴旋转一圈的脉冲数.单位:个.范围(200-40000)",&g_Ini.lPulse[3],PARALONG,0,6,0);

		CSysPara_PutPara("X旋转角度","X轴旋转一圈角度.单位:度.范围(0.5-2000.0)",&g_Ini.fMM[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y送线长度","Y轴旋转一圈长度.单位:mm.范围(0.5-2000.0)",&g_Ini.fMM[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z螺间距","Z轴旋转一圈机械距离.单位:mm.范围(0.5-2000.0)",&g_Ini.fMM[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A螺间距","A轴旋转一圈机械距离.单位:mm.范围(0.5-2000.0)",&g_Ini.fMM[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X运行方向","X轴方向逻辑.0:正转 1:反转,默认0",&g_Ini.iRunDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y运行方向","Y轴方向逻辑.0:正转 1:反转,默认0",&g_Ini.iRunDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z运行方向","Z轴方向逻辑.0:正转 1:反转,默认0",&g_Ini.iRunDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A运行方向","A轴方向逻辑.0:正转 1:反转,默认0",&g_Ini.iRunDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X归零方向","X轴回零方向设置.0:正转 1:反转,默认0",&g_Ini.iBackDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y归零方向","Y轴回零方向设置.0:正转 1:反转,默认0",&g_Ini.iBackDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z归零方向","Z轴回零方向设置.0:正转 1:反转,默认0",&g_Ini.iBackDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A归零方向","A轴回零方向设置.0:正转 1:反转,默认0",&g_Ini.iBackDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X减速比","X轴机器减速比.范围(0.1-100.0),默认1.0",&g_Ini.fGearRate[0],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Y减速比","Y轴机器减速比.范围(0.1-100.0),默认1.0",&g_Ini.fGearRate[1],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Z减速比","Z轴机器减速比.范围(0.1-100.0),默认1.0",&g_Ini.fGearRate[2],PARAFLOAT,1,4,0);
		CSysPara_PutPara("A减速比","A轴机器减速比.范围(0.1-100.0),默认1.0",&g_Ini.fGearRate[3],PARAFLOAT,1,4,0);

		CSysPara_PutPara("X电机速度","额定转速.单位:转/分钟.范围(100-8000),默认2000",&g_Ini.lMaxSpeed[0],PARALONG,0,4,0);
		CSysPara_PutPara("Y电机速度","额定转速.单位:转/分钟.范围(100-8000),默认2000",&g_Ini.lMaxSpeed[1],PARALONG,0,4,0);
		CSysPara_PutPara("Z电机速度","额定转速.单位:转/分钟.范围(100-8000),默认2000",&g_Ini.lMaxSpeed[2],PARALONG,0,4,0);
		CSysPara_PutPara("A电机速度","额定转速.单位:转/分钟.范围(100-8000),默认2000",&g_Ini.lMaxSpeed[3],PARALONG,0,4,0);


		CSysPara_PutPara("X加速度","X轴加工时做加减速的加速度.单位:KPP/秒.范围(50-20000).",&g_Ini.lChAddSpeed[0],PARALONG,0,5,0);
		CSysPara_PutPara("Y加速度","Y轴加工时做加减速的加速度.单位:KPP/秒.范围(50-20000).",&g_Ini.lChAddSpeed[1],PARALONG,0,5,0);
		CSysPara_PutPara("Z加速度","Z轴加工时做加减速的加速度.单位:KPP/秒.范围(50-20000).",&g_Ini.lChAddSpeed[2],PARALONG,0,5,0);
		CSysPara_PutPara("A加速度","A轴加工时做加减速的加速度.单位:KPP/秒.范围(50-20000).",&g_Ini.lChAddSpeed[3],PARALONG,0,5,0);

		CSysPara_PutPara("X手动速度","手动基速度.单位:度/秒.范围(0.1-1000.0),默认60.0",&g_Ini.fHandSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y手动速度","手动基速度.单位:毫米/秒.范围(0.1-1000.0),默认60.0",&g_Ini.fHandSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z手动速度","手动基速度.单位:度/秒.范围(0.1-1000.0),默认60.0",&g_Ini.fHandSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A手动速度","手动基速度.单位:度/秒.范围(0.1-1000.0),默认60.0",&g_Ini.fHandSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X归零速度","归零速度.单位:度/秒.范围(0.1-1000.0),默认90.0",&g_Ini.fBackSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y归零速度","归零速度.单位:度/秒.范围(0.1-1000.0),默认90.0",&g_Ini.fBackSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z归零速度","归零速度.单位:度/秒.范围(0.1-1000.0),默认90.0",&g_Ini.fBackSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A归零速度","归零速度.单位:度/秒.范围(0.1-1000.0),默认90.0",&g_Ini.fBackSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X报警逻辑","X轴伺服报警的电平逻辑.0:常开 1:常闭, 默认1",&g_Ini.iServoLogic[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y报警逻辑","Y轴伺服报警的电平逻辑.0:常开 1:常闭, 默认1",&g_Ini.iServoLogic[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z报警逻辑","Z轴伺服报警的电平逻辑.0:常开 1:常闭, 默认1",&g_Ini.iServoLogic[2],PARA8U,0,1,0);
		CSysPara_PutPara("A报警逻辑","A轴伺服报警的电平逻辑.0:常开 1:常闭, 默认1",&g_Ini.iServoLogic[3],PARA8U,0,1,0);

		CSysPara_PutPara("X伺服零点","X轴归零时是否归伺服零点.0:不回 1:回, 默认0",&g_Ini.iServoZero[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y伺服零点","X轴归零时是否归伺服零点.0:不回 1:回, 默认0",&g_Ini.iServoZero[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z伺服零点","Z轴归零时是否归伺服零点.0:不回 1:回, 默认0",&g_Ini.iServoZero[2],PARA8U,0,1,0);
		CSysPara_PutPara("A伺服零点","A轴归零时是否归伺服零点.0:不回 1:回, 默认0",&g_Ini.iServoZero[3],PARA8U,0,1,0);

		CSysPara_PutPara("Z轴运行方式","Z轴运行方式.0-单圈,1-多圈,2-丝杠, 默认0",&g_Ini.iAxisRunMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A轴运行方式","A轴运行方式.0-单圈,1-多圈,2-丝杠, 默认0",&g_Ini.iAxisRunMode[3],PARAINT,0,1,0);

		CSysPara_PutPara("X回零模式","加工完回零方式.0:就近 1:正向 2:负向 3:不回零",&g_Ini.iBackMode[0],PARAINT,0,1,0);
		CSysPara_PutPara("Z回零模式","加工完回零方式.0:就近 1:正向 2:负向 3:不回零",&g_Ini.iBackMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A回零模式","加工完回零方式.0:就近 1:正向 2:负向 3:不回零",&g_Ini.iBackMode[3],PARAINT,0,1,0);
		//CSysPara_PutPara("Z丝杆回零模式","丝杆模式加工完一个周期,Z是否回零点.0:不回,1:回,默认1",&g_Ini.iZBackZero,PARA8U,0,1,0);

		CSysPara_PutPara("Z轴正软件限位","Z轴正软件限位.范围(-1000~1000),默认0.0",&g_Ini.fMaxLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z轴负软件限位","Z轴负软件限位.范围(-1000~1000),默认0.0",&g_Ini.fMinLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A轴正软件限位","A轴正软件限位.范围(-1000~1000),默认0.0",&g_Ini.fMaxLimit[3],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A轴负软件限位","A轴负软件限位.范围(-1000~1000),默认0.0",&g_Ini.fMinLimit[3],PARAFLOAT,1,6,0);

		//CSysPara_PutPara("Z轴软限位","Z轴软限位是否开启.0:不开启,1:开启,默认0",&g_Ini.iZSoftLimit,PARA8U,0,1,0);
		CSysPara_PutPara("X硬限位检测","X轴是否启用硬件限位检测.0:不开启,1:开启,默认0",&g_Ini.iHardLimitSwitch[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y硬限位检测","Y轴是否启用硬件限位检测.0:不开启,1:开启,默认0",&g_Ini.iHardLimitSwitch[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z硬限位检测","Z轴是否启用硬件限位检测.0:不开启,1:开启,默认0",&g_Ini.iHardLimitSwitch[2],PARA8U,0,1,0);
		CSysPara_PutPara("A硬限位检测","A轴是否启用硬件限位检测.0:不开启,1:开启,默认0",&g_Ini.iHardLimitSwitch[3],PARA8U,0,1,0);

		CSysPara_PutPara("Z原点当负限位","Z原点是否当限位?0:否,1:负限位.2.正限位.",&g_Ini.iZeroReplaceLimit[2],PARA8U,0,1,0);
		//CSysPara_PutPara("探针提前延时","打探针提前延时时间,单位ms,范围0-9999",				&g_Ini.iProbeAheadTime			,PARAINT,0,4,0);
		//CSysPara_PutPara("探针滞后延时","打探针滞后延时时间,单位ms,范围0-9999",				&g_Ini.iProbeLagTime			,PARAINT,0,4,0);


		CSysPara_PutPara("系统型号","系统型号编码,范围0~9999,9999为图片型号.", &g_Ini.iSystemName,PARALONG,0,4,0);
		CSysPara_PutPara("系统轴数","系统运转轴数.单位:个.范围(2-3)",&g_Ini.iAxisCount,PARAINT,0,1,0);
		CSysPara_PutPara("气缸模式","加工数据中气缸的控制表达方式.0-01序列.1-正负编号.",&g_Ini.iCyldMode,PARA8U,0,1,0);
		CSysPara_PutPara("探针模式","探针功能选择,0:为输入等待,1:为探针模式.",&g_Ini.iPbFnSl,PARAINT,0,1,0);
		CSysPara_PutPara("探针等待时间","探针为输入等待时候的延时时间ms.",&g_Ini.iProDelay,PARAFLOAT,0,6,0);

		//CSysPara_PutPara("起始速度","起始速度.单位:米/分钟.范围(1.0-1000.0),默认2.0",&g_Ini.fStartSpeed,PARAFLOAT,1,6,0);
		//CSysPara_PutPara("加速度","轴加速度.单位:米/分钟.范围(1-100000),默认2000",&g_Ini.lAddSpeed,PARALONG,0,6,0);
		CSysPara_PutPara("X轴最大速度","单位:转/分钟.范围(1.0-3000.0),默认500.0",&g_Ini.fXMaxSpeed,PARAFLOAT,1,6,0);

		CSysPara_PutPara("运控方式","从轴运动方式.0:连续,1:加减速.",&g_Ini.iSecondAxisStyle,PARAINT,0,1,0);

		//CSysPara_PutPara("正反延迟比",	"转向变化时的延时比例.范围(0--30000),默认20",&g_Ini.iReverseDly,PARAINT,0,5,0);
		//CSysPara_PutPara("速度延迟比","速度变化时的延迟比例.范围(0--30000),默认2",&g_Ini.iSpeedDly,PARAINT,0,5,0);
		//CSysPara_PutPara("安全门端口号","安全门检测端口号,范围(1-50)",&g_Ini.iSafeDoorNum,PARAINT,0,2,0);
		CSysPara_PutPara("手摇速度","手摇轮的检测速度.单位:次.范围(1-10),默认2",&g_Ini.iHandRunSpeed,PARAINT,0,2,0);
		CSysPara_PutPara("手摇比率","手摇轮速度放大倍率.范围(1.0-1000.0),默认50.0",&g_Ini.fHandRunRate,PARAFLOAT,1,6,0);
		CSysPara_PutPara("参数设置密码","默认值:123",&g_Ini.lPassword,PARALONG,0,6,1);
		CSysPara_PutPara("语言选择","0:中文 1:英文,默认0",&g_Ini.iLanguage,PARAINT,0,1,0);

		CSysPara_PutPara("缠线输入端口配置","默认21 0-24有效 255 无效.",&g_Ini.iIOKinkConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("缠线检测电平配置","默认0 0-低电平 1-高电平",&g_Ini.iIOKinkConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("断线输入端口配置","默认20 0-24有效 255 无效.",&g_Ini.iIOBreakLineConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("断线检测电平配置","默认1 0-低电平 1-高电平",&g_Ini.iIOBreakLineConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("跑线输入端口配置","默认18 0-24有效 255 无效.",&g_Ini.iIORunLineConfig_IN,PARA8U,0,3,0);
		//CSysPara_PutPara("线架加速输出端口配置","默认7 0-24有效 255 无效.",&g_Ini.iIOWireAccConfig_OUT,PARA8U,0,3,0);
		CSysPara_PutPara("外部启动输入端口配置","默认14 0-24有效 255 无效.",&g_Ini.iIOSart_IN,PARA8U,0,3,0);
		CSysPara_PutPara("单条输入端口配置","默认13 0-24有效 255 无效.",&g_Ini.iIOSingleConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("报警输出端口配置","默认9 0-24有效 255 无效.",&g_Ini.iIOAlarmConfig_OUT,PARA8U,0,3,0);
		//CSysPara_PutPara("送线+输入端口配置","默认14 0-24有效 255 无效.",&g_Ini.iIOSendLinePConfig_IN,PARA8U,0,3,0);
		//CSysPara_PutPara("送线-输入端口配置","默认13 0-24有效 255 无效.",&g_Ini.iIOSendLineMConfig_IN,PARA8U,0,3,0);
	}

	CMainFrm_ShowAllControl();

	for(i=SYSPARAS_COL*2;i<SYSOBJECT;i++)
	{
		ptr=CMainFrm_GetItemID(0x9000+i);
		CButton_SetCtrl(ptr);
		CButton_Create();
	}

	CSysPara_DrawButtonInfo(0,0);

	CSysPara_UpdateView(s_StartLine);

}


void CSysPara_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus)
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
		for(i=0;i<7;i++)
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



void	CSysPara_Message(INT8S  *ParaInfo)
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



INT32S 	CSysPara_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* SysPara,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD)
{
	if(s_iSysParaCount>=SYSPARAMAX)
		return FALSE;

	sprintf(s_SysParaName[s_iSysParaCount],"%03d,",s_iSysParaCount+1);
	strcat(s_SysParaName[s_iSysParaCount],ParaName);

	sprintf(s_SysParaInfo[s_iSysParaCount],"%03d,",s_iSysParaCount+1);
	strcat(s_SysParaInfo[s_iSysParaCount],ParaInfo);

	p_SysPara[s_iSysParaCount] 		= SysPara;
	SysParaType[s_iSysParaCount]	= Type;
	SysParaDot[s_iSysParaCount] 	= Dot;
	SysParaLong[s_iSysParaCount]	= DataLong;
	SysParaPas[s_iSysParaCount]		= uPWD;
	s_iSysParaCount++;

	return TRUE;
}


INT32S  CSysPara_UpdateView(INT16S iStartLine)
{
	INT16S j=0;
	CTROL   *ptr;

	for(j=0;j<SYSPARAS_COL*2;j++)
	{
		if(iStartLine+j>= s_iSysParaCount)
		{	
			CDC_PutColorBar(g_iDlgLeft+8+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+40+SYSHOFFSET*(j%SYSPARAS_COL),266+120,28,DARKBLUECOLOR,1);
			continue;
		}

		ptr=CMainFrm_GetItemID(0x9000+j);
		CEdit_SetCtrl(ptr);

		if(j==s_CurLine)
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(LIGHTBLUECOLOR);
			CEdit_SetFirst(TRUE);
			CEdit_SetLen(SysParaLong[iStartLine+j]);

			CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),266,26,LIGHTBLUECOLOR,1);
			//CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],BLACKCOLOR,LIGHTBLUECOLOR,MZK_24);
			CDC_OutText24(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],BLACKCOLOR,-1);
			CSysPara_Message(s_SysParaInfo[iStartLine+j]);
		}
		else
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(WHITECOLOR);

			CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),266,26,LIGHTGRAYCOLOR,1);
			//CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],LIGHTBLUECOLOR,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);
			CDC_OutText24(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],DARKBLUECOLOR,-1);
		}

		switch(SysParaType[iStartLine+j])
		{
		case PARAINT:
			CEdit_SetInt(*((INT16S*)p_SysPara[iStartLine+j]));
			break;

		case PARALONG:
			CEdit_SetLong(*((INT32S*)p_SysPara[iStartLine+j]));
			break;

		case PARAFLOAT:
			CEdit_SetFloat(*((FP32*)p_SysPara[iStartLine+j]),SysParaDot[iStartLine+j]);
			break;

		case PARA8U:
			CEdit_SetInt(*((INT8U*)p_SysPara[iStartLine+j]));
			break;

		default:
			break;
		}

		{
			CRect rect;

			CEdit_GetSize(&rect.left, &rect.top, &rect.right,&rect.bottom);
			CDC_DrawEdit_2(rect,WHITECOLOR);
		}

		CEdit_SetNeedPassword(SysParaPas[iStartLine+j]);

		CEdit_UpdateData(TRUE);
	}


	switch(SysParaType[iStartLine+s_CurLine])
	{
	case PARAINT:
		CEdit_SetInt(*((INT16S*)p_SysPara[iStartLine+s_CurLine]));
		break;

	case PARALONG:
		CEdit_SetLong(*((INT32S*)p_SysPara[iStartLine+s_CurLine]));
		break;

	case PARAFLOAT:
		CEdit_SetFloat(*((FP32*)p_SysPara[iStartLine+s_CurLine]),SysParaDot[iStartLine+s_CurLine]);
		break;

	case PARA8U:
		CEdit_SetInt(*((INT8U*)p_SysPara[iStartLine+s_CurLine]));
		break;

	default:
		break;
	}
	CEdit_SetNeedPassword(SysParaPas[iStartLine+s_CurLine]);
	CEdit_SetTextColor(BLACKCOLOR);										//该函数调用完后，背景颜色与字体颜色均为最后一个编辑框对应的颜色，需要重新置为焦点状态的颜色
	CEdit_SetBackColor(LIGHTBLUECOLOR);	
	return TRUE;
}



void	CSysPara_UpdateObject(INT16S dk,BOOL IsEnter)
{
	CTROL *ptr;

	ptr = CMainFrm_GetItemID(0x9000+s_CurLine);

	if(s_CurLine < SYSPARAS_COL*2) 
	{	
		CEdit_SetCtrl(ptr);

		switch(SysParaType[s_StartLine+s_CurLine])
		{
		case PARAINT:
			if(IsEnter)
			{
				INT16S iTemp = *((INT16S*)p_SysPara[s_StartLine+s_CurLine]);

				*((INT16S*)p_SysPara[s_StartLine+s_CurLine])=CEdit_GetInt();

				if(!SysParaCheck())
				{
					*((INT16S*)p_SysPara[s_StartLine+s_CurLine]) = iTemp;
					return;
				}
			}

			CEdit_SetInt(*((INT16S*)p_SysPara[s_StartLine+s_CurLine]));
			break;

		case PARALONG:
			if(IsEnter)
			{
				INT32S lTemp = *((INT32S*)p_SysPara[s_StartLine+s_CurLine]);

				*((INT32S*)p_SysPara[s_StartLine+s_CurLine])=CEdit_GetLong();

				if(!SysParaCheck())
				{
					*((INT32S*)p_SysPara[s_StartLine+s_CurLine]) = lTemp;
					return;
				}
			}

			CEdit_SetLong(*((INT32S*)p_SysPara[s_StartLine+s_CurLine]));
			break;

		case PARAFLOAT:
			if(IsEnter)
			{
				FP32 fTemp = *((FP32*)p_SysPara[s_StartLine+s_CurLine]);

				*((FP32*)p_SysPara[s_StartLine+s_CurLine])=CEdit_GetFloat();

				if(!SysParaCheck())
				{
					*((FP32*)p_SysPara[s_StartLine+s_CurLine]) = fTemp;
					return;
				}

			}
			CEdit_SetFloat(*((FP32*)p_SysPara[s_StartLine+s_CurLine]),SysParaDot[s_StartLine+s_CurLine]);
			break;

		case PARA8U:
			if(IsEnter)
			{
				INT8U uTemp = *((INT8U*)p_SysPara[s_StartLine+s_CurLine]);

				*((INT8U*)p_SysPara[s_StartLine+s_CurLine])=CEdit_GetInt();

				if(!SysParaCheck())
				{
					*((INT8U*)p_SysPara[s_StartLine+s_CurLine]) = uTemp;
					return;
				}
			}

			CEdit_SetInt(*((INT8U*)p_SysPara[s_StartLine+s_CurLine]));
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

	if(s_StartLine+s_CurLine+dk> s_iSysParaCount-1)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_UpdateData(FALSE);
		return;
	}		
	else if(s_StartLine+s_CurLine+dk< 0)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_UpdateData(FALSE);
		return;
	}

	CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),266,26,LIGHTGRAYCOLOR,1);
	//CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],LIGHTBLUECOLOR,s_CurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);
	CDC_OutText24(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],DARKBLUECOLOR,-1);

	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	CEdit_SetNeedPassword(SysParaPas[s_StartLine+s_CurLine]);
	CEdit_UpdateData(FALSE);


	s_CurLine+=dk;

	if(s_CurLine >= SYSPARAS_COL*2)
	{
		s_StartLine += SYSPARAS_COL*2;
		s_CurLine=0;
		CSysPara_UpdateView(s_StartLine);
		return;
	}	

	if(s_CurLine<0 )
	{
		s_StartLine -= SYSPARAS_COL*2;

		if(s_StartLine<0)
			s_StartLine =0;

		s_CurLine=SYSPARAS_COL*2-1;

		CSysPara_UpdateView(s_StartLine);

		return;
	}

	ptr = CMainFrm_GetItemID(0x9000+s_CurLine);

	if(s_CurLine<SYSPARAS_COL*2)
	{		
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);

		switch(SysParaType[s_StartLine+s_CurLine])
		{
		case PARAINT:
			CEdit_SetInt(*((INT16S*)p_SysPara[s_StartLine+s_CurLine]));
			break;

		case PARALONG:
			CEdit_SetLong(*((INT32S*)p_SysPara[s_StartLine+s_CurLine]));
			break;

		case PARAFLOAT:
			CEdit_SetFloat(*((FP32*)p_SysPara[s_StartLine+s_CurLine]),SysParaDot[s_StartLine+s_CurLine]);
			break;

		case PARA8U:
			CEdit_SetInt(*((INT8U *)p_SysPara[s_StartLine+s_CurLine]));
			break;

		default:
			break;
		}

		CEdit_SetLen(SysParaLong[s_StartLine+s_CurLine]);
		CEdit_SetNeedPassword(SysParaPas[s_StartLine+s_CurLine]);

		CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),266,26,LIGHTBLUECOLOR,1);
		//CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],BLACKCOLOR,LIGHTBLUECOLOR,MZK_24);
		CDC_OutText24(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL),g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],BLACKCOLOR,-1);

		CSysPara_Message(s_SysParaInfo[s_StartLine+s_CurLine]);

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


void	CSysPara_OnButtonOk()
{
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);

	//InitBoard();
	{																			//只需要执行部分有变动的，其他可保持不变
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


void	CSysPara_OnButtonCancel()
{
	SysParaRead();
	Ok_Exit=TRUE;
	CEdit_SetNeedPassword(FALSE);
	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	g_iDlgIDOk=0;
}


void CSysPara_OnButtonBackup()
{
	SysParaWrite();
	SysParaWrite();  

	CSysPara_Message(g_bEnglish?"BackUp Success":"备份成功!");

} 


void CSysPara_OnButtonReadBackup()
{
	WriteDataToFile(g_Ini.iFileName);

	if(SysParaRead())
	{
		SysParaWrite();
		CSysPara_Message(g_bEnglish?"Success":"还原成功!");
		OSTimeDly(500);

		CSysPara_UpdateView(s_StartLine);
		g_bSysReadBackup = TRUE;
	}
	else
	{
		CSysPara_Message(g_bEnglish?"Unsuccess":"还原失败!");
	}

}

void	CSysPara_OnButtonInit()
{
	SysParaInit();
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	OSTimeDly(500);
	CSysPara_UpdateView(s_StartLine);
	CSysPara_Message(g_bEnglish?"Init Success":"初始化成功!");
}


void	CSysPara_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;

	ptr=CMainFrm_GetItemID(0x9000+s_CurLine);
	CEdit_SetCtrl(ptr);

	g_bInsertNum = FALSE;
	//	g_bClearNum = FALSE;

	switch(key)
	{
	if(key!=TSET && (key<'1' || key>'7'))
	{
		s_bFunction = FALSE;
		//			LedOut(CYLINDERLED,s_bFunction?1:0);
		CSysPara_DrawButtonInfo(s_bFunction,-1);
	}
	case TSET:

		s_bFunction =s_bFunction?FALSE:TRUE;

		//			LedOut(CYLINDERLED,s_bFunction?1:0);
		CSysPara_DrawButtonInfo(s_bFunction,-1);

		break;

		//case TSAVE:
	case TSCREEN:   												//无存储按键，用TSCREEN代替存储按键
	{
		CSysPara_OnButtonOk();
	}
	break;

	case TCANCEL:
		CSysPara_OnButtonCancel();
		break;

	case TUP:
		CSysPara_UpdateObject(-1,FALSE);
		break;

	case TDOWN:
		CSysPara_UpdateObject(1,FALSE);
		break;

	case TLEFT:
	{
		if(s_CurLine/SYSPARAS_COL)
			CSysPara_UpdateObject(-SYSPARAS_COL,FALSE);
	}
	break;

	case TRIGHT:
	{
		if(!(s_CurLine/SYSPARAS_COL))
			CSysPara_UpdateObject(SYSPARAS_COL,FALSE);
	}
	break;

	case TPAGEUP:
	{
		if((s_StartLine)/(SYSPARAS_COL*2))
		{
			s_StartLine -= SYSPARAS_COL*2;
			s_CurLine=0;
			CSysPara_UpdateView(s_StartLine);
		}
	}
	break;

	case TPAGEDOWN:
	{
		if((s_StartLine/(SYSPARAS_COL*2))<((s_iSysParaCount-1)/(SYSPARAS_COL*2)))
		{
			s_StartLine += SYSPARAS_COL*2;
			s_CurLine=0;
			CSysPara_UpdateView(s_StartLine);
		}
	}
	break;

	case TENTER:
	{
		CSysPara_UpdateObject(1,TRUE);
		break;
	}


	case '0':case '1':case '2':case '3':case '4':case '5':
	case '6':case '7':
	case '8':case '9':case '.':case '-':

		if(s_bFunction)
		{

			if(key!='1' && key!='2')
			{//按下前面两个按键不改变 状态
				s_bFunction=s_bFunction?FALSE:TRUE;
				//					LedOut(CYLINDERLED,s_bFunction?1:0);
				CSysPara_DrawButtonInfo(s_bFunction,-1);
			}
			switch(key)
			{
			case '1':case '2':
			{
				INT16S tmpLine = 0;

				if(key=='1')
				{
					CSysPara_DrawButtonInfo(-1,1);
					tmpLine = -s_CurLine-1;
				}
				else if(key=='2')
				{
					CSysPara_DrawButtonInfo(-1,2);
					tmpLine = SYSPARAS_COL*2-s_CurLine;
				}

				CSysPara_UpdateObject(tmpLine,FALSE);

			}

			break;
			case '3':
				CSysPara_DrawButtonInfo(-1,3);
				CSysPara_OnButtonBackup();
				break;
			case '4':
				CSysPara_DrawButtonInfo(-1,4);
				CSysPara_OnButtonReadBackup();
				break;

			case '5':
				CSysPara_DrawButtonInfo(-1,5);
				CSysPara_OnButtonInit();
				break;
			case '6':
				CSysPara_DrawButtonInfo(-1,6);
				CSysPara_OnButtonCancel();
				break;
			case '7':
				CSysPara_DrawButtonInfo(-1,7);
				CSysPara_OnButtonOk();
				break;

			default:
				break;
			}



			break;
		}
		g_bInsertNum = TRUE;

	case TCLEAR:
		//	if(s_CurLine<s_iSysParaCount-2)
	{
		CEdit_OnKey(key,tkey);
	}

	break;
	/*
		case TBACKSPACE:
				CEdit_OnKey(key,tkey);
			break;
	 */

	case F5:case TDELALL:
#ifdef	PRINTBMP
	{
		char filename[0x20];
		INT16S i=0;

		strcpy(filename,"syspara0");

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

		OSQPost(MsgQueue,"Write Bmp End!");
	}
#endif
	break;


	default:break;
	}
}


INT16S    CSysPara_DoModal()
{
	INT16U key,tkey=0;

	//g_iParameterEdit = 1;

	{
		CSysPara_Create();
	}

	Ok_Exit=FALSE;

	while(TRUE)
	{
		key = KeyMBoxGetKey();

		if(key!=0xffff)
		{

			CSysPara_OnKey(key,tkey);

			if(Ok_Exit)
				break;
		}

		if(RealIO(STOPIO,0,2))
		{
			CSysPara_OnButtonCancel();
			break;
		}

		OSTimeDly(10);
	}

	Ok_Exit=FALSE;
	//g_iParameterEdit = 0;

	return g_iDlgIDOk;
}


