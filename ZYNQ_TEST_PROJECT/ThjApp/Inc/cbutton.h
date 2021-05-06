# ifndef	___CONTROL_BUTTON_H___
# define	___CONTROL_BUTTON_H___

#include	"os_cpu.h"
#include	"control.h"

void	CButton_Init(void);
void	CButton_Destroy(void);

void 	CButton_Create(void);
void	CButton_SetCtrl(CTROL *ptr);
void	CButton_DrawItem( DRAWSTATUS pstatus);
/*
void	OnMouseMove(UINT nFlags, CPoint point );
void	OnMouseLeftDown( UINT nFlags, CPoint point );
void	OnMouseLeftUp( UINT nFlags, CPoint point );
*/
# endif
