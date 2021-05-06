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
INT8S		m_bStringIn[MAXCHAR];    //20130828

INT32S		m_bTextColor;
INT32S		m_bBackColor;
INT8S		m_bOnlyRead;
INT16S		m_bMaxLen;
INT32S		m_bTimerSpeed=18;
CTROL		*m_bCtrl;
INT16S		m_iFont;

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
WORK_EXT	INT32S 		g_TextColor11;
*/


extern		BOOL		g_bInsertNum;
extern		CRect		g_EditRect[5];
extern		INT16S		g_iCurEditNo;
extern		INT16S		g_iParameterEdit;
extern		BOOL		g_bClearNum;
extern 		COMMAND		Line_Data[MAXLINE+1];

void	CEdit_Init()
{
	m_bOnlyRead=FALSE;
	m_bTextColor=BLACKCOLOR;//WHITECOLOR;
	m_bBackColor=WHITECOLOR;
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
//	if(is)
	{
		INT8S buffer[0x30];
		INT16S len;
		CRect rect;

		INT16S tempx;//,temp;
		COMMAND *ptr;
		int 	row = CList_CurRow()+CList_StartRow();
		ptr = Line_Data+row;


		if(m_bOnlyRead == TRUE)return;
				
		len = strlen(m_bString);
		
		(m_bNeedPassword)?
			memset(buffer,'*',len):
			strcpy(buffer,m_bString);
		
		buffer[len]=NULL;
			
		tempx=m_bCtrl->x+2;
		rect.left=m_bCtrl->x;
		rect.top=m_bCtrl->y;
		rect.right=rect.left+m_bCtrl->w;
		rect.bottom=rect.top+m_bCtrl->h;



		if(9 == g_iCurEditNo || g_iParameterEdit == 1)
		{
			if(g_bInsertNum)					//有数字输入
			{			
				if(m_bNeedPassword)				//密码星号显示
				{
									
					CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+1,len*13+4,m_bCtrl->h-1,BLACKCOLOR,1);
					
					CDC_OutText_UTF(rect.left+2,rect.top+(rect.bottom-rect.top-24)/2.0,buffer,WHITECOLOR,BLACKCOLOR,MZK_24);
					
					if(g_bClearNum)				//需要退格
					{
						CDC_PutColorBar(rect.left+4+len*13,rect.top+1,12+2+4,m_bCtrl->h-1,WHITECOLOR,1);
					}
					
				}
				else
				{
					CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+1,len*12+4,m_bCtrl->h-1,BLACKCOLOR,1);
					
					CDC_OutText_UTF(rect.left+2,rect.top+(rect.bottom-rect.top-24)/2.0,buffer,WHITECOLOR,BLACKCOLOR,MZK_24);
					
					if(g_bClearNum)				//需要退格
					{
						CDC_PutColorBar(rect.left+4+len*12,rect.top+1,12+2,m_bCtrl->h-1,m_bBackColor,1);
					}
				}
			}
			else								//只是移动编辑框，没有输入数字
			{		
				if(m_bNeedPassword)
				{	
					CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+1,m_bCtrl->w-2,m_bCtrl->h-1,m_bBackColor,1);
					CDC_TextSelect_24(rect.left-2,rect.top,rect.right-rect.left,rect.bottom-rect.top,0,buffer,m_bTextColor,m_bBackColor);
				}
				else
				{	
					CDC_PutColorBar(m_bCtrl->x+2,m_bCtrl->y+1,m_bCtrl->w-2,m_bCtrl->h-1,m_bBackColor,1);	//解决没有显示背景颜色差一个像素没有显示	
					CDC_TextSelect_24(rect.left,rect.top,rect.right-rect.left-2,rect.bottom-rect.top,1,buffer,m_bTextColor,m_bBackColor);						
				}
				
			}
		}
		else
		{		
		
		
			if(g_iCurEditNo <= 3)				//料号、目标产量、探针设定、寸动设置
			{
				CDC_PutColorBar(m_bCtrl->x,m_bCtrl->y,m_bCtrl->w,m_bCtrl->h,m_bBackColor,1);
				//rect.top=rect.top-1;
				CDC_OutCenTextSzk24(rect,buffer,m_bTextColor,m_bBackColor);
				//CDC_OutText_UTF(rect.right,rect.top,buffer,m_bTextColor,m_bBackColor,MZK_24);
				//CDC_OutTextLR_UTF(rect.left+2,rect.top,buffer,g_TextColor,m_bBackColor,MZK_24,1,0);
				
			}
			else
			{
				CDC_PutColorBar(m_bCtrl->x,m_bCtrl->y,m_bCtrl->w-2,m_bCtrl->h-1,m_bBackColor,1);
				CDC_OutCenTextSzk24(rect,buffer,m_bTextColor,m_bBackColor);
			}
		}
		
		
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
	//static INT8U stat=0;
	
	if( m_bFirst )  
	{ 
		CEdit_ClearBuffer();
		m_bFirst = 0;
	}
			 
	if(key == TBACKSPACE)
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


void    CEdit_OnKey(INT16U key,INT16S tkey)
{
	INT16S	insert = (tkey & 0x80) ? TRUE:FALSE;
	if( m_bOnlyRead == TRUE ) return;
	
	g_bClearNum=FALSE;

	switch(key)
	{
		//case TCLEAR:
		//	CEdit_ClearBuffer();
		//	CEdit_UpdateData(TRUE);
			//Uart_Printf("AB\n");
			//break;
			
		case TDEL:
			((tkey&LEFT_SHIFT)||(tkey&RIGHT_SHIFT))?
				CEdit_ClearBuffer():
				CEdit_Delete(&m_bCurrentCross);
			//Uart_Printf("AA\n");
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
			
		case THOME:
			m_bCurrentCross = 0;
			CEdit_UpdateData(TRUE);
			break;
			
		case TEND:
			m_bCurrentCross = strlen(m_bString);
			CEdit_UpdateData(TRUE);
			break;
			
		case TENTER:
			break;
			
		case TBACKSPACE:
		/*	m_bCurrentCross=strlen(m_bString);
			
			if((m_bString[0]<='9' && m_bString[0]>='0') ||  m_bString[0] == '-' ||  m_bString[0] == '+')//出现中文等特殊字符时不能删除
			{ 
				CEdit_BackChar(&m_bCurrentCross);
			}*/
			CEdit_ClearBuffer();
			CEdit_UpdateData(TRUE);
		
			break;

			
		default:
			{
				if(key & 0x100 ) break;
				
				if(  !( (key >= '1' && key <= '9') ||
					 key == '0' || key == '-' ||
					 key == '.' ) ) break;

				if(key<' ' || key > 'z' ) break;
				
				//if(key == '0')  key= '0';
				//else if(key == '-') key= '-';
				//else if(key == '.')   key= '.';
				
				if(m_bNeedBig)
					if(key>='a'&&key<='z')key -= 0x20;
				
				
				if( m_bFirst ) CEdit_ClearBuffer();
				
				
				if(insert)
				{
					CEdit_InsterChar(&m_bCurrentCross,key);
					break;
				}
				
				if( key == '-' && CList_CurCol()!=CList_MaxCol()-1)			//气缸列队'-'不做特殊处理输入
				{
					INT16S len;
					INT16S j = 0;
					
					len=strlen(m_bString);
					
					if(m_bString[0]!='-')
					{

						if(m_bString[0]!='+')
						{
							if(len<m_bMaxLen)
							{
								for( j = len; j>=0; j--)
									m_bString[j+1]=m_bString[j];
							
								m_bString[0]='-';
							}
						}
						else
						 	m_bString[0]='-';
												
					}
					else
					{
						if(len>1)
						{
							for( j = 0; j<len; j++)
								m_bString[j]=m_bString[j+1];
								
							g_bClearNum=TRUE;
											
						}
						else if(len==1)
						{
							m_bString[0]='+';
						}
							
					}
					
					m_bCurrentCross=strlen(m_bString);
					
				}
				else
				{
					INT16S len;
					INT16S j = 0;
					len=strlen(m_bString);
					
					if(m_bString[0]=='+')
					{
						for( j = 0; j<len; j++)
								m_bString[j]=m_bString[j+1];			
					}
					
					m_bCurrentCross=strlen(m_bString);
					CEdit_InsterChar(&m_bCurrentCross,key);
				}
				
				//m_bCurrentCross=strlen(m_bString);
				
				//CEdit_InsterChar(&m_bCurrentCross,key);

				CEdit_UpdateData(TRUE);
			}
			m_bFirst = FALSE;
			break;
	}
	
	//m_bFirst = FALSE;
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
	
	//Uart_Printf("Aa=%s\n",m_bString);
	
	if(*index<1 || *index>MAXCHAR-2 || len == 0)return;

	for(j=*index-1; j<len-1; j++)
		m_bString[j]=m_bString[j+1];
	
	m_bString[j]=NULL;
	
	(*index) --;
	
	//Uart_Printf("Ab=%s\n",m_bString);
	
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
	
	//CEdit_UpdateData(TRUE);
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
	
	//Uart_Printf("n=%f,m_bString=%s\n",n,m_bString);

	ftoa(n,(INT8U *)m_bString,dot,0);
	
	//Uart_Printf("m_bString=%s\n",m_bString);
	
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
	m_bCtrl->y=rect.top+1;
	m_bCtrl->w=rect.right-rect.left;
	m_bCtrl->h=rect.bottom-rect.top-2;
	
	//20130822
	//memset(m_bStringIn,NULL,MAXCHAR);
	memset(m_bString,NULL,MAXCHAR);
	m_bCurrentCross	= 0;
}


void	CEdit_MoveWindowXY(INT16S x,INT16S y)
{
	m_bCtrl->x=x;
	m_bCtrl->y=y;
}

void	CEdit_StringIn(INT8S *str)
{
	if( m_bString[0] ){
		strcpy(str,m_bString);
		return;
	}
	*str = NULL;
}


void  CEdit_GetSize(INT16S *left, INT16S *top, INT16S *right,INT16S *bottom)
{
	*left=m_bCtrl->x;
	*top=m_bCtrl->y;
	*right=*left+m_bCtrl->w;
	*bottom=*top+m_bCtrl->h;
}
