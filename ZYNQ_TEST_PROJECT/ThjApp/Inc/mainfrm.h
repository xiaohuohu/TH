# ifndef ___MAINFRM_H___
# define ___MAINFRM_H___

/******************************************************
	��ܻ�������
	Writen by fzz 12.08.2004
	Ver 0.0
******************************************************/

#include	"os_cpu.h"
#include	"rect.h"
#include	"control.h"

//typedef CControl *		PTRCONTROL;

void	CMainFrm_Init(void);
void	CMainFrm_Destroy(void);
void		CMainFrm_ShowAllControl(void);//��ʾ���пؼ�
void		CMainFrm_SetArraySize(INT16S size );//ÿ���������ĳ�Ա����
						//�趨��Ҫװ�ĵĿؼ���������С
void		CMainFrm_Add(INT16S i,CTROL *pCtrl);//���ؼ����ݽṹ������ָ���Ļ�����λ��
CTROL*		CMainFrm_GetItemID(INT16U nID );		//���ؿؼ���˳���

//void	CMainFrm_OnKey(INT16S key,INT16S tkey);// ����������(���ػ����볣Ҫ������)

//void	CMainFrm_LoopCheck();
/*
	 һȺMouse������,
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
	PTRCONTROL			m_ptrMouseControl;//������MOUSE�ƶ�ʱ,
	*/

#endif