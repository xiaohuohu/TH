#ifndef		___SETDOG_H___
#define		___SETDOG_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CSetDog_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CSetDog_Destroy(void);
void	CSetDog_Create(void);
void	CSetDog_OnKey(INT16U key,INT16U tkey);
void	CSetDog_UpdateObject(INT16S dk,BOOL isEnter);

void	CSetDog_OnButtonOk(void);
void	CSetDog_OnButtonCancel(void);

INT16S 	CSetDog_DoModal(void);

#endif