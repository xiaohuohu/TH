# include "includes.h"
#include	"control.h"


/************************************************
	CButton.c
************************************************/
//extern INT16S		g_Color;
CTROL			*m_ptrButton;
extern	INT16S	SVGAmode;

WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;



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
	
	if(SVGAmode==0)
		CDC_DrawButton( rect ,g_nBk);
	else
		CDC_DrawButton( rect ,DARKGRAYCOLOR);
	
	switch(m_ptrButton->algin)
	{
	case LEFT_ALGIN:
		if(SVGAmode==0)
			CDC_OutText24Ex(2,2,m_ptrButton->name,g_TextColor2,g_nBk);		
		else
			CDC_OutText24Ex(2,2,m_ptrButton->name,YELLOWCOLOR,DARKGRAYCOLOR);
		break;
	
	case CENTER_ALGIN:
		if(SVGAmode==0)
			CDC_OutCenText24(rect,m_ptrButton->name,g_TextColor2,g_nBk);
		else
			CDC_OutCenText24(rect,m_ptrButton->name,YELLOWCOLOR,DARKGRAYCOLOR);
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
			CDC_PutColorBar(rect.left+1,rect.top+1,rect.right-rect.left-2,rect.bottom-rect.top-2,g_TextColor2,1);
		else
			CDC_PutColorBar(rect.left+1,rect.top+1,rect.right-rect.left-2,rect.bottom-rect.top-2,REDCOLOR,1);
		
		switch(m_ptrButton->algin)
		{
		case LEFT_ALGIN:
			if(SVGAmode==0)
				CDC_OutText24Ex(2,2,m_ptrButton->name,g_nBk,g_TextColor2);
			else
				CDC_OutText24Ex(2,2,m_ptrButton->name,BLACKCOLOR,REDCOLOR);
			break;
		case CENTER_ALGIN:
			if(SVGAmode==0)
				CDC_OutCenText24(rect,m_ptrButton->name,g_nBk,g_TextColor2);
			else
				CDC_OutCenText24(rect,m_ptrButton->name,BLACKCOLOR,REDCOLOR);
			break;
		}
		break;
		
	case UnFoucs:
		CButton_Create();
		break;
	}
}

