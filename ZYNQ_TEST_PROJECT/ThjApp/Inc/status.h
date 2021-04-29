# ifndef	___STATUS__H___
# define	___STATUS__H___
/****************************************************
	���峣���ĺ궨��ֵ
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


/*********** ��׼��������  ******************/

//# define	STARTIO			62
# define	STOPIO			65


/***********  ���չ��������ź�  *************/

# define	PAOXIANDI		255//18//33			//���߱���	20130816
# define	DUANXIAN		20				//����
# define	SONGXIANIOCONST 255//21				//����
# define	DUXIAN			21				//����

# define	SAFEDOOR		18				//��ȫ��
//# define	SONGXIAN		14				//����
//# define  TUIXIAN			13				//����


# define    STOPID1         38	  			//�ⲿ����1   CLMT-
//# define    STOPID2         37   			//�ⲿ����2   CLMT+
# define	RUNID           35    			//�ⲿ����    BLMT-



/***************����ź�*******************/

#define		O_WARNING		9				//10�ű������
#define		O_RUNLED		10				//11���������
#define		O_STOPLED		11				//12 ͣ����̬�����

#define     O_SENDADD       15				//16�Ÿ� �߼ܼ������


/****************�ŷ�����*******************/

# define	XSERVOWARNING	41//0			
# define	YSERVOWARNING	36//5
# define	ZSERVOWARNING	15//10
# define	USERVOWARNING	10//15
# define	VSERVOWARNING	5//36
# define	WSERVOWARNING	0//41

/****************�ŷ�ԭ��*******************/
//���岻��ȷ����������
# define	XSERVOZERO		68			
# define	YSERVOZERO		69
# define	ZSERVOZERO		70
# define	USERVOZERO		71
# define	VSERVOZERO		72
# define	WSERVOZERO		73

/***************��еԭ��*******************/

# define	XZERO			2		
# define	YZERO			7
# define	ZZERO			12
# define	UZERO			17
# define	VZERO			39
# define	WZERO			42

/***  ����Ϊԭ������λ�ź�һ�㲻���޸�  ***/

# define    XLMTMINUS       25				//����λ
# define    XLMTPLUS       	24				//����λ
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


// ************ ̽�붨�� ************* //
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




// ***************�ֳֺ�IO ����************** //

//�ֳֺ�IO ����
//# define	BMQ_A1			66
//# define	BMQ_B1			67


# define	LOWSPEEDIO		60
# define	MIDDLESPEEDIO	58
# define	HIGHSPEEDIO		56

# define	TXIO			57
# define	TYIO			59
# define	TZIO			61
# define	TAIO			63

# define	STOPIO2			64
# define    RUNIO           22		//�����û����
# define    SINGLEIO        23



// ***************�ֳֺ�IO ����*************** //

//# define	BMQ_A1			66
//# define	BMQ_B1			67
/*
# define	LOWSPEEDIO		56
# define	MIDDLESPEEDIO	58
# define	HIGHSPEEDIO		60

# define	TXIO			57
# define	TYIO			59
# define	TZIO			61
# define	TAIO			63

# define 	TBIO            64
# define	TCIO			22 				//Ŀǰ��ʱ��ʹ��

# define	STOPIO2			23
*/



/*************�������ź�*******************/

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



/************** LED�ƶ��� *******************/


# define	BACKZEROLED		0
# define	YSWITCHLED	    1
# define	ZSWITCHLED		2
# define	USWITCHLED	    3

# define	VSWITCHLED		100			//��5,6��
# define	WSWITCHLED	    100

# define	F1LED	        4
# define	CYLINDERLED		5
# define	RUNLED			6
# define	STOPLED			7
# define	TESTLED			8


# define	XLED			16
# define	YLED			17
# define	ZLED			18
# define	ULED			19

# define	VLED			100			//��5,6��
# define	WLED			100

# define	HANDLED			12
# define	BOXLED			23


/************��������״̬��ʾ****************/

# define    STATUS_STOP  0
# define    STATUS_AUTO  1
# define    STATUS_PAUSE 2
# define    STATUS_TEST  3
# define    STATUS_WHEEL 4


# endif
