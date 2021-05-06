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
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
 */

BOOL		g_bSysReadBackup = FALSE;							//主要防止导出备份参数，由于系统轴数的变化，而导致教导界面显示不正确

extern		INT16S		g_iCurEditNo;
extern		INT16S		g_iParameterEdit;
extern		BOOL		g_bInsertNum;		
extern		BOOL		g_bClearNum;




#define		SYSPARAMAX			200								//最大的参数个数

#define     SYSPARAS_COL    	15								//每列参数个数
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
static 		BOOL		s_bCylinder = FALSE;					//气缸状态


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
	s_bCylinder =FALSE;	
	LedOut(CYLINDERLED,s_bCylinder?1:0);
}


void   CSysPara_Create()
{
	//	INT16S	j=g_iDlgTop+45-SYSHOFFSET;
	CTROL   *ptr;
	INT16S 	i = 0;

	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"System parameter":"系统参数设定");
	CSysPara_Message(NULL);

	{
		g_bEnglish?CSysPara_PutPara("XPulse","Pulses per Round of X-axis.Range(2000-80000).",&g_Ini.lPulse[0],PARALONG,0,6,0)
				:CSysPara_PutPara("X轴脉冲","X轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[0],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("YPulse","Pulses per Round of Y-axis.Range(2000-80000).",&g_Ini.lPulse[1],PARALONG,0,6,0)
				:CSysPara_PutPara("Y轴脉冲","Y轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[1],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZPulse","Pulses per Round of Z-axis.Range(2000-80000).",&g_Ini.lPulse[2],PARALONG,0,6,0)
				:CSysPara_PutPara("Z轴脉冲","Z轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[2],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("APulse","Pulses per Round of A-axis.Range(2000-80000).",&g_Ini.lPulse[3],PARALONG,0,6,0)
				:CSysPara_PutPara("A轴脉冲","A轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[3],PARALONG,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BPulse","Pulses per Round of B-axis.Range(2000-80000).",&g_Ini.lPulse[4],PARALONG,0,6,0)
					:CSysPara_PutPara("B轴脉冲","B轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[4],PARALONG,0,6,0);

			g_bEnglish?CSysPara_PutPara("CPulse","Pulses per Round of C-axis.Range(2000-80000).",&g_Ini.lPulse[5],PARALONG,0,6,0)
					:CSysPara_PutPara("C轴脉冲","C轴旋转一圈的脉冲数.单位:个.范围(2000-80000).",&g_Ini.lPulse[5],PARALONG,0,6,0);
		}

		g_bEnglish?CSysPara_PutPara("XEddyDeg","Degrees per Round of X-axis.UNIT:°.Range(1-2000).",&g_Ini.fMM[0],PARAFLOAT,1,6,0)
				:CSysPara_PutPara("X旋转角度","X轴旋转一圈角度.单位:度.范围(1-2000).",&g_Ini.fMM[0],PARAFLOAT,1,6,0);

		g_bEnglish?CSysPara_PutPara("YSendLen","Length per Round of Y-axis.UNIT:mm.Range(1-2000).",&g_Ini.fMM[1],PARAFLOAT,1,6,0)
				:CSysPara_PutPara("Y送线长度","Y轴旋转一圈长度.单位:毫米.范围(1-2000).",&g_Ini.fMM[1],PARAFLOAT,1,6,0);

		g_bEnglish?
				(CSysPara_PutPara(g_Ini.iAxisRunMode[2]==2?"ZPitch":"ZEddyDeg",
						g_Ini.iAxisRunMode[2]==2?"Pitch of Z-axis.UNIT:mm.Range(1-2000).":"Degrees per Round of Z-axis.UNIT:°.Range(1-2000).",&g_Ini.fMM[2],PARAFLOAT,1,6,0))
						:CSysPara_PutPara(g_Ini.iAxisRunMode[2]==2?"Z丝杆螺距":"Z旋转角度",
								g_Ini.iAxisRunMode[2]==2?"Z轴的螺距大小.单位:毫米.范围(1-2000).":"Z轴旋转一圈角度.单位:度.范围(1-2000).",&g_Ini.fMM[2],PARAFLOAT,1,6,0);

		g_bEnglish?
				(CSysPara_PutPara(g_Ini.iAxisRunMode[3]==2?"APitch":"AEddyDeg",
						g_Ini.iAxisRunMode[3]==2?"Pitch of A-axis.UNIT:mm.Range(1-2000).":"Degrees per Round of A-axis.UNIT:°.Range(1-2000).",&g_Ini.fMM[3],PARAFLOAT,1,6,0))
						:CSysPara_PutPara(g_Ini.iAxisRunMode[3]==2?"A丝杆螺距":"A旋转角度",
								g_Ini.iAxisRunMode[3]==2?"A轴的螺距大小.单位:毫米.范围(1-2000).":"A轴旋转一圈角度.单位:度.范围(1-2000).",&g_Ini.fMM[3],PARAFLOAT,1,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?
					(CSysPara_PutPara(g_Ini.iAxisRunMode[4]==2?"BPitch":"BEddyDeg",
							g_Ini.iAxisRunMode[4]==2?"Pitch of B-axis.UNIT:mm.Range(1-2000).":"Degrees per Round of B-axis.UNIT:°.Range(1-2000).",&g_Ini.fMM[4],PARAFLOAT,1,6,0))
							:CSysPara_PutPara(g_Ini.iAxisRunMode[4]==2?"B丝杆螺距":"B旋转角度",
									g_Ini.iAxisRunMode[4]==2?"B轴的螺距大小.单位:毫米.范围(1-2000).":"B轴旋转一圈角度.单位:度.范围(1-2000).",&g_Ini.fMM[4],PARAFLOAT,1,6,0);

			g_bEnglish?
					(CSysPara_PutPara(g_Ini.iAxisRunMode[5]==2?"CPitch":"CEddyDeg",
							g_Ini.iAxisRunMode[5]==2?"Pitch of C-axis.UNIT:mm.Range(1-2000).":"Degrees per Round of C-axis.UNIT:°.Range(1-2000).",&g_Ini.fMM[5],PARAFLOAT,1,6,0))
							:CSysPara_PutPara(g_Ini.iAxisRunMode[5]==2?"C丝杆螺距":"C旋转角度",
									g_Ini.iAxisRunMode[5]==2?"C轴的螺距大小.单位:毫米.范围(1-2000).":"C轴旋转一圈角度.单位:度.范围(1-2000).",&g_Ini.fMM[5],PARAFLOAT,1,6,0);

		}


		g_bEnglish?CSysPara_PutPara("XRunDir","Running Direction of X-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X运行方向","X轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[0],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("YRunDir","Running Direction of Y-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[1],PARA8U,0,1,0)
				:CSysPara_PutPara("Y运行方向","Y轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[1],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZRunDir","Running Direction of Z-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z运行方向","Z轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ARunDir","Running Direction of A-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A运行方向","A轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[3],PARA8U,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BRunDir","Running Direction of B-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B运行方向","B轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("CRunDir","Running Direction of C-axis.0:Foreward,1:Backward.",&g_Ini.iRunDir[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C运行方向","C轴方向逻辑.(0:正向,1:反向).",&g_Ini.iRunDir[5],PARA8U,0,1,0);

		}


		g_bEnglish?CSysPara_PutPara("XBackDir","BackZero Direction of X-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X归零方向","X轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[0],PARA8U,0,1,0);

		//g_bEnglish?CSysPara_PutPara("YBackDir","BackZero Direction of Y-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[1],PARA8U,0,1,0)
		//:CSysPara_PutPara("Y归零方向","Y轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[1],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZBackDir","BackZero Direction of Z-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z归零方向","Z轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ABackDir","BackZero Direction of A-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A归零方向","A轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[3],PARA8U,0,1,0);


		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BBackDir","BackZero Direction of B-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B归零方向","B轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("CBackDir","BackZero Direction of C-axis.0:Foreward,1:Backward.",&g_Ini.iBackDir[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C归零方向","C轴回零方向设置.(0:正向,1:负向).",&g_Ini.iBackDir[5],PARA8U,0,1,0);
		}


		g_bEnglish?CSysPara_PutPara("XGearRate","Gear Rate of X-axis.Range(1-1000).",&g_Ini.lGearRate[0],PARAFLOAT,1,4,0)
				:CSysPara_PutPara("X减速比","X轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[0],PARAFLOAT,1,4,0);

		g_bEnglish?CSysPara_PutPara("YGearRate","Gear Rate of Y-axis.Range(1-1000).",&g_Ini.lGearRate[1],PARAFLOAT,1,4,0)
				:CSysPara_PutPara("Y减速比","Y轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[1],PARAFLOAT,1,4,0);

		g_bEnglish?CSysPara_PutPara("ZGearRate","Gear Rate of Z-axis.Range(1-1000).",&g_Ini.lGearRate[2],PARAFLOAT,1,4,0)
				:CSysPara_PutPara("Z减速比","Z轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[2],PARAFLOAT,1,4,0);

		g_bEnglish?CSysPara_PutPara("AGearRate","Gear Rate of A-axis.Range(1-1000).",&g_Ini.lGearRate[3],PARAFLOAT,1,4,0)
				:CSysPara_PutPara("A减速比","A轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[3],PARAFLOAT,1,4,0);


		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BGearRate","Gear Rate of B-axis.Range(1-1000).",&g_Ini.lGearRate[4],PARAFLOAT,1,4,0)
					:CSysPara_PutPara("B减速比","B轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[4],PARAFLOAT,1,4,0);

			g_bEnglish?CSysPara_PutPara("CGearRate","Gear Rate of C-axis.Range(1-1000).",&g_Ini.lGearRate[5],PARAFLOAT,1,4,0)
					:CSysPara_PutPara("C减速比","C轴机器减速比.单位:比值.范围(1-1000).",&g_Ini.lGearRate[5],PARAFLOAT,1,4,0);

		}


		g_bEnglish?CSysPara_PutPara("XMotorSpeed","Rated Speed of X-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[0],PARALONG,0,6,0)
				:CSysPara_PutPara("X电机速度","X轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[0],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("YMotorSpeed","Rated Speed of Y-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[1],PARALONG,0,6,0)
				:CSysPara_PutPara("Y电机速度","Y轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[1],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZMotorSpeed","Rated Speed of Z-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[2],PARALONG,0,6,0)
				:CSysPara_PutPara("Z电机速度","Z轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[2],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("AMotorSpeed","Rated Speed of A-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[3],PARALONG,0,6,0)
				:CSysPara_PutPara("A电机速度","A轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[3],PARALONG,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BMotorSpeed","Rated Speed of B-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[4],PARALONG,0,6,0)
					:CSysPara_PutPara("B电机速度","B轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[4],PARALONG,0,6,0);

			g_bEnglish?CSysPara_PutPara("CMotorSpeed","Rated Speed of C-axis Motor.UNIT:r/min.Range(100-8000).",&g_Ini.lMaxSpeed[5],PARALONG,0,6,0)
					:CSysPara_PutPara("C电机速度","C轴电机额定转速.单位:转/分钟.范围(100-8000).",&g_Ini.lMaxSpeed[5],PARALONG,0,6,0);

		}


		g_bEnglish?CSysPara_PutPara("XWarnLogic","Warn Logic Level of X-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X报警逻辑","X轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[0],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("YWarnLogic","Warn Logic Level of Y-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[1],PARA8U,0,1,0)
				:CSysPara_PutPara("Y报警逻辑","Y轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[1],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZWarnLogic","Warn Logic Level of Z-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z报警逻辑","Z轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("AWarnLogic","Warn Logic Level of A-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A报警逻辑","A轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[3],PARA8U,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BWarnLogic","Warn Logic Level of B-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B报警逻辑","B轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("CWarnLogic","Warn Logic Level of C-axis Servo.0:Low,1:High.",&g_Ini.iServoLogic[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C报警逻辑","C轴伺服报警的电平逻辑.(0:常态高电平,1:常态低电平).",&g_Ini.iServoLogic[5],PARA8U,0,1,0);
		}


		g_bEnglish?CSysPara_PutPara("XBackSpeed","BackSpeed of X-axis.UNIT:°/s.Range(1-1000).",&g_Ini.fBackSpeed[0],PARAFLOAT,1,6,0)
				:CSysPara_PutPara("X归零速度","X轴归零的速度.单位:度/秒.范围(1-1000).",&g_Ini.fBackSpeed[0],PARAFLOAT,1,6,0);

		g_bEnglish?CSysPara_PutPara("ZBackSpeed","BackSpeed of Z-axis.UNIT:°/s(mm/s).Range(1-1000).",&g_Ini.fBackSpeed[2],PARAFLOAT,1,6,0)
				:CSysPara_PutPara("Z归零速度","Z轴归零的速度.单位:度/秒(毫米/秒).范围(1-1000).",&g_Ini.fBackSpeed[2],PARAFLOAT,1,6,0);

		g_bEnglish?CSysPara_PutPara("ABackSpeed","BackSpeed of A-axis.UNIT:°/s(mm/s).Range(1-1000).",&g_Ini.fBackSpeed[3],PARAFLOAT,1,6,0)
				:CSysPara_PutPara("A归零速度","A轴归零的速度.单位:度/秒(毫米/秒).范围(1-1000).",&g_Ini.fBackSpeed[3],PARAFLOAT,1,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BBackSpeed","BackSpeed of B-axis.UNIT:°/s(mm/s).Range(1-1000).",&g_Ini.fBackSpeed[4],PARAFLOAT,1,6,0)
					:CSysPara_PutPara("B归零速度","B轴归零的速度.单位:度/秒(毫米/秒).范围(1-1000).",&g_Ini.fBackSpeed[4],PARAFLOAT,1,6,0);

			g_bEnglish?CSysPara_PutPara("CBackSpeed","BackSpeed of C-axis.UNIT:°/s(mm/s).Range(1-1000).",&g_Ini.fBackSpeed[5],PARAFLOAT,1,6,0)
					:CSysPara_PutPara("C归零速度","C轴归零的速度.单位:度/秒(毫米/秒).范围(1-1000).",&g_Ini.fBackSpeed[5],PARAFLOAT,1,6,0);

		}

		g_bEnglish?CSysPara_PutPara("XBackMode","BackMode of X.0:Nearby,1:Foreword,2:Backword.3:Zero.4:No",&g_Ini.iBackMode[0],PARAINT,0,1,0)
				:CSysPara_PutPara("X回零模式","X轴加工完回零的方式.(0:就近,1:正向,2:负向,3:零向,4:不回零).",&g_Ini.iBackMode[0],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZBackMode","BackMode of Z.0:Nearby,1:Foreword,2:Backword.3:Zero.4:No",&g_Ini.iBackMode[2],PARAINT,0,1,0)
				:CSysPara_PutPara("Z回零模式","Z轴加工完回零的方式.(0:就近,1:正向,2:负向,3:零向,4:不回零).",&g_Ini.iBackMode[2],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("ABackMode","BackMode of A.0:Nearby,1:Foreword,2:Backword.3:Zero.4:No",&g_Ini.iBackMode[3],PARAINT,0,1,0)
				:CSysPara_PutPara("A回零模式","A轴加工完回零的方式.(0:就近,1:正向,2:负向,3:零向,4:不回零).",&g_Ini.iBackMode[3],PARAINT,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BBackMode","BackMode of B.0:Nearby,1:Foreword,2:Backword.3:Zero.4:No",&g_Ini.iBackMode[4],PARAINT,0,1,0)
					:CSysPara_PutPara("B回零模式","B轴加工完回零的方式.(0:就近,1:正向,2:负向,3:零向,4:不回零).",&g_Ini.iBackMode[4],PARAINT,0,1,0);

			g_bEnglish?CSysPara_PutPara("CBackMode","BackMode of C.0:Nearby,1:Foreword,2:Backword.3:Zero.4:No",&g_Ini.iBackMode[5],PARAINT,0,1,0)
					:CSysPara_PutPara("C回零模式","C轴加工完回零的方式.(0:就近,1:正向,2:负向,3:零向,4:不回零).",&g_Ini.iBackMode[5],PARAINT,0,1,0);

		}

		g_bEnglish?CSysPara_PutPara("XSearchZero","0:Nope,1:Find machine zero,2:Find servo zero,3:Both",&g_Ini.iSearchZero[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X搜索零点方式","0:不归零,1:搜索机械零点,2:搜索伺服零点,3:搜索机械零点和伺服零点.",&g_Ini.iSearchZero[0],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZSearchZero","0:Nope,1:Find machine zero,2:Find servo zero,3:Both",&g_Ini.iSearchZero[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z搜索零点方式","0:不归零,1:搜索机械零点,2:搜索伺服零点,3:搜索机械零点和伺服零点.",&g_Ini.iSearchZero[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ASearchZero","0:Nope,1:Find machine zero,2:Find servo zero,3:Both",&g_Ini.iSearchZero[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A搜索零点方式","0:不归零,1:搜索机械零点,2:搜索伺服零点,3:搜索机械零点和伺服零点.",&g_Ini.iSearchZero[3],PARA8U,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BSearchZero","0:Nope,1:Find machine zero,2:Find servo zero,3:Both",&g_Ini.iSearchZero[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B搜索零点方式","0:不归零,1:搜索机械零点,2:搜索伺服零点,3:搜索机械零点和伺服零点.",&g_Ini.iSearchZero[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("CSearchZero","0:Nope,1:Find machine zero,2:Find servo zero,3:Both",&g_Ini.iSearchZero[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C搜索零点方式","0:不归零,1:搜索机械零点,2:搜索伺服零点,3:搜索机械零点和伺服零点.",&g_Ini.iSearchZero[5],PARA8U,0,1,0);
		}


		g_bEnglish?CSysPara_PutPara("XHandSpeed","Speed of X-axis when Handing.UNIT:°/s.Range(1-999).",&g_Ini.lHandSpeed[0],PARALONG,0,3,0)
				:CSysPara_PutPara("X手动速度","X单轴手摇和寸动的基础速度.单位:度/秒.范围(1-999).",&g_Ini.lHandSpeed[0],PARALONG,0,3,0);

		g_bEnglish?CSysPara_PutPara("YHandSpeed","Speed of Y-axis when Handing.UNIT:mm/s.Range(1-999).",&g_Ini.lHandSpeed[1],PARALONG,0,3,0)
				:CSysPara_PutPara("Y手动速度","Y单轴手摇和寸动的基础速度.单位:毫米/秒.范围(1-999).",&g_Ini.lHandSpeed[1],PARALONG,0,3,0);

		g_bEnglish?CSysPara_PutPara("ZHandSpeed","Speed of Z-axis when Handing.UNIT:°/s(mm/s).Range(1-999).",&g_Ini.lHandSpeed[2],PARALONG,0,3,0)
				:CSysPara_PutPara("Z手动速度","Z单轴手摇和寸动的基础速度.单位:度/秒(毫米/秒).范围(1-999).",&g_Ini.lHandSpeed[2],PARALONG,0,3,0);

		g_bEnglish?CSysPara_PutPara("AHandSpeed","Speed of A-axis when Handing.UNIT:°/s(mm/s).Range(1-999).",&g_Ini.lHandSpeed[3],PARALONG,0,3,0)
				:CSysPara_PutPara("A手动速度","A单轴手摇和寸动的基础速度.单位:度/秒(毫米/秒).范围(1-999).",&g_Ini.lHandSpeed[3],PARALONG,0,3,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BHandSpeed","Speed of B-axis when Handing.UNIT:°/s(mm/s).Range(1-999).",&g_Ini.lHandSpeed[4],PARALONG,0,3,0)
					:CSysPara_PutPara("B手动速度","B单轴手摇和寸动的基础速度.单位:度/秒(毫米/秒).范围(1-999).",&g_Ini.lHandSpeed[4],PARALONG,0,3,0);

			g_bEnglish?CSysPara_PutPara("CHandSpeed","Speed of C-axis when Handing.UNIT:°/s(mm/s).Range(1-999).",&g_Ini.lHandSpeed[5],PARALONG,0,3,0)
					:CSysPara_PutPara("C手动速度","C单轴手摇和寸动的基础速度.单位:度/秒(毫米/秒).范围(1-999).",&g_Ini.lHandSpeed[5],PARALONG,0,3,0);

		}


		g_bEnglish?CSysPara_PutPara("XAcc","Producting Acc of X-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[0],PARALONG,0,5,0)
				:CSysPara_PutPara("X加速度","X轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[0],PARALONG,0,5,0);

		g_bEnglish?CSysPara_PutPara("YAcc","Producting Acc of Y-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[1],PARALONG,0,5,0)
				:CSysPara_PutPara("Y加速度","Y轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[1],PARALONG,0,5,0);

		g_bEnglish?CSysPara_PutPara("ZAcc","Producting Acc of Z-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[2],PARALONG,0,5,0)
				:CSysPara_PutPara("Z加速度","Z轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[2],PARALONG,0,5,0);

		g_bEnglish?CSysPara_PutPara("AAcc","Producting Acc of A-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[3],PARALONG,0,5,0)
				:CSysPara_PutPara("A加速度","A轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[3],PARALONG,0,5,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BAcc","Producting Acc of B-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[4],PARALONG,0,5,0)
					:CSysPara_PutPara("B加速度","B轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[4],PARALONG,0,5,0);

			g_bEnglish?CSysPara_PutPara("CAcc","Producting Acc of C-axis.UNIT:KPP/s.Range(50-10000).",&g_Ini.lChAddSpeed[5],PARALONG,0,5,0)
					:CSysPara_PutPara("C加速度","C轴加工时做加减速的加速度.单位:KPP/秒.范围(50-10000).",&g_Ini.lChAddSpeed[5],PARALONG,0,5,0);
		}


		g_bEnglish?CSysPara_PutPara("XLag","Upperlimit Pulses of X-axis Laging.Range(0-999999).",&g_Ini.iLag[0],PARALONG,0,6,0)
				:CSysPara_PutPara("X轴滞后量","X轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[0],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("YLag","Upperlimit Pulses of Y-axis Laging.Range(0-999999).",&g_Ini.iLag[1],PARALONG,0,6,0)
				:CSysPara_PutPara("Y轴滞后量","Y轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[1],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZLag","Upperlimit Pulses of Z-axis Laging.Range(0-999999).",&g_Ini.iLag[2],PARALONG,0,6,0)
				:CSysPara_PutPara("Z轴滞后量","Z轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[2],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ALag","Upperlimit Pulses of A-axis Laging.Range(0-999999).",&g_Ini.iLag[3],PARALONG,0,6,0)
				:CSysPara_PutPara("A轴滞后量","A轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[3],PARALONG,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BLag","Upperlimit Pulses of B-axis Laging.Range(0-999999).",&g_Ini.iLag[4],PARALONG,0,6,0)
					:CSysPara_PutPara("B轴滞后量","B轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[4],PARALONG,0,6,0);

			g_bEnglish?CSysPara_PutPara("CLag","Upperlimit Pulses of C-axis Laging.Range(0-999999).",&g_Ini.iLag[5],PARALONG,0,6,0)
					:CSysPara_PutPara("C轴滞后量","C轴运动时实际逻辑位置偏差报警脉冲值.范围(0-999999).",&g_Ini.iLag[5],PARALONG,0,6,0);

		}


		g_bEnglish?CSysPara_PutPara("XActRate","Numerator of XActPos/XLogicPos.Range(0-999999).",&g_Ini.iNumerator[0],PARALONG,0,6,0)
				:CSysPara_PutPara("X实际脉冲比","X轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[0],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("YActRate","Numerator of YActPos/YLogicPos.Range(0-999999).",&g_Ini.iNumerator[1],PARALONG,0,6,0)
				:CSysPara_PutPara("Y实际脉冲比","Y轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[1],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZActRate","Numerator of ZActPos/ZLogicPos.Range(0-999999).",&g_Ini.iNumerator[2],PARALONG,0,6,0)
				:CSysPara_PutPara("Z实际脉冲比","Z轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[2],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("AActRate","Numerator of AActPos/ALogicPos.Range(0-999999).",&g_Ini.iNumerator[3],PARALONG,0,6,0)
				:CSysPara_PutPara("A实际脉冲比","A轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[3],PARALONG,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BActRate","Numerator of BActPos/BLogicPos.Range(0-999999).",&g_Ini.iNumerator[4],PARALONG,0,6,0)
					:CSysPara_PutPara("B实际脉冲比","B轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[4],PARALONG,0,6,0);

			g_bEnglish?CSysPara_PutPara("CActRate","Numerator of CActPos/CLogicPos.Range(0-999999).",&g_Ini.iNumerator[5],PARALONG,0,6,0)
					:CSysPara_PutPara("C实际脉冲比","C轴实际位置与逻辑位置反馈比中的实际位置的比值.范围(0-999999).",&g_Ini.iNumerator[5],PARALONG,0,6,0);
		}


		g_bEnglish?CSysPara_PutPara("XLogicRate","Denominator of XActPos/XLogicPos.Range(-99999~999999).",&g_Ini.iDenominator[0],PARALONG,0,6,0)
				:CSysPara_PutPara("X逻辑脉冲比","X轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[0],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("YLogicRate","Denominator of YActPos/YLogicPos.Range(-99999~999999).",&g_Ini.iDenominator[1],PARALONG,0,6,0)
				:CSysPara_PutPara("Y逻辑脉冲比","Y轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[1],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZLogicRate","Denominator of ZActPos/ZLogicPos.Range(-99999~999999).",&g_Ini.iDenominator[2],PARALONG,0,6,0)
				:CSysPara_PutPara("Z逻辑脉冲比","Z轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[2],PARALONG,0,6,0);

		g_bEnglish?CSysPara_PutPara("ALogicRate","Denominator of AActPos/ALogicPos.Range(-99999~999999).",&g_Ini.iDenominator[3],PARALONG,0,6,0)
				:CSysPara_PutPara("A逻辑脉冲比","A轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[3],PARALONG,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{

			g_bEnglish?CSysPara_PutPara("BLogicRate","Denominator of BActPos/BLogicPos.Range(-99999~999999).",&g_Ini.iDenominator[4],PARALONG,0,6,0)
					:CSysPara_PutPara("B逻辑脉冲比","B轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[4],PARALONG,0,6,0);

			g_bEnglish?CSysPara_PutPara("CLogicRate","Denominator of CActPos/CLogicPos.Range(-99999~999999).",&g_Ini.iDenominator[5],PARALONG,0,6,0)
					:CSysPara_PutPara("C逻辑脉冲比","C轴实际与逻辑位置反馈比中的逻辑位置比值.范围(-99999~999999).",&g_Ini.iDenominator[5],PARALONG,0,6,0);
		}


		g_bEnglish?CSysPara_PutPara("XName","0:Default,X-axis.",&g_Ini.iAxisNameNo[0],PARAINT,0,2,0)
				:CSysPara_PutPara("X轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[0],PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("YName","0:Default,Y-axis.",&g_Ini.iAxisNameNo[1],PARAINT,0,2,0)
				:CSysPara_PutPara("Y轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[1],PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("ZName","0:Default,Z-axis.",&g_Ini.iAxisNameNo[2],PARAINT,0,2,0)
				:CSysPara_PutPara("Z轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[2],PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("AName","0:Default,A-axis.",&g_Ini.iAxisNameNo[3],PARAINT,0,2,0)
				:CSysPara_PutPara("A轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[3],PARAINT,0,2,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BName","0:Default,B-axis.",&g_Ini.iAxisNameNo[4],PARAINT,0,2,0)
					:CSysPara_PutPara("B轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[4],PARAINT,0,2,0);

			g_bEnglish?CSysPara_PutPara("CName","0:Default,C-axis.",&g_Ini.iAxisNameNo[5],PARAINT,0,2,0)
					:CSysPara_PutPara("C轴名称","0默认,1外径,2送线,3转芯,4卷取,5夹耳,6节距,7切刀,8上切,9下切,10转线",&g_Ini.iAxisNameNo[5],PARAINT,0,2,0);
		}


		g_bEnglish?CSysPara_PutPara("XPosSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMaxLimit[0],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("X轴正软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMaxLimit[0],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("XNegSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMinLimit[0],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("X轴负软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMinLimit[0],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("YPosSftLimit","UNIT:0.01mm.Range(-99999~999999).",&g_Ini.fMaxLimit[1],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("Y轴正软限位","单位:0.01毫米.范围(-99999~999999).",&g_Ini.fMaxLimit[1],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("YNegSftLimit","UNIT:0.01mm.Range(-99999~999999).",&g_Ini.fMinLimit[1],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("Y轴负软限位","单位:0.01毫米.范围(-99999~999999).",&g_Ini.fMinLimit[1],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZPosSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMaxLimit[2],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("Z轴正软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMaxLimit[2],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("ZNegSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMinLimit[2],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("Z轴负软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMinLimit[2],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("APosSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMaxLimit[3],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("A轴正软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMaxLimit[3],PARAFLOAT,0,6,0);

		g_bEnglish?CSysPara_PutPara("ANegSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMinLimit[3],PARAFLOAT,0,6,0)
				:CSysPara_PutPara("A轴负软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMinLimit[3],PARAFLOAT,0,6,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BPosSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMaxLimit[4],PARAFLOAT,0,6,0)
					:CSysPara_PutPara("B轴正软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMaxLimit[4],PARAFLOAT,0,6,0);

			g_bEnglish?CSysPara_PutPara("BNegSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMinLimit[4],PARAFLOAT,0,6,0)
					:CSysPara_PutPara("B轴负软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMinLimit[4],PARAFLOAT,0,6,0);

			g_bEnglish?CSysPara_PutPara("CPosSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMaxLimit[5],PARAFLOAT,0,6,0)
					:CSysPara_PutPara("C轴正软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMaxLimit[5],PARAFLOAT,0,6,0);

			g_bEnglish?CSysPara_PutPara("CNegSftLimit","UNIT:0.01mm(Screw);0.1°(Circle(s)).Range(-99999~999999).",&g_Ini.fMinLimit[5],PARAFLOAT,0,6,0)
					:CSysPara_PutPara("C轴负软限位","单位:0.01毫米(丝杆);0.1度(单圈,多圈).范围(-99999~999999).",&g_Ini.fMinLimit[5],PARAFLOAT,0,6,0);
		}

		g_bEnglish?CSysPara_PutPara("XHardLimit","Eable HardLimit Function of X-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X轴硬件限位","是否启用X轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[0],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("YHardLimit","Eable HardLimit Function of Y-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[1],PARA8U,0,1,0)
				:CSysPara_PutPara("Y轴硬件限位","是否启用Y轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[1],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZHardLimit","Eable HardLimit Function of Z-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z轴硬件限位","是否启用Z轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("AHardLimit","Eable HardLimit Function of A-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A轴硬件限位","是否启用A轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[3],PARA8U,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BHardLimit","Eable HardLimit Function of B-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B轴硬件限位","是否启用B轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("CHardLimit","Eable HardLimit Function of C-axis.0:No,1:Yes.",&g_Ini.iHardLimitSwitch[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C轴硬件限位","是否启用C轴的硬件限位检测功能.1:启用,0:不启用.",&g_Ini.iHardLimitSwitch[5],PARA8U,0,1,0);
		}


		g_bEnglish?CSysPara_PutPara("ZRunMode","RunMode of Z-axis.0:Circle,1:Circles,2:Screw.",&g_Ini.iAxisRunMode[2],PARAINT,0,1,0)
				:CSysPara_PutPara("Z轴方式","Z轴的运动类型.0-单圈方式,1-多圈方式,2-丝杆方式.",&g_Ini.iAxisRunMode[2],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("ARunMode","RunMode of A-axis.0:Circle,1:Circles,2:Screw.",&g_Ini.iAxisRunMode[3],PARAINT,0,1,0)
				:CSysPara_PutPara("A轴方式","A轴的运动类型.0-单圈方式,1-多圈方式,2-丝杆方式.",&g_Ini.iAxisRunMode[3],PARAINT,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BRunMode","RunMode of B-axis.0:Circle,1:Circles,2:Screw.",&g_Ini.iAxisRunMode[4],PARAINT,0,1,0)
					:CSysPara_PutPara("B轴方式","B轴的运动类型.0-单圈方式,1-多圈方式,2-丝杆方式.",&g_Ini.iAxisRunMode[4],PARAINT,0,1,0);

			g_bEnglish?CSysPara_PutPara("CRunMode","RunMode of C-axis.0:Circle,1:Circles,2:Screw.",&g_Ini.iAxisRunMode[5],PARAINT,0,1,0)
					:CSysPara_PutPara("C轴方式","C轴的运动类型.0-单圈方式,1-多圈方式,2-丝杆方式.",&g_Ini.iAxisRunMode[5],PARAINT,0,1,0);
		}

		g_bEnglish?CSysPara_PutPara("ZMulRunMode","RunMode of Z-axis.0:Relatively,1:Absolute.",&g_Ini.iAxisMulRunMode[2],PARAINT,0,1,0)
				:CSysPara_PutPara("Z轴多圈表达","当Z轴为多圈模式时,运行位置表达方式.0:相对,1:绝对",&g_Ini.iAxisMulRunMode[2],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("AMulRunMode","RunMode of A-axis.0:Relatively,1:Absolute.",&g_Ini.iAxisMulRunMode[3],PARAINT,0,1,0)
				:CSysPara_PutPara("A轴多圈表达","当A轴为多圈模式时,运行位置表达方式.0:相对,1:绝对",&g_Ini.iAxisMulRunMode[3],PARAINT,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("BMulRunMode","RunMode of B-axis.0:Relatively,1:Absolute.",&g_Ini.iAxisMulRunMode[4],PARAINT,0,1,0)
					:CSysPara_PutPara("B轴多圈表达","当B轴为多圈模式时,运行位置表达方式.0:相对,1:绝对",&g_Ini.iAxisMulRunMode[4],PARAINT,0,1,0);

			g_bEnglish?CSysPara_PutPara("CMulRunMode","RunMode of C-axis.0:Relatively,1:Absolute.",&g_Ini.iAxisMulRunMode[5],PARAINT,0,1,0)
					:CSysPara_PutPara("C轴多圈表达","当C轴为多圈模式时,运行位置表达方式.0:相对,1:绝对",&g_Ini.iAxisMulRunMode[5],PARAINT,0,1,0);
		}
		g_bEnglish?CSysPara_PutPara("XPhyAxis","X PhyAxis (0-4).",&g_Ini.iPhyAxis[0],PARAINT,0,1,0)
				:CSysPara_PutPara("X轴指定接口轴号","X轴指定脉冲DB头接口轴号(0-4)",&g_Ini.iPhyAxis[0],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("YPhyAxis","Y PhyAxis (0-4).",&g_Ini.iPhyAxis[1],PARAINT,0,1,0)
				:CSysPara_PutPara("Y轴指定接口轴号","Y轴指定脉冲DB头接口轴号(0-4)",&g_Ini.iPhyAxis[1],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("ZPhyAxis","Z PhyAxis (0-4).",&g_Ini.iPhyAxis[2],PARAINT,0,1,0)
				:CSysPara_PutPara("Z轴指定接口轴号","Z轴指定脉冲DB头接口轴号(0-4)",&g_Ini.iPhyAxis[2],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("APhyAxis","A PhyAxis (0-4).",&g_Ini.iPhyAxis[3],PARAINT,0,1,0)
				:CSysPara_PutPara("A轴指定接口轴号","A轴指定脉冲DB头接口轴号(0-4)",&g_Ini.iPhyAxis[3],PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("X Z order","axis zero order.",&g_Ini.iBackOrder[0],PARA8U,0,1,0)
				:CSysPara_PutPara("X轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[0],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("Y Z order","axis zero order.",&g_Ini.iBackOrder[1],PARA8U,0,1,0)
				:CSysPara_PutPara("Y轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[1],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("Z Z order","axis zero order.",&g_Ini.iBackOrder[2],PARA8U,0,1,0)
				:CSysPara_PutPara("Z轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[2],PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("A Z order","axis zero order.",&g_Ini.iBackOrder[3],PARA8U,0,1,0)
				:CSysPara_PutPara("A轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[3],PARA8U,0,1,0);

		if(g_Ini.iAxisCount>=5)
		{
			g_bEnglish?CSysPara_PutPara("B Z order","axis zero order.",&g_Ini.iBackOrder[4],PARA8U,0,1,0)
					:CSysPara_PutPara("B轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[4],PARA8U,0,1,0);

			g_bEnglish?CSysPara_PutPara("C Z order","axis zero order.",&g_Ini.iBackOrder[5],PARA8U,0,1,0)
					:CSysPara_PutPara("C轴归零顺序","各轴的归零顺.范围(1-6).",&g_Ini.iBackOrder[5],PARA8U,0,1,0);
		}

		g_bEnglish?CSysPara_PutPara("AxisCount","Numbers of Axis Used In System.Range(2-4).",&g_Ini.iAxisCount,PARAINT,0,1,0)
				:CSysPara_PutPara("系统轴数","该系统使用的轴数.单位:个.范围(2-4).",&g_Ini.iAxisCount,PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("MachineType","0:820,1:428,2:335,3:625,4:635,5:650,6:TK335",&g_Ini.iMachineType,PARAINT,0,2,0)
				:CSysPara_PutPara("机器型号","0:820,1:428,2:335,3:625,4:635,5:650,6:TK335",&g_Ini.iMachineType,PARAINT,0,2,0);


		g_bEnglish?CSysPara_PutPara("XMaxSpeed","Max X-Speed In Command S.UNIT:r/min.Range(1-500).",&g_Ini.fXMaxSpeed,PARAFLOAT,1,6,0)
				:CSysPara_PutPara("产速限定","S指令X轴最大速度.单位:转/分.范围(1-500).",&g_Ini.fXMaxSpeed,PARAFLOAT,1,6,0);

		g_bEnglish?CSysPara_PutPara("ControlMode","Control Mode of Secondary-Axis.0:Continuation,1:Acc/Dec.",&g_Ini.iSecondAxisStyle,PARAINT,0,1,0)
				:CSysPara_PutPara("运控方式","从轴运动方式.0-连续,1-加减速.单位:无.范围(0-1).",&g_Ini.iSecondAxisStyle,PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("CylinderMode","CylinderMode In Col-Cylinder.0:01Sequence,1:Digit.",&g_Ini.iCyldMode,PARA8U,0,1,0)
				:CSysPara_PutPara("气缸模式","气缸表达方式:0:0-02序列 2表示保持气缸 1:正负编号",&g_Ini.iCyldMode,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("CylinderAsSetOrigin","0:Cylinder,1:SetOrigin.",&g_Ini.iCyldOrOrigin,PARA8U,0,1,0)
				:CSysPara_PutPara("气缸设起点","气缸按键当作设起点按键.0-气缸.1-设起点.",&g_Ini.iCyldOrOrigin,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("LeftOrRight","0:Left,1:Right.",&g_Ini.iLeftOrRight,PARA8U,0,1,0)
				:CSysPara_PutPara("左旋右旋","Z轴与A轴名字切换.0-左旋.1-右旋.",&g_Ini.iLeftOrRight,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("FixRate","Fix-YLen/ZRotate.UNIT:1/Million.Range(0-Million).",&g_Ini.lRotatorRate,PARALONG,0,7,0)
				:CSysPara_PutPara("转动比率","Z轴转动时Y轴移动距离.单位:N/百万.N范围(0-百万).",&g_Ini.lRotatorRate,PARALONG,0,7,0);

		g_bEnglish?CSysPara_PutPara("HandCount","Wheel Detection Frequency,Default:2.Range(1-10).",&g_Ini.iHandRunSpeed,PARAINT,0,2,0)
				:CSysPara_PutPara("手摇速度","手摇轮的检测速度,默认2.单位:次.范围(1-10).",&g_Ini.iHandRunSpeed,PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("HandRate","Magnification Times of WheelSpeed.Default:75.Range(1-1000).",&g_Ini.fHandRunRate,PARAFLOAT,0,4,0)
				:CSysPara_PutPara("手摇速率","手摇轮速度放大倍率,默认75.单位:倍.范围(1-1000).",&g_Ini.fHandRunRate,PARAFLOAT,0,4,0);

		g_bEnglish?CSysPara_PutPara("FaceStyle","0: default, 1: blue, 2: black.",&g_Ini.iColorStyle,PARAINT,0,2,0)
				:CSysPara_PutPara("界面样式","0:默认,1:蓝色,2:黑色.",&g_Ini.iColorStyle,PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("Language","0:Chinese,1:English.",&g_Ini.iLanguage,PARAINT,0,2,0)
				:CSysPara_PutPara("语言选择","0:简体中文,1:英语.",&g_Ini.iLanguage,PARAINT,0,2,0);

		g_bEnglish?CSysPara_PutPara("SysPassword","Password for Entering System Parameter Setting.",&g_Ini.lPassword,PARALONG,0,6,1)
				:CSysPara_PutPara("系统密码","进入系统参数界面的密码.",&g_Ini.lPassword,PARALONG,0,6,1);

		g_bEnglish?CSysPara_PutPara("Pswd Time","0 disenable",&g_Ini.iPasswordTime,PARAINT,0,4,0)
				:CSysPara_PutPara("编辑密码间隔","单位:秒,范围:0-1000分钟",&g_Ini.iPasswordTime,PARAINT,0,4,0);

		g_bEnglish?CSysPara_PutPara("IO Send","IN5,IN6 Send Wire enable,0-disable;1-enable.",&g_Ini.iIOSendWire,PARAINT,0,1,0)
				:CSysPara_PutPara("IO送线使能","IN5,IN6送线使能,0-无效;1-有效.",&g_Ini.iIOSendWire,PARAINT,0,1,0);


		g_bEnglish?CSysPara_PutPara("IO single","0-disable;1-enable. Port: probe 7",&g_Ini.iSwhSingleIo,PARAINT,0,1,0)
				:CSysPara_PutPara("IO单条使能","0关闭,1启用.端口:探针7"	,	&g_Ini.iSwhSingleIo				,PARAINT,0,1,0);

		g_bEnglish?CSysPara_PutPara("SafeDoor","0:No 1:Often Open 2:Often Closed, Port: Probe7",&g_Ini.iSafeDoor		    ,PARAINT,0,2,0)
				:CSysPara_PutPara("安全门检测","0:不检测 1:常开 2:常闭, 端口号:探针7",&g_Ini.iSafeDoor		    ,PARAINT,0,2,0);

		//add 2020/4/7
		g_bEnglish?CSysPara_PutPara("Kink IN Port Config","default 21 in7-19,255-Disable.",&g_Ini.iIOKinkConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("缠线输入端口","默认21 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOKinkConfig_IN,PARA8U,0,3,0);
		g_bEnglish?CSysPara_PutPara("Kink IN Level Config","default 0 0-Low 1-High.",&g_Ini.iIOKinkConfig_Level,PARA8U,0,1,0)
				:CSysPara_PutPara("缠线检测电平","默认0 0-常开 1-常闭",&g_Ini.iIOKinkConfig_Level,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("BreakLine IN Port Config","default 20 in6-13,255-Disable.",&g_Ini.iIOBreakLineConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("断线输入端口","默认20 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOBreakLineConfig_IN,PARA8U,0,3,0);
		g_bEnglish?CSysPara_PutPara("BreakLine IN Level Config","default 1 0-Low 1-High.",&g_Ini.iIOBreakLineConfig_Level,PARA8U,0,1,0)
				:CSysPara_PutPara("断线检测电平","默认1 0-常开 1-常闭",&g_Ini.iIOBreakLineConfig_Level,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("RunLine IN Port Config","default 18 in5-14,255-Disable.",&g_Ini.iIORunLineConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("跑线输入端口","默认18 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIORunLineConfig_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("SafeDoor IN Port Config","default 19 0-24,255-Disable.",&g_Ini.iIOSafeDoorConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("安全门输入端口","默认19 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOSafeDoorConfig_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("WireAcc OUT Port Config","default 7 0-out1 1-out2 9-out8,255-Disable.",&g_Ini.iIOWireAccConfig_OUT,PARA8U,0,3,0)
				:CSysPara_PutPara("线架加速输出端口","默认7 0-OUT1 1-OUT2...7-OUT8 255 无效.",&g_Ini.iIOWireAccConfig_OUT,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("Sart IN Port Config","default 62 0-24,255-Disable.",&g_Ini.iIOSart_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("外部启动输入端口","默认62 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOSart_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("Single IN Port Config","default 65 0-24,255-Disable.",&g_Ini.iIOSingleConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("单条输入端口","默认65 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOSingleConfig_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("Alarm OUT Port Config","default 9 0-24,255-Disable.",&g_Ini.iIOAlarmConfig_OUT,PARA8U,0,3,0)
				:CSysPara_PutPara("报警输出端口","默认6 0-OUT1 1-OUT2...7-OUT8 255 无效.",&g_Ini.iIOAlarmConfig_OUT,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("SendLine+ IN Port Config","default 14 0-24,255-Disable.",&g_Ini.iIOSendLinePConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("送线+输入端口","默认14 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOSendLinePConfig_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("SendLine- IN Port Config","default 13 0-24,255-Disable.",&g_Ini.iIOSendLineMConfig_IN,PARA8U,0,3,0)
				:CSysPara_PutPara("送线-输入端口","默认13 21缠线,20断线,18-IN8,19-IN7,13-IN6,14-IN5 255 无效.",&g_Ini.iIOSendLineMConfig_IN,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("Special Test","default 0 0-NO,1-YES.",&g_Ini.iTestFuction,PARA8U,0,3,0)
				:CSysPara_PutPara("特殊测试按钮","默认0 0-禁用 1-开启 每次按凸轮加减5%的速度.",&g_Ini.iTestFuction,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("A-axis compensation","0-6 axis.",&g_Ini.iWorkAxisNo,PARA8U,0,3,0)
				:CSysPara_PutPara("A轴补偿轴号","0-6轴 0-禁用 1-6补偿轴号",&g_Ini.iWorkAxisNo,PARA8U,0,3,0);

		g_bEnglish?CSysPara_PutPara("Compensation molecule","0-999999.",&g_Ini.iWorkAxisTMolecular,PARALONG,0,3,0)
				:CSysPara_PutPara("补偿分子","0-999999",&g_Ini.iWorkAxisTMolecular,PARALONG,0,3,0);

		g_bEnglish?CSysPara_PutPara("Wait Signal","0-disable 1-8 Pro Num.",&g_Ini.iWaitSignal,PARA8U,0,1,0)
				:CSysPara_PutPara("输入等待","配置探针端口做等待信号 0:无效 1-8Pro端口",&g_Ini.iWaitSignal,PARA8U,0,1,0);

		g_bEnglish?CSysPara_PutPara("Probe Delay"," Delay time s.",&g_Ini.iProDelay,PARAINT,0,6,0)
				:CSysPara_PutPara("探针等待时间","探针为输入等待时候的延时时间ms.",&g_Ini.iProDelay,PARAINT,0,6,0);

		g_bEnglish?CSysPara_PutPara("Pro1 IN Port Config","default 1 Set Num 1-8.",&g_Ini.iSetProNum[0],PARA8U,0,1,0)
				:CSysPara_PutPara("探针1端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[0],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro2 IN Port Config","default 2 Set Num 1-8.",&g_Ini.iSetProNum[1],PARA8U,0,1,0)
				:CSysPara_PutPara("探针2端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[1],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro3 IN Port Config","default 3 Set Num 1-8.",&g_Ini.iSetProNum[2],PARA8U,0,1,0)
				:CSysPara_PutPara("探针3端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[2],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro4 IN Port Config","default 4 Set Num 1-8.",&g_Ini.iSetProNum[3],PARA8U,0,3,0)
				:CSysPara_PutPara("探针4端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[3],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro5 IN Port Config","default 5 Set Num 1-8.",&g_Ini.iSetProNum[4],PARA8U,0,3,0)
				:CSysPara_PutPara("探针5端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[4],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro6 IN Port Config","default 6 Set Num 1-8.",&g_Ini.iSetProNum[5],PARA8U,0,3,0)
				:CSysPara_PutPara("探针6端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[5],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro7 IN Port Config","default 7 Set Num 1-8.",&g_Ini.iSetProNum[6],PARA8U,0,1,0)
				:CSysPara_PutPara("探针7端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[6],PARA8U,0,1,0);
		g_bEnglish?CSysPara_PutPara("Pro8 IN Port Config","default 8 Set Num 1-8.",&g_Ini.iSetProNum[7],PARA8U,0,1,0)
				:CSysPara_PutPara("探针8端口映射","端口号探针端口号1-8的映射对应板子上的 pro 1-8.",&g_Ini.iSetProNum[7],PARA8U,0,1,0);



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
		g_bEnglish?CDC_OutText(x,y,"C",BLACKCOLOR,GRAYCOLOR)
				:CDC_OutText(x,y,"缸",BLACKCOLOR,GRAYCOLOR);
	}
	else if(iCliyer==1)
	{
		g_bEnglish?CDC_OutText(x,y,"C",BLACKCOLOR,REDCOLOR)
				:CDC_OutText(x,y,"缸",BLACKCOLOR,REDCOLOR);
	}

	if(NumStatus ==0)
	{//设置几个气缸显示
		for(i=0;i<7;i++)
		{		
			sprintf(buf,"%d",i+1);
			CDC_OutText(x+90+i*113,y,buf,BLACKCOLOR,GRAYCOLOR);
		}
	}
	else if(NumStatus>0 && NumStatus<3)
	{
		sprintf(buf,"%d",NumStatus);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,REDCOLOR,GRAYCOLOR);
		OSTimeDly(100);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,BLACKCOLOR,GRAYCOLOR);
	}
}



void	CSysPara_Message(INT8S  *ParaInfo)
{
	CRect rect;

	rect.left	= g_iDlgLeft+7;
	rect.top	= g_iDlgTop+g_iDlgHeight-75;
	rect.right	= g_iDlgLeft+g_iDlgWidth-7;
	rect.bottom = g_iDlgTop+g_iDlgHeight-75+30;

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
		CDC_OutText_UTF(rect.left+3,rect.top+2,ParaInfo,BLACKCOLOR,GRAYCOLOR,KZK_22);
	}
}



INT32S 	CSysPara_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* SysPara,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD)
{
	if(s_iSysParaCount>=SYSPARAMAX)
		return FALSE;

	sprintf(s_SysParaName[s_iSysParaCount],"%03d, ",s_iSysParaCount+1);
	strcat(s_SysParaName[s_iSysParaCount],ParaName);

	sprintf(s_SysParaInfo[s_iSysParaCount],"%03d, ",s_iSysParaCount+1);
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
			CDC_PutColorBar(g_iDlgLeft+8+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),266+120,28,DARKGRAYCOLOR,1);
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
			CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],BLACKCOLOR,LIGHTBLUECOLOR,KZK_24);
			CSysPara_Message(s_SysParaInfo[iStartLine+j]);
		}
		else
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(WHITECOLOR);

			//	CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),266,26,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,1);
			//	CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],SKYBLUECOLOR,j%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);

			CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),266,26,LIGHTGRAYCOLOR,1);
			CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(j/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(j%SYSPARAS_COL),s_SysParaName[iStartLine+j],SKYBLUECOLOR,LIGHTGRAYCOLOR,KZK_24);
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
		CEdit_DrawActive();
		return;
	}

	if(s_StartLine+s_CurLine+dk< 0)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);
		CEdit_DrawActive();
		return;
	}

	//	CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),266,26,s_CurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,1);
	//	CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],SKYBLUECOLOR,s_CurLine%2?LIGHTGRAYCOLOR:LIGHTWHITECOLOR,MZK_24);

	CDC_PutColorBar(g_iDlgLeft+10+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),266,26,LIGHTGRAYCOLOR,1);
	CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],SKYBLUECOLOR,LIGHTGRAYCOLOR,KZK_24);


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
		CDC_OutText_UTF(g_iDlgLeft+12+ SYSWOFFSET*(s_CurLine/SYSPARAS_COL) ,g_iDlgTop+41+SYSHOFFSET*(s_CurLine%SYSPARAS_COL),s_SysParaName[s_StartLine+s_CurLine],BLACKCOLOR,LIGHTBLUECOLOR,KZK_24);

		CSysPara_Message(s_SysParaInfo[s_StartLine+s_CurLine]);

		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_DrawActive();
	}
	/*	else
	{
		CButton_SetCtrl(ptr);
		CButton_DrawItem(Foucs);
	}*/

}


void	CSysPara_OnButtonOk()
{
	{//归零顺序不能有重复

		INT16S tt,kk;

		for(tt=0;tt<g_Ini.iAxisCount;tt++)
		{
			for(kk=tt+1;kk<g_Ini.iAxisCount;kk++)
			{
				if(g_Ini.iBackOrder[tt] == g_Ini.iBackOrder[kk])
				{
					CSysPara_Message(g_bEnglish?"BackOrder Cannot be Repetition!!":"错误: 归零顺序有重复，请重新修改归零顺序!!!!!!");
					return ;
				}
			}
		}
	}

	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);

	//InitBoard();
	InitBoardPara();

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

void	CSysPara_OnButtonInit()
{
	SysParaInit();
	SysParaWrite();
	WriteDataToFile(g_Ini.iFileName);
	OSTimeDly(500);
	CSysPara_UpdateView(s_StartLine);
	CSysPara_Message(g_bEnglish?"Init Success":"初始化成功!");
}

void CSysPara_OnButtonReadBackup()
{
	WriteDataToFile(g_Ini.iFileName);

	if(SysParaRead())
	{
		SysParaWrite();
		CSysPara_Message(g_bEnglish?"Restore Success":"还原成功!");
		OSTimeDly(500);

		CSysPara_UpdateView(s_StartLine);
		g_bSysReadBackup = TRUE;
	}
	else
	{
		CSysPara_Message(g_bEnglish?"Restore Failure":"还原失败!");
	}

}

void	CSysPara_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;

	ptr=CMainFrm_GetItemID(0x9000+s_CurLine);
	CEdit_SetCtrl(ptr);

	g_bInsertNum = FALSE;
	g_bClearNum = FALSE;


	if(key!=TCYLINDER && (key<'1' || key>'7'))
	{
		s_bCylinder = FALSE;
		LedOut(CYLINDERLED,s_bCylinder?1:0);
		CSysPara_DrawButtonInfo(s_bCylinder,-1);
	}

	switch(key)
	{
	case TCYLINDER:

		s_bCylinder =s_bCylinder?FALSE:TRUE;

		LedOut(CYLINDERLED,s_bCylinder?1:0);
		CSysPara_DrawButtonInfo(s_bCylinder,-1);

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

	case TENTER:
	{
		CSysPara_UpdateObject(1,TRUE);
		break;
	}


	case '0':case '1':case '2':case '3':case '4':case '5':
	case '6':case '7':
	case '8':case '9':case '.':case '-':

		if(s_bCylinder)
		{

			if(key!='1' && key!='2')
			{
				s_bCylinder=s_bCylinder?FALSE:TRUE;
				LedOut(CYLINDERLED,s_bCylinder?1:0);
				CSysPara_DrawButtonInfo(s_bCylinder,-1);
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

		//	if(s_CurLine<s_iSysParaCount-2)
		{
			g_bInsertNum = TRUE;
			CEdit_OnKey(key,tkey);
		}

		break;
	case TPAGEUP:           //上页
	{
		CSysPara_UpdateObject( -s_CurLine-1,FALSE);
	}
	break;

	case TPAGEDOWN:           //下页
	{
		CSysPara_UpdateObject( SYSPARAS_COL*2-s_CurLine,FALSE);
	}
	break;
	case TSAVE:           //储存
	{
		CSysPara_OnButtonOk();
	}
	break;
	case TBACKSPACE:
		CEdit_OnKey(key,tkey);
		break;
		/*case BACK:
				g_bInsertNum = TRUE;
				CEdit_OnKey(key,tkey);
			break;
		 */
		/*case TBACKSPACE:
			if(g_bInsertClearNum)
				g_bInsertNum = TRUE;

			g_bClearNum=TRUE;

			CEdit_OnKey(key,tkey);
			break;	*/


#ifdef	PRINTBMP
	case F5:
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
		CreatBMP(filename,800,600,1);

		OSQPost(MsgQueue,"Write Bmp End!");
	}

	break;
#endif

	default:break;
	}
}


INT16S    CSysPara_DoModal()
{
	INT16U key=0,tkey=0;

	g_iParameterEdit = 1;

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
	g_iParameterEdit = 0;

	return g_iDlgIDOk;
}


