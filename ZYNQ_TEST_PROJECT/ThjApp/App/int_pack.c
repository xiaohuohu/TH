/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        int_pack.c
  创建人：       范志州
  创建日期：		2004/11/20
  主要功能描述:  中断，加工以及数据解码的一些处理操作 
  		
  修改记录：
   1.  修改者:	      范志州
       修改日期:	      2005/08/04
       主要修改内容:   加工工艺控制过程
   2. ...
*************************************************/
//#define		WORK_GLOBALS

#include	"includes.h"
#include    "StrTool.h"

TEST_EXT	OS_EVENT	*RunSpeedCountSem;
TEST_EXT    OS_EVENT 	*RunLineQueue;
TEST_EXT  	OS_EVENT	*FreeSem;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT  	OS_EVENT 	*LedOutQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		g_bPM;
//TEST_EXT	BOOL		g_bBmp;
TEST_EXT	BOOL		g_bRam;
extern 		COMMAND		Line_Data[MAXLINE+1];
extern 		INT16S		Line_Data_Size;											//总共数据行数
extern 		UNITDATA	g_UnitData;
extern 		BASEINI		g_Ini;
extern  	const 		INT16U	g_uXMS;
extern		const 		INT16U	g_uData;
extern 		INT16U		g_uBmpXMS;

INT8U		g_ucXMSArray[XMSSIZE];
INT8U		g_ucDataArray[DATASIZE];

/*
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4; 
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine ; 
WORK_EXT	INT32S 		g_nBk		;
WORK_EXT	INT32S 		g_nTitleBk	;  
WORK_EXT	INT32S 		g_nTableBk	; 
WORK_EXT	INT32S 		g_Color1	;
*/

WORK_EXT 	INT8S		g_pszMsgBuf[0x30];										//存放信息提示内容避免用局部变量的BUG
WORK_EXT	INT8S		g_pszLedOut[8][3];
WORK_EXT	INT16S 		g_iRunLine;		
WORK_EXT 	INT16S		g_iStaRunLine;	
WORK_EXT 	BOOL	  	g_bFirstDispTime;										//指示是否需要全部刷新日期时间显示，TRUE需要
WORK_EXT    INT32S      g_lAxisSpeed[4];
WORK_EXT 	BOOL   		g_bStartRun;  
WORK_EXT 	BOOL		g_bRuning;    
WORK_EXT 	BOOL		g_bModify;  
WORK_EXT	BOOL		g_bModifySave;
WORK_EXT	BOOL		g_bDataCopy; 
WORK_EXT	BOOL		g_bUnpackCorrect;
WORK_EXT	BOOL		g_bUnpackRuning;
WORK_EXT	INT16S		g_iMoveAxis;
WORK_EXT 	BOOL		g_bFirstResetX;
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;
WORK_EXT 	INT16S		g_iWorkStatus;
WORK_EXT 	BOOL		g_bAutoRunMode;
WORK_EXT 	INT32S		g_lStepNode;	
WORK_EXT 	BOOL		g_bDispRunLine;
WORK_EXT 	BOOL		g_bIsHand;	 
WORK_EXT 	BOOL		g_bStepToSingle;
WORK_EXT 	BOOL		g_bStepToSingle2;
WORK_EXT 	BOOL		g_bMode;	  
WORK_EXT	BOOL		g_bEMSTOP; 
WORK_EXT	INT16S		g_iADValue; 
WORK_EXT	BOOL		g_bHandRun;  
WORK_EXT	INT32U		g_lHandRun;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	FP32		g_lSpeed;
WORK_EXT	FP32 		g_lWidthPos;
WORK_EXT	INT16S		g_iCheckStatusIO;
WORK_EXT	INT16S      g_iOutStatus;
WORK_EXT	BOOL		g_bBackZero;											//指示是否为回零过程
WORK_EXT	INT16S		g_iErrorNo;
WORK_EXT	BOOL		g_bCylinder; 											//指示是否处于气缸测试状态
WORK_EXT	BOOL		g_bScrProtectRun;
WORK_EXT 	INT16S		g_iWorkNumber ;
WORK_EXT 	INT16S		g_iSpeedBeilv ;
WORK_EXT	INT16U 		g_uiMoveKey;
WORK_EXT 	INT32S  	g_lXmsSize;    
WORK_EXT	INT32S		g_lDataCopyXmsSize;
WORK_EXT 	INT32S		g_lYCountLen;       									//加工过程中Y轴的补偿量
WORK_EXT	FP32 		g_fCloseDispCount; 
WORK_EXT	INT16U		g_uiSuspendTask;										//suspend 1~5 task,high byte rev
//WORK_EXT	INT32S		g_lAxisValue[CHMAX];									//记录各轴关断之前的位置
WORK_EXT 	BOOL		g_bModify1;
//TEST_EXT	BOOL		g_bRelease;




//20130815
extern		INT32S  	g_lOutSize;					//记录有汽缸的次数
extern 		OUTDATA 	OutData[100];					//用于存放汽缸数据

int 		g_bMode2=0;
int 		g_bUAxiu=0;
INT32S 		Y_Cont = 0;

extern		void		IRQ_Isr1(void);
extern		void		IRQ_Isr2(void);

//extern 		INT32S    	YlinePos[MAXLINE*2];

INT32S      g_iEmpNode[MAXCH];						  	//标记非空行的最小行号，用于加工中不回零模式

#define		MAXLOOP		3

void Fat_Printf(char *id,FP32 data)
{
	char    buffert[32];
	char    bux[64];
	
	ftoa(data,(INT8U *)buffert,3,10);
	sprintf(bux,"%s%s",id,buffert);
	Uart_Printf("%s\n",bux);
}


//void	UnInstallDriverInterrupt()
//{
//	 rINTMSK = (rINTMSK|BIT_EINT1);
//}


INT16S	UnpackData( )
{

	COMMAND *ptr = Line_Data;
	FP32 	x=0,y=0,z=0,a=0,xspeed=15.0,yspeed=15.0,zspeed=15.0,aspeed=15.0;
	FP32	b=0,c=0,bspeed=15.0,cspeed=15.0;
	INT32S 	lx=0,ly=0,lz=0,la=0,lxspeed=15,lyspeed=15,lzspeed=15,laspeed=15;
	INT32S 	lb=0,lc=0,lbspeed=15,lcspeed=15;
	INT32S  lastlx =-999999,lastly =-999999,lastlz =-999999,lastla=-999999;	//用于记录当前的上次，存进MXS的X,Y,Z,A的值 。
	INT32S  lastlb =-999999,lastlc =-999999;
	INT16S 	l=0;
	INT32S  num =0;															//num 用于标示行号
	INT8U  	status,ostatus[MAXCYLINDER];
	INT8U	enostatus;					//该行是否有气缸操作标志。
	

	INT16S 	iCount=0,i=0,j=0,ThisCount[MAXLOOP+1]={0,0,0,0};
	FP32  	fLoopAdd[MAXCH]={0};
	
	FP64 	n[MAXVALUECOL-1];
/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/
	FP32	time = 0.0;
	FP32	speedbeilv=1.0;
	FP32	tempspeed=1.0;
	INT16S  gotoLine=0;
	INT16S	jumpLine = 0;


	INT16S	iLoop[MAXLOOP+1][2]={{0,0},{0,0},{0,0},{0,0}};
	BOOL	flag=FALSE;
	BOOL    bXReset = FALSE;						//表示X到达0或360，-360是否要位置设零
	BOOL    bYReset = FALSE;						//表示X到达0或360，-360是否要位置设零
	BOOL    bZReset = FALSE;						//表示Z到达0或360，-360是否要位置设零
	BOOL    bAReset = FALSE;						//表示A到达0或360，-360是否要位置设零
	BOOL    bBReset = FALSE;						//表示B到达0或360，-360是否要位置设零
	BOOL    bCReset = FALSE;						//表示C到达0或360，-360是否要位置设零
//	BOOL    bLoop = FALSE;							//是否循环
//	BOOL    bSend = TRUE;							//重复指令是否送线				
	INT16S 	nLoopNo = 0;							//用于编号的唯一性确性，每LOOP一次加500编号
	
	

	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif
	
	for(l=0;l<MAXCYLINDER;l++)
		ostatus[l]=0;
		
	
	for(l=0;l<MAXCH;l++)
	{
		g_iEmpNode[l]=99999999;
	}	
	
	
	if( Line_Data_Size< 2 )
	{
		g_bUnpackCorrect=FALSE;
		g_bDataCopy=FALSE;
		return 100;		
	}
	
	OS_ENTER_CRITICAL();	
	g_lXmsSize=0;
	OS_EXIT_CRITICAL();	
	
	
	//Uart_Printf("Line=%d\n",Line_Data_Size);
	
	for(i=0; i<Line_Data_Size; )
	{
	
BEGIN_ACCESS:
		/*if(gotoLine>0)
		{
			i = gotoLine-1;
			gotoLine = 0;
		}*/
			
		ptr = Line_Data + i;

		num =i*2 + nLoopNo*500;												//第0行用于存速度值		

/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/
		
		switch(ptr->value[0][0])
		{
			case 'S':
			{
				OS_ENTER_CRITICAL();
				SetUnitData(num,SPEED);
				OS_EXIT_CRITICAL();
				
				if(ptr->value[1][0])xspeed = GetMMFromAngle(1,stringtof((INT8U *)ptr->value[1])*360.0/60.0);			//速度倍乘在速度平滑算法以具备，我们反馈给算法的是真实速度。
				//if(ptr->value[3][0])yspeed = stringtof((INT8U *)ptr->value[3])*1000.0/60.0;
				
				if(ptr->value[3][0])yspeed = (g_Ini.iAxisRunMode[1] > 1)?(( FP32 )stringtof( ( INT8U * ) ptr->value[3] )/60.0 * 1000.0)
												:GetMMFromAngle(2,stringtof((INT8U *)ptr->value[3])*360.0/60.0);
				if(ptr->value[4][0])zspeed = (g_Ini.iAxisRunMode[2] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[4] )/60.0 * 1000.0)
												:GetMMFromAngle(3,stringtof((INT8U *)ptr->value[4])*360.0/60.0);
				if(ptr->value[5][0])aspeed = (g_Ini.iAxisRunMode[3] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[5] )/60.0 * 1000.0)
												:GetMMFromAngle(4,stringtof((INT8U *)ptr->value[5])*360.0/60.0);
				if(ptr->value[6][0])bspeed = (g_Ini.iAxisRunMode[4] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[6] )/60.0 * 1000.0)
												:GetMMFromAngle(5,stringtof((INT8U *)ptr->value[6])*360.0/60.0);
				if(ptr->value[7][0])cspeed = (g_Ini.iAxisRunMode[5] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[7] )/60.0 * 1000.0)
												:GetMMFromAngle(6,stringtof((INT8U *)ptr->value[7])*360.0/60.0);
		
				lxspeed=GetPulseFromMM(1,xspeed);
				lyspeed=GetPulseFromMM(2,yspeed);
				lzspeed=GetPulseFromMM(3,zspeed);
				laspeed=GetPulseFromMM(4,aspeed);
				lbspeed=GetPulseFromMM(5,bspeed);
				lcspeed=GetPulseFromMM(6,cspeed);
				
				g_UnitData.lValue[0] = lxspeed;
				g_UnitData.lValue[1] = lyspeed;
				g_UnitData.lValue[2] = lzspeed;
				g_UnitData.lValue[3] = laspeed;
				g_UnitData.lValue[4] = lbspeed;
				g_UnitData.lValue[5] = lcspeed;
				
				MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));		//把加工数据存储g_ucDataArray数组
				g_lXmsSize ++;
				
			}break;
			
			case 'M':
			{	
				//*****将字符串数据转化为浮点数据*****//
				{
					BOOL bEndCode = TRUE;
					
					for(j=0;j<MAXVALUECOL-1;j++)
						n[j]=-10000.0;
					
					for( j=0; j<MAXVALUECOL-1; j++)
					{
						
						if( ptr->value[j+1][0] )		
						{							 
							bEndCode = FALSE;				
						}	
					}
					
					if(bEndCode)
					{
						i++;
						continue;
					}
					
/*
value[0]为指令，value[1]X开始角，value[2]X结束角，value[3]送线，value[4]转芯，value[5]卷取
value[6]卷取，value[7]夹耳，value[8]为速比，value[9]为探针，value[10]为气缸，value[11]为延时
*/			
					for( j=0; j<MAXVALUECOL-1; j++)
					{
						if( ptr->value[j+1][0] )		
						{						
							n[j] = (FP64)stringtof((INT8U *)ptr->value[j+1]);				//字符串转化为浮点数,+ - 被转成 0
							
							if(j<7 && g_Ini.iAxisCa==1)
							{
								/*
								if(j==2)													//j=2送线轴
								{
									n[j] = n[j]/100.0;
								}
								else
								*/
								if(j >= 2 && j <= 6 )
								{
									if(g_Ini.iAxisRunMode[j-1] == 1)
										n[j] = n[j];
									else if(g_Ini.iAxisRunMode[j-1] == 0)
										n[j] = n[j]/10.0;
									else if(g_Ini.iAxisRunMode[j-1] == 2 || g_Ini.iAxisRunMode[j-1] == 3)
										n[j] = n[j]/100.0;
								}
								else														//j =0;j=1;j=3;j=4;X开,X结束，转心轴，圈曲										
								{						
									n[j]=n[j]/10.0;
								}
							}
						}				
					}
					
				}
				
				//**************将数据解码后放到g_ucDataArray[]*******************//	
				{
					/*----------------------开始角处理----------------------------*/				
									
					
					if(fabs(n[0]+10000)>0.001)													
					{	
					
					
						/*
						if(g_Ini.iCyldMode == 0)											//气缸01模式
						{																	
							INT16S templen=strlen(ptr->value[MAXVALUECOL-2]);
						
							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;
						
							if(templen>0)
							{
								for( l=0;l<templen;l++)
								{
									if(ptr->value[MAXVALUECOL-2][l]=='0')
										ostatus[l]=0;
									else
									{
										if(ptr->value[MAXVALUECOL-2][l]=='1')
											ostatus[l]=1;
										else
											ostatus[l]=2;
									}
								}
							}
						}
						else															 //正负模式
						{
							INT16S templen=strlen(ptr->value[MAXVALUECOL-2]);
							
							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;
								
							if(templen>0)
							{
								if(templen==1 && ptr->value[MAXVALUECOL-2][0]=='0')
								{
									for(l=0;l<MAXCYLINDER-1;l++)//8号输出为线架输出不关闭。
										ostatus[l]=0;
								}
								else
								{
									for( l=0;l<templen;l++)
									{
										if(ptr->value[MAXVALUECOL-2][l]=='-')
										{
											if(ptr->value[MAXVALUECOL-2][l+1]>'0'&& ptr->value[MAXVALUECOL-2][l+1]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-2][l+1]-'0'-1] = 0;
												
											++l;
											
										}
										else 
										{
											if(ptr->value[MAXVALUECOL-2][l]>'0'&& ptr->value[MAXVALUECOL-2][l]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-2][l]-'0'-1] = 1;
										}

									}
								}
							}
						}
					
						if(fabs(n[10]+10000.0)>0.001 && g_Ini.iAxisCount<5)						//延时
							time = n[10];
						else
							time = 0.0;
					*/
					
						if(strcmp(ptr->value[1],"S")==0)								//开始角就近归零
						{
							if(fabs(lx)<g_Ini.lPulse[0]*0.5)
							{
								lx  = 0;
							}
							else
							{
								if(lx>0)
									lx  = g_Ini.lPulse[0];
								else
									lx  = -g_Ini.lPulse[0];
							}
							
							bXReset = TRUE;
						
						}
						else if(strcmp(ptr->value[1],"+")==0)							//往正方向归零
						{
							if(lx>0)
								lx  = g_Ini.lPulse[0];
							else
								lx  = 0;
								
							bXReset = TRUE;
						
						}
						else if(strcmp(ptr->value[1],"-")==0)							//往负方向归零
						{
							if(lx<0)
								lx  = -g_Ini.lPulse[0];
							else
								lx  = 0;				
					
							bXReset = TRUE;
						}
						else															//解析开始角
						{			
							if( fabs( n[0]+10000.0 ) >0.001 )
							{
								if(g_lXmsSize-1<g_iEmpNode[0])							//找出最小非空行的标号
								{
									g_iEmpNode[0]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
							
								x = n[0];
								lx= GetPulseFromAngle(1,x);
							}
							
							bXReset = FALSE;
						}													
						
						SetUnitData(num-1,MOVE);
						
						g_UnitData.SpindleAxis = 1;										//主轴，1为X轴，2为Y，3为Z，4为A轴
						g_UnitData.lValue[0] = lastlx = lx;
						g_UnitData.lValue[1] = lastly = ly;
						g_UnitData.lValue[2] = lastlz = lz;				
						g_UnitData.lValue[3] = lastla = la;
						g_UnitData.lValue[4] = lastlb = lb;				
						g_UnitData.lValue[5] = lastlc = lc;
						g_UnitData.lValue[6] = lxspeed;	
						
						
					//	for(l=0;l<MAXCYLINDER;l++)
					//		g_UnitData.nOut[l]=ostatus[l];
							
					//	g_UnitData.fDelay = time;
						
						if(bXReset)
						{
							lastlx = lx = 0;
							bXReset = FALSE;
							g_UnitData.nReset = g_UnitData.nReset|1;
						}
						
						MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
						g_lXmsSize ++;
						
					}
					
					/*----------------------结束角处理----------------------------*/
					if(fabs(n[1]+10000)>0.001)											
					{
						if(strcmp(ptr->value[2],"S")==0)								//结束角就近归零
						{
							if(fabs(lx)<g_Ini.lPulse[0]*0.5)
							{
								lx  = 0;
							}
							else
							{
								if(lx>0)
								{
									lx  = g_Ini.lPulse[0];
								}
								else
								{
									lx  = -g_Ini.lPulse[0];
								}
							}		
							
							bXReset = TRUE;
						
						}
						else if(strcmp(ptr->value[2],"+")==0)						//结束角正方向归零
						{
							if(lx>0)
								lx  = g_Ini.lPulse[0];
							else
								lx  = 0;	
								
							bXReset = TRUE;
						
						}
						else if(strcmp(ptr->value[2],"-")==0)						//结束角负方向归零
						{
							if(lx<0)
								lx  = -g_Ini.lPulse[0];
							else
								lx  = 0;	
						
							bXReset = TRUE;
						
						}
						else									 					//解析结束角  在定点送线里的解析
						{
							if( fabs( n[1]+10000.0 ) >0.001)
							{
								if(g_lXmsSize-1<g_iEmpNode[0])							//找出最小非空行的标号
								{
									g_iEmpNode[0]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
								
								x = n[1];
								lx= GetPulseFromAngle(1,x);
							}
							
							bXReset = FALSE;

						}
					}
					
					/*----------------------送线轴位置解析   Y轴与Z轴一样------------------------*/
					/*----------------------送线轴位置解析------------------------*/
					{
						if(g_Ini.iAxisRunMode[1]==0 || g_Ini.iAxisRunMode[1]==1)//单圈或者多圈模式
						{
							INT32S tmply =0;
							INT32S tmpts = 0;
						
							tmply = ly%g_Ini.lPulse[1];							//求余数
							tmpts = ly/g_Ini.lPulse[1];							//求圈
						
							if(strcmp(ptr->value[3],"+")==0)					//轴正向归零
							{
								if(tmply>0)
									ly  = (tmpts+1)*g_Ini.lPulse[1];
								else
									ly  = tmpts*g_Ini.lPulse[1];	
									
								bYReset = TRUE;	
							}					
							else if(strcmp(ptr->value[3],"-")==0)				//轴反向归零
							{
								if(tmply<0)
									ly  = (tmpts-1)*g_Ini.lPulse[1];
								else
									ly  = tmpts*g_Ini.lPulse[1];	
									
								bYReset = TRUE;		
							}	
							else if(g_Ini.iAxisRunMode[1] ==1 && strcmp(ptr->value[3],"0")==0)
							{
								if(labs(tmply)<g_Ini.lPulse[1]*0.5)
								{
									ly  = tmpts*g_Ini.lPulse[1];
								}
								else
								{
									if(ly>0)
										ly  = (tmpts+1)*g_Ini.lPulse[1];
									else
										ly  = (tmpts-1)*g_Ini.lPulse[1];
								}
								bYReset = TRUE;								
							}					
							else												//轴位置解析。
							{
								if( fabs(n[2]+10000)>0.001 )
								{
								
									if(g_lXmsSize-1<g_iEmpNode[1])							//找出最小非空行的标号
									{
										g_iEmpNode[1]=g_lXmsSize-1;
									}
								
								
									if(g_Ini.iAxisRunMode[1] ==0)
									{
										y = n[2];			
										ly = GetPulseFromAngle(2,y);			//单圈解析为绝对位置
									}
									else if(g_Ini.iAxisRunMode[1] ==1)			//多圈走相对位置
									{																											
										INT16S 	temp;
										FP32	tempf;
										FP32 tt=n[2];
								
										temp=(INT16S)tt;						
										tempf=n[2]-temp;
										y=temp*360+tempf*1000.0;
										
										if(g_Ini.iAxisMulRunMode[1]==0)
										{
											ly = ly + GetPulseFromAngle(2,y);		//解析为相对位置						
										}
										else
										{
											ly=GetPulseFromAngle(2,y);
										}
											
									}
								}

								bYReset = FALSE;	
							}						
						
						}
						else if(g_Ini.iAxisRunMode[1] ==2)						//丝杆模式，走绝对位置
						{
							if( fabs(n[2]+10000)>0.001)							//
							{
								if(g_lXmsSize-1<g_iEmpNode[1])							//找出最小非空行的标号
								{
									g_iEmpNode[1]=g_lXmsSize-1;
								}
							
								y= n[2];
								
								if(g_Ini.iAxisMulRunMode[1]==0)						//解析成相对	
								{
									if(GetPulseFromMM(2,y)==0)		//若设0相对位置直接走到零点
										ly=0;
									else
										ly+= GetPulseFromMM(2,y);	
								}
								else			
									ly = GetPulseFromMM(2,y);					
							}

							bYReset = FALSE;
						}					
						else if(g_Ini.iAxisRunMode[1] ==3)						//送线模式做了特殊处理，在RunAllData中，ly的值即为发送的脉冲数值	
						{
							if( fabs(n[2]+10000)>0.001)													
							{
							//	y+= n[2];
							//	ly= GetPulseFromMM(2,y);														
								
								if(iCount>0)
								{
									n[2]+=(fLoopAdd[1]*ThisCount[iCount-1]);
								}
								
								y+= n[2];
								ly+= GetPulseFromMM(2,n[2]);	
						
							}
							
							bYReset = FALSE;
						}
					}
					
					
					/*----------------------  Z轴 转芯轴位置解析------------------------*/	
					{																	//Z轴转芯轴解析过程 
				
						INT32S tmplz =0;
						INT32S tmpts = 0;
					
						tmplz = lz%g_Ini.lPulse[2];										//求余数
						tmpts = lz/g_Ini.lPulse[2];										//求圈
						
						if(strcmp(ptr->value[4],"S")==0)								//转芯轴就近归零
						{
							if(labs(tmplz)<g_Ini.lPulse[2]*0.5)
							{
								lz  = tmpts*g_Ini.lPulse[2];
							}
							else
							{
								if(lz>0)
									lz  = (tmpts+1)*g_Ini.lPulse[2];
								else
									lz  = (tmpts-1)*g_Ini.lPulse[2];
							}
							
							bZReset = TRUE;
							
						}
						else if(strcmp(ptr->value[4],"+")==0)							//转芯轴正向归零
						{
							if(tmplz>0)
								lz  = (tmpts+1)*g_Ini.lPulse[2];
							else
								lz  = tmpts*g_Ini.lPulse[2];	
								
							bZReset = TRUE;	
						}
						else if(strcmp(ptr->value[4],"-")==0)							//转芯轴负向归零
						{
							if(tmplz<0)
								lz  = (tmpts-1)*g_Ini.lPulse[2];
							else
								lz  = tmpts*g_Ini.lPulse[2];	
								
							bZReset = TRUE;	
						}
						else															//转芯轴位置解析。
						{
							if( fabs(n[3]+10000)>0.001 )
							{
								if(g_lXmsSize-1<g_iEmpNode[2])							//找出最小非空行的标号
								{
									g_iEmpNode[2]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
							
								if(g_Ini.iAxisRunMode[2] ==0)							//单圈绝对位置解析
								{
									z = n[3];			
									lz = GetPulseFromAngle(3,z);
								}
								else if(g_Ini.iAxisRunMode[2] ==1)						//多圈走相对或绝对位置
								{																											
									INT16S 	temp;
									FP32	tempf;
									FP64 tt=n[3];
							
									temp=(INT16S)tt;						
									tempf=n[3]-temp;
									z=temp*360+tempf*1000.0;
																			
									if(g_Ini.iAxisMulRunMode[2]==0)	
										lz = lz + GetPulseFromAngle(3,z);					//相对
									else if(g_Ini.iAxisMulRunMode[2]==1)
										lz = GetPulseFromAngle(3,z);						//绝对			

								}
								else  if(g_Ini.iAxisRunMode[2] ==2)						//丝杆走绝对位置
								{
									z = n[3];	
									
									if(g_Ini.iAxisMulRunMode[2]==0)
									{	
										if(GetPulseFromMM(3,z)==0)		//若设0相对位置直接走到零点
											lz=0;
										else
											lz += GetPulseFromMM(3,z);
									}
									else							
										lz = GetPulseFromMM(3,z);
								}
							}
							
							bZReset = FALSE;
						}
					}
			
	
					/*----------------------A轴 卷取轴位置解析------------------------*/	
					{																	//A轴卷取轴解析过程 
						INT32S tmpla =0;
						INT32S tmpts = 0;
					
						tmpla = la%g_Ini.lPulse[3];										//求余数
						tmpts = la/g_Ini.lPulse[3];										//求圈
						
						if(strcmp(ptr->value[5],"S")==0)								//卷取轴就近归零
						{
							if(labs(tmpla)<g_Ini.lPulse[3]*0.5)
							{
								la  = tmpts*g_Ini.lPulse[3];
							}
							else
							{
								if(la>0)
									la  = (tmpts+1)*g_Ini.lPulse[3];
								else
									la  = (tmpts-1)*g_Ini.lPulse[3];
							}
							
							bAReset = TRUE;
							
						}
						else if(strcmp(ptr->value[5],"+")==0)							//卷取轴正向归零
						{
						
							if(tmpla>0)
								la  = (tmpts+1)*g_Ini.lPulse[3];
							else
								la  = tmpts*g_Ini.lPulse[3];	
								
							bAReset = TRUE;	
						}
						else if(strcmp(ptr->value[5],"-")==0)							//卷取轴负向归零
						{
						
							if(tmpla<0)
								la  = (tmpts-1)*g_Ini.lPulse[3];
							else
								la  = tmpts*g_Ini.lPulse[3];	
								
							bAReset = TRUE;	
						}
						else															//卷取轴位置解析。
						{
							if( fabs(n[4]+10000)>0.001 )
							{
							
								if(g_lXmsSize-1<g_iEmpNode[3])							//找出最小非空行的标号
								{
									g_iEmpNode[3]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
								
								if(g_Ini.iAxisRunMode[3] == 0)								//单圈绝对位置解析
								{
									a = n[4];			
									la = GetPulseFromAngle(4,a);
								}
								else if(g_Ini.iAxisRunMode[3] ==1)							//多圈走相对或绝对位置
								{																											
									INT16S 	temp;
									FP32	tempf;
									FP64 tt=n[4];
							
									temp=(INT16S)tt;						
									tempf=n[4]-temp;
									a=temp*360+tempf*1000.0;
																			
									if(g_Ini.iAxisMulRunMode[3]==0)	
										la = la + GetPulseFromAngle(4,a);					//相对位置
									else if(g_Ini.iAxisMulRunMode[3]==1)
										la =  GetPulseFromAngle(4,a);						//绝对位置	

								}
								else  if(g_Ini.iAxisRunMode[3] ==2)						//丝杆走绝对位置
								{
									a = n[4];
									
									if(g_Ini.iAxisMulRunMode[3]==0)	
									{
										if(GetPulseFromMM(4,a)==0)
											la=0;
										else
											la += GetPulseFromMM(4,a);
									}
									else
										la = GetPulseFromMM(4,a);
								}
							}
							
							bAReset = FALSE;
						}
					}
					
						/*----------------------B轴 卷取2轴位置解析------------------------*/	
					{																	//B轴卷取轴解析过程 
						INT32S tmpla =0;
						INT32S tmpts = 0;
					
						tmpla = lb%g_Ini.lPulse[4];										//求余数
						tmpts = lb/g_Ini.lPulse[4];										//求圈
						
						if(strcmp(ptr->value[6],"S")==0)								//卷取轴就近归零
						{
							if(labs(tmpla)<g_Ini.lPulse[4]*0.5)
							{
								lb  = tmpts*g_Ini.lPulse[4];
							}
							else
							{
								if(lb>0)
									lb  = (tmpts+1)*g_Ini.lPulse[4];
								else
									lb  = (tmpts-1)*g_Ini.lPulse[4];
							}
							
							bBReset = TRUE;
							
						}
						else if(strcmp(ptr->value[6],"+")==0)							//卷取轴正向归零
						{
						
							if(tmpla>0)
								lb  = (tmpts+1)*g_Ini.lPulse[4];
							else
								lb  = tmpts*g_Ini.lPulse[4];	
								
							bBReset = TRUE;	
						}
						else if(strcmp(ptr->value[6],"-")==0)							//卷取轴负向归零
						{
						
							if(tmpla<0)
								lb  = (tmpts-1)*g_Ini.lPulse[4];
							else
								lb  = tmpts*g_Ini.lPulse[4];	
								
							bBReset = TRUE;	
						}
						else															//卷取轴位置解析。
						{
							if( fabs(n[5]+10000)>0.001 )
							{
								if(g_lXmsSize-1<g_iEmpNode[4])							//找出最小非空行的标号
								{
									g_iEmpNode[4]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
								
								
								if(g_Ini.iAxisRunMode[4] == 0)								//单圈绝对位置解析
								{
									b = n[5];			
									lb = GetPulseFromAngle(5,b);
								}
								else if(g_Ini.iAxisRunMode[4] ==1)							//多圈走相对或绝对位置
								{																											
									INT16S 	temp;
									FP32	tempf;
									FP64 tt=n[5];
							
									temp=(INT16S)tt;						
									tempf=n[5]-temp;
									b=temp*360+tempf*1000.0;
																			
									if(g_Ini.iAxisMulRunMode[4]==0)	
										lb = lb + GetPulseFromAngle(5,b);					//相对位置		
									else if(g_Ini.iAxisMulRunMode[4]==1)
										lb =  GetPulseFromAngle(5,b);						//绝对位置	

								}
								else  if(g_Ini.iAxisRunMode[4] ==2)						//丝杆走绝对位置
								{
									
									b = n[5];			
									lb = GetPulseFromMM(5,b);
								
								}
							}
							
							bBReset = FALSE;
						}
					}
					
					/*----------------------C轴 夹耳轴位置解析------------------------*/	
					{																	//C轴夹耳轴解析过程 
						INT32S tmpla =0;
						INT32S tmpts = 0;
					
						tmpla = lc%g_Ini.lPulse[5];										//求余数
						tmpts = lc/g_Ini.lPulse[5];										//求圈
						
						if(strcmp(ptr->value[7],"S")==0)								//卷取轴就近归零
						{
							if(labs(tmpla)<g_Ini.lPulse[5]*0.5)
							{
								lc  = tmpts*g_Ini.lPulse[5];
							}
							else
							{
								if(lc>0)
									lc  = (tmpts+1)*g_Ini.lPulse[5];
								else
									lc  = (tmpts-1)*g_Ini.lPulse[5];
							}
							
							bCReset = TRUE;
							
						}
						else if(strcmp(ptr->value[7],"+")==0)							//卷取轴正向归零
						{
						
							if(tmpla>0)
								lc  = (tmpts+1)*g_Ini.lPulse[5];
							else
								lc  = tmpts*g_Ini.lPulse[5];	
								
							bCReset = TRUE;	
						}
						else if(strcmp(ptr->value[7],"-")==0)							//卷取轴负向归零
						{
						
							if(tmpla<0)
								lc  = (tmpts-1)*g_Ini.lPulse[5];
							else
								lc  = tmpts*g_Ini.lPulse[5];	
								
							bCReset = TRUE;	
						}
						else															//卷取轴位置解析。
						{
							if( fabs(n[6]+10000)>0.001 )
							{
								if(g_lXmsSize-1<g_iEmpNode[5])							//找出最小非空行的标号
								{
									g_iEmpNode[5]=g_lXmsSize-1;
									
									//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
								}
							
								if(g_Ini.iAxisRunMode[5] == 0)								//单圈绝对位置解析
								{
									c = n[6];			
									lc = GetPulseFromAngle(6,c);
								}
								else if(g_Ini.iAxisRunMode[5] ==1)							//多圈走相对或绝对位置
								{																											
									INT16S 	temp;
									FP32	tempf;
									FP64 tt=n[6];
							
									temp=(INT16S)tt;						
									tempf=n[6]-temp;
									c=temp*360+tempf*1000.0;
																			
									if(g_Ini.iAxisMulRunMode[5]==0)	
										lc = lc + GetPulseFromAngle(6,c);					//相对位置	
									else if(g_Ini.iAxisMulRunMode[5]==1)
										lc =  GetPulseFromAngle(6,c);						//绝对位置	

								}
								else  if(g_Ini.iAxisRunMode[5] ==2)						//丝杆走绝对位置
								{
									
									c = n[6];			
									lc = GetPulseFromMM(6,c);
								
								}
							}
							
							bCReset = FALSE;
						}
					}
					

					
					if( fabs(n[7]+10000)>0.001 )										//速比 注：其速比有效范围为开始到结束角这一段，
					{																	//这与CNC502B的到开始角，开始角到结束角的有效范围是不一样的
						speedbeilv = n[7];
					}
					else
					{
						speedbeilv = 1.0;
					}
					
					{//气缸
						if(g_Ini.iCyldMode == 0)											//气缸01模式
						{																	
							INT16S templen=strlen(ptr->value[MAXVALUECOL-2]);
						
							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;
						
							if(templen>0)
							{
								for( l=0;l<templen;l++)
								{
									if(ptr->value[MAXVALUECOL-2][l]=='0')
										ostatus[l]=0;
									else
									{
										if(ptr->value[MAXVALUECOL-2][l]=='1')
											ostatus[l]=1;
										else
											ostatus[l]=2;
									}
								}
							}
						}
						else															 //正负模式
						{	
							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;
						
							if(fabs(n[9]+10000.0)>0.001)
							{
								INT16S iout = (INT16S)n[9];
								
								if(iout==0)
								{}
								else if(iout>0 && iout<=16)
								{
									ostatus[abs(iout)-1]=1;
								}
								else if(iout<0 && iout>=-16)
								{
									ostatus[abs(iout)-1]=0;
								}
							}
							
							
						/*
							INT16S templen=strlen(ptr->value[MAXVALUECOL-2]);
							
							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;
								
							if(templen>0)
							{
								if(templen==1 && ptr->value[MAXVALUECOL-2][0]=='0')
								{
									for(l=0;l<MAXCYLINDER-1;l++)//8号输出为线架输出不关闭。
										ostatus[l]=0;
								}
								else
								{
									for( l=0;l<templen;l++)
									{
										if(ptr->value[MAXVALUECOL-2][l]=='-')
										{
											if(ptr->value[MAXVALUECOL-2][l+1]>'0'&& ptr->value[MAXVALUECOL-2][l+1]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-2][l+1]-'0'-1] = 0;
												
											++l;
											
										}
										else 
										{
											if(ptr->value[MAXVALUECOL-2][l]>'0'&& ptr->value[MAXVALUECOL-2][l]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-2][l]-'0'-1] = 1;
										}

									}
								}
							}
							*/
						}
						
						enostatus = 0;			//是否有气缸操作标志
						
						for(l=0;l<MAXCYLINDER;l++)
						{
							if(ostatus[l]!=2)
							{	
								enostatus =1;
								break;
							}	
						}
					}
					
					
					if( fabs(n[8]+10000)>0.001 )										//探针
					{
						status = (INT16S)n[8];
					}
					else
					{
						status = 0;
					}
					
					
					if(fabs(n[10]+10000.0)>0.001 && g_Ini.iAxisCount<5)						//延时
						time = n[10];
					else
						time = 0.0;
					
					
					

					{																	//开关轴的处理													
						if(g_Ini.iAxisSwitch[1]==0)
							ly = 0;					
						
						if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
							lz = 0;
							
						if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
							la = 0;
						if(g_Ini.iAxisCount<5 || g_Ini.iAxisSwitch[4]==0)
							lb = 0;
						if(g_Ini.iAxisCount<6 || g_Ini.iAxisSwitch[5]==0)
							lc = 0;
					}
					
					////******将转化的浮点数据放到全局结构体中g_UnitData****/////
					{
						if( fabs( n[1]+10000.0 ) >0.001 && lx != lastlx)		//当结束角编辑不为空时
						{
							
						//	Uart_Printf("num %10ld  ,YlinePos[num] = %10ld \n",num,YlinePos[num]);
							
							SetUnitData(num,MOVE);
							g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴

							g_UnitData.lValue[0] = lastlx = lx;
							g_UnitData.lValue[1] = lastly = ly;
							g_UnitData.lValue[2] = lastlz = lz;				
							g_UnitData.lValue[3] = lastla = la;
							g_UnitData.lValue[4] = lastlb = lb;				
							g_UnitData.lValue[5] = lastlc = lc;
							tempspeed = xspeed*speedbeilv;
							
							g_UnitData.lValue[6] = GetPulseFromMM(1,tempspeed);
						
							g_UnitData.nIO = status;					//探针
						
							for(l=0;l<MAXCYLINDER;l++)
								g_UnitData.nOut[l]=ostatus[l];
								
							g_UnitData.fDelay = time;
						
							if(bXReset)
							{
								bXReset = FALSE;
								lastlx = lx = 0;
								g_UnitData.nReset = g_UnitData.nReset|1;
							}
							
							if(bYReset)
							{
								bYReset = FALSE;
								lastly = ly = 0;
								g_UnitData.nReset = g_UnitData.nReset|2;
							}
							
							if(bZReset)
							{
								bZReset = FALSE;
								lastlz = lz = 0;
								g_UnitData.nReset = g_UnitData.nReset|4;
							}
							
							if(bAReset)
							{
								bAReset = FALSE;
								lastla = la = 0;
								g_UnitData.nReset = g_UnitData.nReset|8;
							}
							
							if(bBReset)
							{
								bBReset = FALSE;
								lastlb = lb = 0;
								g_UnitData.nReset = g_UnitData.nReset|16;
							}
							
							if(bCReset)
							{
								bCReset = FALSE;
								lastlc = lc = 0;
								g_UnitData.nReset = g_UnitData.nReset|32;
							}
						
							MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
							g_lXmsSize ++;
							
						}
						else											
						{
							if(fabs( n[2]+10000.0 ) >0.001 && ly !=lastly )				//送线轴编辑不为空时
							{
								
						//		Uart_Printf("num %10ld  ,YlinePos[num] = %10ld \n",num,YlinePos[num]);
							
								SetUnitData(num,MOVE);
								g_UnitData.SpindleAxis = 2;					//主轴，1为X轴，2为Y，3为Z，4为A轴

								g_UnitData.lValue[0] = lastlx = lx;
								g_UnitData.lValue[1] = lastly = ly;
								g_UnitData.lValue[2] = lastlz = lz;				
								g_UnitData.lValue[3] = lastla = la;
								g_UnitData.lValue[4] = lastlb = lb;				
								g_UnitData.lValue[5] = lastlc = lc;
								
								
								tempspeed = yspeed*speedbeilv;
							
								g_UnitData.lValue[6] = GetPulseFromMM(2,tempspeed);
								
								g_UnitData.nIO = status;		
								
								for(l=0;l<MAXCYLINDER;l++)
									g_UnitData.nOut[l]=ostatus[l];
								
								g_UnitData.fDelay = time;

								if(bXReset)
								{
									bXReset = FALSE;
									lastlx = lx = 0;
									g_UnitData.nReset = g_UnitData.nReset|1;
								}
								
								if(bYReset)
								{
									bYReset = FALSE;
									lastly = ly = 0;
									g_UnitData.nReset = g_UnitData.nReset|2;
								}
								
								if(bZReset)
								{
									bZReset = FALSE;
									lastlz = lz = 0;
									g_UnitData.nReset = g_UnitData.nReset|4;
								}
									
								if(bAReset)
								{
									bAReset = FALSE;
									lastla = la = 0;
									g_UnitData.nReset = g_UnitData.nReset|8;
								}
								
								if(bBReset)
								{
									bBReset = FALSE;
									lastlb = lb = 0;
									g_UnitData.nReset = g_UnitData.nReset|16;
								}
								
								if(bCReset)
								{
									bCReset = FALSE;
									lastlc = lc = 0;
									g_UnitData.nReset = g_UnitData.nReset|32;
								}
						
								MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
								g_lXmsSize ++;
								
							}
							else
							{
								if(fabs( n[3]+10000.0 ) >0.001 && lz !=lastlz)			//当转芯轴编辑不为空时
								{
								//	YlinePos[num] = ly;
									
								//	Uart_Printf("num %10ld  ,YlinePos[num] = %10ld \n",num,YlinePos[num]);
								
									SetUnitData(num,MOVE);
									g_UnitData.SpindleAxis = 3;							//主轴，1为X轴，2为Y，3为Z，4为A轴


									g_UnitData.lValue[0] = lastlx = lx;
									g_UnitData.lValue[1] = lastly = ly;
									g_UnitData.lValue[2] = lastlz = lz;				
									g_UnitData.lValue[3] = lastla = la;
									g_UnitData.lValue[4] = lastlb = lb;				
									g_UnitData.lValue[5] = lastlc = lc;
									tempspeed = zspeed*speedbeilv;
							
									g_UnitData.lValue[6] = GetPulseFromMM(3,tempspeed);
									
									g_UnitData.nIO = status;
									
									for(l=0;l<MAXCYLINDER;l++)
										g_UnitData.nOut[l]=ostatus[l];
								
									g_UnitData.fDelay = time;							
						
									if(bXReset)
									{
										bXReset = FALSE;
										lastlx = lx = 0;
										g_UnitData.nReset = g_UnitData.nReset|1;
									}
									
									if(bYReset)
									{
										bYReset = FALSE;
										lastly = ly = 0;
										g_UnitData.nReset = g_UnitData.nReset|2;
									}
									
									if(bZReset)
									{
										bZReset = FALSE;
										lastlz = lz = 0;
										g_UnitData.nReset = g_UnitData.nReset|4;
									}
									
									if(bAReset)
									{
										bAReset = FALSE;
										lastla = la = 0;
										g_UnitData.nReset = g_UnitData.nReset|8;
									}
									
									if(bBReset)
									{
										bBReset = FALSE;
										lastlb = lb = 0;
										g_UnitData.nReset = g_UnitData.nReset|16;
									}
									
									if(bCReset)
									{
										bCReset = FALSE;
										lastlc = lc = 0;
										g_UnitData.nReset = g_UnitData.nReset|32;
									}
						
									MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize ++;
									
								}
								else 
								{
									if(fabs( n[4]+10000.0 ) >0.001 && la !=lastla)
									{
										SetUnitData(num,MOVE);
										g_UnitData.SpindleAxis = 4;							//主轴，1为X轴，2为Y，3为Z，4为A轴

										g_UnitData.lValue[0] = lastlx = lx;
										g_UnitData.lValue[1] = lastly = ly;
										g_UnitData.lValue[2] = lastlz = lz;				
										g_UnitData.lValue[3] = lastla = la;
										g_UnitData.lValue[4] = lastlb = lb;				
										g_UnitData.lValue[5] = lastlc = lc;
										tempspeed = aspeed*speedbeilv;
								
										g_UnitData.lValue[6] = GetPulseFromMM(4,tempspeed);
										
										g_UnitData.nIO = status;
										
										for(l=0;l<MAXCYLINDER;l++)
											g_UnitData.nOut[l]=ostatus[l];
									
										g_UnitData.fDelay = time;
																	
										if(bXReset)
										{
											bXReset = FALSE;
											lastlx = lx = 0;
											g_UnitData.nReset = g_UnitData.nReset|1;
										}
										
										if(bYReset)
										{
											bYReset = FALSE;
											lastly = ly = 0;
											g_UnitData.nReset = g_UnitData.nReset|2;
										}
										
										if(bZReset)
										{
											bZReset = FALSE;
											lastlz = lz = 0;
											g_UnitData.nReset = g_UnitData.nReset|4;
										}
										
										if(bAReset)
										{
											bAReset = FALSE;
											lastla = la = 0;
											g_UnitData.nReset = g_UnitData.nReset|8;
										}
										
										if(bBReset)
										{
											bBReset = FALSE;
											lastlb = lb = 0;
											g_UnitData.nReset = g_UnitData.nReset|16;
										}
										
										if(bCReset)
										{
											bCReset = FALSE;
											lastlc = lc = 0;
											g_UnitData.nReset = g_UnitData.nReset|32;
										}
							
										MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
										g_lXmsSize ++;
										
									}
									else
									{
										if(fabs( n[5]+10000.0 ) >0.001 && lb !=lastlb)
										{
											
											SetUnitData(num,MOVE);
											g_UnitData.SpindleAxis = 5;							//主轴，1为X轴，2为Y，3为Z，4为A轴

											g_UnitData.lValue[0] = lastlx = lx;
											g_UnitData.lValue[1] = lastly = ly;
											g_UnitData.lValue[2] = lastlz = lz;				
											g_UnitData.lValue[3] = lastla = la;
											g_UnitData.lValue[4] = lastlb = lb;				
											g_UnitData.lValue[5] = lastlc = lc;
											tempspeed = bspeed*speedbeilv;
									
											g_UnitData.lValue[6] = GetPulseFromMM(5,tempspeed);
											
											g_UnitData.nIO = status;
											
											for(l=0;l<MAXCYLINDER;l++)
												g_UnitData.nOut[l]=ostatus[l];
										
											g_UnitData.fDelay = time;
																		
											if(bXReset)
											{
												bXReset = FALSE;
												lastlx = lx = 0;
												g_UnitData.nReset = g_UnitData.nReset|1;
											}
											
											if(bZReset)
											{
												bZReset = FALSE;
												lastlz = lz = 0;
												g_UnitData.nReset = g_UnitData.nReset|4;
											}
											
											if(bAReset)
											{
												bAReset = FALSE;
												lastla = la = 0;
												g_UnitData.nReset = g_UnitData.nReset|8;
											}
											
											if(bBReset)
											{
												bBReset = FALSE;
												lastlb = lb = 0;
												g_UnitData.nReset = g_UnitData.nReset|16;
											}
											
											if(bCReset)
											{
												bCReset = FALSE;
												lastlc = lc = 0;
												g_UnitData.nReset = g_UnitData.nReset|32;
											}
								
											MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
											g_lXmsSize ++;
										
									
										}
										else
										{
											if(fabs( n[6]+10000.0 ) >0.001 && lc !=lastlc)
											{
																							SetUnitData(num,MOVE);
												g_UnitData.SpindleAxis = 6;							//主轴，1为X轴，2为Y，3为Z，4为A轴

												g_UnitData.lValue[0] = lastlx = lx;
												g_UnitData.lValue[1] = lastly = ly;
												g_UnitData.lValue[2] = lastlz = lz;				
												g_UnitData.lValue[3] = lastla = la;
												g_UnitData.lValue[4] = lastlb = lb;				
												g_UnitData.lValue[5] = lastlc = lc;
												tempspeed = cspeed*speedbeilv;
										
												g_UnitData.lValue[6] = GetPulseFromMM(6,tempspeed);
												
												g_UnitData.nIO = status;
												
												for(l=0;l<MAXCYLINDER;l++)
													g_UnitData.nOut[l]=ostatus[l];
											
												g_UnitData.fDelay = time;
																			
												if(bXReset)
												{
													bXReset = FALSE;
													lastlx = lx = 0;
													g_UnitData.nReset = g_UnitData.nReset|1;
												}
												
												if(bZReset)
												{
													bZReset = FALSE;
													lastlz = lz = 0;
													g_UnitData.nReset = g_UnitData.nReset|4;
												}
												
												if(bAReset)
												{
													bAReset = FALSE;
													lastla = la = 0;
													g_UnitData.nReset = g_UnitData.nReset|8;
												}
												
												if(bBReset)
												{
													bBReset = FALSE;
													lastlb = lb = 0;
													g_UnitData.nReset = g_UnitData.nReset|16;
												}
												
												if(bCReset)
												{
													bCReset = FALSE;
													lastlc = lc = 0;
													g_UnitData.nReset = g_UnitData.nReset|32;
												}
									
												MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
												g_lXmsSize ++;

											}
											else if(bXReset == TRUE || bZReset == TRUE ||  bAReset == TRUE || bBReset == TRUE ||  bCReset == TRUE || time>0.0001 || status>0 || enostatus>0)		//若X,Y,Z数据都不变，但有数据归零情况下，加一条指令 。
											{
											
											//	YlinePos[num] = ly;
												
											//	Uart_Printf("num %10ld  ,YlinePos[num] = %10ld \n",num,YlinePos[num]);
											
												SetUnitData(num,MOVE);
												g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴

												g_UnitData.lValue[0] = lastlx = lx;
												g_UnitData.lValue[1] = lastly = ly;
												g_UnitData.lValue[2] = lastlz = lz;				
												g_UnitData.lValue[3] = lastla = la;
												g_UnitData.lValue[4] = lastlb = lb;				
												g_UnitData.lValue[5] = lastlc = lc;
												tempspeed = xspeed*speedbeilv;
										
												g_UnitData.lValue[6] = GetPulseFromMM(1,tempspeed);
												
												g_UnitData.nIO = status;		
												
												for(l=0;l<MAXCYLINDER;l++)
													g_UnitData.nOut[l]=ostatus[l];
											
												g_UnitData.fDelay = time;
																	
									
												if(bXReset)
												{
													bXReset = FALSE;
													lastlx = lx = 0;
													g_UnitData.nReset = g_UnitData.nReset|1;
												}
												
												if(bZReset)
												{
													bZReset = FALSE;
													lastlz = lz = 0;
													g_UnitData.nReset = g_UnitData.nReset|4;
												}
												
												if(bAReset)
												{
													bAReset = FALSE;
													lastla = la = 0;
													g_UnitData.nReset = g_UnitData.nReset|8;
												}
												
												if(bBReset)
												{
													bBReset = FALSE;
													lastlb = lb = 0;
													g_UnitData.nReset = g_UnitData.nReset|16;
												}
												
												if(bCReset)
												{
													bCReset = FALSE;
													lastlc = lc = 0;
													g_UnitData.nReset = g_UnitData.nReset|32;
												}
									
												MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
												g_lXmsSize ++;
											
											}
										}
										
									}
								
								}
								
							}
						}
					}
					
				}
				
			}
			break;
			
			case 'L':
			{
				ThisCount[iCount]=0;									//记录循环次数，当等于设定的循环次数时，跳出当次循环
				iLoop[iCount][0]=i+1;                                   //循环开始行数
				iLoop[iCount++][1]=stringtoi((INT8U *)ptr->value[1]);   //循环次数
				fLoopAdd[1]=stringtoi((INT8U *)ptr->value[3]);//送线补偿量
				
				if( iCount > MAXLOOP )
				{
					g_bUnpackCorrect=FALSE;
					g_bDataCopy=FALSE;
					return 1;
					
				}
				
			}break;
			
			case 'N':
			{
				if( !iCount )
				{
					g_bUnpackCorrect=FALSE;
					g_bDataCopy=FALSE;
					return 2;
					
				}
				
				ThisCount[iCount-1]++;
				
				if(ThisCount[iCount-1]>=iLoop[iCount-1][1])
				{
					ThisCount[iCount-1]=0;///2019.7.10
					iCount--;	
					break;
				}
				else
				{	
				
					{													//该函数用了计数在进入加工循环前时，X,Z,A轴的位置情况，和循环中总的送线长度
																		//主要是寻找循环开始行之前的位置													
						INT16S loopno = iLoop[iCount-1][0];				
						INT16S l = 0;
						INT32S tempx=0,tempy=0,tempz = 0,tempa=0;
						INT32S tempb=0,tempc=0;
						
						tempy=0;
						
						for(l=0;l<loopno;l++)							//从第一行到循环起始行减1查找
						{
							ptr = Line_Data + l;	
							
							if(strcmp(ptr->value[0],"M") == 0)
							{
								if( ptr->value[1][0] )						//开始角有值	
								{		
								
									if(g_lXmsSize-1<g_iEmpNode[0])							//找出最小非空行的标号
									{
										g_iEmpNode[0]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
									//Uart_Printf("x1 =%s\n",ptr->value[1]);					 
									if(strcmp(ptr->value[1],"S")==0)	
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[1],"+")==0)
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[1],"-")==0)
									{
										tempx = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[1]));
										else
											tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[1])*0.1);
									}
								}
								
								if( ptr->value[2][0] )						//结束角有值
								{			
									
									if(g_lXmsSize-1<g_iEmpNode[0])							//找出最小非空行的标号
									{
										g_iEmpNode[0]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
													 
									if(strcmp(ptr->value[2],"S")==0)	
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[2],"+")==0)
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[2],"-")==0)
									{
										tempx = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[2]));
										else
											tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[2])*0.1);
									}
								}
								
								
								
								
								if( ptr->value[3][0] && g_Ini.iAxisRunMode[1] == 0)						//Y轴有值,且转芯方式为0
								{		
									if(g_lXmsSize-1<g_iEmpNode[1])							//找出最小非空行的标号
									{
										g_iEmpNode[1]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
														 
									if(strcmp(ptr->value[3],"S")==0)	
									{
										tempy = 0;
									}
									else if(strcmp(ptr->value[3],"+")==0)
									{
										tempy = 0;
					
									}
									else if(strcmp(ptr->value[3],"-")==0)
									{
										tempy = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempy= GetPulseFromAngle(2,(FP32)stringtof((INT8U *)ptr->value[3]));
										else
											tempy= GetPulseFromAngle(2,(FP32)stringtof((INT8U *)ptr->value[3])*0.1);
									}
								}
								else if(ptr->value[3][0] && g_Ini.iAxisRunMode[1] == 1 && g_Ini.iAxisMulRunMode[1] ==1)//多圈绝对模式下
								{
								
									if(g_lXmsSize-1<g_iEmpNode[1])
									{
										g_iEmpNode[1]=g_lXmsSize-1;
									}
															 
									if(strcmp(ptr->value[3],"S")==0)	
									{
										tempy = 0;
									}
									else if(strcmp(ptr->value[3],"+")==0)
									{
										tempy = 0;
					
									}
									else if(strcmp(ptr->value[3],"-")==0)
									{
										tempy = 0;
									}
									else
									{	
										INT16S 	temp;
										FP32	tempf;
										FP32	fAngle;
										FP64 	tt=stringtof((INT8U *)ptr->value[3]);
								
										temp=(INT16S)tt;						
										tempf=tt-temp;
										fAngle=temp*360+tempf*1000.0;
										tempy =  GetPulseFromAngle(2,fAngle);							//绝对位置	
									}
								
								}
								else if( ptr->value[3][0] && g_Ini.iAxisRunMode[1] == 2 && g_Ini.iAxisMulRunMode[1] ==1)		//丝杠绝对模式下
								{
									if(g_Ini.iAxisCa==0)
										tempy= GetPulseFromMM(2,(FP32)stringtof((INT8U *)ptr->value[3]));
									else
										tempy= GetPulseFromMM(2,(FP32)stringtof((INT8U *)ptr->value[3])*0.01);
								}
								
								
								if( ptr->value[4][0] && g_Ini.iAxisRunMode[2] == 0)						//转芯轴有值,且转芯方式为0
								{		
									if(g_lXmsSize-1<g_iEmpNode[2])							//找出最小非空行的标号
									{
										g_iEmpNode[2]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
														 
									if(strcmp(ptr->value[4],"S")==0)	
									{
										tempz = 0;
									}
									else if(strcmp(ptr->value[4],"+")==0)
									{
										tempz = 0;
					
									}
									else if(strcmp(ptr->value[4],"-")==0)
									{
										tempz = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempz= GetPulseFromAngle(3,(FP32)stringtof((INT8U *)ptr->value[4]));
										else
											tempz= GetPulseFromAngle(3,(FP32)stringtof((INT8U *)ptr->value[4])*0.1);
									}
								}
								else if(ptr->value[4][0] && g_Ini.iAxisRunMode[2] == 1 && g_Ini.iAxisMulRunMode[2] ==1)//多圈绝对模式下
								{
								
									if(g_lXmsSize-1<g_iEmpNode[2])
									{
										g_iEmpNode[2]=g_lXmsSize-1;
									}
															 
									if(strcmp(ptr->value[4],"S")==0)	
									{
										tempz = 0;
									}
									else if(strcmp(ptr->value[4],"+")==0)
									{
										tempz = 0;
					
									}
									else if(strcmp(ptr->value[4],"-")==0)
									{
										tempz = 0;
									}
									else
									{	
										INT16S 	temp;
										FP32	tempf;
										FP32	fAngle;
										FP64 	tt=stringtof((INT8U *)ptr->value[4]);
								
										temp=(INT16S)tt;						
										tempf=tt-temp;
										fAngle=temp*360+tempf*1000.0;
										tempz =  GetPulseFromAngle(3,fAngle);							//绝对位置	
									}
								
								}
								else if( ptr->value[4][0] && g_Ini.iAxisRunMode[2] == 2 && g_Ini.iAxisMulRunMode[2] ==1)				//丝杠绝对模式下
								{
									if(g_Ini.iAxisCa==0)
										tempz= GetPulseFromMM(3,(FP32)stringtof((INT8U *)ptr->value[4]));
									else
										tempz= GetPulseFromMM(3,(FP32)stringtof((INT8U *)ptr->value[4])*0.01);
								}
								
								
								
								if( ptr->value[5][0] && g_Ini.iAxisRunMode[3] ==0)						//卷曲轴有值,且卷曲方式为0时
								{							 
									if(g_lXmsSize-1<g_iEmpNode[3])							//找出最小非空行的标号
									{
										g_iEmpNode[3]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
									
									if(strcmp(ptr->value[5],"S")==0)	
									{
										tempa = 0;
									}
									else if(strcmp(ptr->value[5],"+")==0)
									{
										tempa = 0;
					
									}
									else if(strcmp(ptr->value[5],"-")==0)
									{
										tempa = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempa= GetPulseFromAngle(4,(FP32)stringtof((INT8U *)ptr->value[5]));
										else
											tempa= GetPulseFromAngle(4,(FP32)stringtof((INT8U *)ptr->value[5])*0.1);

									}
								}
								else if(ptr->value[5][0] && g_Ini.iAxisRunMode[3] == 1 && g_Ini.iAxisMulRunMode[3] ==1)  //弟四轴多圈绝对模式
								{
									if(g_lXmsSize-1<g_iEmpNode[3])
									{
										g_iEmpNode[3]=g_lXmsSize-1;
									}
															 
									if(strcmp(ptr->value[5],"S")==0)	
									{
										tempa = 0;
									}
									else if(strcmp(ptr->value[5],"+")==0)
									{
										tempa = 0;
					
									}
									else if(strcmp(ptr->value[5],"-")==0)
									{
										tempa = 0;
									}
									else
									{	
									
										INT16S 	temp;
										FP32	tempf;
										FP32	fAngle;
										FP64 	tt=stringtof((INT8U *)ptr->value[5]);
								
										temp=(INT16S)tt;						
										tempf=tt-temp;
										fAngle=temp*360+tempf*1000.0;
										tempa =  GetPulseFromAngle(4,fAngle);							//绝对位置	
									}
								}
								else if( ptr->value[5][0] && g_Ini.iAxisRunMode[3] == 2 && g_Ini.iAxisMulRunMode[3] ==1) //丝杠绝对
								{
								
									if(g_Ini.iAxisCa==0)
										tempa= GetPulseFromMM(4,(FP32)stringtof((INT8U *)ptr->value[5]));
									else
										tempa= GetPulseFromMM(4,(FP32)stringtof((INT8U *)ptr->value[5])*0.01);
								}
								
								if( ptr->value[6][0] && g_Ini.iAxisRunMode[4] ==0)						//卷曲2轴有值,且卷曲方式为0时
								{							 
									if(g_lXmsSize-1<g_iEmpNode[4])							//找出最小非空行的标号
									{
										g_iEmpNode[4]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
									
									if(strcmp(ptr->value[6],"S")==0)	
									{
										tempb = 0;
									}
									else if(strcmp(ptr->value[6],"+")==0)
									{
										tempb = 0;
					
									}
									else if(strcmp(ptr->value[6],"-")==0)
									{
										tempb = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempb= GetPulseFromAngle(5,(FP32)stringtof((INT8U *)ptr->value[6]));
										else
											tempb= GetPulseFromAngle(5,(FP32)stringtof((INT8U *)ptr->value[6])*0.1);
									}
								}
								else if(ptr->value[6][0] && g_Ini.iAxisRunMode[4] == 1 && g_Ini.iAxisMulRunMode[4] ==1)  //弟五轴多圈绝对模式
								{
									if(g_lXmsSize-1<g_iEmpNode[4])
									{
										g_iEmpNode[4]=g_lXmsSize-1;
									}
															 
									if(strcmp(ptr->value[6],"S")==0)	
									{
										tempb = 0;
									}
									else if(strcmp(ptr->value[6],"+")==0)
									{
										tempb = 0;
					
									}
									else if(strcmp(ptr->value[6],"-")==0)
									{
										tempb = 0;
									}
									else
									{	
									
										INT16S 	temp;
										FP32	tempf;
										FP32	fAngle;
										FP64 	tt=stringtof((INT8U *)ptr->value[6]);
								
										temp=(INT16S)tt;						
										tempf=tt-temp;
										fAngle=temp*360+tempf*1000.0;
										tempb =  GetPulseFromAngle(5,fAngle);							//绝对位置	
									}
								}
								else if( ptr->value[6][0] && g_Ini.iAxisRunMode[4] == 2)
								{
									if(g_Ini.iAxisCa==0)
										tempb= GetPulseFromMM(5,(FP32)stringtof((INT8U *)ptr->value[6]));
									else
										tempb= GetPulseFromMM(5,(FP32)stringtof((INT8U *)ptr->value[6])*0.01);
								}
								
								if( ptr->value[7][0] && g_Ini.iAxisRunMode[5] ==0)						//夹耳轴有值,且卷曲方式为0时
								{							 
									if(g_lXmsSize-1<g_iEmpNode[5])							//找出最小非空行的标号
									{
										g_iEmpNode[5]=g_lXmsSize-1;
										
										//Uart_Printf("g_iEmpNode[%d]=%d\n",k,g_iEmpNode[k]);
									}
									
									if(strcmp(ptr->value[7],"S")==0)	
									{
										tempc = 0;
									}
									else if(strcmp(ptr->value[7],"+")==0)
									{
										tempc = 0;
					
									}
									else if(strcmp(ptr->value[7],"-")==0)
									{
										tempc = 0;
									}
									else
									{
										if(g_Ini.iAxisCa==0)
											tempc= GetPulseFromAngle(6,(FP32)stringtof((INT8U *)ptr->value[7]));
										else
											tempc= GetPulseFromAngle(6,(FP32)stringtof((INT8U *)ptr->value[7])*0.1);

									}
								}
								else if(ptr->value[7][0] && g_Ini.iAxisRunMode[5] == 1 && g_Ini.iAxisMulRunMode[5] ==1)  //弟六轴多圈绝对模式
								{
									if(g_lXmsSize-1<g_iEmpNode[5])
									{
										g_iEmpNode[5]=g_lXmsSize-1;
									}
															 
									if(strcmp(ptr->value[7],"S")==0)	
									{
										tempc = 0;
									}
									else if(strcmp(ptr->value[7],"+")==0)
									{
										tempc = 0;
					
									}
									else if(strcmp(ptr->value[7],"-")==0)
									{
										tempc = 0;
									}
									else
									{	
									
										INT16S 	temp;
										FP32	tempf;
										FP32	fAngle;
										FP64 	tt=stringtof((INT8U *)ptr->value[7]);
								
										temp=(INT16S)tt;						
										tempf=tt-temp;
										fAngle=temp*360+tempf*1000.0;
										tempc =  GetPulseFromAngle(6,fAngle);							//绝对位置	
									}
								}
								else if( ptr->value[7][0] && g_Ini.iAxisRunMode[5] == 2)
								{
								
									if(g_Ini.iAxisCa==0)
										tempc= GetPulseFromMM(6,(FP32)stringtof((INT8U *)ptr->value[7]));
									else
										tempc= GetPulseFromMM(6,(FP32)stringtof((INT8U *)ptr->value[7])*0.01);
									
								}
							}
							
						} 
						
						//Uart_Printf("x  %ld  %ld\n",tempx,tempz);
						
						for(l=loopno;l<=i;l++)					//从循环开始行到结束行查找
						{
							ptr = Line_Data + l;		
						
							if(l==loopno && ptr->value[1][0] )	//如果循环起始行的开始角有值	
							{							 
								if(strcmp(ptr->value[1],"S")==0)	
								{
									tempx = 0;
								}
								else if(strcmp(ptr->value[1],"+")==0)
								{
									tempx = 0;
								}
								else if(strcmp(ptr->value[1],"-")==0)
								{
									tempx = 0;
								}
								else
								{
									if(g_Ini.iAxisCa==0)
										tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[1]));
									else
										tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[1])*0.1);
								}
							}
							
						}
						
						lx= tempx;
						
					//	ly = GetPulseFromMM(2,y);							//Y轴改成绝对位置
						
						if(g_Ini.iAxisRunMode[1] == 0 || ((g_Ini.iAxisRunMode[1] == 2 || g_Ini.iAxisRunMode[1] == 1) && g_Ini.iAxisMulRunMode[1] ==1))
							ly= tempy;					//绝对
						else 
							ly = ly;					//相对

						if(g_Ini.iAxisRunMode[2] == 0 || ((g_Ini.iAxisRunMode[2] == 2 || g_Ini.iAxisRunMode[2] == 1) && g_Ini.iAxisMulRunMode[2] ==1))
							lz= tempz;					//绝对
						else 
							lz = lz;					//相对	
						
						if(g_Ini.iAxisRunMode[3] ==0 || ((g_Ini.iAxisRunMode[3] == 2 || g_Ini.iAxisRunMode[3] == 1) && g_Ini.iAxisMulRunMode[3] ==1))
							la= tempa;					//绝对
						else 
							la = la;					//相对	
						
						if(g_Ini.iAxisRunMode[4] ==0 || g_Ini.iAxisRunMode[4] == 2 || (g_Ini.iAxisRunMode[4] == 1 && g_Ini.iAxisMulRunMode[4] ==1))
						{
							lb= tempb;
						}
						else if(g_Ini.iAxisRunMode[4] ==1)
						{
							lb = lb;
						}
						
						if(g_Ini.iAxisRunMode[5] ==0 || g_Ini.iAxisRunMode[5] == 2 || (g_Ini.iAxisRunMode[5] == 1 && g_Ini.iAxisMulRunMode[5] ==1))
						{
							lc= tempc;
						}
						else if(g_Ini.iAxisRunMode[5] ==1)
						{
							lc = lc;
						}
						
						
						{													//开关轴的处理													
							if(g_Ini.iAxisSwitch[1]==0)
								ly = 0;					
							
							if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
								lz = 0;
								
							if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
								la = 0;
								
							if(g_Ini.iAxisCount<5 || g_Ini.iAxisSwitch[4]==0)
								lb = 0;
								
							if(g_Ini.iAxisCount<6 || g_Ini.iAxisSwitch[5]==0)
								lc = 0;
						}
						
						
						SetUnitData(num,MOVE);

						if(lx !=lastlx)
						{
							g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lxspeed;

						}
						else if(ly !=lastly)
						{
							g_UnitData.SpindleAxis = 2;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lyspeed;

						}
						else if(lz != lastlz)
						{
							g_UnitData.SpindleAxis = 3;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lzspeed;
						}
						else if(la !=lastla)
						{
							g_UnitData.SpindleAxis = 4;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = laspeed;
						}
						else if(lb !=lastlb)
						{
							g_UnitData.SpindleAxis = 5;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lbspeed;
						}
						else if(lc !=lastlc)
						{
							g_UnitData.SpindleAxis = 6;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lcspeed;
						}
						else
						{
							g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lxspeed;
						}

						g_UnitData.lValue[0] = lastlx = lx;
						g_UnitData.lValue[1] = lastly = ly;
						g_UnitData.lValue[2] = lastlz = lz;				
						g_UnitData.lValue[3] = lastla = la;
						g_UnitData.lValue[4] = lastlb = lb;				
						g_UnitData.lValue[5] = lastlc = lc;
						
						MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
						g_lXmsSize ++;
						
					}
					
					i=iLoop[iCount-1][0];	
					
					nLoopNo++;					
					
					goto BEGIN_ACCESS;
				}
				
			}break;
			
			case 'J':
			{
				jumpLine=stringtoi((INT8U *)ptr->value[1]);
				
				if(jumpLine-1<=i || jumpLine>=Line_Data_Size)
				{					
					g_bUnpackCorrect=FALSE;
					g_bDataCopy=FALSE;
					return 3;
				}
				
				SetUnitData(num,JUMP);
				g_UnitData.lValue[0]=jumpLine;
				g_UnitData.lValue[1]=ly;
				
				
				MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
				g_lXmsSize ++;
				
			}break;
			
			case 'G':
			{
				gotoLine=stringtoi((INT8U *)ptr->value[1]);
				
				if(gotoLine-1<=i || gotoLine>=Line_Data_Size)
				{
					g_bUnpackCorrect=FALSE;
					g_bDataCopy=FALSE;
				//	Uart_Printf("gotoLine = %d ,i=%d ,Line_Data_Size = %d \n",gotoLine,i,Line_Data_Size);
					return 4;
				}

				//SetUnitData(num,GOTO);
				//g_UnitData.lValue[0]=gotoLine;
				
				//MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
				//g_lXmsSize ++;
				
				i = gotoLine-1;
				
				goto BEGIN_ACCESS;
				
			}break;
			
			case 'E':
			{
			
							
				{
					{
						INT32S  k,pulse;
						
						SetUnitData(num,MOVE);
						
						g_UnitData.nReset=0x3f;		//先设置回零表示，根据回零模式再进行相应的设置	
					
						
						if(g_Ini.iBackMode[0]==4)										//不回零模式
						{
							g_UnitData.nReset&=~(0x01<<0);
						}
						else
						{
							if(lx%g_Ini.lPulse[0]!=0)
							{
								if(g_Ini.iBackMode[0]==0)											//S
								{
									if(fabs(lx)<g_Ini.lPulse[0]*0.5)
									{
										lx  = 0;
									}
									else
									{
										if(lx>0)
											lx  = g_Ini.lPulse[0];
										else
											lx  = -g_Ini.lPulse[0];
									}
								}
								else if(g_Ini.iBackMode[0]==1)//+
								{
									if(lx>0)
										lx = g_Ini.lPulse[0];
									else
										lx = 0;
								}
								else if(g_Ini.iBackMode[0]==2)//-
								{
									if(lx>0)
										lx = 0;
									else
										lx = -g_Ini.lPulse[0];
								}
								else if(g_Ini.iBackMode[0]==3)										//零向回零
								{
									lx  = 0;
								}
							}
						}
						
						for(k=1; k<MAXCH;k++)
						{
																		
							switch(k)
							{
							
							case 1: pulse = ly;break;
							case 2: pulse = lz;break;
							case 3: pulse = la;break;
							case 4: pulse = lb;break;
							case 5: pulse = lc;break;
							}
						
							//if((g_Ini.iAxisCount>2)&&(g_Ini.iAxisSwitch[2]==1))
							{
								if(g_Ini.iAxisRunMode[k] == 3)
								{
									// pulse 默认为终点的值;
								}
								else if(g_Ini.iAxisRunMode[k] != 2 )
								{
									if(g_Ini.iBackMode[k]==4 && g_Ini.iAxisRunMode[k]==0)										//不回零模式
									{
										g_UnitData.nReset&=~(0x01<<k);
									}
									else if(pulse%g_Ini.lPulse[k]!=0)  
									{
										INT32S tmplz =0;
										INT32S tmpts = 0;
									
										tmplz = pulse%g_Ini.lPulse[k];								//求余数
										tmpts = pulse/g_Ini.lPulse[k];								//求圈
										
										if(g_Ini.iBackMode[k]==1)									//+
										{
											if(tmplz>0)
												pulse = (tmpts+1)*g_Ini.lPulse[k];
											else
												pulse = tmpts*g_Ini.lPulse[k];		
										}
										else if(g_Ini.iBackMode[k]==2)								//-
										{
											if(tmplz<0)
												pulse  = (tmpts-1)*g_Ini.lPulse[k];
											else
												pulse  = tmpts*g_Ini.lPulse[k];	
										}
										else if(g_Ini.iBackMode[k]==3)								//零向回零
										{
											pulse  = tmpts*g_Ini.lPulse[k];
										}
										else //if(g_Ini.iBackMode[k]==0)							//S 就近回零以及默认行为(多圈和单圈)
										{
											if(labs(tmplz)<g_Ini.lPulse[k]*0.5)
											{
												pulse  = tmpts*g_Ini.lPulse[k];
											}
											else
											{
												if(pulse>0)
													pulse  = (tmpts+1)*g_Ini.lPulse[k];
												else
													pulse  = (tmpts-1)*g_Ini.lPulse[k];
											}
										}
									}
								}
								else if(g_Ini.iAxisRunMode[k] == 2)
								{
									if(g_Ini.iBackMode[k]==4)										//不回零模式
									{
										g_UnitData.nReset&=~(0x01<<k);
									}
									else 
										pulse = 0;
								}
		
							
								
								
							}
							
							switch(k)
							{
							
							case 1: ly = pulse;break;
							case 2: lz = pulse;break;
							case 3: la = pulse;break;
							case 4: lb = pulse;break;
							case 5: lc = pulse;break;
							}
						
						}

	

						
						//SetUnitData(num,MOVE);
						
						if(lx !=lastlx)
						{
							g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lxspeed;

						}
						else if(ly !=lastly)
						{
							g_UnitData.SpindleAxis = 2;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lyspeed;
						}
						else if(lz != lastlz)
						{
							g_UnitData.SpindleAxis = 3;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lzspeed;
						}
						else if(la != lastla)
						{
							g_UnitData.SpindleAxis = 4;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = laspeed;
						}
						else if(lb != lastlb)
						{
							g_UnitData.SpindleAxis = 5;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lbspeed;
						}
						else if(lc != lastlc)
						{
							g_UnitData.SpindleAxis = 6;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lcspeed;
						}
						else
						{
							g_UnitData.SpindleAxis = 1;					//主轴，1为X轴，2为Y，3为Z，4为A轴
							g_UnitData.lValue[6] = lxspeed;
						}
						
						g_UnitData.lValue[0] = lastlx = lx;
						g_UnitData.lValue[1] = lastly = ly;
						g_UnitData.lValue[2] = lastlz = lz;				
						g_UnitData.lValue[3] = lastla = la;
						g_UnitData.lValue[4] = lastlb = lb;				
						g_UnitData.lValue[5] = lastlc = lc;
						
						//g_UnitData.nReset = 63;							//回完零后四个轴一起设零   1111
						
						lastlx = lx = 0;
						lastly = ly = 0;
						lastlz = lz = 0;
						lastla = la = 0;
						lastlb = lb = 0;
						lastlc = lc = 0;

						MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
						g_lXmsSize ++;
					}
					
					flag=TRUE;
					
					SetUnitData(num,PEND);
					MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
					g_lXmsSize ++;
				}
			
			}//end case E;
			break;
		
		}//end  switch(ptr->value[0][0])
		
		i += 1;
		
		if(g_lXmsSize>(DATASIZE-1)/sizeof(UNITDATA))		//检查内存进否溢出
		{
			g_bUnpackCorrect=FALSE;
			g_bDataCopy=FALSE;
			return 5;
			
		}
		
		if(flag ==TRUE)										//得到E指令退出循环加工
			break; 
	
	}//end  for(i=0; i<Line_Data_Size; )
	

	if( g_lXmsSize< 2)			//1为只有速度行
	{
		g_bUnpackCorrect=FALSE;
		g_bDataCopy=FALSE;
		return 100;		
	}
	
	
	{															//检查数据的正确性。
		INT8S 		CheckStatusIO=0;
		BOOL 		WorkEnd=FALSE;
		UNITDATA 	pData;
		INT32S 		iNode=0;
		INT32S 		t1,t2;

		//INT32S 		lPValue[6]={0};
		//FP32 		fPDly=0.0;

		t1=OSTimeGet();
		
	//	Uart_Printf("\npData.nLine,pData.nCode,pData.SpindleAxis,pData.lValue[0],pData.lValue[1],pData.lValue[2],pData.lValue[3],pData.lValue[4],pData.nIO,pData.nOut,pData.nOutAxis,pData.lOutValue[0],pData.lOutValue[1],pData.nReset\n");
		
		
		for( ; iNode<g_lXmsSize; )
		{
			t2=OSTimeGet();
			
		
			if((t2-t1)>350 || (t2-t1)<-350)
			{
				g_bUnpackCorrect=FALSE;
				g_bDataCopy=FALSE;
				return 99;
			}
		
			
			XMSToMEMEx((INT8U *)&pData,g_uData,iNode*sizeof(UNITDATA),sizeof(UNITDATA));
			iNode ++;
			
			/*
			{
				Uart_Printf("\n%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10lf,%10ld,%10lf,%10ld\n"
							,pData.nLine,pData.nCode,pData.SpindleAxis,pData.lValue[0],pData.lValue[1],pData.lValue[2],
							pData.lValue[3],pData.lValue[4],pData.lValue[5],pData.lValue[6]
							,pData.nIO,pData.fDelay,pData.nReset);
							
			}*/
			
			/*
			Uart_Printf("\n%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,",
						pData.nLine,pData.nOut[0],pData.nOut[1],pData.nOut[2],pData.nOut[3],pData.nOut[4],pData.nOut[5],pData.nOut[6],pData.nOut[7]);
			*/
			
			switch( pData.nCode )
			{
			case SPEED:
				XMSToMEMEx((INT8U *)&pData,g_uData,iNode*sizeof(UNITDATA),sizeof(UNITDATA));
				iNode ++;
				
				if(pData.nCode!=JUMP)
					iNode--;
				continue;
				
			case MOVE:
				
				
				{
					CheckStatusIO = pData.nIO;
					
					if(CheckStatusIO && pData.nReset!=0)
					{
						g_bUnpackCorrect=FALSE;
						g_bDataCopy=FALSE;
						return 102;	
					}
				}
				
			//	Uart_Printf("N%dY=%d\n,",iNode,pData.lValue[1]);
				
				if(CheckStatusIO)
				{
					INT16S curline=pData.nLine;
					
					XMSToMEMEx((INT8U *)&pData,g_uData,iNode*sizeof(UNITDATA),sizeof(UNITDATA));
					iNode ++;
					

					
					if(pData.nCode!=JUMP)
						iNode--;
					else
					{
						INT16S temp=((INT16S)pData.lValue[0]-1)*2;
						INT16S jNo=(INT16S)pData.lValue[0];
						
						if(temp<=curline+2)
						{
							g_bUnpackCorrect=FALSE;
							g_bDataCopy=FALSE;
							return 6;							
						}
						
						iNode=0;
						
						while(TRUE)
						{
							XMSToMEMEx((INT8U *)&pData,g_uData,iNode*sizeof(UNITDATA),sizeof(UNITDATA));
							iNode ++; 
							
							if(iNode>g_lXmsSize)
							{
								g_bUnpackCorrect=FALSE;
								g_bDataCopy=FALSE;
								return 7;								
							}
							
							if((pData.nLine+1)==temp || pData.nLine==temp)
							{
								iNode--;
								
								ptr = Line_Data + jNo-1;
								
								//判断跳转行上是否有数据
								if(ptr->value[2][0]==NULL			//X结束角	
								 	||(g_Ini.iAxisSwitch[2]==1 && g_Ini.iAxisCount>2?ptr->value[4][0]==NULL:FALSE)	//Z轴
									||(g_Ini.iAxisSwitch[3]==1 && g_Ini.iAxisCount>3?ptr->value[5][0]==NULL:FALSE)	//A轴
									||(g_Ini.iAxisSwitch[4]==1 && g_Ini.iAxisCount>4?ptr->value[6][0]==NULL:FALSE)	//B轴
									||(g_Ini.iAxisSwitch[5]==1 && g_Ini.iAxisCount>5?ptr->value[7][0]==NULL:FALSE))	//C轴
								{
									return 12;
								}
								
								break;
							}
						}
					}
				}
				break;
			case JUMP:
				continue;
			
			case GOTO:
				{
					INT16S temp=((INT16S)pData.lValue[0]-1)*2-1;
					iNode=0;
					
					while(TRUE)
					{
						XMSToMEMEx((INT8U *)&pData,g_uData,iNode*sizeof(UNITDATA),sizeof(UNITDATA));
						iNode ++; 
						
						//Uart_Printf("@@@=%d ",pData.nLine);
						if(iNode>g_lXmsSize)
						{
						//	Uart_Printf("1=%d 2=%d 3=%d 4=%d\n",iNode,g_lXmsSize,pData.nLine,temp);
							g_bUnpackCorrect=FALSE;
							g_bDataCopy=FALSE;
							return 8;						
						}
						
						//if(pData.nCode == GOTO)
						if(pData.nLine==temp || pData.nLine==temp+1)
						{
							if(pData.nCode!=MOVE && pData.nCode!=SPEED && pData.nCode!=PEND)
							{
								g_bUnpackCorrect=FALSE;
								g_bDataCopy=FALSE;
								return 9;
							}
							iNode--;
							break;
						}
					}
				}
				break;
				
			case PEND:
				WorkEnd=TRUE;
				break;
				
			default:
				break;
			}
			
			if(WorkEnd)
			{
				flag=TRUE;
				break;
			}
		}
		
		if(!WorkEnd)
		{
			flag=FALSE;
		}
	}
	
	g_bDataCopy=FALSE;
	
	if(flag)
	{
		g_bUnpackCorrect=TRUE;
		g_bModify = FALSE;
		
	}
	else
		g_bUnpackCorrect=FALSE;
	//Uart_Printf("g_bDataCopy=%ld\n",g_bDataCopy?1:0);
	return flag?0:101;


}


void	UnpackErrDisp(INT16S no)
{
	switch(no)
	{
		case 1:
			OSQPost(MsgQueue,g_bEnglish?"Loop Count Invalid!":"循环嵌套太多!");
			break;
			
		case 2:
			OSQPost(MsgQueue,g_bEnglish?"Loop End Invalid!":"循环结束无效!");
			break;
			
		case 3:
		case 7:
			OSQPost(MsgQueue,g_bEnglish?"Command J Error,can't Run to End!":"J跳转错误,请跳转执行到E指令上!");
			break;
			
		case 6:
			OSQPost(MsgQueue,g_bEnglish?"Command J Error,can't Jump to this Line!":"J跳转位置错误,不能跳转到该行之上!");
			break;
			
		case 12:
			OSQPost(MsgQueue,g_bEnglish?"Invalid Data at Line which J Jump to!":"请确保J跳转行上每个轴(Y轴除外)都有数据!");
			break;
			
		case 4:
		case 8:
			OSQPost(MsgQueue,g_bEnglish?"Command G Error,can't Run to End!":"G跳转错误,请跳转执行到E指令上!");
			break;
			
		case 9:
			OSQPost(MsgQueue,g_bEnglish?"Command G Error,Make sure G to Command M Line!":"G跳转错误,请证跳转到M或S指令上!");
			break;
			
		case 10:
			OSQPost(MsgQueue,axisMsg(3,g_bEnglish?" Data Format Error!Circles:-100~100":"轴数据错误,圈数:-100~100!",TRUE));
			break;
			
		case 11:
			OSQPost(MsgQueue,axisMsg(4,g_bEnglish?" Data Format Error!Circles:-100~100":"轴数据错误,圈数:-100~100!",TRUE));
			break;
	
		case 5:
			OSQPost(MsgQueue,g_bEnglish?"Loop too much Times!":"循环次数太多,内存不足!");
			break;
			
		case 99:
			OSQPost(MsgQueue,g_bEnglish?"Data Format Error!Flow sink into Dead Loop!":"跳转位置错误,跳转程序会无限循环!");
			break;
			
		case 100:
			OSQPost(MsgQueue,g_bEnglish?"Empty Files!":"文件内容为空!");
			break;
			
		case 101:
			OSQPost(MsgQueue,g_bEnglish?"Lack of E code!":"缺少E指令程序不能执行到E指令位置!");
			break;
			
		case 102:
			OSQPost(MsgQueue,g_bEnglish?"Probe Row can't Exist ZeroMode!":"探针行不可存在回零模式!");
			break;

		case 103:
			OSQPost(MsgQueue,g_bEnglish?" ":"延时后有回零模式冲突或在结束位置存在延时!");
			break;
			
		default:
			OSQPost(MsgQueue,g_bEnglish?"Data Error!":"程序内容错误!");
			break;
	}
	
	if(g_lXmsSize<2)
		OSQPost(MsgQueue,g_bEnglish?"file empty":"文件内容为空!");
}


BOOL	DataCopy(void)
{
	#if OS_CRITICAL_METHOD == 3                     						 	/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  							/* Prevent compiler warning                           */
	#endif 
	
	if(g_bUnpackCorrect)
	{	
		OS_ENTER_CRITICAL();	
		memcpy((INT8U *)g_ucXMSArray,(INT8U *)g_ucDataArray,g_lXmsSize*sizeof(UNITDATA));
		g_bDataCopy=TRUE;
		g_lDataCopyXmsSize=g_lXmsSize;
		OS_EXIT_CRITICAL();
		return TRUE;
	}
	else
	{
		g_bDataCopy=FALSE;
		return FALSE;
	}
}


void	SuspendTasks(INT8S no)
{
	switch(no)
	{
		case 0:	
			g_uiSuspendTask |= 0xFF01;
			break;
			
		case 1:	
			g_uiSuspendTask |= 0xFF02;
			break;
			
		case 2: 
			g_uiSuspendTask |= 0xFF04;
			break;
	
	//	case 3:
			
		case 4: 
			g_uiSuspendTask |= 0xFF10;
			break;
			
		case 5:	
			g_uiSuspendTask |= 0xFF20;
			break;
			
		case 6:	
			g_uiSuspendTask |= 0xFF40;
			break;
			
		case 7:	
			g_uiSuspendTask |= 0xFF80;
			break;
			
		default:
			break;
	}
}


void	ResumeTasks(INT8S no)
{
	switch(no)
	{
		case 0:	
			g_uiSuspendTask &= 0xFFFE;
			OSTaskResume(CHECK_PROBE_TASK_PRIO);
			break;
			
		case 1:	
			g_uiSuspendTask &= 0xFFFD;
			OSTaskResume(RUN_CHECKIO_TASK_PRIO);
			break;
			
		case 2: 
			g_uiSuspendTask &= 0xFFFB;
			OSTaskResume(DISPLAY_RUNSPEEDCOUNT_TASK_PRIO);
			break;
		
	//	case 3:
			
		case 4:	
			g_uiSuspendTask &= 0xFFEF;
			OSTaskResume(DISPLAY_RUNLINE_TASK_PRIO);
			break;
			
		case 5:	
			g_uiSuspendTask &= 0xFFDF;
			OSTaskResume(DISPLAY_XYZ_TASK_PRIO);
			break;
			
		case 6:	
			g_uiSuspendTask &= 0xFFBF;
			OSTaskResume(DISPLAY_MSG_TASK_PRIO);
			break;
			
		case 7:	
			g_uiSuspendTask &= 0xFF7F;
			OSTaskResume(DISPLAY_TIME_TASK_PRIO);
			break;
			
		default:
			break;
	}
}


INT32S	SpeedMax(INT32S speed,INT32S p1,INT32S p2,INT32S p3,INT32S p4)
{

	INT32S 	tempspeed,maxpulse;
	INT32S	tempspeedpulse=speed;
	
	p1 = labs(p1);
	p2 = labs(p2);
	p3 = labs(p3);
	p4 = labs(p4);
	
	tempspeed = p1>p2?p1:p2;
	tempspeed = tempspeed>p3?tempspeed:p3;
	tempspeed = tempspeed>p4?tempspeed:p4;
	
	if(p1 != 0)
	{
		if(tempspeed == p1)
		{
			maxpulse = (INT32S)((FP32)g_Ini.lPulse[0]/g_Ini.lGearRate[0]*g_Ini.lMaxSpeed[0]/60.0);
			
			if(tempspeedpulse>maxpulse)
				tempspeedpulse=maxpulse;
			
		}
		else
		{
			if(tempspeed == p2)
			{
				tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
				maxpulse = (INT32S)((FP32)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
				
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
				
			}
			else
			{
				if(tempspeed == p3)
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}
	}
	else
	{
		if(p2 != 0)
		{
			if(tempspeed == p2)
			{
				maxpulse = (INT32S)((FP32)g_Ini.lPulse[1]/g_Ini.lGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);
				
				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;
				
			}
			else
			{
				if(tempspeed == p3)
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p2*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p2*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
			
		}
		else
		{
			if(p3!=0)
			{
				if(tempspeed == p3)
				{
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.lGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p3*(FP32)tempspeedpulse;
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.lGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);
					
					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}
	}
	
	return tempspeedpulse;

}

