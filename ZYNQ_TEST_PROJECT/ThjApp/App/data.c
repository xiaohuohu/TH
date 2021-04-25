# include   "includes.h"
# include	"xms.h"

TEST_EXT	OS_EVENT	*FileSem;												//文件操作信号量,防止同时进行磁盘IO
TEST_EXT    OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bRam;													//指示是否含有带电RAM或电池是否充足
TEST_EXT	BOOL		g_bTH836;
WORK_EXT	BOOL		g_bLCD;
//WORK_EXT 	INT32S  	g_lRamRunCounts;                                        //该参数在后面保存到铁电或yaffs中未使用到20140807

#define 	SRAMADDR	(volatile unsigned char *)(0x0e000008)					//将参数和数据文件保存到铁电中
#define 	DATAADDR	350
#define 	PASSDDR    	20

BASEINI		g_Ini;
COMMAND		Line_Data[MAXLINE];													//不用直接声明数组的形式可以防止全局变量声明过多导致超过64K的限制
INT16S		Line_Data_Size=0;													//指示当前数据行数
UNITDATA    g_UnitData;

const 		INT16U	 	g_uData=1;	   											//指向加工数据解码后的XMS句柄
const 		INT16U	 	g_uXMS=2;	   											//指向加工数据解码后的XMS句柄
INT16U					g_uVideoXMS=0;
extern 		INT8U		g_ucDataArray[DATASIZE];
extern 		INT8U		g_ucXMSArray[XMSSIZE];
INT8S 		filebuf[30000];														//必须定义为全局变量
extern		INT16S		g_iCurEditNo;
extern		INT32S 		g_iBufSize;

#define  	PRG_PTH     "\\PROG"
#define  	PAR_PTH     "\\PARA"


#define CHECK_DATA(value,min,max) \
		if(g_Ini.value-max>0)g_Ini.value=max;\
		if(g_Ini.value-min<0)g_Ini.value=min;


void File_Init(void)  															//20070123 modify add
{
	char    current_path[30];
	FRESULT res;
	DIR dp;
	DIR dp1;

	strcpy(current_path,"0:\\PROG");
	res = f_opendir(&dp,current_path);

	if(res !=FR_OK)
	{
		res = f_mkdir(PRG_PTH);        /* Create a sub directory */
	}

	f_closedir (&dp);          /* Close an open directory */

	memset(current_path,NULL,sizeof(current_path));
	strcpy(current_path,"0:\\PARA");
	res = f_opendir(&dp,current_path);

	if(res !=FR_OK)
	{
		res = f_mkdir(current_path);        /* Create a sub directory */
	}

	f_closedir (&dp);
}


void	DataCls(INT16S i)
{
	INT8S *ptr = (INT8S *)&Line_Data[i];

	memset(ptr,NULL,MAXVALUECOL*9);
}


void    DataPut(INT16S i,INT16S j,INT8S *str)
{
	strcpy(Line_Data[i].value[j],str);
}



void	WriteDataToFile( INT16S File)
{

	FIL 		fd;
	FRESULT		res;
	UINT		bw;

	INT8S 		tmpbuf[0x20]="0:\\PROG\\"; 										//20070123 modify add
	INT8S 		buffer[0x10]="\0";
	INT8S 		filemessage[0x40]= "";
	INT8S  		ti[10],da[10];
	INT32S 		ibufcount = 0;

	RtcGetDateStr(da);
	RtcGetTimeStr(ti);

	if( !Line_Data_Size )
	{
		//	OSQPost(MsgQueue,"Data is empty!");
		//Uart_Printf("Data is empty!Press Any Key...\n");
		return;
	}

	strcpy(buffer,"THJ000.DAT");
	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	strcat(tmpbuf,buffer);

	res = f_open(&fd,tmpbuf,FA_CREATE_ALWAYS|FA_WRITE);

	if(res != FR_OK)
	{
		f_close(&fd) ;
		return;
	}

	strcat(filemessage,da);
	strcat(filemessage," ");
	strcat(filemessage,ti);

	memset(filebuf,NULL,sizeof(filebuf));												//绝对不可丢掉

	memcpy(filebuf+ibufcount,filemessage,0x40);

	ibufcount += 0x40;

	memcpy(filebuf+ibufcount,&Line_Data_Size,sizeof(Line_Data_Size));
	ibufcount += sizeof(Line_Data_Size);


	memcpy(filebuf+ibufcount,Line_Data,Line_Data_Size*sizeof(COMMAND));
	ibufcount += Line_Data_Size*sizeof(COMMAND);

	CHECK_DATA(lRunCount,0,		9999999L)
	CHECK_DATA(lDestCount,0,	9999999L)
	CHECK_DATA(lProbeCount,0,	99999L)

	{
		INT32S tmp[10];

		tmp[0] = g_Ini.lDestCount;
		tmp[1] = g_Ini.lRunCount;
		tmp[2] = g_Ini.lProbeCount;
		//	tmp[3] = g_Ini.lRunSpeed[0];
		//	tmp[4] = g_Ini.lRunSpeed[1];
		//	tmp[5] = g_Ini.lRunSpeed[2];
		//	tmp[6] = g_Ini.lRunSpeed[3];
		//	tmp[7] = g_Ini.lStartOffset[0];
		//	tmp[8] = g_Ini.lStartOffset[2];
		//	tmp[9] = g_Ini.lStartOffset[3];

		memcpy(filebuf+ibufcount,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);
	}

	res = f_lseek(&fd,0);
	res = f_write(&fd,filebuf,ibufcount,&bw);

	f_close(&fd) ;
}






BOOL	ReadDataFromFile(INT16S File )
{

	FIL 		fd;
	FRESULT		res;
	UINT		br;

	INT16S 		i=0,size=0;
	INT8S 		buffer[0x10]="THJ000.DAT";
	INT8S 		tmpbuf[0x20]="0:\\PROG\\";
	INT8S 		filemessage[0x40]="\0";
	INT32S 		ibufcount = 0;

	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	for(i=0; i<MAXLINE; i++)
		DataCls(i);

	Line_Data_Size = 0;

	strcat(tmpbuf,buffer); 

	res= f_open (&fd, tmpbuf, FA_OPEN_EXISTING|FA_READ);

	if(res != FR_OK)
	{
		f_close(&fd) ;
		OSQPost(MsgQueue,"文件打开错误，或文件不存在");
		Uart_Printf("File Open Err!Press Any Key...\n");
		return FALSE;
	}

	ibufcount = 0;
	res=f_lseek(&fd,ibufcount);
	res=f_read(&fd,&filemessage,0x40,&br);
	ibufcount+=0x40;


	res = f_lseek(&fd,ibufcount);
	res = f_read(&fd,&size,sizeof(size),&br);
	ibufcount += sizeof(size);

	if( size < 1 || size > MAXLINE )
	{
		f_close(&fd) ;
		return FALSE;
	}

	Line_Data_Size = size;

	{

		res = f_lseek(&fd,ibufcount);
		res = f_read(&fd,Line_Data,Line_Data_Size*sizeof(COMMAND),&br);
		ibufcount += Line_Data_Size*sizeof(COMMAND);
	}

	{
		INT32S tmp[10];
		//	INT32S temp[3];

		res = f_lseek(&fd,ibufcount);
		res = f_read(&fd,tmp,sizeof(tmp),&br);
		ibufcount += sizeof(tmp);


		g_Ini.lDestCount= tmp[0];
		g_Ini.lRunCount = tmp[1];
		g_Ini.lProbeCount= tmp[2];

		/*
		g_Ini.lRunSpeed[0] = tmp[3];
		g_Ini.lRunSpeed[1] = tmp[4];
		g_Ini.lRunSpeed[2] = tmp[5];
		g_Ini.lRunSpeed[3] = tmp[6];

		temp[0] = tmp[7];
		temp[1] = tmp[8];
		temp[2] = tmp[9];


		if(temp[0]!=g_Ini.lStartOffset[0])
		{
			g_bFirstResetX = FALSE;
			g_Ini.lStartOffset[0] = temp[0];
		}

		if(temp[1]!=g_Ini.lStartOffset[2])
		{
			g_bFirstResetZ = FALSE;
			g_Ini.lStartOffset[2] = temp[1];
		}

		if(temp[2]!=g_Ini.lStartOffset[3])
		{
			g_bFirstResetU = FALSE;
			g_Ini.lStartOffset[3] = temp[2];
		}

		 */
	}


	f_close(&fd) ;

	return TRUE;
}


void	SysParaInit()
{
	INT16S i=0;

	g_Ini.lPulse[0]=15000;
	g_Ini.lPulse[1]=12500;
	g_Ini.lPulse[2]=36000;
	g_Ini.lPulse[3]=36000;

	g_Ini.fMM[0]=360;
	g_Ini.fMM[1]=157;
	g_Ini.fMM[2]=360;
	g_Ini.fMM[3]=360;

	g_Ini.iRunDir[0]=0;
	g_Ini.iRunDir[1]=0;
	g_Ini.iRunDir[2]=0;
	g_Ini.iRunDir[3]=0;

	g_Ini.fGearRate[0]=3;
	g_Ini.fGearRate[1]=2.5;
	g_Ini.fGearRate[2]=1;
	g_Ini.fGearRate[3]=1;

	/*for(i=0;i<MAXCH;i++)
	{
		g_Ini.lMaxSpeed[i]=2500;
		g_Ini.fHandSpeed[i]=90;
		g_Ini.fBackSpeed[i]=90;
		g_Ini.iBackDir[i]=0;
		g_Ini.iAxisSwitch[i]=1;
		g_Ini.fStartOffset[i]=0;
		g_Ini.iBackMode[i]=1;													//默认正向回零
		g_Ini.iHardLimitSwitch[i]=0;
		g_Ini.iZeroReplaceLimit[i]=0;		
		g_Ini.lChAddSpeed[i]=10000;
	}*/

	g_Ini.lMaxSpeed[0]=2500;
	g_Ini.lMaxSpeed[1]=2500;
	g_Ini.lMaxSpeed[2]=2500;
	g_Ini.lMaxSpeed[3]=2500;
	g_Ini.fHandSpeed[0]=90;
	g_Ini.fHandSpeed[1]=90;
	g_Ini.fHandSpeed[2]=90;
	g_Ini.fHandSpeed[3]=90;
	g_Ini.fBackSpeed[0]=90;
	g_Ini.fBackSpeed[1]=90;
	g_Ini.fBackSpeed[2]=90;
	g_Ini.fBackSpeed[3]=90;
	g_Ini.iBackDir[0]=0;
	g_Ini.iBackDir[1]=0;
	g_Ini.iBackDir[2]=0;
	g_Ini.iBackDir[3]=0;
	g_Ini.iAxisSwitch[0]=1;
	g_Ini.iAxisSwitch[1]=1;
	g_Ini.iAxisSwitch[2]=1;
	g_Ini.iAxisSwitch[3]=1;
	g_Ini.fStartOffset[0]=0;
	g_Ini.fStartOffset[1]=0;
	g_Ini.fStartOffset[2]=0;
	g_Ini.fStartOffset[3]=0;
	g_Ini.iBackMode[0]=0;													//默认正向回零
	g_Ini.iBackMode[1]=0;
	g_Ini.iBackMode[2]=0;
	g_Ini.iBackMode[3]=0;
	g_Ini.iHardLimitSwitch[0]=0;
	g_Ini.iHardLimitSwitch[1]=0;
	g_Ini.iHardLimitSwitch[2]=0;
	g_Ini.iHardLimitSwitch[3]=0;
	g_Ini.iZeroReplaceLimit[0]=0;
	g_Ini.iZeroReplaceLimit[1]=0;
	g_Ini.iZeroReplaceLimit[2]=0;
	g_Ini.iZeroReplaceLimit[3]=0;
	g_Ini.lChAddSpeed[0]=10000;
	g_Ini.lChAddSpeed[1]=10000;
	g_Ini.lChAddSpeed[2]=10000;
	g_Ini.lChAddSpeed[3]=10000;

	g_Ini.lRunCount=0;
	g_Ini.lDestCount=10000;
	g_Ini.lProbeCount=10;
	g_Ini.fXMaxSpeed=500.0;
	g_Ini.lPassword=123;
	g_Ini.fStartSpeed=2.0;
	//g_Ini.lAddSpeed=3000;

	//g_Ini.iAlarmLogic=0;
	g_Ini.iScrProtect=1;
	g_Ini.iScrProtectTime=30;
	g_Ini.iScrProtectBmp=0;	
	g_Ini.iXianjiaDrive=1;


	g_Ini.iHandRunSpeed=2;
	g_Ini.fHandRunRate=50.0;													//指示手摇加工的脉冲计数的比率	
	g_Ini.iReverseDly=20;				//未使用 注释掉  20140807				//正反转延时

	//g_Ini.iSpeedDly=2;				//未使用 注释掉  20140807
	g_Ini.iPaoXian=1;
	g_Ini.iWarningOut=0;
	g_Ini.iYDaoXian=1;
	g_Ini.iProbeAheadTime=0;

	g_Ini.iProbeLagTime=0;
	g_Ini.iFileName = 000;

	//g_Ini.fBackSpeed1[0] = 1.5;
	//g_Ini.fBackSpeed1[1] = 1;
	g_Ini.iLanguage = 0;
	g_Ini.iAxisCount = 2;
	//g_Ini.iUDriver = 1;

	g_Ini.iServoLogic[0] = 1;
	g_Ini.iServoLogic[1] = 1;
	g_Ini.iServoLogic[2] = 1;
	g_Ini.iServoLogic[3] = 1;
	g_Ini.iServoZero[0]  = 0;
	g_Ini.iServoZero[1]  = 0;
	g_Ini.iServoZero[2]  = 0;
	g_Ini.iServoZero[3]  = 0;
	g_Ini.iAxisRunMode[0]= 0;
	g_Ini.iAxisRunMode[1]= 0;
	g_Ini.iAxisRunMode[2]= 0;			//丝杆模式
	g_Ini.iAxisRunMode[3]= 0;


	//g_Ini.iHandStatus=1;
	//g_Ini.iStopMode = 1;
	g_Ini.iPerCount = 0;
	g_Ini.iProbeMode = 1;
	g_Ini.iPbFnSl=1;
	g_Ini.iProDelay=0;
	//g_Ini.iZSoftLimit=0;
	g_Ini.fMinLimit[2]=0.0;
	g_Ini.fMaxLimit[2]=0.0;
	g_Ini.fMinLimit[3]=0.0;
	g_Ini.fMaxLimit[3]=0.0;
	g_Ini.iZBackZero=1;
	g_Ini.iSystemName=0;
	g_Ini.iCyldMode =0;
	g_Ini.iSecondAxisStyle = 0;

	g_Ini.iSendClear=0.0;
	g_Ini.iSwhSingleIo=0;
 	g_Ini.iSwhRunIo=0;

	g_Ini.iIOKinkConfig_IN=21;			//缠线报警端口
	g_Ini.iIOKinkConfig_Level=0;		//缠线报警电平
	g_Ini.iIOBreakLineConfig_IN=20;		//断线报警端口
	g_Ini.iIOBreakLineConfig_Level=1;	//断线报警电平
	g_Ini.iIORunLineConfig_IN=8;		//跑线报警端口
	g_Ini.iIOWireAccConfig_OUT=7;		//线架加速输出端口
	g_Ini.iIOSart_IN=14;				//外部启动输入端口
	g_Ini.iIOSingleConfig_IN=13;		//外部单条输入端口
	g_Ini.iIOAlarmConfig_OUT=9;			//报警输出端口
	g_Ini.iIOSendLinePConfig_IN=14;		//送线+输入端口
	g_Ini.iIOSendLineMConfig_IN=13;		//送线-输入端口
}


void 	DisableParaInit(void)
{
	INT16S i =0;

	for(i=0;i<MAXCH ;i++)
	{
		g_Ini.iLag[i]=0;					
		g_Ini.iNumerator[i]=1;				
		g_Ini.iDenominator[i]=1;
	}

	g_Ini.fVoltageOne = 0.0;
	g_Ini.fVoltageTwo = 0.0;
	g_Ini.iBoxStatus  = 0; 

}


void    SysParaWrite()
{
	FIL 		fd;
	FRESULT		res;
	UINT		bw;

	res = f_open(&fd,"0:\\PARA\\SYSTEM.INI",FA_CREATE_ALWAYS|FA_WRITE);

	if(res != FR_OK)
	{
		f_close(&fd) ;
		return;
	}

	res = f_lseek(&fd,0);
	res = f_write(&fd,&g_Ini,sizeof(BASEINI),&bw);

	f_close(&fd) ;

	return;
}


BOOL	Check_DataInt(INT16S value,INT16S minv,INT16S maxv)
{
	if(value<minv || value>maxv)
		return FALSE;
	else
		return TRUE;
}


BOOL	Check_DataLong(INT32S value,INT32S minv,INT32S maxv)
{
	if(value<minv || value>maxv)
		return FALSE;
	else
		return TRUE;
}


BOOL	Check_DataFloat(FP32 value,FP32 minv,FP32 maxv)
{
	if(value<minv || value>maxv)
		return FALSE;
	else
		return TRUE;
}


BOOL	SysParaRead()
{

	FIL 		fd;
	FRESULT		res;
	UINT		br;

	res= f_open(&fd, "0:\\PARA\\SYSTEM.INI", FA_OPEN_EXISTING|FA_READ);

	if(res != FR_OK)
	{
		goto FILEERR;
	}

	res = f_lseek(&fd,0);
	res = f_read(&fd,&g_Ini,sizeof(BASEINI),&br);

	if(res != FR_OK)
	{
		goto FILEERR;
	}

	if(!SysParaCheck())
	{
		goto FILEERR;
	}

	f_close(&fd) ;
	return TRUE;

	FILEERR:
	f_close(&fd) ;
	return FALSE;
}


BOOL	SysParaCheck()
{
	INT16S i=0;

	for(i=0;i<MAXCH;i++)
	{
		if(!Check_DataLong(g_Ini.lPulse[i],200L,40000L))
			return FALSE;
		if(!Check_DataFloat(g_Ini.fMM[i],0.5,2000.0f))
			return FALSE;
		if(!Check_DataInt(g_Ini.iRunDir[i],0,1))
			return FALSE;
		if(!Check_DataInt(g_Ini.iBackDir[i],0,1))
			return FALSE;
		if(!Check_DataFloat(g_Ini.fHandSpeed[i],0.1,1000.0))
			return FALSE;
		if(!Check_DataFloat(g_Ini.fBackSpeed[i],0.1,1000.0))
			return FALSE;
		if(!Check_DataFloat(g_Ini.fGearRate[i],0.01,100.0))
			return FALSE;
		if(!Check_DataFloat(g_Ini.lMaxSpeed[i],100.0,8000.0))
			return FALSE;
		if(!Check_DataLong(g_Ini.iLag[i],-999999L,999999L))
			return FALSE;
		if(!Check_DataLong(g_Ini.iNumerator[i],1,999999))
			return FALSE;
		if(!Check_DataLong(g_Ini.iDenominator[i],-99999,999999))
			return FALSE;
		if(!Check_DataInt(g_Ini.iServoLogic[i],0,1))
			return FALSE;
		if(!Check_DataInt(g_Ini.iServoZero[i],0,1))
			return FALSE;
		if(!Check_DataInt(g_Ini.iAxisSwitch[i],0,1))
			return FALSE;
		if(!Check_DataFloat(g_Ini.fMaxLimit[i],-1000,1000))
			return FALSE;	
		if(!Check_DataFloat(g_Ini.fMinLimit[i],-1000,1000))
			return FALSE;	
		if(!Check_DataInt(g_Ini.iHardLimitSwitch[i],0,1))
			return FALSE;	
		if(!Check_DataInt(g_Ini.iZeroReplaceLimit[i],0,2))
			return FALSE;							
		if(!Check_DataLong(g_Ini.lChAddSpeed[i],50,20000))
			return FALSE;	
		if(!Check_DataInt(g_Ini.iBackMode[i],0,3))
			return FALSE;

	}

	if(!Check_DataFloat(g_Ini.fStartOffset[0],-360.0,360.0))
		return FALSE;
	if(!Check_DataFloat(g_Ini.fStartOffset[1],-360.0,360.0))
		return FALSE;
	if(!Check_DataFloat(g_Ini.fStartOffset[2],-1000.0,1000.0))
		return FALSE;
	if(!Check_DataFloat(g_Ini.fStartOffset[3],-1000.0,1000.0))
		return FALSE;

	if(!Check_DataLong(g_Ini.lRunCount,0L,9999999L))
		return FALSE;
	if(!Check_DataLong(g_Ini.lDestCount,0L,9999999L))
		return FALSE;
	if(!Check_DataLong(g_Ini.lProbeCount,0L,99999L))
		return FALSE;

	if(!Check_DataLong(g_Ini.lPassword,0L,999999L))
		return FALSE;

	if(!Check_DataFloat(g_Ini.fXMaxSpeed,1.0,3000.0))
		return FALSE;

	//if(!Check_DataFloat(g_Ini.fStartSpeed,1.0,1000.0))
	//return FALSE;


	//if(!Check_DataInt(g_Ini.iAlarmLogic,0,1))
	//return FALSE;

	if(!Check_DataInt(g_Ini.iScrProtect,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iScrProtectTime,1,100))
		return FALSE;

	if(!Check_DataInt(g_Ini.iScrProtectBmp,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iXianjiaDrive,0,1))
		return FALSE;

	//if(!Check_DataInt(g_Ini.iHandStatus,0,1))
	//return FALSE;

	if(!Check_DataInt(g_Ini.iHandRunSpeed,0,10))
		return FALSE;

	if(!Check_DataFloat(g_Ini.fHandRunRate,1.0,1000.0))
		return FALSE;

	//if(!Check_DataInt(g_Ini.iReverseDly,0,30000))//未使用 注释掉  20140807
	//return FALSE;

	if(!Check_DataInt(g_Ini.iPaoXian,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iWarningOut,0,1))
		return FALSE;

	//if(!Check_DataInt(g_Ini.iSpeedDly,0,30000))//未使用 注释掉  20140807
	//return FALSE;

	if(!Check_DataInt(g_Ini.iLanguage,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iAxisCount,2,3))
		return FALSE;

	if(!Check_DataInt(g_Ini.iYDaoXian,0,1))
		return FALSE;

	//if(!Check_DataInt(g_Ini.iProbeAheadTime,0,9999))
	//return FALSE;

	//if(!Check_DataInt(g_Ini.iProbeLagTime,0,9999))
	//return FALSE;

	//if(!Check_DataInt(g_Ini.iUDriver,0,1))
	//return FALSE;

	/*if(!Check_DataInt(g_Ini.iServoLogicX,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoLogicY,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoLogicZ,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoLogicA,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoZeroX,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoZeroZ,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iServoZeroA,0,1))
		return FALSE;*/

	if(!Check_DataInt(g_Ini.iAxisRunMode[2],0,2))
		return FALSE;

	if(!Check_DataInt(g_Ini.iAxisRunMode[3],0,2))
		return FALSE;	

	//if(!Check_DataFloat(g_Ini.fVoltageOne,0.00,10.00))
	//return FALSE;

	//if(!Check_DataFloat(g_Ini.fVoltageTwo,0.00,10.00))
	//return FALSE;

	//if(!Check_DataInt(g_Ini.iBoxStatus,0,1))
	//return FALSE;

	//if(!Check_DataInt(g_Ini.iStopMode,0,1))
	//return FALSE;

	if(!Check_DataLong(g_Ini.iPerCount,0,999999))
		return FALSE;

	if(!Check_DataInt(g_Ini.iProbeMode,0,1))
		return FALSE;

	//if(!Check_DataInt(g_Ini.iZSoftLimit,0,1))
	//return FALSE;

	if(!Check_DataLong(g_Ini.iSystemName,0,9999))
		return FALSE;	

	if(!Check_DataInt(g_Ini.iCyldMode,0,1))
		return FALSE;
	if(!Check_DataInt(g_Ini.iPbFnSl,0,1))
		return FALSE;

	if(!Check_DataFloat(g_Ini.iProDelay,0,9999))
		return FALSE;

	if(!Check_DataInt(g_Ini.iSecondAxisStyle,0,1))
		return FALSE;	
	if(!Check_DataInt(g_Ini.iSwhSingleIo,0,2))
		return FALSE;

	if(!Check_DataInt(g_Ini.iSwhRunIo,0,1))
		return FALSE;
	//add
	if(!Check_DataInt(g_Ini.iIOKinkConfig_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOKinkConfig_Level,0,1))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOBreakLineConfig_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOBreakLineConfig_Level,0,1))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIORunLineConfig_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOWireAccConfig_OUT,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOSart_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOSingleConfig_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOAlarmConfig_OUT,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOSendLinePConfig_IN,0,255))
		return FALSE;
	if(!Check_DataInt(g_Ini.iIOSendLineMConfig_IN,0,255))
		return FALSE;

	return  TRUE;
}


void	SysParaCheckUpdate()
{
	CHECK_DATA(lPulse[0],200L,40000L)
			CHECK_DATA(lPulse[1],200L,40000L)
			CHECK_DATA(lPulse[2],200L,40000L)
			CHECK_DATA(lPulse[3],200L,40000L)

			CHECK_DATA(fMM[0],0.5f,2000.0f)
			CHECK_DATA(fMM[1],0.5f,2000.0f)
			CHECK_DATA(fMM[2],0.5f,2000.0f)
			CHECK_DATA(fMM[3],0.5f,2000.0f)

			CHECK_DATA(iRunDir[0],0,1)
			CHECK_DATA(iRunDir[1],0,1)
			CHECK_DATA(iRunDir[2],0,1)
			CHECK_DATA(iRunDir[3],0,1)

			CHECK_DATA(iBackDir[0],0,1)
			CHECK_DATA(iBackDir[1],0,1)
			CHECK_DATA(iBackDir[2],0,1)
			CHECK_DATA(iBackDir[3],0,1)

			CHECK_DATA(iBackMode[0],0,2)
			CHECK_DATA(iBackMode[1],0,2)
			CHECK_DATA(iBackMode[2],0,2)
			CHECK_DATA(iBackMode[3],0,2)

			CHECK_DATA(fHandSpeed[0],0.1f,1000.0f)
			CHECK_DATA(fHandSpeed[1],0.1f,1000.0f)
			CHECK_DATA(fHandSpeed[2],0.1f,1000.0f)
			CHECK_DATA(fHandSpeed[3],0.1f,1000.0f)

			CHECK_DATA(fBackSpeed[0],0.1f,1000.0f)
			CHECK_DATA(fBackSpeed[1],0.1f,1000.0f)
			CHECK_DATA(fBackSpeed[2],0.1f,1000.0f)
			CHECK_DATA(fBackSpeed[3],0.1f,1000.0f)

			CHECK_DATA(fGearRate[0],0.01f,100.0f)
			CHECK_DATA(fGearRate[1],0.01f,100.0f)
			CHECK_DATA(fGearRate[2],0.01f,100.0f)
			CHECK_DATA(fGearRate[3],0.01f,100.0f)

			CHECK_DATA(lMaxSpeed[0],100,8000)
			CHECK_DATA(lMaxSpeed[1],100,8000)
			CHECK_DATA(lMaxSpeed[2],100,8000)
			CHECK_DATA(lMaxSpeed[3],100,8000)

			CHECK_DATA(fStartOffset[0],-360.0,360.0)
			CHECK_DATA(fStartOffset[1],-360.0,360.0)
			CHECK_DATA(fStartOffset[2],-1000.0,1000.0)
			CHECK_DATA(fStartOffset[3],-1000.0,1000.0)

			CHECK_DATA(lRunCount,0,9999999L)
			CHECK_DATA(lDestCount,0,9999999L)
			CHECK_DATA(lProbeCount,0,99999L)

			CHECK_DATA(lPassword,0L,999999L)
			CHECK_DATA(fXMaxSpeed,1.0,3000.0)

			//CHECK_DATA(fStartSpeed,1.0,1000.0)
			//CHECK_DATA(lAddSpeed,1,100000)

			//CHECK_DATA(iAlarmLogic,0,1)
			CHECK_DATA(iScrProtect,0,1)
			CHECK_DATA(iScrProtectTime,1,100)

			CHECK_DATA(iScrProtectBmp,0,1)
			CHECK_DATA(iXianjiaDrive,0,1)

			//CHECK_DATA(iHandStatus,0,1)
			CHECK_DATA(iHandRunSpeed,0,10)
			CHECK_DATA(fHandRunRate,1.0,1000.0)

			CHECK_DATA(iReverseDly,0,30000)//未使用 注释掉  20140807
			CHECK_DATA(iPaoXian,0,1)
			CHECK_DATA(iWarningOut,0,1)
			//CHECK_DATA(iSpeedDly,0,30000)//未使用 注释掉  20140807

			CHECK_DATA(iLanguage,0,1)
			CHECK_DATA(iAxisCount,2,3)
			//CHECK_DATA(iUDriver,0,1)
			CHECK_DATA(iYDaoXian,0,1)
			CHECK_DATA(iProbeAheadTime,0,9999)
			CHECK_DATA(iProbeLagTime,0,9999)
			//CHECK_DATA(iLanguage,0,1)
			/*CHECK_DATA(iServoLogicX,0,1)
	CHECK_DATA(iServoLogicY,0,1)
	CHECK_DATA(iServoLogicZ,0,1)
	CHECK_DATA(iServoLogicA,0,1)*/

			CHECK_DATA(iServoLogic[0],0,1)
			CHECK_DATA(iServoLogic[1],0,1)
			CHECK_DATA(iServoLogic[2],0,1)
			CHECK_DATA(iServoLogic[3],0,1)


			/*CHECK_DATA(iServoZeroX,0,1)
	CHECK_DATA(iServoZeroZ,0,1)
	CHECK_DATA(iServoZeroA,0,1)*/

			CHECK_DATA(iServoZero[0],0,1)
			CHECK_DATA(iServoZero[1],0,1)
			CHECK_DATA(iServoZero[2],0,1)
			CHECK_DATA(iServoZero[3],0,1)

			CHECK_DATA(iAxisRunMode[2],0,2)
			CHECK_DATA(iAxisRunMode[3],0,2)

			CHECK_DATA(fVoltageOne,0.00,10.00)
			CHECK_DATA(fVoltageTwo,0.00,10.00)
			//CHECK_DATA(iBoxStatus,0,1)
			CHECK_DATA(iProbeMode,0,1)
			CHECK_DATA(iPerCount,0,999999L)
			CHECK_DATA(iLag[0],-999999L,999999L)
			CHECK_DATA(iLag[1],-999999L,999999L)
			CHECK_DATA(iLag[2],-999999L,999999L)
			CHECK_DATA(iLag[3],-999999L,999999L)

			CHECK_DATA(iNumerator[0],1,999999L)
			CHECK_DATA(iNumerator[1],1,999999L)
			CHECK_DATA(iNumerator[2],1,999999L)
			CHECK_DATA(iNumerator[3],1,999999L)

			CHECK_DATA(iDenominator[0],-99999,999999L)
			CHECK_DATA(iDenominator[1],-99999,999999L)
			CHECK_DATA(iDenominator[2],-99999,999999L)
			CHECK_DATA(iDenominator[3],-99999,999999L)

			//CHECK_DATA(iZSoftLimit,0,1)

			CHECK_DATA(fMaxLimit[2],-1000,1000)
			CHECK_DATA(fMinLimit[2],-1000,1000)
			CHECK_DATA(fMaxLimit[3],-1000,1000)
			CHECK_DATA(fMinLimit[3],-1000,1000)

			CHECK_DATA(iZBackZero,0,1)

			CHECK_DATA(iAxisSwitch[0],0,1)
			CHECK_DATA(iAxisSwitch[1],0,1)
			CHECK_DATA(iAxisSwitch[2],0,1)
			CHECK_DATA(iAxisSwitch[3],0,1)
			CHECK_DATA(iSystemName,0,9999)
			CHECK_DATA(iCyldMode,0,1)
			CHECK_DATA(iPbFnSl,0,1)
			CHECK_DATA(iProDelay,0,9999)
}




void	SetUnitData(INT32S nL,INT8U nC)
{
	INT16S i;

	g_UnitData.nLine=nL;
	g_UnitData.nCode=nC;

	for(i=0;i<5;i++)
		g_UnitData.lValue[i]=0L;

	g_UnitData.nIO=0;

	for(i=0;i<MAXCYLINDER;i++)
		g_UnitData.nOut[i]=2;

	g_UnitData.nReset = 0;
}


INT16U MEMToXMSEx(INT8U *p,INT16U handle,INT32U offset,INT16U length)
{
#if OS_CRITICAL_METHOD == 3                          		 				/* Allocate storage for CPU status register      */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                       						/* Prevent compiler warning                      */
#endif

	OS_ENTER_CRITICAL();

	switch(handle)
	{
	case 1 :memcpy(&g_ucDataArray[offset],p,length);break;
	case 2 :memcpy(&g_ucXMSArray[offset],p,length);	break;
	}

	OS_EXIT_CRITICAL();

	return 1;

}


INT16U XMSToMEMEx(INT8U *p,INT16U handle,INT32U offset,INT16U length)
{

#if OS_CRITICAL_METHOD == 3                          				 		/* Allocate storage for CPU status register      */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0;                                      	 					/* Prevent compiler warning                      */
#endif

	OS_ENTER_CRITICAL();

	switch(handle)
	{
	case 1:memcpy(p,&g_ucDataArray[offset],length);	break;
	case 2:memcpy(p,&g_ucXMSArray[offset],length);	break;
	}

	OS_EXIT_CRITICAL();

	return 1;

}



void	ReadRunCount()
{

	if(g_bRam)
	{
		ReadCountFromRam();
	}
	else
	{
		FIL 		fd;
		FRESULT		res;
		UINT		br;

		res= f_open (&fd, "0:\\PARA\\runcount.ini", FA_OPEN_EXISTING|FA_READ);

		if(res != FR_OK)
		{
			goto FILEERR;
		}

		res = f_lseek(&fd,0);

		res = f_read(&fd,&g_Ini.lRunCount,sizeof(g_Ini.lRunCount),&br);

		if(g_Ini.lRunCount>9999999)
			g_Ini.lRunCount=9999999;

		if(g_Ini.lRunCount<0)
			g_Ini.lRunCount=0;

		FILEERR:
		f_close(&fd) ;

	}
}


void	WriteRunCount()
{
	if(g_bRam)
	{
		WriteCountToRam();
	}
	else
	{
		FIL 		fd;
		FRESULT		res;
		UINT		bw;

		res = f_open(&fd,"0:\\PARA\\runcount.ini",FA_CREATE_ALWAYS|FA_WRITE);
		//	fp=FS_FOpen("\\PARA\\runcount.ini","wb");

		if(res != FR_OK)
		{
			f_close(&fd) ;
			return;
		}

		res = f_lseek(&fd,0);
		res = f_write(&fd,&g_Ini.lRunCount,sizeof(g_Ini.lRunCount),&bw);

		f_close(&fd) ;
	}
}


INT16S FileExist(INT16S File)								//判断料号为File的文件是否存在
{

	FIL 		fd;
	FRESULT		res;
	UINT		br;

	INT16S 		i=0,size=0;
	INT8S 		buffer[0x10]="THJ000.DAT";
	INT8S 		tmpbuf[0x20]="0:\\PROG\\";
	INT8S 		filemessage[0x40]="\0";
	INT32S 		ibufcount = 0;

	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	for(i=0; i<MAXLINE; i++)
		DataCls(i);

	Line_Data_Size = 0;

	strcat(tmpbuf,buffer);

	res= f_open (&fd, tmpbuf, FA_OPEN_EXISTING|FA_READ);

	if(res != FR_OK)
	{
		f_close(&fd) ;
		OSQPost(MsgQueue,"文件打开错误，或文件不存在");
		Uart_Printf("File Open Err!Press Any Key...\n");
		return FALSE;
	}

	f_close(&fd) ;

	return TRUE;
}


#define 	TESTRAM_ADDR		0						//铁电测试段起启地址
BOOL	CheckRam(void)
{
	INT16S i=0;
	INT8U  da[4],da1[4];

	int res=-1;

	res = FRAM_Init();

	if(res == 1)
	{
		goto Err;
	}

	for(i=0;i<4;i++)
	{
		da[i]=(1+i*2);
	}

	res = FRAM_Write(da,TESTRAM_ADDR, sizeof(da));

	if(res == 1)
	{
		goto Err;
	}

	res = FRAM_Read (da1,TESTRAM_ADDR, sizeof(da));

	if(res == 1)
	{
		goto Err;
	}

	for(i=0;i<4;i++)
	{
		if(da[i]!=da1[i])
		{
			break;
		}
	}

	if(i==4)
		return TRUE;
	else
		return FALSE;

	Err:
	return FALSE;
}


#define  COUNTRAM_ADDR			8
void WriteCountToRam(void)
{
	INT8U  da[4];

	memcpy(da,&g_Ini.lRunCount,sizeof(g_Ini.lRunCount));
	FRAM_Write(da,COUNTRAM_ADDR, sizeof(g_Ini.lRunCount));

}

void ReadCountFromRam(void)
{
	INT8U da[4];

	FRAM_Read (da,COUNTRAM_ADDR, sizeof(g_Ini.lRunCount));
	memcpy(&g_Ini.lRunCount,da,sizeof(g_Ini.lRunCount));
}


#define		SAVEPOS_ADDR		16
void WritePosToRam(INT16S ch, INT32S pos)
{
	INT8U  da[4];

	memcpy(da,&pos,sizeof(pos));
	FRAM_Write(da,SAVEPOS_ADDR+ch*4, sizeof(pos));
}

INT32S ReadPosFromRam(INT16S ch)
{
	INT8U 	da[4];
	static 	INT32S pos;

	FRAM_Read (da,SAVEPOS_ADDR+ch*4, sizeof(pos));
	memcpy(&pos,da,sizeof(pos));

	return  pos;
}














#if FALSE

void WritePosToRam(INT16S ch, INT32S pos)
{
	*((volatile unsigned int *)(0x28000204+ch*4)) = pos;
}

INT32S ReadPosFromRam(INT16S ch)
{

	return *((volatile unsigned int *)(0x28000204+ch*4));
}


INT16U	ReadPortb(INT16U port)													//读铁电，铁电总大小:32K
{
	return *((volatile unsigned char *)(0x28000004+port));
}


void	WritePortb(INT16U port,INT32U data)										//写铁电
{
	*((volatile unsigned char *)(0x28000004+port)) = data;
}


BOOL	CheckRam(void)
{
	INT16S i=0;
	INT8U  da[4],da1[4];

	for(i=0;i<4;i++)
	{
		da[i]=(1+i*2);
	}

	for(i=0;i<4;i++)
	{
		WritePortb(0x100+i,da[i]);												//铁电使用时，不可使用0x100~0x103地址，否则参数会被覆盖
	}

	for(i=0;i<4;i++)
	{
		da1[i]=ReadPortb(0x100+i);
	}

	for(i=0;i<4;i++)
	{
		if(da[i]!=da1[i])
		{
			break;
		}
	}

	if(i==4)
		return TRUE;
	else
		return FALSE;	
}


void	WriteDataToYaffsFile( INT16S File)
{
	int 	  	fw;
	COMMAND 	*ptr;
	INT8S 		tmpbuf[0x20]="/flash/PROG/"; 						//文件直接放根目录									//20070123 modify add
	INT8S 		buffer[0x10]="\0"; 
	//	INT8S 		buf[10]="\0";
	//	INT16S 		i=0,k=0;
	INT8U 		err;
	INT8S 		filemessage[0x40]= "";
	INT8S  		ti[10],da[10];

	INT16S 		ibufcount = 0;

	//	INT32S 	t1=0,t2=0;
	//	t1 = OSTimeGet();

	RtcGetDateStr(da);						
	RtcGetTimeStr(ti);

	if( !Line_Data_Size )
	{
		//OSQPost(MsgQueue,"Data is empty!");
		Uart_Printf("Data is empty!Press Any Key...\n");
		return;
	}

	strcpy(buffer,"THJ000.DAT");
	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	strcat(tmpbuf,buffer);         								//文件路径名

	fw = yaffs_open(tmpbuf,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

	if( fw == -1 )
	{
		Uart_Printf("yaffs区未格式化!!!\n");
		goto Err;
	}

	strcat(filemessage,da); 
	strcat(filemessage," "); 
	strcat(filemessage,ti); 

	memset(filebuf,NULL,sizeof(filebuf));												//绝对不可丢掉	

	memcpy(filebuf+ibufcount,filemessage,0x40);
	ibufcount += 0x40;

	memcpy(filebuf+ibufcount,&Line_Data_Size,sizeof(Line_Data_Size));
	ibufcount += sizeof(Line_Data_Size);


	ptr = Line_Data;


	memcpy(filebuf+ibufcount,Line_Data,Line_Data_Size*sizeof(COMMAND));
	ibufcount += Line_Data_Size*sizeof(COMMAND);

	CHECK_DATA(lRunCount,0,9999999L)
	CHECK_DATA(lDestCount,0,9999999L)
	CHECK_DATA(lProbeCount,0,99999L)

	{
		INT32S tmp[10];

		tmp[0] = g_Ini.lDestCount;
		tmp[1] = g_Ini.lRunCount;
		tmp[2] = g_Ini.lProbeCount;
		//	tmp[3] = g_Ini.lRunSpeed[0];
		//	tmp[4] = g_Ini.lRunSpeed[1];
		//	tmp[5] = g_Ini.lRunSpeed[2];
		//	tmp[6] = g_Ini.lRunSpeed[3];
		//	tmp[7] = g_Ini.lStartOffset[0];
		//	tmp[8] = g_Ini.lStartOffset[2];
		//	tmp[9] = g_Ini.lStartOffset[3];


		memcpy(filebuf+ibufcount,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);

	}

	yaffs_lseek(fw,0,SEEK_SET);

	err=yaffs_write(fw,filebuf,ibufcount);

	Err:

	yaffs_close(fw);

	//	t2 = OSTimeGet();
	//	Uart_Printf("t2-t1 = %ld\n",t2-t1);
}


BOOL	ReadDataFromYaffsFile(INT16S File )
{
	int		 	fr;
	INT16S 		i=0,size=0;//,j=0,l=0 k=0,
	//	INT8S 		buf[9]="\0";
	INT8S 		buffer[0x10]="THJ000.DAT";
	INT8S 		tmpbuf[0x20]="/flash/PROG/"; 
	INT8S 		filemessage[0x40]="\0";
	//	INT8U 		err;
	//	COMMAND 	*ptr;

	INT16S 		ibufcount = 0;

	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	for(i=0; i<MAXLINE; i++)
		DataCls(i);

	Line_Data_Size = 0;

	strcat(tmpbuf,buffer); 

	fr=yaffs_open(tmpbuf,O_RDONLY , S_IREAD | S_IWRITE);

	if( fr == -1 )
	{
		yaffs_close(fr);
		//	OSQPost(MsgQueue,"文件打开错误，或文件不存在");
		return FALSE;
	}

	ibufcount = 0;
	yaffs_lseek(fr,ibufcount,SEEK_SET);
	yaffs_read(fr,&filemessage,0x40);
	ibufcount += 0x40;

	yaffs_lseek(fr,ibufcount,SEEK_SET);
	yaffs_read(fr,&size,sizeof(size));
	ibufcount += sizeof(size);

	if( size < 1 || size > MAXLINE )
	{
		yaffs_close(fr);
		return FALSE;
	}

	Line_Data_Size = size;

	{
		//	FS_size_t status;

		yaffs_lseek(fr,ibufcount,SEEK_SET);
		yaffs_read(fr,Line_Data,Line_Data_Size*sizeof(COMMAND));
		ibufcount += Line_Data_Size*sizeof(COMMAND);
	}

	{
		INT32S tmp[10];
		INT32S temp[3];

		yaffs_lseek(fr,ibufcount,SEEK_SET);
		yaffs_read(fr,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);



		g_Ini.lDestCount= tmp[0];
		g_Ini.lRunCount = tmp[1];
		g_Ini.lProbeCount= tmp[2];

		//	g_Ini.lRunSpeed[0] = tmp[3];
		//	g_Ini.lRunSpeed[1] = tmp[4];
		//	g_Ini.lRunSpeed[2] = tmp[5];
		//	g_Ini.lRunSpeed[3] = tmp[6];
		/*
		temp[0] = tmp[7];
		temp[1] = tmp[8];
		temp[2] = tmp[9];

		if(temp[0]!=g_Ini.lStartOffset[0])
		{
			g_bFirstResetX = FALSE;
			g_Ini.lStartOffset[0] = temp[0];
		}

		if(temp[1]!=g_Ini.lStartOffset[2])
		{
			g_bFirstResetZ = FALSE;
			g_Ini.lStartOffset[2] = temp[1];
		}

		if(temp[2]!=g_Ini.lStartOffset[3])
		{
			g_bFirstResetU = FALSE;
			g_Ini.lStartOffset[3] = temp[2];
		}*/
	}

	//SysParaCheckUpdate();
	yaffs_close(fr);

	return TRUE;
}


void    SysParaWriteYaffs()
{
	int  		fw;
	INT8U 		err;

	INT8S 		tmpbuf[0x20]="/flash/SYSTEM.INI";

	fw = yaffs_open(tmpbuf,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

	if( fw == -1 )
	{
		Uart_Printf("yaffs区未格式化!!!\n");
		goto Err;
	}	

	yaffs_lseek(fw,0,SEEK_SET);

	err=yaffs_write(fw,&g_Ini,sizeof(BASEINI));

	Err:
	yaffs_close(fw);

}


BOOL	SysParaReadYaffs()
{
	int  		fr;
	//	INT8U 		err;

	INT8S 		tmpbuf[0x20]="/flash/SYSTEM.INI";

	fr=yaffs_open(tmpbuf,O_RDONLY , S_IREAD | S_IWRITE);

	if( fr == -1 )
	{
		Uart_Printf("yaffs区未格式化!!!\n");
		yaffs_close(fr);
		return FALSE;
	}	

	yaffs_lseek(fr,0,SEEK_SET);

	yaffs_read(fr,&g_Ini,sizeof(BASEINI));

	yaffs_close(fr);

	if(!SysParaCheck())
	{
		return FALSE;
	}

	return TRUE;
}






INT16S CopyYaffsToYaffsFile(INT16S FileD, INT16S FileS)			//复制料号为FileS的文件到料号为FileD的文件
{

	INT32S err,err2,fw1,fw2,j=0;
	INT8U 	Semerr;	
	INT8U 	*YaffsCopyBuf=NULL;
	//INT32S lFileLen=0;		
	INT8S  bufferS[0x10]="\0"; 
	INT8S  bufferD[0x10]="\0";
	INT8S  tmpbufS[0x20]="/flash/PROG/"; 
	INT8S  tmpbufD[0x20]="/flash/PROG/"; 

	OSSemPend(FileSem,5000,&Semerr);

	if(Semerr!=OS_NO_ERR)
	{
		return 0;
	}		

	strcpy(bufferS,"THJ000.DAT");
	bufferS[3] = FileS / 100+'0';
	bufferS[4] = (FileS%100) / 10+'0';
	bufferS[5] = FileS % 10+'0';

	strcpy(bufferD,"THJ000.DAT");
	bufferD[3] = FileD / 100+'0';
	bufferD[4] = (FileD%100) / 10+'0';
	bufferD[5] = FileD % 10+'0';


	if(strcmp(bufferD,bufferS)==0)
	{
		OSSemPost(FileSem);
		return 0;
	}
	else 
	{
		strcat(tmpbufS,bufferS);
		strcat(tmpbufD,bufferD);

		fw1 = yaffs_open(tmpbufS,O_RDONLY, S_IREAD | S_IWRITE);

		if( fw1==-1 )
		{
			OSSemPost(FileSem);
			return 0;
		}

		fw2 = yaffs_open(tmpbufD,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

		if( fw2==-1 )
		{
			OSSemPost(FileSem);
			return 0;
		}

		//非常重要的函数，指针起始偏移
		err=yaffs_lseek(fw1,0,SEEK_SET);
		err2=yaffs_lseek(fw2,0,SEEK_SET);
		YaffsCopyBuf=malloc(g_iBufSize);

		for(;;)
		{
			err=yaffs_read(fw1,YaffsCopyBuf,g_iBufSize);	
			err2=yaffs_write(fw2,YaffsCopyBuf,err);	

			j=j+err2;

			if((err != err2) || (err != g_iBufSize))
			{	
				break;
			}
		}

		free(YaffsCopyBuf);
		yaffs_close(fw1);
		yaffs_close(fw2);
		OSSemPost(FileSem);
		return 1;
	}
}

#endif

