#ifndef		___SYSPARA_H___
#define		___SYSPARA_H___

#include	"os_cpu.h"
#include	"rect.h"

#define		PARAINT				1								//代表int型						
#define		PARALONG			2								//代表long型
#define		PARAFLOAT			3								//代表float型
#define		PARA8U				4								//代表INT8U


void	CSysPara_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void	CSysPara_Destroy(void);
void	CSysPara_Create(void);
void	CSysPara_OnKey(INT16U key,INT16U tkey);
void	CSysPara_UpdateObject(INT16S dk,BOOL IsEnter);

void	CSysPara_OnButtonOk(void);
void	CSysPara_OnButtonCancel(void);

INT16S 	CSysPara_DoModal(void);

INT32S 	CSysPara_PutPara(INT8S *ParaName,INT8S *ParaInfo,void* SysPara,INT8U Type ,INT8U Dot,INT8U DataLong,INT8U uPWD);
INT32S  CSysPara_UpdateView(INT16S iStartLine);
void	CSysPara_Message(INT8S  *ParaInfo);
void 	CSysPara_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus);

#endif