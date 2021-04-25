# include	"includes.h"

extern		BOOL		g_lHandRun;
extern		BOOL		g_bHandMove;
extern		INT32S		g_iWorkStatus;
extern 		BASEINI		g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	OS_EVENT	*FreeSem;

WORK_EXT 	BOOL		g_bFirstResetX;											//指示X轴是否复位，FALSE为需要复位
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;
WORK_EXT	INT16S		g_iMoveAxis;
WORK_EXT	FP32 		g_fSpeedBi;
extern		BOOL		g_bBackZero;
WORK_EXT 	INT16S		g_iSpeedBeilv ;

#define		MAXIN	 	36
#define 	MAXOUT		10
//#define			PRINTBMP
CRect		g_ptrOutRect[MAXOUT];
BOOL		g_bOutStatus[MAXOUT];
BOOL		g_bTestFrm=FALSE;
char 		g_InputIO[MAXIN];
char 		g_ProStatus[4];
INT16U		g_iTestAxis;

/***************************************
	输入输出检测

***************************************/

void	CTestFrm_Init()
{
	INT16S i=0;

	for(i=0; i<MAXOUT; i++)
		g_bOutStatus[i]=GetOut(i);	

	g_bTestFrm=TRUE;
	key_set_led(XLED,LEDCLOSE);			
	key_set_led(YLED,LEDCLOSE);			
	key_set_led(ZLED,LEDCLOSE);
}


void	CTestFrm_Destroy()
{
	g_bTestFrm=FALSE;
}


void	CTestFrm_Create()
{
	CDC_PutColorBar(2,2,798,478,g_nBk,1);
	CDC_Rectangle(0,0,getmaxx(),getmaxy(),SILVERCOLOR);
	CDC_Rectangle(1,1,getmaxx()-1,getmaxy()-1,SILVERCOLOR);
	CDC_Rectangle(2,2,getmaxx()-2,getmaxy()-2,SILVERCOLOR);
	CDC_Rectangle(3,3,getmaxx()-3,getmaxy()-3,SILVERCOLOR);
	
	CDC_Rectangle(515,59,791,435,SILVERCOLOR);
	CDC_Rectangle(516,60,790,434,SILVERCOLOR);
	CDC_Rectangle(517,61,789,436,SILVERCOLOR);
	
	CDC_SetRectangle(10,60,381,375,g_bEnglish?"Input":"输入",g_nBk,1);
	CDC_SetRectangle(401,60,105,375,g_bEnglish?"Output":"输出",g_nBk,1);
	
	/*if(g_Ini.iBoxStatus)
	{
		CDC_SetRectangle(515,295,275,139,g_bEnglish?"Hand-Box":"手持盒",g_nBk,1);
	}*/
		
	if(g_bEnglish)
		CDC_OutText24Ex(200,20,"Spring Check(Input/Output)",g_TextColor2,g_nBk);
	else
		CDC_OutText24Ex(260,20,"检修测试(输入/输出)",g_TextColor2,g_nBk);
	
	CDC_PutColorBar(519,75,125,24,LIGHTGRAYCOLOR,1);
	CDC_PutColorBar(519,103,269,24,LIGHTGRAYCOLOR,1);
	CDC_PutColorBar(519,163,269,24,LIGHTGRAYCOLOR,1);
	CDC_PutColorBar(519,223,269,24,LIGHTGRAYCOLOR,1);
	
	CDC_Line(650,91,650,269,g_nBk);
	CDC_Line(651,91,651,269,g_nBk);
	
	CDC_Line(515,100,791,100,SILVERCOLOR);
	CDC_Line(515,130,791,130,SILVERCOLOR);
	CDC_Line(515,160,791,160,SILVERCOLOR);
	CDC_Line(515,190,791,190,SILVERCOLOR);
	CDC_Line(515,220,791,220,SILVERCOLOR);
	CDC_Line(515,250,791,250,SILVERCOLOR);
	CDC_Line(515,280,791,280,SILVERCOLOR);
	
	CDC_OutText24Ex(540,75,g_bEnglish?"HPulse:":"手摇脉冲:",BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24Ex(540,103,g_bEnglish?"X-Axis:":"X轴脉冲:",BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24Ex(540,163,g_bEnglish?"Y-Axis:":"Y轴脉冲:",BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24Ex(540,223,g_bEnglish?"Z-Axis:":"Z轴脉冲:",BLACKCOLOR,LIGHTGRAYCOLOR);
	
	CDC_OutText24Ex(672,103,g_bEnglish?"XCode:":"X编码器:",BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24Ex(672,163,g_bEnglish?"YCode:":"Y编码器:",BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24Ex(672,223,g_bEnglish?"ZCode:":"Z编码器:",BLACKCOLOR,LIGHTGRAYCOLOR);
	
	memset(g_InputIO,2,MAXIN);
	memset(g_ProStatus,2,4);
	
	CTestFrm_UpdateAxis(0);
	
	{
		INT16S k=0,j,i;
		INT16S x=0,y=0,w=85,h=36;
	
		for( j=0; j<10; j++)
		{																		//建立10个输出控制矩形区
			x = 411;
			y = 75 + j * h;
			g_ptrOutRect[j].left=x;
			g_ptrOutRect[j].top=y;
			g_ptrOutRect[j].right=x+w-1;
			g_ptrOutRect[j].bottom=y+h-6;

			CTestFrm_DrawOutputStatus(j,g_bOutStatus[j]);						//根据已输出的状态显示
		}		
	}
	
	CMsg_Init();
	CMsg_SetWH(115,448,550,24,YELLOWCOLOR,g_nBk,WHITECOLOR);
	CDC_OutText24Ex(5,447,g_bEnglish?"Message:":"消息提示:",YELLOWCOLOR,g_nBk);
	CTeach_UpdateSpeed(0);
}

void    CTestFrm_UpdateAxis(INT16S iAxis)
{
	CDC_OutText24(540,75,g_bEnglish?"HPulse:":"手摇脉冲:",BLACKCOLOR,iAxis==0?YELLOWCOLOR:LIGHTGRAYCOLOR);
	CDC_OutText24(540,103,g_bEnglish?"X-Axis:":"X轴脉冲:",BLACKCOLOR,iAxis==1?YELLOWCOLOR:LIGHTGRAYCOLOR);
	CDC_OutText24(540,163,g_bEnglish?"Y-Axis:":"Y轴脉冲:",BLACKCOLOR,iAxis==2?YELLOWCOLOR:LIGHTGRAYCOLOR);
	CDC_OutText24(540,223,g_bEnglish?"Z-Axis:":"Z轴脉冲:",BLACKCOLOR,iAxis==3?YELLOWCOLOR:LIGHTGRAYCOLOR);
}

void    CTestFrm_DrawInputStatus(INT16S io,INT16S status)
{
	char str[0x10]="\0";

	if(io <= MAXIN)
	{  																			//一定要注意防止数组越界
		if(g_InputIO[io]==status)
			return;
		
		g_InputIO[io]=status;
	
		switch(io)
		{
			case 0:strcpy(str,g_bEnglish?"XWarn":"X报警");		break;
			case 1:strcpy(str,g_bEnglish?"XZero":"X零点");		break;
			case 2:strcpy(str,g_bEnglish?"XHome":"X原点");		break;
			case 3:strcpy(str,g_bEnglish?"3":"3");				break;
			case 4:strcpy(str,g_bEnglish?"4":"4");				break;
			case 5:strcpy(str,g_bEnglish?"YWarn":"Y报警");		break;
			case 6:strcpy(str,g_bEnglish?"YZero":"Y零点");		break;
			case 7:strcpy(str,g_bEnglish?"YHome":"Y原点"); 		break;
			case 8:strcpy(str,g_bEnglish?"8":"8");				break;
			case 9:strcpy(str,g_bEnglish?"9":"9");				break;

			case 10:strcpy(str,g_bEnglish?"ZWarn":"Z报警");		break;
			case 11:strcpy(str,g_bEnglish?"ZZero":"Z零点");		break;
			case 12:strcpy(str,g_bEnglish?"ZHome":"Z原点");		break;
			case 13:strcpy(str,g_bEnglish?"Stop" :"单条");			break;
			case 14:strcpy(str,g_bEnglish?"Start":"启动");			break;
			case 15:strcpy(str,g_bEnglish?"AWarn":"A报警");		break;
			case 16:strcpy(str,g_bEnglish?"AZero":"A零点");		break;
			case 17:strcpy(str,g_bEnglish?"AHome":"A原点");		break;
			case 18:strcpy(str,g_bEnglish?"18":"18");			break;
			case 19:strcpy(str,g_bEnglish?"19":"19");			break;
		
			case 20:strcpy(str,g_bEnglish?"BWire":"断线");		break;
			case 21:strcpy(str,g_bEnglish?"FWire":"缠线");		break;
			case 22:strcpy(str,g_bEnglish?"22":"22");			break;
			case 23:strcpy(str,g_bEnglish?"23":"23");			break;
			case 24:strcpy(str,g_bEnglish?"+X":"+X限位");		break;
			case 25:strcpy(str,g_bEnglish?"-X":"-X限位");		break;
			case 26:strcpy(str,g_bEnglish?"+Y":"+Y限位");		break;
			case 27:strcpy(str,g_bEnglish?"-Y":"-Y限位");		break;
			case 28:strcpy(str,g_bEnglish?"+Z":"+Z限位");		break;
			case 29:strcpy(str,g_bEnglish?"-Z":"-Z限位");		break;
			case 30:strcpy(str,g_bEnglish?"+U":"+U限位");		break;
			case 31:strcpy(str,g_bEnglish?"-U":"-U限位");		break;
			case 32:strcpy(str,g_bEnglish?"P-Run":"寸动");		break;
			case 33:strcpy(str,g_bEnglish?"Estop":"急停");		break;
			case 34:strcpy(str,g_bEnglish?"HandA":"手轮A");		break;
			case 35:strcpy(str,g_bEnglish?"HandB":"手轮B");		break;
		}	
		
		{
			INT16S x,y;
		
			x = 23 +((io)/10)*85+io/10*5; 
			y = 75 +((io)%10)*36;
			
			CDC_PutColorBar(x,y,85,30,status==1?LIGHTGRAYCOLOR:REDCOLOR,1);
			CDC_Rectangle(x,y,x+85,y+30,BLACKCOLOR);
			CDC_OutText24Ex(x+2,y+2,str,status==1?BLACKCOLOR:WHITECOLOR,status==1?LIGHTGRAYCOLOR:REDCOLOR);
		}
	}
}


void	CTestFrm_DrawOutputStatus(INT16S i,BOOL status)							//输出状态显示函数
{
	char str[10]="\0";
	
	switch(i)
	{
		case 0:	 strcpy(str,g_bEnglish?"OUT1":"输出1");break;
		case 1:	 strcpy(str,g_bEnglish?"OUT2":"输出2");break;
		case 2:	 strcpy(str,g_bEnglish?"OUT3":"输出3");break;
		case 3:	 strcpy(str,g_bEnglish?"OUT4":"输出4");break;
		case 4:	 strcpy(str,g_bEnglish?"OUT5":"输出5");break;
		case 5:	 strcpy(str,g_bEnglish?"OUT6":"输出6");break;
		case 6:	 strcpy(str,g_bEnglish?"OUT7":"输出7");break;
		case 7:	 strcpy(str,g_bEnglish?"OUT8":"输出8");break;
		case 8:	 strcpy(str,g_bEnglish?"OUT9":"输出9");break;
		case 9:  strcpy(str,g_bEnglish?"OUT10":"输出10");break;
	}
	
	CDC_PutColorBar2(g_ptrOutRect[i],status==0?LIGHTGRAYCOLOR:YELLOWCOLOR,1);
	CDC_Rectangle2(g_ptrOutRect[i],g_TextColor1);
	
	CDC_OutCenText24(g_ptrOutRect[i],str,status==0?BLACKCOLOR:BLUECOLOR,status==0?LIGHTGRAYCOLOR:YELLOWCOLOR);
	WriteBit(i,status?1:0);
}


void	CTestFrm_OnKey(INT16U key,INT16U tkey)
{										
	if(key != TBACKZERO && key!=THANDBOX)									//通过按钮也能处理输入输出
	{
		g_iTestAxis = 0;
		key_set_led(XLED,LEDCLOSE);			
		key_set_led(YLED,LEDCLOSE);			
		key_set_led(ZLED,LEDCLOSE);
	}
		
	if( (key >='1' && key <='9')||( key == '0')||( key == '.') )
	{
    	static char disp[0x03]="\0";
    	static int  dispjs=0;
    	
		if(key >='1' && key <='9' && dispjs==0 )
		{
			key -= '1';
			g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
			CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);
		}
		else
		{
			if(key == '0')
				key = '0';

    		disp[dispjs++] = key;
    		dispjs = dispjs%2;
    		
        	if(strcmp(disp,"00")==0)    
        		key=9;
        		
/*        	if(strcmp(disp,"01")==0)    
        		key=10;
        		
        	if(strcmp(disp,"02")==0)    
        		key=11;
        		
        	if(strcmp(disp,"03")==0)   
        		key=12;
        		
        	if(strcmp(disp,"04")==0)    
        		key=13;
        		
			if(strcmp(disp,"05")==0)    
				key=14;
				
        	if(strcmp(disp,"06")==0)    
        		key=15;
        		
        	if(strcmp(disp,"07")==0)    
        		key=16;
        		
        	if(strcmp(disp,"08")==0)    
        		key=17;
        		
        	if(strcmp(disp,"09")==0)    
        		key=18;
*/
            if(dispjs==0)  
            	memset(disp,NULL,3);
            	
			g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
			CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);					//10号输出点实际写的点位是9
		}
	}

	switch(key)
	{
	#ifdef	PRINTBMP															//屏幕截图 启动键
		case F5:
			{
				char   filename[0x20];
				INT16S i=0;
				
//				CBmp_Init();
				strcpy(filename,"testfrm");
				
				for(i=0; i<strlen(filename); i++)
				{
					if(filename[i] == '.')
					{
						filename[i]='\0';
						break;
					}
				}
				
				strcat(filename,".bmp");
				CreatBMP(filename,800,480,1);
//				CBmp_Destroy();
				OSQPost(MsgQueue,"Write Bmp End!");
			}
			break;
	#endif	

		case TCLEAR:
			{
				INT16S i=0;
				
				for(i=0; i<4; i++)
				{
					set_command_pos(i+1,0);
					set_actual_pos(i+1,0);
				}
				
				g_bFirstResetX=FALSE;
				g_bFirstResetY=FALSE;
				g_bFirstResetZ=FALSE;
				g_bFirstResetU=FALSE;
			}
			break;
			
		case TX :
			g_iTestAxis = 1;
			CTestFrm_UpdateAxis(g_iTestAxis);	
			UpdateCurAxis(g_iTestAxis);	
			/*	
			key_set_led(XLED,LEDOPEN);			
			key_set_led(YLED,LEDCLOSE);			
			key_set_led(ZLED,LEDCLOSE);
			*/
			SetMM(7,0,FALSE);
			SetMM(8,0,FALSE);
			break;
			
		case TY :
			g_iTestAxis = 2;
			CTestFrm_UpdateAxis(g_iTestAxis);
			UpdateCurAxis(g_iTestAxis);	
			/*			
			key_set_led(XLED,LEDCLOSE);			
			key_set_led(YLED,LEDOPEN);			
			key_set_led(ZLED,LEDCLOSE);
			*/
			SetMM(7,0,FALSE);
			SetMM(8,0,FALSE);
			break;
			
		case TZ :
			if(g_Ini.iAxisCount>2)
			{
				g_iTestAxis = 3;
				CTestFrm_UpdateAxis(g_iTestAxis);	
				UpdateCurAxis(g_iTestAxis);	
				/*			
				key_set_led(XLED,LEDCLOSE);			
				key_set_led(YLED,LEDCLOSE);			
				key_set_led(ZLED,LEDOPEN);
				*/
				SetMM(7,0,FALSE);
				SetMM(8,0,FALSE);
			}
			else
			{
				OSQPost(MsgQueue,g_bEnglish?"AxisCount=2,Z Axis can not be selected":"轴数为2,Z轴不能被选择...");
				OSTimeDly(1000);
				OSQPost(MsgQueue,"");
				g_iTestAxis = 0;
				CTestFrm_UpdateAxis(g_iTestAxis);
				UpdateCurAxis(g_iTestAxis);	
			}			
			break;
			
		case TBACKZERO:
			g_bBackZero = TRUE;
			
			if(g_iTestAxis==1)
				CTestFrm_FindZero(1);

			else if(g_Ini.iAxisCount>2 && g_iTestAxis==3)
			{
				if(g_Ini.iAxisRunMode[2]==0||g_Ini.iAxisRunMode[2]==1)
					CTestFrm_FindZero(3);
				else
					OSQPost(MsgQueue,g_bEnglish?"Please select the axis...":"Z轴丝杆模式无法寻零点...");
					OSTimeDly(1000);
					OSQPost(MsgQueue,"");
			}	
			else
			{
				OSQPost(MsgQueue,g_bEnglish?"Please select the axis...":"请选则合适轴...");
				OSTimeDly(1000);
				OSQPost(MsgQueue,"");
			}	
			
			g_bBackZero = FALSE;
			break;

		case TSCREEN:case TCANCEL:
			Ok_Exit=TRUE;
			break;
	
		case THANDBOX:
			{		
				CTeach_UpdateSpeed(1);
				
				if(g_iSpeedBeilv==3)
				{
					key_set_led(HANDBOXLED,LEDOPEN);
				}
				else
				{
					key_set_led(HANDBOXLED,LEDCLOSE);
				}
				break;
			}	
						
		default:
			CTestFrm_UpdateAxis(g_iTestAxis);
			break;
	}
}


void	CTestFrm_FindZero(INT16S ch)
{

	char buf[0x30]="\0";
	BOOL   bStop= FALSE;
	INT32S 	DestPulse =990000L;
	SetMM(ch,0,TRUE);
	
	switch(ch)
	{
		case 1:StringCopy(buf,g_bEnglish?"XFind Zero":"X寻找零点");break;
		case 2:StringCopy(buf,g_bEnglish?"YFind Zero":"Y寻找零点");break;
		case 3:StringCopy(buf,g_bEnglish?"ZFind Zero":"Z寻找零点");break;
		case 4:StringCopy(buf,g_bEnglish?"AFind Zero":"A寻找零点");break;
		default:break;
	}
	
	OSQPost(MsgQueue,buf);
	
	SetMM(ch,0,TRUE);
	
	SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]);
	
	pmove(ch,DestPulse);
	
	while(CheckHome(ch))												//这个while函数的功能？
	{
	 	OSTimeDly(1);
		
		if(RealIO(STOPIO,0,2))
		{
			Stop1(ch);
			bStop = TRUE;
			goto FINDEND;
		}
	} 
	
	OSTimeDly(3);

	set_stop0_mode(ch,1,0);												//停止指令有效，并且低电平有效。触碰到原点信号停止
	
	while(TRUE)
	{
		if(IsFree(ch))
		{
			goto FINDEND;
		}
		
		if(RealIO(STOPIO,0,2))
		{
			Stop1(ch);
			bStop = TRUE;
			goto FINDEND;
		}
		
		OSTimeDly(1);
	}
	
	
FINDEND:

	set_stop0_mode(ch,0,0);
		
	if(bStop)
	{
		OSQPost(MsgQueue,"寻零终止!");
	}
	else if(labs(DestPulse-GetPulse(ch,TRUE))<3)
	{
		OSQPost(MsgQueue,"未找到原点!");
	}
	else
	{
		OSQPost(MsgQueue,"寻零完成!");
		OSTimeDly(1000);
		OSQPost(MsgQueue,"");
	}
	

	
	switch(ch)
	{
		case 1:g_bFirstResetX=FALSE;break;
		case 2:g_bFirstResetY=FALSE;break;
		case 3:g_bFirstResetZ=FALSE;break;
		case 4:g_bFirstResetU=FALSE;break;
		default:break;
	}

	/*
	char buf[0x30];
	
	SetMM(ch,0,TRUE);
	
	switch(ch)
	{
		case 1:StringCopy(buf,g_bEnglish?"XFind Zero":"X寻找零点");break;
		case 2:StringCopy(buf,g_bEnglish?"YFind Zero":"Y寻找零点");break;
		case 3:StringCopy(buf,g_bEnglish?"ZFind Zero":"Z寻找零点");break;
		case 4:StringCopy(buf,g_bEnglish?"AFind Zero":"U寻找零点");break;
		default:break;
	}
	
	OSQPost(MsgQueue,buf);
	
	if(BackZero(ch,g_Ini.iBackDir[ch-1]))
	{
		OSQPost(MsgQueue,g_bEnglish?"Zero Finished,Press any key...":"当前轴归零完成,按任意键测轴脉冲...");
		OSTimeDly(200);
		GetKey(FALSE);
		SetMM(ch,0,TRUE);
		
		SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]);
		
		pmove(ch,GetPulseFromMM(ch,g_Ini.fMM[ch-1]));
		
		while(TRUE)
		{
			if(IsFree(ch))
			{
				break;
			}
			
			if(RealIO(STOPIO,0,2))
			{
				Stop1(ch);
			}
			
			OSTimeDly(1);
		}
	}
	else
	{
		OSQPost(MsgQueue,g_bEnglish?"Zero Error,Press any key...":"寻找零点错误,按任意键继续");
		OSTimeDly(200);
		GetKey(FALSE);
	}
	
	switch(ch)
	{
		case 1:g_bFirstResetX=FALSE;break;
		case 2:g_bFirstResetY=FALSE;break;
		case 3:g_bFirstResetZ=FALSE;break;
		case 4:g_bFirstResetU=FALSE;break;
		default:break;
	}
	
	OSQPost(MsgQueue," ");
	*/
}


INT16S	CTestFrm_DoModal()
{
	INT16U  key,tkey=0;
	INT8S   js=0;
	INT8S   i=0;
	INT16S 	x,y;
	char 	str[10]="探针1";
	char 	stre[10]="Prob1";
	char	status[4]={0,0,0,0};
	
	CTestFrm_Create();
	
	ResumeTasks(5);

	OSTimeDly(20);

	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			CTestFrm_OnKey(key,tkey);
			
			if(Ok_Exit)
			{
				//CTeach_Create();
				break;
			}
		}
		
		switch(js)
		{
			case 0:
				CTestFrm_DrawInputStatus(js,ReadBit(41));
				break;
				
			case 1:
				CTestFrm_DrawInputStatus(js,ReadBit(68));
				break;
				
			case 5:
				CTestFrm_DrawInputStatus(js,ReadBit(36));
				break;
				
			case 6:
				CTestFrm_DrawInputStatus(js,ReadBit(69));
				break;
			
			case 10:
				CTestFrm_DrawInputStatus(js,ReadBit(15));
				break;
				
			case 11:
				CTestFrm_DrawInputStatus(js,ReadBit(70));
				break;
				
			case 15:
				CTestFrm_DrawInputStatus(js,ReadBit(10));
				break;
				
			case 16:
				CTestFrm_DrawInputStatus(js,ReadBit(71));
				break;
			
			case 32:
				CTestFrm_DrawInputStatus(js,ReadBit(65));
				break;
				
			case 33:
				CTestFrm_DrawInputStatus(js,ReadBit(62));
				break;
				
			case 34:
				CTestFrm_DrawInputStatus(js,ReadBit(66));
				break;
			
			case 35:
				CTestFrm_DrawInputStatus(js,ReadBit(67));
				break;
				
			default:
				CTestFrm_DrawInputStatus(js,ReadBit(js));
				break;
		}

		js++;
		js=(js)%MAXIN;
			
		for( i=0; i<4; i++)
		{
			x= 23+((i+36)/10)*85+15;;
			y= 75+((i+36)%10)*36;	
					
			OpenProbe(0,i+1,i+1);
			
			str[4] = '1'+i;
			stre[4] = '1'+i;
			
			status[i] = ReadProbe(0,i+1);
			
			if(g_ProStatus[i] != status[i])
			{	
				CDC_PutColorBar(x,y,85,30,status[i]==0 ? LIGHTGRAYCOLOR:REDCOLOR,1);
				CDC_Rectangle(x,y,x+85,y+30,BLACKCOLOR);
				CDC_OutText24Ex(x+2,y+2,g_bEnglish?stre:str,status[i]==0?BLACKCOLOR:WHITECOLOR,status[i]==0?LIGHTGRAYCOLOR:REDCOLOR);
				
				g_ProStatus[i]=status[i];
			}
			
			CloseProbe(0);
		}
		
		if(g_iTestAxis != 0)
			CTestFrm_HandMove(g_iTestAxis);
		
		OSTimeDly(2);
	}
	
	Ok_Exit=FALSE;	
	return g_iDlgIDOk;
}


void	CTestFrm_HandMove(INT16U ch)
{	
	FP32 	pL[3] = {0.2,0.6,1.0};	
	
	if(ch<g_Ini.iAxisCount+1 && ch!=0)
	{
		INT32S pos=0;
		
		pos = GetPulse(8,FALSE) + (g_Ini.iBoxStatus ? GetPulse(7,FALSE):0);
		
		if(labs(pos)>1)
		{
			g_fSpeedBi = 1 ;
			
			switch(ch)
			{
				case 1:case 3:case 4:
				
					if(CheckHardLimit(ch,pos) || CheckSoftLimit(ch,pos))
					{
						OSTimeDly(100);
						SetMM(7,0,FALSE);
						SetMM(8,0,FALSE);
						g_bHandMove = FALSE;
						g_iMoveAxis = 0;
						return;
					}				
					
					ShortlyPmove(ch,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[ch-1]*GetUnitPulse(ch));

					while (1)
					{
						if(g_Ini.iLag[ch-1]!=0 && (labs(GetPulse(ch,TRUE)*g_Ini.iNumerator[ch-1]-GetPulse(ch,FALSE)*g_Ini.iDenominator[ch-1]) >= g_Ini.iLag[ch-1]))
						{
							Stop1(ch);
							g_iTestAxis = 0;
							OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
							break;
						}
						
						if(CheckHardLimit(ch,pos) || CheckSoftLimit(ch,pos))
						{
							Stop1(ch);
							break;
						}							
						
						if(IsFree(ch)) 
							break;
						
						OSTimeDly(1);
					}

					break;


				case 2:
					
					if(g_Ini.iYDaoXian)
					{
						ShortlyPmove(ch,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[ch-1]*GetUnitPulse(ch));

						while (1)
						{
							if(g_Ini.iLag[ch-1]!=0 && (labs(GetPulse(ch,TRUE)*g_Ini.iNumerator[ch-1]-GetPulse(ch,FALSE)*g_Ini.iDenominator[ch-1]) >= g_Ini.iLag[ch-1]))
							{
								Stop1(ch);
								g_iTestAxis = 0;
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
								break;
							}
							
							if (IsFree(ch)) 
								break;
							
							OSTimeDly(1);
						}

					}
					else
					{
						if(pos>0 )
						{
							ShortlyPmove(ch,(pos>0)?10000000L:-10000000L,pL[g_iSpeedBeilv-1]*g_Ini.fHandSpeed[ch-1]*GetUnitPulse(ch));

							while (1)
							{
								if(g_Ini.iLag[ch-1]!=0 && (labs(GetPulse(ch,TRUE)*g_Ini.iNumerator[ch-1]-GetPulse(ch,FALSE)*g_Ini.iDenominator[ch-1]) >= g_Ini.iLag[ch-1]))
								{
									Stop1(ch);
									g_iTestAxis = 0;
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;
								}
								
								if (IsFree(ch)) 
									break;
								
								OSTimeDly(1);
							}
						}
						else
						{									
							SetMM(7,0,FALSE);
							SetMM(8,0,FALSE);
						}
					}
					break;
					
				default:
					break;
			}

		}					
	}
}
