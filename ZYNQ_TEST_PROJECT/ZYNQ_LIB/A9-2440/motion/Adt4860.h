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

 ��������:	set_axis_enable
 ��������:  ʹ�ܶ�Ӧ��,��ɷ�Ϊ������ʹ�ܺ͸�����ʹ��
 �������:
 	axis : ���[1-8]
 	dir  : ��ʾ������򸺷���
 	  0 - ͬʱ����������͸�����ʹ��
 	  1 - ����������ʹ��
 	  2 - ���ø�����ʹ��
 	  
 	sta  : ��ʾ������λ״̬
 	  0 - ʹ�ܶ�Ӧ��
 	  1 - �رն�Ӧ��
 	  
 �������:  ��
 �� �� ֵ:  
 	0 - ����
 	1 - ����
 ע������:  
   �˹�����ֻ����CNC49xx �� CNC46xx ��ʹ����Ч,���������ݲ�֧��
   
*********************************************************************/
	int  set_axis_enable(const int axis,const int dir,const int sta);
	
/*********************************************************************

 ��������:	get_axis_enable
 ��������:  ���ص�ǰ��ʹ��״̬
 �������:  ��
 �������:  ��
 ����ֵ:       
 	| bit31-bit24 | bit23-bit16 		| bit15-bit8 | bit7-bit0    		|
	| ����        | ��8-��1 ������ʹ��	| ����       | ��8-��1 ������ʹ�� 	|
	
 	��Ӧλ:
 	0 - ��ʾ��ǰ��ʹ��
 	1 - ��ʾ��ǰ��ر�
 	
 ע������:  ��
   
*********************************************************************/
	INT32U  get_axis_enable(void);


	void  motion_init(void);
	int set_pulse_mode(int axis,int value,int logic,int dir_logic);
	int set_stop0_mode(int axis,int value,int logic);
	int set_stop1_mode(int axis,int value,int logic);
	int set_limit_mode(int axis,int v1,int v2,int logic);
//----------------------------------------------------//
//               ����״̬��麯��                     //
//----------------------------------------------------//
	int get_status(int axis,int *value);
	int get_inp_status(int *value);
	int get_int_status(int *value);
//----------------------------------------------------//
//               �˶������趨����                     //
//----------------------------------------------------//
	int set_acc(int axis,long Value);
	int set_startv(int axis,long Value);
	int set_speed(int axis,long Value);
	int set_command_pos(int axis,long Value);
	int set_actual_pos(int axis,long Value);
	int continue_move(int axis,int dir);
//----------------------------------------------------//
//               �˶�������麯��                     //
//----------------------------------------------------//
	int get_command_pos(int axis,long *pos);
	int get_actual_pos(int axis,long *pos);
	int get_coder_speed(int axis,long *pos);
	int get_speed(int axis,long *pos);
	int get_startv(int axis,long *pos);
	int get_acc(int axis,long *pos);
	
//----------------------------------------------------//
//                   ��������                         //
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
//                ����������/�������                 //
//----------------------------------------------------//
						/*p1:ԭʼ�˿ڣ�p2:Ŀ��˿�*/
	int SetInPort(unsigned int p1,unsigned int p2);
	int SetOutPort(unsigned int p1,unsigned int p2);

	int read_bit(int number);
	int write_bit(int number,int value);

//----------------------------------------------------//
//                �жϺ���                            //
//----------------------------------------------------//

	void Clear_Int(void);
	void Enable_ADT834INT(void);
	void Disable_ADT834INT(void);
	//void __irq ADT834Isr(void);
	extern unsigned long  iMoveInpCount;
	
	extern OS_EVENT *gMoveStopSEM;
	
/**********************
* ��IO�жϷ���
*****************************/
	void Clear_IO_Int(void);
	void Enable_IO_Int(void);
	void Disable_IO_Int(void);
/**********************
* Num	:ѡ���ĸ�IO��Ϊ�ж�Դ
* Logic	:�ж�Դ��������1
		 �ж�Դ�½�����0
*****************************/
	int  Set_IO_Int(int Num,int logic);
	extern OS_EVENT *gIOInterruptSEM;

//----------------------------------------------------//
//				   FIFO����                           //
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
/*�汾104����ȡ��ʸ���ٶȲ岹���ܺ���*/
//	int  fifo_vsinp_move2(int axis1,int axis2,long pulse1,long pulse2,long speed);						//ʸ���ٶȻ���岹
//	int  fifo_vsinp_move3(int axis1,int axis2,int axis3,long pulse1,long pulse2,long pulse3,long speed);//ʸ���ٶȻ���岹
//	int  fifo_vsinp_move4(int index,long pulse1,long pulse2,long pulse3,long pulse4,long speed);		//ʸ���ٶȻ���岹

/*axistotal:������������*/
	int fifo_inp_moves(int speed,int axistotal,...);

	int fifo_inp_move(unsigned char axismap,long pulse1,long pulse2,long pulse3,long pulse4,long pulse5,long pulse6,long speed);//���Ỻ��λѡ�岹ָ��,��Ӧ0λΪ1����,1λΪ2����,�Դ�����
	
	int  get_fifo_index(int *value);

	int set_comp1(int axis,long value);																	//�����߼��������ɱ价����ֵ

/*��ȡ��ǰϵͳ�˶�����*/
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

//bit 4:�������bit0
//bit 5:�������bit1
//bit 6:�������bit2
//bit 7:�������bit3

//bit 8:encoder dir_logic
//bit 9:xx
//bit10:xx
//bit11:xx
//bit12:limit valid
//bit13:limit stop mode

	int get_axis_conf(int axis,int *conf);

	int set_actualcount_mode(int axis, int value,int dir,int freq);

    int set_phy_axis(int pro_axis,int phy_axis);		//����������ţ��������Ҫ���ã���ô��motion_init��Ҫ�������ã��ĺ�����Ӱ�����е��˶�ָ�


/**********************
*	void  set_sub_axis(INT8U phy_axis,INT8U pro_axis);
*	�趨������Ŷ�Ӧ���ڲ���ţ�
*	phy_axis:	1~6	��Ӧ�ⲿ��ӿ�x,y,z,a,b,c
*	pro_axis:	0 	�ڲ���ź��ⲿ���һһ��Ӧ
*				1~6	��Ӧ���ⲿ���
*				7	���������
*****************************/
void  set_sub_axis(INT8U phy_axis,INT8U pro_axis);
	
int set_gpio_out(int GPIO,int sta);
int get_gpio_out(int GPIO,int *sta);
int get_gpio_in(int GPIO,int *sta);

/**********************
*	void Set_PWM_Freq(int freq,int dutycycle)
*	����ZM420 ��ͬ��ARM��PWM���ƣ��ú����ǿ���FPGA���PWM��DAת��ʹ�ã���Ͻӿڰ�ADT-9154
*
*	freqset=1/k * 16000/0.001;
*	dutycycleset=rate*freqset;
*
*****************************/
//void Set_PWM_Freq(int freq,int dutycycle);
//void Enable_FPGA_PWM(int mode);

//number ָ���˿ںš�HY700 ��Χ0-3��
//mode   0 �������������[Ĭ��]  1 pwm��ʽ���
int set_pwm_mode(int number, int mode);	//������Ӧ����ڵ�PWMģʽ
int set_pwm_out(int number,int freq,int dutycycle);//������Ӧ����ڵ�Ƶ�ʺ�ռ�ձ� �����TP3540ֱ��ʹ�ô˺���number����Ϊ0��

#define	Enable_FPGA_PWM(mode)			set_pwm_mode(0,mode)
#define	Set_PWM_Freq(freq,dutycycle)	set_pwm_out(0,freq,dutycycle)

int OpenProbe(int cardno, int pro, int output);
int CloseProbe(int cardno);
int ReadProbe(int cardno,int pro);

#endif
