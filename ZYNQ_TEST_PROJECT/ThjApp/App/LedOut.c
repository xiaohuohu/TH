#include	"includes.h"
#include	"ledout.h"

TEST_EXT	OS_EVENT	*LedOutQueue;
WORK_EXT	INT16S		g_iErrorNo;
extern  	BASEINI 	g_Ini;
void  LedOut_Task(void *data)
{
	INT8U  err;
	INT8S  *led;
	data = data;
	
	for (;;)
	{
		led=(INT8S *)OSQPend(LedOutQueue,5000,&err);
		
		if(err==OS_NO_ERR)
		{
			LedOut((INT8U)led[0],(INT8U)led[1]);
			OSTimeDly(15);
		}
		else
			OSTimeDly(30);
		if(g_Ini.iWarningOut==1&&g_iErrorNo)WriteBit(O_WARNING,OPEN);
		else WriteBit(O_WARNING,0);
	}
}
