# ifndef	___VIEWMSG_H___
# define	___VIEWMSG_H___
# include	"includes.h"

void    Display_Msg_Task(void *data);

void	CMsg_Init(void);
void	CMsg_Destroy(void);
void	CMsg_AppendMessage(INT8S *message,BOOL flag);
void	CMsg_SetWH(INT16S x,INT16S y,INT16S w,INT16S h,INT32S tc,INT32S bc,INT32S rc);
void	CMsg_OnDraw(void);

# endif
