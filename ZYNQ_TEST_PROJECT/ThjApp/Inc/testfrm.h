# ifndef	___TEST_FRM_H___
# define	___TEST_FRM_H___

#include	"os_cpu.h"
#include	"rect.h"


void	CTestFrm_Init(void);
void	CTestFrm_Destroy(void);
void	CTestFrm_Create(void);
void	CTestFrm_OnKey(INT16U key,INT16U tkey);
void    CTestFrm_DrawInputStatus(INT16S io,INT16S status);
void	CTestFrm_DrawOutputStatus(INT16S i,BOOL status);
void	CTestFrm_UpdateKeyStatus(INT16S num,BOOL status,BOOL ledout);
void	CTestFrm_FindZero(INT16S ch);
void	CTestFrm_HandMove(INT16U ch);
INT16S	CTestFrm_DoModal(void);
void    CTestFrm_UpdateAxis(INT16S iAxis);
# endif
