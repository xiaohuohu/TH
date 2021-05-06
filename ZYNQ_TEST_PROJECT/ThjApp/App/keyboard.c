#include	"includes.h"
#include	"base.h"

//extern	CMouse g_mouse;


WORK_EXT	BOOL		g_bHandset;

TEST_EXT  	OS_EVENT 	*MsgQueue;


TEST_EXT	INT16S 		g_iDlgLeft;
TEST_EXT	INT16S		g_iDlgTop;
TEST_EXT	INT16S		g_iDlgWidth;
TEST_EXT	INT16S		g_iDlgHeight;

TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	BOOL		g_bEnglish;

//TEST_EXT	INT32S 		g_nBk;
//TEST_EXT	INT32S 		g_TextColor4; 

INT32S    	m_KeyBackColor = 0x4682B4;
INT32S    	m_iBackColor = 0xEBEBEB;
INT32S    	m_KeyColor = 0xB2DFEE;
INT32S   	m_iCurKey = 0;



INT8S *keyName[43] = {"A","B","C","D","E","F","G","1","2","3",
					"H","I","J","K","L","M","N","4","5","6",
					"O","P","Q","R","S","T","_","7","8","9",
					"U","V","W","X","Y","Z"," ","0","-",".",
					"Backspace","Enter","Esc"}; 
INT8S keyInput[0x30] = "";




void	CKeyBoard_Init(INT16S left,INT16S top,INT16S width,INT16S height)
{
	
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	
	
	/*CMainFrm_Init();
	CMainFrm_SetArraySize( KEYBOARDCOUNT );
	i=0;
	k=0;
	
	

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;
	strcpy(temp.name,"确定");
	temp.x=g_iDlgLeft+g_iDlgWidth/2-70;
	temp.y=g_iDlgTop+g_iDlgHeight-40;
	temp.w=60;temp.h=30;
	temp.key=TSAVE;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;//IDC_CANCEL;
	strcpy(temp.name,"取消");
	temp.x=g_iDlgLeft+g_iDlgWidth/2+10;
	temp.y=g_iDlgTop+g_iDlgHeight-40;
	temp.w=60;temp.h=30;
	temp.key=TCANCEL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);*/
	memset(keyInput,NULL,0x30);	
}


void	CKeyBoard_Destroy()
{
	//CMainFrm_Destroy();
}


void	CKeyBoard_DrawKey(INT16S x1 ,INT16S y1,INT16S x2,INT16S y2,INT32S color )
{
	
	CDC_PutColorBar(x1,y1,x2-x1,y2-y1,color,1);
	
	CDC_Line(x1+1,y1+1,x1+1,y2-1,WHITECOLOR);
	CDC_Line(x1+1,y1+1,x2-1,y1+1,WHITECOLOR);

	CDC_Line(x1+1,y2-1,x2-1,y2-1,LIGHTBLACKCOLOR);
	CDC_Line(x2-1,y1+1,x2-1,y2-1,LIGHTBLACKCOLOR);
}


void	CKeyBoard_DrawPressKey(INT16S x1 ,INT16S y1,INT16S x2,INT16S y2)
{

	CDC_Line(x1+1,y2-1,x1+1,y1+1,LIGHTBLACKCOLOR);
	CDC_Line(x1+1,y1+1,x2-1,y1+1,LIGHTBLACKCOLOR);

	CDC_Line(x1+1,y2-1,x2-1,y2-1,WHITECOLOR);
	CDC_Line(x2-1,y2-1,x2-1,y1+1,WHITECOLOR);

}


void	CKeyBoard_String(INT8S *str)
{
	if( keyInput[0] )
	{
		strcpy(str,keyInput);
		return;
	}
	*str = NULL;
}


void	 CKeyBoard_SetString(INT8S *str)
{
	memset(keyInput,NULL,0x30);
	if( !str[0] ) return;
	strcpy(keyInput,str);
}


void	CKeyBoard_UpdateObject(INT16S dk,BOOL bFrist)
{
	
	INT16S x,y;
	INT16S i,j,key = 0;

	if(m_iCurKey<40)
	{
		y= m_iCurKey/10*36+20+36+g_iDlgTop;
		if(m_iCurKey%10<7)x =g_iDlgLeft+ 24+m_iCurKey%10*36;
		else x = g_iDlgLeft+24+m_iCurKey%10*36+36;
			
		CDC_PutColorBar(x,y,30,30,m_KeyBackColor,1);	
		CKeyBoard_DrawKey(x-4,y-4,x+30,y+30,m_KeyBackColor);				
		CDC_OutText24(x+6,y,keyName[m_iCurKey],WHITECOLOR,m_KeyBackColor);
	}
	else
	{
		y = g_iDlgTop+200;	
		
		if(m_iCurKey==40)
		{
			x = g_iDlgLeft+24;
			
			CDC_PutColorBar(x,y,102,30,m_KeyBackColor,1);	
			CKeyBoard_DrawKey(x-4,y-4,x+102,y+30,m_KeyBackColor);				
			CDC_OutText(x+10,y+5,keyName[m_iCurKey],WHITECOLOR,m_KeyBackColor);
		}		
		else if(m_iCurKey==41)
		{
			x = g_iDlgLeft+24+108;		
			CDC_PutColorBar(x,y,138,30,m_KeyBackColor,1);
			CKeyBoard_DrawKey(x-4,y-4,x+138,y+30,m_KeyBackColor);				
			CDC_OutText(x+30,y+5,keyName[m_iCurKey],WHITECOLOR,m_KeyBackColor);
		}		
		else if(m_iCurKey==42)
		{
			x = g_iDlgLeft+24+36*8;	
			CDC_PutColorBar(x,y,102,30,m_KeyBackColor,1);	
			CKeyBoard_DrawKey(x-4,y-4,x+102,y+30,m_KeyBackColor);				
			CDC_OutText(x+30,y+5,keyName[m_iCurKey],WHITECOLOR,m_KeyBackColor);
		}
	}
	
	if(bFrist)
	{
		CRect	rect;
		rect.left = g_iDlgLeft+24;
		rect.top  = g_iDlgTop+20;
		rect.right = g_iDlgLeft+410;
		rect.bottom  = g_iDlgTop+46;
		CDC_DrawEdit(rect,WHITECOLOR);
		CDC_OutRightText24(g_iDlgLeft+400,g_iDlgTop+22,keyInput,BLACKCOLOR,WHITECOLOR);
		y = g_iDlgTop+20;
		
		for(i = 0 ; i < 4 ;i++)
		{
			y+=36;			
			
			for(j = 0 ; j < 11 ;j++)
			{
						
				x = g_iDlgLeft+24+j*36;		
		
				if(j==7)continue;
			
				CDC_PutColorBar(x,y,30,30,m_KeyBackColor,1);
				//CDC_Rectangle(x-4,y+4,x+30,y+30,BLACKCOLOR);
				if(m_iCurKey == i*10+j)CKeyBoard_DrawPressKey(x-4,y-4,x+30,y+30);
				else CKeyBoard_DrawKey(x-4,y-4,x+30,y+30,m_KeyBackColor);				
				CDC_OutText24(x+6,y,keyName[key++],WHITECOLOR,m_KeyBackColor);
				
			}
		}
		
		x = g_iDlgLeft+24;
		y = g_iDlgTop+200;
		
		CDC_PutColorBar(x,y,102,30,m_KeyBackColor,1);
		if(m_iCurKey == 40)CKeyBoard_DrawPressKey(x-4,y-4,x+102,y+30);
		else CKeyBoard_DrawKey(x-4,y-4,x+102,y+30,m_KeyBackColor);				
		CDC_OutText(x+10,y+5,keyName[key++],WHITECOLOR,m_KeyBackColor);
		
		x = g_iDlgLeft+24+108;
		CDC_PutColorBar(x,y,138,30,m_KeyBackColor,1);
		if(m_iCurKey == 42)CKeyBoard_DrawPressKey(x-4,y-4,x+138,y+30);
		else CKeyBoard_DrawKey(x-4,y-4,x+138,y+30,m_KeyBackColor);				
		CDC_OutText(x+30,y+5,keyName[key++],WHITECOLOR,m_KeyBackColor);
		
		x = g_iDlgLeft+24+36*8;
		CDC_PutColorBar(x,y,102,30,m_KeyBackColor,1);
		if(m_iCurKey == 43)CKeyBoard_DrawPressKey(x-4,y-4,x+102,y+30);
		else CKeyBoard_DrawKey(x-4,y-4,x+102,y+30,m_KeyBackColor);				
		CDC_OutText(x+30,y+5,keyName[key++],WHITECOLOR,m_KeyBackColor);
		
		
	}

	if(labs(dk)>1)
	{
		if(dk>0)
		{
			if(m_iCurKey>=30&&m_iCurKey<=32)
			{
				m_iCurKey = 40;
			}
			else if(m_iCurKey>=33&&m_iCurKey<=36)
			{
				m_iCurKey = 41;
			}
			else if(m_iCurKey>=37&&m_iCurKey<=39)
			{
				m_iCurKey = 42;
			}
			else if(m_iCurKey==40)
			{
				m_iCurKey = 0;
			}
			else if(m_iCurKey==41)
			{
				m_iCurKey = 3;
			}
			else if(m_iCurKey==42)
			{
				m_iCurKey = 7;
			}
			else
			{
				m_iCurKey +=dk;	
			}
		}
		else
		{
			if(m_iCurKey>=0&&m_iCurKey<=2)
			{
				m_iCurKey = 40;
			}
			else if(m_iCurKey>=3&&m_iCurKey<=6)
			{
				m_iCurKey = 41;
			}
			else if(m_iCurKey>=7&&m_iCurKey<=9)
			{
				m_iCurKey = 42;
			}
			else if(m_iCurKey==40)
			{
				m_iCurKey = 30;
			}
			else if(m_iCurKey==41)
			{
				m_iCurKey = 33;
			}
			else if(m_iCurKey==42)
			{
				m_iCurKey = 37;
			}
			else
			{
				m_iCurKey +=dk;	
				m_iCurKey = (m_iCurKey+43)%43;
			}
		}
	}
	else
	{
		m_iCurKey +=dk;		
		m_iCurKey = (m_iCurKey+43)%43;
	}

	if(m_iCurKey<40)
	{
		y= g_iDlgTop+ m_iCurKey/10*36+20+36;
		
		if(m_iCurKey%10<7)
			x = g_iDlgLeft+24+m_iCurKey%10*36;
		else 
			x =g_iDlgLeft+ 24+m_iCurKey%10*36+36;
			
		CDC_PutColorBar(x,y,30,30,m_KeyColor,1);	
		CKeyBoard_DrawPressKey(x-4,y-4,x+30,y+30);				
		CDC_OutText24(x+6,y,keyName[m_iCurKey],LIGHTREDCOLOR,m_KeyColor);
	}
	else
	{
		y = g_iDlgTop+200;	
		
		if(m_iCurKey==40)
		{
			x = g_iDlgLeft+24;
			
			CDC_PutColorBar(x,y,102,30,m_KeyColor,1);	
			CKeyBoard_DrawPressKey(x-4,y-4,x+106,y+30);			
			CDC_OutText(x+10,y+5,keyName[m_iCurKey],LIGHTREDCOLOR,m_KeyColor);
		}		
		else if(m_iCurKey==41)
		{
			x = g_iDlgLeft+24+108;		
			CDC_PutColorBar(x,y,138,30,m_KeyColor,1);	
			CKeyBoard_DrawPressKey(x-4,y-4,x+138,y+30);						
			CDC_OutText(x+30,y+5,keyName[m_iCurKey],LIGHTREDCOLOR,m_KeyColor);
		}		
		else if(m_iCurKey==42)
		{
			x = g_iDlgLeft+24+36*8;		
			CDC_PutColorBar(x,y,102,30,m_KeyColor,1);	
			CKeyBoard_DrawPressKey(x-4,y-4,x+106,y+30);				
			CDC_OutText(x+30,y+5,keyName[m_iCurKey],LIGHTREDCOLOR,m_KeyColor);
		}
	
	}

}


void   CKeyBoard_Create()
{
	
	CRect rect;
	rect.left = g_iDlgLeft;
	rect.top  = g_iDlgTop;
	rect.right = g_iDlgLeft+g_iDlgWidth;
	rect.bottom = g_iDlgTop+g_iDlgHeight;

	CDC_DrawRect(rect,m_iBackColor,BLUECOLOR);

	CKeyBoard_UpdateObject(0,TRUE);
}


void	CKeyBoard_OnButtonOk()
{
	Ok_Exit=TRUE;
	g_iDlgIDOk=1;
}


//取消退出
void	CKeyBoard_OnButtonCancel()
{

   Ok_Exit=TRUE;
   CEdit_SetTextColor(BLACKCOLOR);
   CEdit_SetBackColor(WHITECOLOR);
   g_iDlgIDOk=0;
}


void	CKeyBoard_OnButtonBackspace()
{
	INT8S Input[0x30] = "";
	INT16S i = 0;
	//Uart_Printf("%s\n",keyInput);
	if(keyInput==NULL)return;
	Uart_Printf("len = %d\n",strlen(keyInput));
	
	for(i = 0 ; i <strlen(keyInput)-1;i++)
	{
		Input[i] =  keyInput[i];
	}
	
	memset(keyInput,NULL,0x30);
	strcpy(keyInput,Input);
}


void	CKeyBoard_OnKey(INT16U key,INT16U tkey)
{
	switch(key)
	{
	
	case TUP:		
		CKeyBoard_UpdateObject(-10,FALSE);
		break;
		
	case TLEFT:		
		CKeyBoard_UpdateObject(-1,FALSE);
		break;
		
	case TDOWN:	  
		CKeyBoard_UpdateObject(10,FALSE);
		break;
		
	case TRIGHT:			
		CKeyBoard_UpdateObject(1,FALSE);
		break;
		
	//case TSAVE:
		//CKeyBoard_OnButtonOk();
		//break;
		
	case TBACKSPACE:
		{
			CRect	rect;
			rect.left = g_iDlgLeft+24;
			rect.top  = g_iDlgTop+20;
			rect.right = g_iDlgLeft+410;
			rect.bottom  = g_iDlgTop+46;
			memset(keyInput,NULL,0x30);
			
			CDC_DrawEdit(rect,WHITECOLOR);
			CDC_PutColorBar(g_iDlgLeft+26,g_iDlgTop+22,380,22,WHITECOLOR,1);
			CDC_OutRightText24(g_iDlgLeft+400,g_iDlgTop+22,keyInput,BLACKCOLOR,WHITECOLOR);
		
		}
		break;
		
	case TCANCEL:
		CKeyBoard_OnButtonCancel();
		break;
		
	case TENTER:
		{
			CRect	rect;
		
			if(m_iCurKey<40)
			{
				strcat(keyInput,keyName[m_iCurKey]);
			}
			else if(m_iCurKey==40)
			{
				CKeyBoard_OnButtonBackspace();
			}
			else if(m_iCurKey==41)
			{
				CKeyBoard_OnButtonOk();
			}
			else if(m_iCurKey==42)
			{
				CKeyBoard_OnButtonCancel();
			}				
			
			rect.left = g_iDlgLeft+24;
			rect.top  = g_iDlgTop+20;
			rect.right = g_iDlgLeft+410;
			rect.bottom  = g_iDlgTop+46;
		
			
			CDC_DrawEdit(rect,WHITECOLOR);
			CDC_OutRightText24(g_iDlgLeft+400,g_iDlgTop+22,keyInput,BLACKCOLOR,WHITECOLOR);
			break;
		}
	case '0':case '1':case '2':case '3':case '4':
	case '5':case '6':case '7':case '8':case '9':
		{
			//strcat(keyInput,keyName[key]);	
			CRect	rect;
			if(key== '0')key = '0';
			keyInput[strlen(keyInput)] = key;
			rect.left = g_iDlgLeft+24;
			rect.top  = g_iDlgTop+20;
			rect.right = g_iDlgLeft+410;
			rect.bottom  = g_iDlgTop+46;
		
			CDC_DrawEdit(rect,WHITECOLOR);
			CDC_OutRightText24(g_iDlgLeft+400,g_iDlgTop+22,keyInput,BLACKCOLOR,WHITECOLOR);
		}
		break;


#ifdef	PRINTBMP																//屏幕截图 归零键
	case F5:
		{
			char filename[0x20];
			INT16S i=0;
	//		CBmp_Init();
			strcpy(filename,"keybod");
			
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
	//		CBmp_Destroy();
	//		OSQPost(MsgQueue,"Write Bmp End!");
		}
		break;
#endif
		
	default:break;
	}
	

}


INT16S    CKeyBoard_DoModal()
{
	INT16U key,tkey=0;

	CKeyBoard_Create();

	Ok_Exit=FALSE;
	
	while(TRUE)
	{
		key=KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			CKeyBoard_OnKey(key,tkey);
			if(Ok_Exit)
				break;
		}

		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}



