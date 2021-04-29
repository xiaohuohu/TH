# ifndef	___TEST_FRM_H___
# define	___TEST_FRM_H___

#include	"os_cpu.h"
#include	"rect.h"


void	CTestFrm_Init(void);
void	CTestFrm_Destroy(void);

void	CTestIoFrm_Create(void);

void	CTestFrm_OnKey(INT16U key,INT16U tkey);
void    CTestFrm_DrawInputStatus(INT16S io,INT16S status);
void	CTestFrm_DrawOutputStatus(INT16S i,BOOL status);
void	CTestFrm_UpdateKeyStatus(INT16S num,BOOL status,BOOL ledout);
void	CTestFrm_FindZero(INT16S ch);
INT16S	CTestFrm_DoModal(void);
//20130815
void 	CTestFrm_UpdateInputStatuses(void);						//更新外部输入显示状态
void	CTestFrm_UpdateProbeStatuses(void);						//更新探针显示状态
void	CTestFrm_UpdateTestKnob(void);							//更新外部输入点显示状态
void    CTestFrm_UpdateHandWheelPos(void);						//更新手摇脉冲、手盒脉冲
BOOL	CTestFrm_BackZero(INT16S ch,INT16S dir);
# endif
