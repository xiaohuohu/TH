# ifndef	___TELCOM__H___
# define	___TELCOM__H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        telcom.h
  创建人：       众为兴
  创建日期：		2005/08/12
  主要功能描述:   主要功能描述:   暂时只支持COM1,COM2，8位数据位，1位停止位格式
  		
  修改记录：
   1.   ...
*************************************************/
/*
# ifdef 	__cplusplus
	# define __CPPARGS ...
# else
	# define __CPPARGS
# endif
*/
#include	"os_cpu.h"
//#include 	"serial.h"


void	CTelCom_Init(INT16S comnum,INT32S baudrate);
void	CTelCom_Destroy(void);

INT16S	CTelCom_SetCom(INT16S comnum,INT32S baudrate);
void	CTelCom_ResetCom(void);

INT16S 	CTelCom_SendChar(INT8U ch,INT32S lTimeout);
INT16S 	CTelCom_GetChar(INT32S lTimeout);
INT16S 	CTelCom_GetBuffer(void);

INT16S 	CTelCom_SendString(INT8U *buf,INT32S len,INT32S lTimeout);
INT16S 	CTelCom_GetString(INT8U *str,INT32S len,INT32S lTimeout);


#endif
