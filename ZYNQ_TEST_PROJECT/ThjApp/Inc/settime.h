#ifndef		___SETTIME_H___
#define		___SETTIME_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CSetTime_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CSetTime_Destroy(void);
void	CSetTime_Create(void);
void	CSetTime_OnKey(INT16U key,INT16U tkey);
void	CSetTime_UpdateObject(INT16S dk,BOOL isEnter);

void	CSetTime_OnButtonSetDate(void);
void	CSetTime_OnButtonSetTime(void);
void	CSetTime_OnButtonExit(void);

INT16S 	CSetTime_DoModal(void);

#endif