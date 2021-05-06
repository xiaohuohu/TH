# include   "includes.h"
# include   "base.h"

extern		INT16S		SVGAmode;
extern  	BASEINI 		g_Ini;
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
*/




INT16S	m_nMaxRow,m_nMaxCol;

INT16S	m_nCurRow,m_nCurCol;	 				/*值范围在(m_nMaxRow,m_nMaxCol)中*/
INT16S	m_nStartRow,m_nStartCol; 				/*配合m_nCurRow,m_nCurCol则值为数据行的大小*/
INT16S	m_nMaxData;				 				/*最大数据行数*/

INT16S	*m_pListWidth;							/*列宽度数据指针(其大小与m_nMaxRow相当*/
INT16S 	m_nListHeight;							/*单元表格的高度*/

INT16S	m_nListX,m_nListY;						/*列表显示的位置*/


void	CList_Init()
{
	m_nMaxRow=m_nMaxCol=m_nCurRow=m_nCurCol=0;
	m_nStartRow=m_nStartCol=m_nMaxData=0;
	m_nListHeight=m_nListX=m_nListY=0;
	m_pListWidth=NULL;
}


void	CList_Destroy()
{
/*
	if(m_pListWidth)
		free(m_pListWidth);
*/
}


void	CList_SetMaxRowCol( INT16S row,INT16S col,INT16S ndata,INT16S nHeight,INT16S *m_pWidth)
{
	m_nMaxRow	= row;
	m_nMaxCol	= col;
	m_pListWidth= m_pWidth;
	m_nListHeight= nHeight;
	m_nMaxData	= ndata;
}


INT16S		CList_Height()
{
	return m_nListHeight;
}


INT16S		CList_MaxCol()
{
	return m_nMaxCol;
}


INT16S		CList_MaxRow()
{
	return m_nMaxRow;
}


INT16S		CList_MaxData()
{
	return m_nMaxData;
}


INT16S		CList_CurCol()
{
	return m_nCurCol;
}


INT16S		CList_CurRow()
{
	return m_nCurRow;
}


INT16S		CList_StartRow()
{
	return m_nStartRow;
}


INT16S		CList_StartCol()
{
	return m_nStartCol;
}


INT16S		CList_DataRow(INT16S nRow)
{
	return m_nStartRow + nRow;
}


INT16S		CList_DataCol(INT16S nCol)
{
	return m_nStartCol + nCol;
}


void	CList_RowAdd(INT16S row)
{
	m_nCurRow+=row;
}


void	CList_ColAdd(INT16S col)
{
	m_nCurCol+=col;
}


void	CList_SetCurRow(INT16S row)
{
	m_nCurRow=row;
}


void	CList_SetCurCol(INT16S col)
{
	m_nCurCol=col;
}


void	CList_SetStartRow(INT16S row)
{
	m_nStartRow=row;
}


void	CList_SetStartCol(INT16S col)
{
	m_nStartCol=col;
}


INT16S	CList_Left()
{
	return m_nListX;
}


INT16S	CList_Top()
{
	return m_nListY;
}


void	CList_MoveTo(INT16S x,INT16S y)
{
	m_nListX = x;
	m_nListY = y;
}


void	CList_GetItemRect( INT16S row,INT16S col, CRect *rect )
{
	INT16S i=0;
	
	
	if(row<0)
		row=0;
		
	rect->left = m_nListX;
	
	for(i=0; i<col; i++)
	{
		rect->left += m_pListWidth[i];
		
		//Uart_Printf("m_pListWidth[%d] = %d\n",i,m_pListWidth[i]);
	}
	rect->right = rect->left + m_pListWidth[i];

	rect->top 	= row * m_nListHeight + m_nListY;
	rect->bottom	= rect->top + m_nListHeight;
}


void	CList_DrawTitle(  INT8S * tilte[] ,INT32S qcolor,INT32S bcolor,INT8S style)
{
	INT16S j=0;
	CRect rect;
	//INT32S xoffset;

	rect.left=m_nListX;
	rect.top=m_nListY - m_nListHeight;
	rect.right=0;
	rect.bottom=m_nListY;

	for(j=0; j<m_nMaxCol; j++)
	{
		rect.right = rect.left + m_pListWidth[j];	
			
		rect.left+=1;
		CDC_PutColorBar2( rect,bcolor,style);
	
		rect.left+=1;
		rect.right-=1;
		CDC_OutCenTextKzk24( rect, tilte[j], qcolor ,bcolor);
		
		
		rect.left-=2;
		rect.right+=1;
		
		CDC_Line(rect.right-0,rect.top,rect.right-0,rect.bottom,g_TableLine);
		//CDC_Line(rect.right-1,rect.top,rect.right-1,rect.bottom,g_TableLine);
		//CDC_Line(rect.right-2,rect.top,rect.right-2,rect.bottom,g_TableLine);
		
		//if(j == (m_nMaxCol-1))
		//	CDC_Line(rect.right,rect.top+1,rect.right,rect.bottom,bcolor);
		
		
		rect.left = rect.right;
	}
	
}


void	CList_SetDataSize( INT16S size )
{
	m_nMaxData = size;
}
