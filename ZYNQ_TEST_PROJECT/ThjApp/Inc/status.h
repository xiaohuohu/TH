# ifndef	___STATUS__H___
# define	___STATUS__H___
/****************************************************
	定义常见的宏定义值
****************************************************/

# define	NORUN			0
# define	RUNNING			1
# define	PAUSE			2
# define	CONTINUE		3
# define	RUNSTOP			4
# define	SINGLERUN		5

# define	ON				1
# define	OFF				0

# define	IOPRESS			0		
# define	IOREALSE		1	
	
# define	OPEN			1
# define	CLOSE			0

# define	CHA_VALUE		0.005


/*********** 标准操作按键  ******************/

# define	STARTIO			62
# define	STOPIO			65
# define	EXT_STARTIO	    g_Ini.iIOSart_IN		//外部启动
# define	EXT_SINGLEDI	g_Ini.iIOSingleConfig_IN	//外部单条

/***********  工艺过程输入信号  *************/

# define	PAOXIANDI		g_Ini.iIORunLineConfig_IN//33			//跑线报警	20130816
# define	I_SAFEDOOR		g_Ini.iIOSafeDoorConfig_IN//探针7			//安全门

# define	STOPDI			23
# define	DUANXIAN		g_Ini.iIOBreakLineConfig_IN				//断线
# define	SONGXIANIOCONST g_Ini.iIOKinkConfig_IN				//缠线
# define	SONGXIAN		g_Ini.iIOSendLinePConfig_IN					//送线
# define    TUIXIAN			g_Ini.iIOSendLineMConfig_IN				//退线
# define	SINGLEDI1		TTANZHEN7      	//单条IO1  探针7


# define    AHANDADDDI		YLMTPLUS		//手动A转线加
# define	AHANDDECDI      YLMTMINUS		//手动A转线减



/***************输出信号*******************/


#define		CLEARWARNDO		g_Ini.iIOAlarmConfig_OUT		//7号汽缸用于清除线架的报警输出

#define     O_SENDADD       g_Ini.iIOWireAccConfig_OUT				//线架加速输出
#define		O_LEDSWITCH		8
#define		O_WARNING		g_Ini.iIOAlarmConfig_OUT


/****************伺服报警*******************/

# define	XSERVOWARNING	41//0			
# define	YSERVOWARNING	36//5
# define	ZSERVOWARNING	15//10
# define	USERVOWARNING	10//15
# define	VSERVOWARNING	5//36
# define	WSERVOWARNING	0//41

/****************伺服原点*******************/
//定义不明确！！！待测
# define	XSERVOZERO		68			
# define	YSERVOZERO		69
# define	ZSERVOZERO		70
# define	USERVOZERO		71
# define	VSERVOZERO		72
# define	WSERVOZERO		73

/***************机械原点*******************/

# define	XZERO			2		
# define	YZERO			7
# define	ZZERO			12
# define	UZERO			17
# define	VZERO			39
# define	WZERO			42

/***  以下为原点与限位信号一般不做修改  ***/

# define    XLMTMINUS       25				//负限位
# define    XLMTPLUS       	24				//正限位
# define    YLMTMINUS       27
# define    YLMTPLUS       	26
# define    ZLMTMINUS       29
# define    ZLMTPLUS       	28
# define    ULMTMINUS       31
# define    ULMTPLUS       	30
# define    VLMTMINUS       35
# define    VLMTPLUS       	32
# define    WLMTMINUS       38
# define    WLMTPLUS       	37


// ************ 探针定义 ************* //
# define	TTANZHEN1		4				//RealIO
# define	TTANZHEN2		3
# define	TTANZHEN3		9
# define	TTANZHEN4		8
# define	TTANZHEN5		14
# define	TTANZHEN6		13
# define	TTANZHEN7		19
# define	TTANZHEN8		18


# define	TANZHEN1		1
# define	TANZHEN2		2
# define	TANZHEN3		3
# define	TANZHEN4		4
# define	TANZHEN5		5
# define	TANZHEN6		6
# define	TANZHEN7		7
# define	TANZHEN8		8




/***************手持盒IO 四轴**************

//手持盒IO 四轴
# define	BMQ_A1			66
# define	BMQ_B1			67
# define	LOWSPEEDIO		60//56
# define	MIDDLESPEEDIO	58
# define	HIGHSPEEDIO		56//60
# define    SINGLEIO        23
# define	TXIO			57
# define	TYIO			59
# define	TZIO			61
# define	TAIO			63

# define	STOPIO2			64

# define    RUNIO           22
//# define 	TBIO            65
# define	TCIO			68 				//目前暂时不使用*/



/***************手持盒IO 六轴***************/

//手持盒IO 四轴
//# define	BMQ_A1			66
//# define	BMQ_B1			67
# define	LOWSPEEDIO		56
# define	MIDDLESPEEDIO	58
# define	HIGHSPEEDIO		60
# define    SINGLEIO        65
# define	TXIO			57
# define	TYIO			59
# define	TZIO			61
# define	TAIO			63
# define 	TBIO            64
# define	TCIO			22 				//目前暂时不使用

# define	STOPIO2			23
# define    RUNIO           62




/*************编码器信号*******************/

#define		ECAX			54//44
#define		ECAY			52//46
#define		ECAZ			50//48
#define		ECAU			48//50
#define		ECAV			46//52
#define		ECAW			44//54

#define		ECBX			55//45
#define		ECBY			53//47
#define		ECBZ			51//49
#define		ECBU			49//51
#define		ECBV			47//53
#define		ECBW			45//55

#define		ECZX			73//68
#define		ECZY			72//69
#define		ECZZ			71//70
#define		ECZU			70//71
#define		ECZV			69//72
#define		ECZW			68//73



/************** LED灯定义 *******************/


# define	BACKZEROLED		0
# define	YSWITCHLED	    1
# define	ZSWITCHLED		2
# define	USWITCHLED	    3
# define	VSWITCHLED		4
# define	WSWITCHLED	    5

# define	STOPLED			7
# define	TESTLED			8
//# define	WHEELLED		6

# define	XLED			18
# define	YLED			19
# define	ZLED			20
# define	ULED			21
# define	VLED			22
# define	WLED			23

# define	SENDLED			24
# define	STEPLED			25
# define	HANDLED			26

# define	FILELED			30
# define	CYLINDERLED		6//38
# define	PRGLED			46



/************更新运行状态显示****************/

# define    STATUS_STOP  0
# define    STATUS_AUTO  1
# define    STATUS_PAUSE 2
# define    STATUS_TEST  3
# define    STATUS_WHEEL 4


# endif
