#include	"includes.h"
#include	"control.h"


INT16S 			m_nCurrentControl=0;
INT16S 			m_nControlMax=0;

CTROL*			m_ptrArrayCtrl[80];						//暂时限定最大80个CONTROL
CTROL			m_ArrayCtrl[80];
extern			INT32S	g_nBk;
void	CMainFrm_Init()
{
	INT16S i=0;
	#if OS_CRITICAL_METHOD == 3                      	/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  	/* Prevent compiler warning                           */
	#endif  

	m_nCurrentControl=0;
	m_nControlMax=0;	
	OS_ENTER_CRITICAL();

	for(i=0;i<80;i++)
	{
		m_ArrayCtrl[i].style = CTROL_STYLE_EDIT;
		m_ArrayCtrl[i].ID	 = 0XFFFF;
		strcpy(m_ArrayCtrl[i].name,"Empty");
		m_ArrayCtrl[i].x = 0;
		m_ArrayCtrl[i].y = 0;
		m_ArrayCtrl[i].w = 0;
		m_ArrayCtrl[i].h = 0;
		m_ArrayCtrl[i].key = 0;
		m_ArrayCtrl[i].algin = CENTER_ALGIN;
		m_ptrArrayCtrl[i]=&m_ArrayCtrl[i];
	}
	
	OS_EXIT_CRITICAL();	
}


void	CMainFrm_Destroy()
{
	m_nControlMax=0;
/*
	INT16S i;
	OS_ENTER_CRITICAL();	
	if(m_ptrArrayCtrl)
	{
		CTROL*  *ptr = m_ptrArrayCtrl;
		for(i=0; i<m_nControlMax; i++,ptr++)
			free(*ptr);
	   free(m_ptrArrayCtrl);
	   m_ptrArrayCtrl=NULL;
	}
	OS_EXIT_CRITICAL();
*/
}


void		CMainFrm_SetArraySize(INT16S size )
{

	if( !size ) return;
	
	m_nControlMax	= size;

	/*
	if( m_ptrArrayCtrl )
	{
		CTROL*  *ptr = m_ptrArrayCtrl;
		OS_ENTER_CRITICAL();
		for(i=0; i<m_nControlMax; i++,ptr++)
			free(ptr);
	   free(m_ptrArrayCtrl);
	   m_ptrArrayCtrl=NULL;
	   OS_EXIT_CRITICAL();
	}
	m_nControlMax	= size;
	OS_ENTER_CRITICAL();
	m_ptrArrayCtrl  = malloc((INT32S)size*4);//(INT32S)sizeof(CTROL));
	OS_EXIT_CRITICAL();
	if(m_ptrArrayCtrl == NULL)
	{
		Uart_Printf(" MainFrm Array memory malloc Error! \n");
		Uart_Printf(" Press key to exit!");
		Uart_Getch();
		ExitSystem(0);
	}
	for(i=0;i<m_nControlMax;i++)
	{
		//ptr=m_ptrArrayCtrl[i];
		OS_ENTER_CRITICAL();
		m_ptrArrayCtrl[i]=malloc(sizeof(CTROL));
		OS_EXIT_CRITICAL();
		if(m_ptrArrayCtrl[i] == NULL)
		{
			Uart_Printf(" MainFrm Array unit memory malloc Error! \n");
			Uart_Printf(" Press key to exit!");
			Uart_Getch();
			ExitSystem(0);
		}
	}
	*/
}


void   CMainFrm_Add(INT16S i,CTROL *pCtrl)
{

	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif  

	if( i >= m_nControlMax ) return;

	OS_ENTER_CRITICAL();
	
	memcpy(m_ptrArrayCtrl[i],pCtrl,sizeof(CTROL));
	
	OS_EXIT_CRITICAL();
}


CTROL*	CMainFrm_GetItemID( INT16U nID )
{
	INT16S i=0;
	CTROL*  *ptr = m_ptrArrayCtrl;
	
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif 
	
	if( !m_nControlMax || !m_ptrArrayCtrl) return NULL;
	
	OS_ENTER_CRITICAL();
	
	for(i=0; i<m_nControlMax&&(*ptr); i++,ptr++)
	{
		if( (*ptr)->ID == nID )
		{
			OS_EXIT_CRITICAL();
			return *ptr;
		}
	}
	
	OS_EXIT_CRITICAL();
	
	return NULL;
}


void	 CMainFrm_ShowAllControl()
{
	INT16S 		i=0;
	CRect 		rect;
	
	CTROL*  *ptr = m_ptrArrayCtrl;
	
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                  /* Prevent compiler warning                           */
	#endif  

	if( m_ptrArrayCtrl == NULL ) return;
	
	OS_ENTER_CRITICAL();
	
	for(i=0;i<m_nControlMax;i++,ptr++)
	{
		rect.left=(*ptr)->x;
		rect.top=(*ptr)->y;
		rect.right=(*ptr)->x+(*ptr)->w;
		rect.bottom=(*ptr)->y+(*ptr)->h;
		
		switch((*ptr)->style)
		{
		case CTROL_STYLE_EDIT:
			CDC_DrawEdit(rect,WHITECOLOR);
			break;
		case CTROL_STYLE_BUTTON:
			CDC_DrawButton(rect,DARKGRAYCOLOR);
			break;
		default:break;
		}
	}
	
	OS_EXIT_CRITICAL();
}
