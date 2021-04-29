# ifndef 	___ADT4860_H___
# define 	___ADT4860_H___

# include	"os_cpu.h"


	#define ERROR_FLAG	1
	#define SUCCESSFUL	0
	
    #define ADT_4860_IRQ_ID 61
//----------------------------------------------------//
//            add by gjt[2014.06.13]				  //
//----------------------------------------------------//
/*********************************************************************

 函数名称:	set_axis_enable
 函数功能:  使能对应轴,其可分为正方向使能和负方向使能
 输入参数:
 	axis : 轴号[1-8]
 	dir  : 表示正方向或负方向
 	  0 - 同时设置正方向和负方向使能
 	  1 - 设置正方向使能
 	  2 - 设置负方向使能
 	  
 	sta  : 表示设置限位状态
 	  0 - 使能对应轴
 	  1 - 关闭对应轴
 	  
 输出参数:  无
 返 回 值:  
 	0 - 正常
 	1 - 错误
 注意事项:  
   此功能现只有在CNC49xx 和 CNC46xx 上使用有效,其它机型暂不支持
   
*********************************************************************/
	int  set_axis_enable(const int axis,const int dir,const int sta);
	
/*********************************************************************

 函数名称:	get_axis_enable
 函数功能:  返回当前轴使能状态
 输入参数:  无
 输出参数:  无
 返回值:       
 	| bit31-bit24 | bit23-bit16 		| bit15-bit8 | bit7-bit0    		|
	| 保留        | 轴8-轴1 负方向使能	| 保留       | 轴8-轴1 正方向使能 	|
	
 	对应位:
 	0 - 表示当前轴使能
 	1 - 表示当前轴关闭
 	
 注意事项:  无
   
*********************************************************************/
	INT32U  get_axis_enable(void);


	void  motion_init(void);
	int set_pulse_mode(int axis,int value,int logic,int dir_logic);
	int set_stop0_mode(int axis,int value,int logic);
	int set_stop1_mode(int axis,int value,int logic);
	int set_limit_mode(int axis,int v1,int v2,int logic);
//----------------------------------------------------//
//               驱动状态检查函数                     //
//----------------------------------------------------//
	int get_status(int axis,int *value);
	int get_inp_status(int *value);
	int get_int_status(int *value);
//----------------------------------------------------//
//               运动参数设定函数                     //
//----------------------------------------------------//
	int set_acc(int axis,long Value);
	int set_startv(int axis,long Value);
	int set_speed(int axis,long Value);
	int set_command_pos(int axis,long Value);
	int set_actual_pos(int axis,long Value);
	int continue_move(int axis,int dir);
//----------------------------------------------------//
//               运动参数检查函数                     //
//----------------------------------------------------//
	int get_command_pos(int axis,long *pos);
	int get_actual_pos(int axis,long *pos);
	int get_coder_speed(int axis,long *pos);
	int get_speed(int axis,long *pos);
	int get_startv(int axis,long *pos);
	int get_acc(int axis,long *pos);
	
//----------------------------------------------------//
//                   驱动函数                         //
//----------------------------------------------------//
	int pmove(int axis,long pulse);
	int dec_stop(int axis);
	int sudden_stop(int axis);
	int inp_move2(int axis1,int axis2,long pulse1,long pulse2);
	int inp_move3(int axis1,int axis2,int axis3,long pulse1,long pulse2,long pulse3);
	int inp_move4(long pulse1,long pulse2,long pulse3,long pulse4);
	int inp_move5(long pulse1,long pulse2,long pulse3,long pulse4,long pulse5);
	int inp_move6(long pulse1,long pulse2,long pulse3,long pulse4,long pulse5,long pulse6);

	int inp_moves(int axistotal,...);
//----------------------------------------------------//
//                开关量输入/输出函数                 //
//----------------------------------------------------//
						/*p1:原始端口，p2:目标端口*/
	int SetInPort(unsigned int p1,unsigned int p2);
	int SetOutPort(unsigned int p1,unsigned int p2);

	int read_bit(int number);
	int write_bit(int number,int value);

//----------------------------------------------------//
//                中断函数                            //
//----------------------------------------------------//

	void Clear_Int(void);
	void Enable_ADT834INT(void);
	void Disable_ADT834INT(void);
	//void __irq ADT834Isr(void);
	extern unsigned long  iMoveInpCount;
	
	extern OS_EVENT *gMoveStopSEM;
	
/**********************
* 开IO中断服务
*****************************/
	void Clear_IO_Int(void);
	void Enable_IO_Int(void);
	void Disable_IO_Int(void);
/**********************
* Num	:选择哪个IO作为中断源
* Logic	:中断源上升触发1
		 中断源下降触发0
*****************************/
	int  Set_IO_Int(int Num,int logic);
	extern OS_EVENT *gIOInterruptSEM;

//----------------------------------------------------//
//				   FIFO函数                           //
//----------------------------------------------------//
	void set_range(long Value,int mode);
	void reset_fifo(void);
	void read_fifo(unsigned short *value);
	int  fifo_inp_move1(int axis1,long pulse1,long speed);
	int  fifo_inp_move2(int axis1,int axis2,long pulse1,long pulse2,long speed);
	int  fifo_inp_move3(int axis1,int axis2,int axis3,long pulse1,long pulse2,long pulse3,long speed);
	int  fifo_inp_move4(long pulse1,long pulse2,long pulse3,long pulse4,long speed);
	int  fifo_inp_move5(long pulse1,long pulse2,long pulse3,long pulse4,long pulse5,long speed);
	int  fifo_inp_move6(long pulse1,long pulse2,long pulse3,long pulse4,long pulse5,long pulse6,long speed);
	int  fifo_index_inp_move4(int index,long pulse1,long pulse2,long pulse3,long pulse4,long speed);
/*版本104以上取消矢量速度插补功能函数*/
//	int  fifo_vsinp_move2(int axis1,int axis2,long pulse1,long pulse2,long speed);						//矢量速度缓冲插补
//	int  fifo_vsinp_move3(int axis1,int axis2,int axis3,long pulse1,long pulse2,long pulse3,long speed);//矢量速度缓冲插补
//	int  fifo_vsinp_move4(int index,long pulse1,long pulse2,long pulse3,long pulse4,long speed);		//矢量速度缓冲插补

/*axistotal:轴数，例如两*/
	int fifo_inp_moves(int speed,int axistotal,...);

	int fifo_inp_move(unsigned char axismap,long pulse1,long pulse2,long pulse3,long pulse4,long pulse5,long pulse6,long speed);//六轴缓冲位选插补指令,对应0位为1号轴,1位为2号轴,以此类推
	
	int  get_fifo_index(int *value);

	int set_comp1(int axis,long value);																	//设置逻辑计数器可变环计数值

/*获取当前系统运动方向*/
	int  read_dir(int axis);
	


#define DirLogic_b			(0x00000001<<0)
#define PulseLogic_b		(0x00000001<<1)
#define EncoderDir_b		(0x00000001<<8)
#define LimitPlusEnable_b	(0x00000001<<12)
#define LimitMinusEnable_b	(0x00000001<<13)
#define LimitEffLevle_b		(0x00000001<<14)


//bit 0:pulse dir_logic
//bit 1:pulse logic
//bit 2:pulse mode
//bit 3:xx

//bit 4:物理轴号bit0
//bit 5:物理轴号bit1
//bit 6:物理轴号bit2
//bit 7:物理轴号bit3

//bit 8:encoder dir_logic
//bit 9:xx
//bit10:xx
//bit11:xx
//bit12:limit valid
//bit13:limit stop mode

	int get_axis_conf(int axis,int *conf);

	int set_actualcount_mode(int axis, int value,int dir,int freq);

    int set_phy_axis(int pro_axis,int phy_axis);		//设置物理轴号，如果有需要设置，那么在motion_init后要进行设置，改函数会影响所有的运动指令。


/**********************
*	void  set_sub_axis(INT8U phy_axis,INT8U pro_axis);
*	设定物理轴号对应的内部轴号，
*	phy_axis:	1~6	对应外部轴接口x,y,z,a,b,c
*	pro_axis:	0 	内部轴号和外部轴号一一对应
*				1~6	对应的外部轴号
*				7	不输出脉冲
*****************************/
void  set_sub_axis(INT8U phy_axis,INT8U pro_axis);
	
int set_gpio_out(int GPIO,int sta);
int get_gpio_out(int GPIO,int *sta);
int get_gpio_in(int GPIO,int *sta);

/**********************
*	void Set_PWM_Freq(int freq,int dutycycle)
*	用于ZM420 不同于ARM的PWM控制，该函数是控制FPGA输出PWM做DA转换使用，配合接口板ADT-9154
*
*	freqset=1/k * 16000/0.001;
*	dutycycleset=rate*freqset;
*
*****************************/
//void Set_PWM_Freq(int freq,int dutycycle);
//void Enable_FPGA_PWM(int mode);

//number 指定端口号【HY700 范围0-3】
//mode   0 正常开关量输出[默认]  1 pwm方式输出
int set_pwm_mode(int number, int mode);	//设置相应输出口的PWM模式
int set_pwm_out(int number,int freq,int dutycycle);//设置相应输出口的频率和占空比 【如果TP3540直接使用此函数number必须为0】

#define	Enable_FPGA_PWM(mode)			set_pwm_mode(0,mode)
#define	Set_PWM_Freq(freq,dutycycle)	set_pwm_out(0,freq,dutycycle)

int OpenProbe(int cardno, int pro, int output);
int CloseProbe(int cardno);
int ReadProbe(int cardno,int pro);

#endif
