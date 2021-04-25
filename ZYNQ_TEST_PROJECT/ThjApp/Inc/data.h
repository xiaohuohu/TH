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

										//74K���ҵ�24���������ֿ�
   
#define		PAI			3.1415926
#define		MULTIPLIER	5
#define		DIVISOR		3
#define		MAXMOD		99999989
#define		MAXLINE		200
#define		MAXVALUECOL	8
#define		MAXCYLINDER	4
#define		MAXCH		4

#define 	MZK_32		9      			//�����ֿ�Ĵ洢����2--9��
#define 	MZK_24		8
#define 	MZK_16		7
#define 	YH_32		6

typedef struct
{
  INT8S	value[MAXVALUECOL][9];
}COMMAND;

typedef struct
{
	INT32S 	lPulse[MAXCH];				//ÿת����
	FP32	fMM[MAXCH];					//ÿת�г�
	INT32S	lMaxSpeed[MAXCH];			//������ת������
	FP32	fHandSpeed[MAXCH];			//�ֶ��ٶ�
	FP32	fBackSpeed[MAXCH];          //��ԭ���ٶ�
	FP32	fGearRate[MAXCH];			//���ٱ�
	FP32	fStartOffset[MAXCH];		//ָʾ����ԭ��λ�������ԭ�㿪�ص�ƫ��ֵ
	INT8U 	iRunDir[MAXCH];				//���з���
	INT8U	iBackDir[MAXCH];			//����㷽��
	INT16S	iBackMode[MAXCH];			//�ӹ������ģʽ    0�ͽ�����  1�������  2������� 3������
	INT32S	lRunCount;                  //�ӹ�����
	INT32S	lDestCount;					//Ŀ�����
	INT32S	lProbeCount;				//̽��ʧ�ܴ�������
	INT32S	lPassword;					//����ϵͳ�����ͳ��ò���������
	FP32	fXMaxSpeed;					//�������ӹ��ٶ�   ��λ��R/Min
	FP32	fStartSpeed;				//��ʼ�ٶ�		
	//INT32S	lAddSpeed;					//���ٶ�
	//INT16S	iAlarmLogic;			//�����߼�        ����	
	INT16S	iScrProtect;				//�Ƿ�������				��0--������������1--����������
	INT16S	iScrProtectTime;			//��������ʱ��  			��λ������
	INT16S	iScrProtectBmp;				//�Ƿ���ͼƬ����			��0--������������1--����������
	INT16S	iXianjiaDrive;				//ָʾ�Ƿ������߼���������	��0--��������1--������
	//INT16S	iHandStatus;				//ָʾ����״̬               �ò���δ�����ڲ������ý���  ��δ�ã�
	INT16S 	iHandRunSpeed;				//��ҡ�ٶ�
	FP32    fHandRunRate;				//��ҡ����
	INT16S	iReverseDly;			//������ʱ��     ���ò������ڲ������ý���ȥ����20140807
	INT16S 	iPaoXian; 					//���߼�⣨1��⣬0����⣩	
	//INT16S	iSpeedDly;  			//�ٶ���ʱ��	 ���ò������ڲ������ý���ȥ����20140807
	INT16S  iWarningOut;				//���������1�����0�������
	INT16S  iFileName;					//�Ϻţ��ӹ��ļ���
	INT16S  iAxisCount;					//ϵͳ����
	//INT16S	iSafeDoor;					//��ȫ�ż�⣨1��⣬0����⣩ �ò���δʹ�ã��Ƿ�ע�͵���
	INT16S	iYDaoXian;					//Y����ҡ�Ƿ�ɵ���
	INT16S	iProbeAheadTime;			//̽��ǰ��ʱ  ��λ��ms
	INT16S	iProbeLagTime;				//̽�����ʱ  ��λ��ms
	//INT32S	lModify;					//�Ǽ�ϵͳ����					�����ã�	
	//FP32    fBackSpeed1[2];			//������٣��ɲ��ã��������һ����1/10�Ļ����ٶȣ����ã� ����
	INT16S  iLanguage; 					//����ѡ��
	//INT16S  iUDriver;					//U��Ϊ�����ᣨ���ã�			����
	/*INT8U	iServoLogicX;				//X�ŷ������߼� ��0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч��			 
	INT8U	iServoLogicY;
	INT8U	iServoLogicZ;
	INT8U	iServoLogicA;*/
	
	INT8U	iServoLogic[MAXCH];			//�ŷ������߼� ��0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч��
	INT8U	iServoZero[MAXCH];			//����ʱ�Ƿ�Ѱ���ŷ����(0:��  1:��)
	INT16S  iAxisRunMode[MAXCH];		//������ģʽ��(0:��Ȧ��1:��Ȧ��2:˿�ܣ�3:����)
	/*INT8U	iServoZeroX;				//����ʱ�Ƿ�Ѱ���ŷ����(0:��  1:��)
	INT8U	iServoZeroZ;
	INT8U	iServoZeroA;*/
	
	
	FP32	fVoltageOne;				//ģ���ѹ���1��0-10V����ʱ��δ�ã�
	FP32	fVoltageTwo;				//ģ���ѹ���2��0-10V����ʱ��δ�ã�
	INT16U	iBoxStatus;					//�Ƿ������ֳֺй���    (δ��)�Ѿ�����
	
	//INT8U	iStopMode;					//������ֹͣ�л�      δʹ��
	INT32S	iLag[MAXCH];				//�����߼�������ʵ���������ͺ�����Χ
	INT32S	iPerCount;					//ÿ������
	INT16U	iProbeMode;					//̽��ģʽ��1�����ŷ����������0�����ŷ����������
	INT32S	iNumerator[MAXCH];			//������ӳ��ֱȷ���
	INT32S	iDenominator[MAXCH];		//������ӳ��ֱȷ�ĸ
	
	//INT8U   iZSoftLimit;					//�Ƿ���Z�������λ 1����  0������

	FP32 	fMaxLimit[MAXCH];			//��������λ
	FP32	fMinLimit[MAXCH];			//��������λ
	INT8U	iZBackZero;					//�ӹ���һ�����ɺ�Z���Ƿ���㣬0����(ͣ�ڳ�������һ��λ��) 1����
	INT8U   iAxisSwitch[MAXCH];			//���Ὺ��ѡ��
	INT32S	iSystemName;				//�������ͺ�
	INT8U   iCyldMode;					//����ģʽ
	INT8U	iHardLimitSwitch[MAXCH];	//Ӳ����λ����־����1������0�ر�,Ĭ��Ϊ0�����
	INT8U   iZeroReplaceLimit[MAXCH];	//ԭ�㵱��λ��0-ԭ�㣬1-����λ��2-����λ
	INT32S	lChAddSpeed[MAXCH];			//����ӹ����ٶ�
	INT16S	iSecondAxisStyle;			//����������ʽ��0Ϊ������1Ϊ������
	INT16S  iAxisNameNo[MAXCH];			//�����ֱ��,0Ĭ��,1����,2ת��,3תо,4�ھ�,5о��,6��ȡ,7�ж�,8�е�,9����,10����
	
	INT32S   iSendClear;				//����������ENT�Ƿ�����������
	
	INT16S  iZLimitSwh;					// Z����λ�л�����̽��5̽��6ѡ��
	INT16S  iPbFnSl;					//̽�빦��ѡ��0Ϊ����ȴ�����1Ϊ̽��ģʽ����Ϊ��������Ч
	FP32  	iProDelay;					//̽��Ϊ����ȴ�ʱ�����ʱʱ��ms
	INT16S 	iSwhSingleIo;				//��������㿪��	̽��5.6
	INT16S 	iSwhRunIo;					//�Զ�����㿪�ء�  ̽��7
	INT8U 	iIOKinkConfig_IN;				//���߱����˿�(0-24,255����)
	INT8U 	iIOKinkConfig_Level;			//���߱�����ƽ(0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч)
	INT8U 	iIOBreakLineConfig_IN;		//���߱����˿�(0-24,255����)
	INT8U 	iIOBreakLineConfig_Level;		//���߱�����ƽ(0:�͵�ƽ��Ч  1:�ߵ�ƽ��Ч)
	INT8U 	iIORunLineConfig_IN;			//���߱����˿�(0-24,255����)
	INT8U 	iIOWireAccConfig_OUT;			//�߼ܼ�������˿�(0-24,255����)
	INT8U 	iIOSart_IN;					//�ⲿ��������˿�(0-24,255����)
	INT8U 	iIOSingleConfig_IN;			//�ⲿ��������˿�(0-24,255����)
	INT8U 	iIOAlarmConfig_OUT;			//��������˿�(0-24,255����)
	INT8U 	iIOSendLinePConfig_IN;		//����+����˿�(0-24,255����)
	INT8U 	iIOSendLineMConfig_IN;		//����-����˿�(0-24,255����)
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

#define SILVERCOLOR     0xEDEDED
#define LIGHTWHITECOLOR 0xF4F4F4
#define LIGHTBLUECOLOR2 0x4682B4


typedef struct
{
	INT32S	nLine;							//�к�
	INT8U 	nCode;							//ָ��		
	INT8U	SpindleAxis	;					//�����ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��			
	INT32S	lValue[5];						//v0-v3ΪX,Y,Z,A�ᣬv4Ϊ�ٶ�
	INT8U	nIO;							//̽��
	INT8U	nOut[MAXCYLINDER];				//����
	INT8U 	nReset;							//�����ʾ�������ư�λ��ʾ1111��0000
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
INT16S  FileExist(INT16S File);								//�ж��Ϻ�ΪFile���ļ��Ƿ����
//INT16S CopyYaffsToYaffsFile(INT16S FileD, INT16S FileS);
INT16U MEMToXMSEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);	
INT16U XMSToMEMEx(INT8U *p,INT16U handle,INT32U offset,INT16U length);
# endif
