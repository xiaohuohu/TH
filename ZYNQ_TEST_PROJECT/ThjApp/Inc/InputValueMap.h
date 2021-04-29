#ifndef _InputKeyMap_H_
#define _InputKeyMap_H_



typedef enum KeyValue{
//NULL_KEY   = 0,//		�ռ�
G_KEY	='G',              
M_KEY	='M',              
F_KEY	='F',              
R_KEY	='R',              
T_KEY	='T',              
S_KEY	='S',              
X_KEY	='X',              
Y_KEY	='Y',              
Z_KEY	='Z',              
A_KEY	='A',              
P_KEY	='P',              
O_KEY	='O',              

INPUT1_KEY='1',
INPUT2_KEY='2',
INPUT3_KEY='3',
INPUT4_KEY='4',
INPUT5_KEY='5',
INPUT6_KEY='6',
INPUT7_KEY='7',
INPUT8_KEY='8',
INPUT9_KEY='9',
INPUT0_KEY='0',

ENTER_KEY  =0x80,//		//EOB
DELETE_KEY      ,// 		//ɾ��
ESC_KEY       ,//		//����
INSERT_KEY    ,//		//����
RESET_KEY	  ,//		��λ
FILEMANAGE_KEY,//		�ļ�����

LEFT_KEY      ,//		//����
RIGHT_KEY     ,//		//�ҹ��
DOWN_KEY      ,//		//�¹��
UP_KEY        ,//		//�Ϲ��
PAGEDOWN_KEY  ,//		//��ҳ��
PAGEUP_KEY    ,//		//��ҳ��

COORD         ,//		//λ�ø�
PROG          ,//		//�����
PARAM         ,// 		//������
KNIFEXPIATE	  ,		//����
DIAGNOSE      ,// 		//��ϸ�

//TAB				,//		�л�ѡ��Χ
MIDI_KEY	  ,//		�޸�
CHANGE_KEY	  ,//		�任
BLANK_KEY	  ,//		�հ׼�
 
RECORD_KEY    ,// 		//¼��
EDIT_KEY	  ,//		�༭
AUTO_KEY      ,// 		//�Զ�
HAND_KEY      ,// 		//�ֶ�
BACKZERO_KEY  ,// 		//����
WHEEL_KEY     ,//  		//��������

SINGLE_KEY    ,// 		//����
SKIP_KEY      ,// 		//����

PAUSE_KEY     ,///��ͣ
START_KEY     ,///����

//RATE_PLUS,	//����+
//RATE_MINUS,	//����-

//FAST_KEY0     ,///����-
//FAST_KEY1     ,///����+

//INP_KEY1      ,///����+
//INP_KEY0      ,///����-
              //
//MAIN_KEY0	  ,//��-
//MAIN_KEY1	  ,//��+
              //
MAINPOS_KEY	  ,///������ת
MAINSTOP_KEY  ,///����ͣ
MAINNEG_KEY	  ,///���ᷴת
COOL_KEY      ,///��ȴ
OIL_KEY       ,///��
KNIFE_KEY     ,///����
CLAMP		  ,//�о�
              //
//A1_KEY        ,///A+
//Y0_KEY        ,///Y-
//Z0_KEY        ,///Z-
//X1_KEY	   	  ,///X+
//Z1_KEY        ,///Z+
//Y1_KEY        ,///Y+
//A0_KEY        ,///A-
//X0_KEY        ,///X-
              
//SPEED_KEY	  ,//�ٶ�
              
//STEPINC1_KEY  ,//1
//STEPINC2_KEY  ,//0.1
////STEPINC3_KEY  ,//0.01
//STEPINC4_KEY  ,//0.001
//AXIS_X_KEY	  ,//X��
//AXIS_Y_KEY	  ,//Y��
//AXIS_Z_KEY	  ,//Z��
//AXIS_A_KEY	  ,//A��
               
MENU_LEFT_KEY,
MENU_F1_KEY	 ,//
MENU_F2_KEY	 ,//
MENU_F3_KEY	 ,//
MENU_F4_KEY	 ,//
MENU_F5_KEY	 ,//
MENU_F6_KEY	 ,//
MENU_RIGHT_KEY,

POS_KEY,
PROG_KEY,
SYSTEM_KEY,
OFFSET_KEY,
DGNOS_KEY


}int_key;



#endif 