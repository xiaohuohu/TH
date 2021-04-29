# ifndef	___FILEFRM_H___
# define	___FILEFRM_H___



#include	"def.h"
#include	"rect.h"

typedef struct
{
  	INT8S	Note[0x20];
  	INT8S 	ModifyTime[0x20];
}FILENOTE;





INT16S    	CFileFrm_Init(INT16S left,INT16S top,INT16S width,INT16S height);
void 		CFileFrm_Create(void);
INT16S    	CFileFrm_DoModal(void);
void		CFileFrm_Destroy(void);

void 		CFileFrm_UpdateView(INT16S iStartLine,BOOL bFreshFile);
void		CFileFrm_UpdateObject(INT16S dk,BOOL IsEnter);
void        CFileFrm_OnKey(INT16U key,INT16U tkey);

void 		CFileFrm_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus);
void 		CFileFrm_Message(INT8S *ParaInfo);
void		CFileFrm_UpdateNowFile(void);


void 		CFileFrm_OnButtonCancel(void);
void 		CFileFrm_OnButtonOk(void);

void 		CFileFrm_OnButtonPageDown(void);
void 		CFileFrm_OnButtonPageUp(void);
void 		CFileFrm_OnButtonCopy(void);
void 		CFileFrm_OnButtonDel(void);
void 		CFileFrm_OnButtonLoad(void);


void		WriteFileNote(void);
BOOL		ReadFileNote(void);

#endif