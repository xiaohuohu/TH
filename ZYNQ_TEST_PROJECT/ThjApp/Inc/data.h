# ifndef	___DATA_H___
# define	___DATA_H___
# include	"os_cpu.h"
//#include "rtcapi.h"

//# include	"dos.h"
# define	SPEED		0x10
# define	MOVE		0x11
# define	PEND		0x12
# define	JUMP		0x13
# define	GOTO		0x14
//# define  AUTO      	0x15
//# define  HOMEBACK  	0x16

//2M 
# define	XMSSIZE		(INT32U)(2*1024*1024)
# define	DATASIZE	(INT32U)(2*1024*1024+1)

//74K左右的24点阵西文字库
   
#define		PAI			3.1415926
#define		MULTIPLIER	5
#define		DIVISOR		3
#define		MAXMOD		99999989

#define		MAXLINE		200

#define		MAXVALUECOL	12
#define		MAXCYLINDER	8
#define		MAXCH		6//4


/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/

typedef struct
{
  INT8S	value[MAXVALUECOL][9];
}COMMAND;


typedef struct
{
	INT32S 	lPulse[MAXCH];				//每转脉冲
	FP32	fMM[MAXCH];					//每转行程
	INT32S	lMaxSpeed[MAXCH];			//电机速度
	INT32S	lHandSpeed[MAXCH];			//手动速度（参数界面未见）			修正
	FP32	fBackSpeed[MAXCH];			//回原点速度
	FP32	lGearRate[MAXCH];			//减速比
	
	INT8U 	iRunDir[MAXCH];				//运行方向
	INT8U	iBackDir[MAXCH];			//回原点方向
	INT32S	lRunCount;					//加工产量
	INT32S	lDestCount;					//目标产量
	INT32S	lProbeCount;				//探针次数
	INT32S	lPassword;					//登陆参数密码
	
	FP32	fXMaxSpeed;					//X轴最大速度，产速限定
	FP32	fStartSpeed;				//起始速度   （参数界面未见，当前功能未使用到）
	FP32	fAddSpeed;					//加速度	  (参数界面未见，当前功能未使用到）
	
	
	INT16S	iScrProtect;				//屏保选择
	INT16S	iScrProtectTime;			//屏保时间
	INT16S	iScrProtectBmp;				//屏保图片
	
	INT16S	iHandStatus;				//指示手轮状态
	INT16S 	iHandRunSpeed;				//手摇速度
	FP32    fHandRunRate;				//手摇速率
	INT8U	iBackOrder[MAXCH];			//归零顺序

	INT16S 	iPaoXian; 					//跑线检测（1检测，0不检测）			
	INT16S  iWarningOut;				//报警输出（1输出，0不输出）
	INT16S  iFileName;					//料号，加工文件名
	INT16S  iAxisCount;					//系统轴数
	INT16S	iYDaoXian;					//Y轴是否可倒线（参数未开放）		修正
	
	INT8U   iAxisSwitch[MAXCH];			//各轴开关选择

	INT16S  iLanguage; 					//语言选择
	
	INT8U	iServoLogic[MAXCH];			//伺服报警逻辑 （0:低电平有效  1:高电平有效）

	INT8U	iSearchZero[MAXCH];			//搜索零点方式（0：不归零，1：搜索机械零点，2：搜索伺服零点，3：搜索机械和伺服零点）

	INT16S  iAxisRunMode[MAXCH];		//轴运行模式；(0:单圈，1:多圈，2:丝杠，3:送线)
	
	FP32	fVoltageOne;				//模拟电压输出1（0-10V）暂时（未用）
	FP32	fVoltageTwo;				//模拟电压输出2（0-10V）暂时（未用）
	
	INT16S	iBackMode[MAXCH];			//加工完回零模式
	INT32S  lRunSpeed[MAXCH];			//各轴的运行速度	以由S指令代替	(未用)
	INT16S	iStepDis[MAXCH];			//寸动距离
	INT16S	iStepRate[MAXCH];			//手摇倍率
	
	FP32	fMaxLimit[MAXCH];			//软件正向限位
	FP32	fMinLimit[MAXCH];			//软件负向限位
	
	FP32	fStartOffset[MAXCH];		//指示各轴原点位置相对于原点开关的偏移值,单位:0.1度(角度), 0.1毫米(长度)

	INT32S	lChAddSpeed[MAXCH];			//各轴加工加速度
	
	INT16S	iSecondAxisStyle;			//从轴连续方式，0为连续，1为不连续
	
	INT32S	iLag[MAXCH];				//实际位置滞后逻辑位置的最大允许脉冲 轴滞后量
	INT32S  iNumerator[MAXCH];			//实际脉冲比，脉冲比:实际位置/逻辑位置
	INT32S	iDenominator[MAXCH];		//逻辑脉冲比，脉冲比:实际位置/逻辑位置
	
	INT16S	iEnterNumber;				//按记忆键是否将坐标的显示输入到数据显示

	INT16S	iDispLeavingTime;			//显示剩余的加工时间

	INT16S	iXianjiaDrive;				//指示是否启用线架驱动功能

	INT32S	lRotatorRate;				//转线轴跟送线轴的比率参数  Y/A=lRotatorRate
	
	INT16S  iColorStyle;				//系统颜色样式
	
	INT8U   iCyldMode;					//气缸模式
	
	INT16S  iAxisNameNo[MAXCH];			//轴名字编号,0默认,1送线,2转线,3转芯,4节距,5芯刀,6卷取,7夹耳,8切刀,9上切,10下切
	
	INT8U 	iHardLimitSwitch[MAXCH];	//是否开启硬件限位检测：1-开启硬件限位检测  0-关闭硬件限位检测
	
	INT8U 	iCyldOrOrigin;				//气缸按键模式（0：气缸按键，1：设起点按键）
	
	INT8U	iLeftOrRight;				//左旋右旋功能：第三轴与第四轴轴名切换（0：左旋，1：右旋）
	
	INT16S  iAxisMulRunMode[MAXCH];		//1:多圈数据相对绝对表达方式(0相对.，1绝对)
	INT16S  iMachineType;				//系统型号编号
	
	INT16S  iIOSendWire;				//送线使能

	INT16S 	iSwhSingleIo;				//单条输入点开关	探针5
	INT16S 	iSwhRunIo;					//自动输入点开关。  探针7
	INT16S  iPhyAxis[MAXCH];			//设置轴的物理端口

	INT16S  iPasswordTime;				//修改需要密码的时间
	INT16S	iSafeDoor;					//安全门0无效 1低电平有效（常开）    2高电平有效（常闭）

	INT8U 	iIOKinkConfig_IN;				//缠线报警端口(0-24,255禁用)
	INT8U 	iIOKinkConfig_Level;			//缠线报警电平(0:低电平有效  1:高电平有效)
	INT8U 	iIOBreakLineConfig_IN;		//断线报警端口(0-24,255禁用)
	INT8U 	iIOBreakLineConfig_Level;		//断线报警电平(0:低电平有效  1:高电平有效)
	INT8U 	iIORunLineConfig_IN;			//跑线报警端口(0-24,255禁用)
	INT8U 	iIOSafeDoorConfig_IN;			//安全门输入端口(0-24,255禁用)
	INT8U 	iIOWireAccConfig_OUT;			//线架加速输出端口(0-24,255禁用)
	INT8U 	iIOSart_IN;					//外部启动输入端口(0-24,255禁用)
	INT8U 	iIOSingleConfig_IN;			//外部单条输入端口(0-24,255禁用)
	INT8U 	iIOAlarmConfig_OUT;			//报警输出端口(0-24,255禁用)
	INT8U 	iIOSendLinePConfig_IN;		//送线+输入端口(0-24,255禁用)
	INT8U 	iIOSendLineMConfig_IN;		//送线-输入端口(0-24,255禁用)
	INT8U 	iTestFuction;					//特殊测试按键(0-禁用,1-启用)
	INT8U 	iWorkAxisNo;				//A轴补偿轴号(0-6轴 0-禁用 1-6补偿轴号 )
	INT32S  iWorkAxisTMolecular;		//补偿分子(0-999999)
	INT8U	iSetProNum[8];			    //设置端口号探针端口号1-8的映射对应板子上的 pro 1-8
	INT8U	iWaitSignal;			    //配置探针端口做等待信号 pro 1-8
	INT32S  iProDelay;					//探针为输入等待时候的延时时间ms
	
//	INT16S  iAxisZero[MAXCH];			//设置轴的回零端口
}BASEINI;


typedef struct
{
	INT32S	Dog_InitData;   						//狗的初始化
	INT32S  First_Use;  							//设定加密狗后第一次开机后开始加密狗计数。

	INT32S  Adtech_ComputerNo;						//1)	公司级客户电脑编号
	INT32S	Adtech_Days;							//2)	公司级限定天数
	INT32S	Adtech_Password;						//3)	公司级软件狗设定登入密码

	INT32S	UserDogIdentifier;						//4)	用户级软件狗标别码
	
	INT32S	User_ComputerNo;						//5)	用户级客户机器编号
	INT32S	User_Days;								//6)	用户级限定天数
	INT32S	User_Password;							//7)	用户级软件狗设定登入密码

	INT32S	Adtech_Active;  						//指示是否解除公司级密码限制,12345678解除限制
	DATE_T  Adtech_LastDay; 						//公司级最后一次访问软件狗的日期
	TIME_T  Adtech_NowTime; 						//记录用户级访问软件狗的当前时间

	INT32S	User_Active;     						//指示是否解除用户级密码限制,12345678解除限制
	DATE_T 	User_LastDay; 							//用户级最后一次访问软件狗的日期
	TIME_T  User_NowTime; 							//记录用户级访问软件狗的当前时间
	
}NEWDOG;


typedef struct
{
	INT16U	nLine;
	INT8U	nOUT;
	INT8U	nAxis;
	INT32S	lValue[2];
}OUTDATA;

typedef struct
{
	INT32S	outPos[4];
	INT8U	nOUT;
	INT8U	nEnable;
}TEMPOUTDATA;

#define SILVERCOLOR     0xEDEDED
#define LIGHTWHITECOLOR 0xF4F4F4
#define LIGHTBLUECOLOR2 0x4682B4


#define 	MZK_24	5      	//生成字库的存储区域（2--9）

#define		KZK_22	7
#define		KZK_24	8
#define 	KZK_32	9      	//生成字库的存储区域（2--9）
/*
typedef struct
{
	INT16U	nLine;
	INT8U 	nCode;
	INT32S	lValue[5];
	INT8U	nIO;
	INT8U	nOUT[MAXCYLINDER];

}UNITDATA;
*/


typedef struct
{
	INT32S	nLine;							//行号
	INT8U 	nCode;							//指令		
	INT8U	SpindleAxis	;					//有主轴，1为X轴，2为Y，3为Z，4为A轴			
	INT32S	lValue[7];						//v0-v5为X,Y,Z,A,B,C轴，v6为速度
	INT8U	nIO;							//探针
	INT8U	nOut[MAXCYLINDER];				//气缸
//	INT8U	nOutAxis;						//参考轴
//	INT32S	lOutValue[2];					//缸开始，缸结束。
	INT8U 	nReset;							//回零标示。二进制按位表示1111，0000
	FP32	fDelay;							//

}UNITDATA;



void	DataCls(INT16S i);
void	DataPut(INT16S i,INT16S j,INT8S *str);
void	SysParaInit(void);

BOOL	SysParaRead(void);
void	SysParaWrite(void);

//BOOL	SysParaReadYaffs(void);
//void	SysParaWriteYaffs(void);


BOOL	SysParaCheck(void);

void	ReadRunCount(void);
void	WriteRunCount(void);
//void	ReadRunCountYaffs(void);
//void	WriteRunCountYaffs(void);


void	SetUnitData(INT32S nL,INT8U nC);

void	WriteDataToFile( INT16S File );
BOOL	ReadDataFromFile(INT16S File );
//INT16S 	YaffsFileExist(INT16S File);
INT16S FileExist(INT16S File);								//判断料号为File的文件是否存在

//void	WriteDataToYaffsFile( INT16S File );
//BOOL	ReadDataFromYaffsFile(INT16S File );


//BOOL	ReadFileMessage(INT16S File ,INT8S *message );
//INT16U	ReadPortb(INT16U port);
//void	WritePortb(INT16U port,INT32U data);
BOOL	CheckRam(void);
INT16U 	XMSToMEMEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);
INT16U 	MEMToXMSEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);
//void	InitMem();
//void	DestroyMem();
//void	InitXMS(void);
//void	DestroyXMS(void);
void 	WritePosToRam(INT16S ch, INT32S pos);
INT32S 	ReadPosFromRam(INT16S ch);

void 	WriteCountToRam(void);
void 	ReadCountFromRam(void);


# endif
