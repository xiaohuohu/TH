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

//74K���ҵ�24���������ֿ�
   
#define		PAI			3.1415926
#define		MULTIPLIER	5
#define		DIVISOR		3
#define		MAXMOD		99999989

#define		MAXLINE		200

#define		MAXVALUECOL	12
#define		MAXCYLINDER	8
#define		MAXCH		6//4


/*
value[0]Ϊָ�value[1]X��ʼ�ǣ�value[2]X�����ǣ�value[3]���ߣ�value[4]תо��value[5]��ȡ
value[6]��ȡ��value[7]�ж���value[8]Ϊ�ٱȣ�value[9]Ϊ̽�룬value[10]Ϊ���ף�value[11]Ϊ��ʱ
*/

typedef struct
{
  INT8S	value[MAXVALUECOL][9];
}COMMAND;


typedef struct
{
	INT32S 	lPulse[MAXCH];				//ÿת����
	FP32	fMM[MAXCH];					//ÿת�г�
	INT32S	lMaxSpeed[MAXCH];			//����ٶ�
	INT32S	lHandSpeed[MAXCH];			//�ֶ��ٶȣ���������δ����			����
	FP32	fBackSpeed[MAXCH];			//��ԭ���ٶ�
	FP32	lGearRate[MAXCH];			//���ٱ�
	
	INT8U 	iRunDir[MAXCH];				//���з���
	INT8U	iBackDir[MAXCH];			//��ԭ�㷽��
	INT32S	lRunCount;					//�ӹ�����
	INT32S	lDestCount;					//Ŀ�����
	INT32S	lProbeCount;				//̽�����
	INT32S	lPassword;					//��½��������
	
	FP32	fXMaxSpeed;					//X������ٶȣ������޶�
	FP32	fStartSpeed;				//��ʼ�ٶ�   ����������δ������ǰ����δʹ�õ���
	FP32	fAddSpeed;					//���ٶ�	  (��������δ������ǰ����δʹ�õ���
	
	
	INT16S	iScrProtect;				//����ѡ��
	INT16S	iScrProtectTime;			//����ʱ��
	INT16S	iScrProtectBmp;				//����ͼƬ
	
	INT16S	iHandStatus;				//ָʾ����״̬
	INT16S 	iHandRunSpeed;				//��ҡ�ٶ�
	FP32    fHandRunRate;				//��ҡ����
	INT8U	iBackOrder[MAXCH];			//����˳��

	INT16S 	iPaoXian; 					//���߼�⣨1��⣬0����⣩			
	INT16S  iWarningOut;				//���������1�����0�������
	INT16S  iFileName;					//�Ϻţ��ӹ��ļ���
	INT16S  iAxisCount;					//ϵͳ����
	INT16S	iYDaoXian;					//Y���Ƿ�ɵ��ߣ�����δ���ţ�		����
	
	INT8U   iAxisSwitch[MAXCH];			//���Ὺ��ѡ��

	INT16S  iLanguage; 					//����ѡ��
	
	INT8U	iServoLogic[MAXCH];			//�ŷ������߼� ��0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч��

	INT8U	iSearchZero[MAXCH];			//������㷽ʽ��0�������㣬1��������е��㣬2�������ŷ���㣬3��������е���ŷ���㣩

	INT16S  iAxisRunMode[MAXCH];		//������ģʽ��(0:��Ȧ��1:��Ȧ��2:˿�ܣ�3:����)
	
	FP32	fVoltageOne;				//ģ���ѹ���1��0-10V����ʱ��δ�ã�
	FP32	fVoltageTwo;				//ģ���ѹ���2��0-10V����ʱ��δ�ã�
	
	INT16S	iBackMode[MAXCH];			//�ӹ������ģʽ
	INT32S  lRunSpeed[MAXCH];			//����������ٶ�	����Sָ�����	(δ��)
	INT16S	iStepDis[MAXCH];			//�綯����
	INT16S	iStepRate[MAXCH];			//��ҡ����
	
	FP32	fMaxLimit[MAXCH];			//���������λ
	FP32	fMinLimit[MAXCH];			//���������λ
	
	FP32	fStartOffset[MAXCH];		//ָʾ����ԭ��λ�������ԭ�㿪�ص�ƫ��ֵ,��λ:0.1��(�Ƕ�), 0.1����(����)

	INT32S	lChAddSpeed[MAXCH];			//����ӹ����ٶ�
	
	INT16S	iSecondAxisStyle;			//����������ʽ��0Ϊ������1Ϊ������
	
	INT32S	iLag[MAXCH];				//ʵ��λ���ͺ��߼�λ�õ������������ ���ͺ���
	INT32S  iNumerator[MAXCH];			//ʵ������ȣ������:ʵ��λ��/�߼�λ��
	INT32S	iDenominator[MAXCH];		//�߼�����ȣ������:ʵ��λ��/�߼�λ��
	
	INT16S	iEnterNumber;				//��������Ƿ��������ʾ���뵽������ʾ

	INT16S	iDispLeavingTime;			//��ʾʣ��ļӹ�ʱ��

	INT16S	iXianjiaDrive;				//ָʾ�Ƿ������߼���������

	INT32S	lRotatorRate;				//ת�����������ı��ʲ���  Y/A=lRotatorRate
	
	INT16S  iColorStyle;				//ϵͳ��ɫ��ʽ
	
	INT8U   iCyldMode;					//����ģʽ
	
	INT16S  iAxisNameNo[MAXCH];			//�����ֱ��,0Ĭ��,1����,2ת��,3תо,4�ھ�,5о��,6��ȡ,7�ж�,8�е�,9����,10����
	
	INT8U 	iHardLimitSwitch[MAXCH];	//�Ƿ���Ӳ����λ��⣺1-����Ӳ����λ���  0-�ر�Ӳ����λ���
	
	INT8U 	iCyldOrOrigin;				//���װ���ģʽ��0�����װ�����1������㰴����
	
	INT8U	iLeftOrRight;				//�����������ܣ�������������������л���0��������1��������
	
	INT16S  iAxisMulRunMode[MAXCH];		//1:��Ȧ������Ծ��Ա�﷽ʽ(0���.��1����)
	INT16S  iMachineType;				//ϵͳ�ͺű��
	
	INT16S  iIOSendWire;				//����ʹ��

	INT16S 	iSwhSingleIo;				//��������㿪��	̽��5
	INT16S 	iSwhRunIo;					//�Զ�����㿪�ء�  ̽��7
	INT16S  iPhyAxis[MAXCH];			//�����������˿�

	INT16S  iPasswordTime;				//�޸���Ҫ�����ʱ��
	INT16S	iSafeDoor;					//��ȫ��0��Ч 1�͵�ƽ��Ч��������    2�ߵ�ƽ��Ч�����գ�

	INT8U 	iIOKinkConfig_IN;				//���߱����˿�(0-24,255����)
	INT8U 	iIOKinkConfig_Level;			//���߱�����ƽ(0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч)
	INT8U 	iIOBreakLineConfig_IN;		//���߱����˿�(0-24,255����)
	INT8U 	iIOBreakLineConfig_Level;		//���߱�����ƽ(0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч)
	INT8U 	iIORunLineConfig_IN;			//���߱����˿�(0-24,255����)
	INT8U 	iIOSafeDoorConfig_IN;			//��ȫ������˿�(0-24,255����)
	INT8U 	iIOWireAccConfig_OUT;			//�߼ܼ�������˿�(0-24,255����)
	INT8U 	iIOSart_IN;					//�ⲿ��������˿�(0-24,255����)
	INT8U 	iIOSingleConfig_IN;			//�ⲿ��������˿�(0-24,255����)
	INT8U 	iIOAlarmConfig_OUT;			//��������˿�(0-24,255����)
	INT8U 	iIOSendLinePConfig_IN;		//����+����˿�(0-24,255����)
	INT8U 	iIOSendLineMConfig_IN;		//����-����˿�(0-24,255����)
	INT8U 	iTestFuction;					//������԰���(0-����,1-����)
	INT8U 	iWorkAxisNo;				//A�Ჹ�����(0-6�� 0-���� 1-6������� )
	INT32S  iWorkAxisTMolecular;		//��������(0-999999)
	INT8U	iSetProNum[8];			    //���ö˿ں�̽��˿ں�1-8��ӳ���Ӧ�����ϵ� pro 1-8
	INT8U	iWaitSignal;			    //����̽��˿����ȴ��ź� pro 1-8
	INT32S  iProDelay;					//̽��Ϊ����ȴ�ʱ�����ʱʱ��ms
	
//	INT16S  iAxisZero[MAXCH];			//������Ļ���˿�
}BASEINI;


typedef struct
{
	INT32S	Dog_InitData;   						//���ĳ�ʼ��
	INT32S  First_Use;  							//�趨���ܹ����һ�ο�����ʼ���ܹ�������

	INT32S  Adtech_ComputerNo;						//1)	��˾���ͻ����Ա��
	INT32S	Adtech_Days;							//2)	��˾���޶�����
	INT32S	Adtech_Password;						//3)	��˾��������趨��������

	INT32S	UserDogIdentifier;						//4)	�û�������������
	
	INT32S	User_ComputerNo;						//5)	�û����ͻ��������
	INT32S	User_Days;								//6)	�û����޶�����
	INT32S	User_Password;							//7)	�û���������趨��������

	INT32S	Adtech_Active;  						//ָʾ�Ƿ�����˾����������,12345678�������
	DATE_T  Adtech_LastDay; 						//��˾�����һ�η��������������
	TIME_T  Adtech_NowTime; 						//��¼�û�������������ĵ�ǰʱ��

	INT32S	User_Active;     						//ָʾ�Ƿ����û�����������,12345678�������
	DATE_T 	User_LastDay; 							//�û������һ�η��������������
	TIME_T  User_NowTime; 							//��¼�û�������������ĵ�ǰʱ��
	
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


#define 	MZK_24	5      	//�����ֿ�Ĵ洢����2--9��

#define		KZK_22	7
#define		KZK_24	8
#define 	KZK_32	9      	//�����ֿ�Ĵ洢����2--9��
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
	INT32S	nLine;							//�к�
	INT8U 	nCode;							//ָ��		
	INT8U	SpindleAxis	;					//�����ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��			
	INT32S	lValue[7];						//v0-v5ΪX,Y,Z,A,B,C�ᣬv6Ϊ�ٶ�
	INT8U	nIO;							//̽��
	INT8U	nOut[MAXCYLINDER];				//����
//	INT8U	nOutAxis;						//�ο���
//	INT32S	lOutValue[2];					//�׿�ʼ���׽�����
	INT8U 	nReset;							//�����ʾ�������ư�λ��ʾ1111��0000
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
INT16S FileExist(INT16S File);								//�ж��Ϻ�ΪFile���ļ��Ƿ����

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
