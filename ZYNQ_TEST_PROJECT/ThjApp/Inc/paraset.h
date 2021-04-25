#ifndef		___PARASET_H___
#define		___PARASET_H___

#include	"os_cpu.h"
#include	"rect.h"

void	CParaSet_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CParaSet_Destroy(void);
void	CParaSet_Create(void);
void	CParaSet_OnKey(INT16U key,INT16U tkey);
void	CParaSet_UpdateObject(INT16S dk,BOOL IsEnter);

void	CParaSet_OnButtonOk(void);
void	CParaSet_OnButtonCancel(void);

INT16S 	CParaSet_DoModal(void);
void 	CParaSet_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus);
void	CParaSet_Message(INT8S  *ParaInfo);
INT32S 	CParaSet_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* SysPara,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD);
INT32S  CParaSet_UpdateView(INT16S iStartLine);

#endif