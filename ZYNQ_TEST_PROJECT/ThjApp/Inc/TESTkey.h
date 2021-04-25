# ifndef	___TEST_KEY_H___
# define	___TEST_KEY_H___

#include	"os_cpu.h"
#include	"rect.h"


void	CTestKey_Init(void);
void	CTestKey_Destroy(void);
void	CTestKey_Create(void);
void	CTestKey_OnKey(INT16U key,INT16U tkey);
void	CTestKey_UpdateKeyStatus(INT16S num,BOOL status,BOOL ledout);

INT16S	CTestKey_DoModal(void);

# endif
