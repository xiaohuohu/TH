#include	"includes.h"

extern 	BOOL		g_bSetTime;
extern 	INT16S		SVGAmode;
extern	INT16S 		g_iDlgLeft;
extern	INT16S		g_iDlgTop;
extern  BOOL 		g_bScreen;
extern  BOOL		g_bFirstDispTime;
extern	BOOL		g_bScrProtectRun;											//指示是否处于屏保运行状态,采用关闭显示器模式时使用
extern	INT16U		g_uiSuspendTask;											//suspend 0~5 task,high byte rev

/*
extern	INT32S 		g_TextColor1;
extern	INT32S 		g_TextColor2;
extern	INT32S 		g_TextColor3;
extern	INT32S 		g_TextColor4;
extern	INT32S 		g_TextColor5;
extern	INT32S 		g_TextColor7;
extern	INT32S		g_TextColor11;
extern	INT32S 		g_TableLine;
extern	INT32S 		g_nBk;
extern	INT32S		g_nTechfrmBk;
extern	INT32S 		g_nTitleBk;
extern	INT32S 		g_nTableBk;
extern	INT32S 		g_Color1;
*/

void  Display_Time_Task(void *data)
{
	char  	ti[10],da[10];
	char 	oldti[10],oldda[10];
	INT16S 	i=0;
	
	data = data;
	
	for (;;)
	{
		if(g_bSetTime)
		{
			RtcGetDateStr(da);
			CDC_OutText24(g_iDlgLeft+60,g_iDlgTop+45,da,g_TextColor2,g_TextColor7);
			RtcGetTimeStr(ti);
			CDC_OutText24(g_iDlgLeft+250,g_iDlgTop+45,ti,g_TextColor2,g_TextColor7);
		}
		else
		{
			if(!g_bScrProtectRun)
			{
				if(g_bFirstDispTime)
				{
					RtcGetDateStr(da);
					//CDC_OutText_UTF(680,40,da,g_TextColor11,g_nBk,MZK_20);
					RtcGetTimeStr(ti);
					//CDC_OutText_UTF(680,62,ti,g_TextColor11,g_nBk,MZK_20);
					g_bFirstDispTime = FALSE;
					
					for(i=0; i<10; i++)
					{
						oldti[i] = '\0';
						oldda[i] = '\0';
                    }
				}
				else
				{
					RtcGetTimeStr(ti);
					
					for(i=0; i<strlen(ti); i++)
					{	
						if(i==2 || i==5 )
						{
							if(oldti[i]!=ti[i])
							{	
								//减少显示字符的数量,减少刷新时间同时可以防止抖动现象的产生
								if(i==2)
								CDC_AsciiCharMzk_24(710,67,ti[i],g_TextColor11,g_nBk);
								else
								CDC_AsciiCharMzk_24(744,67,ti[i],g_TextColor11,g_nBk);
								oldti[i] = ti[i];
							}
						}
						else
						{
							if(oldti[i]!=ti[i])
							{														//减少显示字符的数量,减少刷新时间同时可以防止抖动现象的产生
								CDC_AsciiCharMzk_24(682+i*12,67,ti[i],g_TextColor11,g_nBk);
								oldti[i] = ti[i];
							}
						}
					}
					
					RtcGetDateStr(da);
					
					for(i=0; i<strlen(da); i++)
					{
						if(i==4 || i== 7)
						{
							if(oldda[i] != da[i])
							{
							
								if(i==4)
									CDC_AsciiCharMzk_24(683+i*11,42,':',g_TextColor11,g_nBk);
								else
									CDC_AsciiCharMzk_24(680+i*11,42,':',g_TextColor11,g_nBk);

								oldda[i]=da[i];
							}
						}
						else
						{
							if(oldda[i] != da[i])
							{
								if(i<5)
									CDC_AsciiCharMzk_24(682+i*11,42,da[i],g_TextColor11,g_nBk);
								else if(i<8)
									CDC_AsciiCharMzk_24(679+i*11,42,da[i],g_TextColor11,g_nBk);
								else
									CDC_AsciiCharMzk_24(676+i*11,42,da[i],g_TextColor11,g_nBk);

								
								
								oldda[i]=da[i];
							}
						}
					}
				}

			}	
		} 

		OSTimeDlyHMSM(0, 0, 0, 200); 
		
		if((g_uiSuspendTask&0x0080) != 0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}