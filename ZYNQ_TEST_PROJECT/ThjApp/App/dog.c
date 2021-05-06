#include	"includes.h"

extern		INT16S		SVGAmode;

TEST_EXT	BOOL		g_bEnglish;
//TEST_EXT	INT32S 		g_TextColor2;     										//黄
WORK_EXT    BOOL 		g_bScreen;
NEWDOG		NewDog;

#define		DOGSECTORSTART	SECTORSTART-5*32  									//U盘扇区起始块之前5个块可以作为DOG数据存放的位置
#define		MAXPASSDAY		(365*2)												//前一记录日期与当前日期的最大差值限制
#define		WR24C080OFFSET		(1024)

INT32U	InitDog(void)
{
	NewDog.Dog_InitData = 886688;												//狗的初始化
	NewDog.First_Use 	= 123456;  												//是否为第一次开启控制器,用于狗的初始化

	NewDog.Adtech_ComputerNo = 123456;											//1)	公司级客户电脑编号
	NewDog.Adtech_Days = 0;														//2)	公司级限定天数
	NewDog.Adtech_Password = 123456;											//3)	公司级软件狗设定登入密码

	NewDog.UserDogIdentifier = 8001;											//4)	用户级软件狗标别码
	
	NewDog.User_ComputerNo = 123;												//5)	用户级客户机器编号h
	NewDog.User_Days = 0;														//6)	用户级限定天数
	NewDog.User_Password = 1234;												//7)	用户级软件狗设定登入密码

	NewDog.Adtech_Active = 12345678;  											//指示是否解除公司级密码限制,12345678解除限制
	RtcGetDate(&NewDog.Adtech_LastDay); 										//公司级最后一次访问软件狗的日期
	RtcGetTime(&NewDog.Adtech_NowTime); 										//记录用户级访问软件狗的当前时间

	NewDog.User_Active =12345678; 												//指示是否解除用户级密码限制,12345678解除限制
	RtcGetDate(&NewDog.User_LastDay); 											//用户级最后一次访问软件狗的日期
	RtcGetTime(&NewDog.User_NowTime); 											//记录用户级访问软件狗的当前时间

	return TRUE;
}



#define		DOGDATA_ADDR		256

INT32U  WriteDog(void)
{
	/*
		unsigned int i;//,j,save_E,save_PE;
	    U8	tmp[256]="\0";
	    memcpy(tmp,&NewDog,sizeof(NewDog));

	    for(i=0;i<sizeof(NewDog);i++)
	        _Wr24C080(0xa4+WR24C080OFFSET,(U8)i,tmp[i]);
	 */

		U8	tmp[256]="\0";

		memcpy(tmp,&NewDog,sizeof(NewDog));
		FRAM_Write(tmp,DOGDATA_ADDR, sizeof(NewDog));

		return 1;
}


INT32U  ReadDog(void)
{
	/*
		unsigned int 	i;//,save_E,save_PE;
	    U8				tmp[256];

	    for(i=0;i<sizeof(NewDog);i++)
	        _Rd24C080(0xa4+WR24C080OFFSET,(U8)i,&tmp[i]);

	    memcpy(&NewDog,tmp,sizeof(NewDog));

	    return 1;
	*/

		U8	tmp[256]="\0";

		FRAM_Read (tmp,DOGDATA_ADDR, sizeof(NewDog));
	    memcpy(&NewDog,tmp,sizeof(NewDog));

	    return 1;
}


void  CheckPassword()
{

	BOOL 	Set1=TRUE,Set2=TRUE;
	DATE_T 	today;
	TIME_T 	now;
	INT32S 	NewDay = 0;
	char 	buf[0x40];
	char 	bufE[0x40];
	BOOL 	sta=FALSE;

	#if OS_CRITICAL_METHOD == 3 
		OS_CPU_SR  cpu_sr;
	#endif
	
	Stop1(1);
	Stop1(2);
	Stop1(3);
	Stop1(4);
	
	OSTimeDly(100);
	OS_ENTER_CRITICAL();
	sta=ReadDog();
	OS_EXIT_CRITICAL();
	
/*	if(sta==0)
	{
		CMsgDlg_Init(200,200,200,120,g_bEnglish?"Dog Data Error!":"狗数据错误!!!",g_bEnglish?"Warning":"警告",
					YELLOWCOLOR,g_bEnglish?"Ok":"确定",g_bEnglish?"Cancel":"取消");
		CMsgDlg_DoModal();
		CMsgDlg_Destroy();
		
		while(1);
	}*/
	
	if(NewDog.Dog_InitData != 886688)
	{
		InitDog();
		WriteDog();
	}

	if(NewDog.First_Use != 123456)									//如果上一次使用机器时修改了加密狗
	{
		NewDog.First_Use = 123456;
		RtcGetDate(&NewDog.User_LastDay); 							//用户级最后一次访问软件狗的日期
		RtcGetDate(&NewDog.Adtech_LastDay); 						//公司级最后一次访问软件狗的日期
		//Uart_Printf("test0=%d\n",GetDays(NewDog.User_LastDay));
		WriteDog();
	}
	
	if(NewDog.User_Active==12345678)								//表示未加密
		Set1=FALSE;
	
	if(NewDog.Adtech_Active==12345678)								//表示未加密
		Set2=FALSE;
	
	if(NewDog.User_Days>9999)NewDog.User_Days=9999;
	
	if(NewDog.Adtech_Days>9999)NewDog.Adtech_Days=9999;
	
	RtcGetDate(&today);
	RtcGetTime(&now);
	//Uart_Printf("test1=%d\n",GetDays(today));
	if(Set1)														//用户级加密狗算法流程
	{

		INT32S date1,date2,passday;			
		date1=GetDays(today);
		date2=GetDays(NewDog.User_LastDay);
		passday=date1-date2;
		//Uart_Printf("test2=%d,test3=%d,test4=%d\n",date1,date2,passday);
	
		if(passday>0)
		{
			if(passday>MAXPASSDAY)									//本次开机相对于上次开机天数已经大于用户限制天数，日期时间丢失重新复位会出现此现象
				NewDog.User_Days-=1;
			else
			{
				if(passday>=NewDog.User_Days)
				{
					NewDog.User_Days=0;
				}
				else
					NewDog.User_Days-=passday;
			}
		}
		else														//当前一天内反复开机
		{
			if(passday==0 
				&& NewDog.User_LastDay.year==today.year
				&& NewDog.User_LastDay.month==today.month
				&& NewDog.User_LastDay.day==today.day)
			{
																
				if(GetTimes(now)<=GetTimes(NewDog.User_NowTime))		//时间被更改提前则限制天数自动减1
					NewDog.User_Days--;
			}
			else													//日期被更改提前则试用天数自动减1,同时记录当前日期
			{
				NewDog.User_Days--;
			}
		}
		
		NewDog.User_LastDay	= today; 								//记录本次开机对应的日期
		NewDog.User_NowTime = now;									//记录本次开机对应的时间
		
		
		if(NewDog.User_Days<=5)
		{
			while(1)
			{
				char da[10];

				RtcGetDateStr(da);
				
				sprintf(buf," 日期:%s  \n 软件狗识别码:%d  \n 用户机器编号:%d",da,NewDog.UserDogIdentifier,NewDog.User_ComputerNo);
				sprintf(bufE," DATA:%s  \n UserNo.:%d		\n MacSerial:%d",da,NewDog.UserDogIdentifier,NewDog.User_ComputerNo);
			
				if(NewDog.User_Days>0)
					CPassword_Init(200,150,360,250,g_bEnglish?"User-Probation Ending":"试用期快满请输入用户密码",g_bEnglish?bufE:buf,g_TextColor2);
				else
					CPassword_Init(200,150,360,250,g_bEnglish?"User-Probation Ended":"试用期已满请输入用户密码",g_bEnglish?bufE:buf,g_TextColor2);

				if(CPassword_DoModal())
				{
					INT32S 	pass;
					//INT32S 	UserDogIdentifier = 0;
					INT32S 	User_ComputerNo =0;
					INT32S 	ldate = 0;
					INT32S	isetl = 0;								//存低1-10位 并移到高11-20位
					INT32S	iseth = 0;								//存高11-20位并移到低1-10位
					
					pass=CPassword_GetPassword();
					
					CPassword_Destroy();

					isetl = pass & 0x3ff;
					
					isetl = isetl<<10;
					
					iseth = pass>>10;
					
					iseth = iseth & 0x3ff;
					
					pass = pass>>20;
					
					pass = pass<<20;
					
					pass = pass | iseth;
					
					pass = pass | isetl;


					ldate = today.year*365+today.month*30+today.day;
					
					NewDay = pass  &  0x3ff ;						// 1-10 1024 值用来存日期

					User_ComputerNo = pass>>10;
					User_ComputerNo = User_ComputerNo - NewDog.UserDogIdentifier - ldate;
					
						
					Uart_Printf("pass=%ld\n,NewDay=%ld\n,User_ComputerNo =%ld\n,ldate=%ld \n",pass,NewDay,User_ComputerNo,ldate)	;			
			
					if( User_ComputerNo == NewDog.User_ComputerNo % 1000000)
					{
			   			if(NewDay!=0)								//在解密码时同时在进行密码日期限定
						{
							NewDog.User_Days=NewDay;				
						}
						else
						{
							NewDog.User_Active=12345678; 			//解除密码限制
							NewDog.User_Days=0;
						}
						
						break;
					}
					else
						continue;
				}
				else
				{
					CPassword_Destroy();	
					
					if(NewDog.User_Days>0)
						break;				 
				}
			}														//endif while(1)		
		}															//endif(Dog.User_Days<=0)

		
		OS_ENTER_CRITICAL();		
		WriteDog();
		OS_EXIT_CRITICAL();
		
	}																//if(Set1)
	
///////////////////////////////////////////////////////////////	
	
	if(Set2)
	{
		INT32S date1,date2,passday;	
				
		date1 = GetDays(today);
		date2 = GetDays(NewDog.Adtech_LastDay);
		passday = date1-date2;
	
		if(passday>0)
		{
			if(passday>MAXPASSDAY)									//本次开机相对于上次开机天数已经大于用户限制天数，日期时间丢失重新复位会出现此现象
				NewDog.Adtech_Days-=1;
			else
			{
				if(passday>=NewDog.Adtech_Days)
				{
					NewDog.Adtech_Days=0;
				}
				else
					NewDog.Adtech_Days-=passday;
			}
		}
		else														//当前一天内反复开机
		{
			if(passday==0 
				&& NewDog.Adtech_LastDay.year==today.year
				&& NewDog.Adtech_LastDay.month==today.month
				&& NewDog.Adtech_LastDay.day==today.day)
			{
																
				if(GetTimes(now)<GetTimes(NewDog.Adtech_NowTime))	//时间被更改提前则限制天数自动减1
					NewDog.Adtech_Days--;
			}
			else													//日期被更改提前则试用天数自动减1,同时记录当前日期
			{
				NewDog.Adtech_Days--;
			}
		}
		
		NewDog.Adtech_LastDay = today; 								//记录本次开机对应的日期
		NewDog.Adtech_NowTime = now;								//记录本次开机对应的时间
	
		if(NewDog.Adtech_Days<=5)
		{
			while(1)
			{
				char da[10];

				RtcGetDateStr(da);
				
				sprintf(buf," 日期:%s  \n 公司级电脑编号:%d\n",da,NewDog.Adtech_ComputerNo);
				sprintf(bufE," DATA:%s  \n AdtechSerial:%d",da,NewDog.User_ComputerNo);
			
				if(NewDog.Adtech_Days>0)
					CPassword_Init(200,150,360,250,g_bEnglish?"Firm-Probation Ending":"试用期快满请输入公司级密码",g_bEnglish?bufE:buf,g_TextColor2);
				else
					CPassword_Init(200,150,360,250,g_bEnglish?"Firm-Probation Ended":"试用期已满请输入公司级密码",g_bEnglish?bufE:buf,g_TextColor2);

				if(CPassword_DoModal())
				{
					INT32S 	pass;
					INT32S 	Adtech_ComputerNo =0;
					INT32S 	ldate = 0;
					INT32S	isetl = 0;								//存低1-10位 并移到高11-20位
					INT32S	iseth = 0;								//存高11-20位并移到低1-10位
					
					pass=CPassword_GetPassword();
					
					CPassword_Destroy();

					isetl = pass & 0x3ff;
					
					isetl = isetl<<10;
					
					iseth = pass>>10;
					
					iseth = iseth & 0x3ff;
					
					pass = pass>>20;
					
					pass = pass<<20;
					
					pass = pass | iseth;
					
					pass = pass | isetl;


					ldate = today.year*365+today.month*30+today.day;
					
					NewDay = pass  &  0x3ff ;						// 1-10 1024 值用来存新的使用日期

					Adtech_ComputerNo = pass>>10;
					Adtech_ComputerNo = Adtech_ComputerNo - 284597 - ldate;
						
					Uart_Printf("pass=%ld\n,NewDay=%ld\n,Adtech_ComputerNo =%ld\n,ldate=%ld \n",pass,NewDay,Adtech_ComputerNo,ldate)	;			
			
					if( Adtech_ComputerNo == NewDog.Adtech_ComputerNo % 1000000)
					{
			   			if(NewDay!=0)								//在解密码时同时在进行密码日期限定
						{
							NewDog.Adtech_Days=NewDay;				
						}
						else
						{
							NewDog.Adtech_Active=12345678; 			//解除密码限制
							NewDog.Adtech_Days=0;
						}
						
						break;
					}
					else
						continue;
				}
				else
				{
					CPassword_Destroy();	
					
					if(NewDog.Adtech_Days>0)
						break;				 
				}
			}														//endif while(1)		
		}															//endif(Dog.User_Days<=5)

		OS_ENTER_CRITICAL();		
		WriteDog();
		OS_EXIT_CRITICAL();

	}																// end Set2
}


