#ifndef		___PASSWORD_H___
#define		___PASSWORD_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CPassword_Init(INT16S left,INT16S top,INT16S width,INT16S height,
					   INT8S *title,INT8S *message,INT32S tc);
void	CPassword_Destroy(void);
void	CPassword_Create(void);
void	CPassword_OnKey(INT16U key,INT16U tkey);
void	CPassword_UpdateObject(INT16S dk);
INT32S	CPassword_GetPassword(void);

void	CPassword_OnButtonOk(void);
void	CPassword_OnButtonCancel(void);

INT16S 	CPassword_DoModal(void);

#endif