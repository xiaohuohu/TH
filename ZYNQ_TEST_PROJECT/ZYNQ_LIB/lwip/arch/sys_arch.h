#ifndef __ARCH_H__
#define	__ARCH_H__

#include "arch/cc.h"
#include "ucos_ii.h"
#include "arch.h"

//#define 	LWIP_TASK_MAX    		8		//LWIP��ռ������(����������)
//#define   	T_LWIP_STKSIZE      	1024	//LWIP����ջ��С

extern		void Uart_Printf(char *fmt,...);
#define		LWIP_OUT	Uart_Printf			//���Դ�ӡ����


//typedef OS_EVENT 		sys_mutex_t;
//typedef OS_EVENT        sys_sem_t;
//typedef OS_EVENT        sys_mbox_t;
typedef u32_t           sys_thread_t;



#define SYS_MBOX_NULL   (void*)0
#define SYS_SEM_NULL    (void*)0
#define MAX_QUEUES        64//number of mboxs
#define MAX_QUEUE_ENTRIES 32

typedef struct {
	void*       pMem;		//Ԥ��4Byte����MEM��addr��ַ
    OS_EVENT*   pQ;
    void*       pvQEntries[MAX_QUEUE_ENTRIES];
} TQ_DESCR, *PQ_DESCR;

typedef OS_EVENT* sys_mutex_t;
typedef OS_EVENT* sys_sem_t;
typedef PQ_DESCR sys_mbox_t;

#endif
