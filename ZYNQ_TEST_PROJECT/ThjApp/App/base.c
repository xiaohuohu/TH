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
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;

extern  	INT16U		g_uHzk16XMS;
//extern		INT8U		g_ucHzk16XMSArray[HZK16XMSSIZE];
//extern		INT8U		g_ucHzk16zXMSArray[HZK16ZXMSSIZE];
extern		INT16S		g_iCurEditNo;
TEST_EXT	BOOL		g_bEnglish;

//INT8U		g_ucBmpXMSArray[BMPXMSSIZE];
INT16U		g_uiPressKeyTime=0;
INT32S		g_lVclkRising=0; 
INT16S		SVGAmode=0;
char 		FileCopyBuf[5000];
extern		INT32S		g_iStopScrTime;	
/* 
SVGAmode=...
2 -> 640x480  
3 -> 800x600   
4 -> 1024x768  
5 -> 1280x1024 
6 -> 1600x1200 
*/


/*BOOL	CopyFile(INT8S *dest,INT8S *source)
{
	FS_FILE 	*fp2;
	FS_FILE 	*fp1;
	INT32U		lFileLen=0;
	INT8U 		err;
	INT32S 		iBufSize = sizeof(FileCopyBuf);
		
	OSSemPend(FileSem,5000,&err);
	
	if(err!=OS_NO_ERR)
	{
		OSQPost(MsgQueue,"Copy File Sem Timeout!");
		return FALSE;
	}
			
	memset(FileCopyBuf,NULL,iBufSize);
	
	fp1 = FS_FOpen(source,"rb");
	
	if( !fp1 )
	{
		OSSemPost(FileSem);
		return FALSE;
	}
			
	lFileLen=fp1->size;
	
	fp2 = FS_FOpen(dest,"wb");
	
	if( !fp2 )
	{
		OSSemPost(FileSem);
		return FALSE;
	}
		
	if(lFileLen<iBufSize)
	{
		FS_FRead(FileCopyBuf,lFileLen,1,fp1);
		FS_FWrite(FileCopyBuf,lFileLen,1,fp2);
	}
	else
	{
		while(1)
		{
			FS_FRead(FileCopyBuf,iBufSize,1,fp1);
			FS_FWrite(FileCopyBuf,iBufSize,1,fp2);
			
			lFileLen-=iBufSize;
			
			if(lFileLen<iBufSize)
			{
				FS_FRead(FileCopyBuf,lFileLen,1,fp1);
				FS_FWrite(FileCopyBuf,lFileLen,1,fp2);
				break;
			}
		}
	}

	FS_FClose(fp1);
	FS_FClose(fp2);
	
	OSSemPost(FileSem);
	
	return TRUE;
}

*/
void ExitSystem(INT16S ex)
{
	DestroyXMS();
}


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


/*const INT16U keytableTH08DA[255] =
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


const INT16U keytableTH08HA[255] =
{
	NULL_KEY,F1,F7,'s',UP,'p','7','8','9',
	         F10,F11,LEFT,'-',RIGHT,'4','5','6',
	         F3,F8,'i',DOWN,'j','1','2','3',
	         F4,F9,ESC,'=','q','.','0',ENTER,
	         F5,'w','t','r',F12,'z','h','e',
	         F6,'a','b','c','d','m','x','y'
};


INT16U	GetKey_BotMap(void)
{
	INT16U 	KeyValue1=0;
    INT16U 	KeyScan;
    
   	KeyScan  =  GetKey_Bot();
   	
   	
	KeyValue1 =  keytableTH08HA[KeyScan];
	
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
				if((key=(INT16U)GetKey_BotMap())!=0)
				{			
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
					
					if(lastkey!=key)
					{
						INT16U i=0;
						
						g_uiPressKeyTime=0;
						lastkey=key;
					
						for(i=0;i<300;i++)
						{
							OSTimeDly(1);
							
							if(GetKey_BotMap()==0)
								break;
						}
						//OSTimeDlyHMSM(0,0,0,300);
					}
					else
					{
						INT16U i=0;

						for(i=0;i<96;i++)
						{
							OSTimeDly(1);
						
							if(GetKey_BotMap()==0)
							{
								g_uiPressKeyTime=96;
								break;
							}
						}
						
						g_uiPressKeyTime+=96;
						
						if(g_uiPressKeyTime>60000)
							g_uiPressKeyTime=60000;
					}	

					if(!g_bRelease)
					{

						if(key==TX || key==TY || key==TZ)
						{
							ResetKey[prekey]=key;
							prekey++;
						
							if(prekey>2)
								prekey=0;

							if((ResetKey[0]==TX && ResetKey[1]==TY && ResetKey[2]==TZ))
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
					lastkey=0;
					OSTimeDly(10);

				}
			}
			else															//邮箱满时的处理
			{
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
		
		g_iStopScrTime = OSTimeGet();
		
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


void	CDC_AsciiChar24(INT16S x,INT16S y,  INT8S ch,INT32S color,INT32S bk)
{
	char buf[2]="\0";
	
	buf[0]=ch;
	CDC_OutText24Ex(x,y,buf,color,bk);
}


void	CDC_OutCenText24(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;
	
	if( string == NULL ) return;
	
	if(g_bEnglish)
		len = strlen(string)*12;
	else
		len = strlen(string)*12;
	
	i=FindEnterCount(string)+1;
	
	CDC_OutText24Ex((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*24)/2+rect.top,
		string,
		color,bk);
}


void	CDC_OutCenText24Ex(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;

	if( string == NULL ) return;
	
	len = strlen(string)*14;
	i=FindEnterCount(string)+1;
	
	CDC_OutText24Ex((rect.right-rect.left-len/i)/2+rect.left,
		(rect.bottom-rect.top-i*24)/2+rect.top,
		string,
		color,bk);

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

	CDC_Line(x+1,y,x+1,yy-1,BLACKCOLOR);
	CDC_Line(x,y+1,xx-1,y+1,BLACKCOLOR);
	
	CDC_Line(x,yy,xx,yy,WHITECOLOR);
	CDC_Line(xx,y,xx,yy,WHITECOLOR);

	CDC_Line(x+1,yy-1,xx-1,yy-1,WHITECOLOR);
	CDC_Line(xx-1,y+1,xx-1,yy-1,WHITECOLOR);


}

void	CDC_DrawWin24(INT16S x,INT16S y,INT16S w,INT16S h,char *title)
{
	CRect rect;
	
	CDC_PutColorBar(x,y,w,h,g_nBk,100);
	CDC_PutColorBar(x,y,w,34,g_nBk,100);

	CDC_Rectangle(x,y,w+x,y+34,SILVERCOLOR);
	CDC_Rectangle(x+1,y+1,w+x-1,y+34-1,SILVERCOLOR);
	CDC_Rectangle(x+2,y+2,w+x-1,y+34-2,SILVERCOLOR);
	CDC_Rectangle(x+3,y+3,w+x-1,y+34-3,SILVERCOLOR);
	CDC_Rectangle(x,y,x+w,h+y,SILVERCOLOR);
	CDC_Rectangle(x+1,y+1,x+w-1,h+y-1,SILVERCOLOR);
	CDC_Rectangle(x+2,y+2,x+w-2,h+y-2,SILVERCOLOR);
	CDC_Rectangle(x+3,y+3,x+w-3,h+y-3,SILVERCOLOR);

	rect.left=x;
	rect.right=x+w;
	rect.top=y+2;
	rect.bottom=y+32;
	
	CDC_OutCenText24(rect,title,YELLOWCOLOR,g_nBk);
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

	CDC_OutText24Ex((x-len),y,string,color,bk);
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


//*************************** 截图BMB *************************//

/*typedef __packed struct 				//tagRGBQUADE 
{
	UBYTE   rgbBlue;    				// 蓝色的亮度(值范围为0-255)
	UBYTE   rgbGreen;   				// 绿色的亮度(值范围为0-255)
	UBYTE   rgbRed;     				// 红色的亮度(值范围为0-255)
	UBYTE   rgbReserved;				// 保留，必须为0
} RGBQUADE;



typedef __packed struct 				//tagRGBQUADE 
{
	UBYTE   Blue;    					// 蓝色的亮度(值范围为0-255)
	UBYTE   Green;   					// 绿色的亮度(值范围为0-255)
	UBYTE   Red;     					// 红色的亮度(值范围为0-255)
} RGBDATA;



typedef __packed struct 				//tagBITMAPINFO 
{

	INT16U  bfType; 					//位图文件的类型，必须为BM
	INT32U  bfSize; 					//位图文件的大小，以字节为单位
	INT16U  bfReserved1; 				// 位图文件保留字，必须为0
	INT16U  bfReserved2; 				// 位图文件保留字，必须为0
	INT32U  bfOffBits; 					// 位图数据的起始位置，以相对于位图


	INT32U  biSize;       				// 本结构所占用字节数
	INT32U  biWidth;      				// 位图的宽度，以像素为单位
	INT32U  biHeight;     				// 位图的高度，以像素为单位
	INT16U  biPlanes;     				// 目标设备的级别，必须为1
	INT16U  biBitCount;      			// 每个像素所需的位数，必须是1(双色),
	                                	// 4(16色)，8(256色)或24(真彩色)之一
	INT32U  biCompression;   			// 位图压缩类型，必须是 0(不压缩),
	                               		// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	INT32U  biSizeImage;     			// 位图的大小，以字节为单位
	INT32U  biXPelsPerMeter; 			// 位图水平分辨率，每米像素数
	INT32U  biYPelsPerMeter; 			// 位图垂直分辨率，每米像素数
	INT32U  biClrUsed;       			// 位图实际使用的颜色表中的颜色数
	INT32U  biClrImportant;  			// 位图显示过程中重要的颜色数

	RGBQUADE bmiColors[1];

}BITMAPINFO;


typedef __packed struct					
{

	BITMAPINFO  CreatBmp;
	RGBDATA cdata[800*480];
	
}CBMPDATA;*/

#if FALSE

static CBMPDATA  CBMP;
extern U32 (*GetPixel)(U32,U32);

INT32U GetPixelTem(INT16S x,INT16S y)
{
	INT16U Color;
	INT32U red,green,blue,tmp;
 
	Color = GetPixel(x,y);
	red   =Color&0x001f;
	green =(Color&0x07e0)<<4;
	blue  =(Color&0xf800)<<8;   
	tmp=red|green|blue;    
	return tmp;
}

/*****************************************************************
函数功能:创建位图文件
******************************************************************/
char CreatBMP(void *filename,INT16S Wide,INT16S Hight,INT16S color)
{

	INT16S 		W,H1;
	INT8U 		*temp;
	INT32U 		data;
	FS_FILE 	*pfile;

	{

		Wide=Wide-Wide%4;

		CBMP.CreatBmp.bfType=0x4D42; 				// 位图文件的类型，必须为BM
		CBMP.CreatBmp.bfSize=0x36+Wide*Hight*3; 	// 位图文件的大小，以字节为单位
		CBMP.CreatBmp.bfReserved1=0; 				// 位图文件保留字，必须为0
		CBMP.CreatBmp.bfReserved2=0; 				// 位图文件保留字，必须为0

		CBMP.CreatBmp.bfOffBits=0x36; 				// 位图数据的起始位置，以相对于位图
		CBMP.CreatBmp.biSize=   0x28;       		// 本结构所占用字节数

		CBMP.CreatBmp.biWidth=  Wide;      			// 位图的宽度，以像素为单位
		CBMP.CreatBmp.biHeight= Hight;     			// 位图的高度，以像素为单位
		CBMP.CreatBmp.biPlanes= 1;     				// 目标设备的级别，必须为1
		CBMP.CreatBmp.biBitCount=0x18;      		// 每个像素所需的位数，必须是1(双色),
		                                			// 4(16色)，8(256色)或24(真彩色)之一
		CBMP.CreatBmp.biCompression=0x0;   			// 位图压缩类型，必须是 0(不压缩),
		                                			// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
		CBMP.CreatBmp.biSizeImage=Wide*Hight*3;     	// 位图的大小，以字节为单位
		CBMP.CreatBmp.biXPelsPerMeter=0; 			// 位图水平分辨率，每米像素数
		CBMP.CreatBmp.biYPelsPerMeter=0; 			// 位图垂直分辨率，每米像素数
		CBMP.CreatBmp.biClrUsed=0;       			// 位图实际使用的颜色表中的颜色数
		CBMP.CreatBmp.biClrImportant=0;  			// 位图显示过程中重要的颜色数

		for(H1=0;H1<Hight;H1++)
		{
			for(W=0;W<Wide;W++)
			{

				data = GetPixelTem(W,Hight-H1-1);

				temp = (INT8U *)&data;
				CBMP.cdata[W+H1*Wide].Red = (*temp<<3)+0x7;
				CBMP.cdata[W+H1*Wide].Blue  = *(temp+1)<<1;
				CBMP.cdata[W+H1*Wide].Green = *(temp+2)+0x1;

				if(!color)
				{

					CBMP.cdata[W+H1*Wide].Red  = (CBMP.cdata[W+H1*Wide].Red&0xff)*0x10000+
												(CBMP.cdata[W+H1*Wide].Green&0xff)*0x100+
												(CBMP.cdata[W+H1*Wide].Blue&0xff);

					CBMP.cdata[W+H1*Wide].Blue  = CBMP.cdata[W+H1*Wide].Red;
					CBMP.cdata[W+H1*Wide].Green = CBMP.cdata[W+H1*Wide].Red;

				}

			}
		}

		pfile=FS_FOpen(filename,"w");

		FS_FWrite((INT8U *)&CBMP,1,CBMP.CreatBmp.bfSize,pfile);

	}

	FS_FClose(pfile);
	return 0;
}
#endif

void	CDC_DrawArcEdit(INT16S x, INT16S y, INT16S w, INT16S h,INT32S color)	//绘制带圆弧方框
{
	CDC_Line(x+3,y,x+w-3,y,color);
	CDC_Line(x+w-3,y,x+w,y+3,color);
	CDC_Line(x+w,y+3,x+w,y+h-3,color);
	
	CDC_Line(x+w,y+h-3,x+w-3,y+h,color);
	CDC_Line(x+3,y+h,x+w-3,y+h,color);
	CDC_Line(x+3,y+h,x,y+h-3,color);
	
	CDC_Line(x,y+3,x,y+h-3,color);
	CDC_Line(x,y+3,x+3,y,color);
	
}


void	CDC_TextCen24(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	
	posx = (INT16S)(x+(w-strlen(string)*12)/2.0);
	posy = (INT16S)(y+(h-24)/2.0);
	
	CDC_OutText24Ex(posx,posy,string,color,bk);

//	CDC_OutText_UTF(posx,posy,string,color,bk,MZK_24);

}


void	CDC_TextCen32(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	
	posx = (INT16S)(x+(w-strlen(string)*20)/2.0);
	posy = (INT16S)(y+(h-40)/2.0);
	
	CDC_OutText_UTF(posx,posy,string,color,bk,YH_32);
	
//	CDC_OutText32(posx,posy,string,color,bk);

}


void	CDC_TextSelect24(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	
	switch(style)
	{
		case 0:																	//居中对齐
			posx = (INT16S)(x+(w-strlen(string)*14)/2.0);
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 1:																	//右对齐
			posx = x+w-strlen(string)*14-4;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
			
		case 2:																	//左对齐
			posx = x;
			posy = (INT16S)(y+(h-24)/2.0);
			break;
		
		default:
			break;
	}
	
	CDC_OutText24Ex(posx,posy,string,color,bk);	
}

extern CRect m_CurView;
extern INT16U  g_uHzk16XMS;
extern INT16U  g_uHzk16zXMS;
extern INT16U  g_uHzk24XMS;
extern INT16U  g_uHzk24zXMS;

void CDC_OutText24Ex(INT16S x,INT16S y,char *string,INT32S color,INT32S bk)
{
	INT8U qcode,wcode;
	INT32U record;
	INT8U buffer[72];
	INT16S i,j,k;
	INT16S oldx = x;
	
	x = x+m_CurView.left;
	y = y+m_CurView.top;
	
	if(!CDC_Lock())
		return;
		
	while(* string)
	{
		INT8U ch1,ch2;
		
		ch1 = *string;
		ch2 = *(string+1);
		
		if(((ch1>>7)==1) && ((ch2>>7)==1))										//is chinese
		{ 
			qcode = *string-176;
			wcode = *(string+1)-161;    
			record= (INT32U)(qcode*94+wcode)*72L; 								//fseek(hzk24,record,SEEK_SET);
																				//fread(buffer,72,1,hzk24);
			XMSToMEM((INT8U *)buffer,g_uHzk24XMS,record,72);
			
			for(i=0;i<24;i++)
			{
				for(j=0;j<3;j++)
				{
					for(k=0;k<8;k++)
					{
						if(buffer[i*3+j]>>(7-k)&1)
							CDC_PutPoint(x+i,y+j*8+k,color);
						else if( bk != -1 )
							CDC_PutPoint(x+i,y+j*8+k,bk);
					}
				}
			}
			string+=2;
			x+=24;
			continue;
		}
		else
		{
			INT16S offset;
			char ch;
			
			if(*string=='\n')
			{
				x = oldx;
				y += 30;
				string++;
				continue;
			}
			
			ch = *string;   
			offset = ch-' ';
			offset = offset*48L;
			
			XMSToMEM((INT8U *)buffer,g_uHzk24zXMS,offset,48);
			
			for(i=0;i<24;i++)
			{
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(buffer[i*2+j]>>(7-k)&1)
							CDC_PutPoint(x+j*8+k,y+i,color);
					 	else
					 	{
							if(bk!=-1)
								CDC_PutPoint(x+j*8+k,y+i,bk);
						}
					}
				}
			}
			
			x += 14;
			string ++;
		}
	}// end while
	
	CDC_UnLock();
}

void CDC_OutText_32(INT16S x,INT16S y,char *string,INT32S color,INT32S bk)
{
	CDC_OutText_UTF(x,y-4,string,color,bk,MZK_32);
}


void CDC_OutText_48(INT16S x,INT16S y,char *string,INT32S color,INT32S bk)
{
	CDC_OutText_UTF(x,y-4,string,color,bk,YH_32);
}

void	CDC_TextSelect_32(INT16S x, INT16S y, INT16S w, INT16S h,INT16S style,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	INT16S len =0;
	
	if(strstr(string, ".")==NULL)
	{
		len = strlen(string)*16;
	}
	else 
	{
		len = strlen(string)*16-8;										//"."宽度为8，正常字符为16							
	}
	
	switch(style)
	{
		case 0:																	//居中对齐
			posx = (INT16S)(x+(w-len)/2.0);
			posy = (INT16S)(y+(h-32)/2.0);
			break;
			
		case 1:																	//右对齐
			posx = x+w-len;
			if(string[0]=='N')
				posx-=8;
			else
				posx-=4;
				
			posy = (INT16S)(y+(h-32)/2.0);
			break;
			
		case 2:																	//左对齐
			posx = x;
			posy = (INT16S)(y+(h-32)/2.0);
			break;
		
		default:
			break;
	}
	
	if(string[0]=='N')
		CDC_OutText_UTF(posx-8,posy,string,color,bk,MZK_32);
	else
		CDC_OutText_UTF(posx,posy,string,color,bk,MZK_32);
}


void	CDC_TextCen_32(INT16S x, INT16S y, INT16S w, INT16S h,INT8S *string,INT32S color, INT32S bk)
{
	INT16S posx = 0;
	INT16S posy = 0;
	if(g_bEnglish==0)
		posx = (INT16S)(x+(w-strlen(string)*16)/2.0);
	else
		posx = (INT16S)(x+(w-strlen(string)*12)/2.0);
		
	posy = (INT16S)(y+(h-32)/2.0);
	

	CDC_OutText_UTF(posx,posy,string,color,bk,MZK_32);

}

//-----------------------右对齐输出----------------------//
void	CDC_OutRightText24Ex(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;

	if( string == NULL ) 
		return;

	if(g_iCurEditNo!=3)
	{
		if(strstr(string, ".")==NULL)
		{
			len = strlen(string)*16;
		}
		else 
		{
			len = strlen(string)*16-8;										//"."宽度为8，正常字符为16							
		}
	}
	else
	{
		len = strlen(string)*16;
	}
	
	i = FindEnterCount(string)+1;

	if(g_iCurEditNo!=3)
	{
		if(string[0]=='N')
			CDC_OutText_UTF((rect.right-len)-9,(rect.bottom-rect.top-i*32)/2+rect.top+1,string,color,bk,MZK_32);
		else
			CDC_OutText_UTF((rect.right-len)-5,(rect.bottom-rect.top-i*32)/2+rect.top+1,string,color,bk,MZK_32);
	}
	else
	{	
		CDC_OutText24((rect.right-len)-5,(rect.bottom-rect.top-i*24)/2+rect.top+1,string,color,bk);
	}
}

//-----------------------左对齐输出----------------------//
void	CDC_OutLeftText24Ex(CRect rect,char *string,INT32S color,INT32S bk)
{
	INT16S len,i;

	if( string == NULL ) return;

	len = strlen(string)*16;
	i = FindEnterCount(string)+1;

	if(g_iCurEditNo!=3)
	{
		CDC_OutText_UTF(rect.left+2,(rect.bottom-rect.top-i*32)/2+rect.top+1,string,color,bk,MZK_32);
	}
	else
	{	
		CDC_OutText24(rect.left+2,(rect.bottom-rect.top-i*24)/2+rect.top+1,string,color,bk);
	}
}

//---------------左右对齐输出固定长度字符串--------------//
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


