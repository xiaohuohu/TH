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
void	CTestFrm_HandMove(INT16S ch);
INT16S	CTestFrm_DoModal(void);
//20130815
void 	CTestFrm_UpdateInputStatuses(void);						//�����ⲿ������ʾ״̬
void	CTestFrm_UpdateProbeStatuses(void);						//����̽����ʾ״̬
void	CTestFrm_UpdateTestKnob(void);							//�����ⲿ�������ʾ״̬
void	CTestFrm_HandWheel(INT16S ch);							//�����ҡ�����з���ֹͣ����
void	CTestFrm_UpdateAxisPos(INT16S ch);						//���µ����ֶ��߼�λ�á�ʵ��λ�á��ͺ�����
void    CTestFrm_UpdateHandWheelPos(void);						//������ҡ���塢�ֺ�����
void 	CTestFrm_ClearComActPos(INT16S ch);						//�����߼�λ�á�ʵ��λ��
BOOL	CTestFrm_BackZero(INT16S ch,INT16S dir);
# endif
