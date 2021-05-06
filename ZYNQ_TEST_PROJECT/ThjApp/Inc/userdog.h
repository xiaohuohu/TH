#ifndef		___USERDOG_H___
#define		___USERDOG_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CUserDog_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CUserDog_Destroy(void);
void	CUserDog_Create(void);
void	CUserDog_OnKey(INT16U key,INT16U tkey);
void    CUserDog_UpdateObject(INT16S dk,BOOL isEnter);
void	CUserDog_OnButtonOk(void);
void	CUserDog_OnButtonCancel(void);

INT16S 	CUserDog_DoModal(void);

#endif