# ifndef	___DATA_H___
# define	___DATA_H___

#include 	"includes.h"
# include	"os_cpu.h"
//# include   "rtcapi.h"
//# include	"dos.h"

# define	SPEED		0x10
# define	MOVE		0x11
# define	PEND		0x12
# define	JUMP		0x13
# define	GOTO		0x14
# define	LOOP		0x15
//# define  AUTO      	0x15
//# define  HOMEBACK  	0x16

										//3M 
# define	XMSSIZE		(INT32U)(2*1024*1024)
# define	DATASIZE	(INT32U)(2*1024*1024+1)

										//74K左右的24点阵西文字库
   
#define		PAI			3.1415926
#define		MULTIPLIER	5
#define		DIVISOR		3
#define		MAXMOD		99999989
#define		MAXLINE		200
#define		MAXVALUECOL	8
#define		MAXCYLINDER	4
#define		MAXCH		4

#define 	MZK_32		9      			//生成字库的存储区域（2--9）
#define 	MZK_24		8
#define 	MZK_16		7
#define 	YH_32		6

typedef struct
{
  INT8S	value[MAXVALUECOL][9];
}COMMAND;

typedef struct
{
	INT32S 	lPulse[MAXCH];				//每转脉冲
	FP32	fMM[MAXCH];					//每转行程
	INT32S	lMaxSpeed[MAXCH];			//电机最大转速限制
	FP32	fHandSpeed[MAXCH];			//手动速度
	FP32	fBackSpeed[MAXCH];          //回原点速度
	FP32	fGearRate[MAXCH];			//减速比
	FP32	fStartOffset[MAXCH];		//指示各轴原点位置相对于原点开关的偏移值
	INT8U 	iRunDir[MAXCH];				//运行方向
	INT8U	iBackDir[MAXCH];			//回零点方向
	INT16S	iBackMode[MAXCH];			//加工完回零模式    0就近归零  1正向归零  2负向归零 3不回零
	INT32S	lRunCount;                  //加工数量
	INT32S	lDestCount;					//目标产量
	INT32S	lProbeCount;				//探针失败次数设置
	INT32S	lPassword;					//进入系统参数和常用参数的密码
	FP32	fXMaxSpeed;					//主轴最大加工速度   单位：R/Min
	FP32	fStartSpeed;				//起始速度		
	//INT32S	lAddSpeed;					//加速度
	//INT16S	iAlarmLogic;			//报警逻辑        无用	
	INT16S	iScrProtect;				//是否开启屏保				（0--不开启屏保，1--开启屏保）
	INT16S	iScrProtectTime;			//屏保开启时间  			单位：分钟
	INT16S	iScrProtectBmp;				//是否开启图片屏保			（0--不开启屏保，1--开启屏保）
	INT16S	iXianjiaDrive;				//指示是否启用线架驱动功能	（0--不驱动，1--驱动）
	//INT16S	iHandStatus;				//指示手轮状态               该参数未出现在参数设置界面  （未用）
	INT16S 	iHandRunSpeed;				//手摇速度
	FP32    fHandRunRate;				//手摇倍率
	INT16S	iReverseDly;			//正反延时比     无用参数（在参数设置界面去除）20140807
	INT16S 	iPaoXian; 					//跑线检测（1检测，0不检测）	
	//INT16S	iSpeedDly;  			//速度延时比	 无用参数（在参数设置界面去除）20140807
	INT16S  iWarningOut;				//报警输出（1输出，0不输出）
	INT16S  iFileName;					//料号，加工文件名
	INT16S  iAxisCount;					//系统轴数
	//INT16S	iSafeDoor;					//安全门检测（1检测，0不检测） 该参数未使用，是否注释掉？
	INT16S	iYDaoXian;					//Y轴手摇是否可倒线
	INT16S	iProbeAheadTime;			//探针前延时  单位：ms
	INT16S	iProbeLagTime;				//探针后延时  单位：ms
	//INT32S	lModify;					//登记系统密码					（无用）	
	//FP32    fBackSpeed1[2];			//回零低速，可不用，回零低速一般用1/10的回零速度（无用） 修正
	INT16S  iLanguage; 					//语言选择
	//INT16S  iUDriver;					//U轴为送线轴（无用）			修正
	/*INT8U	iServoLogicX;				//X伺服报警逻辑 （0:低电平有效  1:高电平有效）			 
	INT8U	iServoLogicY;
	INT8U	iServoLogicZ;
	INT8U	iServoLogicA;*/
	
	INT8U	iServoLogic[MAXCH];			//伺服报警逻辑 （0:低电平有效  1:高电平有效）
	INT8U	iServoZero[MAXCH];			//归零时是否寻找伺服零点(0:否  1:是)
	INT16S  iAxisRunMode[MAXCH];		//轴运行模式；(0:单圈，1:多圈，2:丝杠，3:送线)
	/*INT8U	iServoZeroX;				//归零时是否寻找伺服零点(0:否  1:是)
	INT8U	iServoZeroZ;
	INT8U	iServoZeroA;*/
	
	
	FP32	fVoltageOne;				//模拟电压输出1（0-10V）暂时（未用）
	FP32	fVoltageTwo;				//模拟电压输出2（0-10V）暂时（未用）
	INT16U	iBoxStatus;					//是否屏蔽手持盒功能    (未用)已经屏蔽
	
	//INT8U	iStopMode;					//单条与停止切换      未使用
	INT32S	iLag[MAXCH];				//各轴逻辑脉冲与实际脉冲间的滞后量范围
	INT32S	iPerCount;					//每批产量
	INT16U	iProbeMode;					//探针模式（1：有伺服清零输出，0：无伺服清零输出）
	INT32S	iNumerator[MAXCH];			//各轴电子齿轮比分子
	INT32S	iDenominator[MAXCH];		//各轴电子齿轮比分母
	
	//INT8U   iZSoftLimit;					//是否开启Z轴软件限位 1开启  0不开启

	FP32 	fMaxLimit[MAXCH];			//正向软限位
	FP32	fMinLimit[MAXCH];			//负向软限位
	INT8U	iZBackZero;					//加工完一条弹簧后Z轴是否归零，0：否(停在程序的最后一行位置) 1：是
	INT8U   iAxisSwitch[MAXCH];			//各轴开关选择
	INT32S	iSystemName;				//控制器型号
	INT8U   iCyldMode;					//气缸模式
	INT8U	iHardLimitSwitch[MAXCH];	//硬件限位检测标志量，1开启，0关闭,默认为0不检测
	INT8U   iZeroReplaceLimit[MAXCH];	//原点当限位：0-原点，1-负限位，2-正限位
	INT32S	lChAddSpeed[MAXCH];			//各轴加工加速度
	INT16S	iSecondAxisStyle;			//从轴连续方式，0为连续，1为不连续
	INT16S  iAxisNameNo[MAXCH];			//轴名字编号,0默认,1送线,2转线,3转芯,4节距,5芯刀,6卷取,7夹耳,8切刀,9上切,10下切
	
	INT32S   iSendClear;				//送线列输入ENT是否清送线坐标
	
	INT16S  iZLimitSwh;					// Z轴限位切换，到探针5探针6选择。
	INT16S  iPbFnSl;					//探针功能选择0为输入等待，设1为探针模式，在为送线轴有效
	FP32  	iProDelay;					//探针为输入等待时候的延时时间ms
	INT16S 	iSwhSingleIo;				//单条输入点开关	探针5.6
	INT16S 	iSwhRunIo;					//自动输入点开关。  探针7
	INT8U 	iIOKinkConfig_IN;				//缠线报警端口(0-24,255禁用)
	INT8U 	iIOKinkConfig_Level;			//缠线报警电平(0:低电平有效  1:高电平有效)
	INT8U 	iIOBreakLineConfig_IN;		//断线报警端口(0-24,255禁用)
	INT8U 	iIOBreakLineConfig_Level;		//断线报警电平(0:低电平有效  1:高电平有效)
	INT8U 	iIORunLineConfig_IN;			//跑线报警端口(0-24,255禁用)
	INT8U 	iIOWireAccConfig_OUT;			//线架加速输出端口(0-24,255禁用)
	INT8U 	iIOSart_IN;					//外部启动输入端口(0-24,255禁用)
	INT8U 	iIOSingleConfig_IN;			//外部单条输入端口(0-24,255禁用)
	INT8U 	iIOAlarmConfig_OUT;			//报警输出端口(0-24,255禁用)
	INT8U 	iIOSendLinePConfig_IN;		//送线+输入端口(0-24,255禁用)
	INT8U 	iIOSendLineMConfig_IN;		//送线-输入端口(0-24,255禁用)
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

#define SILVERCOLOR     0xEDEDED
#define LIGHTWHITECOLOR 0xF4F4F4
#define LIGHTBLUECOLOR2 0x4682B4


typedef struct
{
	INT32S	nLine;							//行号
	INT8U 	nCode;							//指令		
	INT8U	SpindleAxis	;					//有主轴，1为X轴，2为Y，3为Z，4为A轴			
	INT32S	lValue[5];						//v0-v3为X,Y,Z,A轴，v4为速度
	INT8U	nIO;							//探针
	INT8U	nOut[MAXCYLINDER];				//气缸
	INT8U 	nReset;							//回零标示。二进制按位表示1111，0000
	FP32	fDelay;
}UNITDATA;

typedef struct
{
	INT8U nCode;
	FP32  lValue[4];
}AUTODATA;


void	DataCls(INT16S i);
void	DataPut(INT16S i,INT16S j,INT8S *str);
void	SysParaInit(void);

BOOL	SysParaRead(void);
void	SysParaWrite(void);
//BOOL	SysParaReadYaffs(void);
//void	SysParaWriteYaffs(void);

void	SysParaCheckUpdate(void);
BOOL	SysParaCheck(void);

void	ReadRunCount(void);
void	WriteRunCount(void);
//void	ReadRunCountYaffs(void);
//void	WriteRunCountYaffs(void);
void 	WriteCountToRam(void);
void 	ReadCountFromRam(void);



void	WriteDataToFile( INT16S File );
BOOL	ReadDataFromFile(INT16S File );
//void	WriteDataToYaffsFile( INT16S File );
//BOOL	ReadDataFromYaffsFile(INT16S File );

void	SetUnitData(INT32S nL,INT8U nC);
//BOOL	ReadFileMessage(INT16S File ,INT8S *message );
//INT16U	ReadPortb(INT16U port);
//void	WritePortb(INT16U port,INT32U data);
void 	WritePosToRam(INT16S ch, INT32S pos);
INT32S ReadPosFromRam(INT16S ch);



BOOL	CheckRam(void);
//void 	WritePosData(void);
//void 	ReadPosData(void);
void 	DisableParaInit(void);
INT16S  FileExist(INT16S File);								//判断料号为File的文件是否存在
//INT16S CopyYaffsToYaffsFile(INT16S FileD, INT16S FileS);
INT16U MEMToXMSEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);	
INT16U XMSToMEMEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);
# endif
