# include   "includes.h"
/***************************************
	提示信息显示任务
***************************************/
TEST_EXT	OS_EVENT	*MsgQueue;
WORK_EXT	BOOL		g_bScrProtectRun;										//指示是否处于屏保运行状态,采用关闭显示器模式时使用
WORK_EXT	INT16U		g_uiSuspendTask;										//suspend 0~5 task,high byte rev
WORK_EXT    BOOL 		g_bScreen;
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S		g_nTechfrmBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;

INT16S	m_MsgLine;
INT16S	m_MsgMaxLine;
INT32S 	m_MsgBackColor;
INT32S 	m_MsgRectColor;
INT32S 	m_MsgTextColor;
INT16S  m_MsgLeft;
INT16S  m_MsgTop;
INT16S	m_MsgWidth;
INT16S  m_MsgHeight;
extern  BOOL			g_bInterface;

void	CMsg_Init()
{
	m_MsgBackColor=0;
	m_MsgRectColor=g_nBk;
	m_MsgTextColor=g_TextColor1;
	m_MsgMaxLine=0;
	m_MsgLine=0;

}


void	CMsg_Destroy()
{
//	if(m_MsgBuffer)
//		free(m_MsgBuffer);
}


void	CMsg_AppendMessage(INT8S *message,BOOL flag)
{

//	CDC_PutColorBar(m_MsgLeft+2,m_MsgTop+2,m_MsgWidth-2,m_MsgHeight-2,m_MsgBackColor,m_MsgBackColor);
//	CDC_OutText24Ex(m_MsgLeft+2,m_MsgTop+2,message,m_MsgTextColor,m_MsgBackColor);

	CDC_PutColorBar(m_MsgLeft,m_MsgTop,m_MsgWidth-4,m_MsgHeight,m_MsgBackColor,m_MsgBackColor);
	CDC_OutText24Ex(m_MsgLeft,m_MsgTop,message,m_MsgTextColor,m_MsgBackColor);
}


void	CMsg_SetWH(INT16S x,INT16S y ,INT16S w,INT16S h,INT32S tc,INT32S bc,INT32S rc)
{
	m_MsgBackColor=bc;
	m_MsgRectColor=rc;
	m_MsgTextColor=tc;
	m_MsgLeft=x;
	m_MsgTop=y;
	m_MsgWidth=w;
	m_MsgHeight=h;
	m_MsgMaxLine 	= ((h>>4)-1)*16;
}


void	CMsg_OnDraw()
{
	CDC_PutColorBar(m_MsgLeft,m_MsgTop,m_MsgWidth,m_MsgHeight,m_MsgBackColor,1);
	CDC_Rectangle(m_MsgLeft-1,m_MsgTop-1,m_MsgWidth+1,m_MsgHeight+1,m_MsgRectColor);	

	m_MsgLine = 0;
}


void	Display_Msg_Task(void *data)
{
	INT8U  err;
	INT8S  *msg;
	
	data = data;
	
//	CMsg_Init();
//	CMsg_SetWH(70,442,605,26,YELLOWCOLOR,g_nTechfrmBk,WHITECOLOR);
	
	for(;;)
	{
		msg = (INT8S *)OSQPend(MsgQueue,0,&err);

		if(err==OS_NO_ERR && (g_uiSuspendTask&0x0040)==0)  //修正由于任务进入睡眠状态导致任务挂起滞后，挂起任务之前还会显示消息20140808
		{
			ExitScrProtect();
			OSTimeDly(5);
			
			CMsg_AppendMessage(msg,TRUE);

			OSTimeDly(5);
			
		}
		else
			OSTimeDly(10);

		
		if((g_uiSuspendTask&0x0040)!=0)
			OSTaskSuspend(OS_PRIO_SELF);
	}
}