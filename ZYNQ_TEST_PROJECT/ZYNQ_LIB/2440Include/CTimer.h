#ifndef	_CTIMER_H__
#define _CTIMER_H__

#include "os_cpu.h"

//��ʼ�����ж�ʱ��
void	CTimer_Init(void);
//��ָ����ʱ�� 0<=no<=99
void	CTimer_Open(INT16S no);
//�ر�ָ����ʱ�� 0<=no<=99
void	CTimer_Close(INT16S no);
//�趨��ʱ����ʱʱ�� ��ʱʱ��t<4294967296  0<=��ʱ����no<=99,��ʱ��δ�򿪷���FALSE
BOOL	CTimer_SetTimes(INT32U t,INT16S no);
//��ʱ����ʼ��ʱ,��ʱ��û�д�ʱ����FALSE
BOOL	CTimer_Begin(INT16S no);
//�ж϶�ʱ���Ƿ�ʱ����ʱ����TRUE��û�г�ʱ��ʱ��û�д򿪷���FALSE
BOOL	CTimer_IsTimeOut(INT16S no);

#endif