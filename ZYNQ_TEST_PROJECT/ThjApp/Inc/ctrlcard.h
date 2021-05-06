# ifndef	___CTRLCARD_H___
# define	___CTRLCARD_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        ctrlcard.h
  创建人：       范志州
  创建日期：		2005/08/09
  主要功能描述:   进一步封装控制卡的库函数保证对外接口的一致性 

  修改记录：
   1、...
*************************************************/
#include	"base.h"

//使用850卡则定义为1否则定义为0
# define	CARD850		0

#if CARD850
	#include	"adt850.h"
#else
	//#include	"th838.h"	
	#include	"adt4860.h"
#endif




# define	CHMAX		8

typedef void (*FUNCTION)(void);


void	CtrlCard_Init(void);
void	CtrlCard_Destroy(void);

BOOL  	InitBoard(void) ;
BOOL  	InitBoardPara(void);
void	SetUnitPulse(INT16S ch,FP32 mm,INT32S pulse);

FP32	GetUnitPulse(INT16S ch);
void	SetMMConSpeed(INT16S ch,FP32 mmspeed);
void	SetMMAddSpeed(INT16S ch,FP32  mmstartspeed,FP32 mmspeed,FP32 mmaddspeed);

void	SetPulseConSpeed(INT16S ch,INT32S speed);
void	SetMM(INT16S ch,FP32 mm,BOOL cur);
void	SetAngle(INT16S ch,float angle,BOOL cur);
void	SetPulse(INT16S ch, INT32S pos,BOOL cur);
FP32	GetMM(INT16S ch,BOOL cur);
FP32	GetAngle(INT16S ch,BOOL cur);
INT32S	GetPulse(INT16S ch,BOOL cur);
float	GetMMFromPulse(INT16S ch,INT32S pulse) ;
INT32S	GetPulseFromMM(INT16S ch,FP32 mm);

float	GetAngleFromPulse(INT16S ch,INT32S pulse);
INT32S	GetPulseFromAngle(INT16S ch,FP32 angle);
float	GetMMFromAngle(INT16S ch,FP32 angle);
float	GetAngleFromMM(INT16S ch,FP32 mm);
void	Start( INT16S ch );
void	Stop1( INT16S ch );
void	Stop2( INT16S ch );
INT16S	IsFree(INT16S ch ) ;
INT16S 	IsInpEnd(INT16S no) ;
INT16S 	IsNextInp(INT16S no); 

INT16U  GetStopData(INT16S ch);
FP32    GetInpSpeed(void);

void	ShortlyPmove(INT16S ch,INT32S pos2,INT32S speed);
void	ShortlyInPmove(INT16S ch,INT32S pos,INT32S speed);
void	ShortlyLine2l(INT32S pos1,INT32S pos2,INT32S speed);
void	ShortlyLine3l(INT32S pos1,INT32S pos2,INT32S pos3,INT32S speed);
void	ShortlyLine4l(INT32S pos1,INT32S pos2,INT32S pos3,INT32S pos4,INT32S speed);
void	ShortlyLine4(INT32S pos1,INT32S pos2,INT32S pos3,INT32S pos4,INT32S speed);

void	GotoAngle(INT16S ch,FP32 angle,FP32 speed,FUNCTION pfn);
void	MoveAngle(INT16S ch,FP32 angle,FP32 speed,FUNCTION pfn);
void	ShortlyMoveAngle(INT16S ch,FP32 angle,FP32 speed);

void	MoveMM(INT16S ch,FP32 mm,FP32 speed,FUNCTION pfn);
void	AddMoveMM(INT16S ch,FP32 pos,FP32 startspeed,FP32 speed,FP32 addspeed);
void	AddMoveMM1(INT16S ch,FP32 pos,FP32 startspeed,FP32 speed,FP32 addspeed);

void	FifoMoveMM(INT16S ch,FP32 pos,FP32 mmspeed);

void	ShortlyMoveMM(INT16S ch, FP32 mm, FP32 speed);
BOOL 	CheckHome(INT16S);
BOOL	TestHome(INT16S ch);
BOOL 	Home(INT16S ch, INT16S dir);
BOOL	BackZero(INT16S ch,INT16S dir);
BOOL	FindZero(INT16S ch, INT16S style, INT32S speed);
BOOL	FindZero2(INT32S speed1,INT32S speed2);
BOOL	FindZero3(INT32S speed1,INT32S speed2,INT32S speed3);
BOOL	FindZero4( INT32S speed1, INT32S speed2, INT32S speed3, INT32S speed4);

void	MovePulse2(INT32S p1,INT32S p2,INT32S speed1,INT32S speed2);
void	MovePulse3(INT32S p1,INT32S p2,INT32S p3,INT32S speed1,INT32S speed2,INT32S speed3);
void	MovePulse4(INT32S p1,INT32S p2,INT32S p3,INT32S p4,INT32S speed1,INT32S speed2,INT32S speed3,INT32S speed4);

BOOL	GotoMM(INT16S ch,FP32 pos,FP32 speed,BOOL istrue);
BOOL	GotoMM4(INT16S ch1,INT16S ch2,INT16S ch3,INT16S ch4,FP32 pos1,FP32 pos2,FP32 pos3,FP32 pos4,FP32 speed,BOOL istrue);    

INT16S 	ReadBit(INT16S io);
INT16S 	RealIO(INT16S io,INT16S status,INT16S ti);
INT16S 	WriteBit(INT16S io,INT16S status);
INT16S 	GetOut(INT16S io);

BOOL	BackZero4(INT16S dir1,INT16S dir2,INT16S dir3,INT16S dir4,INT16S step);
void	BackZeroRun(INT32S p1,INT32S p2,INT32S p3,INT32S p4,INT32S speed1,INT32S speed2,INT32S speed3,INT32S speed4,INT32S mode);
void 	Move(int ch,INT32S pos,INT32S speed);


INT16S  get_stopdata(INT16S cardno,INT16S axis,INT16S *value);

//INT16S  CloseProbe(INT16S cardno);
//INT16S  ReadProbe(INT16S cardno,INT16S pro);
//INT16S  OpenProbe(INT16S cardno,INT16S pro,INT16S output);
//INT16U  inportb(INT16U port);
//void    outportb(INT16U port,INT16U data);
BOOL 	DA_Out(FP32 value,INT16S ch);
void 	SetPortVal(INT16U add,INT16U value ,INT16S number);
void 	GetPortVal(INT16U add,INT16U *value ,INT16S number);
BOOL	TestBackZero(INT16S ch,INT16S dir);


INT16S CheckWarningNoSafeDoor(void);

INT16S  CheckWarning(void);								//报警检查函数
INT16S 	CheckHardLimit(INT16S ch, INT32S dir);
INT16S  CheckSoftLimit(INT16S ch, INT32S dir);
INT16S  CheckLimit(BOOL bSoftCk,BOOL bHardCk,INT16S ch,INT32S dir);

int BaseSetCommandPos(int axis,long pos);				//当缓存设零时，与get_command_pos偏移量基值 

int BaseGetCommandPos(int axis,long *pos);				//获取缓存偏移量基值上座标位置 ;

INT16S	SetQuiteRange(INT32S rate);

INT32S 	GetRange(void);
INT16S	SetRange(INT32S rate);
int 	vm_sync(void);
INT16S  ReadFifo(void);

BOOL  PulseLagCheck(INT16U axis);

long GetCommandPos(int axis);
long GetActualPos(int axis);
int SetActualPos(int axis,long pos);
int SetCommandPos(int axis,long pos);
void	ShortlyInp_Move2(INT32S pos1,INT32S pos2,INT32S speed);

INT16S  SetPhyAxis(INT16S pro_axis,INT16S phy_axis);
# endif
