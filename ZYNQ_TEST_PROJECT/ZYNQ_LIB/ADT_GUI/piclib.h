#ifndef __PICLIB_H
#define __PICLIB_H	  		  
#include "exfuns.h"
#include "picture.h"

#define PIC_FORMAT_ERR		0x27	//��ʽ����
#define PIC_SIZE_ERR		0x28	//ͼƬ�ߴ����
#define PIC_WINDOW_ERR		0x29	//�����趨����
#define PIC_MEM_ERR			0x21	//�ڴ����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif 

typedef struct
{
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������
}_lcd_dev;

//ͼƬ��ʾ�����ӿ�  
typedef struct 
{
	unsigned int(*read_point)(signed short,signed short);							//���㺯��
	void(*draw_point)(signed short,signed short,signed int);						//���㺯��
 	void(*fill)(signed short,signed short,signed short,signed short,signed int);	//��ɫ��亯��
 	void(*draw_hline)(signed short,signed short,signed short,signed int);			//��ˮƽ�ߺ���
 	void(*fillcolor)(signed short,signed short,signed short,signed short,signed int);//��ɫ���
}_pic_phy; 

//ͼ����Ϣ
typedef struct
{
	u16 lcdwidth;	//LCD�Ŀ��
	u16 lcdheight;	//LCD�ĸ߶�
	u32 ImgWidth; 	//ͼ���ʵ�ʿ�Ⱥ͸߶�
	u32 ImgHeight;
	u32 Div_Fac;  	//����ϵ�� (������8192����)
	u32 S_Height; 	//�趨�ĸ߶ȺͿ��
	u32 S_Width;
	u32	S_XOFF;	  	//x���y���ƫ����
	u32 S_YOFF;
	u32 staticx; 	//��ǰ��ʾ���ģ�������
	u32 staticy;																 	
}_pic_info;

extern _pic_info picinfo;//ͼ����Ϣ
extern _pic_phy pic_phy;
extern _lcd_dev lcddev;	//����LCD��Ҫ����

/*********************************************
������:       	piclib_alpha_blend
��Ҫ����:    	����ALPHA BLENDING�㷨.
�������:        src:Դ��ɫ
				dst:Ŀ����ɫ
				alpha:͸���̶�(0~32)
�������:        ��
����ֵ:         	 ��Ϻ����ɫ.
*********************************************/
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha);	//alphablend����



/*********************************************
������:       	ai_draw_init
��Ҫ����:    	��ʼ�����ܻ��㣬�ڲ�����
�������:        ��
�������:        ��
����ֵ:         	 ��
*********************************************/
void ai_draw_init(void);							//��ʼ�����ܻ�ͼ



/*********************************************
������:       	is_element_ok
��Ҫ����:    	�ж���������Ƿ������ʾ
�������:        (x,y) :����ԭʼ����
				chg   :���ܱ���.
�������:        ��
����ֵ:         	 0,����Ҫ��ʾ.1,��Ҫ��ʾ
*********************************************/
u8 is_element_ok(u16 x,u16 y,u8 chg);				//�ж������Ƿ���Ч



#endif

