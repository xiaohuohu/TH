/*
 * xms.h
 *
 *  Created on: 2014-2-13
 *      Author: Administrator
 */

#ifndef XMS_H_
#define XMS_H_

#include "adt_datatype.h"

# define	HZK16XMSSIZE 	(INT32U)(300*1024)
# define	HZK16ZXMSSIZE 	(INT32U)(30*1024)
# define	HZK24XMSSIZE 	(INT32U)(500*1024)
# define	HZK24ZXMSSIZE 	(INT32U)(74*1024)
# define	BMPXMSSIZE	(INT32U)(2*1024*1024)

void	DestroyXMS(void);
void	InitXMS(void);

/*****************************************
判断XMS或XMS驱动程序是否存在
如果存在,则获取XMS远指针,并返回以BCD码表示的XMS版本号
如果不存在则返回0

ads版 永远返回1
******************************************/
INT16U GetXMS(void);
/*****************************************
返回可以利用的XMS内存空间(单位：KB)

ads版  永远返回65535
******************************************/
INT16U AvailXMS(void);
/*****************************************
申请分配XMS内存
n：申请分配的内存字节数
成功返回指向XMS的句柄（1～6），失败返回0

ads版返回分配的句柄，注意此时要保证每次调用此函数的参数的唯一性，
******************************************/
INT16U AllocXMS(INT32U n);
/*****************************************
释放XMS内存空间
XMSHandle: 指向XMS的句柄

ads版无意义
******************************************/
void FreeXMS(INT16U XMSHandle);
/*****************************************
移动常规内存中的数据到XMS中
p:			要移动的常规内存的指针
XMSHandle:	指向XMS的目标句柄（1～6）
XMSOffset:	XMS的目标偏移量
length:		要移动的数据长度(必须为偶数)

成功返回1
失败返回0
******************************************/

INT16U MEMToXMS(INT8U *p,INT16U XMSHandle,INT32U XMSOffset,INT32U length);
/*****************************************
移动XMS中的数据到常规内存中
p:			要移动到的常规内存的指针
XMSHandle:	指向XMS的源句柄（1～6）
XMSOffset:	XMS的源偏移量
length:		要移动的数据长度(必须为偶数)

成功返回1
失败返回0
******************************************/
INT16U XMSToMEM(INT8U *p,INT16U XMSHandle,INT32U XMSOffset,INT32U length);

#endif /* XMS_H_ */
