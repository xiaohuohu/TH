# include	"includes.h"

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


WORK_EXT	BOOL		g_bHandset;
WORK_EXT	BOOL		g_bCylinder; 

#define	   	MAXTESTKEY	51

CRect		g_ptrKeyRect[MAXTESTKEY];
BOOL		g_bKeyStatus[MAXTESTKEY];

INT16S  	CR = 0;
BOOL    	LED =0;



extern INT32S g_nPreBk ;

//#define PRINTBMP
/***************************************
	按键检测

***************************************/

void	CTestKey_Init()
{
	INT16S i=0;
	
	g_nPreBk = g_nBk;
	g_nBk=DARKGRAYCOLOR;														//该模块临时改变背景颜色
	
	for( i=0; i<MAXTESTKEY; i++)
		g_bKeyStatus[i]=FALSE;	
		
	AllLedOff();
}


void	CTestKey_Destroy()
{
	g_nBk=g_nPreBk;
	CMsg_SetWH(60,566,600,28,g_TextColor11,g_nBk,g_nBk);

	InitLed();
}


void	CTestKey_Create()
{
	INT16S k=0,j,i;
	INT16S x=0,y=0,w=0,w1=0,h=0;
	//Uart_Printf("%d\n",getmaxx());											//打印值为799								
	CDC_DrawWin24(0,0,getmaxx(),getmaxy(),g_bEnglish?"Spring Key Check":"弹簧机按键检修测试");
	
	
//	CDC_SetRectangle(10,56,780,534-69,g_bEnglish?"TestKey":"按键测试",g_nBk,1);
	CDC_SetRectangle3(10,56,780,534-69,g_bEnglish?"TestKey":"按键测试",g_bEnglish?100:100,g_nBk,1);

//	CDC_OutText24(10,570,g_bEnglish?"Msg:":"讯息:",WHITECOLOR,g_nBk);		
//	CMsg_SetWH(70,570,630,25,g_TextColor2,g_nBk,g_nBk);							//绘制消息提示框范围	
	CDC_OutText_UTF(6,getmaxy()-31,g_bEnglish?"Msg:":"讯息:",g_TextColor1,DARKGRAYCOLOR,KZK_24);
	CMsg_SetWH(60,566,600,28,g_TextColor11,DARKGRAYCOLOR,DARKGRAYCOLOR);

	OSQPost(MsgQueue,g_bEnglish?"Keep press key <<Cancel>> to exit!":"长按<<取消>>键退出检修界面!");
	
	for( i=0;i<MAXTESTKEY;i++)
		g_bKeyStatus[i]=FALSE;
			
	{
		w=85;
		w1=100;
		h=55;
		y = 90;
		k=0;
		
		for(j=0; j<9; j++,k++)
		{
			x = 20 + j * w;
			
			g_ptrKeyRect[k].left=x;
			g_ptrKeyRect[k].top=y;
			g_ptrKeyRect[k].right=x+w;
			g_ptrKeyRect[k].bottom=y+h;
			
			CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
		}
		
		y=y+h+25;
		
		for(i=0;i<6;i++)
		{
			for(j=0; j<7; j++,k++)
			{
				x = 20 + j * w1;
				if(j>3)
					x+=65;
				g_ptrKeyRect[k].left=x;
				g_ptrKeyRect[k].top=y;
				g_ptrKeyRect[k].right=x+w1;
				g_ptrKeyRect[k].bottom=y+h;
				CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
			}
			y+=h;
		}	
		
	}
	
}


void	CTestKey_UpdateKeyStatus(INT16S i,BOOLEAN status,BOOLEAN ledout)		//键盘状态显示函数
{
	char str[10]="\0";
	char str1[10]="\0";
	BOOL bEnglish=g_bEnglish;	
	CRect rect;																	//用于显示双层字符按键
	switch(i)
	{
		case 0:		strcpy(str,g_bEnglish?"Reset":"归零");		break;			//第一行
		case 1:		strcpy(str,g_bEnglish?"Y-swt":"Y停");		break;
		case 2:		strcpy(str,g_bEnglish?"Z-swt":"Z停");		break;
		case 3:		strcpy(str,g_bEnglish?"A-swt":"A停");		break;
		case 4:		strcpy(str,g_bEnglish?"F1":"F1");		break;
		case 5:		strcpy(str,g_bEnglish?"Cyld":"汽缸");		break;
		case 6:		strcpy(str,g_bEnglish?"Start":"启动");		break;
		case 7:		strcpy(str,g_bEnglish?"Stop":"停机");		break;
		case 8:		strcpy(str,g_bEnglish?"Test":"测试");		break;
		
		case 9: 	strcpy(str,g_bEnglish?"File":"料号");		break;			//第二行
		case 10:	strcpy(str,g_bEnglish?"List":"目录");		break;
		case 11:	strcpy(str,g_bEnglish?"PrePg":"上页");	break;
		case 12:	strcpy(str,g_bEnglish?"Save":"保存");		break;
		case 13:	strcpy(str,"7");							break;
		case 14:	strcpy(str,"8");							break;
		case 15:	strcpy(str,"9");							break;
		
		case 16:	strcpy(str,g_bEnglish?"Ratio":"倍率");		break;			//第三行
		case 17:	strcpy(str,g_bEnglish?"Help":"帮助");		break;
		case 18:	strcpy(str,g_bEnglish?"NextPg":"下页");	break;
		case 19:	strcpy(str,g_bEnglish?"Cancel":"取消");		break;
		case 20:	strcpy(str,"4");							break;
		case 21:	strcpy(str,"5");							break;
		case 22:	strcpy(str,"6");							break;
		
		case 23:	strcpy(str,g_bEnglish?"Insert":"插入");		break;			//第四行
		case 24:	strcpy(str,g_bEnglish?"Del":"删除");		break;
		case 25:	strcpy(str,g_bEnglish?"DelAll":"全删");		break;
		case 26:	strcpy(str,g_bEnglish?"Commd":"指令");	break;
		case 27:	strcpy(str,"1");							break;
		case 28:	strcpy(str,"2");							break;
		case 29:	strcpy(str,"3");							break;		
		
		case 30:																//第五行
			{
				strcpy(str,g_bEnglish?"Probe":"探针");
				strcpy(str1,g_bEnglish?"Set":"设定");	
			}break;	
	
		case 31:	
			{
				strcpy(str,g_bEnglish?"Count":"产量");
				strcpy(str1,g_bEnglish?"Set":"设定");	
			}break;	
	
		case 32:	
			{
				strcpy(str,g_bEnglish?"Work":"工作");
				strcpy(str1,g_bEnglish?"Para":"参数");	
			}break;	
	
		case 33:	
			{
				strcpy(str,g_bEnglish?"Box":"手盒");	
				strcpy(str1,g_bEnglish?"Switch":"开关");
			}break;		
	
		case 34:	strcpy(str,".");	break;
		case 35:	strcpy(str,"UP");	break;
		case 36:	strcpy(str,"0");	break;	
	
	
		case 37:																//第六行																	
			{
				strcpy(str,g_bEnglish?"Probe":"探针");
				strcpy(str1,g_bEnglish?"Clear":"清零");	
			}
		break;	
	
		case 38:	
			{
				strcpy(str,g_bEnglish?"Count":"产量");
				strcpy(str1,g_bEnglish?"Clear":"清零");	
			}break;	
																					
		case 39:	
		{
			strcpy(str,g_bEnglish?"Systm":"系统");	
			strcpy(str1,g_bEnglish?"Mange":"管理");
		}break;
		
		case 40:	
		{
			strcpy(str,g_bEnglish?"Hand":"手轮");	
			strcpy(str1,g_bEnglish?"Switch":"开关");
		}break;			
		
		case 41:	strcpy(str,"LEFT");							break;
		case 42:	strcpy(str,"ENTER");						break;
		case 43:	strcpy(str,"RIGHT");						break;			
		
		
		case 44:	strcpy(str,g_bEnglish?"Axis X":"X轴");		break;			//第七行	
		case 45:	strcpy(str,g_bEnglish?"Axis Y":"Y轴");		break;
		case 46:	strcpy(str,g_bEnglish?"Axis Z":"Z轴");		break;		
		case 47:	strcpy(str,g_bEnglish?"Axis A":"A轴");		break;
		case 48:	strcpy(str,"+/-");							break;
		case 49:	strcpy(str,"DOWN");							break;
		case 50:	strcpy(str,g_bEnglish?"Clear":"清除");		break;

	}
	
	if((i<6)|| 8==i || 23==i || 44==i|| 45==i || 46==i || 47==i || 48==i|| 50==i)//单行，深蓝背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:DARKBLUECOLOR,1);
		CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:DARKBLUECOLOR);
	}
	else if(42==i)
	{
		g_bEnglish=TRUE;
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:DARKBLUECOLOR,1);
		CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:DARKBLUECOLOR);
		g_bEnglish=bEnglish;
		
	}
	else if(30==i || 31==i || 37==i || 38==i || 39==i)							//双行，深蓝背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:DARKBLUECOLOR,1);
		
		rect.left=g_ptrKeyRect[i].left;
		rect.right=g_ptrKeyRect[i].right;
		rect.top=g_ptrKeyRect[i].top;
		rect.bottom=g_ptrKeyRect[i].bottom-24;
		CDC_OutCenText24(rect,str,WHITECOLOR,status?MAGENTACOLOR:DARKBLUECOLOR);
		
		rect.left=g_ptrKeyRect[i].left;
		rect.right=g_ptrKeyRect[i].right;
		rect.top=g_ptrKeyRect[i].top+24;
		rect.bottom=g_ptrKeyRect[i].bottom;
		CDC_OutCenText24(rect,str1,WHITECOLOR,status?MAGENTACOLOR:DARKBLUECOLOR);
	}
	else if(32 == i || 33==i|| 40==i )											 //双行，黄色背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:YELLOWCOLOR,1);
		
		rect.left=g_ptrKeyRect[i].left;
		rect.right=g_ptrKeyRect[i].right;
		rect.top=g_ptrKeyRect[i].top;
		rect.bottom=g_ptrKeyRect[i].bottom-24;
		CDC_OutCenText24(rect,str,BLACKCOLOR,status?MAGENTACOLOR:YELLOWCOLOR);
		
		rect.left=g_ptrKeyRect[i].left;
		rect.right=g_ptrKeyRect[i].right;
		rect.top=g_ptrKeyRect[i].top+24;
		rect.bottom=g_ptrKeyRect[i].bottom;
		CDC_OutCenText24(rect,str1,BLACKCOLOR,status?MAGENTACOLOR:YELLOWCOLOR);		
	}
	else
	{
		if(10==i || 11==i || 12 == i || 18 == i || 19 == i|| 26 == i)			//单行，蓝色背景
		{
			CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLUECOLOR,1);
			CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:BLUECOLOR);
		}
		else
		{
			if(9==i || 16==i || 17 == i || 24 == i || 25 == i)					//单行，黄色背景
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:YELLOWCOLOR,1);
				CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:YELLOWCOLOR);
			}
			else if(6==i)														//单行，绿色背景
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:GREENCOLOR,1);
				CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:GREENCOLOR);
			}
			else if(7==i)														//单行，黄色背景
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:REDCOLOR,1);
				CDC_OutCenText24(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:REDCOLOR);
			}
			else if(35==i || 41==i || 43 == i || 49 == i)
			{
				g_bEnglish=TRUE;
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutCenText24(g_ptrKeyRect[i],str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR);
				g_bEnglish=bEnglish;			
			}
			else
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutCenText24(g_ptrKeyRect[i],str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR);
			}
		}
	}
	
	CDC_Rectangle2(g_ptrKeyRect[i],BLACKCOLOR);
	
	if(i<9||33==i||40==i||44==i||45==i||46==i||47==i)
	{
		if(status)
			CDC_Fillellipse(g_ptrKeyRect[i].left+(g_ptrKeyRect[i].right-g_ptrKeyRect[i].left)/2,g_ptrKeyRect[i].top+5,3,3,REDCOLOR);
		else
			CDC_Fillellipse(g_ptrKeyRect[i].left+(g_ptrKeyRect[i].right-g_ptrKeyRect[i].left)/2,g_ptrKeyRect[i].top+5,3,3,WHITECOLOR);
	}
	
	if(ledout)
	{
		if(i<9)
			LedOut(i,status?1:0);
		else
		{
			if(44==i||45==i||46==i||47==i)
				LedOut(i-28,status?1:0);
			else if(33==i)	
				LedOut(BOXLED,status?1:0);
			else if(40==i)	
				LedOut(HANDLED,status?1:0);
		}
	}		
}


void	CTestKey_OnKey(INT16U key,INT16U tkey)									//通过按钮也能处理输入输出
{
	INT16S i=-1;
		
	switch(key)
	{
		case TF1:
			{
				char filename[0x20];
				INT16S j=0;		
				i=4;
				
				#ifdef	PRINTBMP				
//				CBmp_Init();
				strcpy(filename,"testkey");
				
				for(j=0; j<strlen(filename); j++)
				{
					if(filename[j] == '.')
					{
						filename[j]='\0';
						break;
					}
				}
				
				strcat(filename,".bmp");
				CreatBMP(filename,800,600,1);
//				CBmp_Destroy();
				OSQPost(MsgQueue,"Write Bmp End!");
				#endif
			}
			break;
			
		case TBACKZERO:	i=0;break;
		case TYSWITCH :	i=1;break;
		case TZSWITCH :	i=2;break;
		case TUSWITCH :	i=3;break;
		case TCYLINDER:	i=5;break;
		case TRUN :		i=6;break;
		case TSTOP :	i=7;break;
		case TTEST:		i=8;break;
		case TREAD :	i=9;break;
		case TFILE :    i=10;break;
		case TPAGEUP :	i=11;break;
		case TSAVE :	i=12;break;
		case '7' :		i=13;break;
		case '8' :		i=14;break;
		case '9' :		i=15;break;

		case TBEILV:	i=16;break;
		case THELP:		i=17;break;
		case TPAGEDOWN :i=18;break;
		
		case TCANCEL:	
		    if(PressKeyTime(50))			
        		Ok_Exit=TRUE;
        	else
				i=19;
			break;
		
		case '4' :		i=20;break;
		case '5' :		i=21;break;
		case '6' :		i=22;break;

		case TINSERT :	i=23;break;
		case TDEL	 :  i=24;break;
		case TDELALL :	i=25;break;
		case TCMD 	 :	i=26;break;
		case '1' :		i=27;break;
		case '2' :		i=28;break;
		case '3' :		i=29;break;
		case TPROBESET:	i=30;break;
		case TCOUNTSET:	i=31;break;
		case TWORKPARA:	i=32;break;
		case THANDBOX:	i=33;break;
		case '.' :		i=34;break;	
		case TUP :		i=35;break;
		case TZERO:		i=36;break;
		case TPROBECLR:	i=37;break;
		case TCOUNTCLR: i=38;break;
		case TSYSMANGE:	i=39;break;
		case THANDSWITCH:i=40;break;
		case TLEFT:		i=41;break;
		case TENTER:	i=42;break;
		case TRIGHT :	i=43;break;
		case TX :		i=44;break;	
		case TY :		i=45;break;	
		case TZ :		i=46;break;	
		case TU :		i=47;break;
		case TMINUS :	i=48;break;
		case TDOWN :	i=49;break;
		case TCLEAR :	i=50;break;
		
		default:
			return;
	}
	if(i!=-1)
	{
		g_bKeyStatus[i]=g_bKeyStatus[i]?FALSE:TRUE;
		CTestKey_UpdateKeyStatus(i,g_bKeyStatus[i],TRUE);
	}
}


INT16S	CTestKey_DoModal()
{
	INT16U key,tkey=0;
	
	CTestKey_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			CTestKey_OnKey(key,tkey);
		
			if(Ok_Exit)
				break;
		}

		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}