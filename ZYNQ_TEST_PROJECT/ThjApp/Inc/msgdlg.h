#ifndef		___MSGDLG_H___
#define		___MSGDLG_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CMsgDlg_Init(INT16S left,INT16S top,INT16S width,INT16S height,
					 INT8S *message,INT8S *title,INT32S tc,INT8S *button1,
					 INT8S *button2);
void	CMsgDlg_Destroy(void);
void	CMsgDlg_Create(void);
void	CMsgDlg_OnKey(INT16U key,INT16U tkey);
//void	CMsgDlg_UpdateObject(INT16S dk);

void	CMsgDlg_OnButtonOk(void);
void	CMsgDlg_OnButtonCancel(void);

INT16S 	CMsgDlg_DoModal(void);

#endif