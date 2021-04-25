#ifndef		___KEYBOARD_H___
#define		___KEYBOARD_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CKeyBoard_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CKeyBoard_Destroy(void);
void	CKeyBoard_Create(void);
void	CKeyBoard_OnKey(INT16U key,INT16U tkey);
void	CKeyBoard_UpdateObject(INT16S dk,BOOL bFrist);

void	CKeyBoard_OnButtonOk(void);
void	CKeyBoard_OnButtonCancel(void);
void	CKeyBoard_OnButtonBackspace(void);

INT16S 	CKeyBoard_DoModal(void);
void	CKeyBoard_DrawPressKey(INT16S x1 ,INT16S y1,INT16S x2,INT16S y2);
void	CKeyBoard_DrawKey(INT16S x1 ,INT16S y1,INT16S x2,INT16S y2,INT32S color );

#endif
