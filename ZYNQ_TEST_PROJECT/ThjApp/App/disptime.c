#include	"includes.h"

extern 	BOOL		g_bSetTime;
extern 	INT16S		SVGAmode;
extern	INT16S 		g_iDlgLeft;
extern	INT16S		g_iDlgTop;
extern  BOOL 		g_bScreen;
extern  BOOL		g_bFirstDispTime;
extern	BOOL		g_bScrProtectRun;											//指示是否处于屏保运行状态,采用关闭显示器模式时使用
extern	INT16U		g_uiSuspendTask;											//suspend 0~5 task,high byte rev
extern	INT32S 		g_TextColor1;
extern	INT32S 		g_TextColor2;
extern	INT32S 		g_TextColor3;
extern	INT32S 		g_TextColor4;
extern	INT32S 		g_TextColor5;
extern	INT32S 		g_TableLine;
extern	INT32S 		g_nBk;
extern	INT32S		g_nTechfrmBk;
extern	INT32S 		g_nTitleBk;
extern	INT32S 		g_nTableBk;
extern	INT32S 		g_Color1;
extern	BOOL		g_bInterface;
extern	INT32S 		g_nBackColor;


void  Display_Time_Task(void *data)
{
	char  	ti[10],da[10];
	char 	oldti[10],oldda[10];
	INT16S 	i=0;
	
	data = data;
	
	for (;;)
	{
		if(g_bInterface)
		{
			if(g_bSetTime)
			{
				RtcGetTimeStr(ti);
				CDC_OutText24Ex(668,397,ti,WHITECOLOR,g_nBackColor);
			}
			else
			{
				if(!g_bScrProtectRun)
				{
					if(g_bFirstDispTime)
					{
						RtcGetTimeStr(ti);
						CDC_OutText24Ex(668,397,ti,WHITECOLOR,g_nBackColor);
						g_bFirstDispTime = FALSE;
						
						for(i=0; i<10; i++)
						{
							oldti[i] = -1;
							oldda[i] = -1;
	                    }
					}
					else
					{
						RtcGetTimeStr(ti);
						
						for(i=0; i<strlen(ti); i++)
						{
							if(oldti[i]!=ti[i])
							{													//减少显示字符的数量,减少刷新时间同时可以防止抖动现象的产生
								CDC_AsciiChar24(668+i*14,397,ti[i],WHITECOLOR,g_nBackColor);
								oldti[i] = ti[i];
							}
						}
					}

				}	
			} 
		}
		else
		{
			if(g_bSetTime)
			{
//				RtcGetDateStr(da);
//				CDC_OutText24Ex(g_iDlgLeft+80,g_iDlgTop+45,da,g_TextColor2,g_nTechfrmBk);
				RtcGetTimeStr(ti);
				CDC_OutText24Ex(668,447,ti,g_TextColor2,g_nTechfrmBk);
			}
			else
			{
				if(!g_bScrProtectRun)
				{
					if(g_bFirstDispTime)
					{
//						RtcGetDateStr(da);
//						CDC_OutText24Ex(656,11,da+2,WHITECOLOR,g_nTechfrmBk);
						RtcGetTimeStr(ti);
						CDC_OutText24Ex(668,447,ti,WHITECOLOR,g_nTechfrmBk);
						g_bFirstDispTime = FALSE;
						
						for(i=0; i<10; i++)
						{
							oldti[i] = -1;
							oldda[i] = -1;
	                    }
					}
					else
					{
						RtcGetTimeStr(ti);
						
						for(i=0; i<strlen(ti); i++)
						{
							if(oldti[i]!=ti[i])
							{														//减少显示字符的数量,减少刷新时间同时可以防止抖动现象的产生
								CDC_AsciiChar24(668+i*14,447,ti[i],WHITECOLOR,g_nTechfrmBk);
								oldti[i] = ti[i];
							}
						}
						
/*						RtcGetDateStr(da);
						
						for(i=2; i<strlen(da); i++)
						{
							if(oldda[i] != da[i])
							{
								CDC_AsciiChar24(668+(i-2)*16,11,da[i],WHITECOLOR,g_nTechfrmBk);
								oldda[i]=da[i];
							}
						}
*/
					}

				}	
			} 
		}
		
		OSTimeDlyHMSM(0, 0, 0, 200); 
		
		if((g_uiSuspendTask&0x0080) != 0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}