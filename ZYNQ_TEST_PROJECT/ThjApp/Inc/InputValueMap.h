#ifndef _InputKeyMap_H_
#define _InputKeyMap_H_



typedef enum KeyValue{
//NULL_KEY   = 0,//		空键
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
DELETE_KEY      ,// 		//删除
ESC_KEY       ,//		//返回
INSERT_KEY    ,//		//插入
RESET_KEY	  ,//		复位
FILEMANAGE_KEY,//		文件操作

LEFT_KEY      ,//		//左光标
RIGHT_KEY     ,//		//右光标
DOWN_KEY      ,//		//下光标
UP_KEY        ,//		//上光标
PAGEDOWN_KEY  ,//		//下页改
PAGEUP_KEY    ,//		//上页改

COORD         ,//		//位置改
PROG          ,//		//程序改
PARAM         ,// 		//参数改
KNIFEXPIATE	  ,		//刀补
DIAGNOSE      ,// 		//诊断改

//TAB				,//		切换选择范围
MIDI_KEY	  ,//		修改
CHANGE_KEY	  ,//		变换
BLANK_KEY	  ,//		空白键
 
RECORD_KEY    ,// 		//录入
EDIT_KEY	  ,//		编辑
AUTO_KEY      ,// 		//自动
HAND_KEY      ,// 		//手动
BACKZERO_KEY  ,// 		//归零
WHEEL_KEY     ,//  		//单步手轮

SINGLE_KEY    ,// 		//单段
SKIP_KEY      ,// 		//跳过

PAUSE_KEY     ,///暂停
START_KEY     ,///启动

//RATE_PLUS,	//倍率+
//RATE_MINUS,	//倍率-

//FAST_KEY0     ,///快速-
//FAST_KEY1     ,///快速+

//INP_KEY1      ,///进给+
//INP_KEY0      ,///进给-
              //
//MAIN_KEY0	  ,//轴-
//MAIN_KEY1	  ,//轴+
              //
MAINPOS_KEY	  ,///主轴正转
MAINSTOP_KEY  ,///主轴停
MAINNEG_KEY	  ,///主轴反转
COOL_KEY      ,///冷却
OIL_KEY       ,///润滑
KNIFE_KEY     ,///换刀
CLAMP		  ,//夹具
              //
//A1_KEY        ,///A+
//Y0_KEY        ,///Y-
//Z0_KEY        ,///Z-
//X1_KEY	   	  ,///X+
//Z1_KEY        ,///Z+
//Y1_KEY        ,///Y+
//A0_KEY        ,///A-
//X0_KEY        ,///X-
              
//SPEED_KEY	  ,//速度
              
//STEPINC1_KEY  ,//1
//STEPINC2_KEY  ,//0.1
////STEPINC3_KEY  ,//0.01
//STEPINC4_KEY  ,//0.001
//AXIS_X_KEY	  ,//X轴
//AXIS_Y_KEY	  ,//Y轴
//AXIS_Z_KEY	  ,//Z轴
//AXIS_A_KEY	  ,//A轴
               
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