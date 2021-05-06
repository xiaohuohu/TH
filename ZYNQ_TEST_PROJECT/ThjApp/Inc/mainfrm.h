# ifndef ___MAINFRM_H___
# define ___MAINFRM_H___

/******************************************************
	框架基本函数
	Writen by fzz 12.08.2004
	Ver 0.0
******************************************************/

#include	"os_cpu.h"
#include	"rect.h"
#include	"control.h"

//typedef CControl *		PTRCONTROL;

void	CMainFrm_Init(void);
void	CMainFrm_Destroy(void);
void		CMainFrm_ShowAllControl(void);//显示所有控件
void		CMainFrm_SetArraySize(INT16S size );//每个类必须调的成员函数
						//设定将要装的的控件缓冲区大小
void		CMainFrm_Add(INT16S i,CTROL *pCtrl);//将控件数据结构传送至指定的缓冲区位置
CTROL*		CMainFrm_GetItemID(INT16U nID );		//返回控件的顺序号

//void	CMainFrm_OnKey(INT16S key,INT16S tkey);// 按键处理函数(工控机代码常要重载它)

//void	CMainFrm_LoopCheck();
/*
	 一群Mouse处理函数,
	void	OnMouseMove(UINT nFlags, CPoint point );
	void	OnMouseLeftDown( UINT nFlags, CPoint point );
	void	OnMouseLeftUp( UINT nFlags, CPoint point );
	void	OnMouseRightDown( UINT nFlags, CPoint point );
	void	OnMouseRightUp( UINT nFlags, CPoint point );
	void 	CheckStatus();
*/

//void		CMainFrm_UpdateDrawObject(int dk);

/*
private:
	PTRCONTROL			*m_ptrArrayWnd;
	PTRCONTROL			m_ptrCurrentWnd,m_pMouseEventWnd;
	PTRCONTROL			m_ptrMouseControl;//仅跟踪MOUSE移动时,
	*/

#endif