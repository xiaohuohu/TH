#include "includes.h"

//INT32U	g_ulTimerMax1=4294967296;
INT32U	g_ulTimerMax1=4294967295u; 

INT32U 	g_ulTimerValue1=0;
INT32U  g_ulCount1=0;
INT32U	g_ulStartTime1=0;

void	CTimer_SetTimes_1(INT32U t)
{
	g_ulTimerValue1=t;
}

void	CTimer_Begin_1()
{
	g_ulCount1=0;
	g_ulStartTime1=OSTimeGet();
}

BOOL	CTimer_IsTimeOut_1()
{
	INT32U tmp;
	
	tmp=OSTimeGet();
	
	if(tmp<g_ulStartTime1)
	{		
		g_ulCount1=g_ulTimerMax1-g_ulStartTime1+tmp;
	}
	else
		g_ulCount1=tmp-g_ulStartTime1;
		
	if(g_ulCount1>=g_ulTimerValue1)
		return TRUE;
	else
		return FALSE;
}
