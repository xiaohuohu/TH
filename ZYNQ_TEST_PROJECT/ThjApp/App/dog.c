#include	"includes.h"

extern		INT16S		SVGAmode;

TEST_EXT	BOOL		g_bEnglish;
//TEST_EXT	INT32S 		g_TextColor2;     										//��
WORK_EXT    BOOL 		g_bScreen;
NEWDOG		NewDog;

#define		DOGSECTORSTART	SECTORSTART-5*32  									//U��������ʼ��֮ǰ5���������ΪDOG���ݴ�ŵ�λ��
#define		MAXPASSDAY		(365*2)												//ǰһ��¼�����뵱ǰ���ڵ�����ֵ����
#define		WR24C080OFFSET		(1024)

INT32U	InitDog(void)
{
	NewDog.Dog_InitData = 886688;												//���ĳ�ʼ��
	NewDog.First_Use 	= 123456;  												//�Ƿ�Ϊ��һ�ο���������,���ڹ��ĳ�ʼ��

	NewDog.Adtech_ComputerNo = 123456;											//1)	��˾���ͻ����Ա��
	NewDog.Adtech_Days = 0;														//2)	��˾���޶�����
	NewDog.Adtech_Password = 123456;											//3)	��˾��������趨��������

	NewDog.UserDogIdentifier = 8001;											//4)	�û�������������
	
	NewDog.User_ComputerNo = 123;												//5)	�û����ͻ��������h
	NewDog.User_Days = 0;														//6)	�û����޶�����
	NewDog.User_Password = 1234;												//7)	�û���������趨��������

	NewDog.Adtech_Active = 12345678;  											//ָʾ�Ƿ�����˾����������,12345678�������
	RtcGetDate(&NewDog.Adtech_LastDay); 										//��˾�����һ�η��������������
	RtcGetTime(&NewDog.Adtech_NowTime); 										//��¼�û�������������ĵ�ǰʱ��

	NewDog.User_Active =12345678; 												//ָʾ�Ƿ����û�����������,12345678�������
	RtcGetDate(&NewDog.User_LastDay); 											//�û������һ�η��������������
	RtcGetTime(&NewDog.User_NowTime); 											//��¼�û�������������ĵ�ǰʱ��

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
		CMsgDlg_Init(200,200,200,120,g_bEnglish?"Dog Data Error!":"�����ݴ���!!!",g_bEnglish?"Warning":"����",
					YELLOWCOLOR,g_bEnglish?"Ok":"ȷ��",g_bEnglish?"Cancel":"ȡ��");
		CMsgDlg_DoModal();
		CMsgDlg_Destroy();
		
		while(1);
	}*/
	
	if(NewDog.Dog_InitData != 886688)
	{
		InitDog();
		WriteDog();
	}

	if(NewDog.First_Use != 123456)									//�����һ��ʹ�û���ʱ�޸��˼��ܹ�
	{
		NewDog.First_Use = 123456;
		RtcGetDate(&NewDog.User_LastDay); 							//�û������һ�η��������������
		RtcGetDate(&NewDog.Adtech_LastDay); 						//��˾�����һ�η��������������
		//Uart_Printf("test0=%d\n",GetDays(NewDog.User_LastDay));
		WriteDog();
	}
	
	if(NewDog.User_Active==12345678)								//��ʾδ����
		Set1=FALSE;
	
	if(NewDog.Adtech_Active==12345678)								//��ʾδ����
		Set2=FALSE;
	
	if(NewDog.User_Days>9999)NewDog.User_Days=9999;
	
	if(NewDog.Adtech_Days>9999)NewDog.Adtech_Days=9999;
	
	RtcGetDate(&today);
	RtcGetTime(&now);
	//Uart_Printf("test1=%d\n",GetDays(today));
	if(Set1)														//�û������ܹ��㷨����
	{

		INT32S date1,date2,passday;			
		date1=GetDays(today);
		date2=GetDays(NewDog.User_LastDay);
		passday=date1-date2;
		//Uart_Printf("test2=%d,test3=%d,test4=%d\n",date1,date2,passday);
	
		if(passday>0)
		{
			if(passday>MAXPASSDAY)									//���ο���������ϴο��������Ѿ������û���������������ʱ�䶪ʧ���¸�λ����ִ�����
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
		else														//��ǰһ���ڷ�������
		{
			if(passday==0 
				&& NewDog.User_LastDay.year==today.year
				&& NewDog.User_LastDay.month==today.month
				&& NewDog.User_LastDay.day==today.day)
			{
																
				if(GetTimes(now)<=GetTimes(NewDog.User_NowTime))		//ʱ�䱻������ǰ�����������Զ���1
					NewDog.User_Days--;
			}
			else													//���ڱ�������ǰ�����������Զ���1,ͬʱ��¼��ǰ����
			{
				NewDog.User_Days--;
			}
		}
		
		NewDog.User_LastDay	= today; 								//��¼���ο�����Ӧ������
		NewDog.User_NowTime = now;									//��¼���ο�����Ӧ��ʱ��
		
		
		if(NewDog.User_Days<=5)
		{
			while(1)
			{
				char da[10];

				RtcGetDateStr(da);
				
				sprintf(buf," ����:%s  \n �����ʶ����:%d  \n �û��������:%d",da,NewDog.UserDogIdentifier,NewDog.User_ComputerNo);
				sprintf(bufE," DATA:%s  \n UserNo.:%d		\n MacSerial:%d",da,NewDog.UserDogIdentifier,NewDog.User_ComputerNo);
			
				if(NewDog.User_Days>0)
					CPassword_Init(200,150,360,250,g_bEnglish?"User-Probation Ending":"�����ڿ����������û�����",g_bEnglish?bufE:buf,g_TextColor2);
				else
					CPassword_Init(200,150,360,250,g_bEnglish?"User-Probation Ended":"�����������������û�����",g_bEnglish?bufE:buf,g_TextColor2);

				if(CPassword_DoModal())
				{
					INT32S 	pass;
					//INT32S 	UserDogIdentifier = 0;
					INT32S 	User_ComputerNo =0;
					INT32S 	ldate = 0;
					INT32S	isetl = 0;								//���1-10λ ���Ƶ���11-20λ
					INT32S	iseth = 0;								//���11-20λ���Ƶ���1-10λ
					
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
					
					NewDay = pass  &  0x3ff ;						// 1-10 1024 ֵ����������

					User_ComputerNo = pass>>10;
					User_ComputerNo = User_ComputerNo - NewDog.UserDogIdentifier - ldate;
					
						
					Uart_Printf("pass=%ld\n,NewDay=%ld\n,User_ComputerNo =%ld\n,ldate=%ld \n",pass,NewDay,User_ComputerNo,ldate)	;			
			
					if( User_ComputerNo == NewDog.User_ComputerNo % 1000000)
					{
			   			if(NewDay!=0)								//�ڽ�����ʱͬʱ�ڽ������������޶�
						{
							NewDog.User_Days=NewDay;				
						}
						else
						{
							NewDog.User_Active=12345678; 			//�����������
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
			if(passday>MAXPASSDAY)									//���ο���������ϴο��������Ѿ������û���������������ʱ�䶪ʧ���¸�λ����ִ�����
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
		else														//��ǰһ���ڷ�������
		{
			if(passday==0 
				&& NewDog.Adtech_LastDay.year==today.year
				&& NewDog.Adtech_LastDay.month==today.month
				&& NewDog.Adtech_LastDay.day==today.day)
			{
																
				if(GetTimes(now)<GetTimes(NewDog.Adtech_NowTime))	//ʱ�䱻������ǰ�����������Զ���1
					NewDog.Adtech_Days--;
			}
			else													//���ڱ�������ǰ�����������Զ���1,ͬʱ��¼��ǰ����
			{
				NewDog.Adtech_Days--;
			}
		}
		
		NewDog.Adtech_LastDay = today; 								//��¼���ο�����Ӧ������
		NewDog.Adtech_NowTime = now;								//��¼���ο�����Ӧ��ʱ��
	
		if(NewDog.Adtech_Days<=5)
		{
			while(1)
			{
				char da[10];

				RtcGetDateStr(da);
				
				sprintf(buf," ����:%s  \n ��˾�����Ա��:%d\n",da,NewDog.Adtech_ComputerNo);
				sprintf(bufE," DATA:%s  \n AdtechSerial:%d",da,NewDog.User_ComputerNo);
			
				if(NewDog.Adtech_Days>0)
					CPassword_Init(200,150,360,250,g_bEnglish?"Firm-Probation Ending":"�����ڿ��������빫˾������",g_bEnglish?bufE:buf,g_TextColor2);
				else
					CPassword_Init(200,150,360,250,g_bEnglish?"Firm-Probation Ended":"���������������빫˾������",g_bEnglish?bufE:buf,g_TextColor2);

				if(CPassword_DoModal())
				{
					INT32S 	pass;
					INT32S 	Adtech_ComputerNo =0;
					INT32S 	ldate = 0;
					INT32S	isetl = 0;								//���1-10λ ���Ƶ���11-20λ
					INT32S	iseth = 0;								//���11-20λ���Ƶ���1-10λ
					
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
					
					NewDay = pass  &  0x3ff ;						// 1-10 1024 ֵ�������µ�ʹ������

					Adtech_ComputerNo = pass>>10;
					Adtech_ComputerNo = Adtech_ComputerNo - 284597 - ldate;
						
					Uart_Printf("pass=%ld\n,NewDay=%ld\n,Adtech_ComputerNo =%ld\n,ldate=%ld \n",pass,NewDay,Adtech_ComputerNo,ldate)	;			
			
					if( Adtech_ComputerNo == NewDog.Adtech_ComputerNo % 1000000)
					{
			   			if(NewDay!=0)								//�ڽ�����ʱͬʱ�ڽ������������޶�
						{
							NewDog.Adtech_Days=NewDay;				
						}
						else
						{
							NewDog.Adtech_Active=12345678; 			//�����������
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


