#include	"includes.h"
#include	"ledout.h"

TEST_EXT	OS_EVENT	*LedOutQueue;

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
	}
}