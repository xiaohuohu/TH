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
�ж�XMS��XMS���������Ƿ����
�������,���ȡXMSԶָ��,��������BCD���ʾ��XMS�汾��
����������򷵻�0

ads�� ��Զ����1
******************************************/
INT16U GetXMS(void);
/*****************************************
���ؿ������õ�XMS�ڴ�ռ�(��λ��KB)

ads��  ��Զ����65535
******************************************/
INT16U AvailXMS(void);
/*****************************************
�������XMS�ڴ�
n�����������ڴ��ֽ���
�ɹ�����ָ��XMS�ľ����1��6����ʧ�ܷ���0

ads�淵�ط���ľ����ע���ʱҪ��֤ÿ�ε��ô˺����Ĳ�����Ψһ�ԣ�
******************************************/
INT16U AllocXMS(INT32U n);
/*****************************************
�ͷ�XMS�ڴ�ռ�
XMSHandle: ָ��XMS�ľ��

ads��������
******************************************/
void FreeXMS(INT16U XMSHandle);
/*****************************************
�ƶ������ڴ��е����ݵ�XMS��
p:			Ҫ�ƶ��ĳ����ڴ��ָ��
XMSHandle:	ָ��XMS��Ŀ������1��6��
XMSOffset:	XMS��Ŀ��ƫ����
length:		Ҫ�ƶ������ݳ���(����Ϊż��)

�ɹ�����1
ʧ�ܷ���0
******************************************/

INT16U MEMToXMS(INT8U *p,INT16U XMSHandle,INT32U XMSOffset,INT32U length);
/*****************************************
�ƶ�XMS�е����ݵ������ڴ���
p:			Ҫ�ƶ����ĳ����ڴ��ָ��
XMSHandle:	ָ��XMS��Դ�����1��6��
XMSOffset:	XMS��Դƫ����
length:		Ҫ�ƶ������ݳ���(����Ϊż��)

�ɹ�����1
ʧ�ܷ���0
******************************************/
INT16U XMSToMEM(INT8U *p,INT16U XMSHandle,INT32U XMSOffset,INT32U length);

#endif /* XMS_H_ */
