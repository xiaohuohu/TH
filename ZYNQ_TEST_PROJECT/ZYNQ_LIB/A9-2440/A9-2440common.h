/*
 * A9-2440common.h
 *
 *  Created on: 2018��10��18��
 *      Author: PC
 */

#ifndef A9_2440COMMON_H_
#define A9_2440COMMON_H_

#include "os.h"
#include "GlobalType.h"
unsigned char ReadLed(int num, unsigned char *sta);
unsigned char WriteLed(int num, unsigned char sta);
void Delay(int times);
/******************************�����룺Lib�ⱨ��******************************/
#define 	ERR_NONE					0			// �ɹ�
#define 	ERR_PARAMETER				1			// ��������
#define		ERR_SEM_CREATE				2			// �ź�����������
#define 	ERR_ARC_AXIS_MAP			3			// Բ����ѡλ����
#define 	ERR_ARC_NO_EXIST			4			// Բ��������
#define 	ERR_RESPONSE_TIMEOUT		5			// �˶�����Ӧ��ʱ
#define 	ERR_ARC_PARALLEL			6			// Բ���ϵĵ�ƽ��
#define		ERR_AXIS_MAP_CONFLICT		7			// ��ӳ���ͻ�������
#define		ERR_ARCBLEND_INPUT_SEG		8			//���������߶�(Բ�����ɴ���)
#define		ERR_ARCBLEND_INPUT_PARA		9			//���ɾ��Ȼ��߲�����ֵҪ�����(Բ�����ɴ���)
#define		ERR_ARCBLEND_INCOPLANARITY	10			//����Բ����ʱ�������߲�����(Բ�����ɴ���)
#define		ERR_ARCBLEND_INPUT_ANGLE	11			//�������߼нǳ����趨��ֵ��Χ(Բ�����ɴ���)
#define		ERR_ARCBLEND_TOO_SHORT		12			//�������߳���̫С(Բ�����ɴ���)
#define		ERR_ARCBLEND_OUTPUT_SEG		13			//���ɺ�, ԭ�߶εĳ���С��min_len(Բ�����ɴ���)
#define		ERR_ARCBLEND_BLENDARC_R		14			//���ɺ�, Բ�뾶�������Բ�뾶С��min_R(Բ�����ɴ���)
#define		ERR_ARCBLEND_BLENDARC_len	15			//���ɺ�, ����Բ����С��min_len(Բ�����ɴ���)
#define		ERR_ARCBLEND_EXECUTE_FAIL	16			//������Բ�������޽�(Բ�����ɴ���)


/******************************�����룺Motion�ⱨ��***************************/
#define 	_ERR_NONE					0			// �ɹ�
#define 	_ERR_PARAMETER				101			// ��������
#define 	_ERR_DATA_SIZE				102			// ͨ���������쳣
#define		_ERR_AXIS_STOP				103			// ��Ӧ�ᴦ��ֹͣ״̬
#define		_ERR_MOTION_CONFLICT		104			// �˶���ͻ
#define		_ERR_PARA_SET_FORBIT		105			// ��ǰ״̬�������޸Ĳ���
#define		_ERR_DATA_MODE				106			// ��������ģʽ����
#define		_ERR_ADMODE					107			// �Ӽ���ģʽ���ô���
#define		_ERR_S_SPEED_CALC			108			// S���ٶȹ滮ʱ�������
#define		_ERR_EMERGENCY		 		109			// �ⲿ����ֹͣ�ź���Ч��
#define		_ERR_MOTION_DATA			110			// �ײ��˶�Ŀ��λ�������쳣
#define		_ERR_CMD_NULL		 		111			// ��ָ�����Чָ��
#define		_ERR_FPGA_WR				112			// FPGA��д���Դ���
#define		_ERR_INP_FIFO_FULL			113			// �岹�˶�ָ���������
#define		_ERR_PTP_FIFO_FULL			114			// ��λ�˶�ָ���������
#define		_ERR_POS_INCONSISTENT		115			// �ָ�����ʱ����λ��ƫ��
#define		_ERR_FIRMWARE_NOT_SUPPORT	116			// ��ǰ�̼���֧��
#define		_ERR_AXIS_TYPE				117			// �����ʹ���
#define		_ERR_EtherCAT_CONNECT		118			// ����ͨ�Ź���
#define 	_ERR_AXIS_ALARM				119			// �ᱨ��
#define		_ERR_MAILBOX_WR				120			// ��������ͨ�Ŵ���
#define		_ERR_RIGID_TAPPING			121			// ���Թ�˿�����и���������
#define		_ERR_FIFO_EVENT				122			// �����¼�δ���������쳣


/******************************��Ŷ���******************************/
#define		AXIS_1					1			// ��1�����
#define		AXIS_2					2			// ��2�����
#define		AXIS_3					3			// ��3�����
#define		AXIS_4					4			// ��4�����
#define		AXIS_5					5			// ��5�����
#define		AXIS_6					6			// ��6�����
#define		AXIS_7					7			// ��7�����
#define		AXIS_8					8			// ��8�����
#define		AXIS_9					9			// ��9�����
#define		AXIS_10					10			// ��10�����
#define		AXIS_11					11			// ��11�����
#define		AXIS_12					12			// ��12�����
#define		AXIS_13					13			// ��13�����
#define		AXIS_14					14			// ��14�����
#define		AXIS_15					15			// ��15�����
#define		AXIS_16					16			// ��16�����
#define		HW_AXIS					17			// �������
#define		SP_AXIS					18			// �������
#define 	INPA_AXIS				63			// A��岹�����(����)
#define 	INPB_AXIS				62			// B��岹�����


INT32U My_OSTimeGet(void);
void LCD_Power(int sta);


//���ܺ���Ҫ���°�
void pro_disable(void);

/***********************************************
 * ��������bind_axis_pro
 * ����:
 * 		axis_num: 1-6����
 * 		pro_num:  0-7��̽��
 *
 * ���ܣ�����ź�̽��ţ�ÿ�ΰ�ʱ�Դ�ʹ�ܡ�
 * ����ֵ�����󷵻� -1���󶨳ɹ����� 1
 *
 ************************************************/
int bind_axis_pro(int axis_num,int pro_num);


/************************************************
 *��������read_pro_state
 *����ֵ��
 *		���ص�ǰ��������̽��� ��0-7
 *		���󷵻�ֵ�� ���� 7
 *
 ************************************************/
int read_pro_state(void);

#endif /* A9_2440COMMON_H_ */
