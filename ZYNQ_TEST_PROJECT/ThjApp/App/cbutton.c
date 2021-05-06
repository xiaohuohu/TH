# include "includes.h"
#include	"control.h"


/************************************************
	CButton.c
************************************************/
//extern INT16S		g_Color;
CTROL			*m_ptrButton;
extern	INT16S	SVGAmode;


void	CButton_Init()
{
	m_ptrButton=NULL;
}


void	CButton_Destroy()
{
}


void	CButton_SetCtrl(CTROL *ptr)
{
	m_ptrButton=ptr;
}


void	CButton_Create()
{
	CRect rect;
	
	rect.left=m_ptrButton->x;
	rect.top=m_ptrButton->y;
	rect.right=m_ptrButton->x+m_ptrButton->w;
	rect.bottom=m_ptrButton->y+m_ptrButton->h;	
	CDC_DrawButton( rect ,DARKGRAYCOLOR);
	
	switch(m_ptrButton->algin)
	{
	case LEFT_ALGIN:
		CDC_OutText_UTF(2,2,m_ptrButton->name,g_TextColor2,DARKGRAYCOLOR,MZK_24);
		break;
	
	case CENTER_ALGIN:		
		CDC_OutCenTextKzk24(rect,m_ptrButton->name,g_TextColor2,DARKGRAYCOLOR);
		break;
	}
}


void	CButton_DrawItem( DRAWSTATUS pstatus)
{
	CRect rect;
	
	rect.left=m_ptrButton->x;
	rect.top=m_ptrButton->y;
	rect.right=m_ptrButton->x+m_ptrButton->w;
	rect.bottom=m_ptrButton->y+m_ptrButton->h;
	
	switch( pstatus)
	{
	case Normal:
	case Draw:
		CButton_Create();
		break;
	
	case Active:
		CDC_PressButton(rect);
		break;
	
	case Reset:
		CDC_RlsButton(rect);
		break;
	
	case Foucs:
		if(SVGAmode==0)
			CDC_PutColorBar(rect.left+1,rect.top+1,rect.right-rect.left-2,rect.bottom-rect.top-2,g_TextColor6,1);
		else
			CDC_PutColorBar(rect.left+1,rect.top+1,rect.right-rect.left-2,rect.bottom-rect.top-2,REDCOLOR,1);
		
		switch(m_ptrButton->algin)
		{
		case LEFT_ALGIN:
			if(SVGAmode==0)
				CDC_OutText_UTF(2,2,m_ptrButton->name,g_nBk,g_TextColor6,MZK_24);
			else
				CDC_OutText_UTF(2,2,m_ptrButton->name,BLACKCOLOR,REDCOLOR,MZK_24);
			break;
		case CENTER_ALGIN:
			if(SVGAmode==0)
				CDC_OutCenTextKzk24(rect,m_ptrButton->name,g_nBk,g_TextColor6);
			else
				CDC_OutCenTextKzk24(rect,m_ptrButton->name,BLACKCOLOR,REDCOLOR);
			break;
		}
		break;
		
	case UnFoucs:
		CButton_Create();
		break;
	}
}

