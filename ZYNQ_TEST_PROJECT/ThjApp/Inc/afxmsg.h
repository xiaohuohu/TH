#ifndef		___AFXMSG_H___
#define		___AFXMSG_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CAfxMsgDlg_Init(INT16S left,INT16S top,INT16S width,INT16S height,
						INT8S *message,INT8S *title,INT32S tc);
void	CAfxMsgDlg_Destroy(void);
void	CAfxMsgDlg_Create(void);
void	CAfxMsgDlg_OnKey(INT16U key,INT16U tkey);
//void	CAfxMsgDlg_UpdateObject(INT16S dk);

void	CAfxMsgDlg_OnButtonOk(void);
//void	CAfxMsgDlg_OnButtonCancel();

INT16S 	CAfxMsgDlg_DoModal(void);

#endif