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

/*************************************************/
/*
# define	BACKZEROLED		0
# define	YSWITCHLED	    1
# define	ZSWITCHLED		2
# define	USWITCHLED	    3
# define	CYLINDERLED		5
# define	RUNLED			6
# define	STOPLED			7
# define	TESTLED			8
# define	XLED			20
# define	YLED			21
# define	ZLED			22
# define	ULED			13
# define	HANDLED			12
*/

# define	YLED			15
# define	XLED			14
# define	HANDLELED		13
# define	AIRDLED			12
# define	AIRCLED			11
# define	AIRBLED			10
# define	AIRALED			9
# define	STOPLED			8

# define	HANDBOXLED		6
# define	ZLED			5
# define	ZSWITCHLED		3
# define	YSWITCHLED		2
# define	STEPLED		    1
# define	RUNLED		    0

# define	LEDOPEN			1
# define	LEDCLOSE		0

/************************************************/

# define	XSERVOWARNING	41//0		//伺服报警
# define	YSERVOWARNING	36//5
# define	ZSERVOWARNING	15//10
# define	USERVOWARNING	10//15
# define	VSERVOWARNING	5 //36
# define	WSERVOWARNING	0 //41

# define	XSERVOZERO		68		//伺服原点
# define	YSERVOZERO		69
# define	ZSERVOZERO		70
# define	USERVOZERO		71

# define	XZERO			2		//机械原点
# define	YZERO			7
# define	ZZERO			12
# define	UZERO			17

/**********手持盒IO 四轴*************/

# define	LOWSPEEDIO		60
# define	HIGHSPEEDIO		56
# define	MIDSPEEDIO		58
# define    SINGLEIO        23
# define	TXIO			57
# define	TYIO			59
# define	TZIO			61
# define	TAIO			63
# define	STOPIO2			64
# define    RUNIO           22

/******  标准操作按键    ******/

# define	RUNDI			g_Ini.iIOSart_IN				//默认启动 探针5
# define	SINGLEDI		g_Ini.iIOSingleConfig_IN      	//默认单条 探针6

# define	STEPDI			65		//寸动
# define  	STOPIO			62		//急停


/******  工艺过程输入信号  ******/

# define	PAOXIANDI		g_Ini.iIORunLineConfig_IN		//跑线
# define	DUANXIAN		g_Ini.iIOBreakLineConfig_IN		//断线
# define	SONGXIANIOCONST g_Ini.iIOKinkConfig_IN		//缠线


/***  以下为原点与限位信号一般不做修改  ***/

# define    XLMTMINUS       25		//负限位
# define    XLMTPLUS       	24		//正限位
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

/*******************探针信号***************/

#define		TTANZHEN1		4
#define		TTANZHEN2		3
#define		TTANZHEN3		9
#define		TTANZHEN4		8
#define 	TTANZHEN5       14
#define     TTANZHEN6       13
#define     TTANZHEN7       19
#define     TTANZHEN8       18

/***************输出信号*******************/

#define		O_WARNING		g_Ini.iIOAlarmConfig_OUT
#define		SXDRIVEDO		3

#define		O_SENDADD		(MAXCYLINDER-1)


# endif
