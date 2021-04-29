/*
 * A9-2440common.h
 *
 *  Created on: 2018年10月18日
 *      Author: PC
 */

#ifndef A9_2440COMMON_H_
#define A9_2440COMMON_H_

#include "os.h"
#include "GlobalType.h"
unsigned char ReadLed(int num, unsigned char *sta);
unsigned char WriteLed(int num, unsigned char sta);
void Delay(int times);
/******************************错误码：Lib库报错******************************/
#define 	ERR_NONE					0			// 成功
#define 	ERR_PARAMETER				1			// 参数错误
#define		ERR_SEM_CREATE				2			// 信号量创建错误
#define 	ERR_ARC_AXIS_MAP			3			// 圆弧轴选位错误
#define 	ERR_ARC_NO_EXIST			4			// 圆弧不存在
#define 	ERR_RESPONSE_TIMEOUT		5			// 运动库响应超时
#define 	ERR_ARC_PARALLEL			6			// 圆弧上的点平行
#define		ERR_AXIS_MAP_CONFLICT		7			// 轴映射冲突引起错误
#define		ERR_ARCBLEND_INPUT_SEG		8			//非相连曲线段(圆弧过渡处理)
#define		ERR_ARCBLEND_INPUT_PARA		9			//过渡精度或者参数阈值要求过高(圆弧过渡处理)
#define		ERR_ARCBLEND_INCOPLANARITY	10			//存在圆弧段时输入曲线不共面(圆弧过渡处理)
#define		ERR_ARCBLEND_INPUT_ANGLE	11			//输入曲线夹角超出设定阈值范围(圆弧过渡处理)
#define		ERR_ARCBLEND_TOO_SHORT		12			//输入曲线长度太小(圆弧过渡处理)
#define		ERR_ARCBLEND_OUTPUT_SEG		13			//过渡后, 原线段的长度小于min_len(圆弧过渡处理)
#define		ERR_ARCBLEND_BLENDARC_R		14			//过渡后, 圆半径过大或者圆半径小于min_R(圆弧过渡处理)
#define		ERR_ARCBLEND_BLENDARC_len	15			//过渡后, 过渡圆弧长小于min_len(圆弧过渡处理)
#define		ERR_ARCBLEND_EXECUTE_FAIL	16			//求解过渡圆过程中无解(圆弧过渡处理)


/******************************错误码：Motion库报错***************************/
#define 	_ERR_NONE					0			// 成功
#define 	_ERR_PARAMETER				101			// 参数错误
#define 	_ERR_DATA_SIZE				102			// 通信数据量异常
#define		_ERR_AXIS_STOP				103			// 对应轴处于停止状态
#define		_ERR_MOTION_CONFLICT		104			// 运动冲突
#define		_ERR_PARA_SET_FORBIT		105			// 当前状态不允许修改参数
#define		_ERR_DATA_MODE				106			// 输入数据模式错误
#define		_ERR_ADMODE					107			// 加减速模式设置错误
#define		_ERR_S_SPEED_CALC			108			// S型速度规划时计算错误
#define		_ERR_EMERGENCY		 		109			// 外部紧急停止信号生效中
#define		_ERR_MOTION_DATA			110			// 底层运动目标位置数据异常
#define		_ERR_CMD_NULL		 		111			// 空指令或无效指令
#define		_ERR_FPGA_WR				112			// FPGA读写测试错误
#define		_ERR_INP_FIFO_FULL			113			// 插补运动指令缓冲区已满
#define		_ERR_PTP_FIFO_FULL			114			// 点位运动指令缓冲区已满
#define		_ERR_POS_INCONSISTENT		115			// 恢复倍率时存在位置偏差
#define		_ERR_FIRMWARE_NOT_SUPPORT	116			// 当前固件不支持
#define		_ERR_AXIS_TYPE				117			// 轴类型错误
#define		_ERR_EtherCAT_CONNECT		118			// 总线通信故障
#define 	_ERR_AXIS_ALARM				119			// 轴报警
#define		_ERR_MAILBOX_WR				120			// 邮箱命令通信错误
#define		_ERR_RIGID_TAPPING			121			// 刚性攻丝过程中跟随误差过大
#define		_ERR_FIFO_EVENT				122			// 缓存事件未触发或处理异常


/******************************轴号定义******************************/
#define		AXIS_1					1			// 第1轴轴号
#define		AXIS_2					2			// 第2轴轴号
#define		AXIS_3					3			// 第3轴轴号
#define		AXIS_4					4			// 第4轴轴号
#define		AXIS_5					5			// 第5轴轴号
#define		AXIS_6					6			// 第6轴轴号
#define		AXIS_7					7			// 第7轴轴号
#define		AXIS_8					8			// 第8轴轴号
#define		AXIS_9					9			// 第9轴轴号
#define		AXIS_10					10			// 第10轴轴号
#define		AXIS_11					11			// 第11轴轴号
#define		AXIS_12					12			// 第12轴轴号
#define		AXIS_13					13			// 第13轴轴号
#define		AXIS_14					14			// 第14轴轴号
#define		AXIS_15					15			// 第15轴轴号
#define		AXIS_16					16			// 第16轴轴号
#define		HW_AXIS					17			// 手轮轴号
#define		SP_AXIS					18			// 主轴轴号
#define 	INPA_AXIS				63			// A组插补轴轴号(常用)
#define 	INPB_AXIS				62			// B组插补轴轴号


INT32U My_OSTimeGet(void);
void LCD_Power(int sta);


//禁能后需要重新绑定
void pro_disable(void);

/***********************************************
 * 函数名：bind_axis_pro
 * 参数:
 * 		axis_num: 1-6个轴
 * 		pro_num:  0-7号探针
 *
 * 功能：绑定轴号和探针号，每次绑定时自带使能。
 * 返回值：错误返回 -1，绑定成功返回 1
 *
 ************************************************/
int bind_axis_pro(int axis_num,int pro_num);


/************************************************
 *函数名：read_pro_state
 *返回值：
 *		返回当前被触发的探针号 ：0-7
 *		错误返回值： 大于 7
 *
 ************************************************/
int read_pro_state(void);

#endif /* A9_2440COMMON_H_ */
