# include   "includes.h"
# include	"xms.h"

TEST_EXT	OS_EVENT	*FileSem;												//文件操作信号量,防止同时进行磁盘IO
TEST_EXT    OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bRam;													//指示是否含有带电RAM或电池是否充足
TEST_EXT	BOOL		g_bTH836;
WORK_EXT	BOOL		g_bLCD;
WORK_EXT 	INT32S  	g_lRamRunCounts;
WORK_EXT 	BOOL		g_bFirstResetX;
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;

#define 	SRAMADDR	(volatile unsigned char *)(0x0e000008)					//将参数和数据文件保存到铁电中
#define 	DATAADDR	350
#define 	PASSDDR    	20

BASEINI		g_Ini;
COMMAND		Line_Data[MAXLINE+1];												//不用直接声明数组的形式可以防止全局变量声明过多导致超过64K的限制
INT16S		Line_Data_Size=0;													//指示当前数据行数
UNITDATA    g_UnitData;

const 		INT16U	 	g_uData=1;	   											//指向加工数据解码后的XMS句柄
const 		INT16U	 	g_uXMS=2;	   											//指向加工数据解码后的XMS句柄
INT16U					g_uVideoXMS=0;
extern 		INT8U		g_ucDataArray[DATASIZE];
extern 		INT8U		g_ucXMSArray[XMSSIZE];
INT8S 		filebuf[60000];														//必须定义为全局变量

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
		INT32S tmp[7];
		FP32   tmpf[6];

		tmp[0] = g_Ini.lDestCount;
		tmp[1] = g_Ini.lRunCount;
		tmp[2] = g_Ini.lProbeCount;
		tmp[3] = g_Ini.lRunSpeed[0];
		tmp[4] = g_Ini.lRunSpeed[1];
		tmp[5] = g_Ini.lRunSpeed[2];
		tmp[6] = g_Ini.lRunSpeed[3];

		tmpf[0] = g_Ini.fStartOffset[0];
		tmpf[1] = g_Ini.fStartOffset[2];
		tmpf[2] = g_Ini.fStartOffset[3];
		tmpf[3] = g_Ini.fStartOffset[4];
		tmpf[4] = g_Ini.fStartOffset[5];

		memcpy(filebuf+ibufcount,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);

		memcpy(filebuf+ibufcount,tmpf,sizeof(tmpf));
		ibufcount += sizeof(tmpf);

	}

	res = f_lseek(&fd,0);
	res = f_write(&fd,filebuf,ibufcount,&bw);

	f_close(&fd) ;
}


BOOL	ReadFileMessage(INT16S File ,INT8S *message )
{
//	FS_FILE 	*fp;
	FIL 		fd;
	FRESULT 	res;
	UINT		br;
	INT16S 		i=0;//,k=0,size=0,j=0,l=0;
	//INT8S 		buf[9]="\0";
	INT8S 		buffer[0x10]="THJ000.DAT";
	INT8S 		tmpbuf[0x20]="\\PROG\\"; 
	INT8S 		filemessage[0x40]="\0";
	INT32S 		ibufcount = 0;
	//COMMAND		*ptr;
	
	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';
	
	for(i=0; i<MAXLINE; i++)
		DataCls(i);
		
	Uart_Printf("ReedFileMessage\n");
	Line_Data_Size = 0;
	
//	strcat(tmpbuf,buffer);
//
//	fp=FS_FOpen(tmpbuf,"r");


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

	strcpy(message,filemessage);
	
	f_close(&fd) ;

	OSSemPost(FileSem);

	return TRUE;
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
		INT32S tmp[7];
		FP32   tmpf[6];

		res = f_lseek(&fd,ibufcount);
		res = f_read(&fd,tmp,sizeof(tmp),&br);
		ibufcount += sizeof(tmp);


		g_Ini.lDestCount= tmp[0];
		g_Ini.lRunCount = tmp[1];
		g_Ini.lProbeCount= tmp[2];

		g_Ini.lRunSpeed[0] = tmp[3];
		g_Ini.lRunSpeed[1] = tmp[4];
		g_Ini.lRunSpeed[2] = tmp[5];
		g_Ini.lRunSpeed[3] = tmp[6];

		res = f_lseek(&fd,ibufcount);
		res = f_read(&fd,tmpf,sizeof(tmpf),&br);
		ibufcount += sizeof(tmp);

		if(fabs(tmpf[0]-g_Ini.fStartOffset[0])>0.000001)
		{
			//g_bFirstResetX = FALSE;
			g_Ini.fStartOffset[0] = tmpf[0];
		}

		if(fabs(tmpf[1]-g_Ini.fStartOffset[2])>0.000001)
		{
			//g_bFirstResetZ = FALSE;
			g_Ini.fStartOffset[2] = tmpf[1];
		}

		if(fabs(tmpf[2]-g_Ini.fStartOffset[3])>0.000001)
		{
			//g_bFirstResetU = FALSE;
			g_Ini.fStartOffset[3] = tmpf[2];
		}

		if(fabs(tmpf[3]-g_Ini.fStartOffset[4])>0.000001)
		{
			//g_bFirstResetV = FALSE;
			g_Ini.fStartOffset[4] = tmpf[3];
		}

		if(fabs(tmpf[4]-g_Ini.fStartOffset[5])>0.000001)
		{
			//g_bFirstResetW = FALSE;
			g_Ini.fStartOffset[5] = tmpf[4];
		}
	}


	f_close(&fd) ;

	return TRUE;
}


void	SysParaInit()
{
	INT16S i=0;

	for(i=0;i<MAXCH;i++)
	{
		g_Ini.lPulse[i]=7200;
		g_Ini.fMM[i]=360.0;
		g_Ini.lMaxSpeed[i]=2500;
		g_Ini.lHandSpeed[i]=50;
		g_Ini.fBackSpeed[i]=60;
		g_Ini.lGearRate[i]=1.0;
		g_Ini.iRunDir[i]=0;
		g_Ini.iBackDir[i]=0;
		g_Ini.iAxisSwitch[i]=1;
		g_Ini.iLag[i] = 0;
		g_Ini.iNumerator[i]=1;
		g_Ini.iDenominator[i]=1;
		
		g_Ini.iStepRate[i]=2;		
		g_Ini.iBackMode[i] = 0;
		g_Ini.lRunSpeed[i] = 30;
		g_Ini.iStepDis[i]=10;
		//g_Ini.lMM[i]=3600;
		g_Ini.fStartOffset[i]=0.0;
		g_Ini.lChAddSpeed[i]=3000;
		g_Ini.iSearchZero[i]=1;
		g_Ini.iServoLogic[i]=0;
		g_Ini.iAxisRunMode[i]=0;
		g_Ini.iAxisMulRunMode[i]=0;

		
		g_Ini.fMaxLimit[i]=0.;
		g_Ini.fMinLimit[i]=0.;
		
		g_Ini.iAxisNameNo[i]=0;
		g_Ini.iHardLimitSwitch[i]=0;
	}
	
	
	g_Ini.lPulse[0]=7200;
	g_Ini.lPulse[1]=3600;
	g_Ini.lPulse[2]=2000;
	g_Ini.lPulse[3]=2000;

	g_Ini.fMM[2]=10.0;
	g_Ini.fMM[3]=5.0;


	g_Ini.fBackSpeed[2]=30;
	g_Ini.fBackSpeed[3]=30;

	g_Ini.iBackDir[1]=1;
	g_Ini.iBackDir[2]=1;
	g_Ini.iBackDir[3]=1;

	g_Ini.lGearRate[0]=6.0;

	g_Ini.iSearchZero[0]=1;
	g_Ini.iSearchZero[1]=2;
	g_Ini.iSearchZero[2]=3;

	g_Ini.iAxisRunMode[1]=1;
	g_Ini.iAxisRunMode[2]=2;
	g_Ini.iAxisRunMode[3]=2;

	g_Ini.iBackMode[0] = 1;
	g_Ini.iBackMode[1] = 1;
	g_Ini.iBackMode[2] = 3;
	g_Ini.iBackMode[3] = 3;

	g_Ini.iHardLimitSwitch[2]=1;

	
	
	
	g_Ini.lRunCount=0;
	g_Ini.lDestCount=100000;
	g_Ini.lProbeCount=1;
	g_Ini.fXMaxSpeed=150.0;
	g_Ini.lPassword=123;
	g_Ini.fStartSpeed=2.0;
	g_Ini.fAddSpeed=2000.0;
	

	g_Ini.iScrProtect=1;
	g_Ini.iScrProtectTime=10;
	g_Ini.iScrProtectBmp=0;	
	g_Ini.iXianjiaDrive=0;
	
	g_Ini.iHandStatus=1;
	g_Ini.iHandRunSpeed=2;
	g_Ini.fHandRunRate=75.0;									//指示手摇加工的脉冲计数的比率	

	g_Ini.iPaoXian=0;
	g_Ini.iWarningOut=0;
	g_Ini.iYDaoXian=1;
	
	g_Ini.iFileName = 000;

	g_Ini.iLanguage = 0;
	g_Ini.iAxisCount = 3;

	g_Ini.iBackOrder[0]=3;
	g_Ini.iBackOrder[1]=2;
	g_Ini.iBackOrder[2]=1;
	g_Ini.iBackOrder[3]=4;
	g_Ini.iBackOrder[4]=5;
	g_Ini.iBackOrder[5]=6;

	g_Ini.iDuanXianLogic = 0;
	g_Ini.iDuanXianNum	 = 0;
	g_Ini.iSongXianNum	 = 0;
	g_Ini.iPaoXianNum    = 0;
	
	g_Ini.fVoltageOne = 0.0;
	g_Ini.fVoltageTwo = 0.0;

	g_Ini.iSecondAxisStyle = 0;
	g_Ini.iEnterNumber = 0;
	g_Ini.iDispLeavingTime = 0;
	
	g_Ini.iColorStyle = 0;
	
	g_Ini.iCyldMode = 1;
	g_Ini.iF1 = 0;
	g_Ini.iLeftOrRight = 0;
	g_Ini.iMachineType = 0;

	g_Ini.iIOSendWire=0;
	
	g_Ini.iAxisCa=0;

	g_Ini.iLimitStopAxis=3;
	
	g_Ini.StopLevel=0;		//急停按钮默认检测高电平
	
	g_Ini.iPasswordTime=0;
	
	g_Ini.iBackStyle=0;

	g_Ini.iSafeDoor=0;
	g_Ini.iDuXianNum=0;
	g_Ini.iProDelay=10000;

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

BOOL	SysParaCheck()
{
	INT16S i=0;
	
	for(i=0;i<MAXCH;i++)
	{
		//系统参数范围检测
		if(!Check_DataLong(g_Ini.lPulse[i],2000L,80000L))
			return FALSE;
			
		if(!Check_DataFloat(g_Ini.fMM[i],0.5,2000.0f))
			return FALSE;	
		
		if(!Check_DataInt(g_Ini.iRunDir[i],0,1))
			return FALSE;
		
		if(!Check_DataInt(g_Ini.iBackDir[i],0,1))
			return FALSE;
		
		if(!Check_DataFloat(g_Ini.lGearRate[i],0.01,1000.0))
			return FALSE;
		
		if(!Check_DataLong(g_Ini.lMaxSpeed[i],100,8000))
			return FALSE;
			
		if(!Check_DataInt(g_Ini.iServoLogic[i],0,1))
			return FALSE;
					
		if(!Check_DataFloat(g_Ini.fBackSpeed[i],0.1,1000.0))
			return FALSE;		
		
		if(!Check_DataInt(g_Ini.iBackMode[i],0,4))
			return FALSE;
			
		if(!Check_DataInt(g_Ini.iAxisMulRunMode[i],0,1))
			return FALSE;	
			
		
		if(!Check_DataInt(g_Ini.iSearchZero[i],0,3))
			return FALSE;		
				
		if(!Check_DataLong(g_Ini.lHandSpeed[i],1,999))
			return FALSE;
			
		if(!Check_DataLong(g_Ini.lChAddSpeed[i],50,10000))
			return FALSE;

		if(!Check_DataLong(g_Ini.iLag[i],0,999999))
			return FALSE;
			
		if(!Check_DataLong(g_Ini.iNumerator[i],0,999999))
			return FALSE;
		
		if(!Check_DataLong(g_Ini.iDenominator[i],-999999,999999))
			return FALSE;
		
//		if(!Check_DataInt(g_Ini.iAxisRunMode[i],0,2))
//			return FALSE;
			
		//////////////////////////////////////
		
		if(!Check_DataInt(g_Ini.iStepRate[i],1,99))
			return FALSE;		
		
		if(!Check_DataLong(g_Ini.lRunSpeed[i],1,5000))
			return FALSE;
			
		if(!Check_DataInt(g_Ini.iStepDis[i],1,5000))
			return FALSE;	
			
		if(!Check_DataFloat(g_Ini.fStartOffset[i],-99999.0,99999.0))
			return FALSE;
		
		if(!Check_DataLong(g_Ini.fMaxLimit[i],-999999,999999))
			return FALSE;
		
		if(!Check_DataLong(g_Ini.fMinLimit[i],-999999,999999))
			return FALSE;
			
		if(!Check_DataInt(g_Ini.iAxisNameNo[i],0,12))
			return FALSE;
			
		if(!Check_DataInt(g_Ini.iHardLimitSwitch[i],0,1))
			return FALSE;	
			
			
		if(!Check_DataInt(g_Ini.iBackOrder[i],1,6))
			return FALSE;
			
	}	
	
	
	if(!Check_DataInt(g_Ini.iAxisRunMode[1],0,3))
		return FALSE;	
	if(!Check_DataInt(g_Ini.iAxisRunMode[2],0,2))
		return FALSE;	
	if(!Check_DataInt(g_Ini.iAxisRunMode[3],0,2))
		return FALSE;		
	//系统参数检测
	if(!Check_DataInt(g_Ini.iAxisCount,2,4))
		return FALSE;
		

	
	if(!Check_DataFloat(g_Ini.fXMaxSpeed,1.0,500.0))
		return FALSE;
			
	if(!Check_DataInt(g_Ini.iSecondAxisStyle,0,1))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iCyldMode,0,1))
		return FALSE;
			
	if(!Check_DataInt(g_Ini.iF1,0,1))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iEnterNumber,0,1))
		return FALSE;
	

	
	if(!Check_DataInt(g_Ini.iHandRunSpeed,1,10))
		return FALSE;
		
	if(!Check_DataFloat(g_Ini.fHandRunRate,1.0,1000.0))
		return FALSE;	
		
	if(!Check_DataLong(g_Ini.lPassword,0L,999999L))
		return FALSE;
		
	//常用参数范围检测
	if(!Check_DataInt(g_Ini.iScrProtect,0,1))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iScrProtectTime,1,100))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iScrProtectBmp,0,1))
		return FALSE;

	if(!Check_DataInt(g_Ini.iXianjiaDrive,0,1))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iWarningOut,0,2))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iPaoXian,0,1))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iYDaoXian,0,1))
		return FALSE;
	
	
	/*
	if(!Check_DataLong(g_Ini.lRunCount,0L,999999L))
		return FALSE;
		
	if(!Check_DataLong(g_Ini.lDestCount,0L,999999L))
		return FALSE;
	
	if(!Check_DataLong(g_Ini.lProbeCount,0L,99999L))
		return FALSE;
	*/	
		
		
	if(!Check_DataFloat(g_Ini.fStartSpeed,1.0,1000.0))
		return FALSE;
		
	if(!Check_DataFloat(g_Ini.fAddSpeed,1.0,100000.0))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iHandStatus,0,1))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iLanguage,0,1))
		return FALSE;	

	if(!Check_DataInt(g_Ini.iDuanXianLogic,0,1))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iDuanXianNum,0,50))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iSongXianNum,0,50))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iPaoXianNum,0,50))
		return FALSE;

	if(!Check_DataInt(g_Ini.iSafeDoor,0,1))
		return FALSE;

	if(!Check_DataLong(g_Ini.iDuXianNum,0L,999999L))
		return FALSE;
	/*
	if(!Check_DataFloat(g_Ini.fVoltageOne,0.00,10.00))
		return FALSE;
		
	if(!Check_DataFloat(g_Ini.fVoltageTwo,0.00,10.00))
		return FALSE;
	*/		
			
	if(!Check_DataInt(g_Ini.iDispLeavingTime,0,1))
		return FALSE;
			
	if(!Check_DataInt(g_Ini.iColorStyle,0,2))
		return FALSE;
	
	if(!Check_DataInt(g_Ini.iLeftOrRight,0,1))
		return FALSE;	

	if(!Check_DataInt(g_Ini.iMachineType,0,4))
		return FALSE;
			
	if(!Check_DataInt(g_Ini.iIOSendWire,0,1))
		return FALSE;
				
				
	if(!Check_DataInt(g_Ini.iAxisCa,0,1))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.iLimitStopAxis,0,4))
		return FALSE;
		
	if(!Check_DataInt(g_Ini.StopLevel,0,1))//急停按钮默认检测低电平
		return FALSE;	
		
	if(!Check_DataInt(g_Ini.iPasswordTime,0,1000))
		return FALSE;
		
				
	if(!Check_DataInt(g_Ini.iBackStyle,0,1))
		return FALSE;
				
	if(!Check_DataFloat(g_Ini.iProDelay,0,99999))
		return FALSE;

	return  TRUE;
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



//#pragma warn -par
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
void	SetUnitData(INT32S nL,INT8U nC)
{
	INT16S i;
	
	g_UnitData.nLine=nL;
	g_UnitData.nCode=nC;
	g_UnitData.SpindleAxis = 1;
	
	for(i=0;i<=MAXCH;i++)
		g_UnitData.lValue[i]=0L;
	
	//g_UnitData.lValue[4]=1L;

	g_UnitData.nIO=0;
	
	for(i=0;i<MAXCYLINDER;i++)
		g_UnitData.nOut[i]=2;
		
	g_UnitData.fDelay = 0;
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


//INT16U	ReadPortb(INT16U port)													//读铁电，铁电总大小:32K
//{
//	return *((volatile unsigned char *)(0x28000004+port));
//}


//void	WritePortb(INT16U port,INT32U data)										//写铁电
//{
//	*((volatile unsigned char *)(0x28000004+port)) = data;
//}


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
INT16S YaffsFileExist(INT16S File)								//判断料号为File的文件是否存在
{
	INT8S 		tmpbuf[0x20]="/flash/PROG/"; 						
	INT8S 		buffer[0x10]="\0"; 
	int 	  	fr;
	
	strcpy(buffer,"THJ000.DAT");
	buffer[3] = File / 100+'0';
	buffer[4] = (File%100) / 10+'0';
	buffer[5] = File % 10+'0';

	strcat(tmpbuf,buffer);  
	
	fr= yaffs_open(tmpbuf,O_RDONLY , S_IREAD );

	if(fr == -1)
	{
	  	yaffs_close(fr);
		return 0;
	}
	else		
	{
	  	yaffs_close(fr);
		return 1;
	}     
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
	
	INT32S 		ibufcount = 0;
	
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
		INT32S tmp[7];
		FP32   tmpf[6];
		
		tmp[0] = g_Ini.lDestCount;
		tmp[1] = g_Ini.lRunCount;
		tmp[2] = g_Ini.lProbeCount;
		tmp[3] = g_Ini.lRunSpeed[0];
		tmp[4] = g_Ini.lRunSpeed[1];
		tmp[5] = g_Ini.lRunSpeed[2];
		tmp[6] = g_Ini.lRunSpeed[3];
		
		tmpf[0] = g_Ini.fStartOffset[0];
		tmpf[1] = g_Ini.fStartOffset[2];
		tmpf[2] = g_Ini.fStartOffset[3];
		tmpf[3] = g_Ini.fStartOffset[4];
		tmpf[4] = g_Ini.fStartOffset[5];
		
		
		
		memcpy(filebuf+ibufcount,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);
		
//		memcpy(filebuf+ibufcount,tmpf,sizeof(tmpf));//起点不随文件保存
//		ibufcount += sizeof(tmpf);

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
	
	INT32S 		ibufcount = 0;

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
		INT32S tmp[7];
		FP32   tmpf[6];
		
		yaffs_lseek(fr,ibufcount,SEEK_SET);
		yaffs_read(fr,tmp,sizeof(tmp));
		ibufcount += sizeof(tmp);


	
		g_Ini.lDestCount= tmp[0];
		g_Ini.lRunCount = tmp[1];
		g_Ini.lProbeCount= tmp[2];
		
		g_Ini.lRunSpeed[0] = tmp[3];
		g_Ini.lRunSpeed[1] = tmp[4];
		g_Ini.lRunSpeed[2] = tmp[5];
		g_Ini.lRunSpeed[3] = tmp[6];
		
/*		
		yaffs_lseek(fr,ibufcount,SEEK_SET);
		yaffs_read(fr,tmpf,sizeof(tmpf));
		ibufcount += sizeof(tmpf);
		
		if(fabs(tmpf[0]-g_Ini.fStartOffset[0])>0.000001)
		{
			//g_bFirstResetX = FALSE;
			g_Ini.fStartOffset[0] = tmpf[0];
		}
		
		if(fabs(tmpf[1]-g_Ini.fStartOffset[2])>0.000001)
		{
			//g_bFirstResetZ = FALSE;
			g_Ini.fStartOffset[2] = tmpf[1];
		}
		
		if(fabs(tmpf[2]-g_Ini.fStartOffset[3])>0.000001)
		{
			//g_bFirstResetU = FALSE;
			g_Ini.fStartOffset[3] = tmpf[2];
		}
		
		if(fabs(tmpf[3]-g_Ini.fStartOffset[4])>0.000001)
		{
			//g_bFirstResetV = FALSE;
			g_Ini.fStartOffset[4] = tmpf[3];
		}
		
		if(fabs(tmpf[4]-g_Ini.fStartOffset[5])>0.000001)
		{
			//g_bFirstResetW = FALSE;
			g_Ini.fStartOffset[5] = tmpf[4];
		}
		*/
	}
		
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



void	ReadRunCountYaffs()
{
	#if OS_CRITICAL_METHOD == 3 
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;             
	#endif  
	
	if(g_bRam)
	{
		INT32S da1,da2,da3,da4;
		
		OS_ENTER_CRITICAL();
		
		da1=(INT32S)(ReadPortb(0x104));
		da2=(INT32S)(ReadPortb(0x105));
		da3=(INT32S)(ReadPortb(0x106));
		da4=(INT32S)(ReadPortb(0x107));
		
		{
			INT32S tmp1,tmp2,tmp3;
			
			tmp1=da4<<24;
			tmp2=da3<<16;
			tmp3=da2<<8;
			
			g_Ini.lRunCount=tmp1+tmp2+tmp3+da1;
		}
		
		if(g_Ini.lRunCount>9999999)
			g_Ini.lRunCount=9999999;
			
		if(g_Ini.lRunCount<0)
			g_Ini.lRunCount=0;
			
		OS_EXIT_CRITICAL();
	}
	else
	{
		int  		fr;
	//	INT8U 		err;

		INT8S 		tmpbuf[0x20]="/flash/runcount.ini";

		fr=yaffs_open(tmpbuf,O_RDONLY , S_IREAD | S_IWRITE);

		if( fr == -1 )
		{
			Uart_Printf("yaffs区未格式化!!!\n");
			yaffs_close(fr);
			return ;
		}	

		yaffs_lseek(fr,0,SEEK_SET);

		yaffs_read(fr,&g_Ini.lRunCount,sizeof(g_Ini.lRunCount));

		yaffs_close(fr);

		return ;
		
	}
}


void	WriteRunCountYaffs()
{
	#if OS_CRITICAL_METHOD == 3 
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;             
	#endif  
	
	if(g_bRam)
	{
		INT32U da1,da2,da3,da4;

		OS_ENTER_CRITICAL();
		
		da1=(g_Ini.lRunCount>>24&0xff);
		da2=((g_Ini.lRunCount & 0x00ff0000)>>16);
		da3=((g_Ini.lRunCount & 0x0000ff00)>>8);
		da4=(g_Ini.lRunCount & 0x000000ff);
		
		WritePortb(0x104,(INT8U)da4);
		WritePortb(0x105,(INT8U)da3);
		WritePortb(0x106,(INT8U)da2);
		WritePortb(0x107,(INT8U)da1);
		
		OS_EXIT_CRITICAL();
	}
	else
	{
		
		int  		fw;
		INT8U 		err;

		INT8S 		tmpbuf[0x20]="/flash/runcount.ini";
		
		fw = yaffs_open(tmpbuf,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

		if( fw == -1 )
		{
			Uart_Printf("yaffs区未格式化!!!\n");
			goto Err;
		}	
		
		yaffs_lseek(fw,0,SEEK_SET);

		err=yaffs_write(fw,&g_Ini.lRunCount,sizeof(g_Ini.lRunCount));
		
	Err:
		yaffs_close(fw);
	
	}
}
#endif

