# include "includes.h"
# include "ucos_ii.h"

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        base.c
  创建人：       范志州
  创建日期：	2004/11/19
  主要功能描述:   各个任务的优先级以及ID分配，各个常用全局函数声明，另外包含了按键检测任务

  修改记录：
   1.  修改者:	      范志州
	   修改日期:	      2005/08/03
	   主要修改内容:   修正了最小堆栈空间指示，更改了全局函数的一些声明
   2. ...
*************************************************/

TEST_EXT	OS_EVENT 	*KeyMBox;
TEST_EXT	OS_EVENT 	*FileSem;
TEST_EXT  	OS_EVENT 	*MsgQueue;
//TEST_EXT	BOOL		g_bBmp;
TEST_EXT	BOOL		g_bPM;
TEST_EXT	BOOL		g_bRelease;

WORK_EXT 	INT8S		g_pszMsgBuf[0x30];
WORK_EXT	BOOL		g_bScrProtectRun;										//指示是否处于屏保运行状态,采用关闭显示器模式时使用

extern  	INT16U		g_uHzk16XMS;
//extern		INT8U		g_ucHzk16XMSArray[HZK16XMSSIZE];
//extern		INT8U		g_ucHzk16zXMSArray[HZK16ZXMSSIZE];
extern		BOOL		g_bInterface;
extern		BOOL		g_bEndCom;
extern 		BASEINI		g_Ini;
extern      BOOL        g_bEnglish;

//INT8U		g_ucBmpXMSArray[BMPXMSSIZE];
INT16U		g_uiPressKeyTime=0;
INT32S		g_lVclkRising=0; 
INT16S		SVGAmode=0;
char 		FileCopyBuf[5000];

/* 
SVGAmode=...
2 -> 640x480  
3 -> 800x600   
4 -> 1024x768  
5 -> 1280x1024 
6 -> 1600x1200 
*/



void ExitSystem(INT16S ex)
{
	DestroyXMS();
}

/*
void	LedOut(INT8U num,INT8U status)
{
#if FALSE
	char buf[10];
	char *p;
	static int pv=FALSE;	//单片机总线通讯加pv结互锁
	
	while(pv)OSTimeDly(10);
	pv = TRUE;
	p = buf;

	if(num>17)return;

	if(status)
		sprintf(buf,"%02d1\r\n",num);
	else
		sprintf(buf,"%02d0\r\n",num);

	Uart_Printf2(buf);
	
	OSTimeDly(5);
	
	pv = FALSE;

#endif	
}
*/

void	LedOut(INT8U num,INT8U status)
{
//*
//502按键板处理
	INT8U buf[0x10]="\0";
	INT16S st=-1;
	
	buf[0]=num/10+0x30;
	buf[1]=num%10+0x30;
	buf[2]=status?'1':'0';
	
	st=CTelCom_SendString(buf,3,10000);
	
	if(st!=0)
	{
		Uart_Printf("Send String Time Out\n");
	}
	
	OSTimeDly(4);
	
}




void	AllLedOff()
{
	INT16S i=0;

	for(i=0;i<23;i++)
		LedOut(i,0);
}


BOOLEAN  IsPressKey()
{
	return FALSE;	
}

/*
const INT16U keytableTH08DA[255] =
{
    NULL_KEY,F9,NULL_KEY,F5,F3,F1,F2,F12,    											//0x00 ~0x07   
    NULL_KEY,F10,F8,F6,F4,TAB,'`',NULL_KEY,      										//0x08 ~0x0f   
    NULL_KEY,ALT,LEFT_SHIFT,NULL_KEY,CTRL,'q','1',NULL_KEY,    							//0x10 ~0x17   
    NULL_KEY,NULL_KEY,'z','s','a','w','2',NULL_KEY,                  					//0x18 ~0x1f   
    NULL_KEY,'c','x','d','e','4','3',NULL_KEY,                       					//0x20 ~0x27   
    NULL_KEY,SPACE,'v','f','t','r','5',NULL_KEY,                 						//0x28 ~0x2f    
    NULL_KEY,'n','b','h','g','y','6',NULL_KEY,                       					//0x30 ~0x37   
    NULL_KEY,NULL_KEY,'m','j','u','7','8',NULL_KEY,                  					//0x38 ~0x3f   
    NULL_KEY,',','k','i','o','0','9',NULL_KEY,                       					//0x40 ~0x47
    NULL_KEY,'.','/','l',';','p','-',NULL_KEY,                       					//0x48 ~0x4f   
    NULL_KEY,NULL_KEY,'\'',NULL_KEY,'[','=',NULL_KEY,NULL_KEY,       					//0x50 ~0x57
    CAPS_LUCK,RIGHT_SHIFT,ENTER,']',NULL_KEY,'\\',NULL_KEY,NULL_KEY,      				//0x58 ~0x5f   
    NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,BACK,NULL_KEY, 				//0x60 ~0x67
    NULL_KEY,END,NULL_KEY,LEFT,HOME,NULL_KEY,NULL_KEY,NULL_KEY,   						//0x68 ~0x6f   
    INSERT,DEL,DOWN,NULL_KEY,RIGHT,UP,ESC,NUM_LUCK,  									//0x70 ~0x77
    F11,'+',PAGEDOWN,'-','*',PAGEUP,SCROLL_LUCK,NULL_KEY,          						//0x78 ~0x7f   
    NULL_KEY,NULL_KEY,NULL_KEY,F7,NULL_KEY                         						//0x80 ~0x87
};
*/


const INT16U keytableCNC820[255] =
{
    NULL_KEY,F9,NULL_KEY,F5,F3,F1,F2,F12,    //0x00 ~0x07   
    NULL_KEY,F10,F8,F6,F4,TAB,'`',NULL_KEY,      //0x08 ~0x0f   
    NULL_KEY,ALT,LEFT_SHIFT,NULL_KEY,CTRL,'q','1',NULL_KEY,    //0x10 ~0x17   
    NULL_KEY,NULL_KEY,'z','s','a','w','2',NULL_KEY,                  //0x18 ~0x1f   
    NULL_KEY,'c','x','d','e','4','3',NULL_KEY,                       //0x20 ~0x27   
    NULL_KEY,SPACE,'v','f','t','r','5',NULL_KEY,                 //0x28 ~0x2f    
    NULL_KEY,'n','b','h','g','y','6',NULL_KEY,                       //0x30 ~0x37   
    NULL_KEY,NULL_KEY,'m','j','u','7','8',NULL_KEY,                  //0x38 ~0x3f   
    NULL_KEY,',','k','i','o','0','9',NULL_KEY,                       //0x40 ~0x47
    NULL_KEY,'.','/','l',';','p','-',NULL_KEY,                       //0x48 ~0x4f   
    NULL_KEY,NULL_KEY,'\'',NULL_KEY,'[','=',NULL_KEY,NULL_KEY,       //0x50 ~0x57
    CAPS_LUCK,RIGHT_SHIFT,ENTER,']',NULL_KEY,'\\',NULL_KEY,NULL_KEY,      //0x58 ~0x5f   
    NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,NULL_KEY,BACK,NULL_KEY, //0x60 ~0x67
    NULL_KEY,END,NULL_KEY,LEFT,HOME,NULL_KEY,NULL_KEY,NULL_KEY,   //0x68 ~0x6f   
    INSERT,DEL,DOWN,NULL_KEY,RIGHT,UP,ESC,NUM_LUCK,  //0x70 ~0x77
    F11,'+',PAGEDOWN,'-','*',PAGEUP,SCROLL_LUCK,NULL_KEY,          //0x78 ~0x7f   
    NULL_KEY,NULL_KEY,NULL_KEY,F7,NULL_KEY                         //0x80 ~0x87
};




INT16U	GetKey_BotMap(void)
{
	INT16U 	KeyValue1=0;
    INT16U 	KeyScan;
    
   	KeyScan  =  GetKey_Bot();
   	
	KeyValue1 =  keytableCNC820[KeyScan];
	
    return KeyValue1;
}


INT16U GetKey(BOOL fastreturn)
{
	if(fastreturn)
		return GetKey_BotMap();
	else
	{
		while(1)
		{
			if(GetKey_BotMap()!=0)
				return 0;
			else
				OSTimeDly(1);
		}
	}
}


//#define	RESETDEBUG
#define	BIOSSTARTADDR	0x30000000

void  Check_Key_Task(void *data)
{
	INT16U 			key=0;
	INT8U			sta=0;
	INT16U 			lastkey=0;
	OS_MBOX_DATA	KeyMBoxData;
	INT16U  		ResetKey[3]={0,0,0};
	INT16S  		prekey=0;
	
	Uart_Printf("Check_Key_Task...\n");
	
	data = data;

	for (;;)
	{
		sta=OSMboxQuery(KeyMBox,&KeyMBoxData);
	
		if(sta == OS_NO_ERR)
		{		
			if(KeyMBoxData.OSMsg == (void *)0)  //邮箱为空时才检测并读取按键
			{
				//Uart_Printf("tes-key=%d\n",GetKey_BotMap());	
				
				if((key=(INT16U)GetKey_BotMap())!=0)
				{	
					//Uart_Printf("test\n");		
					sta=OSMboxPost(KeyMBox,(void *)&key);
					
					if(sta != OS_NO_ERR)
					{
					/*
						switch(sta)
						{
						case OS_MBOX_FULL:			CDC_OutText(360,290,"KeyMbox Full!",14,0);break;
						case OS_ERR_EVENT_TYPE:		CDC_OutText(360,310,"KeyMbox Event type!",14,0);break;
						case OS_ERR_PEVENT_NULL:	CDC_OutText(360,330,"KeyMbox Pevent Null!",14,0);break;
						case OS_ERR_POST_NULL_PTR:	CDC_OutText(360,350,"KeyMbox Post Null Ptr!",14,0);break;
						}
					*/
					}
					
					#ifdef	DEBUGDISP
					CDC_PutColorBar(520,10,32,20,0,1);
					CDC_OutText(520,10,"T2_0",YELLOWCOLOR,0);
					#endif
					
					if(lastkey!=key)
					{
						INT16U i=0;
						
						g_uiPressKeyTime=0;
						lastkey=key;
					
						for(i=0;i<350;i++)
						{
							OSTimeDly(1);
							
							if(GetKey_BotMap()==0)		//增加一道一豪秒的滤波
							{
								OSTimeDly(1);
								if(GetKey_BotMap()==0)
									break;
							}
						}
						//OSTimeDlyHMSM(0,0,0,300);
					}
					else
					{
						INT16U i=0;

						for(i=0;i<46;i++)
						{
							OSTimeDly(1);
						
							if(GetKey_BotMap()==0)
							{
								g_uiPressKeyTime=46;
								break;
							}
						}
						
						g_uiPressKeyTime+=46;
						
						if(g_uiPressKeyTime>60000)
							g_uiPressKeyTime=60000;
					}	
					
					
					if(!g_bRelease)
					{

						if(key=='X'||key=='Y'||key=='Z'||key=='x'||key=='y'||key=='z')
						{
							ResetKey[prekey]=key;
							prekey++;
						
							if(prekey>2)
								prekey=0;

							if((ResetKey[0]=='X'&&ResetKey[1]=='Y'&&ResetKey[2]=='Z')||(ResetKey[0]=='x'&&ResetKey[1]=='y'&&ResetKey[2]=='z'))
							{			
								Uart_Printf("Reset Start...\n");

								{
									void (*fpbois)(void) = (void (*)(void))BIOSSTARTADDR;
									(*fpbois)();

								}
							}
						}
						else
						{
							prekey=0;
							ResetKey[0]=0;
							ResetKey[1]=0;
							ResetKey[2]=0;
						}
					}
				}
				else														//没有按键时的处理
				{
					#ifdef	DEBUGDISP
					
						char buf[0x04];
						
						sprintf(buf,"%d",OSIntNesting);
						CDC_OutText(496,10,buf,WHITECOLOR,0);
						CDC_PutColorBar(520,10,32,20,0,1);
						CDC_OutText(520,10,"T2_1",YELLOWCOLOR,0);
						
					#endif
					
					lastkey=0;
					OSTimeDly(10);

				}
			}
			else															//邮箱满时的处理
			{
				#ifdef	DEBUGDISP
					CDC_PutColorBar(520,10,32,20,0,1);
					CDC_OutText(520,10,"T2_2",YELLOWCOLOR,0);
				#endif
				
				key=GetKey_BotMap();
				
//				if(key!=lastkey)
//					lastkey=key;

				OSTimeDlyHMSM(0,0,0,100);
			}
		}
		else
		{
			OSTimeDlyHMSM(0,0,0,100);
		}
	}
}



INT16U	KeyMBoxGetKey()
{
	INT16U key;
	void *msg;
	
	msg = OSMboxAccept(KeyMBox);
	
	if(msg!=(void *)0)
	{
		key=*(INT16U*)msg;
		
		return key;
	}
	else
		return 0xffff;
}


void DispTaskStat (INT8U id)
{

	OS_STK_DATA  	data[30];
	char 			s[80]="\0";
	INT8U			err;
	static INT32S	maxstk[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	err=OSTaskStkChk(id+(TEACH_TASK_PRIO),&data[id]);

	if(err==OS_NO_ERR)
	{
		if(data[id].OSUsed!=maxstk[id]&&data[id].OSUsed>maxstk[id])
		{
			maxstk[id]=data[id].OSUsed;	
			memset(s,NULL,80);
			
			sprintf(s, "%-24s   %5u    %5d      %5u",
					TaskUserData[id+(TEACH_TASK_PRIO)].TaskName,
					data[id].OSFree+data[id].OSUsed,
					maxstk[id],
					data[id].OSFree);
					
            CDC_OutText(5,((INT16S)id)*16,s,g_TextColor1,g_nBk);
		}
		
	}
}


BOOL	PressKeyTime(INT16U ti)
{
	return (g_uiPressKeyTime>ti)?TRUE:FALSE;
}


void	UartDisp(void)
{
	Uart_Printf("Enter Int\n");
}


void CDC_SetRectangle1(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT32S color,INT16S style)
{
	INT16S x1=x+w,y1=y+h;
	INT16S len=strlen(str1)*8+16;
	
	CDC_Rectangle4(x+1,y+1,x1+1,y1+1,WHITECOLOR);
	CDC_Rectangle4(x,y,x1,y1,CYANCOLOR);
	CDC_PutColorBar(x+44,y-8,len,16,color,style);
	CDC_OutCenText1(x+40,x+len+40,y-10,str1,WHITECOLOR,color);
	CDC_Rectangle4(x+44,y-12,x+len+56,y+13,WHITECOLOR);
	CDC_Rectangle4(x+45,y-13,x+len+55,y+14,WHITECOLOR);
}


void CDC_SetRectangle2(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT32S color,INT16S style)
{
	INT16S x1=x+w,y1=y+h;
	INT16S len=strlen(str1)*8+16;
	
	CDC_Rectangle4(x+1,y+1,x1+1,y1+1,WHITECOLOR);
	CDC_Rectangle4(x,y,x1,y1,CYANCOLOR);
	CDC_PutColorBar(x+44,y-8,len,16,color,style);
	CDC_OutCenText1(x+40,x+len+40,y-10,str1,WHITECOLOR,color);
	CDC_Rectangle4(x+44,y-12,x+len+46,y+13,WHITECOLOR);
	CDC_Rectangle4(x+45,y-13,x+len+45,y+14,WHITECOLOR);
}

void CDC_SetRectangle3(INT16S x,INT16S y,INT16S w,INT16S h,char *str1,INT16S titleLen,INT32S color,INT16S style)
{
	INT16S x1=x+w,y1=y+h;
	
	CDC_Rectangle4(x+1,y+1,x1+1,y1+1,WHITECOLOR);
	CDC_Rectangle4(x,y,x1,y1,CYANCOLOR);
	CDC_PutColorBar(x+44,y-8,titleLen,16,color,style);
	CDC_OutTextLR_UTF(x+50,y-10,str1,WHITECOLOR,color,KZK_22,0,1);
	CDC_Rectangle4(x+44,y-12,x+titleLen+44,y+13,WHITECOLOR);
	CDC_Rectangle4(x+45,y-13,x+titleLen+45,y+14,WHITECOLOR);
}



void	CDC_OutCenText1(INT16S x1,INT16S x2,INT16S y,char *string,INT32S color,INT32S bk)
{
	INT16S len;
	if( string == NULL ) return;
	len = strlen(string)*8;
	CDC_OutText_UTF((x2-x1-len)/2+x1,y,string,color,bk,KZK_22);
}


void	CDC_AsciiChar24(INT16S x,INT16S y,  INT8S ch,INT32S color,INT32S bk)
{
	char buf[2]="\0";
	
	buf[0]=ch;
	CDC_OutText24(x,y,buf,color,bk);
}


void	CDC_AsciiCharMzk_24(INT16S x,INT16S y,  INT8S ch,INT32S color,INT32S bk)
{
	char buf[2]="\0";
	
	buf[0]=ch;
	CDC_OutText_UTF(x,y,buf,color,bk,MZK_24);
}


void	CDC_OutCenText24(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*12;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText24((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*24)/2+rect.top,
		string,
		color,bk);
}


void	CDC_OutCenTextKzk24(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*12;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*24)/2+rect.top,
		string,
		color,bk,KZK_24);
}


/*void	CDC_OutCenTextKzk16(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*8;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*16)/2+rect.top,
		string,
		color,bk,KZK_16);
}*/


void	CDC_OutCenTextKzk22Newline(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*11;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
		rect.top+2,
		string,
		color,bk,KZK_22);
}


void	CDC_OutCenTextKzk22(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*11;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*22)/2+rect.top,
		string,
		color,bk,KZK_22);
}


void	CDC_OutCenTextSzk24(CRect rect,char *string,INT32S color,INT32S bk)		//数字字库显示
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*12;
	
	i=FindEnterCount(string)+1;
	
	{
		CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
			(rect.bottom-rect.top-i*24)/2+rect.top,
			string,
			color,bk,MZK_24);
	}
}


void	CDC_TextSelect_24_CRect(CRect rect,INT16S style,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	INT16S len =0;
	
	
	INT16S x=rect.left; 
	INT16S y=rect.top;
	INT16S w=rect.right-rect.left;
	INT16S h=rect.bottom-rect.top;
	
	
	len = strlen(string)*12;

	if(strstr(string, ".")!=NULL)
	{
		len = len-6;
	}
	
	if(strstr(string, "M")!=NULL)
	{
		len = len+6;
	}	

	switch(style)
	{
		case 0:																	//居中对齐
			posx = (INT16S)(x+(w-len)/2.0);
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 1:																	//右对齐
			posx = x+w-len;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 2:																	//左对齐
			posx = x;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
		
		default:
			break;
	}

	CDC_OutText_UTF(posx,posy,string,color,bk,MZK_24);
}




/*void	CDC_OutCenTextSzk20(CRect rect,char *string,INT32S color,INT32S bk)		//数字字库显示
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*10;
	
	i=FindEnterCount(string)+1;
	
	{
		CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
			(rect.bottom-rect.top-i*20)/2+rect.top,
			string,
			color,bk,MZK_20);
	}
}*/


void	CDC_OutLeftTextSzk24(CRect rect,char *string,INT32S color,INT32S bk)		//数字字库显示
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*12;
	
	i=FindEnterCount(string)+1;
	
	{
		CDC_OutText_UTF(rect.left+2,
			(rect.bottom-rect.top-24)/2+rect.top,
			string,
			color,bk,MZK_24);
	}
}


/*void	CDC_OutCenTextKzk26(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	len = strlen(string)*13;
	
	i=FindEnterCount(string)+1;
	
	
	
	CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*26)/2+rect.top,
		string,
		color,bk,KZK_26);
}*/


void	CDC_OutCenText24Ex(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;

	if( string == NULL ) return;

	len = strlen(string)*12;
	
	i = FindEnterCount(string)+1;
	
	/*if(g_bInterface)
	{
		CDC_OutText_UTF((rect.right-rect.left-len/i)/2+rect.left,
			(rect.bottom-rect.top-i*32)/2+rect.top+1,
			string,
			color,bk,MZK_32);
	}
	else*/
	{	
		CDC_OutText24((rect.right-rect.left-len/i)/2+rect.left,
			(rect.bottom-rect.top-i*24)/2+rect.top,
			string,
			color,bk);
	}
}


void	CDC_TextSelect_24(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	BOOL   display = FALSE;
	INT16S i = 0;
	
	switch(style)
	{
		case 0:		
																		//居中对齐
			posx = (INT16S)(x+(w-strlen(string)*12)/2.0);
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 1:																	//右对齐
			posx = x+w-strlen(string)*12-4;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 2:																	//左对齐
			posx = x;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
		
		default:
			break;
	}
	
	for(i=0;i<strlen(string);i++)
	{
		if(strncmp(string+i,".",1)==0)
		{
			display = TRUE;
			break;
		}
		else
			display = FALSE;
	}
	
	
	if(display)
		CDC_OutText_UTF(posx+6,posy,string,color,bk,MZK_24);
	else
		CDC_OutText_UTF(posx,posy,string,color,bk,MZK_24);
}


void	CDC_DrawEdit_1(CRect	rect,INT32S color)
{
	INT16S x1 = rect.left;
	INT16S x2 = rect.right;
	INT16S y1 = rect.top;
	INT16S y2 = rect.bottom;

	INT16S x=x1,y=y1,xx=x2,yy=y2;

	CDC_PutColorBar(x1,y1,x2-x1,y2-y1,color,1);
	

	//CDC_Line(x,y,x,yy,LIGHTBLACKCOLOR);
	//CDC_Line(x,y,xx,y,LIGHTBLACKCOLOR);
	
	CDC_Line(x,y,x,yy,BLACKCOLOR);
	CDC_Line(x,y,xx,y,BLACKCOLOR);

	//CDC_Line(x+1,y+1,x+1,yy-1,BLACKCOLOR);
	//CDC_Line(x+1,y+1,xx-1,y+1,BLACKCOLOR);
	
	//CDC_Line(x+1,y+2,x+1,yy-2,BLACKCOLOR);
	//CDC_Line(x+1,y+2,xx-1,y+2,BLACKCOLOR);

	CDC_Line(x,yy,xx,yy,WHITECOLOR);
	CDC_Line(xx,y,xx,yy,WHITECOLOR);

	CDC_Line(x+1,yy-1,xx-1,yy-1,WHITECOLOR);
	CDC_Line(xx-1,y+1,xx-1,yy-1,WHITECOLOR);


}

void	CDC_DrawEdit_2(CRect	rect,INT32S color)
{
	INT16S x1 = rect.left;
	INT16S x2 = rect.right;
	INT16S y1 = rect.top;
	INT16S y2 = rect.bottom;

	INT16S x=x1,y=y1,xx=x2,yy=y2;

//	CDC_PutColorBar(x1,y1,x2-x1,y2-y1,color,1);

	
	CDC_Line(x,y,x,yy,BLACKCOLOR);
	CDC_Line(x,y,xx,y,BLACKCOLOR);

	CDC_Line(x,yy,xx,yy,WHITECOLOR);
	CDC_Line(xx,y,xx,yy,WHITECOLOR);

	CDC_Line(x+1,yy-1,xx-1,yy-1,WHITECOLOR);
	CDC_Line(xx-1,y+1,xx-1,yy-1,WHITECOLOR);


}

/*void	CDC_DrawWin26(INT16S x,INT16S y,INT16S w,INT16S h,char *title)
{
	CRect rect;

	CDC_PutColorBar(x,y,w,h, DARKGRAYCOLOR,1);
	CDC_PutColorBar(x,y,w,28, DARKBLUECOLOR,100);
	//CDC_PutColorBar(x,y,w,28,g_nBk,1);
	
	CDC_Rectangle4(x,y,w+x,y+28,YELLOWCOLOR);
	CDC_Rectangle4(x,y,x+w,h+y,WHITECOLOR);
	
	rect.left=x;
	rect.right=x+w;
	rect.top=y;
	rect.bottom=y+28;
	
	CDC_OutCenTextKzk26(rect,title,WHITECOLOR,DARKBLUECOLOR);
}*/

void	CDC_DrawWin24(INT16S x,INT16S y,INT16S w,INT16S h,char *title)
{
	CRect rect;
	
	CDC_PutColorBar(x,y,w,h,DARKGRAYCOLOR,1);
	CDC_PutColorBar(x,y,w,34,DARKBLUECOLOR,1);

	CDC_Rectangle(x,y,w+x,y+34,SILVERCOLOR);
	CDC_Rectangle(x+1,y+1,w+x-1,y+34-1,GREENCOLOR);
	CDC_Rectangle(x+2,y+2,w+x-1,y+34-2,GREENCOLOR);
	//CDC_Rectangle(x+3,y+3,w+x-1,y+34-3,SILVERCOLOR);
	
	CDC_Rectangle(x,y,x+w,h+y,SILVERCOLOR);
	CDC_Rectangle(x+1,y+1,x+w-1,h+y-1,GREENCOLOR);
	CDC_Rectangle(x+2,y+2,x+w-2,h+y-2,GREENCOLOR);
	//CDC_Rectangle(x+3,y+3,x+w-3,h+y-3,SILVERCOLOR);

	rect.left=x;
	rect.right=x+w;
	rect.top=y+2;
	rect.bottom=y+32;
	
	//CDC_OutCenText24(rect,title,YELLOWCOLOR,g_nBk);
	CDC_OutCenTextKzk24(rect,title,WHITECOLOR,DARKBLUECOLOR);
}

#if FALSE
void	CBmp_GetFullScr24Bit(char *filename,INT16S isVirtual)
{

	INT32S i,j;
	INT32U	dispram=0x33000000;
	INT32S count=0;
	FS_FILE	*fp;
	INT8U err;
	
	INT8U buf[]={0x42,0x4d,0x36,0x10,0x0e,0,0,0,0,0,0x36,00,0,0,40,0,0,0,32,3,0,0,88,2,0,0,
				 1,0,0x18,0,0,0,0,0,0,0x10,0x0e,0,0xc4,0x0e,0,0,0xc4,0x0e,0,0,0,0,0,0,0,0,0,0};
	if(isVirtual==1)
	{
		buf[18]=((getmaxx()+1)*2)%256;
		buf[19]=((getmaxx()+1)*2)/256;
	}
	else
	{
		buf[18]=((getmaxx()+1))%256;
		buf[19]=((getmaxx()+1))/256;
	}
	
	buf[22]=(getmaxy()+1)%256;
	buf[23]=(getmaxy()+1)/256;
	
	if(SVGAmode!=0)
		buf[28]=24;
	else
	{
		buf[28]=1;
		buf[2]=0xbe;buf[3]=0x25;buf[4]=0;
		buf[10]=0x3e;
		buf[34]=0x80;
		buf[35]=0x25;
		buf[36]=0;
		buf[37]=0;
	}
	
	OSSemPend(FileSem,10000,&err);
	
	if(err!=OS_NO_ERR)
	{		
		Uart_Printf("Write 24Bit Bmp File Sem Timeout!\n");		
		return ;
	}
	
	fp=FS_FOpen(filename,"wb");
	
	if(fp==NULL)
	{
		Uart_Printf("Write 24Bit Bmp File Open Err!\n");
		OSSemPost(FileSem);
		return;
	}
	
	FS_FWrite(buf,54,1,fp);
	
	if(isVirtual==0)
	{
		for(i=getmaxy();i>=0;i--)
		{
			INT8U r,g,b,r1,g1,b1;
			INT8U tmp0,tmp1,tmp2,tmp3;
			
			#ifdef	LCDG64K	
				for(j=0;j<=getmaxx()*2;j+=4)
				{
					
					
					tmp2=*(INT8U *)(dispram+(i*800*2)*2+j+0);
					tmp3=*(INT8U *)(dispram+(i*800*2)*2+j+1);
					
					tmp0=*(INT8U *)(dispram+(i*800*2)*2+j+2);
					tmp1=*(INT8U *)(dispram+(i*800*2)*2+j+3);
					
					r=(tmp0&0x1f)<<3;
					g=((tmp1&0x07)<<5)|((tmp0&0xe0)>>3);
					b=(tmp1&0xf8);
					
					r1=(tmp2&0x1f)<<3;
					g1=((tmp3&0x07)<<5)|((tmp2&0xe0)>>3);
					b1=(tmp3&0xf8);
					
					g_ucBmpXMSArray[count++]=r;
					g_ucBmpXMSArray[count++]=g;
					g_ucBmpXMSArray[count++]=b;
					
					g_ucBmpXMSArray[count++]=r1;
					g_ucBmpXMSArray[count++]=g1;
					g_ucBmpXMSArray[count++]=b1;
				}
			#else		
				for(j=0;j<(getmaxx()+1)*4;j+=4)
				{
					
					g_ucBmpXMSArray[count++]=*(INT8U *)(dispram+(i*800*2)*4+j+1);
					g_ucBmpXMSArray[count++]=*(INT8U *)(dispram+(i*800*2)*4+j+2);
					g_ucBmpXMSArray[count++]=*(INT8U *)(dispram+(i*800*2)*4+j+3);
					
				}
			#endif		
		}
	}
	else
	{
		for(i=getmaxy();i>=0;i--)
		{
			INT8U r,g,b,r1,g1,b1;
			INT8U tmp0,tmp1,tmp2,tmp3;
			
			for(j=0;j<=getmaxx()*4;j+=4)
			{
				
				
				tmp2=*(INT8U *)(dispram+(i*800*2)*2+j+0);
				tmp3=*(INT8U *)(dispram+(i*800*2)*2+j+1);
				
				tmp0=*(INT8U *)(dispram+(i*800*2)*2+j+2);
				tmp1=*(INT8U *)(dispram+(i*800*2)*2+j+3);
					
				
				
				r=(tmp0&0x1f)<<3;
				g=((tmp1&0x07)<<5)|((tmp0&0xe0)>>3);
				b=(tmp1&0xf8);
				
				r1=(tmp2&0x1f)<<3;
				g1=((tmp3&0x07)<<5)|((tmp2&0xe0)>>3);
				b1=(tmp3&0xf8);
				
				g_ucBmpXMSArray[count++]=r;
				g_ucBmpXMSArray[count++]=g;
				g_ucBmpXMSArray[count++]=b;
				
				g_ucBmpXMSArray[count++]=r1;
				g_ucBmpXMSArray[count++]=g1;
				g_ucBmpXMSArray[count++]=b1;
			}		
		}
	}
	
	FS_FWrite(g_ucBmpXMSArray,count,1,fp);
	FS_FClose(fp);
	
	OSSemPost(FileSem);
	
}
#endif



void	CDC_OutRightText24(INT16S x,INT16S y,char *string,INT32S color,INT32S bk)
{
	INT16S i = 0,len;
	
	if( string == NULL ) return;
	
	for(i=0;i<strlen(string);i++)
	{
		if(string[i]==NULL)break;

	}
	
	len = i*16;

	CDC_OutText24((x-len),y,string,color,bk);
}


INT32S SuperPassword(INT32S ID ,INT32S SerialNo)
{

	INT32S num = ID+SerialNo+8672405;
	INT32S pass = 0,c=0;

	while (num !=0)
	{
			c= num%9;
			
			num = num / 9;
			pass = pass*10 + c;
			
			
	} 

	Uart_Printf("%ld %ld %ld\n",ID,SerialNo,pass);
	
	return pass;
}





/*void	CDC_OutCenText32(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT32S	x,y;
	
	x = rect.left + (rect.right - rect.left - strlen(string)*16)/2.0;
	y = rect.top + (rect.bottom - rect.top - 32)/2.0;
	
	CDC_OutText_UTF(x,y,string,color,bk,MZK_32);
}*/


void	CDC_Rectangle3(CRect rect,INT32S color)
{
	CDC_Line(rect.left,rect.top,rect.right,rect.top,color);
	CDC_Line(rect.left,rect.bottom,rect.right,rect.bottom,color);
	
	CDC_Line(rect.left,rect.top,rect.left,rect.bottom,g_TextColor8);
	CDC_Line(rect.right,rect.top,rect.right,rect.bottom,g_TextColor8);
}

void	CDC_Rectangle4(INT16S x1,INT16S y1,INT16S x2,INT16S y2,INT32S color)
{
	CDC_Line(x1,y1,x2,y1,color);
	CDC_Line(x1,y2,x2,y2,color);
	
	CDC_Line(x1,y1,x1,y2,LIGHTGRAYCOLOR);
	CDC_Line(x2,y1,x2,y2,LIGHTGRAYCOLOR);
}


void	CDC_DrawWin1(INT16S x,INT16S y,INT16S w,INT16S h,char *title)
{
	CRect rect;
	
	CDC_PutColorBar(x,y,w,h,BLACKCOLOR,1);
	CDC_PutColorBar(x,y,w,28,DARKBLUECOLOR,100);
	CDC_Rectangle4(x,y,w+x,y+28,YELLOWCOLOR);
	
	CDC_Rectangle4(x,y,x+w,h+y,WHITECOLOR);
	rect.left=x;
	rect.right=x+w;
	rect.top=y;
	rect.bottom=y+28;
	CDC_OutCenTextKzk24(rect,title,WHITECOLOR,DARKBLUECOLOR);
}


/*注以下代码段放BASE.C的结尾*/

void CDC_OutTextLR_UTF(INT16S x,INT16S y,char *string,INT32S color,INT32S bk,int zktype,int ilen,int iLR)
{

	INT8S tBuf[0x30];
	
	INT16S  i=0; 
	INT16S j=0;
	
	memset(tBuf,NULL,sizeof(tBuf));

	if(strlen(string)>20)
		return;

	if(ilen>20)
		ilen = 20;

	if(ilen<0)
		ilen =0;

	if( iLR==0)								//固定长度右对齐
	{
		j=strlen(string);
		
		for(i=0;i<(ilen+1)*2-j*2;i++)
			tBuf[i]=' ';
			
		strcat(tBuf,string);
		
		CDC_OutText_UTF(x,y,tBuf,color,bk,zktype);
	}	
	else 									//固定长度左对齐
	{   
	
		strcpy(tBuf,string);

		j=strlen(tBuf);
		
		for(i=strlen(tBuf)*2;i<(ilen+1)*2;i++)
			tBuf[j++]=' ';
	
		CDC_OutText_UTF(x,y,tBuf,color,bk,zktype);
	}
}		
	
	
	

INT8S* axisMsg(INT16S ch,INT8S* msg,BOOL bMsg)			//合并轴名与消息，当该函数直接作为OSQPost的参数时，给bMsg赋值为TRUE
{
	INT8S  *axisNames[15] ={"外径","送线","转芯","卷取","夹耳","节距","切刀","上切","下切","转线","","",""};
	static INT8S  nameBuff[0x80]="";
	static INT8S  msgBuff[0x80]="";
	INT8S  *defaultName[MAXCH]={"凸轮","送线","转芯","卷取","卷取","夹耳"};
	
	INT8S  *englishName[MAXCH]={"X-Axis","Y-Axis","Z-Axis","A-Axis","B-Axis","C-Axis"};
	
	memset(nameBuff,0,sizeof(nameBuff));
	memset(msgBuff,0,sizeof(msgBuff));
	
	//Uart_Printf("ch = %d,g_Ini.iLeftOrRight = %d\n",ch,g_Ini.iLeftOrRight);
	
	//对调第三四轴的名字
	if(g_Ini.iLeftOrRight == 1)
	{
		if(ch == 3)
		{
			ch = 4;
		}
		else if(ch == 4)
		{	
			ch = 3;
		}
		
		//Uart_Printf("2ch = %d,g_Ini.iLeftOrRight = %d\n",ch,g_Ini.iLeftOrRight);
	}
	
	
	if(ch<1 || ch>MAXCH || strlen(msg)>0x7a)
	{	
		if(!bMsg)
			return nameBuff;
		else
			return msgBuff;
	}
	else
	{	
	
		if(g_bEnglish)
		{
			strcpy(msgBuff,englishName[ch-1]);
			return strcat(msgBuff,msg);
		}
		else
		{
			if(!bMsg)
			{
				if(g_Ini.iAxisNameNo[ch-1] == 0)
				{
					strcpy(nameBuff,defaultName[ch-1]);
				}
				else
				{		
					strcpy(nameBuff,axisNames[g_Ini.iAxisNameNo[ch-1]-1]);
				}
				
				return strcat(nameBuff,msg);
			}
			else
			{
				if(g_Ini.iAxisNameNo[ch-1] == 0)
				{
					strcpy(msgBuff,defaultName[ch-1]);
				}
				else
				{		
					strcpy(msgBuff,axisNames[g_Ini.iAxisNameNo[ch-1]-1]);
				}
				
				return strcat(msgBuff,msg);
			}
		}
	}
}


void ps_debugout(char *fmt, ...)
{
	va_list ap;
	char str[256];

	va_start(ap,fmt);
	vsprintf(str, fmt, ap);

	printf("%s",str);

	va_end(ap);

	return ;
}


void Uart_Printf(char *fmt, ...)
{
	va_list ap;
	char str[256];

	va_start(ap,fmt);
	vsprintf(str, fmt, ap);

	printf("%s",str);

	va_end(ap);

	return ;
}


	
	
	
	
