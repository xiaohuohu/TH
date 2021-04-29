#ifndef		__DOG_H___
#define		__DOG_H___


#include	"rtc.h"

typedef struct
{
	INT32S 	Factory_Password; //�������뼰�û���������
	INT32S 	User_Password;	 //�û�һ������
	INT32S	User_Active;     //ָʾ�Ƿ����û�����������,12345678�������
	INT32S	User_Days;		//�û�һ����������
	DATE_T 	User_LastDay; //�û������һ�η��������������
	INT32S 	Adtech_Password;//��˾һ������
	INT32S	Adtech_Days;    //��˾һ����������
	INT32S	Adtech_Active;  //ָʾ�Ƿ�����˾����������,12345678�������
	//*
	INT32S	Adtech_First;   //����δ��
	DATE_T Adtech_LastDay; //��˾�����һ�η��������������
	INT32S  First_Use;  //�Ƿ�Ϊ��һ��ʹ�ò�Ʒ
	INT32S	Serial_No;    	//���������к�
	TIME_T  Now_Time; //��¼�û�������������ĵ�ǰʱ��
	TIME_T  Now_Time2;//��¼�û�������������ĵ�ǰʱ��
	INT32S 	User_Password2; //�û�����������
	INT32S	User_Days2;		//�û���������������
	INT32S	Machine_Password;//�ͻ��趨�����������Ĭ��Ϊ1234
	INT32S 	Machine_SerialNo;//�ͻ��������к�Ĭ��Ϊ123456
	INT32S 	Dog_SerialNo;   //�������к�
	INT32S	Dog_Password;   //��������
	INT32S 	Adtech_Password2;//��˾����������
	INT32S	Adtech_Days2;    //��˾��������������
	INT32S	User_Level;  //ȷ���û���ǰ������ȷ����������һ���������
	INT32S	Adtech_Level;//ȷ����˾����ǰ������ȷ����������һ���������
	INT32S 	Adtech_Password3;//��˾����������
	INT32S	Adtech_Days3;    //��˾��������������
	INT32S 	User_Password3; //�û�����������
	INT32S	User_Days3;		//�û���������������
	//*/
}DOG;

INT32U 	InitDog(void);
INT32U  WriteDog(void);
INT32U  ReadDog(void);
void   CheckPassWord(void);

#endif