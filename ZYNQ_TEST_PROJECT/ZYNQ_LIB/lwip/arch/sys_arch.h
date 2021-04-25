#ifndef __ARCH_H__
#define	__ARCH_H__

#include "arch/cc.h"
#include "ucos_ii.h"
#include "arch.h"

//#define 	LWIP_TASK_MAX    		8		//LWIP所占任务数(不能随便更改)
//#define   	T_LWIP_STKSIZE      	1024	//LWIP任务栈大小

extern		void Uart_Printf(char *fmt,...);
#define		LWIP_OUT	Uart_Printf			//调试打印函数


//typedef OS_EVENT 		sys_mutex_t;
//typedef OS_EVENT        sys_sem_t;
//typedef OS_EVENT        sys_mbox_t;
typedef u32_t           sys_thread_t;



#define SYS_MBOX_NULL   (void*)0
#define SYS_SEM_NULL    (void*)0
#define MAX_QUEUES        64//number of mboxs
#define MAX_QUEUE_ENTRIES 32

typedef struct {
	void*       pMem;		//预留4Byte保存MEM中addr地址
    OS_EVENT*   pQ;
    void*       pvQEntries[MAX_QUEUE_ENTRIES];
} TQ_DESCR, *PQ_DESCR;

typedef OS_EVENT* sys_mutex_t;
typedef OS_EVENT* sys_sem_t;
typedef PQ_DESCR sys_mbox_t;

#endif
