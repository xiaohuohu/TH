#ifndef __PICLIB_H
#define __PICLIB_H	  		  
#include "exfuns.h"
#include "picture.h"

#define PIC_FORMAT_ERR		0x27	//格式错误
#define PIC_SIZE_ERR		0x28	//图片尺寸错误
#define PIC_WINDOW_ERR		0x29	//窗口设定错误
#define PIC_MEM_ERR			0x21	//内存错误
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif 

typedef struct
{
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
}_lcd_dev;

//图片显示物理层接口  
typedef struct 
{
	unsigned int(*read_point)(signed short,signed short);							//读点函数
	void(*draw_point)(signed short,signed short,signed int);						//画点函数
 	void(*fill)(signed short,signed short,signed short,signed short,signed int);	//单色填充函数
 	void(*draw_hline)(signed short,signed short,signed short,signed int);			//画水平线函数
 	void(*fillcolor)(signed short,signed short,signed short,signed short,signed int);//颜色填充
}_pic_phy; 

//图像信息
typedef struct
{
	u16 lcdwidth;	//LCD的宽度
	u16 lcdheight;	//LCD的高度
	u32 ImgWidth; 	//图像的实际宽度和高度
	u32 ImgHeight;
	u32 Div_Fac;  	//缩放系数 (扩大了8192倍的)
	u32 S_Height; 	//设定的高度和宽度
	u32 S_Width;
	u32	S_XOFF;	  	//x轴和y轴的偏移量
	u32 S_YOFF;
	u32 staticx; 	//当前显示到的ｘｙ坐标
	u32 staticy;																 	
}_pic_info;

extern _pic_info picinfo;//图像信息
extern _pic_phy pic_phy;
extern _lcd_dev lcddev;	//管理LCD重要参数

/*********************************************
函数名:       	piclib_alpha_blend
主要功能:    	快速ALPHA BLENDING算法.
输入参数:        src:源颜色
				dst:目标颜色
				alpha:透明程度(0~32)
输出参数:        无
返回值:         	 混合后的颜色.
*********************************************/
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha);	//alphablend处理



/*********************************************
函数名:       	ai_draw_init
主要功能:    	初始化智能画点，内部调用
输入参数:        无
输出参数:        无
返回值:         	 无
*********************************************/
void ai_draw_init(void);							//初始化智能画图



/*********************************************
函数名:       	is_element_ok
主要功能:    	判断这个像素是否可以显示
输入参数:        (x,y) :像素原始坐标
				chg   :功能变量.
输出参数:        无
返回值:         	 0,不需要显示.1,需要显示
*********************************************/
u8 is_element_ok(u16 x,u16 y,u8 chg);				//判断像素是否有效



#endif

