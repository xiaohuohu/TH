/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        ctrlcard.c
  创建人：       范志州
  创建日期：	 2005/08/05
  主要功能描述:  进一步封装控制卡的库函数保证对外接口的一致性 

  修改记录：
   1、...
*************************************************/

#include	"includes.h"

//定义是否需要在调用834库函数之前是否关闭中断定义后为打开开关中断功能
//# define	OPENCLOSEINT

#define		CARD_NO		0

TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT	BOOL		g_bTH836;
TEST_EXT	BOOL		g_bRelease;
TEST_EXT  	OS_EVENT 	*MsgQueue;
WORK_EXT	int			g_iADValue;  
WORK_EXT	BOOL		g_bTestStart;
WORK_EXT	BOOL		g_bHandRun;  
WORK_EXT	float 		g_fSpeedBi;
WORK_EXT	float		g_lSpeed;
WORK_EXT	BOOL		g_bHandset;
WORK_EXT	INT16S		g_iErrorNo;
extern  	BASEINI 	g_Ini;
TEST_EXT	BOOL		g_bEnglish;


FP64		g_fPulseOfAngle[CHMAX];
FP64		g_fPulseOfMM[CHMAX];
INT32S		g_lPulseOfCircle[CHMAX];
int			g_iCardBeiLv[CHMAX];
int  		g_iCardStopFlag[CHMAX];	
INT32U  	g_ulOutStatus;
INT8U		g_ucAxisSwitch;

//WORK_EXT	INT32S		lDisYDceLong   	;				//前一结点的Y轴
//WORK_EXT 	INT32S		PreCurYlong 	;				//前一结点的Y轴
WORK_EXT	INT32S		lYProbelong 	;
WORK_EXT	INT32S		lYProbelong1 	;


WORK_EXT 	BOOL		g_bFirstResetX;
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;



////////////////////////////////// 探针操作 ////////////////////////////////////

#define WR0					0x0000
#define WR1					0x0002
#define WR2					0x0004
#define WR3					0x0006
#define WR4					0x0008
#define WR5					0x000a
#define WR6					0x000c
#define WR7					0x000e
#define WR8					0x0010
#define WR9					0x0012
#define WR10				0x0014
#define WR11				0x0016
#define WR12				0x0018
#define WR13				0x0020

#define MAXCARDNUMBER		16

INT16S max_card_number=MAXCARDNUMBER;
INT16U CardIOAddress[MAXCARDNUMBER]=
{
	0x0000,0x0001,0x0002,0x0003,
	0x0004,0x0005,0x0006,0x0007,
	0x0008,0x0009,0x000a,0x000b,
	0x000c,0x000d,0x000e,0x000f
};


extern 	int		lTagCount;
extern	int		lTagLastCount;					//缓存弹簧条数计数



void	CtrlCard_Init()
{
	int i = 0;

	for(i=0; i<CHMAX; i++)
	{
		g_fPulseOfMM[i]	= 20.0;
		g_fPulseOfAngle[i]	= 20.0;
		g_lPulseOfCircle[i]= 7200;
		g_iCardBeiLv[i] = 1;
		g_iCardStopFlag[i] = 0;
	}

	g_ulOutStatus = 0;
	g_ucAxisSwitch=1*1+g_Ini.iAxisSwitch[1]*2+g_Ini.iAxisSwitch[2]*4+g_Ini.iAxisSwitch[3]*8+g_Ini.iAxisSwitch[4]*8+g_Ini.iAxisSwitch[5]*8;
}



void	CtrlCard_Destroy()
{

}


BOOL  InitBoard()
{
	INT16S j;
	long pos;

	motion_init();
	Enable_ADT834INT();
	
	BaseSetCommandPos(1,10L);
	BaseGetCommandPos(1,&pos);
	
	vm_motion_init();															//初始化运动
	
	if(pos!=10L)
	{
		Uart_Printf("Card Init Err,pos=%ld\n",pos);
		return FALSE;
	}
	
	for(j=0; j<CHMAX; j++)
	{
	//	if(j==1)
			BaseSetCommandPos(j+1,0);
	//	else
	//		BaseSetCommandPos(j+1,ReadPosFromRam(j+1));
		
		//Uart_Printf("ReadPosFromRam(%d) = %d\n",j,ReadPosFromRam(j+1));
		
		set_actual_pos(j+1,0);
		
		SetCommandPos(j+1,0);
		SetActualPos(j+1,0);
		
		//SetUnitPulse(j+1,g_Ini.lMM[j]*0.1,g_Ini.lPulse[j]); 						//设定各轴脉冲当量
		if(j<MAXCH)
		{
			SetUnitPulse(j+1,g_Ini.fMM[j],g_Ini.lPulse[j]); 						//设定各轴脉冲当量
			set_pulse_mode(j+1,1,0,g_Ini.iRunDir[j]);
		}
		
		set_startv(j+1,1000);
		set_speed(j+1,1000);
		set_acc(j+1,1000);
		set_limit_mode(j+1,1,1,0);                                             //设置限位无效 20141127
	}
	
//	PWM_Init();
	return TRUE;
}

BOOL  InitBoardPara(void)
{
	INT16S j;

	for(j=0; j<MAXCH; j++)
	{
		SetUnitPulse(j+1,g_Ini.fMM[j],g_Ini.lPulse[j]); 						//设定各轴脉冲当量
		set_pulse_mode(j+1,1,0,g_Ini.iRunDir[j]);
	}
	
	return TRUE;
}

int vm_sync(void)							//该函数在工程应用中须使用到，从THMotion移到 ctrlcard.c里
{
	volatile int i;

//Uart_Printf("VM sync\r\n");
	memset(&VM,0x00,sizeof(VM));

	vm_efunc_reset();
	SYS_State=VM_IDLE;
	SYS_ErrNo=0;


	for(i=Ax;i<MAXAXIS;i++)
	{
		VM.Amax[i]=3000;
		
		VM.SpdMax[i]=((FP32)g_Ini.lMaxSpeed[i]/(g_Ini.lGearRate[i])*g_Ini.lPulse[i]/60.0/1000.0);		//最大速度限定单位KP/S
	
		if(i==Ax)
		{
			if(VM.SpdMax[i]>400.0)//限制最大速度 400    20141117
				VM.SpdMax[i] = 400.0;
		}
		else
		{
			if(VM.SpdMax[i]>250.0)//限制最大速度 250    20150625
				VM.SpdMax[i] = 250.0;
		}
		
		SYS_acc(i)=(float)g_Ini.lChAddSpeed[i];						//加速度
		SYS_dec(i)=(float)g_Ini.lChAddSpeed[i];						//减速度    注：这两个值必须一样。

		SYS_Rate2[i]=1.;
	}

	SYS_Rate1=1;
	SYS_Rate2[INPA]=1.;

	SYS_acc(INPA)=(float)g_Ini.lChAddSpeed[0];						//插补轴的加速度
	SYS_dec(INPA)=(float)g_Ini.lChAddSpeed[0];						//插补轴的减速度， 注：这两个值必须一样。

//	Uart_Printf("g_Ini.lChAddSpeed[0]\r\n",g_Ini.lChAddSpeed[0]);
	
	VM.SpdMax[INPA]=1000;											//插补轴的最大速度
		
	BaseGetCommandPos(Ax+1,&lastpulse[Ax]);
	BaseGetCommandPos(Ay+1,&lastpulse[Ay]);
	BaseGetCommandPos(Az+1,&lastpulse[Az]);
	BaseGetCommandPos(Aa+1,&lastpulse[Aa]);
	BaseGetCommandPos(Ab+1,&lastpulse[Ab]);
	BaseGetCommandPos(Ac+1,&lastpulse[Ac]);
	
	SYS_RMacPos[Ax]=lastpulse[Ax]/1000.;
	SYS_RMacPos[Ay]=lastpulse[Ay]/1000.;
	SYS_RMacPos[Az]=lastpulse[Az]/1000.;
	SYS_RMacPos[Aa]=lastpulse[Aa]/1000.;
	SYS_RMacPos[Ab]=lastpulse[Ab]/1000.;
	SYS_RMacPos[Ac]=lastpulse[Ac]/1000.;


//////////////////////////////////////////////缓存条数初始化
	lTagCount=0;
	lTagLastCount = g_Ini.lRunCount+TAGBUFCOUNT;
	CurTag=g_Ini.lRunCount;
//////////////////////////////////////////////
	
	MPCount=0;
	
	if(0 == g_Ini.iSecondAxisStyle)							//0为从轴连续，1为从轴不连续
		SetMethodFlag(0);
	else if(1 == g_Ini.iSecondAxisStyle)	
		SetMethodFlag(1);	
	
	return 0;
}


static long BaseCount[CHMAX+1]={0};

int BaseSetCommandPos(int axis,long pos)
{
	long var;  
	
	get_command_pos(axis,&var);
	BaseCount[axis-1]=var-pos;  
	//Uart_Printf("testpoint501\n");
	return 0;
} 

int BaseSetCommandOffset(int axis,long posoffset)
{
 	BaseCount[axis]-=posoffset;
//	Uart_Printf("offset:%d base:%d\r\n",posoffset,BaseCount[axis]);
	return 0;
}

int BaseGetCommandPos(int axis,long *pos)
{
 	get_command_pos(axis,pos);
 	*pos=*pos-BaseCount[axis-1];
	
	return 0;
}



///---------处理滞后脉冲
static long BaseComCount[CHMAX+1]={0};
static long BaseActCount[CHMAX+1]={0};

int SetCommandPos(int axis,long pos)
{
	long var;  
	get_command_pos(axis,&var);
	BaseComCount[axis-1]=var-pos;  
	return 0;
}

long GetCommandPos(int axis)
{
	long pos;
	
	get_command_pos(axis,&pos);
	pos=pos-BaseComCount[axis-1];
	return pos;
}

int SetActualPos(int axis,long pos)
{
	long var;  
	get_actual_pos(axis,&var);
//	var = -var;
	BaseActCount[axis-1]=var-pos;  
	return 0;
}

long GetActualPos(int axis)
{
	long pos;
	
	get_actual_pos(axis,&pos);
//	pos = -pos;
	pos=pos-BaseActCount[axis-1];
	return pos;
}
///---------处理滞后脉冲


BOOL  PulseLagCheck(INT16U axis)
{
	if(axis>CHMAX)
		return FALSE;
		
	if(g_Ini.iLag[axis-1]>0 && (labs(GetCommandPos(axis)*g_Ini.iNumerator[axis-1]-GetActualPos(axis)*g_Ini.iDenominator[axis-1]) >= g_Ini.iLag[axis-1]))
		return TRUE;
	else
		return FALSE;
}

void	SetUnitPulse(INT16S ch,FP32 mm,INT32S pulse)
{
	if( mm < 0.001 ) 
		mm = 1.0;
		
	if( pulse < 200) 
		pulse = 200;
		
	g_fPulseOfMM[ch-1] = (FP64)(pulse)/mm;
	g_fPulseOfAngle [ch-1] = (FP64)(pulse)/360.0;
	g_lPulseOfCircle[ch-1] = pulse;
//	vm_setrange2(ch,8000000L/(INT32S)g_iCardBeiLv[ch-1]);
}


FP32	GetUnitPulse(INT16S ch)
{
	return g_fPulseOfMM[ch-1];
}


void	SetMMConSpeed(INT16S ch,FP32 mmspeed)
{
	INT32S pulse;
	
	INT32S speed;

	if(ch>CHMAX)
		return;

	pulse = (INT32S)(g_fPulseOfMM[ch-1]*mmspeed);
	speed = pulse/(INT32S)g_iCardBeiLv[ch-1];
	speed = speed<1?1:speed; 													//add 12.17
	 													
	set_startv(ch,speed);
	set_speed(ch,speed);
	set_acc(ch,32000);
}


void	SetMMAddSpeed(INT16S ch,FP32  mmstartspeed,FP32 mmspeed,FP32 mmaddspeed)
{
	INT32S startspeed;
	INT32S speed;
	INT32S addspeed;

	if(ch>CHMAX)
		return;
		
	if(mmstartspeed>mmspeed)
		mmstartspeed=mmspeed;
		
	startspeed = (INT32S)(g_fPulseOfMM[ch-1]*mmstartspeed);
	startspeed = startspeed/(INT32S)g_iCardBeiLv[ch-1];
	startspeed = startspeed<10?10:startspeed; 	

	speed = (INT32S)(g_fPulseOfMM[ch-1]*mmspeed);
	speed = speed/(INT32S)g_iCardBeiLv[ch-1];
	speed = speed<1?1:speed; 	
	
	addspeed = (INT32S)(g_fPulseOfMM[ch-1]*mmaddspeed);
	addspeed = addspeed/(INT32S)g_iCardBeiLv[ch-1]/250.0;		//注加速度要除于250倍才是想要的速度
	addspeed = addspeed<250?250:addspeed; 

//	Uart_Printf("startspeed==%d,speed=%d,addspeed=%d\n",startspeed,speed,addspeed);
	
	set_startv(ch,startspeed);
	set_speed(ch,speed);
	set_acc(ch,addspeed);
}


void	SetPulseConSpeed(INT16S ch,INT32S speed)
{
	INT32S speed1;

	if(ch>CHMAX)
		return;

	speed1 = speed/(INT32S)g_iCardBeiLv[ch-1];
	
	set_startv(ch,speed1);
	set_speed(ch,speed1);
	set_acc(ch,32000);
}

void	SetMM(INT16S ch,FP32 mm,BOOL cur)
{
	if(ch>CHMAX)//&& ch!=6)
		return;

	cur==TRUE?
		BaseSetCommandPos(ch,GetPulseFromMM(ch,mm)):
		set_actual_pos(ch,GetPulseFromMM(ch,mm));
		
		
	if(ch ==2 && fabs(mm)<0.001 && cur ==TRUE)				//正常加工结束缓存当前行设为0;	
	{														
		CurIndex = 0;										//回零时缓存当前行设为0;		
	//	lDisYDceLong = 0;									//回零时缓存当前行设为显示偏移为0;
	//	PreCurYlong = 0;
		lYProbelong = 0;
		lYProbelong1 = 0;

	}
}


void	SetAngle(INT16S ch,FP32 angle,BOOL cur)
{
	if(ch>CHMAX && ch!=6)
		return;
		
	cur==TRUE?
		BaseSetCommandPos(ch,angle*g_fPulseOfAngle[ch-1]):
		set_actual_pos(ch,angle*g_fPulseOfAngle[ch-1]);
}


void	SetPulse(INT16S ch, INT32S pos,BOOL cur)
{
	if(ch>CHMAX)// && ch!=7)
		return;
		
	cur==TRUE?
			BaseSetCommandPos(ch,pos):
			set_actual_pos(ch,pos);
}


FP32	GetMM(INT16S ch,BOOL current)
{
	long p1=0;

	if(ch>CHMAX)//&& ch!=6)
		return 0.0;

	if(current)
		BaseGetCommandPos(ch,&p1);
	else
		get_actual_pos(ch,&p1);

	return GetMMFromPulse(ch,p1);
}


FP32	GetAngle(INT16S ch,BOOL cur)
{
	long pulse;

	if(ch>CHMAX)//&& ch!=6)
		return 0.0;

	cur == TRUE ?
		BaseGetCommandPos(ch,&pulse):
		get_actual_pos(ch,&pulse);

	return GetAngleFromPulse(ch,pulse);
}


INT32S    GetPulse(INT16S ch,BOOL cur)
{
	long p1=0;

//	if(ch>CHMAX && ch!=6)
	if(ch>CHMAX)
		return -1;

	if(cur)
		BaseGetCommandPos(ch,&p1);
	else
	{
		get_actual_pos(ch,&p1);
		
		if(ch == 7 || ch == 8)
			p1 = -p1;

	}

	return p1;
}


FP32	GetMMFromPulse(INT16S ch,INT32S pulse)
{
	if(ch > CHMAX)
		return 0.0;
		
	return  (float)(pulse)/g_fPulseOfMM[ch-1];
}


INT32S	GetPulseFromMM(INT16S ch,FP32 mm)
{
	if(ch > CHMAX)
		return 0.0;
		
	if(mm*g_fPulseOfMM[ch-1]>0.000001)
		return (INT32S)(mm*g_fPulseOfMM[ch-1]+0.5);
	else
		return (INT32S)(mm*g_fPulseOfMM[ch-1]-0.5);
}


FP32	GetAngleFromPulse(INT16S ch,INT32S pulse)
{
	if(ch > CHMAX)
		return 0.0;
	
	pulse = (pulse)%g_lPulseOfCircle[ch-1];

	return (float)(pulse)/g_fPulseOfAngle[ch-1];
}


INT32S	GetPulseFromAngle(INT16S ch,FP32 angle)
{
	if(ch > CHMAX)
		return 0;
		
	if(angle*g_fPulseOfAngle[ch-1]>0.000001)
		return (INT32S)(angle*g_fPulseOfAngle[ch-1]+0.5);
	else
		return (INT32S)(angle*g_fPulseOfAngle[ch-1]-0.5);
}


FP32	GetMMFromAngle(INT16S ch,FP32 angle)
{
	if(ch > CHMAX)
		return 0;
		
	return g_fPulseOfAngle[ch-1]*angle/g_fPulseOfMM[ch-1];
}


FP32	GetAngleFromMM(INT16S ch,FP32 mm)
{
	if(ch > CHMAX)
		return 0;
		
	return g_fPulseOfMM[ch-1]*mm/g_fPulseOfAngle[ch-1];
}


void	Start( INT16S ch )
{
	if(ch > CHMAX)
		return;
		
	g_iCardStopFlag[ch-1]=0;
}


void	Stop1( INT16S ch )
{
	if(ch > CHMAX)
		return ;
		
	sudden_stop(ch);
	g_iCardStopFlag[ch-1] = 1;
}


void	Stop2( INT16S ch )
{
	if(ch > CHMAX)
		return;
		
	dec_stop(ch);
	g_iCardStopFlag[ch-1] = 2;
}


INT16S	IsFree(INT16S ch )
{
	int status;
	
	if(ch > CHMAX)
		return -1;
	
	get_status(ch,&status);

	return status==0?1:0;
}


INT16S 	IsInpEnd(INT16S no)
{
	int status=-1;
	
	get_inp_status(&status);
	return status==0?1:0;
}


INT16S 	IsNextInp(INT16S no) 
{
	int status=-1;
	
//	get_inp_status2(&status);
	unsigned int fifonum = 0;
//  read_fifo(&fifonum);
    status = fifonum&0x1000>>8;
//  fifonum = fifonum&0x0fff;
//  status  = fifonum==0?1:0;
	return status==1?1:0;
}


INT16U   GetStopData(INT16S ch)
{
	int value=0;
	
	if(ch>CHMAX)return 0;
//	get_stopdata(ch,&value);
	return value;
}


float  GetInpSpeed()
{
	long	speed;
	get_speed(1,&speed);
	return (float)speed/g_fPulseOfMM[0];
}


void	ShortlyPmove(INT16S ch,INT32S pos,INT32S speed)
{
//	set_startv(ch,speed/(INT32S)g_iCardBeiLv[ch-1]);
//	set_speed(ch,speed/(INT32S)g_iCardBeiLv[ch-1]);
	g_lSpeed=speed/(INT32S)g_iCardBeiLv[ch-1];
	
//	if(g_bTestStart&&g_bHandRun)
	{
//		g_bTestStart=FALSE;
		set_startv(ch,(INT32S)((float)speed/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi));
		set_speed(ch,(INT32S)((float)speed/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi));
		set_acc(ch,speed);
	}
	pmove(ch,pos);

}

void	ShortlyInp_Move2(INT32S pos1,INT32S pos2,INT32S speed)
{
	g_lSpeed=speed/(INT32S)g_iCardBeiLv[1];
	
	//set_startv(2,(INT32S)((float)speed/(float)g_iCardBeiLv[1]*g_fSpeedBi));
	//set_speed(2,(INT32S)((float)speed/(float)g_iCardBeiLv[1]*g_fSpeedBi));
	SetPulseConSpeed(3,(INT32S)((float)speed*g_fSpeedBi));
	
	inp_move4(0,pos1,0,pos2);
}

void	ShortlyInPmove(INT16S ch,INT32S pos,INT32S speed)   
{
	/*
	speed*=2;
	g_lSpeed=speed/(INT32S)g_iCardBeiLv[ch-1]; //记录设定之运行速度	
	
	SetPulseConSpeed(1,(INT32S)((float)speed*g_fSpeedBi));
	switch(ch)
	{
	case 1:
		inp_move4(pos,0,0,0);break;
	case 2:
		inp_move4(0,pos,0,0);break;
	case 3:
		inp_move4(0,0,pos,0);break;
	case 4:
		inp_move4(0,0,0,pos);break;
	default:break;
	}
    */
}


void	ShortlyLine2l(INT32S pos1,INT32S pos2,INT32S speed)
{
	INT32S 	p1,p2,pp1=100,pp2=100;
	INT32S 	temppulse;
	INT32S 	maxpulse;
	INT32S	tempspeedpulse=speed;
	
	p1=labs(pos1);
	p2=labs(pos2);
	
	pp1=pos1;
	pp2=pos2;

	temppulse=p1>p2?p1:p2;
	
	if(p1!=0)
	{
		if(temppulse!=p1)
		{
			tempspeedpulse=(float)temppulse/(float)p1*(float)tempspeedpulse;
			
			if(temppulse==p2)
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
			
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
		}
	}
	else
	{
		if(p2!=0)
		{
			tempspeedpulse=speed;
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
				
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
		}
	}

	tempspeedpulse *= 2;
	
	set_startv(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	set_speed(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	
	g_lSpeed=tempspeedpulse/(INT32S)g_iCardBeiLv[0];

	if(g_bTestStart && g_bHandRun)
	{
		g_bTestStart=FALSE;
		set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
	}
	else
	{
		if(g_bHandRun)
		{
			set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
		}
		
/*		if(g_bTestStart)
		{
			int temp;
			
			if(g_iADValue!=-1 && g_iADValue!=2000)
			{
				temp=g_iADValue;
				
				if(temp>2 && temp<127)
				{
					set_speed(1,(INT32S)((float)g_lSpeed*(float)temp/126.0));
	//				set_startv(3,(INT32S)((float)g_lSpeed*(float)temp/126.0));
	//				set_speed(3,(INT32S)((float)g_lSpeed*(float)temp/126.0));
				}
			}
		}*/
	}
	
//	inp_dec_enable(1);
	inp_move2(1,2,pp1,pp2);
}


void	ShortlyLine3l(INT32S pos1,INT32S pos2,INT32S pos3,INT32S speed)
{
	INT32S p1,p2,p3,pp1=100,pp2=100,pp3=100;
	INT32S temppulse;
	INT32S maxpulse;
	INT32S	tempspeedpulse=speed;
	
	p1=labs(pos1);
	p2=labs(pos2);
	p3=labs(pos3);
	pp1=pos1;
	pp2=pos2;
	pp3=pos3;
	temppulse=p1>p2?p1:p2;
	temppulse=temppulse>p3?temppulse:p3;
	
	if(p1!=0)
	{
		if(temppulse!=p1)
		{
			tempspeedpulse=(float)temppulse/(float)p1*(float)tempspeedpulse;
			if(temppulse==p2) //Y is INT32S axis
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
			else
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
		}
	}//end if(p1!=0)
	else
	{
		if(p2!=0)
		{
			if(temppulse != p2)
			{
				tempspeedpulse=(float)temppulse/(float)p2*(float)tempspeedpulse; //speed change
				maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
			else
			{
				tempspeedpulse=speed;
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}//end if(p2!=0)
		else
		{
//			if(p3!=0)
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
		}
	}

	tempspeedpulse *= 2;
	
	set_startv(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	set_speed(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);

	g_lSpeed=tempspeedpulse/(INT32S)g_iCardBeiLv[0];

	if(g_bTestStart && g_bHandRun)
	{
		g_bTestStart=FALSE;
		set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
	}
	else
	{
		if(g_bHandRun)
		{
			set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
		}
/*		if(g_bTestStart)
		{
			int temp;
			if(g_iADValue!=-1&&g_iADValue!=2000)
			{
				temp=g_iADValue;
				if(temp>2&&temp<127)
				{
					set_speed(1,(INT32S)((float)g_lSpeed*(float)temp/126.0));
				}
			}
		}	*/
	}

	inp_move3(1,2,3,pp1,pp2,pp3);
}


void Move(int ch,INT32S pos,INT32S speed)
{
	set_startv(ch,(INT32S)(g_Ini.fStartSpeed*1000.0/60.0*GetUnitPulse(ch)/g_iCardBeiLv[ch-1]));
	set_speed(ch,(INT32S)((float)speed/g_iCardBeiLv[ch-1]));
	set_acc(ch,(INT32S)(g_Ini.fAddSpeed*1000.0/60.0*GetUnitPulse(ch)/g_iCardBeiLv[ch-1]/125));
	pmove(ch,pos);
}


void	ShortlyLine4l(INT32S pos1,INT32S pos2,INT32S pos3,INT32S pos4,INT32S speed)
{
	INT32S p1,p2,p3,p4,pp1=100,pp2=100,pp3=100,pp4=100;
	INT32S temppulse;
	INT32S maxpulse;
	INT32S	tempspeedpulse=speed;
	
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif
	
	p1=labs(pos1);
	p2=labs(pos2);
	p3=labs(pos3);
	p4=labs(pos4);
	
	pp1=pos1;
	pp2=pos2;
	pp3=pos3;
	pp4=pos4;
	
	temppulse=p1>p2?p1:p2;
	temppulse=temppulse>p3?temppulse:p3;
	temppulse=temppulse>p4?temppulse:p4;

	if(p1!=0)
	{
		if(temppulse!=p1)
		{
			tempspeedpulse=(float)temppulse/(float)p1*(float)tempspeedpulse;
			if(temppulse==p2) //X is INT32S axis
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[0]/g_Ini.lGearRate[0]*g_Ini.lMaxSpeed[0]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
			else
			{
				if(temppulse==p3)	//Z is INT32S axis
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{     //U is INT32S axis
					maxpulse=(INT32S)((float)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}
	}//end if(p1!=0)
	else
	{
		if(p2!=0)
		{
			if(temppulse != p2)
			{
				tempspeedpulse=(float)temppulse/(float)p2*(float)tempspeedpulse; //speed change
				if(temppulse == p3)
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
			else
			{
				tempspeedpulse=speed;
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[0]/g_Ini.lGearRate[0]*g_Ini.lMaxSpeed[0]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}//end if(p2!=0)
		else
		{
			if(p3!=0)
			{
				if(temppulse != p3)
				{
					tempspeedpulse=(float)temppulse/(float)p3*(float)tempspeedpulse; //speed change
					maxpulse=(INT32S)((float)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					maxpulse=(INT32S)((float)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
			else
			{
				maxpulse=(INT32S)((float)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
			}
		}
	}	

	tempspeedpulse *= 2;
	
	set_startv(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	set_speed(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	SetPulseConSpeed(3,tempspeedpulse);
	
	OS_ENTER_CRITICAL();
	g_lSpeed=tempspeedpulse/(INT32S)g_iCardBeiLv[0];
	OS_EXIT_CRITICAL();
	
	if(g_bTestStart && g_bHandRun)
	{
		g_bTestStart=FALSE;
		set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
		SetPulseConSpeed(3,(INT32S)((float)tempspeedpulse*g_fSpeedBi));
	}
	else
	{
		if(g_bHandRun)
		{
			set_speed(1,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[0]*g_fSpeedBi));
			SetPulseConSpeed(3,(INT32S)((float)tempspeedpulse*g_fSpeedBi));
		}
/*		if(g_bTestStart)
		{
			int temp;
			if(g_iADValue!=-1&&g_iADValue!=2000)
			{
				temp=g_iADValue;
				if(temp>2&&temp<127)
				{
					set_speed(1,(INT32S)((float)g_lSpeed*(float)temp/126.0));
					set_startv(3,(INT32S)((float)g_lSpeed*(float)temp/126.0));
					set_speed(3,(INT32S)((float)g_lSpeed*(float)temp/126.0));
				}
			}
		}*/
	}	

/*	if(g_Ini.iAxisSwitch[0])
	{	
		inp_move3(1,2,3,pos1,pos2,pos3);
	}
	else
	{
		inp_move3(2,3,4,pos2,pos3,pos4);
	}*/
	
	inp_move4(pos1,pos2,pos3,pos4);
}


void	ShortlyLine4(INT32S pos1,INT32S pos2,INT32S pos3,INT32S pos4,INT32S speed)
{
	int  ch=1;
	INT32S	tempspeedpulse=speed*2;

	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif

	ch = (g_Ini.iAxisSwitch[3]&&!g_Ini.iAxisSwitch[0])?2:1;
	
	set_startv(ch,(INT32S)(g_Ini.fStartSpeed*1000.0/60.0*GetUnitPulse(ch)/g_iCardBeiLv[ch-1]));
	set_speed(ch,(INT32S)((float)tempspeedpulse/g_iCardBeiLv[ch-1]));
	set_acc(ch,(INT32S)(g_Ini.fAddSpeed*1000.0/60.0*GetUnitPulse(ch)/g_iCardBeiLv[ch-1]/125));

	OS_ENTER_CRITICAL();
	g_lSpeed=tempspeedpulse/(INT32S)g_iCardBeiLv[ch-1];
	OS_EXIT_CRITICAL();
	
	if(g_bTestStart&&g_bHandRun)
	{
		g_bTestStart=FALSE;
		set_speed(ch,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi));
//		speedfifo = (INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi);
//		SetPulseConSpeed(3,(INT32S)((float)tempspeedpulse*g_fSpeedBi));
	}
	else
	{
		if(g_bHandRun)
		{
			set_startv(ch,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi));
			set_speed(ch,(INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi));
//			speedfifo = (INT32S)((float)tempspeedpulse/(float)g_iCardBeiLv[ch-1]*g_fSpeedBi);
		}
	}
	
	set_startv(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	set_speed(1,tempspeedpulse/(INT32S)g_iCardBeiLv[0]);
	inp_move4(pos1,pos2,pos3,pos4);

}


void	GotoAngle(INT16S ch,FP32 angle,FP32 speed,FUNCTION pfn)
{
	MoveAngle(ch,angle-GetAngle(ch,TRUE),speed,pfn);
}


void	MoveAngle(INT16S ch,FP32 angle,FP32 speed,FUNCTION pfn)
{
	int status=-1;
	SetMMConSpeed(ch,speed);
	pmove(ch,GetPulseFromAngle(ch,angle));
	while(status)
	{
		get_status(ch,&status);
		if(pfn)pfn();
	}
}


void	ShortlyMoveAngle(INT16S ch,FP32 angle,FP32 speed)
{
	INT32S p = 0;
	
	
	p = GetPulseFromAngle(ch,angle);
	SetMMConSpeed(ch,speed);
	pmove(ch,p);
}

void	MoveMM(INT16S ch,FP32 pos,FP32 speed,FUNCTION pfn)
{
	INT32S p;
	int status=-1;
	
	if(ch>CHMAX)return;
	
	SetMMConSpeed(ch,speed);
	
	p=(INT32S)(pos*g_fPulseOfMM[ch-1]);
	pmove(ch,p);
	
	while(status)
	{
		get_status(ch,&status);
		
		if(pfn)pfn();
		
		OSTimeDly(1);
	}
}



void	AddMoveMM(INT16S ch,FP32 pos,FP32 startspeed,FP32 speed,FP32 addspeed)
{
	INT32S p;
	
	if(ch>CHMAX)return;
	
	SetMMAddSpeed(ch,startspeed,speed,addspeed);
	
	p=(INT32S)(pos*g_fPulseOfMM[ch-1]);
	
	pmove(ch,p);
}


void	AddMoveMM1(INT16S ch,FP32 pos,FP32 startspeed,FP32 speed,FP32 addspeed)
{
	INT32S p;

	if(ch>CHMAX)return;
	
	SetMMAddSpeed(ch,startspeed,speed,addspeed);
	p=(INT32S)(pos*g_fPulseOfMM[ch-1]);
	pmove(ch,p);
}


void	FifoMoveMM(INT16S ch,FP32 pos,FP32 mmspeed)
{
	INT32S 	p;
	INT32S	speed;
	long pos1=0;
	long pos2=0;
	long pos3=0;
	long pos4=0;

	if(ch>CHMAX)return;
	
	
	
	speed = (INT32S)(g_fPulseOfMM[ch-1]*mmspeed);
	speed = speed/(INT32S)g_iCardBeiLv[ch-1];
	speed = speed<1?1:speed; 
	
	p=(INT32S)(pos*g_fPulseOfMM[ch-1]);
	
	switch(ch)
	{
		case 1:
			pos1=p;
			break;
		case 2:
			pos2=p;
			break;
		case 3:
			pos3=p;
			break;
		case 4:
			pos4=p;
			break;
		default:
			break;
	}

//	fifo_inp_move1(ch,p,speed*4);						//发该函数VMmotion.c会改变 vm_getindex()，故改为fifo_index_inp_move4并将其index参数设置为0
	
	fifo_index_inp_move4(vm_getindex(),pos1,pos2,pos3,pos4,speed*4);

}




void	ShortlyMoveMM(INT16S ch,FP32 pos,FP32 speed)
{
	INT32S p;
	if(ch>CHMAX)return;
//	Start(ch);
	SetMMConSpeed(ch,speed);
	p=(INT32S)(pos*g_fPulseOfMM[ch-1]);
	pmove(ch,p);
}


BOOL CheckHome(INT16S ch)
{
	char nD[]={XZERO,YZERO,ZZERO,UZERO,VZERO,WZERO};
	
	if(ch>CHMAX+1)
		return FALSE;
		
	return read_bit(nD[ch-1])?FALSE:TRUE;
}


BOOL	TestHome(INT16S ch)
{
	INT16S workstep = 0;
	int status=0;
	INT16S logic=0;
	INT16S value=-1;
	INT32S DestPulse = 100000000L;
	INT16U key;
	INT16U dir = g_Ini.iBackDir[ch-1];
	//INT16S stopdata = -1;
	
	if(ch>CHMAX)
		return FALSE;

	SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]);
	Start(ch);
	
	while( TRUE )
	{
		switch( workstep )
		{
		case 0:
			if( CheckHome(ch) )
			{
				workstep = 3;
				break;
			}
			set_stop0_mode(ch,1,logic);
			pmove(ch,dir?-DestPulse:DestPulse);
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:
			get_status(ch,&status);
			
			if( status ) break;
	
			if(CheckHome(ch))
				value=2;
			else
				value=0;
			if(value==0)
			{
				set_stop0_mode(ch,0,logic);
				return FALSE;
			}
			workstep ++;
			break;
	   case 3:			
			SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/10.0);
			set_stop0_mode(ch,1,logic?0:1);
			pmove(ch,dir?DestPulse:-DestPulse);
			workstep ++;
			workstep ++;
			
	   case 5:
			get_status(ch,&status);
			if( status ) break;

			if(!CheckHome(ch))
				value=2;
			else
				value=0;
			if(value==0)
			{
				set_stop0_mode(ch,0,logic);
				return FALSE;
			}
			workstep++;
			SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/10.0);			
			set_stop0_mode(ch,1,logic);
			pmove(ch,dir?-DestPulse:DestPulse);			
			break;

	   case 6:
			get_status(ch,&status);

			if( status ) break;

			if(CheckHome(ch))
				value=2;
			else
				value=0;
			if(value==0)
			{
				set_stop0_mode(ch,0,logic);
				return FALSE;
			}
			workstep++;
			set_stop0_mode(ch,0,logic);
			BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
			OSTimeDly(40);
			break;
			
		case 7:
			
			if((1==ch && g_Ini.iSearchZero[0]==1) || (3==ch && g_Ini.iSearchZero[2]==1) || (4==ch && g_Ini.iSearchZero[3]==1))//回伺服原点
			{
				set_stop1_mode(ch,1,logic);
				SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/2.0);
				OSTimeDly(20);
				pmove(ch,dir?-DestPulse:DestPulse);
			}
			else
			{
				workstep ++;
			}
			
			workstep++;
			break;
			
		case 8:
			get_status(ch,&status);
			
			if(status)
				break;
				
			if(g_iErrorNo || g_iCardStopFlag[ch-1]!=0)
			{
				set_stop1_mode(ch,0,logic);
				return FALSE;
			}
			else
			{
				workstep++;
				set_stop1_mode(ch,0,logic);
				break;
			}
			
		case 9:
		
			BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
			OSTimeDly(80);
			
			{
				/*FP32 pos=0.0;
				
				if(g_Ini.iAxisRunMode[ch-1]==0 || g_Ini.iAxisRunMode[ch-1]==1 )
				{
					pos=g_Ini.fStartOffset[ch-1]/10.0;
				}
				else if(g_Ini.iAxisRunMode[ch-1]==2)
				{
					pos=g_Ini.fStartOffset[ch-1]/100.0;
				}*/
				
				ShortlyMoveAngle(ch,g_Ini.fStartOffset[ch-1],g_Ini.fBackSpeed[ch-1]);
			}
			
			while(!IsFree(ch))
			{
				OSTimeDly(1);
			}

			if(g_iCardStopFlag[ch-1]!=0)
			{
			   return FALSE;
			}
			else
			{
				BaseSetCommandPos(ch,0);
//				set_actual_pos(ch,0);
			}
			return TRUE;
	   }
	   	
	  	key=KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			if(key == TSTOP)
			{
				Stop1(ch);
				break;
			}
		}

		if(RealIO(STOPIO,g_Ini.StopLevel,2))
		{
			Stop1(ch);
			break;
		}
		
	   	OSTimeDly(2);	   
	}
	
//BACKZEROEND:

	set_stop0_mode(ch,0,logic);
	set_stop1_mode(ch,0,logic);
	
	return FALSE;

}




BOOL 	Home(INT16S ch, INT16S dir)
{
	INT8U err;
	INT16S workstep =0;
	//INT16S workstep1=3;
	int status=0;
	INT16S logic=0;
	INT16S value=-1;
	INT32S DestPulse;
    INT16U sta=0x00ff;
	BOOL	bDriver=FALSE;
	INT32S pulse;
	INT16S  tempio;
	Start(ch);

	if(g_Ini.iRunDir[ch-1]==1)
	     dir = dir?0:1;

	DestPulse=(INT32S)(365*g_fPulseOfAngle[ch-1]);

	if(ch==3 ||ch==4)
	    DestPulse=1000000L;

	if(ch==3)
	    tempio=ZLMTMINUS;

	if(ch==4)
	    tempio=ULMTMINUS;
	    
																		//不可直接调用本类中的速度设定函数,防止g_Ini.Dir的重复使用
	pulse = (INT32S)(g_fPulseOfMM[ch-1]*g_Ini.fBackSpeed[ch-1]);

	set_startv(ch,pulse/g_iCardBeiLv[ch-1]);
	set_speed(ch,pulse/g_iCardBeiLv[ch-1]);

	while( TRUE )
	{
		switch( workstep )
		{
		case 0:															//检查已经信号
			{
				if( CheckHome(ch) )
				{														//有信号
					workstep = 5;
					break;
				}
				set_stop0_mode(ch,1,logic);
				pmove(ch,dir?-DestPulse:DestPulse);
			}
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:			
			get_status(ch,&status);
			
			if( status ) 
				break;
				
			sta=0x00ff;
			
			if(CheckHome(ch))
				value=1;
			else
			{
				if(ch==3 ||ch==4)
				{
    				if(!ReadBit(tempio) || !ReadBit(tempio+1))	
    					value=8;	
    				else
    					value=0;
				}
				else
				    value=0;
			}
			sta = sta & (INT16U)value;
			
			if(sta!=1)															//判断停止信息是否因stop1停止  //2轴为stop0停止
			{
				if(ch==3 ||ch==4)
				{
    				if( sta==8 ) 												//仅第2,3轴检测是否为限位停止之驱动
    				{
    					workstep++;  
    					break;
    				}
    				else
    				{
    					goto HOMEEND;				
    				}
				}
				else
				    goto HOMEEND;	
			}			
			workstep +=3;
			break;
			
	   case 3:
			if(sta==8)
			{
				bDriver=TRUE;  													//指示2轴已经驱动
				pmove(ch,dir?DestPulse:-DestPulse);
				
			}
			workstep ++;
			
		case 4:
			get_status(ch,&status);
			
			if((bDriver?status:0))
				break;
				
			sta=0x00ff;
			
			if(CheckHome(ch))
				value=1;
			else
				value=0;
				
			sta=sta&(INT16U)value;
			
			if((bDriver?sta!=1:0))
			{
				goto HOMEEND;
			}
			workstep++;
			break;
			
	   case 5:	   		
    		 {
    			SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/10.0);
    			set_stop0_mode(ch,1,logic?0:1);
    			pmove(ch,dir?DestPulse:-DestPulse);
    			workstep ++;
			}

	   case 6:	   		
			get_status(ch,&status);
		
			if( status ) 
				break;	
				
			sta=0x00ff;
			
			if(!CheckHome(ch))
				value=1;
			else
				value=0;
				
			sta = sta & (INT16U)value;
			
			if(sta!=1)
			{
				goto HOMEEND;				
			}
												
			SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/10.0);
			set_stop0_mode(ch,1,logic);
			pmove(ch,dir?-DestPulse:DestPulse);
			workstep++;

	   case 7:
			{
    			get_status(ch,&status);
    			
    			if( status ) 
    				break;	
    			sta=0x00ff;
    			
    			if(CheckHome(ch))
    				value=1;
    			else
    				value=0;
    				
    			sta = sta & (INT16U)value;
    			
    			if(sta!=1)
    			{
    				goto HOMEEND;				
    			}							
				workstep ++;
				set_stop0_mode(ch,0,logic);
				BaseSetCommandPos(ch,0);						
//				set_actual_pos(ch,0);
				OSTimeDly(20);													//must
				OSSemSet(FreeSem,0,&err);
				return TRUE;
			}
		}
	   
		OSTimeDly(2);
		
		if( g_iCardStopFlag[ch-1] != 0)
		{
			break;
		}
	}
	
HOMEEND:
	set_stop0_mode(ch,0,logic);	
	OSSemSet(FreeSem,0,&err);
	return FALSE;
}


BOOL	BackZero(INT16S ch,INT16S dir)
{
	INT16S 	workstep = 0;
	int 	status=0;
	INT16S 	logic=0;
	int 	value=0;											//检查到正确状态:0无，1为有。
	long 	DestPulse = 100000000L, DestPulse1 = 100000000L;
	long  lCompPulse = 0;
	long  lCompPulse1 = 0;

	
	if(ch>CHMAX)
		return FALSE;
		
	if(g_Ini.iAxisRunMode[ch-1]!=2)
	{
		DestPulse1=GetPulseFromAngle(ch,365);
		DestPulse =GetPulseFromAngle(ch,365);
	}
	
	SetMMAddSpeed(ch,1,g_Ini.fBackSpeed[ch-1],g_Ini.fBackSpeed[ch-1]*200.0);

	Start(ch);
	
	if(g_Ini.iSearchZero[ch-1] == 1 || g_Ini.iSearchZero[ch-1] == 3)
		workstep = 0;
	else if(g_Ini.iSearchZero[ch-1] == 2)
		workstep = 7;
	else if(g_Ini.iSearchZero[ch-1] == 0)
		workstep = 9; 
	
	while( TRUE )
	{
		switch( workstep )
		{
		case 0:
			if( CheckHome(ch) )
			{
				workstep = 3;
				break;
			}
			
		//	set_stop0_mode(ch,1,logic);
			set_stop0_mode(ch,0,logic);							//改成无效
			pmove(ch,dir?-DestPulse:DestPulse);
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:
		
			if((CheckHome(ch) ||  HomeCheckLimit(ch,dir)) && value==0)  						//到原点
			{
				value =1;
				dec_stop(ch);
			}
			
			get_status(ch,&status);
			
			if( status )
			{
				break;
			}
			
			
			if(HomeCheckLimit(ch,dir))							//住正向碰到正限位时，或往负向碰到负限位时，往反方向搜原点
			{
				workstep=13;
				break;
			}
			
			
			
			if(CheckHome(ch))									//高速接近原点不能过冲
				value=1;
			else
				value=0;
				
			if(value == 0)
			{
				set_stop0_mode(ch,0,logic);
				return FALSE;
			}
			value =0;											//检查到原点状态需置0
			workstep ++;
			break;

		case 13:												//遇限位往反方向搜原点	
			SetMMAddSpeed(ch,1,g_Ini.fBackSpeed[ch-1],g_Ini.fBackSpeed[ch-1]*200.0);
			pmove(ch,dir?DestPulse:-DestPulse);					//反方向
			workstep ++;
			value =0;											//检查到原点状态需置0
		case 14:	
			if((CheckHome(ch) ||  HomeCheckLimit(ch,(dir?0:1))) && value==0)  		//到原点
			{
				value =1;
				dec_stop(ch);
			}
			
			get_status(ch,&status);
			
			if( status )
			{
				break;
			}
			
			if(CheckHome(ch))									//高速接近原点不能过冲,若是限位和其它停则出错
				value=1;
			else
				value=0;
				
			if(value == 0)
			{
				set_stop0_mode(ch,0,logic);
				return FALSE;
			}
			value =0;											//检查到原点状态需置0
			workstep =3;
			break;
			
	   case 3:			
			SetMMAddSpeed(ch,1,g_Ini.fBackSpeed[ch-1]/5.0,g_Ini.fBackSpeed[ch-1]*200.0);
		
		//	set_stop0_mode(ch,1,logic?0:1);
			set_stop0_mode(ch,0,logic?0:1);						//设置为无效

			pmove(ch,dir?DestPulse:-DestPulse);
			workstep ++;
			workstep ++;
			
	   case 5:
			
			if(!CheckHome(ch) && value==0)						//离开原点
			{
				dec_stop(ch);
				value =1;
			}
			
			get_status(ch,&status);
			
			if( status ) 
			{
				break;
			}

			if(value==0)
			{
				set_stop0_mode(ch,0,logic);
				Uart_Printf("zerotest3\n");
				return FALSE;
			}
			
			value = 0;
			
			workstep++;
			SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/5.0);	
			set_stop0_mode(ch,1,logic);
			BaseGetCommandPos(ch,&lCompPulse);
			pmove(ch,dir?-DestPulse:DestPulse);			
			break;

	   case 6:
			get_status(ch,&status);								//低速回原点后不在检测是否在原点

			if( status )
			{ 
				break;
			}
			
			BaseGetCommandPos(ch,&lCompPulse1);
			lCompPulse1 = labs(lCompPulse1-lCompPulse);
			if(labs(lCompPulse1-DestPulse)<2)
			{
				set_stop0_mode(ch,0,logic);
				Uart_Printf("zerotest4\n");
				return FALSE;
			}
			
			
			/*if(CheckHome(ch))									//低速接近原点
				value=2;
			else
				value=0;
				
			if(value==0)
			{
				set_stop0_mode(ch,0,logic);
				Uart_Printf("zerotest4\n");
				return FALSE;
			}*/
			
			workstep++;
			set_stop0_mode(ch,0,logic);
			
			BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
			
			OSTimeDly(40);
			
			break;
			
		case 7:
			if(g_Ini.iSearchZero[ch-1]==2 ||  g_Ini.iSearchZero[ch-1]==3)//回伺服原点
			{
			
				BaseSetCommandPos(ch,0);//搜零模式2直接到 第七步，要清坐标下，不然下面做比较脉冲会出错

				set_stop1_mode(ch,1,logic);
				
				SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]/5.0);
				OSTimeDly(20);
				
			//	pmove(ch,dir?-DestPulse1:DestPulse1);
				pmove(ch,DestPulse1);
			}
			else
			{
				workstep ++;
			}
			
			workstep++;
			break;
			
		case 8:
			get_status(ch,&status);
			
			if(status)
				break;
			
			if(labs(GetPulse(ch,TRUE))>=DestPulse1)
			{
				OSQPost(MsgQueue,g_bEnglish?"Can't find SevoZero!!!":"脉冲已发完未找到伺服原点!!!");
				OSTimeDly(1000);
				set_stop1_mode(ch,0,logic);
				return FALSE;
			}
			
			if(g_iErrorNo || g_iCardStopFlag[ch-1]!=0)
			{
				set_stop1_mode(ch,0,logic);
				return FALSE;
			}
			else
			{
				workstep++;
				set_stop1_mode(ch,0,logic);
				break;
			}

		case 9:
		
			BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
			OSTimeDly(80);
			
			if(g_Ini.iSearchZero[ch-1]!=0 )
			{
				/*FP32 pos=0.0;
				
				if(g_Ini.iAxisRunMode[ch-1]==0 || g_Ini.iAxisRunMode[ch-1]==1 )
				{
					pos=g_Ini.fStartOffset[ch-1]/10.0;
				}
				else if(g_Ini.iAxisRunMode[ch-1]==2)
				{
					pos=g_Ini.fStartOffset[ch-1]/100.0;
				}*/
				
			//	ShortlyMoveAngle(ch,g_Ini.fStartOffset[ch-1],g_Ini.fBackSpeed[ch-1]);
				
				AddMoveMM(ch,g_Ini.fStartOffset[ch-1],1.0,g_Ini.fBackSpeed[ch-1],g_Ini.fBackSpeed[ch-1]*25);

				
			}
			
			while(!IsFree(ch))
			{
				OSTimeDly(1);
			}
			
			
			//Uart_Printf("test3\n");
			if(g_iCardStopFlag[ch-1]!=0)
			{
			   return FALSE;
			}
			else
			{
				BaseSetCommandPos(ch,0);
//				set_actual_pos(ch,0);
			}
			
			return TRUE;
	   }
	   	
	   if(g_iCardStopFlag[ch-1]!=0)
	   {
		   break;
	   }
	   
	   OSTimeDly(3);	   
	}
	
//BACKZEROEND:
	set_stop0_mode(ch,0,logic);
	set_stop1_mode(ch,0,logic);
	
	return FALSE;
}


BOOL	TestBackZero(INT16S ch,INT16S dir)
{
	INT16S 	workstep = 0;
	int 	status=0;
	INT16S 	logic=0;
	INT16S 	value=-1;
	long 	DestPulse = 0, DestPulse1 = 0;
	INT16S 	ChkHomeTimes = 0;  //检测原点次数，若多次检测仍在原点，则返回错误
	INT16U 	key;
	
	if(ch>CHMAX)
		return FALSE;
		

	{
		DestPulse1=GetPulseFromAngle(ch,3650);
		DestPulse=GetPulseFromAngle(ch,3650);
	}
	
	SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]);
	Start(ch);
	
	while( TRUE )
	{
		switch( workstep )
		{
		case 0:
			if( CheckHome(ch) )
			{	
				if(ChkHomeTimes++ > 2)
				{
					return FALSE;
				}
				set_stop0_mode(ch,1,logic?1:0);
				pmove(ch,dir?-DestPulse:DestPulse);
				{

					while(1)
					{
						if(IsFree(ch))break;
						OSTimeDly(1);
					}
				}
				set_stop0_mode(ch,0,logic?1:0);
				break;
			}
			
			set_stop0_mode(ch,1,logic);
			pmove(ch,dir?-DestPulse:DestPulse);
			
			//Uart_Printf("test1\n");
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:
			get_status(ch,&status);
			
			if( status )
			{
				break;
			}
			
			//get_stopdata(CARD_NO,ch,&value);
			//Uart_Printf("value=%d\n",value);
			
			if( CheckHome(ch) )
			{
				value = 0;
			}
			else
				value = 1;
				
			//Uart_Printf("value=%d\n",value);
		
			if(value != 0)
			{
				set_stop0_mode(ch,0,logic);
				
				return FALSE;
			}
			workstep ++;
			//Uart_Printf("workstep=%d\n",workstep);
			break;
			
	   case 3:	
	   		//Uart_Printf("workstep=%d\n",workstep);
	   		set_stop0_mode(ch,0,logic);
	   		
	   		return TRUE;
	   }
	   
	   
	   OSTimeDly(2);
		key=KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			if(key == TSTOP)
			{
				Stop1(ch);
				break;
			}
		}
		
		if(RealIO(STOPIO,g_Ini.StopLevel,2)||(g_bHandset?RealIO(STOPIO2,0,3):0)||(g_bHandset?RealIO(SINGLEIO,0,3):0))
		{
			Stop1(ch);
			break;
		}
	   
	 }
	//TESTHOMEEND:
	set_stop0_mode(ch,0,logic);
	
	return FALSE;
	   		
}


BOOL	FindZero(INT16S ch,INT16S style, INT32S speed)
{
	INT32S temp;
	INT32S p = 0;

	if(ch>CHMAX)
	{
		return FALSE;
	}
	
	if(g_Ini.iSearchZero[ch-1] == 0)
	{
		BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
		return  TRUE;
	}
	
	
	SetPulseConSpeed(ch,speed);
	
	
	if(style == 2 )
		temp = GetPulse(ch, TRUE);
	else
		temp = GetPulse(ch, TRUE)%g_Ini.lPulse[ch-1];
	
	
	{
		if(temp==0)
		{
			BaseSetCommandPos(ch,0);
//			set_actual_pos(ch,0);
			return  TRUE;
		}
		
		Start(ch);
		
		if(style == 2)
			p  = 0 - temp;
		else
		{
			if(g_Ini.iBackMode[ch-1]==3)		//0 零向归零
			{
				p = 0-temp;
			}
			else if(g_Ini.iBackMode[ch-1]==1)	//+	正方向走
			{
				if(temp>0)
					p = g_Ini.lPulse[ch-1]-temp;
				else
					p = 0-temp;
			}
			else if(g_Ini.iBackMode[ch-1]==2)	//-	负方向走
			{
				if(temp>0)
					p = 0-temp;
				else
					p = -g_Ini.lPulse[ch-1]-temp;
			}
			else								//不归零模式回零和就近归零
			{
				
				if(fabs(temp)<g_Ini.lPulse[ch-1]*0.5)
				{
					p  = 0 - temp;
				}
				else
				{
					if(temp>0)
						p  = g_Ini.lPulse[ch-1]-temp;
					else
						p  = -g_Ini.lPulse[ch-1]-temp;
				}
			
			}		
		}
		
		pmove(ch,p);
		
		while (1)
		{
			if (IsFree(ch)) break;
			OSTimeDly(1);
		}
		
		if(g_iCardStopFlag[ch-1]!=0)
		{
			return FALSE;
		}
		
		BaseSetCommandPos(ch,0);
//		set_actual_pos(ch,0);
		
		return	TRUE;	

	}
}


BOOL	BackZero2(int dir1,int dir2,int step)
{
	
	INT8U  err;
	INT16S ch1=2,ch2=3;
	INT16S workstep = step;
	//INT16S workstep1=3;
	int status1=0,status2=0;
	INT16S logic=0;
	INT16S value1=-1,value2=-1;
	INT32S DestPulse1,DestPulse2;
    INT16U sta1=0x00ff,sta2=0x00ff;
	BOOL   bDriver2=FALSE;
	FP32   fspeed1,fspeed2;
	INT32S DestBack;
	
	
	if(g_Ini.iAxisSwitch[0])
	{
		ch2 = 1;
		fspeed2 = g_Ini.fBackSpeed[ch2-1];
		
	}
	else
	{
		if(g_Ini.iAxisSwitch[2])
			ch2 = 3;
		else 
		{
			if(g_Ini.iAxisSwitch[3])
				ch2 = 4;
		}
		
		fspeed2 = g_Ini.fBackSpeed[ch2-1];
	}
	
	fspeed1 = g_Ini.fBackSpeed[ch1-1]*6.0;
	
	if(g_Ini.iRunDir[ch1-1]==1)
	     dir1 = dir1?0:1;
	     
	if(g_Ini.iRunDir[ch2-1]==1)
	     dir2 = dir2?0:1;
	
	DestPulse1=1000000L;
	DestPulse2=1000000L;
	DestBack = GetPulseFromAngle(ch1,10);

	SetMMConSpeed(ch1,fspeed1);
	SetMMConSpeed(ch2,fspeed2);

	Start(ch1);
	Start(ch2);
																				//不可直接调用本类中的速度设定函数,防止g_Ini.Dir的重复使用
	while( TRUE )
	{
		switch( workstep )
		{
		case -2:
		      pmove(ch2,dir2?-DestPulse2:DestPulse2);
		      workstep ++;
		      
		case -1:
		      get_status(ch2,&status2);
		      if( status2 ) break;
		      workstep ++;
		      
		case 0:																	//检查已经信号
			{
				if( CheckHome(ch1)&&CheckHome(ch2) )							//有信号
				{
					workstep = 5;
					break;
				}
				
				set_stop1_mode(ch1,1,logic);
				set_stop0_mode(ch2,1,logic);				
				pmove(ch1,dir1?-DestPulse1:DestPulse1);
				pmove(ch2,dir2?-DestPulse2:DestPulse2);
			}
			
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:			
			get_status(ch1,&status1);
			get_status(ch2,&status2);
			
			if( status1 || status2) 
				break;
				
			sta1=0x00ff;sta2=0x00ff;
		
			value2=1;
			
			if(CheckHome(ch2))
				value2=1;
			else
			{
				if(g_Ini.iAxisSwitch[2])
				{
					if(!ReadBit(ZLMTMINUS) || !ReadBit(ZLMTPLUS))	
						value2=8;	
					else
						value2=0;
				}
				else
				{	
					value2 = 0;
				}
			}
	
			sta1=sta1&(INT16U)value1;
			sta2=sta2&(INT16U)value2;
		
			
			if(sta1!=1||sta2!=1)												//判断停止信息是否因stop1停止  //2轴为stop0停止
			{
				if( sta2==8 ) 													//仅第2,3轴检测是否为限位停止之驱动
				{
					workstep++;  
					break;
				}
				else
				{
					goto BACKZEROEND3;				
				}
			}			
			workstep +=3;
			break;
			
			
	   case 3:
			if(sta2==8)
			{
				bDriver2=TRUE;  												//指示3轴已经驱动
				pmove(ch2,dir2?DestPulse2:-DestPulse2);
				
			}
			
			if(sta1==8&&!g_Ini.iAxisSwitch[0])
			{
				bDriver2=TRUE;  												//指示2轴已经驱动
				pmove(ch1,dir1?DestPulse1:-DestPulse1);
			}
			workstep ++;
			
		case 4:
			get_status(ch2,&status2);
			
			if((bDriver2?status2:0))
				break;
				
			sta2=0x00ff;
			
			if(CheckHome(ch2))
				value2=1;
			else
				value2=0;
	
			sta2 = sta2 & (INT16U)value2;
			
			if((bDriver2?sta2!=1:0) )
			{
				goto BACKZEROEND3;
			}
			
			workstep++;
			
	   case 5:	   		
			 {
    			SetMMConSpeed(ch1,fspeed1/10.0);
    			SetMMConSpeed(ch2,fspeed2/10.0);
   
				set_stop1_mode(ch1,0,logic?0:1);
				set_stop0_mode(ch2,1,logic?0:1);				
				pmove(ch1,dir1?DestBack:-DestBack);
				pmove(ch2,dir2?DestPulse2:-DestPulse2);
	    		workstep ++;
			}

	   case 6:	   		
			get_status(ch1,&status1);
			get_status(ch2,&status2);	
				
			if( status1 || status2) 
				break;	

			sta1=0x00ff;
			sta2=0x00ff;
			
			if(!CheckHome(ch1))
				value1=1;
			else
				value1=0;
				
			if(!CheckHome(ch2))
				value2=1;
			else
				value2=0;
				
			sta1 = sta1&(INT16U)value1;
			sta2 = sta2&(INT16U)value2;
			
			if(sta1!=1 || sta2!=1)
			{
				goto BACKZEROEND3;				
			}			
			
    		SetMMConSpeed(ch1,5);
    		SetMMConSpeed(ch2,fspeed2/10.0);
		
			set_stop1_mode(ch1,1,logic);
			set_stop0_mode(ch2,1,logic);				
			pmove(ch1,dir1?-DestPulse1:DestPulse1);
			pmove(ch2,dir2?-DestPulse2:DestPulse2);	
			workstep++;
			
		case 7:
			{
    			get_status(ch1,&status1);
    			get_status(ch2,&status2);	
    			
    			if( status1 || status2) 
    				break;	
    				
    			sta1=0x00ff;
    			sta2=0x00ff;
    			
    			if(CheckHome(ch1))
    				value1=1;
    			else
    				value1=0;
    				
    			if(CheckHome(ch2))
    				value2=1;
    			else
    				value2=0;
    				
    			sta1 = sta1&(INT16U)value1;
    			sta2 = sta2&(INT16U)value2;
    			
    			if(sta1!=1||sta2!=1)
    			{
    				goto BACKZEROEND3;				
    			}	
    		
				workstep ++;
							
				set_stop1_mode(ch1,0,logic);
				set_stop0_mode(ch2,0,logic);
				BaseSetCommandPos(ch1,0);						
				BaseSetCommandPos(ch2,0);
//				set_actual_pos(ch1,0);
//				set_actual_pos(ch2,0);	
										
				OSTimeDly(20);
				OSSemSet(FreeSem,0,&err);
				return TRUE;
			}
		}
		
		OSTimeDly(2);
	
		if( g_iCardStopFlag[ch1-1] != 0 || g_iCardStopFlag[ch2-1] !=0)
		{
			break;
		}
	}
	
BACKZEROEND3:
	set_stop1_mode(ch1,0,logic);
	set_stop0_mode(ch2,0,logic);
	OSSemSet(FreeSem,0,&err);
	return FALSE;
}


BOOL	BackZero3(int dir1,int dir2,int dir3,int step)
{
	
	INT8U  err;
	INT16S ch1=2,ch2=3,ch3=4;
	INT16S workstep = step;
	//INT16S workstep1=3;
	int status1=0,status2=0,status3=0;
	INT16S logic=0;
	INT16S value1=-1,value2=-1,value3=-1;
	INT32S DestPulse1,DestPulse2,DestPulse3;
    INT16U sta1=0x00ff,sta2=0x00ff,sta3=0x00ff;
	BOOL   bDriver2=FALSE,bDriver3=FALSE;
	FP32   fspeed1,fspeed2,fspeed3;
	INT32S DestBack;
	
	if(g_Ini.iAxisSwitch[0])
	{
	   ch1=1;
		
	   if(g_Ini.iAxisSwitch[1])
	   {
	   		ch2=2;
	   		if(g_Ini.iAxisSwitch[2])
	   			ch3=3;
	   		else
	   		    ch3=4;
	   		    
	   		fspeed1 = g_Ini.fBackSpeed[ch1-1]*6.0;
		    fspeed2 = g_Ini.fBackSpeed[ch2-1]*6.0;
	 	    fspeed3 = g_Ini.fBackSpeed[ch3-1];
	   }
	   else
	   {
	   		ch2 =3;
	   		ch3 =4;
	   		fspeed1 = g_Ini.fBackSpeed[ch1-1]*6.0;
	   		fspeed2 = g_Ini.fBackSpeed[ch2-1];
	  		fspeed3 = g_Ini.fBackSpeed[ch3-1];
	   }		
		   
	   DestPulse1=GetPulseFromAngle(ch1,365);
	   
	}
	else
	{
		ch1=4;
		ch2=2;
		ch3=3;
		
	 	fspeed1 = g_Ini.fBackSpeed[ch1-1];
	 	fspeed2 = g_Ini.fBackSpeed[ch2-1]*6.0;
	 	fspeed3 = g_Ini.fBackSpeed[ch3-1];
	   
	 	DestPulse1=1000000L;
	}
	
	
	if(g_Ini.iRunDir[ch1-1]==1)
	     dir1 = dir1?0:1;
	     
	if(g_Ini.iRunDir[ch2-1]==1)
	     dir2 = dir2?0:1;
	     
	if(g_Ini.iRunDir[ch3-1]==1)
	     dir3 = dir3?0:1;
	
	DestPulse2= 1000000L;
	DestPulse3= 1000000L;
	DestBack  = GetPulseFromAngle(ch2,10);

	SetMMConSpeed(ch1,fspeed1);
	SetMMConSpeed(ch2,fspeed2);
	SetMMConSpeed(ch3,fspeed3);
	Start(ch1);
	Start(ch2);
	Start(ch3);
	
	while( TRUE )
	{
		switch( workstep )
		{
		case -2:
		      pmove(ch3,dir3?-DestPulse3:DestPulse3);
		      workstep ++;
		      
		case -1:
		      get_status(ch3,&status3);
		      
		      if( status3 ) 
		      	break;
		      	
		      workstep ++;
		      
		case 0:																	//检查已经信号
			{
				if( CheckHome(ch1)&&CheckHome(ch2)&&CheckHome(ch3) )			//有信号
				{
					workstep = 5;
					break;
				}

				set_stop0_mode(ch1,1,logic);
				set_stop1_mode(ch2,1,logic);
				set_stop0_mode(ch3,1,logic);				
				pmove(ch1,dir1?-DestPulse1:DestPulse1);
				pmove(ch2,dir2?-DestPulse2:DestPulse2);
				pmove(ch3,dir3?-DestPulse3:DestPulse3);
				
			}
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:			
			get_status(ch1,&status1);
			get_status(ch2,&status2);
			get_status(ch3,&status3);
			
			if( status1 || status2 || status3 ) 
				break;

			sta1 = 0x00ff;
			sta2 = 0x00ff;
			sta3 = 0x00ff;
			
			if(g_Ini.iAxisSwitch[0])
			{
				if(CheckHome(ch1))
					value1=1;
				else
					value1=0;
			}
			
			if(g_Ini.iAxisSwitch[3])
			{
				if(CheckHome(ch1))
					value1=1;
				else
				{
					if(!ReadBit(ULMTMINUS) || !ReadBit(ULMTPLUS))	
						value1=8;	
					else
						value1=0;
				}
			
			}

			if(g_Ini.iAxisSwitch[1])
				value2=1;
				
			if(g_Ini.iAxisSwitch[2])
			{
				if(CheckHome(ch3))
					value3=1;
				else
				{
					if(!ReadBit(ZLMTMINUS) || !ReadBit(ZLMTPLUS))	
						value3=8;	
					else
						value3=0;
				}
			}
			
			sta1 = sta1&(INT16U)value1;
			sta2 = sta2&(INT16U)value2;
			sta3 = sta3&(INT16U)value3;
			
			if(sta1!=1 || sta2!=1 || sta3!=1)									//判断停止信息是否因stop1停止  //2轴为stop0停止
			{
				if( sta3==8|| (sta1==8&&!g_Ini.iAxisSwitch[0]) ) 				//仅第2,3轴检测是否为限位停止之驱动
				{
					workstep++;  
					break;
				}
				else
				{
					goto BACKZEROEND3;				
				}
			}			
			workstep +=3;
			break;
			
	   case 3:
			if(sta3==8)
			{
				bDriver3=TRUE;  												//指示3轴已经驱动
				pmove(ch3,dir3?DestPulse3:-DestPulse3);
				
			}
			if(sta1==8&&!g_Ini.iAxisSwitch[0])
			{
				bDriver2=TRUE;  												//指示2轴已经驱动
				pmove(ch1,dir1?DestPulse1:-DestPulse1);
			}
			workstep ++;
	
		case 4:
			get_status(ch1,&status1);
			get_status(ch3,&status3);
	
			if((bDriver3?status3:0) || (bDriver2?status1:0))
				break;
	
			sta1=0x00ff;sta3=0x00ff;
	
			if(CheckHome(ch1))
				value1=1;
			else
				value1=0;
	
			if(CheckHome(ch3))
				value3=1;
			else
				value3=0;
	
			sta1 = sta1&(INT16U)value1;
			sta3 = sta3&(INT16U)value3;
	
			if((bDriver3?sta3!=1:0)||(bDriver2?sta1!=1:0) )
			{
				goto BACKZEROEND3;
			}
			workstep++;

	   case 5:	   		
			 {

    			SetMMConSpeed(ch1,fspeed1/10.0);
    			SetMMConSpeed(ch2,fspeed2/10.0);
    			SetMMConSpeed(ch3,fspeed3/10.0);
    		
    		    
				set_stop0_mode(ch1,1,logic?0:1);
				set_stop1_mode(ch2,0,logic?0:1);
				set_stop0_mode(ch3,1,logic?0:1);				
				pmove(ch1,dir1?DestPulse1:-DestPulse1);
				pmove(ch2,dir2?DestBack:-DestBack);
				pmove(ch3,dir3?DestPulse3:-DestPulse3);
    			workstep ++;
			}

	   case 6:	   		
			get_status(ch1,&status1);
			get_status(ch2,&status2);	
			get_status(ch3,&status3);		
			
			if( status1 || status2 || status3 ) 
				break;	

			sta1=0x00ff;
			sta2=0x00ff;
			sta3=0x00ff;
			
			if(!CheckHome(ch1))
				value1=1;
			else
				value1=0;
				
			if(!CheckHome(ch2))
				value2=1;
			else
				value2=0;
				
			if(!CheckHome(ch3))
				value3=1;
			else
				value3=0;
				
			sta1 = sta1&(INT16U)value1;
			sta2 = sta2&(INT16U)value2;
			sta3 = sta3&(INT16U)value3;
			
			if(sta1!=1||sta2!=1||sta3!=1)
			{
				goto BACKZEROEND3;				
			}	
					
			SetMMConSpeed(ch1,fspeed1/10.0);
    		SetMMConSpeed(ch2,5);
    		SetMMConSpeed(ch3,fspeed3/10.0);
			
			set_stop0_mode(ch1,1,logic);
			set_stop1_mode(ch2,1,logic);
			set_stop0_mode(ch3,1,logic);				
			
			pmove(ch1,dir1?-DestPulse1:DestPulse1);
			pmove(ch2,dir2?-DestPulse2:DestPulse2);	
			pmove(ch3,dir3?-DestPulse3:DestPulse3);			
			workstep++;

		case 7:
			{    			   				
  	 			get_status(ch1,&status1);
    			get_status(ch2,&status2);	
    			get_status(ch3,&status3);		
    			
    			if( status1 || status2 || status3 ) 
    				break;
    					
    			sta1 = 0x00ff;
    			sta2 = 0x00ff;
    			sta3 = 0x00ff;
    			
    			if(CheckHome(ch1))
    				value1=1;
    			else
    				value1=0;
    				
    			if(CheckHome(ch2))
    				value2=1;
    			else
    				value2=0;
    				
    			if(CheckHome(ch3))
    				value3=1;
    			else
    				value3=0;
    				
    			sta1 = sta1&(INT16U)value1;
    			sta2 = sta2&(INT16U)value2;
    			sta3 = sta3&(INT16U)value3;
    			
    			if(sta1!=1 || sta2!=1 || sta3!=1)
    			{
    				goto BACKZEROEND3;				
    			}
    				
    			if(g_Ini.iAxisSwitch[0])
    			{
	    			set_stop0_mode(ch1,0,logic);		
					set_stop1_mode(ch1,1,logic);
					SetMMConSpeed(ch1,100.0/10.0);
					pmove(ch1,dir1?-DestPulse1:DestPulse1);
				}
				
				workstep ++;
			}
			
	   case 8:
			{
				if(g_Ini.iAxisSwitch[0])
				{
	    			get_status(ch1,&status1);
	   			
	    			if( status1 ) 
	    				break;
	    					
	    			sta1 = 0x00ff;
	    			sta2 = 0x00ff;
	    			sta3 = 0x00ff;
	    			
	    			if(CheckHome(5))
	    				value1=1;
	    			else
	    				value1=0;
	    			sta1=sta1&(INT16U)value1;
	    			
	    			if(sta1!=1)
	    			{
	    				goto BACKZEROEND3;				
	    			}
	    				
	    			set_stop1_mode(ch1,0,logic);
    			}	
    			else
    			{
    				set_stop0_mode(ch1,0,logic);
    			}	
				
				workstep ++;
				
								
				set_stop1_mode(ch2,0,logic);
				set_stop0_mode(ch3,0,logic);
				BaseSetCommandPos(ch1,0);						
				BaseSetCommandPos(ch2,0);
				BaseSetCommandPos(ch3,0);
//				set_actual_pos(ch1,0);
//				set_actual_pos(ch2,0);	
//				set_actual_pos(ch3,0);						
				OSTimeDly(20);													//must
				OSSemSet(FreeSem,0,&err);
				return TRUE;
			}
		}
		
		OSTimeDly(2);
		
		if( g_iCardStopFlag[ch1-1] != 0 || g_iCardStopFlag[ch2-1] !=0|| g_iCardStopFlag[ch3-1] !=0  )
		{

			break;
		}
		
	}
	
BACKZEROEND3:
	set_stop0_mode(ch1,0,logic);
	set_stop1_mode(ch1,0,logic);	
	set_stop1_mode(ch2,0,logic);
	set_stop0_mode(ch3,0,logic);
	
	OSSemSet(FreeSem,0,&err);
	return FALSE;
}


BOOL	FindZero2(INT32S speed1, INT32S speed2)
{
	INT8U 	err;
	INT32S 	temp1,temp2;
	int 	ch1=1,ch2=2;
	
	temp1=GetPulse(ch1,TRUE)%g_Ini.lPulse[ch1-1];
	temp2=GetPulse(ch2,TRUE)%g_Ini.lPulse[ch2-1];
	
	if(temp1==0&&temp2==0)
	{
		BaseSetCommandPos(ch1,0);
//		set_actual_pos(ch1,0);
		BaseSetCommandPos(ch2,0);
//		set_actual_pos(ch2,0);
		return  TRUE;
	}
	
	{
		MovePulse2(temp1<=0?(0-temp1):(g_Ini.lPulse[ch1-1]-temp1),
				  temp2<=0?(0-temp2):(g_Ini.lPulse[ch2-1]-temp2),
				  speed1,speed2);
		
		if(g_iCardStopFlag[ch1-1]!=0||g_iCardStopFlag[ch2-1]!=0)
		{
			OSSemSet(FreeSem,0,&err);
			return FALSE;
		}
		else
		{
			BaseSetCommandPos(ch1,0);
//			set_actual_pos(ch1,0);
			BaseSetCommandPos(ch2,0);
//			set_actual_pos(ch2,0);
			OSSemSet(FreeSem,0,&err);
			return  TRUE;
		}
	}	

}


BOOL	FindZero3( INT32S speed1, INT32S speed2, INT32S speed3)
{
	INT8U err;
	INT32S temp1,temp2,temp3;
	int ch1=1,ch2=2,ch3=3;


	temp1=GetPulse(ch1,TRUE)%g_Ini.lPulse[ch1-1];
	temp2=GetPulse(ch2,TRUE)%g_Ini.lPulse[ch2-1];
	temp3=GetPulse(ch3,TRUE);

	
	if(temp1==0&&temp2==0&&temp3==0)
	{
		BaseSetCommandPos(ch1,0);
//		set_actual_pos(ch1,0);
		BaseSetCommandPos(ch2,0);
//		set_actual_pos(ch2,0);
		BaseSetCommandPos(ch3,0);
//		set_actual_pos(ch3,0);
		return  TRUE;
	}
	{
		MovePulse3(temp1<=0?(0-temp1):(g_Ini.lPulse[ch1-1]-temp1),
				   temp2<=0?(0-temp2):(g_Ini.lPulse[ch2-1]-temp2),
				   0-temp3,
				   speed1,speed2,speed3);
	
		if(g_iCardStopFlag[ch1-1]!=0||g_iCardStopFlag[ch2-1]!=0||g_iCardStopFlag[ch3-1]!=0)
		{
			OSSemSet(FreeSem,0,&err);
			return FALSE;
		}
		else
		{
			BaseSetCommandPos(ch1,0);
//			set_actual_pos(ch1,0);
			BaseSetCommandPos(ch2,0);
//			set_actual_pos(ch2,0);
			BaseSetCommandPos(ch3,0);
//			set_actual_pos(ch3,0);
			OSSemSet(FreeSem,0,&err);
			return  TRUE;
		}
	}	
}


BOOL	FindZero4( INT32S speed1, INT32S speed2, INT32S speed3, INT32S speed4)
{
	INT8U  err;
	INT32S temp1,temp2,temp3,temp4;
	int    ch1=1,ch2=2,ch3=3,ch4=4;
	
	temp1=GetPulse(ch1,TRUE)%g_Ini.lPulse[ch1-1];
	temp2=GetPulse(ch2,TRUE)%g_Ini.lPulse[ch2-1];
	temp3=GetPulse(ch3,TRUE);
	temp4=GetPulse(ch4,TRUE);

//	if(g_Ini.iUDriver == 1)
//		temp4 = 0;																//2011/8/31 U同步送线不需要回零
	
	
	{
		if(temp1==0 && temp2==0 && temp3==0 && temp4==0)
		{
			BaseSetCommandPos(ch1,0);
//			set_actual_pos(ch1,0);
			BaseSetCommandPos(ch2,0);
//			set_actual_pos(ch2,0);
			BaseSetCommandPos(ch3,0);
//			set_actual_pos(ch3,0);
			BaseSetCommandPos(ch4,0);
//			set_actual_pos(ch4,0);			
			return  TRUE;
		}
		
		{
			MovePulse4(temp1<=0?(0-temp1):(g_Ini.lPulse[ch1-1]-temp1),
					   temp2<=0?(0-temp2):(g_Ini.lPulse[ch2-1]-temp2),
					   0-temp3,0-temp4,
					   speed1,speed2,speed3,speed4);
			
			if(g_iCardStopFlag[ch1-1]!=0 || g_iCardStopFlag[ch2-1]!=0 || g_iCardStopFlag[ch3-1]!=0 || g_iCardStopFlag[ch4-1]!=0)
			{
				OSSemSet(FreeSem,0,&err);
				return FALSE;
			}
			else
			{
				BaseSetCommandPos(ch1,0);
//				set_actual_pos(ch1,0);
				BaseSetCommandPos(ch2,0);
//				set_actual_pos(ch2,0);
				BaseSetCommandPos(ch3,0);
//				set_actual_pos(ch3,0);
				BaseSetCommandPos(ch4,0);
//				set_actual_pos(ch4,0);
				OSSemSet(FreeSem,0,&err);
				return  TRUE;
			}
		}	
	}
}


void	MovePulse2(INT32S p1,INT32S p2,INT32S speed1,INT32S speed2)
{
	int status1=-1,status2=-1;
	INT16S ch1=1,ch2=2;
	
	Start(ch1);
	Start(ch2);
	SetPulseConSpeed(ch1,speed1);
	SetPulseConSpeed(ch2,speed2);
	pmove(ch1,p1);
	pmove(ch2,p2);

	while(status1||status2)
	{
		get_status(ch1,&status1);
		get_status(ch2,&status2);
		OSTimeDly(2);
	}
}


void	MovePulse3(INT32S p1,INT32S p2,INT32S p3,INT32S speed1,INT32S speed2,INT32S speed3)
{
	//int status1=-1,status2=-1,status3=-1;
	INT16S ch1=1,ch2=2,ch3=3;


	Start(ch1);
	Start(ch2);
	Start(ch3);

    set_startv(1,100);
	set_speed(1,200);
	set_startv(2,100);
	set_speed(2,200);

    set_startv(3,100);
	set_speed(3,200);

	set_acc(1,2000);
	set_acc(2,2000);
	set_acc(3,2000);
	pmove(ch1,p1);
	pmove(ch2,p2);
	pmove(ch3,p3);


	while(1)
	{
	    if(IsFree(1)==1 && IsFree(2)==1 && IsFree(3)==1)
	        break;
	        
	    OSTimeDly(1);
	}
}


void	MovePulse4(INT32S p1,INT32S p2,INT32S p3,INT32S p4,INT32S speed1,INT32S speed2,INT32S speed3,INT32S speed4)
{
	int status1=-1,status2=-1,status3=-1,status4=-1;
	INT16S ch1=1,ch2=2,ch3=3,ch4=4;
	//INT16S logic=0;
	
	
	if(g_Ini.iAxisSwitch[0])Start(ch1);	
	if(g_Ini.iAxisSwitch[1])Start(ch2);
	if(g_Ini.iAxisSwitch[2])Start(ch3);
	if(g_Ini.iAxisSwitch[3])Start(ch4);
	
	SetPulseConSpeed(ch1,speed1);
	SetPulseConSpeed(ch2,speed2);
	SetPulseConSpeed(ch3,speed3);
	SetPulseConSpeed(ch4,speed4);
	
	if(g_Ini.iAxisSwitch[0])pmove(ch1,p1);	
	if(g_Ini.iAxisSwitch[1])pmove(ch2,p2);
	if(g_Ini.iAxisSwitch[2])pmove(ch3,p3);
	if(g_Ini.iAxisSwitch[3])pmove(ch4,p4);
	
	while(status1||status2||status3||status4)
	{
		get_status(ch1,&status1);		
		get_status(ch2,&status2);
		get_status(ch3,&status3);
		get_status(ch4,&status4);
		
		OSTimeDly(2);
	}	
}


BOOL	GotoMM(INT16S ch,FP32 pos,FP32 speed,BOOL istrue)
{
	INT32S p;
	int status=-1;
	BOOL flag=TRUE;
	
	Start(ch);
	SetMMConSpeed(ch,speed);
	
	p=(INT32S)(pos*g_fPulseOfAngle[ch-1]);
	p=p-GetPulse(ch,TRUE);
   
    if(ch==3 ||ch==4)
    {
        ;
    }
    else
    {
    	p=(p%g_lPulseOfCircle[ch-1]+g_lPulseOfCircle[ch-1])%g_lPulseOfCircle[ch-1];	
    
    	if(p>g_lPulseOfCircle[ch-1]/2)											//20070525modify
    		p-=g_lPulseOfCircle[ch-1];
	}
	
	pmove(ch,p);
	
	while(status)
	{
		get_status(ch,&status);
		
		if(g_iCardStopFlag[ch-1] )
		{
			flag=FALSE;
			break;
		}
		
		OSTimeDly(2);
	}
	
//GOTOMMEND:
	 return flag;
}


BOOL	GotoMM4(INT16S ch1,INT16S ch2,INT16S ch3,INT16S ch4,FP32 pos1,FP32 pos2,FP32 pos3,FP32 pos4,FP32 speed,BOOL istrue)    
{
	INT8U  err;
	INT32S p1,p2,p3,p4;
	INT32S pp1,pp2,pp3,pp4;
	INT32S maxp;
	int status1=-1,status2=-1,status3=-1,status4=-1;
	BOOL   flag=TRUE;
	
	Start(ch1);Start(ch2);Start(ch3);Start(ch4);

	p1=(INT32S)(pos1*g_fPulseOfAngle[ch1-1]);
	p1=p1-GetPulse(ch1,TRUE);
	p2=(INT32S)(pos2*g_fPulseOfMM[ch2-1]);
	p2=p2-GetPulse(ch2,TRUE);	
	p3=(INT32S)(pos3*g_fPulseOfMM[ch3-1]);
	p3=p3-GetPulse(ch3,TRUE);	
	p4=(INT32S)(pos4*g_fPulseOfMM[ch4-1]);
	p4=p4-GetPulse(ch4,TRUE);	

	pp1=labs(p1); pp2=labs(p2); pp3=labs(p3); pp4=labs(p4);
	maxp=pp1;
	maxp=pp1>pp2?pp1:pp2;
	maxp=maxp>pp3?maxp:pp3;	
	maxp=maxp>pp4?maxp:pp4;
	
	{
		if( maxp==pp1 )
		{
			if( !pp1 )  
			{
				flag=TRUE;
				goto GOTOMMEND;
			}
			
			SetMMConSpeed( ch1, speed );
			SetMMConSpeed( ch2, speed*((double)pp2/(double)pp1) );
			SetMMConSpeed( ch3, speed*((double)pp3/(double)pp1) );
			SetMMConSpeed( ch4, speed*((double)pp4/(double)pp1) );
		}
		else
		{
			if( maxp==pp2 )
			{
				if( !pp2 )
				{
					flag=TRUE;
					goto GOTOMMEND;
				}
				
				SetMMConSpeed( ch1, speed*((double)pp1/(double)pp2) );
				SetMMConSpeed( ch2, speed );
				SetMMConSpeed( ch3, speed*((double)pp3/(double)pp2) );
				SetMMConSpeed( ch4, speed*((double)pp4/(double)pp2) );
			}
			else
			{
			    if( maxp==pp3 )
    			{
    				if( !pp3 )
    				{
    					flag=TRUE;
    					goto GOTOMMEND;
    				}
    				SetMMConSpeed( ch1, speed*((double)pp1/(double)pp3) );    				
    				SetMMConSpeed( ch2, speed*((double)pp2/(double)pp3) );
    				SetMMConSpeed( ch3, speed );
    				SetMMConSpeed( ch4, speed*((double)pp4/(double)pp3) );
    			}
    			else
    			{
    			    if( !pp4 )
    				{
    					flag=TRUE;
    					goto GOTOMMEND;
    				}
    				SetMMConSpeed( ch1, speed*((double)pp1/(double)pp4) );    				
    				SetMMConSpeed( ch2, speed*((double)pp2/(double)pp4) );    				
    				SetMMConSpeed( ch3, speed*((double)pp3/(double)pp4) );
    				SetMMConSpeed( ch4, speed );
    			}
			}
		}
	}

	pmove(ch1,p1);
	pmove(ch2,p2);
	pmove(ch3,p3);
	pmove(ch4,p4);
	
	while( status1||status2||status3||status4 )
	{
		get_status(ch1,&status1);
		get_status(ch2,&status2);
		get_status(ch3,&status3);
		get_status(ch4,&status4);
		
		if( g_iCardStopFlag[ch1-1]&&g_iCardStopFlag[ch2-1]&&g_iCardStopFlag[ch3-1]&&g_iCardStopFlag[ch4-1] )
		{
			flag=FALSE;
			break;
		}
		
		OSTimeDly(2);
	}
	
GOTOMMEND:
    OSSemSet(FreeSem,0,&err);    
	return flag;
}


INT16S ReadBit(INT16S io)
{
	INT16S st=0;
	
	if(g_Ini.iSafeDoor == 0 && io == SAFEDOOR)
	{
		return -1;
	}

	if(io<100)
	{ 
		st=read_bit(io);
		return st;
	}
	else
	{
		return -1;
	}
}


INT16S RealIO(INT16S io, INT16S status, INT16S ti)
{


	if( ReadBit(io) != status ) 
		return 0;
	
	while( ti-- )
	{
		if(ti%100==0)
		{
			OSTimeDly(1);
		}
	}
	
	if( ReadBit(io) != status ) 
		return 0;
	
	return 1;
}


INT16S WriteBit(INT16S io, INT16S status)
{
	INT32U a=0x00000001;
	int io2=0;
	
	io2=io;				
	
	if(status)
	{
		a <<= io2;
		g_ulOutStatus |= a;
	}
	else
	{
		a <<= io2;
		g_ulOutStatus &= ~a;
	}

	write_bit(io2,status);
	return status;
}


INT16S GetOut(INT16S io)
{
	INT32U a=0x00000001;
	
	a<<=io;
	return (g_ulOutStatus & a)!=0;
}


void	BackZeroRun(INT32S p1,INT32S p2,INT32S p3,INT32S p4,INT32S speed1,INT32S speed2,INT32S speed3,INT32S speed4,INT32S mode)
{
	
	float    add = 100;
	INT32S  lx = 0;
	INT32S  ly = 0;
	long  px,py,pz,pu;

	#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                 				 			/* Prevent compiler warning                           */
	#endif

	set_startv(1,(INT32S)((float)speed1/g_iCardBeiLv[0]));
	set_speed(1,(INT32S)((float)speed1/g_iCardBeiLv[0]));
	add =  g_Ini.fAddSpeed*GetUnitPulse(1)*6.0;	
	set_acc(1,(INT32S)(add/g_iCardBeiLv[0]/125));
	
	set_startv(2,(INT32S)((float)speed2/100.0/g_iCardBeiLv[1]));
	set_speed(2,(INT32S)((float)speed2/g_iCardBeiLv[1]));
	add =  g_Ini.fAddSpeed*GetUnitPulse(2)*6.0;	
	set_acc(2,(INT32S)(add/g_iCardBeiLv[1]/125));
	
	set_startv(3,(INT32S)((float)speed3/100.0/g_iCardBeiLv[2]));
	set_speed(3,(INT32S)((float)speed3/g_iCardBeiLv[2]));
	add =  g_Ini.fAddSpeed*GetUnitPulse(3)*6.0;
	set_acc(3,(INT32S)(add/g_iCardBeiLv[2]/125));
	
	set_startv(4,(INT32S)((float)speed4/100.0/g_iCardBeiLv[3]));
	set_speed(4,(INT32S)((float)speed4/g_iCardBeiLv[3]));
	add =  g_Ini.fAddSpeed*GetUnitPulse(4)*6.0;
	set_acc(4,(INT32S)(add/g_iCardBeiLv[3]/125));
	
	BaseGetCommandPos(1,&px);
	BaseGetCommandPos(2,&py);
	BaseGetCommandPos(3,&pz);
	BaseGetCommandPos(4,&pu);
	

	if(mode == 1)
	{
  	  	if(px%g_Ini.lPulse[0]>=(INT32S)(g_Ini.lPulse[0]*0.95)||px==g_Ini.lPulse[0])
		{
			set_stop1_mode(1,1,0);
			SetMMConSpeed(1,100.0/10.0);
		}	
		
		if(labs(py%g_Ini.lPulse[1])>=(INT32S)(g_Ini.lPulse[1]*0.98)||py==g_Ini.lPulse[1])
		{
			set_stop1_mode(2,1,0);
			SetMMConSpeed(2,5);
		}	
		
		if(labs(pz)<=(INT32S)(1.0*g_fPulseOfMM[2]))
		{
			set_stop0_mode(3,1,0);
			SetMMConSpeed(3,g_Ini.fBackSpeed[2]/10.0);
		}	
		
		px = g_Ini.lPulse[0]+100;
		py = g_Ini.lPulse[1]+100;
	}
	else
	{
		lx= px%g_Ini.lPulse[0];
		
		if(lx!=0&&lx!=g_Ini.lPulse[0])
		{
			px=g_Ini.lPulse[0]-lx;
		
		}
		else
			px = 0;
	}
					
	ly = py%g_Ini.lPulse[1];
	
	if(ly<0)
		ly = ly + g_Ini.lPulse[1];
	
	if(ly!=0 && ly!=g_Ini.lPulse[1])
	{
		py=g_Ini.lPulse[1]-ly;
	}
	else
		py = 0;
		
	Start(1);
	Start(2);
	Start(3);
	Start(4);

	if(g_Ini.iAxisSwitch[0])
		pmove(1,px);
		
	pmove(2,p2);
	pmove(3,p3);
	
	if(g_Ini.iAxisSwitch[3])
		pmove(4,p4);
	
}



BOOL	BackZero4(INT16S dir1,INT16S dir2,INT16S dir3,INT16S dir4,INT16S step)
{
	INT8U 	err;
	INT16S 	ch1=1,ch2=2,ch3=3,ch4=4;
	INT16S 	workstep = step;
	//INT16S 	workstep1=3;
	int 	status1=0,status2=0,status3=0,status4=0;
	INT16S 	logic=0;
	INT16S 	value1=-1,value2=-1,value3=-1,value4=-1;
	INT32S 	DestPulse1,DestPulse2,DestPulse3,DestPulse4;
    INT16U 	sta1=0x00ff,sta2=0x00ff,sta3=0x00ff,sta4=0x00ff;
	BOOL	bDriver3=FALSE,bDriver4=FALSE;

	INT32S 	DestBack;
	FP32 	fspeed1 = 0, fspeed2 = 0, fspeed3 = 0, fspeed4 = 0 ,fspeed11 = 0 , fspeed12 = 0;
	BOOL   	bX = FALSE, bY = FALSE, bZ = FALSE, bU = FALSE;
	
	
	set_limit_mode(3,0,0,0);
	set_limit_mode(4,0,0,0);
	
	bX = g_Ini.iAxisSwitch[0]?TRUE:FALSE;
	bY = g_Ini.iAxisSwitch[1]?TRUE:FALSE;
	bZ = g_Ini.iAxisSwitch[2]?TRUE:FALSE;
	bU = g_Ini.iAxisSwitch[3]?TRUE:FALSE;
	
	if(g_Ini.iAxisCount<4)
		bU = FALSE;
		
//	if(g_Ini.iUDriver == 1 )
//		bU  = FALSE;
		
	if(g_Ini.iRunDir[ch1-1]==1)
	     dir1 = dir1?0:1;
	
	if(g_Ini.iRunDir[ch2-1]==1)
	     dir2 = dir2?0:1;
	
	if(g_Ini.iRunDir[ch3-1]==1)
	     dir3 = dir3?0:1;
	
	if(g_Ini.iRunDir[ch4-1]==1)
	     dir4 = dir4?0:1;
	     
	fspeed1 = g_Ini.fBackSpeed[ch1-1]*6.0;
	fspeed2 = g_Ini.fBackSpeed[ch2-1]*6.0;
	fspeed3 = g_Ini.fBackSpeed[ch3-1];
	fspeed4 = g_Ini.fBackSpeed[ch4-1];
	
//	fspeed11 = g_Ini.fBackSpeed1[0]*6.0;
//	fspeed12 = g_Ini.fBackSpeed1[1]*6.0;

	fspeed11 = g_Ini.fBackSpeed[0]*6.0/10.0;
	fspeed12 = g_Ini.fBackSpeed[1]*6.0/10.0;
	
	
	DestPulse1=GetPulseFromAngle(ch1,365);
	DestPulse2=1000000L;
	DestPulse3=1000000L;
	DestPulse4=1000000L;
	
	DestBack = GetPulseFromAngle(ch2,10);

	SetMMConSpeed(ch1,fspeed1);
	SetMMConSpeed(ch2,fspeed2);
	SetMMConSpeed(ch3,fspeed3);
	SetMMConSpeed(ch4,fspeed4);
	
	Start(ch1);
	Start(ch2);
	Start(ch3);
	Start(ch4);
	
	//不可直接调用本类中的速度设定函数,防止g_Ini.Dir的重复使用
	//set_limit_mode(3,0,0,0);
	//set_limit_mode(4,0,0,0);

	while( TRUE )
	{
		switch( workstep )
		{
		case -2:
			  if(bZ)
		     	 pmove(ch3,dir3?-DestPulse3:DestPulse3);
		      workstep ++;
		      
		case -1:
			  if(bZ)
			  {
		   	     get_status(ch3,&status3);
		     	 if( status3 ) break;
		      }
		      workstep ++;
		      
		case 0:																	//检查已经信号
			{
				if( !(bX^(bX && CheckHome(ch1))) && 
				    !(bY^(bY && CheckHome(ch2))) && 
				    !(bZ^(bZ && CheckHome(ch3))) && 
				    !(bU^(bU && CheckHome(ch4)))
				   )
				{																//有信号
					workstep = 5;
					break;
				}
				
				if(bX)
					set_stop0_mode(ch1,1,logic);
					
				if(bY)
					set_stop1_mode(ch2,1,logic);
					
				if(bZ)
					set_stop0_mode(ch3,1,logic);
						
				if(bU)
					set_stop0_mode(ch4,1,logic);
								
				if(bX)
					pmove(ch1,dir1?-DestPulse1:DestPulse1);
					
				if(bY)
					pmove(ch2,dir2?-DestPulse2:DestPulse2);
					
				if(bZ)
					pmove(ch3,dir3?-DestPulse3:DestPulse3);	
							
				if(bU)
					pmove(ch4,dir4?-DestPulse4:DestPulse4);
				
			}
			workstep ++;
			
		case 1:
			workstep ++;
			
		case 2:	
			status1 = status2 = status3 = status4 = 0;
					
			if(bX)
				get_status(ch1,&status1);
				
			if(bY)
				get_status(ch2,&status2);
				
			if(bZ)
				get_status(ch3,&status3);
				
			if(bU)
				get_status(ch4,&status4);
			
			if( status1 || status2 || status3 || status4 ) 
				break;
				
			sta1 = 0x00ff;
			sta2 = 0x00ff;
			sta3 = 0x00ff;
			sta4 = 0x00ff;
			
			value1 = value2 = value3 = value4 = 1;
			
			if(bX)
			{
				if(CheckHome(ch1))
					value1=1;
				else
					value1=0;
			}

			value2=1;
			
			if(bZ)
			{
				if(CheckHome(ch3))
					value3=1;
				else
				{
					if(!ReadBit(ZLMTMINUS) || !ReadBit(ZLMTPLUS))	
						value3=8;	
					else
						value3=0;
				}
			}
			
			if(bU)
			{
				if(CheckHome(ch4))
					value4=1;
				else
				{
					if(!ReadBit(ULMTMINUS) || !ReadBit(ULMTPLUS))	
						value4=8;	
					else
						value4=0;
				}
			}
			
			sta1=sta1&(INT16U)value1;
			sta2=sta2&(INT16U)value2;
			sta3=sta3&(INT16U)value3;
			sta4=sta4&(INT16U)value4;
			
																				//判断停止信息是否因stop1停止  //2轴为stop0停止
			if(sta1!=1 || sta2!=1 || sta3!=1 || sta4!=1)
			{
				if( sta3==8 || sta4==8 ) 										//仅第2,3轴检测是否为限位停止之驱动
				{
					workstep++;  
					break;
				}
				else
				{
					goto BACKZEROEND4;				
				}
			}			
			workstep += 3;
			break;
			
	   case 3:
			if(sta3==8)
			{
				bDriver3=TRUE;  												//指示2轴已经驱动
				pmove(ch3,dir3?DestPulse3:-DestPulse3);
			}
			
			if(sta4==8)
			{
				bDriver4=TRUE;  												//指示3轴已经驱动
				pmove(ch4,dir4?DestPulse4:-DestPulse4);
			}
			workstep ++;
			
		case 4:
			get_status(ch3,&status3);
			get_status(ch4,&status4);
			
			if((bDriver3?status3:0)	|| (bDriver4?status4:0))
				break;
				
			sta3 = 0x00ff;
			sta4 = 0x00ff;
			
			if(CheckHome(ch3))
				value3=1;
			else
				value3=0;
				
			if(CheckHome(ch4))
				value4=1;
			else
				value4=0;
				
			sta3 = sta3&(INT16U)value3;
			sta4 = sta4&(INT16U)value4;
			
			if((bDriver3?sta3!=1:0) || (bDriver4?sta4!=1:0))
			{
				goto BACKZEROEND4;
			}
			workstep++;

	   case 5:	   		
			 {
    			if(bX)
    				SetMMConSpeed(ch1,fspeed1/10.0);
    				
    			if(bY)
    				SetMMConSpeed(ch2,fspeed2/10.0);
    				
    			if(bZ)
    				SetMMConSpeed(ch3,fspeed3/10.0);
    				
    			if(bU)
    				SetMMConSpeed(ch4,fspeed4/10.0);
    		
				if(bX)
					set_stop0_mode(ch1,1,logic?0:1);
					
				if(bY)
					set_stop1_mode(ch2,0,logic?0:1);
					
				if(bZ)
					set_stop0_mode(ch3,1,logic?0:1);
						
				if(bU)
					set_stop0_mode(ch4,1,logic?0:1);	
							
				if(bX)
					pmove(ch1,dir1?DestPulse1:-DestPulse1);
					
				if(bY)
					pmove(ch2,dir2?DestBack:-DestBack);
					
				if(bZ)
					pmove(ch3,dir3?DestPulse3:-DestPulse3);
					
				
				if(bU)
					pmove(ch4,dir4?DestPulse4:-DestPulse4);
							
    			workstep ++;
    			status1  = status2 = status3 = status4 = 0;		
			}

	   case 6:
	   		   		
			if(bX)
				get_status(ch1,&status1);
			
			if(bY)
				get_status(ch2,&status2);	
			
			if(bZ)
				get_status(ch3,&status3);
			
			if(bU /* && g_Ini.iUDriver==0 */)
				get_status(ch4,&status4);
							
			if( status1||status2||status3||status4 ) 
				break;	
			
			sta1 = 0x00ff;
			sta2 = 0x00ff;
			sta3 = 0x00ff;
			sta4 = 0x00ff;
			
			value1 = value2 = value3 = value4 = 1;
			
			OSTimeDly(100);
			
			if(bX)
			{
				if(!CheckHome(ch1))
					value1=1;
				else
					value1=0;
			}
			
			if(bY)
			{
				value2 = 1;
			}
			
			if(bZ)
			{
				if(!CheckHome(ch3))
					value3=1;
				else
					value3=0;
			}
			
			if(bU)
			{
				if(!CheckHome(ch4))
					value4=1;
				else
					value4=0;
			}
			
			sta1=sta1&(INT16U)value1;
			sta2=sta2&(INT16U)value2;
			sta3=sta3&(INT16U)value3;
			sta4=sta4&(INT16U)value4;
			
			if(sta1!=1||sta2!=1||sta3!=1||sta4!=1)
			{
				goto BACKZEROEND4;				
			}			
						
			if(bX)SetMMConSpeed(ch1,fspeed1/10.0);
			if(bY)SetMMConSpeed(ch2,fspeed12);
			if(bZ)SetMMConSpeed(ch3,fspeed3/10.0);
			if(bU)SetMMConSpeed(ch4,fspeed4/10.0);
			
			if(bX)set_stop0_mode(ch1,1,logic);
			if(bY)set_stop1_mode(ch2,1,logic);
			if(bZ)set_stop0_mode(ch3,1,logic);	
			if(bU)set_stop0_mode(ch4,1,logic);	
						
			if(bX)pmove(ch1,dir1?-DestPulse1:DestPulse1);
			if(bY)pmove(ch2,dir2?-DestPulse2:DestPulse2);	
			if(bZ)pmove(ch3,dir3?-DestPulse3:DestPulse3);			
			if(bU)pmove(ch4,dir4?-DestPulse4:DestPulse4);
						
			workstep++;
			status1  = status2 = status3 = status4 = 0;	
				 
		case 7:
			{
    			get_status(ch1,&status1);
    			get_status(ch2,&status2);	
    			get_status(ch3,&status3);	
    			get_status(ch4,&status4);	
    				
    			if( status1 || status2 || status3 || status4 ) 
    				break;	
    		
    			sta1 = 0x00ff;
    			sta2 = 0x00ff;
    			sta3 = 0x00ff;
    			sta4 = 0x00ff;
    			
    			value1 = value2 = value3 = value4 = 1;
    			
    			OSTimeDly(100);
    			
    			if(bX)
    			{
	    			if(CheckHome(ch1))
	    				value1=1;
	    			else
	    				value1=0;
    			}
    		
    			if(bY)
    			{
	    			value2 = 1;
    			}
    			
    			if(bZ)
    			{
	    			if(CheckHome(ch3))
	    				value3=1;
	    			else
	    				value3=0;
    			}
    			
    			if(bU /*&&g_Ini.iUDriver==0*/)
    			{
	    			if(CheckHome(ch4))
	    				value4=1;
	    			else
	    				value4=0;
    			}
    			
    			sta1=sta1&(INT16U)value1;
    			sta2=sta2&(INT16U)value2;
    			sta3=sta3&(INT16U)value3;
    			sta4=sta4&(INT16U)value4;
    			
    			if(sta1!=1||sta2!=1||sta3!=1||sta4!=1)
    			{
    				goto BACKZEROEND4;				
    			}
    			
    			if(bX)
    			{	
	    			set_stop0_mode(ch1,0,logic);		
					set_stop1_mode(ch1,1,logic);
					SetMMConSpeed(ch1,fspeed11);
				
					pmove(ch1,dir1?-DestPulse1:DestPulse1);
				}
				
				if(bZ)
    			{	
	    			set_stop0_mode(ch3,0,logic);		
					set_stop1_mode(ch3,1,logic);
					SetMMConSpeed(ch3,1.5);
					
					pmove(ch3,dir3?-DestPulse3:DestPulse3);
				}
				
				if(bU)
    			{	
	    			set_stop0_mode(ch4,0,logic);		
					set_stop1_mode(ch4,1,logic);
					SetMMConSpeed(ch4,1.5);
					
					pmove(ch4,dir4?-DestPulse4:DestPulse4);
				}
				workstep ++;
				
			}
	   case 8:
			{
    			get_status(ch1,&status1);
   				get_status(ch3,&status3);
   				get_status(ch4,&status4);
    			
    			if( status1||status3||status4 ) break;		
    			
    			sta1=0x00ff;sta2=0x00ff;sta3=0x00ff;
    			sta1=sta1&(INT16U)value1;
    			
    			if(sta1!=1)
    			{
    				goto BACKZEROEND4;				
    			}			
				
				workstep ++;
				
				if(bX)set_stop1_mode(ch1,0,logic);
				if(bY)set_stop1_mode(ch2,0,logic);
				if(bZ)set_stop1_mode(ch3,0,logic);
				if(bU)set_stop1_mode(ch4,0,logic);
				
				BaseSetCommandPos(ch1,0);						
				BaseSetCommandPos(ch2,0);
				BaseSetCommandPos(ch3,0);
				BaseSetCommandPos(ch4,0);
				
//				set_actual_pos(ch1,0);
//				set_actual_pos(ch2,0);	
//				set_actual_pos(ch3,0);	
//				set_actual_pos(ch4,0);							
				OSTimeDly(20);
				OSSemSet(FreeSem,0,&err);

				return TRUE;
			}
		}
		
		OSTimeDly(2);
		
		if( g_iCardStopFlag[ch1-1]!= 0 || g_iCardStopFlag[ch2-1]!=0 || g_iCardStopFlag[ch3-1]!=0 || g_iCardStopFlag[ch4-1]!=0 )
		{
			break;
		}
	}
	
BACKZEROEND4:
	if(bX)set_stop0_mode(ch1,0,logic);
	if(bX)set_stop1_mode(ch1,0,logic);	
	if(bY)set_stop1_mode(ch2,0,logic);
	if(bZ)set_stop0_mode(ch3,0,logic);
	if(bZ)set_stop1_mode(ch3,0,logic);
	if(bU)set_stop0_mode(ch4,0,logic);
	if(bU)set_stop1_mode(ch4,0,logic);
	
	
	set_limit_mode(3,1,1,0);
	set_limit_mode(4,1,1,0);
	
	OSSemSet(FreeSem,0,&err);
	return FALSE;
}

#if FALSE
INT16U inportb(INT16U port)														
{
	return *((volatile unsigned short *)(0x10000000 +port ));
}


void outportb(INT16U port,INT16U data)										
{
	*((volatile unsigned short *)(0x10000000 +port )) = data;
}



//BOOL isProbeOpen[2]={FALSE,FALSE};						//探针打开标识

INT16S OpenProbe(INT16S cardno,INT16S pro,INT16S output)
{
	INT16U value=0;
	
	#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  							/* Prevent compiler warning                           */
	#endif
	
	
	if (cardno<0 || cardno >=max_card_number )
		return ERROR_FLAG;

	if (pro>8 || pro<1 )
		return ERROR_FLAG;
		
	if (output>6 || output<1)
		return ERROR_FLAG;

//	isProbeOpen[pro-1] = TRUE;											//探针打开标示

	value = 0xc0+((output-1)<<3)+(pro-1);
	
	#ifdef USEINT
		OS_ENTER_CRITICAL();
	#endif

	outportb(WR13,value);
	
	#ifdef USEINT
		OS_EXIT_CRITICAL();
	#endif
	
	return SUCCESSFUL;
}


INT16S CloseProbe(INT16S cardno)
{	
	#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  							/* Prevent compiler warning                           */
	#endif
	
	//isProbeOpen[0] = FALSE;											//探针关闭标示
	//isProbeOpen[1] = FALSE;											//探针关闭标示

	
	if (cardno<0 || cardno>=max_card_number )
		return ERROR_FLAG;
	
	#ifdef USEINT
		OS_ENTER_CRITICAL();
	#endif
	
	outportb(CardIOAddress[cardno]+WR13,0x00);
	
	#ifdef USEINT
		OS_EXIT_CRITICAL();
	#endif
	
	return SUCCESSFUL;
}


INT16S ReadProbe(INT16S cardno,INT16S pro)
{
	INT16U temp=0;
	
	#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  							/* Prevent compiler warning                           */
	#endif
	
//	if(isProbeOpen[pro-1] ==FALSE)												//探针未打开，返回-1;
//		return -1;
	
	
	if (cardno<0 || cardno >=max_card_number )
		return ERROR_FLAG;
	
	#ifdef USEINT
		OS_ENTER_CRITICAL();
	#endif
	
	temp = inportb(WR13);
	
	#ifdef USEINT
		OS_EXIT_CRITICAL();
	#endif
	
	switch(pro)
	{
		case 1:	
			if((temp & 0x01)==0)
				return 0;
			else 
				return 1;
		case 2:	
			if((temp&0x02)==0)
				return 0;
			else 
				return 1;
		case 3:	
			if((temp&0x04)==0)
				return 0;
			else 
				return 1;
		case 4:	
			if((temp&0x08)==0)
				return 0;
			else 
				return 1;
		case 5:	
			if((temp&0x10)==0)
				return 0;
			else 
				return 1;
		case 6:	
			if((temp&0x20)==0)
				return 0;
			else 
				return 1;
		case 7:	
			if((temp&0x40)==0)
				return 0;
			else 
				return 1;
		case 8:	
			if((temp&0x80)==0)
				return 0;
			else 
				return 1;
		default:
			return ERROR_FLAG+1;
	}
}
#endif

BOOL DA_Out(FP32 value,INT16S ch)
{
	FP32 v = 0.0;

	if(ch>1 || ch<0)
		return FALSE;
		
	if(fabs(value-0.0)<0.00001)
		value=0.0;
		
	if(value>10.0)
		value=10.0;
		
	v = value/10.0*100;
	
//	PWM_Control(1000,v,ch);
	
	return TRUE;
}


/**************************************************************

INT16S  get_stopdata(INT16S cardno,INT16S axis,INT16S *value)

value  错误状态的指针
  0：无错误
非0：value为两个字节长度的值，各位的含义如下：

D0为最低位  D15为最高位
D0：由STOP0停止
D1：由STOP1停止
D2：由STOP2停止
D3；由正限位LMT+停止
D4：由负限位LMT+停止
D5：由伺服报警停止
D6：COMP+ 寄存器限位驱动停止
D7：COMP- 寄存器限位驱动停止
D8-D15：未用

****************************************************************/

INT16S  get_stopdata(INT16S cardno,INT16S axis,INT16S *value)
{
 	INT16U temp1;//,temp2;
 	
	 #if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
	     OS_CPU_SR  cpu_sr;
	     cpu_sr = 0;                                  							/* Prevent compiler warning                           */
	 #endif
	  
	 if(cardno<0 || cardno >=max_card_number)
	 	return ERROR_FLAG;
	 	
	 if (axis<1 || axis >6)
	 	return ERROR_FLAG;
	 	  
	 #ifdef USEINT
	 	OS_ENTER_CRITICAL();
	 #endif  
	 
	 SetPortVal(CardIOAddress[cardno]+WR0,(0x0080<<axis)|0x0014,2);
	 GetPortVal(CardIOAddress[cardno]+WR6,&temp1,2);
	 
//	 GetPortVal(CardIOAddress[cardno]+WR7,&temp2,2);
//	 *value=(INT32U)(temp1&0xffff)+((INT32U)temp2<<16);

	 *value=(INT32U)(temp1 & 0xffff);
	 
	 Uart_Printf("value=%d\n",*value);
	 
	 #ifdef USEINT
	 	OS_EXIT_CRITICAL();
	 #endif 
	  
	 return SUCCESSFUL;
}


void SetPortVal(INT16U add,INT16U value ,INT16S number)
{
//	outportb(add+1,value>>8);
//	outportb(add,value & 0X00FF);

	*((volatile unsigned char *)(0x10000000+add+1)) = value>>8;
	*((volatile unsigned char *)(0x10000000+add)) = value & 0X00FF;
	
}


void GetPortVal(INT16U add,INT16U *value ,INT16S number)
{
	INT16U v1=0,v2=0;
	
//	v1 = inportb(add);
//	v2 = inportb(add+1);	

	v1 = (INT8U)(*((volatile unsigned char *)(0x10000000 +add)));
	v2 = (INT8U)(*((volatile unsigned char *)(0x10000000 +add+1)));
	*value = v1+(v2<<8);
	
}


	



INT16S CheckWarning(void)
{
	//INT16U i;

	if(g_bRelease ==FALSE)
		return 0;



	if( RealIO(DUANXIAN,g_Ini.iDuanXianLogic,16))
	{
		OSQPost(MsgQueue,g_bEnglish?"Broken Wire Warning":"断线报警!");
		return 2;					
	}
	
	if( RealIO(SONGXIANIOCONST,IOPRESS,2))
	{
		OSQPost(MsgQueue,g_bEnglish?"Foul Wire Warning":"缠线报警!!");
		return 3;
	}

	if( RealIO(PAOXIANDI,IOPRESS,2) && g_Ini.iPaoXian)
	{
		OSQPost(MsgQueue,g_bEnglish?"Exceed Wire Warning":"跑线报警!");
		return 4;
	}
	
	if(g_Ini.iSafeDoor==1)
	{
		if(RealIO(SAFEDOOR,1,2))
		{
			OSQPost(MsgQueue,g_bEnglish?"Safe door Waring":"安全门打开状态!!!");
			return 5;
		}

	}
	switch(g_Ini.iAxisCount)
	{
	case 6:
		if( g_Ini.iAxisSwitch[5]==1 && RealIO(WSERVOWARNING,g_Ini.iServoLogic[5],5))
		{
			OSQPost(MsgQueue,axisMsg(6,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 12;	
		}

	case 5:
		if( g_Ini.iAxisSwitch[4]==1 && RealIO(VSERVOWARNING,g_Ini.iServoLogic[4],5))
		{
			OSQPost(MsgQueue,axisMsg(5,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 11;	
		}
	
	case 4:
		if( g_Ini.iAxisSwitch[3]==1 && RealIO(USERVOWARNING,g_Ini.iServoLogic[3],5))
		{
			OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 10;	
		}

	case 3:
		if( g_Ini.iAxisSwitch[2]==1 && RealIO(ZSERVOWARNING,g_Ini.iServoLogic[2],5))
		{
			OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 9;	
		}

	case 2:
		
		if( g_Ini.iAxisSwitch[0]==1 && RealIO(XSERVOWARNING,g_Ini.iServoLogic[0],5))
		{

			OSQPost(MsgQueue,axisMsg(1,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 8;
		}
		
		if( g_Ini.iAxisSwitch[1]==1 && RealIO(YSERVOWARNING,g_Ini.iServoLogic[1],5))
		{
			OSQPost(MsgQueue,axisMsg(2,g_bEnglish?" Servo Warning!  Please Power off Check!":"伺服故障,请断电检查!",TRUE));
			return 7;	
		}
		
		break;
	}
	
	return 0;	
}


//---------------------硬件限位检测----------------------//
//返回值:
//	0:无硬限位报警;非0:硬限位报警
//参数:
//	ch:轴号
//	dir:
//		dir>0:检测正向;dir==0:检测正负方向;dir<0:检测负向
INT16S  CheckHardLimit(INT16S ch, INT32S dir)
{
	INT16S LIMITMINUS[MAXCH]={XLMTMINUS,YLMTMINUS,ZLMTMINUS,ULMTMINUS,VLMTMINUS,WLMTMINUS};
	INT16S LIMITPLUS[MAXCH]={XLMTPLUS,YLMTPLUS,ZLMTPLUS,ULMTPLUS,VLMTPLUS,WLMTPLUS};
	
	INT16S negErrNo[MAXCH]={20,22,24,26,28,30};					//与RunEnd中的g_iErrorNo一致
	INT16S posErrNo[MAXCH]={21,23,25,27,29,31};
	
	
	if(ch<1 || ch>MAXCH)
		return 0;
	
	if(g_Ini.iHardLimitSwitch[ch-1]==1)
	{
		if(dir<0 && RealIO(LIMITMINUS[ch-1],0,2))
		{
			return negErrNo[ch-1];
		}
		else if(dir>0 && RealIO(LIMITPLUS[ch-1],0,2))
		{
			return posErrNo[ch-1];
		}
		else if(dir==0)
		{
			if(RealIO(LIMITMINUS[ch-1],0,2))
			{	
				return negErrNo[ch-1];
			}	
			else if(RealIO(LIMITPLUS[ch-1],0,2))
			{	
				return posErrNo[ch-1];
			}
		}
	}	
	return 0;
}


//---------------------软件限位检测----------------------//
//返回值:0-无软限位报警;非0-软限位报警
//参数:
//	ch:轴号
//	dir:
//		dir>0:检测正向;dir==0:检测正负方向;dir<0:检测负向
INT16S  CheckSoftLimit(INT16S ch,INT32S dir)
{
	INT16S negErrNo[MAXCH]={32,34,36,38,40,42};						//与RunEnd中的g_iErrorNo一致
	INT16S posErrNo[MAXCH]={33,35,37,39,41,43};
	
	if(ch<1 || ch>MAXCH)
	{
		return 0;		
	}	

	if(g_Ini.fMaxLimit[ch-1]-g_Ini.fMinLimit[ch-1]>0.001)		//正软限位>负软限位 时,才进行软件限位检测
	{			
		if(dir<0)
		{
		 	FP32 fPos=(g_Ini.iAxisRunMode[ch-1]==0?GetAngle(ch,TRUE):GetMM(ch,TRUE))-(g_Ini.fMinLimit[ch-1]/(g_Ini.iAxisRunMode[ch-1]==2?100.0:10.0));
		 	
		 	if(fPos <= 0)
			{					
				return negErrNo[ch-1];
			}
		}
		else if(dir>0)
		{
			FP32 fPos=(g_Ini.fMaxLimit[ch-1]/(g_Ini.iAxisRunMode[ch-1]==2?100.0:10.0))-(g_Ini.iAxisRunMode[ch-1]==0?GetAngle(ch,TRUE):GetMM(ch,TRUE));
		 	
		 	if(fPos <= 0)
			{
				return posErrNo[ch-1];
			}
		}
		else if(dir==0)
		{		
			if((g_Ini.iAxisRunMode[ch-1]==0?GetAngle(ch,TRUE):GetMM(ch,TRUE))-(g_Ini.fMinLimit[ch-1]/(g_Ini.iAxisRunMode[ch-1]==2?100.0:10.0))<=0)
			{
				return negErrNo[ch-1];
			}		
			else if((g_Ini.fMaxLimit[ch-1]/(g_Ini.iAxisRunMode[ch-1]==2?100.0:10.0))-(g_Ini.iAxisRunMode[ch-1]==0?GetAngle(ch,TRUE):GetMM(ch,TRUE))<=0)
			{
				return posErrNo[ch-1];
			}
		}
	}

	return 0;											

}


/*
INT32S GetFifoUse(INT16S axiss)
{
	INT16U value;

	read_fifo(&value);
	
	value&=0x7ff;
	
	
	if(axiss ==1)
	{
		return value/3;
	}
	else if(axiss ==2)
	{
		return value/4;

	}
	else if(axiss ==3)
	{
		return value/5;

	}
	else if(axiss ==4)
	{
		return value/6;
	}
	else
		return -1;

}
*/

INT16S ReadFifo(void)
{
	INT16U sta;
	read_fifo(&sta);
	sta&=0x7ff;
	return sta;
}

INT16S ResetFifo(void)
{
	reset_fifo();
	return TRUE;
}


static INT32S g_lFifoRange =0;
static INT32S g_lrate=0;

INT16S	SetRange(INT32S rate)
{
	INT32S lRangeTime = 0;

	g_lrate = rate;

	if(g_lrate>8000)
		g_lrate = 8000;
	if(g_lrate<0)
		g_lrate = 0;


	while(TRUE)
	{
		if(labs(g_lrate-g_lFifoRange)>400)
		{
			if(g_lrate>g_lFifoRange)
				g_lFifoRange+=400;
				
			else if(g_lrate<g_lFifoRange)
				g_lFifoRange-=400;

			vm_setrange2(g_lFifoRange);
			
			lRangeTime++;
			
			if(lRangeTime>100 )
			{	
				Uart_Printf("g_lRangeTime error!\n");
				break;
			}
		}
		else
		{
			g_lFifoRange = g_lrate;
			vm_setrange2(g_lFifoRange);
			break;
		}
		
		OSTimeDly(1);
	}
	
	return TRUE;
}



INT16S	SetQuiteRange(INT32S rate)
{
	g_lFifoRange = rate;
	vm_setrange2(g_lFifoRange);

	return TRUE;
}

INT32S GetRange()
{
	 return g_lFifoRange;
}

//bSoftCk:是否检测软件限位
//bHardCk:是否检测硬件限位
//ch:
//	ch==0:检测全部轴; ch>0:只检测轴号为ch的轴
//dir:
//  dir>0:只检测正向; dir==0:正负都检测;dir<0:只检测负向

INT16S CheckLimit(BOOL bSoftCk,BOOL bHardCk,INT16S ch,INT32S dir)
{	
	INT16S i=0;
	INT16S res=0;
	
	for(i=0;i<g_Ini.iAxisCount;i++)
	{
		if(ch>0 && ch!=i+1)
			continue;
			
		//if(g_Ini.iAxisSwitch[i]==1 && g_Ini.iAxisRunMode[i]>=0 && g_Ini.iAxisRunMode[i]<=2)
		{
			if(bSoftCk)
			{
				res=CheckSoftLimit(1+i,dir);
				
				if(res!=0)
				{
					if(res%2==0)
						OSQPost(MsgQueue,axisMsg(1+i,g_bEnglish?" Negative Soft Limit Warning!":"软件负限位报警!",TRUE));
					else
						OSQPost(MsgQueue,axisMsg(1+i,g_bEnglish?" Positive Soft Limit Warning!":"软件正限位报警!",TRUE));
					
					return res;
				}
			}
			
			if(bHardCk)
			{
				res=CheckHardLimit(1+i,dir);
				
				if(res!=0)
				{
					if(res%2==0)
						OSQPost(MsgQueue,axisMsg(1+i,g_bEnglish?" Negative Hard Limit Warning!":"硬件负限位报警!",TRUE));
					else
						OSQPost(MsgQueue,axisMsg(1+i,g_bEnglish?" Positive Hard Limit Warning!":"硬件正限位报警!",TRUE));
						
					return res;
				}
			}
		}
	}
	
	return 0;
}

BOOL HomeCheckLimit(INT16S ch,INT16S dir)
{
	char nDMINUS[]={XLMTMINUS,YLMTMINUS,ZLMTMINUS,ULMTMINUS,VLMTMINUS,WLMTMINUS};			//负限位
	char nDPLUS[]={XLMTPLUS,YLMTPLUS,ZLMTPLUS,ULMTPLUS,VLMTPLUS,WLMTPLUS};				//正限位
	
	
	if(ch>CHMAX+1)
		return FALSE;
		
	if(dir)	//1为负限位
	{
		return read_bit(nDMINUS[ch-1])?FALSE:TRUE;
	}
	else	//0为正限位
	{
		return read_bit(nDPLUS[ch-1])?FALSE:TRUE;
	}
}



BOOL CheckLimitStop(INT8S ch,INT8S dir)   //dir 大于零检测正限位  dir小于零检测负限位  dir等于零不检测
{
	long    DestPulse = 1000000L;
	int 	status=1;
	INT8S   buf[0x30]={0};
	BOOL    bCheckLimit = FALSE;
	
	if(ch>MAXCH+1)
		return FALSE;
		
	if(ch==1 && g_bFirstResetX)      //回零时不需要找限位
		goto ERR;
	if(ch==2 && g_bFirstResetY)
		goto ERR;
	if(ch==3 && g_bFirstResetZ && g_Ini.iAxisCount>2)
		goto ERR;
	if(ch==4 && g_bFirstResetU && g_Ini.iAxisCount>3)
		goto ERR; 
		

	Start(ch);
	
	SetMMAddSpeed(ch,1,g_Ini.fBackSpeed[ch-1],g_Ini.fBackSpeed[ch-1]*200.0);
	
	switch(ch)
	{
		case 1:strcpy(buf,g_bEnglish?"X axis finding limit position":"X寻找限位");break;
		case 2:strcpy(buf,g_bEnglish?"Y axis finding limit position":"Y寻找限位");break;
		case 3:strcpy(buf,g_bEnglish?"Z axis finding limit position":"Z寻找限位");break;
		case 4:strcpy(buf,g_bEnglish?"A axis finding limit position":"A寻找限位");break;
		default:break;
	}
	
	OSQPost(MsgQueue,buf);	
	BaseSetCommandPos(ch,0);	  		// 每次寻找Z的限位时  清零一次坐标值					
//	set_actual_pos(ch,0);	       		//
	
	pmove(ch,dir>0?-DestPulse:DestPulse);

	while(1)
	{

		if(HomeCheckLimit(ch,dir) && bCheckLimit==FALSE)
		{
			bCheckLimit =TRUE;
			dec_stop(ch);
		}
		
		get_status(ch,&status);	
		
		if( !status )
		{	
			 break;
		}

	   	OSTimeDly(3);	   
	}	
	
   	if(g_iCardStopFlag[ch-1]!=0)
   	{
		return FALSE;
   	}
	
	if(labs(GetPulse(ch,TRUE))>=DestPulse)
		return FALSE;
	
ERR:	
	return TRUE;	
		
}



INT16S  RunStatusOutCtr(INT16S  status)//1报警，2加工，3停机
{
	static INT16S ostatus = -1;


	if(g_Ini.iWarningOut==0)
		return;
		
//	if(ostatus==status)	
//		return;
		
	ostatus=status;
		
 	if(g_Ini.iWarningOut==1)//单色灯
	{
		switch(status)
		{
			case 0:			//停机
				WriteBit(O_WARNING,0);
				break; 
				
			case 1:    		//报警
				WriteBit(O_WARNING,1);
				break;
			
			case 2:			//加工
				WriteBit(O_WARNING,0);
				break;
		}
	}

 	if(g_Ini.iWarningOut==2)//三色灯
	{
		switch(status)
		{
			case 0:			//停机
				WriteBit(O_WARNING,0);
				WriteBit(O_RUNLED,0);
				WriteBit(O_STOPLED,1);
				break; 
			
			case 1:    		//报警
				WriteBit(O_WARNING,1);
				WriteBit(O_RUNLED,0);
				WriteBit(O_STOPLED,0);
				break;
			
			case 2:			//加工
				WriteBit(O_WARNING,0);
				WriteBit(O_RUNLED,1);
				WriteBit(O_STOPLED,0);
				break;
		}
	}
}



