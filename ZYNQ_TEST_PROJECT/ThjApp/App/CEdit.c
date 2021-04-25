# include   "includes.h"
# include	"control.h"
#include    "StrTool.h"

TEST_EXT	OS_EVENT	*EditSem;

BOOL		m_bFirst;
BOOL		m_bNeedBig;
BOOL		m_bNeedPassword;
BOOL		m_bNeedTimer;
INT16S		m_bCurrentCross;
INT8S		m_bString[MAXCHAR];
INT32S		m_bTextColor;
INT32S		m_bBackColor;
INT8S		m_bOnlyRead;
INT16S		m_bMaxLen;
INT32S		m_bTimerSpeed=18;
CTROL		*m_bCtrl;
INT16S		m_iFont;

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
WORK_EXT	BOOL		g_bHandMove;
extern 		COMMAND		Line_Data[MAXLINE];
extern		BOOL		g_bInsertNum;
extern		INT16S		g_iCurEditNo;
extern		CRect		g_EditRect[3];

void	CEdit_Init()
{
	m_bOnlyRead=FALSE;
	m_bTextColor=WHITECOLOR;//BLACKCOLOR;
	m_bBackColor=BLACKCOLOR;//WHITECOLOR;
	m_bNeedBig=FALSE;
	m_bNeedPassword=FALSE;
	m_bFirst=TRUE;
	m_bMaxLen=10;
	m_bCtrl = NULL;
	m_iFont = FONT24;
	CEdit_ClearBuffer();
	m_bNeedTimer = TRUE;
}


void	CEdit_SetCtrl(CTROL *ptr)
{
	m_bCtrl=ptr;
}


void	CEdit_Destroy()
{
//	if( m_bCtrl )
//		free(m_bCtrl);
}


void	CEdit_SetFont(INT16S font)
{
	m_iFont = font;
}


void	CEdit_UpdateData(BOOL is)
{
	if(is)
	{
		INT8S buffer[0x30];
		static INT8S buf[0x30];
		INT16S len;
		CRect rect;
		CRect rect1;
		INT16S tempx;//,temp;
		
		if(m_bOnlyRead == TRUE)
			return;
			
		memset(buffer,0,0x30);//不能去掉，否则会出现显示乱码20140812

		len = strlen(m_bString);
		
		(m_bNeedPassword)?
			memset(buffer,'*',len):
			strcpy(buffer,m_bString);
		//Uart_Printf("m_bMaxLen=%d\n",m_bMaxLen);
		buffer[len]=NULL;
		tempx=m_bCtrl->x+2;
				
//		CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+2,m_bCtrl->w-3,m_bCtrl->h-3,m_bBackColor,1);

		rect.left=m_bCtrl->x;
		rect.top=m_bCtrl->y;
		rect.right=rect.left+m_bCtrl->w;
		rect.bottom=rect.top+m_bCtrl->h;
		
		switch(g_iCurEditNo)
		{
			case 9:
				//CList_GetItemRect( CList_StartRow()+CList_CurRow(),CList_StartCol()+CList_CurCol(), &rect1);
				
				CList_GetItemRect(CList_CurRow(),CList_StartCol()+CList_CurCol(), &rect1);
				break;
				
			case 0:
				rect1.left = g_EditRect[0].left;
				rect1.top  = g_EditRect[0].top;
				rect1.right= g_EditRect[0].right;
				rect1.bottom = g_EditRect[0].bottom;
				break;
				
			case 1:
				rect1.left = g_EditRect[1].left;
				rect1.top  = g_EditRect[1].top;
				rect1.right= g_EditRect[1].right;
				rect1.bottom = g_EditRect[1].bottom;
				break;
				
			case 2:
				rect1.left = g_EditRect[2].left;
				rect1.top  = g_EditRect[2].top;
				rect1.right= g_EditRect[2].right;
				rect1.bottom = g_EditRect[2].bottom;
				break;
				
			default:break;
		}
		/*
		if(g_bInsertNum)
		{
			CDC_PutColorBar(rect1.left+2,rect1.top+2,len*16+4,rect1.bottom-rect1.top-4,YELLOWCOLOR,1);
			CDC_OutText_UTF(rect.left+3,rect.top+(rect.bottom-rect.top-32)/2.0,buffer,BLACKCOLOR,YELLOWCOLOR,MZK_32);
		}
		else
		{
			if(g_iCurEditNo==3)
			{
				CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+2,m_bCtrl->w-3,m_bCtrl->h-3,m_bBackColor,1);
				CDC_TextSelect24(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,buffer,m_bTextColor,m_bBackColor);
			}
			else
			{
				CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+2,m_bCtrl->w-3,m_bCtrl->h-3,m_bBackColor,1);
				CDC_TextSelect_32(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,DISPSTYLE,buffer,m_bTextColor,m_bBackColor);
			}
		}
		*/
		if(g_bInsertNum)												//有数字输入
		{
			CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+2,len*16+1,m_bCtrl->h-2,YELLOWCOLOR,1);			
			CDC_OutLeftText24Ex(rect,buffer,BLACKCOLOR,YELLOWCOLOR);
					
		}
		else
		{
			CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+2,m_bCtrl->w-3,m_bCtrl->h-2,m_bBackColor,1);	
			CDC_OutRightText24Ex(rect,buffer,m_bTextColor,m_bBackColor);//右对齐
		}
	}
	else
	{
		OSSemPost(EditSem);
		if(g_bHandMove)
			OSTimeDly(1);
		else
			OSTimeDly(15);
	}
}


void	CEdit_SetLen(INT16S len )
{
	if( !m_bCtrl )
		 return;
	m_bMaxLen=len;
}


INT16S  CEdit_GetLen()
{
	return m_bMaxLen;
}


void	CEdit_SetNeedPassword(BOOL is)
{
	if( !m_bCtrl )
		return;
	m_bNeedPassword=is;
}


void CEdit_Fer(INT16S key)
{
	static INT8U stat=0;
	
	if( m_bFirst )  
	{ 
		CEdit_ClearBuffer();
		m_bFirst = 0;
	}
			 
	if(key == TCLEAR)
	{
		CEdit_ClearBuffer();
	} 
	else 
	{
		m_bCurrentCross=strlen(m_bString);
		if(key=='q')key = 'a';
		if(key == '0')  key= '0';
		else if(key == '-') key= '-';
		else if(key == '.')   key= '.';
		
		CEdit_InsterChar(&m_bCurrentCross,key);
	}
	
	CEdit_UpdateData(TRUE);		
}


void    CEdit_OnKey(INT16S key,INT16S tkey)
{
//	INT16S	insert = (tkey & 0x80) ? TRUE:FALSE;

	INT16S	insert = FALSE;
	
	if( m_bOnlyRead == TRUE ) return;
	
	switch(key)
	{
	case TCLEAR:
		{
			if(g_iCurEditNo==9)
			{
				COMMAND *ptr;
				INT32S	index = 0;
				
				index = CList_StartRow() + CList_CurRow();
				
				ptr = Line_Data + index;
								
				if(ptr->value[0][0]=='N')
				{
					CEdit_ClearBuffer();
					
					if(CList_CurCol()==1)
					{
						m_bString[0]='N';
					}
					else if(CList_CurCol()==2)
					{
						strcpy(m_bString,"次数:");
					}
				}
				else
					CEdit_ClearBuffer();
			}
			else
				CEdit_ClearBuffer();
				
			CEdit_UpdateData(TRUE);
		}
		break;
		
	case TBACK:
		CEdit_BackChar(&m_bCurrentCross);
		break;
		
	case TDEL:
		((tkey&LEFT_SHIFT)||(tkey&RIGHT_SHIFT))?
			CEdit_ClearBuffer():
			CEdit_Delete(&m_bCurrentCross);
		CEdit_UpdateData(TRUE);
		break;
		
	case TLEFT:
		m_bCurrentCross -- ;
		if(m_bCurrentCross<0)
			m_bCurrentCross =0;
		CEdit_UpdateData(TRUE);
		break;
		
	case TRIGHT:
		m_bCurrentCross ++ ;
		
		if(m_bCurrentCross>strlen(m_bString))
			m_bCurrentCross = strlen(m_bString);
		CEdit_UpdateData(TRUE);
		break;
				
	case TENTER:
		break;
		
	case TCANCEL:
		break;
		
	default:
		{
			if(key & 0x100 ) 
				break;
				
			if(!((key >= '1' && key <= '9') || key == '0' || key == '-' || key == '.')) 
				break;

			if(key<' ' || key > 'z' ) 
				break;
			
			if(key == '0')  
				key= '0';
				
			else if(key == '.')   
				key= '.';
				
			if(m_bNeedBig)
			{
				if(key>='a' && key<='z')
					key -= 0x20;
			}
			if(insert)
			{
				CEdit_InsterChar(&m_bCurrentCross,key);
				break;
			}			
			if( m_bFirst )
			{
				if(g_iCurEditNo==9)
				{
					COMMAND *ptr;
					INT32S	index = 0;
					
					index = CList_StartRow() + CList_CurRow();
					
					ptr = Line_Data + index;
									
					if(ptr->value[0][0]=='N')
					{
						CEdit_ClearBuffer();
						
						if(CList_CurCol()==1)
						{
							m_bString[0]='N';
						}
						else if(CList_CurCol()==2)
						{
							strcpy(m_bString,"次");
						}
					}
					else
						CEdit_ClearBuffer();
				}
				else
					CEdit_ClearBuffer();
			}
			
			if(key == '-' && strlen(m_bString)==1)					//+、-符号切换
			{
				if(m_bString[0]=='-')
				{
					m_bString[0]='+';
				}
				else if(m_bString[0]=='+')
				{
					m_bString[0]='-';
				}
				else 
				{
					m_bCurrentCross=strlen(m_bString);				//m_bCurrentCross为当前字符串字符数
					CEdit_InsterChar(&m_bCurrentCross,key);
				}
			}		
			else 
			{
				m_bCurrentCross=strlen(m_bString);					//m_bCurrentCross为当前字符串字符数
				CEdit_InsterChar(&m_bCurrentCross,key);
			}

			CEdit_UpdateData(TRUE);
		}
		break;
	}
	m_bFirst = FALSE;
}


void	CEdit_LoopCheck()
{
/*
	static INT16S yes = FALSE;
	INT32S *ptr = ( INT32S *)0x0000046c;
	INT16S t1;
	if(m_bOnlyRead == TRUE)return;
	t1 = ((*ptr)%m_bTimerSpeed==0)?TRUE:FALSE;
	if(t1 != yes )if(t1)CEdit_OnTimer();
	yes = t1;
*/
}


void	CEdit_OnTimer()
{

	static INT16S temp=FALSE;
	
	if( m_bNeedTimer == FALSE )return;
	
	temp = temp?FALSE:TRUE;
	CEdit_UpdateData(temp);

}


void	CEdit_ClearBuffer()
{
	memset(m_bString,NULL,MAXCHAR);
	m_bCurrentCross	= 0;
}


void	CEdit_BackChar(INT16S *index)
{
	INT16S len=strlen(m_bString);
	INT16S j;
	
	if(*index<1 || *index>MAXCHAR-2 || len == 0)return;

	for(j=*index-1; j<len-1; j++)
		m_bString[j]=m_bString[j+1];
	
	m_bString[j]=NULL;
	
	(*index) --;
	CEdit_UpdateData(TRUE);
}


void	CEdit_Delete(INT16S *index)
{
	INT16S len=strlen(m_bString);
	INT16S j;
	
	if(*index<0 || *index>MAXCHAR-2 || len == 0 || *index>len)return;

	for( j=*index; j<len-1; j++)
		m_bString[j]=m_bString[j+1];
		
	m_bString[j]=NULL;
	CEdit_UpdateData(TRUE);
}


void	CEdit_InsterChar(INT16S *index,INT8S ch)
{
	INT16S len=strlen(m_bString);
	INT16S j;
	
	if(len+1>m_bMaxLen)return;
	if(len+1>m_bCtrl->w/8-1)return;
	if(*index>= MAXCHAR - 2 || len >= MAXCHAR -3) return;

	for( j = len; j>*index; j--)
	{
		m_bString[j]=m_bString[j-1];
	}
	
	m_bString[(*index)++]=ch;
		
	m_bString[len+1]=NULL;
	CEdit_UpdateData(TRUE);
}


void	CEdit_Create()
{
	//m_bBackColor = BLACKCOLOR;
	//m_bTextColor = WHITECOLOR;
	//CEdit_UpdateData(FALSE);
}


void	CEdit_DrawActive()
{
	CEdit_UpdateData( FALSE );
}


void	CEdit_ReDrawAll()
{
	CEdit_Create();
}


void	CEdit_SetTextColor(INT32S color)
{
	m_bTextColor	= color;
}


void	CEdit_SetBackColor(INT32S color)
{
	m_bBackColor	= color;
}


/******************* GetValue ang SetValue ***************/
void	 CEdit_SetString(INT8S *str)
{
	CEdit_ClearBuffer();
	if( !str[0] ) return;
	strcpy(m_bString,str);
}


void CEdit_SetIntBt(INT8S *Name)
{

	CEdit_ClearBuffer();
	strcpy(m_bString,Name);
	m_bCurrentCross = strlen(m_bString);
}


void    CEdit_SetInt(INT16S n)
{
	CEdit_ClearBuffer();
	sprintf(m_bString,"%d",n);
	m_bCurrentCross = strlen(m_bString);
}


void CEdit_SetLong(INT32S n)
{
	CEdit_ClearBuffer();

	ltoa(n,(INT8U *)m_bString);
	m_bCurrentCross = strlen(m_bString);
}


void 	CEdit_SetFloat(FP32 n,INT16S dot)
{

	CEdit_ClearBuffer();
	ftoa(n,(INT8U *)m_bString,dot,0);
	m_bCurrentCross = strlen(m_bString);
}


void	CEdit_String(INT8S *str)
{
	if( m_bString[0] )
	{
		strcpy(str,m_bString);
		return;
	}
	
	*str = NULL;
}


INT16S	CEdit_IsEmpty()
{
	return m_bString[0] == NULL;
}


void   CEdit_GetName(INT8S * Name)
{
	if( Name ==NULL )return ;
	
		strcpy(Name,m_bString);
	
}


INT16S CEdit_GetInt()
{
	INT32S tmp;

	tmp=stringtol((INT8U *)m_bString);

	return (INT16S)tmp;
}


INT32S CEdit_GetLong()
{
	return stringtol((INT8U *)m_bString);
}


FP32 CEdit_GetFloat()
{
	return (FP32)(stringtof((INT8U *)m_bString));
	//return atof((INT8U *)m_bString);
}


void	CEdit_SetOnlyRead(BOOL style)
{	
	m_bOnlyRead = style;
}


void	CEdit_SetFirst(BOOL bFirst)
{
	m_bFirst = bFirst;
}


void	CEdit_MoveWindowRect(CRect rect)
{
	m_bCtrl->x=rect.left;
	m_bCtrl->y=rect.top;
	m_bCtrl->w=rect.right-rect.left;
	m_bCtrl->h=rect.bottom-rect.top;
}


void	CEdit_MoveWindowXY(INT16S x,INT16S y)
{
	m_bCtrl->x=x;
	m_bCtrl->y=y;
}

void  CEdit_GetSize(INT16S *left, INT16S *top, INT16S *right,INT16S *bottom)
{
	*left=m_bCtrl->x;
	*top=m_bCtrl->y;
	*right=*left+m_bCtrl->w;
	*bottom=*top+m_bCtrl->h;
}




















