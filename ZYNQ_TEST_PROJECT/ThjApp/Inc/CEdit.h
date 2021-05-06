# ifndef	___EDIT_CONTROL_H___
# define	___EDIT_CONTROL_H___

# include	"os_cpu.h"
# include	"cbutton.h"
# include	"rect.h"
# ifndef	MAXCHAR
# define	MAXCHAR			20
# endif

#define  FONT16 0
#define  FONT24 1

void	CEdit_Init(void);
void	CEdit_Destroy(void);
void	CEdit_SetLen(INT16S len);
void	CEdit_SetNeedPassword(BOOL is);
INT16S  CEdit_GetLen(void);
void	CEdit_SetCtrl(CTROL *ptr);

void 	CEdit_Create(void);
void	CEdit_ReDrawAll(void);
void    CEdit_OnKey(INT16U key,INT16S tkey);
void	CEdit_DrawActive(void);
void	CEdit_UpdateData(BOOL is);
void	CEdit_LoopCheck(void);
void	CEdit_OnTimer(void);

void	CEdit_SetFont(INT16S font);

void	CEdit_SetTextColor(INT32S color);
void	CEdit_SetBackColor(INT32S color);
void	CEdit_SetOnlyRead(BOOL style);
INT16S	CEdit_IsEmpty(void) ;
void	CEdit_SetFirst(BOOL bFirst);
void	CEdit_String(INT8S *str);
INT16S 	CEdit_GetInt(void) ;
INT32S 	CEdit_GetLong(void);
FP32 	CEdit_GetFloat(void);
void 	CEdit_SetString( INT8S *str);
void 	CEdit_SetInt( INT16S n);
void 	CEdit_SetLong( INT32S n);
void 	CEdit_SetFloat( FP32 n, INT16S dot);

void 	CEdit_ClearBuffer(void);

void	CEdit_Delete( INT16S *index);
void	CEdit_BackChar(INT16S *index);
void	CEdit_InsterChar(INT16S *index, INT8S ch);

void	CEdit_MoveWindowRect(CRect rect);
void	CEdit_MoveWindowXY(INT16S x,INT16S y);

void	CEdit_StringIn(INT8S *str);
void  CEdit_GetSize(INT16S *left, INT16S *top, INT16S *right,INT16S *bottom);
# endif
