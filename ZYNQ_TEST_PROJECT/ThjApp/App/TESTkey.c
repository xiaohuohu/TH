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
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;

WORK_EXT	BOOL		g_bHandset;
WORK_EXT	BOOL		g_bCylinder; 
/*
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
*/

#define	   	MAXTESTKEY	67

CRect		g_ptrKeyRect[MAXTESTKEY];
BOOL		g_bKeyStatus[MAXTESTKEY];
BOOL		g_bTestKey=FALSE;
INT16S  	CR = 0;
BOOL    	LED =0;

/***************************************
	按键检测

***************************************/

void	CTestKey_Init()
{
	INT16S i=0;
	
	g_bTestKey=TRUE;
	
	for( i=0; i<MAXTESTKEY; i++)
		g_bKeyStatus[i]=FALSE;	
		
	AllLedOff();
	LedOut(STEPLED,0);			//关闭"手盒"指示灯,退出测试界面时再根据g_bHandset更新灯的状态。
	LedOut(HANDLED,0);			//关闭"手轮"指示灯,退出测试界面时再根据g_Ini.iHandStatus更新灯的状态。

}


void	CTestKey_Destroy()
{
	
	CMsg_SetWH(60,566,600,28,g_TextColor11,g_nBk,g_nBk);
	
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


void	CTestKey_Create()
{
	INT16S k=0,j,i;
	INT16S x=0,y=0,w=0,h=0;
	
	
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
		CDC_OutText24(275,4,"Spring Key Check",g_TextColor11,g_nTableBk);
	else
		CDC_OutText_UTF(280,4,"弹簧机按键检修测试",g_TextColor11,g_nTableBk,KZK_24);

	
	CDC_PutColorBar(3,36,794,530,g_TextColor3,1);
	CDC_SetRectangle3(10,56,780,534-69,g_bEnglish?"TestKey":"按键测试",g_bEnglish?102:100,g_TextColor3,1);
	OSQPost(MsgQueue,g_bEnglish?"Press Key \"Test\" to Enter Key-Test State.":"按\"测试\"进入按键测试状态.");
	

	g_bTestKey = FALSE;
	
	for( i=0;i<MAXTESTKEY;i++)
		g_bKeyStatus[i]=FALSE;
			
	{
		w=85;
		h=48;
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
		
		y=y+h+15;
		
		for(i=0;i<5;i++)
		{
			for(j=0; j<8; j++,k++)
			{
				x = 20 + j * w;
				if(j>3)
					x+=w;
				g_ptrKeyRect[k].left=x;
				g_ptrKeyRect[k].top=y;
				g_ptrKeyRect[k].right=x+w;
				g_ptrKeyRect[k].bottom=y+h;
				CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
			}
			y+=h;
		}	
			
		y+=20;
		
		for(j=0; j<9; j++,k++)
		{
			x = 20 + j * w;
			g_ptrKeyRect[k].left=x;
			g_ptrKeyRect[k].top=y;
			g_ptrKeyRect[k].right=x+w;
			g_ptrKeyRect[k].bottom=y+h;
			CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
		}
		
		y+=h;
		
		for(j=0; j<9; j++,k++)
		{
			x = 20 + j * w;
			g_ptrKeyRect[k].left=x;
			g_ptrKeyRect[k].top=y;
			g_ptrKeyRect[k].right=x+w;
			g_ptrKeyRect[k].bottom=y+h;
			CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
		}
	}
	
}


void	CTestKey_UpdateKeyStatus(INT16S i,BOOLEAN status,BOOLEAN ledout)		//键盘状态显示函数
{
	char str[10]="\0";
	char str1[10]="\0";
		
	switch(i)
	{
		case 0:		strcpy(str,g_bEnglish?"Reset":"归零");		break;
		case 1:		strcpy(str,g_bEnglish?"Y-swt":"停线");		break;
		case 2:		strcpy(str,g_bEnglish?"Z-swt":axisMsg(3,"",FALSE));		break;
		case 3:		strcpy(str,g_bEnglish?"A-swt":axisMsg(4,"",FALSE));		break;
		case 4:		strcpy(str,g_bEnglish?"B-swt":axisMsg(5,"",FALSE));		break;
		case 5:		strcpy(str,g_bEnglish?"C-swt":axisMsg(6,"",FALSE));		break;
		case 6:		strcpy(str,g_bEnglish?"Cyldr":"汽缸");		break;
		case 7:		strcpy(str,g_bEnglish?"Stop":"单条");		break;
		case 8:		strcpy(str,g_bEnglish?"Test":"测试");		break;
		case 9: 	strcpy(str,g_bEnglish?"File":"料号");		break;
		
		case 10:	
			{
				strcpy(str,g_bEnglish?"S":"S");	
				//strcpy(str1,"设定");						
			}break;
		
		case 11:	
			{
				strcpy(str,g_bEnglish?"G":"G");
				//strcpy(str1,"模式");						
			}
			break;
		case 12:	strcpy(str,g_bEnglish?"M":"M");				break;
		case 13:	strcpy(str,g_bEnglish?"Save":"存储");		break;
		case 14:	strcpy(str,"7");							break;
		case 15:	strcpy(str,"8");							break;
		case 16:	strcpy(str,"9");							break;
		
		case 17:	
			{
				strcpy(str,g_bEnglish?"Probe":"探针");
				strcpy(str1,g_bEnglish?"Clear":"清零");	
			}
		break;
		case 18:	
			{
				strcpy(str,g_bEnglish?"Probe":"探针");
				strcpy(str1,g_bEnglish?"  Set":"设定");	
			}break;
		case 19: 	strcpy(str,"J");	break;
		case 20:	strcpy(str,"E");	break;
		case 21:	strcpy(str,g_bEnglish?"Cancel":"取消");	break;
		case 22:	strcpy(str,"4");	break;
		case 23:	strcpy(str,"5");	break;
		case 24:	strcpy(str,"6");	break;
		
		case 25:	
			{
				strcpy(str,g_bEnglish?"Count":"产量");
				strcpy(str1,g_bEnglish?"Clear":"清零");	
			}break;
		case 26:	
		{
			strcpy(str,g_bEnglish?"Count":"产量");
			strcpy(str1,g_bEnglish?"  Set":"设定");	
		}break;
		case 27:	strcpy(str,g_bEnglish?"Insert":"插行");	break;
		case 28:	strcpy(str,"L");	break;
		case 29: 	strcpy(str,"-");	break;
		case 30:	strcpy(str,"1");	break;
		case 31:	strcpy(str,"2");	break;
		case 32:	strcpy(str,"3");	break;
		
		case 33:	
		{
			strcpy(str,g_bEnglish?"Yclear":"送线");	
			strcpy(str1,g_bEnglish?"YClear":"清零");
		}break;
		
		case 34:	
			{
				strcpy(str,g_bEnglish?" Work":"工作");
				strcpy(str1,g_bEnglish?" Para":"参数");	
			}break;
		case 35:	strcpy(str,g_bEnglish?"Del":"删除");	break;
		case 36:	strcpy(str,"N");	break;
		case 37:	strcpy(str,"UP");	break;
		case 38:	strcpy(str,".");	break;
		case 39:	strcpy(str,"0");	break;
		case 40: 	strcpy(str,g_bEnglish?"Enter":"回车");	break;
		
		case 41:	strcpy(str,g_bEnglish?"Ratio":"倍率");	break;
		case 42:	
			{
				strcpy(str,g_bEnglish?"Speed":"寸动");
				strcpy(str1,g_bEnglish?"  Set":"速度");	
			}break;
		case 43:	strcpy(str,g_bEnglish?"DelAll":"全删");	break;
		case 44:	
		{
			strcpy(str,g_bEnglish?" Systm":"系统");	
			strcpy(str1,g_bEnglish?"Mange":"管理");
		}break;
		case 45:	strcpy(str,"DOWN");	break;
		case 46:	
		{
			strcpy(str,g_bEnglish?"Clear":"清除");
			//strcpy(str1,g_bEnglish?"Clear":"命令");	
		}break;
		case 47:	strcpy(str,"LEFT");	break;
		case 48:	strcpy(str,"RIGHT");	break;
		
		case 49:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(1,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ X":msg);
			break;
		}
		case 50:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(2,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ Y":msg);
			break;
		}
		case 51:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(3,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ Z":msg);
			break;
		}
		case 52:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(4,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ A":msg);
			break;
		}
		case 53:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(5,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ B":msg);
			break;
		}case 54:
		{
			char	msg[6] = "+ ";
			
			strcat(msg,axisMsg(6,"",FALSE));
			msg[4]='\0';
			strcpy(str,g_bEnglish?"+ C":msg);
			break;
		}
	
		case 55:	
		{
			strcpy(str,g_bEnglish?"PgU":"上页");		
		}break;
		
		case 56:	strcpy(str,g_bEnglish?"Start":"开始");	break;
		case 57:	strcpy(str,g_bEnglish?"End":"结束");	break;
		
		case 58:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(1,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ X":msg);
		 	break;
		 }
		case 59:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(2,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ Y":msg);
		 	break;
		 }
		 case 60:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(3,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ Z":msg);
		 	break;
		 }
		 case 61:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(4,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ A":msg);
		 	break;
		 }
		 case 62:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(5,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ B":msg);
		 	break;
		 }
		 case 63:
		{
			char	msg[6] = "\0";
			
			strcpy(msg,axisMsg(6,"",FALSE));
			msg[0]='-';
			msg[1]=' ';
		 	strcpy(str,g_bEnglish?"+ C":msg);
		 	break;
		 }
		
		
		
		case 64:	
		{
			strcpy(str,g_bEnglish?"PgD":"下页");
		}break;	
		
		case 65:	
		{
			//strcpy(str,g_bEnglish?"Handset":"单行");
			strcpy(str,g_bEnglish?"  Box":"手盒");	
			strcpy(str1,g_bEnglish?"Switch":"开关");
		}break;	
		case 66: 	
		{
			strcpy(str,g_bEnglish?" Hand":"各轴");	
			strcpy(str1,g_bEnglish?"Switch":"手轮");
		}break;
	}
	
	if((i<13 && i != 9)||(i>=49&&i!=56&&i!=57&&i!=65&&i!=66)||i==19||i==20||i==28||i==36)			//单行，黑色背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLACKCOLOR,1);
		CDC_OutCenTextKzk22(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:BLACKCOLOR);
	}
	else if(44==i || 65==i || 66==i)																//双行，黑色背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLACKCOLOR,1);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?4:20),g_ptrKeyRect[i].top+2,str,WHITECOLOR,status?MAGENTACOLOR:BLACKCOLOR,KZK_22);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?4:20),g_ptrKeyRect[i].top+2+24,str1,WHITECOLOR,status?MAGENTACOLOR:BLACKCOLOR,KZK_22);
	}
	else if(34 == i || (33==i && !g_bEnglish) || 42==i)											 	//双行，蓝色背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLUECOLOR,1);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?7:20),g_ptrKeyRect[i].top+2,str,WHITECOLOR,status?MAGENTACOLOR:BLUECOLOR,KZK_22);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?7:20),g_ptrKeyRect[i].top+2+24,str1,WHITECOLOR,status?MAGENTACOLOR:BLUECOLOR,KZK_22);
	}
	else if(25==i || 26==i || 17 == i || 18 == i)													//双行，黄色背景
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:YELLOWCOLOR,1);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?7:20),g_ptrKeyRect[i].top+2,str,BLACKCOLOR,status?MAGENTACOLOR:YELLOWCOLOR,KZK_22);
		CDC_OutText_UTF(g_ptrKeyRect[i].left+(g_bEnglish?7:20),g_ptrKeyRect[i].top+2+24,str1,BLACKCOLOR,status?MAGENTACOLOR:YELLOWCOLOR,KZK_22);
	}
	else
	{
		if(i==9||i==41 || (i==33 && g_bEnglish))													//单行，蓝色背景
		{
			CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLUECOLOR,1);
			CDC_OutCenTextKzk22(g_ptrKeyRect[i],str,WHITECOLOR,status?MAGENTACOLOR:BLUECOLOR);
		}
		else
		{
			if(i==27||i==35||i==40||i==56||i==57 || 43==i)											//单行，黄色背景
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:YELLOWCOLOR,1);
				CDC_OutCenTextKzk22(g_ptrKeyRect[i],str,BLACKCOLOR,status?MAGENTACOLOR:YELLOWCOLOR);
			}
			else if(45==i || 48==i)
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutText_UTF(g_ptrKeyRect[i].left+5,g_ptrKeyRect[i].top+14,str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR,KZK_22);
				//CDC_OutCenTextKzk22(g_ptrKeyRect_1[i],str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR);
			}
			else if(47==i)
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutText_UTF(g_ptrKeyRect[i].left+15,g_ptrKeyRect[i].top+14,str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR,KZK_22);
			}
			else if(21==i)
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutText_UTF(g_ptrKeyRect[i].left+2,g_ptrKeyRect[i].top+14,str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR,KZK_22);
			}
			else
			{
				CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
				CDC_OutCenTextKzk22(g_ptrKeyRect[i],str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR);

			}
		}
	}
	
	CDC_Rectangle2(g_ptrKeyRect[i],GREENCOLOR);
	
	if((i<9||i>=58)&&i!=64)
	{
		if(status)
			CDC_Fillellipse(g_ptrKeyRect[i].left+40,g_ptrKeyRect[i].top+5,3,3,g_TextColor3);
	}
	
	if(ledout)
	{
		if(i<9)
			LedOut(i,status?1:0);
		else if(i == 12 || i == 20 || i == 28)
		{
			if(i == 12)
			 LedOut(FILELED,status?1:0);
			 
			if(i == 20)
			 LedOut(CYLINDERLED,status?1:0);
			 
			if(i == 28)
			 LedOut(PRGLED,status?1:0);
		}
		else
		{
			if(i>=58)
				LedOut(i-40,status?1:0);
		}
	}		
}


void	CTestKey_OnKey(INT16U key,INT16U tkey)															//通过按钮也能处理输入输出
{
	INT16S i=-1,j=0;
	//INT8S buffMsg[80]=""; 
	
	
	
	switch(key)
	{
	
	case TBACKZERO:
	
		{
		#ifdef	PRINTBMP
			{
				char filename[0x20];
				INT16S i=0;
				
				
				strcpy(filename,"testKey");
				
				
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
		
		
		i=0;
		break;
		
	case TYSWITCH :	i=1;break;
	case TZSWITCH :	i=2;break;
	case TUSWITCH :	i=3;break;
	case TVSWITCH :	i=4;break;
	case TWSWITCH :	i=5;break;
	case TCYLINDER: i=6;break;
	case TSTOP 	  :	i=7;break;
	
	case TTEST:
		
		{
			g_bTestKey=g_bTestKey?FALSE:TRUE;
			
			if(g_bEnglish)
				OSQPost(MsgQueue,g_bTestKey?"Enter key test state!":"Exit key test state!");
			else
				OSQPost(MsgQueue,g_bTestKey?"进入按键测试状态!":"退出按键测试状态!");
				
			i=8;
			
			if(g_bTestKey==FALSE)
			{
				for(j=0;j<MAXTESTKEY;j++)
				{
					CTestKey_UpdateKeyStatus(j,FALSE,TRUE);
				}
				
				//AllLedOff();
			}
			else
			{
				for(j=0;j<MAXTESTKEY;j++)
					g_bKeyStatus[j]=FALSE;
				
			}
		}
		break;

	case TREAD     : i=9;break;
	case TSCOMMAND : i=10;break;
	case TGCOMMAND : i=11;break;
	
	case TMCOMMAND :
		if(!g_bTestKey)
			Ok_Exit=TRUE;
		else
			i=12;
		break;
		
	case TSAVE 	: 	i=13;break;
	case '7' 	: 	i=14;break;
	case '8' 	: 	i=15;break;
	case '9' 	: 	i=16;break;
	
	case TPROBECLR :i=17;break;
	case TPROBESET :i=18;break;
	case TJCOMMAND :i=19;break;
	case TECOMMAND :i=20;break;
			
	case TCANCEL: 
		{
			if(!g_bTestKey)
				Ok_Exit=TRUE;
			else
				i=21;
		}
		break;
	case '4' 		: i=22;break;
	case '5'		: i=23;break;
	case '6' 		: i=24;break;
	
	case TCOUNTCLR 	: i=25;break;
	case TCOUNTSET 	: i=26;break;
	case TINSERT   	: i=27;break;
	case TLCOMMAND 	: i=28;break;
	case TMINUS 	: i=29;break;
	
	case '1' 		: i=30;break;		
	case '2' 		: i=31;break;
	case '3' 		: i=32;break;
	

	case TSENDCLR 	: i=33;break;
	case TWORKPARA  : i=34;break;
	case TDEL 	  	: i=35;break;
	case TNCOMMAND  : i=36;break;
	case TUP      	: i=37;break;
	case TDOT		: i=38;break;
	case '0' 		: i=39;break;
	
	case TENTER		: i=40;break;

	case TBEILV 	:i=41;break;
	case TSTEPSPEED :i=42;break;
	case TDELALL	:i=43;break;
	case TSYSMANGE 	:i=44;break;
	case TDOWN 		:i=45;break;
	case TBACKSPACE :i=46;break;
		
	case TLEFT 	:	i=47;break;
	case TRIGHT :	i=48;break;
	
	case TXPLUS :   i=49;break;
	case TYPLUS :   i=50;break;
	case TZPLUS :   i=51;break;
	case TUPLUS :   i=52;break;
	case TVPLUS :   i=53;break;
	case TWPLUS :   i=54;break;
	
		
	case TPAGEUP : i=55;break;
	case TSTARTINPUT:i=56;break;
	case TENDINPUT :i=57;break;
	
	case TXMINUS :  i=58;break;
	case TYMINUS :  i=59;break;
	case TZMINUS :  i=60;break;
	case TUMINUS :  i=61;break;
	case TVMINUS :  i=62;break;
	case TWMINUS :  i=63;break;
	
		
		
	case TPAGEDOWN : i=64;break;
	case THANDBOX : i=65;break;
	
	case THANDSWITCH : i=66;break;
	
	default:		
		return;
	
	}
	if(RealIO(EXT_SINGLEDI,0,2))
	{
		i=7;
	}
	if((g_bTestKey||i==8)&&i!=-1)
	{
		g_bKeyStatus[i]=g_bKeyStatus[i]?FALSE:TRUE;
		CTestKey_UpdateKeyStatus(i,g_bKeyStatus[i],TRUE);
	}
	
	
		
}


INT16S	CTestKey_DoModal()
{
	INT16U key=0,tkey=0;
	//INT8S  js=0;
	//INT16S i=0;
	
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
