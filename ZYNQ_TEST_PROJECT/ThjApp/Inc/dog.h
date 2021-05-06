#ifndef		__DOG_H___
#define		__DOG_H___


#include	"rtc.h"

typedef struct
{
	INT32S 	Factory_Password; //出厂密码及用户备用密码
	INT32S 	User_Password;	 //用户一级密码
	INT32S	User_Active;     //指示是否解除用户级密码限制,12345678解除限制
	INT32S	User_Days;		//用户一级试用天数
	DATE_T 	User_LastDay; //用户级最后一次访问软件狗的日期
	INT32S 	Adtech_Password;//公司一级密码
	INT32S	Adtech_Days;    //公司一级试用天数
	INT32S	Adtech_Active;  //指示是否解除公司级密码限制,12345678解除限制
	//*
	INT32S	Adtech_First;   //保留未用
	DATE_T Adtech_LastDay; //公司级最后一次访问软件狗的日期
	INT32S  First_Use;  //是否为第一次使用产品
	INT32S	Serial_No;    	//控制器序列号
	TIME_T  Now_Time; //记录用户级访问软件狗的当前时间
	TIME_T  Now_Time2;//记录用户级访问软件狗的当前时间
	INT32S 	User_Password2; //用户级二级密码
	INT32S	User_Days2;		//用户级二级试用天数
	INT32S	Machine_Password;//客户设定软件狗的密码默认为1234
	INT32S 	Machine_SerialNo;//客户机器序列号默认为123456
	INT32S 	Dog_SerialNo;   //狗的序列号
	INT32S	Dog_Password;   //狗的密码
	INT32S 	Adtech_Password2;//公司级二级密码
	INT32S	Adtech_Days2;    //公司级二级试用天数
	INT32S	User_Level;  //确定用户当前级别以确定需输入那一级别的密码
	INT32S	Adtech_Level;//确定公司级当前级别以确定需输入那一级别的密码
	INT32S 	Adtech_Password3;//公司级三级密码
	INT32S	Adtech_Days3;    //公司级三级试用天数
	INT32S 	User_Password3; //用户级三级密码
	INT32S	User_Days3;		//用户级三级试用天数
	//*/
}DOG;

INT32U 	InitDog(void);
INT32U  WriteDog(void);
INT32U  ReadDog(void);
void   CheckPassWord(void);

#endif