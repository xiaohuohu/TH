# include	"includes.h"

extern		BOOL		g_lHandRun;
extern		INT32S		g_iWorkStatus;
extern 		BASEINI		g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	OS_EVENT	*FreeSem;

WORK_EXT 	BOOL		g_bFirstResetX;							//指示X轴是否复位，FALSE为需要复位
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;

WORK_EXT	INT16S		g_iMoveAxis;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	BOOL		g_bHandset;
WORK_EXT	BOOL		g_bCylinder; 
//WORK_EXT	BOOL		g_bFunc;

#define		MAXIN	 		56
#define 	MAXOUT			10



static      CRect		g_ptrOutRect[MAXOUT];
BOOL		g_bOutStatus[MAXOUT];
BOOL		g_bTestFrm=FALSE;
char 		g_InputIO[MAXIN];
char 		g_ProStatus[8];
INT16U		g_iTestAxis;
INT16U		g_iTestBoxAxis=0;
BOOL        g_handAxis=FALSE;									//表示各轴手轮是否打开，打开状态下可以选轴后进行驱动。


/***************************************
	输入输出检测

***************************************/

void	CTestFrm_Init()
{
	INT16S i=0;
	
	g_bTestFrm=TRUE;
	
	
		
	for(i=0;i<MAXOUT;i++)
		g_bOutStatus[i]=GetOut(i);	
		

	
	for(i=0; i<MAXCH; i++)
	{
		SetCommandPos(i+1,0);
		SetActualPos(i+1,0);
	}
				
	AllLedOff();
	LedOut(STEPLED,0);			//关闭"手盒"指示灯,退出测试界面时再根据g_bHandset更新灯的状态。

}



void    CTestFrm_Frame_Create()
{
	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),g_TextColor3,1);
	
	CDC_OutText_UTF(6,getmaxy()-31,g_bEnglish?"Msg:":"讯息:",g_TextColor1,g_TextColor3,KZK_24);
	CMsg_SetWH(60,566,600,28,g_TextColor11,g_TextColor3,g_TextColor3);
	
	CDC_Rectangle(0,0,getmaxx(),getmaxy(),g_TextColor10);
	CDC_Rectangle(1,1,getmaxx()-1,getmaxy()-1,g_TextColor10);
	CDC_Rectangle(2,2,getmaxx()-2,getmaxy()-2,g_TextColor10);
	
	CDC_PutColorBar(3,3,getmaxx()-6,32,g_nTableBk,100);
		
	CDC_Line(3,33,getmaxx()-3,33,g_TextColor10);
	CDC_Line(3,34,getmaxx()-3,34,g_TextColor10);
	CDC_Line(3,35,getmaxx()-3,35,g_TextColor10);
	
	if(g_bEnglish)
		CDC_OutText24(215,4,"Spring Input/Output Check",g_TextColor11,g_nTableBk);
	else
		CDC_OutText_UTF(220,4,"弹簧机输入/输出检修测试",g_TextColor11,g_nTableBk,KZK_24);
}

void	CTestIoFrm_Create()
{	
	int i;
	AllLedOff();
	
	g_handAxis=FALSE;
	LedOut(HANDLED,g_handAxis?1:0);	
	OSQPost(MsgQueue,g_bEnglish?"Press Key \"HandSwitch\" to Handle Aixs.":"按 \"各轴手轮\" 进行手轮驱动");
	
	CDC_PutColorBar(3,36,getmaxx()-6,530,g_TextColor3,1);
	
	CDC_SetRectangle3(10,56,540,500,g_bEnglish?"Input":"输入",g_bEnglish?75:55,g_TextColor3,1);
	CDC_SetRectangle3(560,56,230,240,g_bEnglish?"Output":"输出",g_bEnglish?95:55,g_TextColor3,1);
		
	CDC_OutText_UTF(30,g_bEnglish?492:490, g_bEnglish?"TestKnob:":"测试旋钮:",g_TextColor11,g_TextColor3,KZK_22);
	CDC_OutText_UTF(265,g_bEnglish?492:490,g_bEnglish?"HandPulse:":"手摇脉冲:",g_TextColor11,g_TextColor3,KZK_22);
	CDC_OutText_UTF(30,g_bEnglish?522:520,g_bEnglish?"BoxPulse:":"手盒脉冲:",g_TextColor11,g_TextColor3,KZK_22);
	CDC_OutText_UTF(265,g_bEnglish?522:520,g_bEnglish?"LagPulse:":"滞后脉冲:",g_TextColor11,g_TextColor3,KZK_22);
	
	CDC_Rectangle(560,304,790,556,WHITECOLOR);
	
	for(i=1;i<7;i++)
		CDC_Line(560,304+36*i,790,304+36*i,WHITECOLOR);
	
	
	CDC_Line(675,304,675,556,WHITECOLOR);
	
	CDC_OutText_UTF(571,310,g_bEnglish?"Logical":"逻辑位置",g_TextColor11,g_TextColor3,KZK_24);
	CDC_OutText_UTF(681,310,g_bEnglish?"Actual":"实际位置",g_TextColor11,g_TextColor3,KZK_24);
	
	CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff ":"手轮关",REDCOLOR,g_TextColor3,KZK_22);
	
	memset(g_InputIO,2,MAXIN);
	memset(g_ProStatus,2,8);
	
	{
		INT16S k=0,j,i;
		INT16S x=0,y=0,w=85,h=43;
		
		for( j=0; j<5; j++)
		{//建立10个输出控制矩形区
			for(i=0; i<2; i++,k++)
			{
				x = 594 + i * w;
				y = 80 + j * h;
				
				//Uart_Printf("k=%d\n",k);
				g_ptrOutRect[k].left=x;
				g_ptrOutRect[k].top=y;
				g_ptrOutRect[k].right=x+w-8;
				g_ptrOutRect[k].bottom=y+h-12;
				//SetRect(x,y,x+w-5,y+h-10);
				CTestFrm_DrawOutputStatus(k,g_bOutStatus[k]);//根据已输出的状态显示
			}
		}

	}
	
	{
		INT16S j;
		
		for(j=0;j<g_Ini.iAxisCount;j++)
		{
			CTestFrm_ClearComActPos(j+1);
		}
	}
}


void	CTestFrm_Destroy()
{
	g_bTestFrm=FALSE;
	CMsg_SetWH(60,566,600,28,g_TextColor11,g_nBk,g_nBk);
	
	g_handAxis=FALSE;
	
	BackZeroLed();
	LedOut(STOPLED,1);
	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
	LedOut(CYLINDERLED,g_bCylinder?1:0);	
	LedOut(STEPLED,g_bHandset?1:0);	
	
	LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]?0:1);
	LedOut(ZSWITCHLED,g_Ini.iAxisCount>2?(g_Ini.iAxisSwitch[2]?0:1):0);
	LedOut(USWITCHLED,g_Ini.iAxisCount>3?(g_Ini.iAxisSwitch[3]?0:1):0);
	LedOut(VSWITCHLED,g_Ini.iAxisCount>4?(g_Ini.iAxisSwitch[4]?0:1):0);
	LedOut(WSWITCHLED,g_Ini.iAxisCount>5?(g_Ini.iAxisSwitch[5]?0:1):0);
	UpdateCurAxis();
}


void    CTestFrm_DrawInputStatus(INT16S io,INT16S status)
{
	char str[0x10]="\0";
	if(io<=MAXIN)
	{   //一定要注意防止数组越界
		if(g_InputIO[io]==status)return;
		g_InputIO[io]=status;
		
	//	Uart_Printf("a=%d b=%d\n",io,status);
		
		switch(io)
		{
			case 0:strcpy(str,g_bEnglish?"XAlm":"X报警");		break;
			case 1:strcpy(str,g_bEnglish?"XOrg":"X零点");		break;
			case 2:strcpy(str,g_bEnglish?"XHom":"X原点");		break;
			case 3:strcpy(str,g_bEnglish?"    3":"3");			break;
			case 4:strcpy(str,g_bEnglish?"    4":"4");			break;
			
			case 5:strcpy(str,g_bEnglish?"YAlm":"Y报警");		break;
			case 6:strcpy(str,g_bEnglish?"YOrg":"Y零点");		break;
			case 7:strcpy(str,g_bEnglish?"YHom":"Y原点");		break;
			case 8:strcpy(str,g_bEnglish?"    8":"8");			break;
			case 9:strcpy(str,g_bEnglish?"    9":"9");			break;
			
			case 10:strcpy(str,g_bEnglish?"ZAlm":"Z报警");		break;
			case 11:strcpy(str,g_bEnglish?"ZOrg":"Z零点");		break;
			case 12:strcpy(str,g_bEnglish?"ZHom":"Z原点");		break;
			case 13:strcpy(str,g_bEnglish?"BckW":"退线");		break;
			case 14:strcpy(str,g_bEnglish?"SndW":"送线");		break;
			
			case 15:strcpy(str,g_bEnglish?"AAlm":"A报警");		break;
			case 16:strcpy(str,g_bEnglish?"AOrg":"A零点");		break;
			case 17:strcpy(str,g_bEnglish?"AHom":"A原点");		break;
			case 18:strcpy(str,g_bEnglish?"    18":"跑线");		break;
			case 19:strcpy(str,g_bEnglish?"    19":"19");		break;
			
			case 20:strcpy(str,g_bEnglish?"BAlm":"B报警");		break;
			case 21:strcpy(str,g_bEnglish?"BOrg":"B零点");		break;
			case 22:strcpy(str,g_bEnglish?"BHom":"B原点");		break;
			
			case 23:strcpy(str,g_bEnglish?"CAlm":"C报警");		break;
			case 24:strcpy(str,g_bEnglish?"COrg":"C零点");		break;
			case 25:strcpy(str,g_bEnglish?"CHom":"C原点");		break;
			
			case 26:strcpy(str,g_bEnglish?"XLmt+":"X正限");		break;
			case 27:strcpy(str,g_bEnglish?"XLmt-":"X负限");		break;			
			case 28:strcpy(str,g_bEnglish?"YLmt+":"Y正限");		break;
			case 29:strcpy(str,g_bEnglish?"YLmt-":"Y负限");		break;			
			case 30:strcpy(str,g_bEnglish?"ZLmt+":"Z正限");		break;
			case 31:strcpy(str,g_bEnglish?"ZLmt-":"Z负限");		break;			
			case 32:strcpy(str,g_bEnglish?"ALmt+":"A正限");		break;
			
			case 33:strcpy(str,g_bEnglish?"ALmt-":"A负限");		break;			
			case 34:strcpy(str,g_bEnglish?"BLmt+":"B正限");		break;
			case 35:strcpy(str,g_bEnglish?"BLmt-":"B负限");		break;
			case 36:strcpy(str,g_bEnglish?"CLmt+":"C正限");		break;
			case 37:strcpy(str,g_bEnglish?"CLmt-":"C负限");		break;
			case 38:strcpy(str,g_bEnglish?"BrkW":"断线");		break;
			case 39:strcpy(str,g_bEnglish?"FulW":"缠线");		break;	
			case 40:strcpy(str,g_bEnglish?"Start":"启动");		break;
			case 41:strcpy(str,g_bEnglish?"Stop1":"停止");		break;
			case 42:strcpy(str,g_bEnglish?"HndA":"手轮A");		break;
			case 43:strcpy(str,g_bEnglish?"HndB":"手轮B");		break;
			
			case 44:strcpy(str,g_bEnglish?"BoxA":"手盒A");		break;
			case 45:strcpy(str,g_bEnglish?"BoxB":"手盒B");		break;
			case 46:strcpy(str,g_bEnglish?"Low ":"低速");		break;
			case 47:strcpy(str,g_bEnglish?"Mid ":"中速");		break;
			case 48:strcpy(str,g_bEnglish?"High":"高速");		break;
			case 49:strcpy(str,g_bEnglish?"Stop2":"急停2");		break;
			case 50:strcpy(str,g_bEnglish?"    X":"X轴");		break;
			case 51:strcpy(str,g_bEnglish?"    Y":"Y轴");		break;
			case 52:strcpy(str,g_bEnglish?"    Z":"Z轴");		break;
			case 53:strcpy(str,g_bEnglish?"    A":"A轴");		break;
			
			case 54:strcpy(str,g_bEnglish?"    B":"B轴");		break;
			case 55:strcpy(str,g_bEnglish?"    C":"C轴");		break;
			
			
			
		}
		
		{
			INT16S x,y;
			
			{
				CRect rc;
			
				
				x= 25 +(io/11)*85;
				
				y=80+(io%11)*37;
				
				
				rc.left = x;
				rc.top = y;
				rc.right = x+80;
				rc.bottom = y+30;
				
				CDC_PutColorBar(x,y,80,30,status==0?REDCOLOR:BLUECOLOR,1);
			
				if(!g_bEnglish)
				{
					CDC_OutCenTextKzk22(rc, str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR);
				}
				else
				{
					CDC_OutTextLR_UTF(rc.left+3,rc.top+6,str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR,KZK_22,0,1);
				}
				
				CDC_Rectangle4(x,y,x+80,y+32,YELLOWCOLOR);

			}
		}
	}
}


void	CTestFrm_DrawOutputStatus(INT16S i,BOOL status)		//输出状态显示函数
{
	char str[10]="\0";
	
	switch(i)
	{
		case 0:	strcpy(str,g_bEnglish?"Cyld1":"汽缸1");	break;
		case 1:	strcpy(str,g_bEnglish?"Cyld2":"汽缸2");	break;
		case 2:	strcpy(str,g_bEnglish?"Cyld3":"汽缸3");	break;
		case 3:	strcpy(str,g_bEnglish?"Cyld4":"汽缸4");	break;
		case 4:	strcpy(str,g_bEnglish?"Cyld5":"汽缸5");	break;
		case 5:	strcpy(str,g_bEnglish?"Cyld6":"汽缸6");	break;
		case 6:	strcpy(str,g_bEnglish?"Cyld7":"汽缸7");	break;
		case 7:	strcpy(str,g_bEnglish?"Cyld8":"线架");	break;
		case 8:	strcpy(str,g_bEnglish?"Cyld9":"汽缸9");	break;
		case 9: strcpy(str,g_bEnglish?"Warn":"报警");	break;
	}
	
	
	CDC_PutColorBar2(g_ptrOutRect[i],status==0?BLUECOLOR:REDCOLOR,1);
	CDC_Rectangle3(g_ptrOutRect[i],YELLOWCOLOR);
	CDC_OutCenTextKzk22(g_ptrOutRect[i],str,WHITECOLOR,status==0?BLUECOLOR:REDCOLOR);
	WriteBit(i,status?1:0);
}


void	CTestFrm_OnKey(INT16U key,INT16U tkey)
{
	INT16S i=-1;//,j=0;
	INT8S buffMsg[80]=""; 
	
	
	{
		if( key >='0' && key <='9' )
		{
			if(key>='1')
			{
				key -= '1';
				g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
				CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);
			}
			else
			{
				key = 9;
				g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
				CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);
			}
		}

	}
	
	switch(key)
	{
	
	case TBACKZERO:
	
		{
		#ifdef	PRINTBMP
			{
				char filename[0x20];
				INT16S i=0;
				
				
				strcpy(filename,"testFrm");
				
				for(i=0;i<strlen(filename);i++)
				{
					if(filename[i]=='.')
					{
						filename[i]='\0';
						break;
					}
				}
				strcat(filename,".bmp");
				CreatBMP(filename,800,600,1);
				
				OSQPost(MsgQueue,"Write Bmp End!");
			}
			
			break;
			#endif
		}
		
		
		{
			INT16U	i = 0;
		
			for(i=0; i<MAXCH; i++)
			{
				SetCommandPos(i+1,0);
				SetActualPos(i+1,0);
			}
			
			OSQPost(MsgQueue," ");
		}
		break;
		
	case TYSWITCH :	i=1;break;
	case TZSWITCH :	i=2;break;
	case TUSWITCH :	i=3;break;
	case TVSWITCH :	i=4;break;
	case TWSWITCH :	i=5;break;
	case TCYLINDER: i=6;break;
	case TSTOP 	  :	i=7;break;
	
	
	
	case TCANCEL: 
		{
				Ok_Exit=TRUE;
		}
		break;
	
	
	
	case TXPLUS :
		
		{
			
			{
				g_iTestAxis =0;
				CTestFrm_ClearComActPos(1);			//清除显示的字符串，避免残留
				CTestFrm_FindZero(1);
			}
		}
		break;
	
		
	case TZPLUS :
		
		{
			if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
				break;
			
			if( g_Ini.iAxisRunMode[2] != 2)
			{
				g_iTestAxis =0;
				CTestFrm_ClearComActPos(3);			//清除显示的字符串，避免残留
				CTestFrm_FindZero(3);
			}
			
		}
		break;
		
	case TUPLUS :
		
		{
			if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
				break;
			
			if( g_Ini.iAxisRunMode[3] != 2)
			{	
				g_iTestAxis =0;
				CTestFrm_ClearComActPos(4);			//清除显示的字符串，避免残留
				CTestFrm_FindZero(4);
			}
		}
		break;
		
	case TVPLUS :
		
		{
			if(g_Ini.iAxisCount<5 || g_Ini.iAxisSwitch[4]==0)
				break;
				
			if( g_Ini.iAxisRunMode[4] != 2)
			{	
				g_iTestAxis =0;
				CTestFrm_ClearComActPos(5);			//清除显示的字符串，避免残留
				CTestFrm_FindZero(5);
			}
		}
		break;
		
	case TWPLUS :
	
		
		{
			if(g_Ini.iAxisCount<6 || g_Ini.iAxisSwitch[5]==0)
				break;
			
			if( g_Ini.iAxisRunMode[5] != 2)
			{	
				g_iTestAxis =0;
				CTestFrm_ClearComActPos(6);			//清除显示的字符串，避免残留
				CTestFrm_FindZero(6);
			}
		}
		break;
		
	
	
	case TXMINUS :
		
		{
			if( g_handAxis)
			{
				SetPulse(7,0,FALSE);
				SetPulse(8,0,FALSE);
				g_iTestAxis = 1;
				CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
				
				strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
				OSQPost(MsgQueue,strcat(buffMsg,axisMsg(1,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
				CTestFrm_ClearComActPos(g_iTestAxis);
			}
		}
		break;
		
	case TYMINUS :
		
		{
			if(g_handAxis)
			{	
				SetPulse(7,0,FALSE);
				SetPulse(8,0,FALSE);
				g_iTestAxis = 2;
				CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
				
				strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
				OSQPost(MsgQueue,strcat(buffMsg,axisMsg(2,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
				CTestFrm_ClearComActPos(g_iTestAxis);
			}
		}
		break;
		
	case TZMINUS :
		
		{
			if(g_Ini.iAxisCount>2 && g_Ini.iAxisSwitch[2]==1)
			{
				if( g_handAxis)
				{
					SetPulse(7,0,FALSE);
					SetPulse(8,0,FALSE);
					g_iTestAxis = 3;
					CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
					
					strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
					OSQPost(MsgQueue,strcat(buffMsg,axisMsg(3,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
					CTestFrm_ClearComActPos(g_iTestAxis);
				}
			}
		}
		break;
		
	case TUMINUS :
		
		{
			if(g_Ini.iAxisCount>3 && g_Ini.iAxisSwitch[3]==1)
			{
				if( g_handAxis)
				{
					SetPulse(7,0,FALSE);
					SetPulse(8,0,FALSE);
					g_iTestAxis = 4;
					CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
					
					strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
					OSQPost(MsgQueue,strcat(buffMsg,axisMsg(4,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
					CTestFrm_ClearComActPos(g_iTestAxis);
				}
			}
		}
		break;
		
	case TVMINUS :
		
		{
			if(g_Ini.iAxisCount>4 && g_Ini.iAxisSwitch[4]==1)
			{
				if( g_handAxis)
				{	
					SetPulse(7,0,FALSE);
					SetPulse(8,0,FALSE);
					g_iTestAxis = 5;
					CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
					
					strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
					OSQPost(MsgQueue,strcat(buffMsg,axisMsg(5,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
					CTestFrm_ClearComActPos(g_iTestAxis);
				}
			}
		}
		break;
		
	case TWMINUS :
		
		{
			if(g_Ini.iAxisCount>5 && g_Ini.iAxisSwitch[5]==1)
			{
				if(g_handAxis)
				{	
					SetPulse(7,0,FALSE);
					SetPulse(8,0,FALSE);
					g_iTestAxis = 6;
					CDC_PutColorBar(g_bEnglish?400:380,489,140,30,g_TextColor3,1);
					
					strcpy(buffMsg,g_bEnglish?"Enter ":"进入");
					OSQPost(MsgQueue,strcat(buffMsg,axisMsg(6,g_bEnglish?" Hand-Move State.":"手轮驱动",TRUE)));
				
					CTestFrm_ClearComActPos(g_iTestAxis);
				}
			}
		}
		break;
		
	
	
	case THANDSWITCH :
	{	
		{
			
			{	
				if(!g_handAxis)
				{
					g_handAxis=TRUE;
					LedOut(HANDLED,g_handAxis?1:0);	
					OSQPost(MsgQueue,g_bEnglish?"Press \"-X,-Y,-Z,-A,-B,-C\" to enter handmove state":"进入手轮驱动,按 \"-轮,-线,-芯,-取,-取,-耳\" 进行选轴");
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOn ":"手轮开",g_TextColor11,g_TextColor3,KZK_22);	
				}
				else
				{	
					g_handAxis=FALSE;
					g_iTestAxis=0;
					LedOut(HANDLED,g_handAxis?1:0);	
					OSQPost(MsgQueue,g_bEnglish?"Exit Hand-Move State":"退出各轴手轮驱动");
					CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff ":"手轮关",REDCOLOR,g_TextColor3,KZK_22);
				}
			}
		}
		
		break;
	}	
	
	default:		
		return;
	
	}
	
	if(RealIO(EXT_SINGLEDI,0,2))
	{
		i=7;
	}
	
	//if(1==g_iKeyTestChoose && !g_bSwitchFrm)
	{
		g_iTestAxis==1?LedOut(XLED,1):LedOut(XLED,0);
		g_iTestAxis==2?LedOut(YLED,1):LedOut(YLED,0);
		g_iTestAxis==3?LedOut(ZLED,1):LedOut(ZLED,0);
		g_iTestAxis==4?LedOut(ULED,1):LedOut(ULED,0);
		g_iTestAxis==5?LedOut(VLED,1):LedOut(VLED,0);
		g_iTestAxis==6?LedOut(WLED,1):LedOut(WLED,0);
	}
		
}


void	CTestFrm_FindZero(INT16S ch)
{
	char buf[0x30];
	//char axis[0x10];
	SetMM(ch,0,TRUE);
	
	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" FindZero...":"寻找零点...",TRUE));
	
	
	
	{
		if(CTestFrm_BackZero(ch,g_Ini.iBackDir[ch-1]))
		{
			sprintf(buf,g_bEnglish?"%s FindZero Finished,AxisPulse:%5ld":"%s归零完成,轴脉冲:%5ld",axisMsg(ch,"",FALSE),GetPulse(ch,TRUE));
			OSQPost(MsgQueue,buf);
			OSTimeDly(200);
			//GetKey(FALSE);
			SetMM(ch,0,TRUE);
			
		}
		else
		{
			OSQPost(MsgQueue,g_bEnglish?"FindZero Error":"寻找零点错误!");
			OSTimeDly(500);
			//GetKey(FALSE);		
		}
	}
	
	//GetKey(FALSE);		
	//OSQPost(MsgQueue," ");
	
	switch(ch)
	{
		case 1:g_bFirstResetX=FALSE;break;
		case 2:g_bFirstResetY=FALSE;break;
		case 3:g_bFirstResetZ=FALSE;break;
		case 4:g_bFirstResetU=FALSE;break;
		case 5:g_bFirstResetV=FALSE;break;
		case 6:g_bFirstResetW=FALSE;break;
		default:break;
	}
}


INT16S	CTestFrm_DoModal()
{
	INT16U key=0,tkey=0;
//	INT8S  js=0;
//	INT16S i=0;


	g_iTestAxis = 0;
	
	CTestFrm_Frame_Create();									//测试界面框架
	CTestIoFrm_Create();							    		//IO诊断界面	
	
	while(TRUE)
	{	
		
		{	
						
			CTestFrm_UpdateInputStatuses();						//更新外部输入点显示状态
			CTestFrm_UpdateProbeStatuses();						//更新探针显示状态
			CTestFrm_UpdateTestKnob();							//更新测试旋钮显示值
			
			if(g_iTestAxis != 0 && g_handAxis)
			{		
				CTestFrm_HandMove(g_iTestAxis);
			}
			else
			{
				CTestFrm_UpdateHandWheelPos();
			}
					
		}
		
		
		key=KeyMBoxGetKey();
				
		if(key!=0xffff)
		{			
			CTestFrm_OnKey(key,tkey);
			
			if(Ok_Exit)
			{
				break;
			}
		}
		
		OSTimeDly(3);
	}
	
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}



void	CTestFrm_HandMove(INT16S ch)
{
	
	if(ch<g_Ini.iAxisCount+1 && ch!=0)
	{
	//	INT32S pos=0;
		INT32S pos1=0;
		
		//pos = GetPulse(7,FALSE);
		pos1= GetPulse(8,FALSE);
		
		if(labs(pos1)>1 )
		{
			g_fSpeedBi = 1 ;
			
			switch(ch)
			{
				case 1:case 3:case 4:case 5:case 6:

					AddMoveMM1(ch,pos1>0?100000:-100000,0,g_Ini.lHandSpeed[ch-1],g_Ini.lHandSpeed[ch-1]*25);
					
					while (1)
					{	
						if(PulseLagCheck(ch))
						{
							Stop1(ch);
							g_iTestAxis = 0;
							OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
							break;	
						}
						
						CTestFrm_HandWheel(ch);
						CTestFrm_UpdateAxisPos(ch);					
						
						if(IsFree(ch)) 
							break;
						
						OSTimeDly(13);
					}
					
					

					break;

				case 2:
					if(g_Ini.iYDaoXian)
					{
						AddMoveMM1(ch,pos1>0?100000:-100000,0,g_Ini.lHandSpeed[ch-1],g_Ini.lHandSpeed[ch-1]*25);

						while (1)
						{
							if(PulseLagCheck(ch))
							{
								Stop1(ch);
								g_iTestAxis = 0;
								OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
								break;	
							}
							
							CTestFrm_HandWheel(ch);
							CTestFrm_UpdateAxisPos(ch);		
	
							if (IsFree(ch)) 
								break;
							
							OSTimeDly(13);
						}

					}
					else
					{
						if(pos1>0)
						{
							AddMoveMM1(ch,pos1>0?100000:-100000,0,g_Ini.lHandSpeed[ch-1],g_Ini.lHandSpeed[ch-1]*25);

							while (1)
							{
								if(PulseLagCheck(ch))
								{
									Stop1(ch);
									g_iTestAxis = 0;
									OSQPost(MsgQueue,g_bEnglish?"Current Axis Lag Warning!!!":"当前轴脉冲滞后报警!!!");
									break;	
								}
								
								CTestFrm_HandWheel(ch);
								CTestFrm_UpdateAxisPos(ch);	
						
								if (IsFree(ch)) 
									break;
								
								OSTimeDly(1);
							}
						}
						else
						{									
							//SetMM(7,0,FALSE);
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



void	CTestFrm_UpdateInputStatuses()
{
	static INT8S js=0;

	switch(js)
	{
		case 0:
			CTestFrm_DrawInputStatus(js,ReadBit(41));		//X伺服报警
			break;
			
		case 1:
			CTestFrm_DrawInputStatus(js,ReadBit(68));		//X伺服零点
			break;
		
		case 2:
			CTestFrm_DrawInputStatus(js,ReadBit(2));		//X伺服原点
			break;
		
		case 3:
			CTestFrm_DrawInputStatus(js,ReadBit(3));		//3
			break;
		
		case 4:
			CTestFrm_DrawInputStatus(js,ReadBit(4));		//4
			break;	
			
		case 5:
			CTestFrm_DrawInputStatus(js,ReadBit(36));		//Y伺服报警
			break;
			
		case 6:
			CTestFrm_DrawInputStatus(js,ReadBit(69));		//Y伺服零点
			break;
			
		case 7:
			CTestFrm_DrawInputStatus(js,ReadBit(7));		//Y伺服原点
			break;
			
		case 8:
			CTestFrm_DrawInputStatus(js,ReadBit(8));		//8
			break;
		
		case 9:
			CTestFrm_DrawInputStatus(js,ReadBit(9));		//9
			break;	
		
		case 10:
			CTestFrm_DrawInputStatus(js,ReadBit(15));		//Z伺服报警
			break;
			
		case 11:
			CTestFrm_DrawInputStatus(js,ReadBit(70));		//Z伺服零点
			break;
		
		case 12:
			CTestFrm_DrawInputStatus(js,ReadBit(12));		//Z伺服原点
			break;
			
		case 13:
			CTestFrm_DrawInputStatus(js,ReadBit(13));		//13  退线
			break;
		
		case 14:
			CTestFrm_DrawInputStatus(js,ReadBit(14));		//14  送线
			break;	
			
			
		case 15:
			CTestFrm_DrawInputStatus(js,ReadBit(10));		//A伺服报警
			break;
			
		case 16:
			CTestFrm_DrawInputStatus(js,ReadBit(71));		//A伺服零点
			break;
			
		case 17:
			CTestFrm_DrawInputStatus(js,ReadBit(17));		//A伺服原点
			break;
			
		case 18:
			CTestFrm_DrawInputStatus(js,ReadBit(18));		//18  跑线
			break;
		
		case 19:
			CTestFrm_DrawInputStatus(js,ReadBit(19));		//19
			break;	
				
			
		case 20:
			CTestFrm_DrawInputStatus(js,ReadBit(5));		//B伺服报警
			break;
			
		case 21:
			CTestFrm_DrawInputStatus(js,ReadBit(72));		//B伺服零点
			break;
		
		case 22:
			CTestFrm_DrawInputStatus(js,ReadBit(39));		//B伺服原点
			break;
			
		case 23:
			CTestFrm_DrawInputStatus(js,ReadBit(0));		//C伺服报警
			break;
			
		case 24:
			CTestFrm_DrawInputStatus(js,ReadBit(73));		//C伺服零点
			break;
			
		case 25:
			CTestFrm_DrawInputStatus(js,ReadBit(42));		//C伺服原点
			break;
		
		case 26:
			CTestFrm_DrawInputStatus(js,ReadBit(24));		//X正限位
			break;
			
		case 27:
			CTestFrm_DrawInputStatus(js,ReadBit(25));		//X负限位
			break;
		
		case 28:
			CTestFrm_DrawInputStatus(js,ReadBit(26));		//Y正限位
			break;
			
		case 29:
			CTestFrm_DrawInputStatus(js,ReadBit(27));		//Y负限位
			break;
				
		case 30:
			CTestFrm_DrawInputStatus(js,ReadBit(28));		//Z正限位
			break;
			
		case 31:
			CTestFrm_DrawInputStatus(js,ReadBit(29));		//Z负限位
			break;
		
		case 32:
			CTestFrm_DrawInputStatus(js,ReadBit(30));		//A正限位
			break;
			
		case 33:
			CTestFrm_DrawInputStatus(js,ReadBit(31));		//A负限位
			break;
			
		case 34:
			CTestFrm_DrawInputStatus(js,ReadBit(32));		//B正限位
			break;
			
		case 35:
			CTestFrm_DrawInputStatus(js,ReadBit(35));		//B负限位
			break;
		
		case 36:
			CTestFrm_DrawInputStatus(js,ReadBit(37));		//C正限位
			break;
			
		case 37:
			CTestFrm_DrawInputStatus(js,ReadBit(38));		//C负限位
			break;
		
		
		case 38:
			CTestFrm_DrawInputStatus(js,ReadBit(20));		//断线
			break;
			
		case 39:
			CTestFrm_DrawInputStatus(js,ReadBit(21));		//缠线
			break;
				
		case 40:
			CTestFrm_DrawInputStatus(js,ReadBit(62));	//启动按钮
			break;
			
		case 41:
			CTestFrm_DrawInputStatus(js,ReadBit(65));	//停止按钮
			break;
			
		case 42:
			CTestFrm_DrawInputStatus(js,ReadBit(66));		//手轮A
			break;
			
		case 43:
			CTestFrm_DrawInputStatus(js,ReadBit(67));		//手轮B
			break;
			
		case 44:
			CTestFrm_DrawInputStatus(js,ReadBit(33));		//手盒A 
			break;
			
		case 45:
			CTestFrm_DrawInputStatus(js,ReadBit(34));		//手盒B
			break;
			
			
			
			
		case 46:
			CTestFrm_DrawInputStatus(js,ReadBit(56));	//低速
			break;
			
		case 47:
			CTestFrm_DrawInputStatus(js,ReadBit(58));	//中速
			break;
		
		case 48:
			CTestFrm_DrawInputStatus(js,ReadBit(60));	//高速
			break;
			
		case 49:
			CTestFrm_DrawInputStatus(js,ReadBit(23));	//急停2
			break;	
			
		case 50:
			CTestFrm_DrawInputStatus(js,ReadBit(57));	//X
			break;
			
		case 51:
			CTestFrm_DrawInputStatus(js,ReadBit(59));	//Y
			break;
			
		case 52:
			CTestFrm_DrawInputStatus(js,ReadBit(61));	//Z
			break;
			
		case 53:
			CTestFrm_DrawInputStatus(js,ReadBit(63));	//A
			break;
			
		case 54:
			CTestFrm_DrawInputStatus(js,ReadBit(64));	//B
			break;
			
		case 55:
			CTestFrm_DrawInputStatus(js,ReadBit(22));	//C
			break;
				
			
		
			
			
		default:
			CTestFrm_DrawInputStatus(js,ReadBit(js));
			break;
		
	}
	
	js++;
	
	//Uart_Printf("js = %d\n",js++);		
		
	if(js>=MAXIN)
		js=0;
}


void	CTestFrm_UpdateProbeStatuses()
{
	static INT16S i=0;
	
	{
		INT16S 	x,y;
		char 	str[10]="探针1";
		char    strE[10]="Prb1";
		char	status[8]={0,0,0,0,0,0,0,0};
		{
			
			{
				//if (i==2)
					//x=25 +(i+1+37)/8*85;
				//else
					x=25 +5*85;
					
				y=80+(i+1)*37;
				
			}
			
			
			OpenProbe(0,i+1,2);
			
			if(g_bEnglish)
				strE[3]='1'+i;
			else
				str[4]='1'+i;
				
			status[i]=(ReadProbe(0,i+1));
			
			if(g_ProStatus[i]!=status[i])
			{
				{
					CRect rc ; 
					
					rc.left = x;
					rc.top = y;
					rc.right = x+77;
					rc.bottom = y+30;
					
					CDC_PutColorBar(x,y,80,30,status[i]==1?REDCOLOR:BLUECOLOR,1);
					CDC_Rectangle4(x,y,x+80,y+30,YELLOWCOLOR);
					CDC_OutCenTextKzk24(rc,g_bEnglish?strE:str,WHITECOLOR,status[i]==1?REDCOLOR:BLUECOLOR);
					
				}
				
				g_ProStatus[i]=status[i];
			}
			
			CloseProbe(0);
		}
	}
	
	i++;
	
	if(i>=(g_Ini.iAxisCount<3?4:8))
		i=0;
}


void	CTestFrm_UpdateTestKnob()								//更新测试旋钮
{	
	INT16S temp=0;
	char   buf[10];
	static INT16S oldad=-1;
					
	temp=CTelCom_GetChar(1);
	
	if(temp!=2000)	
		temp=temp/2;
		
	if(temp!=oldad && temp!=2000)
	{
		oldad=temp;		
		sprintf(buf,"%-4d",temp);
		
		CDC_OutText_UTF(g_bEnglish?150:140,490,buf,g_TextColor11,g_TextColor3,MZK_24);
	}
}

void	CTestFrm_HandWheel(INT16S ch)	
{
	static BOOL	predir=TRUE;
	static BOOL	curdir=TRUE;
	static BOOL	IsRun=FALSE;
	static INT16S	Count=0; 	
	static INT16U   preCh=0;
	//INT32S pos=0;
	INT32S pos1=0;
	
	if(ch!=preCh)
	{
		preCh=ch;
		predir=TRUE;
		curdir=TRUE;
		IsRun=FALSE;
		Count=0;
	}
		
	if(ch)
	{
		Count++;

		if(Count>g_Ini.iHandRunSpeed)									//指示手摇加工滞后之时间
		{
			Count=0;				
			//pos=GetPulse(7,FALSE);
			pos1=GetPulse(8,FALSE);		
							
			//SetMM(7,0,FALSE);
			SetMM(8,0,FALSE);
			
			if(pos1>0)
				curdir=TRUE;
			else
				curdir=FALSE;
			
			if(abs(pos1)<3)									//add by xiaomeidi,20080418避免干扰采用滤波
				IsRun=TRUE;
			
			if(abs(pos1)>2)									//if(pos!=0)modify by xiaomeidi,20080418避免干扰采用滤波
			{
				IsRun=TRUE;
				
				if(predir!=curdir)
				{
					predir=curdir;	
					Stop1(ch);							
				}
			}
			else
			{					
				 if(IsRun)
				 {
					Stop1(ch);					
					IsRun=FALSE;
			     } 			   
			}
		}
	}
}


void CTestFrm_UpdateAxisPos(INT16S ch)			//更新显示单轴手动逻辑位置、实际位置、滞后脉冲
{
	char   buf[10];
	INT32S p = 0;
	//INT32S i=0;
	
	if(ch>CHMAX || ch<1)
		return;

	p = GetCommandPos(ch)*g_Ini.iNumerator[ch-1];										
	
	sprintf(buf,"%-9d",p);
	CDC_OutText_UTF(566,346+(36*(ch-1)),buf,g_TextColor11,g_TextColor3,MZK_24);	

	p = GetActualPos(ch)*g_Ini.iDenominator[ch-1];
	sprintf(buf,"%-9d",p);				
	CDC_OutText_UTF(681,346+(36*(ch-1)),buf,g_TextColor11,g_TextColor3,MZK_24);
	
	{
		p=labs(GetCommandPos(g_iTestAxis)*g_Ini.iNumerator[g_iTestAxis-1]-GetActualPos(g_iTestAxis)*g_Ini.iDenominator[g_iTestAxis-1]);
		sprintf(buf,"%-10d",p);				
		CDC_OutText_UTF(g_bEnglish?400:380,520,buf,g_TextColor11,g_TextColor3,MZK_24);
	}
	
}


void CTestFrm_UpdateHandWheelPos()						//更新显示手摇脉冲、手盒脉冲
{
	static INT32S  oldpos=-1;
	static INT32S  oldpos1=-1;
	char buf[10];
	
	INT32S p=GetPulse(8,FALSE);
	INT32S p1=GetPulse(7,FALSE);
					
	if(g_iTestAxis==0)
	{
		if(p!=oldpos)
		{
			oldpos=p;
			sprintf(buf,"%-10d",p);
			
			CDC_OutText_UTF(g_bEnglish?400:380,490,buf,g_TextColor11,g_TextColor3,MZK_24);
		}
		
		if(p1!=oldpos1)
		{
			oldpos1=p1;
			sprintf(buf,"%-10d",p1);
			
			CDC_OutText_UTF(g_bEnglish?150:140,520,buf,g_TextColor11,g_TextColor3,MZK_24);
		}
	}

}


void CTestFrm_ClearComActPos(INT16S ch)						//清零逻辑位置、实际位置
{
	//INT8S i=0;
	
	if(ch>CHMAX || ch<1)
		return;		
	
	SetCommandPos(ch,0);
	SetActualPos(ch,0);
	
	CDC_PutColorBar(562,341+36*(ch-1),105,34,g_TextColor3,1);
	CDC_PutColorBar(680,341+36*(ch-1),105,34,g_TextColor3,1);					
	
	CDC_PutColorBar(g_bEnglish?400:380,520,140,30,g_TextColor3,1);

	CTestFrm_UpdateAxisPos(ch);

}

BOOL	CTestFrm_BackZero(INT16S ch,INT16S dir)				//替换掉TestBackZero，方便坐标同时显示
{
	INT16S 	workstep = 0;
	int 	status=0;
	INT16S 	logic=0;
	INT16S 	value=-1;
	long 	DestPulse = 0, DestPulse1 = 0;

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
				set_stop0_mode(ch,1,logic?0:1);
				pmove(ch,dir?-DestPulse:DestPulse);
				workstep ++;	
			}
			else
			{
				workstep =2;
			}			
			
			break;
			
		case 1:	
			get_status(ch,&status);
			
			CTestFrm_UpdateAxisPos(ch);
		
			if( status )
			{
				break;
			}
			
			workstep ++;
			break;
			
		case 2:			
			set_stop0_mode(ch,1,logic);	
			pmove(ch,dir?-DestPulse:DestPulse);
				
			workstep ++;
			break;
			
		case 3:			
			get_status(ch,&status);
			
			CTestFrm_UpdateAxisPos(ch);
		
			if( status )
			{
				break;
			}
			
			if( CheckHome(ch) )
			{
				value = 0;
			}
			else
				value = 1;
				
		
			if(value != 0)
			{
				set_stop0_mode(ch,0,logic);
				
				return FALSE;
			}
			workstep ++;

			break;
			
	   case 4:	

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
		
		if(RealIO(STOPIO,0,3)||RealIO(EXT_SINGLEDI,0,2)||(g_bHandset?RealIO(STOPIO2,0,3):0)||(g_bHandset?RealIO(SINGLEIO,0,3):0))
		{
			Stop1(ch);
			break;
		}
	   
	}

	set_stop0_mode(ch,0,logic);
	
	return FALSE;
	   		
}




