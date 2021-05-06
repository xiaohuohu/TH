#ifndef	_CTIMER_H__
#define _CTIMER_H__

#include "os_cpu.h"

//初始化所有定时器
void	CTimer_Init(void);
//打开指定定时器 0<=no<=99
void	CTimer_Open(INT16S no);
//关闭指定定时器 0<=no<=99
void	CTimer_Close(INT16S no);
//设定定时器定时时间 定时时间t<4294967296  0<=定时器号no<=99,定时器未打开返回FALSE
BOOL	CTimer_SetTimes(INT32U t,INT16S no);
//定时器开始计时,定时器没有打开时返回FALSE
BOOL	CTimer_Begin(INT16S no);
//判断定时器是否超时，超时返回TRUE，没有超时或定时器没有打开返回FALSE
BOOL	CTimer_IsTimeOut(INT16S no);

#endif