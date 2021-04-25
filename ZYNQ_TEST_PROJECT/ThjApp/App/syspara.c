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

BOOL		g_bSysReadBackup = FALSE;							//��Ҫ��ֹ�������ݲ���������ϵͳ�����ı仯�������½̵�������ʾ����ȷ

extern		INT16S		g_iCurEditNo;
//extern		INT16S		g_iParameterEdit;
extern		BOOL		g_bInsertNum;		
//extern		BOOL		g_bClearNum;




#define		SYSPARAMAX			120								//���Ĳ�������

#define     SYSPARAS_COL    	11								//ÿ�в�������
#define		SYSOBJECT			(SYSPARAS_COL*2+7)				//�༭��ť�������

#define		SYSHOFFSET			32								//��ֱƫ��ֵ
#define		SYSWOFFSET			399								//ˮƽƫ��ֵ

static 		INT32S 		s_iSysParaCount=0;						//����������
static 		INT8S		s_SysParaName[SYSPARAMAX][0x20];		//����˵��
static 		INT8S		s_SysParaInfo[SYSPARAMAX][0x50];		//����˵��

static 		void 		*p_SysPara[SYSPARAMAX];					//ָ�����ݵ�ָ��
static  	INT8U		SysParaType[SYSPARAMAX];				//��������
static 		INT8U		SysParaDot[SYSPARAMAX];					//С����λ��
static 		INT8U		SysParaLong[SYSPARAMAX];				//�������趨�ĳ���λ��
static 		INT8U		SysParaPas[SYSPARAMAX];					//�������趨�ĳ���λ��

static		INT16S		s_StartLine=0;							//ָʾ��ʾ��ʼ��
static		INT16S		s_CurLine=0;							//ָʾ��ʾ��ǰ��
static 		BOOL		s_bFunction = FALSE;					//������Ϊ�Ƿ���"*"�ı�־������ƽ̨�Ǳ�ʾ�Ƿ���"����"�����ı�־����
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

	strcpy(temp.name,g_bEnglish?"PrePage":"�� ҳ");
	temp.x=g_iDlgLeft+3;
	temp.y=g_iDlgTop+g_iDlgHeight-42;
	temp.w=113;
	temp.h=40;
	temp.key=NULL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"NxtPage ":"�� ҳ");
	temp.x=g_iDlgLeft+3+temp.w*1;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"BackUp ":"�� ��");
	temp.x=g_iDlgLeft+3+temp.w*2;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"ReBack":"�� ԭ");
	temp.x=g_iDlgLeft+3+temp.w*3;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Init":"��ʼ��");
	temp.x=g_iDlgLeft+3+temp.w*4;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Cancel":"ȡ ��");
	temp.x=g_iDlgLeft+3+temp.w*5;
	temp.key=TSAVE;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Ok":"�� ��");
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

	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"System parameter":"ϵͳ�����趨");
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
		//CSysPara_PutPara("�ٶ��ӳٱ�","�ٶȱ仯ʱ���ӳٱ���.��Χ(0--30000),Ĭ��2",&g_Ini.iSpeedDly,PARAINT,0,5,0);

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
		CSysPara_PutPara("X������","X����תһȦ��������.��λ:��.��Χ(200-40000)",&g_Ini.lPulse[0],PARALONG,0,6,0);
		CSysPara_PutPara("Y������","Y����תһȦ��������.��λ:��.��Χ(200-40000)",&g_Ini.lPulse[1],PARALONG,0,6,0);
		CSysPara_PutPara("Z������","Z����תһȦ��������.��λ:��.��Χ(200-40000)",&g_Ini.lPulse[2],PARALONG,0,6,0);
		CSysPara_PutPara("A������","A����תһȦ��������.��λ:��.��Χ(200-40000)",&g_Ini.lPulse[3],PARALONG,0,6,0);

		CSysPara_PutPara("X��ת�Ƕ�","X����תһȦ�Ƕ�.��λ:��.��Χ(0.5-2000.0)",&g_Ini.fMM[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y���߳���","Y����תһȦ����.��λ:mm.��Χ(0.5-2000.0)",&g_Ini.fMM[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z�ݼ��","Z����תһȦ��е����.��λ:mm.��Χ(0.5-2000.0)",&g_Ini.fMM[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A�ݼ��","A����תһȦ��е����.��λ:mm.��Χ(0.5-2000.0)",&g_Ini.fMM[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X���з���","X�᷽���߼�.0:��ת 1:��ת,Ĭ��0",&g_Ini.iRunDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y���з���","Y�᷽���߼�.0:��ת 1:��ת,Ĭ��0",&g_Ini.iRunDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z���з���","Z�᷽���߼�.0:��ת 1:��ת,Ĭ��0",&g_Ini.iRunDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A���з���","A�᷽���߼�.0:��ת 1:��ת,Ĭ��0",&g_Ini.iRunDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X���㷽��","X����㷽������.0:��ת 1:��ת,Ĭ��0",&g_Ini.iBackDir[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y���㷽��","Y����㷽������.0:��ת 1:��ת,Ĭ��0",&g_Ini.iBackDir[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z���㷽��","Z����㷽������.0:��ת 1:��ת,Ĭ��0",&g_Ini.iBackDir[2],PARA8U,0,1,0);
		CSysPara_PutPara("A���㷽��","A����㷽������.0:��ת 1:��ת,Ĭ��0",&g_Ini.iBackDir[3],PARA8U,0,1,0);

		CSysPara_PutPara("X���ٱ�","X��������ٱ�.��Χ(0.1-100.0),Ĭ��1.0",&g_Ini.fGearRate[0],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Y���ٱ�","Y��������ٱ�.��Χ(0.1-100.0),Ĭ��1.0",&g_Ini.fGearRate[1],PARAFLOAT,1,4,0);
		CSysPara_PutPara("Z���ٱ�","Z��������ٱ�.��Χ(0.1-100.0),Ĭ��1.0",&g_Ini.fGearRate[2],PARAFLOAT,1,4,0);
		CSysPara_PutPara("A���ٱ�","A��������ٱ�.��Χ(0.1-100.0),Ĭ��1.0",&g_Ini.fGearRate[3],PARAFLOAT,1,4,0);

		CSysPara_PutPara("X����ٶ�","�ת��.��λ:ת/����.��Χ(100-8000),Ĭ��2000",&g_Ini.lMaxSpeed[0],PARALONG,0,4,0);
		CSysPara_PutPara("Y����ٶ�","�ת��.��λ:ת/����.��Χ(100-8000),Ĭ��2000",&g_Ini.lMaxSpeed[1],PARALONG,0,4,0);
		CSysPara_PutPara("Z����ٶ�","�ת��.��λ:ת/����.��Χ(100-8000),Ĭ��2000",&g_Ini.lMaxSpeed[2],PARALONG,0,4,0);
		CSysPara_PutPara("A����ٶ�","�ת��.��λ:ת/����.��Χ(100-8000),Ĭ��2000",&g_Ini.lMaxSpeed[3],PARALONG,0,4,0);


		CSysPara_PutPara("X���ٶ�","X��ӹ�ʱ���Ӽ��ٵļ��ٶ�.��λ:KPP/��.��Χ(50-20000).",&g_Ini.lChAddSpeed[0],PARALONG,0,5,0);
		CSysPara_PutPara("Y���ٶ�","Y��ӹ�ʱ���Ӽ��ٵļ��ٶ�.��λ:KPP/��.��Χ(50-20000).",&g_Ini.lChAddSpeed[1],PARALONG,0,5,0);
		CSysPara_PutPara("Z���ٶ�","Z��ӹ�ʱ���Ӽ��ٵļ��ٶ�.��λ:KPP/��.��Χ(50-20000).",&g_Ini.lChAddSpeed[2],PARALONG,0,5,0);
		CSysPara_PutPara("A���ٶ�","A��ӹ�ʱ���Ӽ��ٵļ��ٶ�.��λ:KPP/��.��Χ(50-20000).",&g_Ini.lChAddSpeed[3],PARALONG,0,5,0);

		CSysPara_PutPara("X�ֶ��ٶ�","�ֶ����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��60.0",&g_Ini.fHandSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y�ֶ��ٶ�","�ֶ����ٶ�.��λ:����/��.��Χ(0.1-1000.0),Ĭ��60.0",&g_Ini.fHandSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z�ֶ��ٶ�","�ֶ����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��60.0",&g_Ini.fHandSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A�ֶ��ٶ�","�ֶ����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��60.0",&g_Ini.fHandSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X�����ٶ�","�����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��90.0",&g_Ini.fBackSpeed[0],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Y�����ٶ�","�����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��90.0",&g_Ini.fBackSpeed[1],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z�����ٶ�","�����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��90.0",&g_Ini.fBackSpeed[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A�����ٶ�","�����ٶ�.��λ:��/��.��Χ(0.1-1000.0),Ĭ��90.0",&g_Ini.fBackSpeed[3],PARAFLOAT,1,6,0);

		CSysPara_PutPara("X�����߼�","X���ŷ������ĵ�ƽ�߼�.0:���� 1:����, Ĭ��1",&g_Ini.iServoLogic[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y�����߼�","Y���ŷ������ĵ�ƽ�߼�.0:���� 1:����, Ĭ��1",&g_Ini.iServoLogic[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z�����߼�","Z���ŷ������ĵ�ƽ�߼�.0:���� 1:����, Ĭ��1",&g_Ini.iServoLogic[2],PARA8U,0,1,0);
		CSysPara_PutPara("A�����߼�","A���ŷ������ĵ�ƽ�߼�.0:���� 1:����, Ĭ��1",&g_Ini.iServoLogic[3],PARA8U,0,1,0);

		CSysPara_PutPara("X�ŷ����","X�����ʱ�Ƿ���ŷ����.0:���� 1:��, Ĭ��0",&g_Ini.iServoZero[0],PARA8U,0,1,0);
		CSysPara_PutPara("Y�ŷ����","X�����ʱ�Ƿ���ŷ����.0:���� 1:��, Ĭ��0",&g_Ini.iServoZero[1],PARA8U,0,1,0);
		CSysPara_PutPara("Z�ŷ����","Z�����ʱ�Ƿ���ŷ����.0:���� 1:��, Ĭ��0",&g_Ini.iServoZero[2],PARA8U,0,1,0);
		CSysPara_PutPara("A�ŷ����","A�����ʱ�Ƿ���ŷ����.0:���� 1:��, Ĭ��0",&g_Ini.iServoZero[3],PARA8U,0,1,0);

		CSysPara_PutPara("Z�����з�ʽ","Z�����з�ʽ.0-��Ȧ,1-��Ȧ,2-˿��, Ĭ��0",&g_Ini.iAxisRunMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A�����з�ʽ","A�����з�ʽ.0-��Ȧ,1-��Ȧ,2-˿��, Ĭ��0",&g_Ini.iAxisRunMode[3],PARAINT,0,1,0);

		CSysPara_PutPara("X����ģʽ","�ӹ�����㷽ʽ.0:�ͽ� 1:���� 2:���� 3:������",&g_Ini.iBackMode[0],PARAINT,0,1,0);
		CSysPara_PutPara("Z����ģʽ","�ӹ�����㷽ʽ.0:�ͽ� 1:���� 2:���� 3:������",&g_Ini.iBackMode[2],PARAINT,0,1,0);
		CSysPara_PutPara("A����ģʽ","�ӹ�����㷽ʽ.0:�ͽ� 1:���� 2:���� 3:������",&g_Ini.iBackMode[3],PARAINT,0,1,0);
		//CSysPara_PutPara("Z˿�˻���ģʽ","˿��ģʽ�ӹ���һ������,Z�Ƿ�����.0:����,1:��,Ĭ��1",&g_Ini.iZBackZero,PARA8U,0,1,0);

		CSysPara_PutPara("Z���������λ","Z���������λ.��Χ(-1000~1000),Ĭ��0.0",&g_Ini.fMaxLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("Z�Ḻ�����λ","Z�Ḻ�����λ.��Χ(-1000~1000),Ĭ��0.0",&g_Ini.fMinLimit[2],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A���������λ","A���������λ.��Χ(-1000~1000),Ĭ��0.0",&g_Ini.fMaxLimit[3],PARAFLOAT,1,6,0);
		CSysPara_PutPara("A�Ḻ�����λ","A�Ḻ�����λ.��Χ(-1000~1000),Ĭ��0.0",&g_Ini.fMinLimit[3],PARAFLOAT,1,6,0);

		//CSysPara_PutPara("Z������λ","Z������λ�Ƿ���.0:������,1:����,Ĭ��0",&g_Ini.iZSoftLimit,PARA8U,0,1,0);
		CSysPara_PutPara("XӲ��λ���","X���Ƿ�����Ӳ����λ���.0:������,1:����,Ĭ��0",&g_Ini.iHardLimitSwitch[0],PARA8U,0,1,0);
		CSysPara_PutPara("YӲ��λ���","Y���Ƿ�����Ӳ����λ���.0:������,1:����,Ĭ��0",&g_Ini.iHardLimitSwitch[1],PARA8U,0,1,0);
		CSysPara_PutPara("ZӲ��λ���","Z���Ƿ�����Ӳ����λ���.0:������,1:����,Ĭ��0",&g_Ini.iHardLimitSwitch[2],PARA8U,0,1,0);
		CSysPara_PutPara("AӲ��λ���","A���Ƿ�����Ӳ����λ���.0:������,1:����,Ĭ��0",&g_Ini.iHardLimitSwitch[3],PARA8U,0,1,0);

		CSysPara_PutPara("Zԭ�㵱����λ","Zԭ���Ƿ���λ?0:��,1:����λ.2.����λ.",&g_Ini.iZeroReplaceLimit[2],PARA8U,0,1,0);
		//CSysPara_PutPara("̽����ǰ��ʱ","��̽����ǰ��ʱʱ��,��λms,��Χ0-9999",				&g_Ini.iProbeAheadTime			,PARAINT,0,4,0);
		//CSysPara_PutPara("̽���ͺ���ʱ","��̽���ͺ���ʱʱ��,��λms,��Χ0-9999",				&g_Ini.iProbeLagTime			,PARAINT,0,4,0);


		CSysPara_PutPara("ϵͳ�ͺ�","ϵͳ�ͺű���,��Χ0~9999,9999ΪͼƬ�ͺ�.", &g_Ini.iSystemName,PARALONG,0,4,0);
		CSysPara_PutPara("ϵͳ����","ϵͳ��ת����.��λ:��.��Χ(2-3)",&g_Ini.iAxisCount,PARAINT,0,1,0);
		CSysPara_PutPara("����ģʽ","�ӹ����������׵Ŀ��Ʊ�﷽ʽ.0-01����.1-�������.",&g_Ini.iCyldMode,PARA8U,0,1,0);
		CSysPara_PutPara("̽��ģʽ","̽�빦��ѡ��,0:Ϊ����ȴ�,1:Ϊ̽��ģʽ.",&g_Ini.iPbFnSl,PARAINT,0,1,0);
		CSysPara_PutPara("̽��ȴ�ʱ��","̽��Ϊ����ȴ�ʱ�����ʱʱ��ms.",&g_Ini.iProDelay,PARAFLOAT,0,6,0);

		//CSysPara_PutPara("��ʼ�ٶ�","��ʼ�ٶ�.��λ:��/����.��Χ(1.0-1000.0),Ĭ��2.0",&g_Ini.fStartSpeed,PARAFLOAT,1,6,0);
		//CSysPara_PutPara("���ٶ�","����ٶ�.��λ:��/����.��Χ(1-100000),Ĭ��2000",&g_Ini.lAddSpeed,PARALONG,0,6,0);
		CSysPara_PutPara("X������ٶ�","��λ:ת/����.��Χ(1.0-3000.0),Ĭ��500.0",&g_Ini.fXMaxSpeed,PARAFLOAT,1,6,0);

		CSysPara_PutPara("�˿ط�ʽ","�����˶���ʽ.0:����,1:�Ӽ���.",&g_Ini.iSecondAxisStyle,PARAINT,0,1,0);

		//CSysPara_PutPara("�����ӳٱ�",	"ת��仯ʱ����ʱ����.��Χ(0--30000),Ĭ��20",&g_Ini.iReverseDly,PARAINT,0,5,0);
		//CSysPara_PutPara("�ٶ��ӳٱ�","�ٶȱ仯ʱ���ӳٱ���.��Χ(0--30000),Ĭ��2",&g_Ini.iSpeedDly,PARAINT,0,5,0);
		//CSysPara_PutPara("��ȫ�Ŷ˿ں�","��ȫ�ż��˿ں�,��Χ(1-50)",&g_Ini.iSafeDoorNum,PARAINT,0,2,0);
		CSysPara_PutPara("��ҡ�ٶ�","��ҡ�ֵļ���ٶ�.��λ:��.��Χ(1-10),Ĭ��2",&g_Ini.iHandRunSpeed,PARAINT,0,2,0);
		CSysPara_PutPara("��ҡ����","��ҡ���ٶȷŴ���.��Χ(1.0-1000.0),Ĭ��50.0",&g_Ini.fHandRunRate,PARAFLOAT,1,6,0);
		CSysPara_PutPara("������������","Ĭ��ֵ:123",&g_Ini.lPassword,PARALONG,0,6,1);
		CSysPara_PutPara("����ѡ��","0:���� 1:Ӣ��,Ĭ��0",&g_Ini.iLanguage,PARAINT,0,1,0);

		CSysPara_PutPara("��������˿�����","Ĭ��21 0-24��Ч 255 ��Ч.",&g_Ini.iIOKinkConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("���߼���ƽ����","Ĭ��0 0-�͵�ƽ 1-�ߵ�ƽ",&g_Ini.iIOKinkConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("��������˿�����","Ĭ��20 0-24��Ч 255 ��Ч.",&g_Ini.iIOBreakLineConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("���߼���ƽ����","Ĭ��1 0-�͵�ƽ 1-�ߵ�ƽ",&g_Ini.iIOBreakLineConfig_Level,PARA8U,0,1,0);
		CSysPara_PutPara("��������˿�����","Ĭ��18 0-24��Ч 255 ��Ч.",&g_Ini.iIORunLineConfig_IN,PARA8U,0,3,0);
		//CSysPara_PutPara("�߼ܼ�������˿�����","Ĭ��7 0-24��Ч 255 ��Ч.",&g_Ini.iIOWireAccConfig_OUT,PARA8U,0,3,0);
		CSysPara_PutPara("�ⲿ��������˿�����","Ĭ��14 0-24��Ч 255 ��Ч.",&g_Ini.iIOSart_IN,PARA8U,0,3,0);
		CSysPara_PutPara("��������˿�����","Ĭ��13 0-24��Ч 255 ��Ч.",&g_Ini.iIOSingleConfig_IN,PARA8U,0,3,0);
		CSysPara_PutPara("��������˿�����","Ĭ��9 0-24��Ч 255 ��Ч.",&g_Ini.iIOAlarmConfig_OUT,PARA8U,0,3,0);
		//CSysPara_PutPara("����+����˿�����","Ĭ��14 0-24��Ч 255 ��Ч.",&g_Ini.iIOSendLinePConfig_IN,PARA8U,0,3,0);
		//CSysPara_PutPara("����-����˿�����","Ĭ��13 0-24��Ч 255 ��Ч.",&g_Ini.iIOSendLineMConfig_IN,PARA8U,0,3,0);
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
	CEdit_SetTextColor(BLACKCOLOR);										//�ú���������󣬱�����ɫ��������ɫ��Ϊ���һ���༭���Ӧ����ɫ����Ҫ������Ϊ����״̬����ɫ
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
	{																			//ֻ��Ҫִ�в����б䶯�ģ������ɱ��ֲ���
		INT16S j=0;

		for(j=0; j<CHMAX; j++)
		{
			SetUnitPulse(j+1,g_Ini.fMM[j],g_Ini.lPulse[j]); 					//�趨�������嵱��		
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

	CSysPara_Message(g_bEnglish?"BackUp Success":"���ݳɹ�!");

} 


void CSysPara_OnButtonReadBackup()
{
	WriteDataToFile(g_Ini.iFileName);

	if(SysParaRead())
	{
		SysParaWrite();
		CSysPara_Message(g_bEnglish?"Success":"��ԭ�ɹ�!");
		OSTimeDly(500);

		CSysPara_UpdateView(s_StartLine);
		g_bSysReadBackup = TRUE;
	}
	else
	{
		CSysPara_Message(g_bEnglish?"Unsuccess":"��ԭʧ��!");
	}

}

void	CSysPara_OnButtonInit()
{
	SysParaInit();
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	OSTimeDly(500);
	CSysPara_UpdateView(s_StartLine);
	CSysPara_Message(g_bEnglish?"Init Success":"��ʼ���ɹ�!");
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
	case TSCREEN:   												//�޴洢��������TSCREEN����洢����
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
			{//����ǰ�������������ı� ״̬
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


