#ifndef		___NETSET_H___
#define		___NETSET_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CNetSet_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CNetSet_Destroy(void);
void	CNetSet_Create(void);
void	CNetSet_OnKey(INT16U key,INT16U tkey);
void	CNetSet_UpdateObject(INT16S dk);

void	CNetSet_OnButtonOk(void);
void	CNetSet_OnButtonCancel(void);

INT16S 	CNetSet_DoModal(void);

#endif