# ifndef	___I_LIST_HEADER___
# define	___I_LIST_HEADER___
# include	"rect.h"

void	CList_Init(void);
void	CList_Destroy(void);

void	CList_MoveTo( INT16S x,INT16S y);	/*设定列表显示的位置*/
/* 设定最大列表格数,及装载的数据大小,列表的数据宽度数组*/
void	CList_SetMaxRowCol( INT16S row,INT16S col,INT16S ndata,INT16S nHeight,INT16S *m_pWidth);
void	CList_SetDataSize( INT16S size );
INT16S		CList_MaxData(void)  ;


void	CList_DrawTitle( INT8S * tilte[] ,INT32S qcolor, INT32S bcolor,INT8S style);
void	CList_GetItemRect( INT16S row,INT16S col, CRect *rect );
/*void	CList_Update( INT16S row,INT16S col );
void	CList_OnDraw( );
void	CList_DrawItem( INT16S row,INT16S col ,STATUS status;);
*/
INT16S		CList_MaxCol(void) ;
INT16S		CList_MaxRow(void) ;
INT16S		CList_DataRow( INT16S nRow ) ;
INT16S		CList_DataCol( INT16S nCol ) ;
INT16S		CList_CurCol(void) ;
INT16S		CList_CurRow(void) ;
INT16S		CList_StartRow(void);
INT16S		CList_StartCol(void);
INT16S    	CList_Left(void);
INT16S		CList_Top(void);
INT16S		CList_Height(void);
void	CList_RowAdd(INT16S row);
void	CList_ColAdd(INT16S col);
void	CList_SetCurRow(INT16S row);
void	CList_SetCurCol(INT16S col);
void	CList_SetStartRow(INT16S row);
void	CList_SetStartCol(INT16S col);
# endif
