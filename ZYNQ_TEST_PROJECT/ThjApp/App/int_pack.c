/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        int_pack.c
  �����ˣ�       ��־��
  �������ڣ�		2004/11/20
  ��Ҫ��������:  �жϣ��ӹ��Լ����ݽ����һЩ������� 

  �޸ļ�¼��
   1.  �޸���:	      ��־��
       �޸�����:	      2005/08/04
       ��Ҫ�޸�����:   �ӹ����տ��ƹ���
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
extern 		COMMAND		Line_Data[MAXLINE];
extern 		INT16S		Line_Data_Size;											//�ܹ���������
extern 		UNITDATA	g_UnitData;
extern 		BASEINI		g_Ini;
extern  	const 		INT16U	g_uXMS;
extern		const 		INT16U	g_uData;
extern 		INT16U		g_uBmpXMS;

INT8U		g_ucXMSArray[XMSSIZE];
INT8U		g_ucDataArray[DATASIZE];
INT32S      g_iEmpNode[MAXCH];

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
WORK_EXT 	INT8S		g_pszMsgBuf[0x30];										//�����Ϣ��ʾ���ݱ����þֲ�������BUG
WORK_EXT	INT8S		g_pszLedOut[8][3];
WORK_EXT	INT16S 		g_iRunLine;		
WORK_EXT 	INT16S		g_iStaRunLine;	
WORK_EXT 	BOOL	  	g_bFirstDispTime;										//ָʾ�Ƿ���Ҫȫ��ˢ������ʱ����ʾ��TRUE��Ҫ
WORK_EXT	INT32S		g_lRunTime;		 
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
WORK_EXT 	INT16S		g_iWorkStatus;
WORK_EXT 	BOOL		g_bAutoRunMode;
WORK_EXT 	INT32S		g_lStepNode;	
WORK_EXT 	BOOL		g_bStepIO ;  
WORK_EXT 	BOOL		g_bDispRunLine;
WORK_EXT 	BOOL		g_bIsHand;	 
WORK_EXT 	BOOL		g_bStepToSingle;
WORK_EXT 	BOOL		g_bMode;	  
WORK_EXT	BOOL		g_bEMSTOP; 
WORK_EXT	INT16S		g_iADValue; 
WORK_EXT	BOOL		g_bTestStart;
WORK_EXT	BOOL		g_bHandRun;  
WORK_EXT	INT32U		g_lHandRun;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	FP32		g_lSpeed;
WORK_EXT	FP32 		g_lWidthPos;
WORK_EXT	INT16S		g_iCheckStatusIO;
WORK_EXT	INT16S      g_iOutStatus;
WORK_EXT	BOOL		g_bBackZero;											//ָʾ�Ƿ�Ϊ�������
WORK_EXT	INT16S		g_iErrorNo;
//WORK_EXT	BOOL		g_bCylinder; 											//ָʾ�Ƿ������ײ���״̬
WORK_EXT	BOOL		g_bScrProtectRun;
WORK_EXT 	INT16S		g_iWorkNumber ;
WORK_EXT 	INT16S		g_iSpeedBeilv ;
WORK_EXT	INT16U 		g_uiMoveKey;
WORK_EXT 	INT32S  	g_lXmsSize;    
WORK_EXT	INT32S		g_lDataCopyXmsSize;
WORK_EXT 	INT32S		g_lYCountLen;       									//�ӹ�������Y��Ĳ�����
WORK_EXT	FP32 		g_fCloseDispCount; 
WORK_EXT	INT16U		g_uiSuspendTask;										//suspend 1~5 task,high byte rev
WORK_EXT 	BOOL		g_bModify1;

int 		g_bMode2=0;
int 		g_bUAxiu=0;
INT32S 		Y_Cont = 0;

extern		void		IRQ_Isr1(void);
extern		void		IRQ_Isr2(void);

#define		MAXLOOP		3

void Fat_Printf(char *id,FP32 data)
{
	char    buffert[32];
	char    bux[64];

	ftoa(data,(INT8U *)buffert,3,10);
	sprintf(bux,"%s%s",id,buffert);
	Uart_Printf("%s\n",bux);
}


void	UnInstallDriverInterrupt()
{
	//	 rINTMSK = (rINTMSK|BIT_EINT1);
}


INT16S	UnpackData()
{

	COMMAND *ptr = Line_Data;
	FP32	x=0,y=0,z=0,a=0,xspeed=15.0,yspeed=15.0,zspeed=15.0,aspeed=15.0;
	INT32S	lx=0,ly=0,lz=0,la=0,lxspeed=15,lyspeed=15,lzspeed=15,laspeed=15;
	INT32S	lastlx =0,lastly =0,lastlz =0,lastla=0; 					//���ڼ�¼��ǰ���ϴΣ����MXS��X,Y,Z,A��ֵ ��
	INT16S	l=0;
	INT32S	num =0; 													//num ���ڱ�ʾ�к�
	INT8U	status,ostatus[MAXCYLINDER];


	INT16S	iCount=0,i=0,j=0,ThisCount[MAXLOOP+1]={0,0,0,0};
	FP64	n[MAXVALUECOL-1];
	/*
value[0]Ϊָ�value[1]X��ʼ�ǣ�value[2]X�����ǣ�value[3]���ߣ�value[4]תо��value[5]��ȡ
value[6]Ϊ�ٱȣ�value[7]Ϊ̽�룬value[8]Ϊ���ף�value[9]Ϊ��ʱ
	 */
	FP32	time = 0.0;
	FP32	speedbeilv=1.0;
	FP32	tempspeed=1.0;
	INT16S	gotoLine=0;
	INT16S	jumpLine = 0;


	INT16S	iLoop[MAXLOOP+1][2]={{0,0},{0,0},{0,0},{0,0}};
	BOOL	flag=FALSE;
	BOOL	bXReset = FALSE;						//��ʾX����0��360��-360�Ƿ�Ҫλ������
	BOOL	bZReset = FALSE;						//��ʾZ����0��360��-360�Ƿ�Ҫλ������
	BOOL	bAReset = FALSE;						//��ʾA����0��360��-360�Ƿ�Ҫλ������
	INT16S	nLoopNo = 0;							//���ڱ�ŵ�Ψһ��ȷ�ԣ�ÿLOOPһ�μ�500���

	BOOL    bLoop = FALSE;														//�Ƿ�ѭ��
	BOOL    bSend = TRUE;														//�ظ�ָ���Ƿ�����				

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr;
	cpu_sr = 0; 								 /* Prevent compiler warning						   */
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


	for(i=0; i<Line_Data_Size; )
	{

		BEGIN_ACCESS:


		ptr = Line_Data + i;

		num =i*2 + nLoopNo*500; 											//��0�����ڴ��ٶ�ֵ 	

		/*
value[0]Ϊָ�value[0]X��ʼ�ǣ�value[1]X�����ǣ�value[2]���ߣ�value[3]תо��value[4]��ȡ
value[5]Ϊ�ٱȣ�value[6]Ϊ̽�룬value[7]Ϊ���ף�(value[9]Ϊ��ʱ)

		Uart_Printf("\n 0=%s,1=%s,2=%s,3=%s,4=%s,5=%s,6=%s,7=%s,8=%s,9=%s\n",
						ptr->value[0],ptr->value[1],ptr->value[2],ptr->value[3],ptr->value[4],
						ptr->value[5],ptr->value[6],ptr->value[7],ptr->value[8],ptr->value[9]);
		 */

		{
			if(strcmp(ptr->value[0],"YF")==0)
			{
				OS_ENTER_CRITICAL();
				SetUnitData(num,SPEED);
				OS_EXIT_CRITICAL();

				if(ptr->value[1][0])xspeed = GetMMFromAngle(1,stringtof((INT8U *)ptr->value[1])*360.0/60.0);			//�ٶȱ������ٶ�ƽ���㷨�Ծ߱������Ƿ������㷨������ʵ�ٶȡ�
				if(ptr->value[2][0])yspeed = stringtof((INT8U *)ptr->value[2])*1000.0/60.0;
				if(ptr->value[3][0])zspeed = (g_Ini.iAxisRunMode[2] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[3] )/60.0 * 1000.0)
						:GetMMFromAngle(3,stringtof((INT8U *)ptr->value[3])*360.0/60.0);
				if(ptr->value[4][0])aspeed = (g_Ini.iAxisRunMode[3] == 2)?(( FP32 )stringtof( ( INT8U * ) ptr->value[4] )/60.0 * 1000.0)
						:GetMMFromAngle(4,stringtof((INT8U *)ptr->value[4])*360.0/60.0);


				lxspeed=GetPulseFromMM(1,xspeed);
				lyspeed=GetPulseFromMM(2,yspeed);
				lzspeed=GetPulseFromMM(3,zspeed);
				laspeed=GetPulseFromMM(4,aspeed);

				g_UnitData.lValue[0] = lxspeed;
				g_UnitData.lValue[1] = lyspeed;
				g_UnitData.lValue[2] = lzspeed;
				g_UnitData.lValue[3] = laspeed;

				MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA)); 	//�Ѽӹ����ݴ洢g_ucDataArray����
				g_lXmsSize ++;

			}
			else if(ptr->value[0][0]=='N')
			{
				if(bLoop==FALSE)															//��Ȧָ�����Ƕ�ף��ʼӸ�����������
				{
					ThisCount[iCount]=0;													//����ѭ��ָ���������ѭ������

					iLoop[iCount][0]=stringtoi((INT8U *)(strtok(ptr->value[0],"N")));	//ѭ����ʼ��
					iLoop[iCount][1]=stringtoi((INT8U *)(strtok(ptr->value[1],g_bEnglish?"L ":"��")))+1;	//ѭ������
					//bSend = (strcmp(ptr->value[3],g_bEnglish?"Y OFF":"���߹�")==0)?FALSE:TRUE;

					if(ptr->value[2][0]=='1')			//ѭ��ģʽ123-321-123.....
					{
						bSend = TRUE;
					}
					else
					{
						bSend = FALSE;
					}

					if(iLoop[iCount][0]>=i || iLoop[iCount][0] < 0)										//��ֹѭ����ʼ��ָ��N��ָ��
					{
						g_bUnpackCorrect=FALSE;
						g_bDataCopy=FALSE;
						return 1;
					}				

					bLoop=TRUE;
				} 

				{
					ThisCount[iCount]++;						

					if(ThisCount[iCount]>=iLoop[iCount][1])
					{
						bLoop = FALSE;
					}
					else
					{																														

						if(bSend==FALSE)//���߹�
						{				//�ú������˼����ڽ���ӹ�ѭ��ǰʱ��X,Z,A���λ���������ѭ�����ܵ����߳��ȡ�
							INT16S loopno = iLoop[iCount][0];							//ѭ����ʼ�м�һ ��Ϊ����Ǵ�1��ʼ�����ݴ洢�Ǵ�0��ʼ��
							INT16S l = 0;
							INT32S tempx=0,tempy=0,tempz = 0,tempa=0;

							for(l=0;l<loopno;l++)											//�ӵ�һ�е�ѭ����ʼ�м�1����
							{
								ptr = Line_Data + l;	

								if( ptr->value[0][0] )										//��ʼ����ֵ	
								{		
									if(g_lXmsSize-1<g_iEmpNode[0])							//Ѱ�������к����ڲ�����ģʽ�����㡣����ѭ���������壬��Ϊ����ѭ���оͱ�����
									{
										g_iEmpNode[0]=g_lXmsSize-1;
									}

									if(strcmp(ptr->value[0],"S")==0)	
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[0],"+")==0)
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[0],"-")==0)
									{
										tempx = 0;
									}
									else
									{
										tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[0]));
									}
								}

								if( ptr->value[1][0] )										//��������ֵ
								{							 
									if(g_lXmsSize-1<g_iEmpNode[0])
									{
										g_iEmpNode[0]=g_lXmsSize-1;
									}

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
										tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[1]));
									}
								}

								if( ptr->value[3][0] && g_Ini.iAxisRunMode[2] == 0)			//תо����ֵ,��תо��ʽΪ0
								{							 
									if(g_lXmsSize-1<g_iEmpNode[2])
									{
										g_iEmpNode[2]=g_lXmsSize-1;
									}

									if(strcmp(ptr->value[3],"S")==0)	
									{
										tempz = 0;
									}
									else if(strcmp(ptr->value[3],"+")==0)
									{
										tempz = 0;

									}
									else if(strcmp(ptr->value[3],"-")==0)
									{
										tempz = 0;
									}
									else
									{
										tempz= GetPulseFromAngle(3,(FP32)stringtof((INT8U *)ptr->value[3]));
									}
								}								
								else if( ptr->value[3][0] && g_Ini.iAxisRunMode[2] == 2)
								{
									if(g_lXmsSize-1<g_iEmpNode[2])
									{
										g_iEmpNode[2]=g_lXmsSize-1;
									}

									tempz= GetPulseFromMM(3,(FP32)stringtof((INT8U *)ptr->value[3]));
								}

								if( ptr->value[4][0] && g_Ini.iAxisRunMode[3] ==0)				//��������ֵ,�Ҿ�����ʽΪ0ʱ
								{							 
									if(g_lXmsSize-1<g_iEmpNode[3])
									{
										g_iEmpNode[3]=g_lXmsSize-1;
									}

									if(strcmp(ptr->value[4],"S")==0)	
									{
										tempa = 0;
									}
									else if(strcmp(ptr->value[4],"+")==0)
									{
										tempa = 0;
									}
									else if(strcmp(ptr->value[4],"-")==0)
									{
										tempa = 0;
									}
									else
									{
										tempa= GetPulseFromAngle(4,(FP32)stringtof((INT8U *)ptr->value[4]));
									}
								}															
								else if( ptr->value[4][0] && g_Ini.iAxisRunMode[3] == 2)
								{
									if(g_lXmsSize-1<g_iEmpNode[3])
									{
										g_iEmpNode[3]=g_lXmsSize-1;
									}

									tempa= GetPulseFromMM(4,(FP32)stringtof((INT8U *)ptr->value[4]));
								}

							}

							for(l=loopno;l<=i;l++)			//��ѭ����ʼ�е������в���
							{
								ptr = Line_Data + l;		

								if(l==loopno && ptr->value[0][0] )	//���ѭ����ʼ�еĿ�ʼ����ֵ	
								{							 
									if(strcmp(ptr->value[0],"S")==0)	
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[0],"+")==0)
									{
										tempx = 0;
									}
									else if(strcmp(ptr->value[0],"-")==0)
									{
										tempx = 0;
									}
									else
									{
										tempx= GetPulseFromAngle(1,(FP32)stringtof((INT8U *)ptr->value[0]));
									}
								}

								if( ptr->value[2][0] )		
								{							 
									tempy+= (FP32)stringtof((INT8U *)ptr->value[2]);  	//����ѭ�����ܵ����߳���
								}
							}

							lx= tempx;
							y = bSend?tempy+y:y;
							ly = GetPulseFromMM(2,y);										//Y��ĳɾ���λ��

							if(g_Ini.iAxisRunMode[2] == 0  || g_Ini.iAxisRunMode[2] == 2)
								lz= tempz;
							else if(g_Ini.iAxisRunMode[2] == 1)
								lz = lz;

							if(g_Ini.iAxisRunMode[3] ==0  || g_Ini.iAxisRunMode[3] == 2)
							{
								la= tempa;
							}
							else if(g_Ini.iAxisRunMode[3] ==1)
							{
								la = la;
							}

							{																//������Ĵ���													
								if(g_Ini.iAxisSwitch[1]==0)
									ly = 0;					

								if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
									lz = 0;

								if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
									la = 0;
							}

							SetUnitData(num,MOVE);

							if(lx !=lastlx)
							{
								g_UnitData.SpindleAxis = 1;									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
								g_UnitData.lValue[4] = lxspeed;
							}
							else if(ly !=lastly)
							{
								g_UnitData.SpindleAxis = 2;									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
								g_UnitData.lValue[4] = lyspeed;
							}
							else if(lz != lastlz)
							{
								g_UnitData.SpindleAxis = 3;									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
								g_UnitData.lValue[4] = lzspeed;
							}
							else if(la !=lastla)
							{
								g_UnitData.SpindleAxis = 4;									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
								g_UnitData.lValue[4] = laspeed;
							}
							else
							{
								g_UnitData.SpindleAxis = 1;									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
								g_UnitData.lValue[4] = lxspeed;
							}

							g_UnitData.lValue[0] = lastlx = lx;
							g_UnitData.lValue[1] = lastly = ly;
							g_UnitData.lValue[2] = lastlz = lz;				
							g_UnitData.lValue[3] = lastla = la;		

							MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
							g_lXmsSize ++;

							i=iLoop[iCount][0];											//��ת��ѭ����ʼ��
							nLoopNo ++;														//���ڱ�ŵ�Ψһ��ȷ�ԣ�ÿLOOPһ�μ�500���

							goto BEGIN_ACCESS;
						}
						else //���߿�
						{
							INT16S loopno = iLoop[iCount][0]-1;							//ѭ����ʼ�м�һ ��Ϊ����Ǵ�1��ʼ�����ݴ洢�Ǵ�0��ʼ��

							INT32S		lXmsSize=0;
							INT32S		lXmsSize1=0;

							INT32S		lDYPulse=0;											//����������֮���Y��������ֵ
							UNITDATA	pDataLoop;
							UNITDATA	pDataLoop1;

							lXmsSize1  = g_lXmsSize-1;

							for( ;ThisCount[iCount]<iLoop[iCount][1]; ThisCount[iCount]++)
							{

								lXmsSize = lXmsSize1;

								for( ; ; )													//ѭ�����������ʼ�м�һ
								{
									XMSToMEMEx((INT8U *)&pDataLoop,g_uData,(lXmsSize)*sizeof(UNITDATA),sizeof(UNITDATA));		//���һ�н����ǣ��Խ�������ʼΪ������ʼ������һ����
									XMSToMEMEx((INT8U *)&pDataLoop1,g_uData,(lXmsSize-1)*sizeof(UNITDATA),sizeof(UNITDATA));	//���һ�п�ʼ��
									lDYPulse=pDataLoop.lValue[1]-pDataLoop1.lValue[1];											//��������֮��Y��ľ���λ�ò�ֵ

									pDataLoop1.nLine=pDataLoop1.nLine%500 + (++nLoopNo)*500;
									pDataLoop1.lValue[1]=lastly=ly=ly+lDYPulse;
									pDataLoop1.SpindleAxis=pDataLoop.SpindleAxis;												//ע������Ҫ�л������ٽǵ�����
									MEMToXMSEx((INT8U *)&pDataLoop1,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize++;

									lXmsSize--;

									if((pDataLoop1.nLine%500) <= ((loopno+1)*2-1))												//������ʱ,������nLineС�ڵ���ѭ����ʼ�б�ʱ�˳�
										break;

									if(g_lXmsSize>(DATASIZE/sizeof(UNITDATA)-10))								//����ڴ�������
									{
										g_bUnpackCorrect=FALSE;
										g_bDataCopy=FALSE;
										return 5;
									}

								}

								lXmsSize = lXmsSize;
								nLoopNo++;		//����nLine��С���󣬲���Ҫÿһ����һ��

								for( ; ; )													//ѭ����һ�У�������м�һ
								{
									XMSToMEMEx((INT8U *)&pDataLoop,g_uData,(lXmsSize)*sizeof(UNITDATA),sizeof(UNITDATA));		//��һ�п�ʼ�ǣ��Կ�ʼ������Ϊ������������ʼһ����
									XMSToMEMEx((INT8U *)&pDataLoop1,g_uData,(lXmsSize+1)*sizeof(UNITDATA),sizeof(UNITDATA));	//��һ�п�ʼ��
									lDYPulse=pDataLoop1.lValue[1]-pDataLoop.lValue[1];											//��������֮��Y��ľ���λ�ò�ֵ

									pDataLoop1.nLine=pDataLoop1.nLine%500 + nLoopNo*500;
									pDataLoop1.lValue[1]=lastly=ly=ly+lDYPulse;

									MEMToXMSEx((INT8U *)&pDataLoop1,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize++;

									lXmsSize++;

									if(lXmsSize >= lXmsSize1)												//lXmsSize�����Ǵ��ڼ�¼lXmsSize1��ʱ�޳�����ѭ����
										break;

									if(g_lXmsSize>(DATASIZE/sizeof(UNITDATA)-10))								//����ڴ�������
									{
										g_bUnpackCorrect=FALSE;
										g_bDataCopy=FALSE;
										return 5;
									}
								}

							}

							lastly = ly;
							y = GetMMFromPulse(2,ly);

							//iCount--;
							bLoop = FALSE; 				//ѭ���������ѭ����־

						}
					}
				}
			}
			else//��ͬ��Mָ��
			{	
				//*****���ַ�������ת��Ϊ��������*****//
				{
					BOOL bEndCode = TRUE;

					for(j=0;j<MAXVALUECOL-1;j++)
						n[j]=-10000.0;


					for( j=0; j<MAXVALUECOL; j++)
					{
						if( ptr->value[j][0] )
							n[j] = (FP32)stringtof((INT8U *)ptr->value[j]);
					}

					/*
value[0]Ϊָ�value[0]X��ʼ�ǣ�value[1]X�����ǣ�value[2]���ߣ�value[3]תо��value[4]��ȡ
value[5]Ϊ�ٱȣ�value[6]Ϊ̽�룬value[7]Ϊ���ף�value[8]Ϊ��ʱ

n[0]��ʼ�ǣ�n[1]������, n[2]����, n[3]תо, n[4]��ȡ, n[5]�ٱ�, n[6]̽��, n[7]����, n[8]��ʱ

					 */

				}

				//**************�����ݽ����ŵ�g_ucDataArray[]*******************//	
				{
					/*----------------------��ʼ�Ǵ���----------------------------*/
					if(fabs(n[0]+10000)>0.001)													
					{	

						if(g_lXmsSize-1<g_iEmpNode[0])
						{
							g_iEmpNode[0]=g_lXmsSize-1; 		
						}


						if(g_Ini.iCyldMode == 0)											//����01ģʽ
								{
							INT16S templen=strlen(ptr->value[MAXVALUECOL-1]);

							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;

							if(templen>0)
							{
								for( l=0;l<templen;l++)
								{
									if(ptr->value[MAXVALUECOL-1][l]=='0')
										ostatus[l]=0;
									else
									{
										if(ptr->value[MAXVALUECOL-1][l]=='1')
											ostatus[l]=1;
										else
											ostatus[l]=2;
									}
								}
							}
								}
						else															 //����ģʽ
						{
							INT16S templen=strlen(ptr->value[MAXVALUECOL-1]);

							for(l=0;l<MAXCYLINDER;l++)
								ostatus[l]=2;

							if(templen>0)
							{
								if(templen==1 && ptr->value[MAXVALUECOL-1][0]=='0')
								{
									for(l=0;l<MAXCYLINDER;l++)
									{
										if(l==O_SENDADD && g_Ini.iXianjiaDrive==1)
											continue;

										ostatus[l]=0;
									}
								}
								else
								{
									for( l=0;l<templen;l++)
									{
										if(ptr->value[MAXVALUECOL-1][l]=='-')
										{
											if(ptr->value[MAXVALUECOL-1][l+1]>'0'&& ptr->value[MAXVALUECOL-1][l+1]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-1][l+1]-'0'-1] = 0;

											++l;

										}
										else 
										{
											if(ptr->value[MAXVALUECOL-1][l]>'0'&& ptr->value[MAXVALUECOL-1][l]<MAXCYLINDER+'1')
												ostatus[ptr->value[MAXVALUECOL-1][l]-'0'-1] = 1;
										}

									}
								}
							}
						}

						//if(fabs(n[8]+10000.0)>0.001)						//��ʱ
						//	time = n[8];
						//else
						//	time = 0.0;

						if(strcmp(ptr->value[0],"S")==0)								//��ʼ�Ǿͽ�����
						{
							if(fabs(lx)<g_Ini.lPulse[0]*0.5)
							{
								lx	= 0;
							}
							else
							{
								if(lx>0)
									lx	= g_Ini.lPulse[0];
								else
									lx	= -g_Ini.lPulse[0];
							}

							bXReset = TRUE;

						}
						else if(strcmp(ptr->value[0],"+")==0)							//�����������
						{

							if(lx>0)
								lx	= g_Ini.lPulse[0];
							else
								lx	= 0;

							bXReset = TRUE;

						}
						else if(strcmp(ptr->value[0],"-")==0)							//�����������
						{
							if(lx<0)
								lx	= -g_Ini.lPulse[0];
							else
								lx	= 0;				

							bXReset = TRUE;
						}
						else															//������ʼ��
						{			
							if( fabs( n[0]+10000.0 ) >0.001 )
							{
								x = n[0];
								lx= GetPulseFromAngle(1,x);
							}

							bXReset = FALSE;
						}													

						SetUnitData(num-1,MOVE);

						g_UnitData.SpindleAxis = 1; 									//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
						g_UnitData.lValue[0] = lastlx = lx;
						g_UnitData.lValue[1] = lastly = ly;
						g_UnitData.lValue[2] = lastlz = lz; 			
						g_UnitData.lValue[3] = lastla = la;

						g_UnitData.lValue[4] = lxspeed; 

						for(l=0;l<MAXCYLINDER;l++)
							g_UnitData.nOut[l]=ostatus[l];

						//g_UnitData.fDelay = time;

						if(bXReset)
						{
							lastlx = lx = 0;
							bXReset = FALSE;
							g_UnitData.nReset = g_UnitData.nReset|1;
						}

						MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
						g_lXmsSize ++;

					}

					/*----------------------�����Ǵ���----------------------------*/
					if(fabs(n[1]+10000)>0.001)											
					{
						if(g_lXmsSize-1<g_iEmpNode[0])
						{
							g_iEmpNode[0]=g_lXmsSize-1;
						}


						if(strcmp(ptr->value[1],"S")==0)								//�����Ǿͽ�����
						{
							if(fabs(lx)<g_Ini.lPulse[0]*0.5)
							{
								lx	= 0;
							}
							else
							{
								if(lx>0)
								{
									lx	= g_Ini.lPulse[0];
								}
								else
								{
									lx	= -g_Ini.lPulse[0];
								}
							}		

							bXReset = TRUE;

						}
						else if(strcmp(ptr->value[1],"+")==0)						//���������������
						{
							if(lx>0)
								lx	= g_Ini.lPulse[0];
							else
								lx	= 0;	

							bXReset = TRUE;

						}
						else if(strcmp(ptr->value[1],"-")==0)						//�����Ǹ��������
						{
							if(lx<0)
								lx	= -g_Ini.lPulse[0];
							else
								lx	= 0;	

							bXReset = TRUE;

						}
						else														//����������  �ڶ���������Ľ���
						{
							if( fabs( n[1]+10000.0 ) >0.001)
							{
								x = n[1];
								lx= GetPulseFromAngle(1,x);
							}

							bXReset = FALSE;

						}
					}

					/*----------------------������λ�ý���------------------------*/
					if( fabs(n[2]+10000)>0.001)
					{
						y += n[2];
						ly=GetPulseFromMM(2,y);
					}


					/*----------------------תо��λ�ý���------------------------*/	
					{																	//Z��תо��������� 

						INT32S tmplz =0;
						INT32S tmpts = 0;

						tmplz = lz%g_Ini.lPulse[2]; 									//������
						tmpts = lz/g_Ini.lPulse[2]; 									//��Ȧ

						if(ptr->value[3][0])
						{
							if(g_lXmsSize-1<g_iEmpNode[2])
							{
								g_iEmpNode[2]=g_lXmsSize-1;
							}
						}

						if(strcmp(ptr->value[3],"S")==0)								//תо��ͽ�����
						{
							if(labs(tmplz)<g_Ini.lPulse[2]*0.5)
							{
								lz	= tmpts*g_Ini.lPulse[2];
							}
							else
							{
								if(lz>0)
									lz	= (tmpts+1)*g_Ini.lPulse[2];
								else
									lz	= (tmpts-1)*g_Ini.lPulse[2];
							}

							bZReset = TRUE;

						}
						else if(strcmp(ptr->value[3],"+")==0)							//תо���������
						{
							if(tmplz>0)
								lz	= (tmpts+1)*g_Ini.lPulse[2];
							else
								lz	= tmpts*g_Ini.lPulse[2];	

							bZReset = TRUE; 
						}
						else if(strcmp(ptr->value[3],"-")==0)							//תо�Ḻ�����
						{
							if(tmplz<0)
								lz	= (tmpts-1)*g_Ini.lPulse[2];
							else
								lz	= tmpts*g_Ini.lPulse[2];	

							bZReset = TRUE; 
						}
						else															//תо��λ�ý�����
						{
							if( fabs(n[3]+10000)>0.001 )
							{		
								if(g_Ini.iAxisRunMode[2] ==0)							//��Ȧ����λ�ý���
								{
									z = n[3];			
									lz = GetPulseFromAngle(3,z);
								}
								else if(g_Ini.iAxisRunMode[2] ==1)						//��Ȧ�����λ��
								{																											
									INT16S	temp;
									FP32	tempf;
									FP64 tt=n[3];

									temp=(INT16S)tt;						
									tempf=n[3]-temp;
									z=temp*360+tempf*1000.0;

									lz = lz + GetPulseFromAngle(3,z);						

								}
								else  if(g_Ini.iAxisRunMode[2] ==2) 					//˿���߾���λ��
								{
									z = n[3];			
									lz = GetPulseFromMM(3,z);
								}
							}

							bZReset = FALSE;
						}
					}


					/*----------------------��ȡ��λ�ý���------------------------*/	
					{																	//A���ȡ��������� 
						INT32S tmpla =0;
						INT32S tmpts = 0;

						tmpla = la%g_Ini.lPulse[3]; 									//������
						tmpts = la/g_Ini.lPulse[3]; 									//��Ȧ

						if(ptr->value[4][0])
						{
							if(g_lXmsSize-1<g_iEmpNode[3])
							{
								g_iEmpNode[3]=g_lXmsSize-1;
							}
						}


						if(strcmp(ptr->value[4],"S")==0)								//��ȡ��ͽ�����
						{
							if(labs(tmpla)<g_Ini.lPulse[3]*0.5)
							{
								la	= tmpts*g_Ini.lPulse[3];
							}
							else
							{
								if(la>0)
									la	= (tmpts+1)*g_Ini.lPulse[3];
								else
									la	= (tmpts-1)*g_Ini.lPulse[3];
							}

							bAReset = TRUE;

						}
						else if(strcmp(ptr->value[4],"+")==0)							//��ȡ���������
						{

							if(tmpla>0)
								la	= (tmpts+1)*g_Ini.lPulse[3];
							else
								la	= tmpts*g_Ini.lPulse[3];	

							bAReset = TRUE; 
						}
						else if(strcmp(ptr->value[4],"-")==0)							//��ȡ�Ḻ�����
						{

							if(tmpla<0)
								la	= (tmpts-1)*g_Ini.lPulse[3];
							else
								la	= tmpts*g_Ini.lPulse[3];	

							bAReset = TRUE; 
						}
						else															//��ȡ��λ�ý�����
						{
							if( fabs(n[4]+10000)>0.001 )
							{
								if(g_Ini.iAxisRunMode[3] == 0)								//��Ȧ����λ�ý���
								{
									a = n[4];			
									la = GetPulseFromAngle(4,a);
								}
								else if(g_Ini.iAxisRunMode[3] ==1)							//��Ȧ�����λ��
								{																											
									INT16S	temp;
									FP32	tempf;
									FP64 tt=n[4];

									temp=(INT16S)tt;						
									tempf=n[4]-temp;
									a=temp*360+tempf*1000.0;

									la = la + GetPulseFromAngle(4,a);						

								}
								else  if(g_Ini.iAxisRunMode[3] ==2) 					//˿���߾���λ��
								{
									a = n[4];			
									la = GetPulseFromMM(4,a);
								}
							}

							bAReset = FALSE;
						}
					}


					if( fabs(n[5]+10000)>0.001 )										//�ٱ� ע�����ٱ���Ч��ΧΪ��ʼ����������һ�Σ�
					{																	//����CNC502B�ĵ���ʼ�ǣ���ʼ�ǵ������ǵ���Ч��Χ�ǲ�һ����
						speedbeilv = n[5];
					}
					else
					{
						speedbeilv = 1.0;
					}

					status = 0;
					if( fabs(n[6]+10000)>0.001 )										//̽��
					{
						status = (INT16S)n[6];
					}
					else
					{
						status = 0;
					}


					{																	//������Ĵ���													
						if(g_Ini.iAxisSwitch[1]==0)
							ly = 0; 				

						if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
							lz = 0;

						if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
							la = 0;
					}

					////******��ת���ĸ������ݷŵ�ȫ�ֽṹ����g_UnitData****/////
					{
						if( fabs( n[1]+10000.0 ) >0.001 && lx != lastlx)		//�������Ǳ༭��Ϊ��ʱ
						{

							//	Uart_Printf("num %10ld	,YlinePos[num] = %10ld \n",num,YlinePos[num]);

							SetUnitData(num,MOVE);
							g_UnitData.SpindleAxis = 1; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��

							g_UnitData.lValue[0] = lastlx = lx;
							g_UnitData.lValue[1] = lastly = ly;
							g_UnitData.lValue[2] = lastlz = lz; 			
							g_UnitData.lValue[3] = lastla = la;

							tempspeed = xspeed*speedbeilv;

							g_UnitData.lValue[4] = GetPulseFromMM(1,tempspeed);

							g_UnitData.nIO = status;					//̽��

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

							MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
							g_lXmsSize ++;

						}
						else											
						{
							if(fabs( n[2]+10000.0 ) >0.001 && ly !=lastly ) 			//������༭��Ϊ��ʱ
							{

								//		Uart_Printf("num %10ld	,YlinePos[num] = %10ld \n",num,YlinePos[num]);

								SetUnitData(num,MOVE);
								g_UnitData.SpindleAxis = 2; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��

								g_UnitData.lValue[0] = lastlx = lx;
								g_UnitData.lValue[1] = lastly = ly;
								g_UnitData.lValue[2] = lastlz = lz; 			
								g_UnitData.lValue[3] = lastla = la;

								tempspeed = yspeed*speedbeilv;

								g_UnitData.lValue[4] = GetPulseFromMM(2,tempspeed);

								g_UnitData.nIO = status;		


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

								MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
								g_lXmsSize ++;

							}
							else
							{
								if(fabs( n[3]+10000.0 ) >0.001 && lz !=lastlz)			//��תо��༭��Ϊ��ʱ
								{
									//	YlinePos[num] = ly;

									//	Uart_Printf("num %10ld	,YlinePos[num] = %10ld \n",num,YlinePos[num]);

									SetUnitData(num,MOVE);
									g_UnitData.SpindleAxis = 3; 						//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��


									g_UnitData.lValue[0] = lastlx = lx;
									g_UnitData.lValue[1] = lastly = ly;
									g_UnitData.lValue[2] = lastlz = lz; 			
									g_UnitData.lValue[3] = lastla = la;

									tempspeed = zspeed*speedbeilv;

									g_UnitData.lValue[4] = GetPulseFromMM(3,tempspeed);

									g_UnitData.nIO = status;


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

									MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize ++;

								}
								else if(fabs( n[4]+10000.0 ) >0.001 && la !=lastla)
								{

									SetUnitData(num,MOVE);
									g_UnitData.SpindleAxis = 4; 						//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��

									g_UnitData.lValue[0] = lastlx = lx;
									g_UnitData.lValue[1] = lastly = ly;
									g_UnitData.lValue[2] = lastlz = lz; 			
									g_UnitData.lValue[3] = lastla = la;

									tempspeed = aspeed*speedbeilv;

									g_UnitData.lValue[4] = GetPulseFromMM(4,tempspeed);

									g_UnitData.nIO = status;


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

									MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize ++;

								}
								else if(bXReset == TRUE || bZReset == TRUE ||  bAReset == TRUE) 	//��X,Y,Z���ݶ����䣬�������ݹ�������£���һ��ָ�� ��
								{

									//	YlinePos[num] = ly;

									//	Uart_Printf("num %10ld	,YlinePos[num] = %10ld \n",num,YlinePos[num]);

									SetUnitData(num,MOVE);
									g_UnitData.SpindleAxis = 1; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��

									g_UnitData.lValue[0] = lastlx = lx;
									g_UnitData.lValue[1] = lastly = ly;
									g_UnitData.lValue[2] = lastlz = lz; 			
									g_UnitData.lValue[3] = lastla = la;

									tempspeed = xspeed*speedbeilv;

									g_UnitData.lValue[4] = GetPulseFromMM(1,tempspeed);

									g_UnitData.nIO = status;		


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

									MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
									g_lXmsSize ++;

								}
							}
						}
					}

				}

			}

		}//end	switch(ptr->value[0][0])

		i += 1;

		if(g_lXmsSize>(DATASIZE-1)/sizeof(UNITDATA))		//����ڴ�������, 
		{
			g_bUnpackCorrect=FALSE;
			g_bDataCopy=FALSE;
			return 5;

		}

		if(flag ==TRUE) 									//�õ�Eָ���˳�ѭ���ӹ�
			break; 

	}//end	for(i=0; i<Line_Data_Size; )

	{

		{
			UNITDATA  pData1;
			XMSToMEMEx((INT8U *)&pData1,g_uData,(g_lXmsSize-1)*sizeof(UNITDATA),sizeof(UNITDATA));

			num=pData1.nLine+1;
			SetUnitData(num,MOVE);

			g_UnitData.nReset = 15; 						


			if(g_Ini.iBackMode[0]==3)//������
					{
				g_UnitData.nReset&=~(0x01<<0);
					}
			else if(lx%g_Ini.lPulse[0]!=0)
			{
				if(g_Ini.iBackMode[0]==0)											//S
				{
					if(fabs(lx)<g_Ini.lPulse[0]*0.5)
					{
						lx	= 0;
					}
					else
					{
						if(lx>0)
							lx	= g_Ini.lPulse[0];
						else
							lx	= -g_Ini.lPulse[0];
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
			}

			if(g_Ini.iAxisRunMode[2] != 2 )
			{

				if(g_Ini.iBackMode[2]==3 && g_Ini.iAxisRunMode[2]==0)				//��Ȧ������
				{
					g_UnitData.nReset&=~(0x01<<2);

				}
				else if(lz%g_Ini.lPulse[2]!=0)
				{
					INT32S tmplz =0;
					INT32S tmpts = 0;


					tmplz = lz%g_Ini.lPulse[2]; 										//������
					tmpts = lz/g_Ini.lPulse[2]; 										//��Ȧ

					if(g_Ini.iBackMode[2]==2)										//-
					{
						if(tmplz<0)
							lz	= (tmpts-1)*g_Ini.lPulse[2];
						else
							lz	= tmpts*g_Ini.lPulse[2];	
					}
					else if(g_Ini.iBackMode[2]==1)										//+
					{
						if(tmplz>0)
							lz	= (tmpts+1)*g_Ini.lPulse[2];
						else
							lz	= tmpts*g_Ini.lPulse[2];		
					}
					else// if(g_Ini.iBackMode[2]==0)									//S�ͽ����� �Լ� Ĭ����Ϊ
					{
						if(labs(tmplz)<g_Ini.lPulse[2]*0.5)
						{
							lz	= tmpts*g_Ini.lPulse[2];
						}
						else
						{
							if(lz>0)
								lz	= (tmpts+1)*g_Ini.lPulse[2];
							else
								lz	= (tmpts-1)*g_Ini.lPulse[2];
						}
					}
				}
			}
			else if(g_Ini.iAxisRunMode[2] == 2)
			{
				if(g_Ini.iBackMode[2]==3)		//������
				{
					g_UnitData.nReset&=~(0x01<<2);

				}
				else
				{
					lz = 0;
				}
			}


			if( g_Ini.iAxisRunMode[3] != 2)
			{
				if(g_Ini.iBackMode[3]==3 && g_Ini.iAxisRunMode[3]==0)				//��Ȧģʽ������
				{
					g_UnitData.nReset&=~(0x01<<3);
				}
				else if(la%g_Ini.lPulse[3]!=0) 
				{
					INT32S tmpla =0;
					INT32S tmpts = 0;

					tmpla = la%g_Ini.lPulse[3]; 										//������
					tmpts = la/g_Ini.lPulse[3]; 										//��Ȧ

					if(g_Ini.iBackMode[3]==2)										//-
					{
						if(tmpla<0)
							la	= (tmpts-1)*g_Ini.lPulse[3];
						else
							la	= tmpts*g_Ini.lPulse[3];	
					}
					else if(g_Ini.iBackMode[3]==1)										//+
					{

						if(tmpla>0)
							la	= (tmpts+1)*g_Ini.lPulse[3];
						else
							la	= tmpts*g_Ini.lPulse[3];	

					}
					else// if(g_Ini.iBackMode[3]==0)									//S�ͽ������Լ� Ĭ����Ϊ
					{
						if(labs(tmpla)<g_Ini.lPulse[3]*0.5)
						{
							la	= tmpts*g_Ini.lPulse[3];
						}
						else
						{
							if(la>0)
								la	= (tmpts+1)*g_Ini.lPulse[3];
							else
								la	= (tmpts-1)*g_Ini.lPulse[3];
						}
					}
				}

			}
			else if(g_Ini.iAxisRunMode[3] == 2)
			{
				if(g_Ini.iBackMode[3]==3)
				{
					g_UnitData.nReset&=~(0x01<<3);
				}
				else
				{
					la = 0;
				}
			}						


			if(lx !=lastlx)
			{
				g_UnitData.SpindleAxis = 1; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
				g_UnitData.lValue[4] = lxspeed;

			}
			else if(ly !=lastly)
			{
				g_UnitData.SpindleAxis = 2; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
				g_UnitData.lValue[4] = lyspeed;
			}
			else if(lz != lastlz)
			{
				g_UnitData.SpindleAxis = 3; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
				g_UnitData.lValue[4] = lzspeed;
			}
			else if(la != lastla)
			{
				g_UnitData.SpindleAxis = 4; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
				g_UnitData.lValue[4] = laspeed;
			}
			else
			{
				g_UnitData.SpindleAxis = 1; 				//���ᣬ1ΪX�ᣬ2ΪY��3ΪZ��4ΪA��
				g_UnitData.lValue[4] = lxspeed;
			}

			g_UnitData.lValue[0] = lastlx = lx;
			g_UnitData.lValue[1] = lastly = ly;
			g_UnitData.lValue[2] = lastlz = lz; 			
			g_UnitData.lValue[3] = lastla = la;


			lastly = ly = 0;

			if(g_Ini.iBackMode[0]==3)
				lastlx = lx;
			else
				lastlx = lx = 0;

			if(g_Ini.iBackMode[2]==3 && g_Ini.iAxisRunMode[2]!=1)
				lastlz = lz;
			else
				lastlz = lz = 0;

			if(g_Ini.iBackMode[3]==3 && g_Ini.iAxisRunMode[3]!=1)	
				lastla = la;
			else
				lastla = la = 0;

			MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
			g_lXmsSize ++;
		}

		flag=TRUE;

		SetUnitData(num,PEND);
		MEMToXMSEx((INT8U  *)&g_UnitData,g_uData,g_lXmsSize*sizeof(UNITDATA),sizeof(UNITDATA));
		g_lXmsSize ++;
	}


	if( g_lXmsSize< 2)	//�ļ������ٰ���һ���ٶ�ָ��˶�ָ��ͽ���ָ�� 20140809
	{
		g_bUnpackCorrect=FALSE;
		g_bDataCopy=FALSE;
		return 100; 	
	}


	{			//������ݵ���ȷ�ԡ�
		INT8S		CheckStatusIO=0;
		BOOL		WorkEnd=FALSE;
		UNITDATA	pData,pDataTmp;
		INT32S		iNode=0;
		INT32S		t1,t2;

		INT32S		lPValue[6]={0};
		FP32		fPDly=0.0;


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
				Uart_Printf("\n%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10ld,%10lf,%10ld\n"
							,pData.nLine,pData.nCode,pData.SpindleAxis,pData.lValue[0],pData.lValue[1],pData.lValue[2],
							pData.lValue[3],pData.lValue[4]
							,pData.nIO,pData.fDelay,pData.nReset);

			}
			 */

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
				/*
				{
					//��ǰһ������ʱ����ǰ���л���ģʽ�����Ҹ����޶���ʱ��ʾ����ʱ������ģʽ��ͻ���ڽ���λ�ô�����ʱ
					if(fPDly>0.0001 && pData.nReset!=0 && labs(lPValue[0]-pData.lValue[0])<2 && labs(lPValue[1]-pData.lValue[1])<2 &&labs(lPValue[2]-pData.lValue[2])<2 &&labs(lPValue[3]-pData.lValue[3])<2 )
					{
						g_bUnpackCorrect=FALSE;
						g_bDataCopy=FALSE;
						return 103; 
					}

					lPValue[0]=pData.lValue[0];
					lPValue[1]=pData.lValue[1];
					lPValue[2]=pData.lValue[2];
					lPValue[3]=pData.lValue[3];
					fPDly	  =pData.fDelay;

					if(pData.nReset)
					{
						if(pData.nReset & 0x01)
							lPValue[0]=0;

						if(pData.nReset & 0x02)
							lPValue[1]=0;

						if(pData.nReset & 0x04)
							lPValue[2]=0;

						if(pData.nReset & 0x08)
							lPValue[3]=0;
					}					
				}*/


				CheckStatusIO = pData.nIO;

				if(CheckStatusIO && pData.nReset!=0)
				{
					g_bUnpackCorrect=FALSE;
					g_bDataCopy=FALSE;
					return 102; 
				}

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

						//Uart_Printf("temp=%d,curline+2=%d,g_lXmsSize=%d\n",temp,curline+2,g_lXmsSize);				
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
								//Uart_Printf("iNode=%d\n",iNode);
								g_bUnpackCorrect=FALSE;
								g_bDataCopy=FALSE;
								return 7;								
							}

							//Uart_Printf("pData.nLine=%d\n",pData.nLine);

							if((pData.nLine+1)==temp || pData.nLine==temp)
							{
								iNode--;

								ptr = Line_Data + jNo-1;

								//�ж���ת�����Ƿ�������
								if(ptr->value[2][0]==NULL			//X������	
										||(g_Ini.iAxisSwitch[2]==1 && g_Ini.iAxisCount>2?ptr->value[4][0]==NULL:FALSE)	//Z��
										||(g_Ini.iAxisSwitch[3]==1 && g_Ini.iAxisCount>3?ptr->value[5][0]==NULL:FALSE)) //A��
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
		OSQPost(MsgQueue,g_bEnglish?" Start of Loop is Invalid":"ѭ����ʼ�в���������Nָ��֮���Ϊ0!");
		break;

	case 2:
		OSQPost(MsgQueue,g_bEnglish?"Loop is Invalid":"ѭ����Ч!");
		break;

	case 3:
	case 7:
		OSQPost(MsgQueue,g_bEnglish?"Data Format Error!":"J��ת����,����תִ�е�Eָ����!");
		break;

	case 6:
		OSQPost(MsgQueue,g_bEnglish?"Data Format Error!":"J��תλ�ô���,������ת������֮��!");
		break;

	case 4:
	case 8:
		OSQPost(MsgQueue,g_bEnglish?"Data Format Error!":"G��ת����,����תִ�е�Eָ����!");
		break;

	case 9:
		OSQPost(MsgQueue,g_bEnglish?"Data Format Error!":"G��ת����,��֤��ת��M��Sָ����!");
		break;

	case 5:
		OSQPost(MsgQueue,g_bEnglish?"Loop too INT32S":"ѭ������̫��,�ڴ治��!");
		break;

	case 99:
		OSQPost(MsgQueue,g_bEnglish?"Data Format Error!":"��תλ�ô���,��ת���������ѭ��!");
		break;

	case 100:
		OSQPost(MsgQueue,g_bEnglish?"Empty files":"�ļ�����Ϊ��!");
		break;

	case 101:
		OSQPost(MsgQueue,g_bEnglish?"Lack of E code":"ȱ��Eָ�������ִ�е�Eָ��λ��!");
		break;

	case 102:
		OSQPost(MsgQueue,g_bEnglish?"Probe row can't exist zero mode!":"̽���в��ɴ��ڼӹ��л���+/-!");
		break;

	default:
		OSQPost(MsgQueue,g_bEnglish?"Data erro":"�������ݴ���!");
		break;
	}


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


void SuspendTasks(INT8S no)
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

	INT32S tempspeed,maxpulse;
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
			maxpulse = (INT32S)((FP32)g_Ini.lPulse[0]/g_Ini.fGearRate[0]*g_Ini.lMaxSpeed[0]/60.0);

			if(tempspeedpulse>maxpulse)
				tempspeedpulse=maxpulse;

		}
		else
		{
			if(tempspeed == p2)
			{
				tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
				maxpulse = (INT32S)((FP32)g_Ini.lPulse[1]/g_Ini.fGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);

				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;

			}
			else
			{
				if(tempspeed == p3)
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.fGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);

					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p1*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.fGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);

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
				maxpulse = (INT32S)((FP32)g_Ini.lPulse[1]/g_Ini.fGearRate[1]*g_Ini.lMaxSpeed[1]/60.0);

				if(tempspeedpulse>maxpulse)
					tempspeedpulse=maxpulse;

			}
			else
			{
				if(tempspeed == p3)
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p2*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.fGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);

					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p2*(FP32)tempspeedpulse; 
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.fGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);

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
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[2]/g_Ini.fGearRate[2]*g_Ini.lMaxSpeed[2]/60.0);

					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
				else
				{
					tempspeedpulse = (FP32)tempspeed/(FP32)p3*(FP32)tempspeedpulse;
					maxpulse = (INT32S)((FP32)g_Ini.lPulse[3]/g_Ini.fGearRate[3]*g_Ini.lMaxSpeed[3]/60.0);

					if(tempspeedpulse>maxpulse)
						tempspeedpulse=maxpulse;
				}
			}
		}
	}

	return tempspeedpulse;

}

