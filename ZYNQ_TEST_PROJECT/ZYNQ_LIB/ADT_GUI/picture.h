/*
 * picture.h
 *
 *  Created on: 2016年12月22日
 *      Author: Administrator
 */

#ifndef PICTURE_H_
#define PICTURE_H_


/*********************************************
函数名:       	piclib_init
主要功能:    	画图初始化函数,在画图之前,必须先调用此函数
输入参数:        无
输出参数:        无
返回值:         	 无
*********************************************/
void piclib_init(void);


/***********************************************************
函数名:       	ai_load_picfile
主要功能:    	智能画图函数
输入参数:        FileName:要显示的图片文件  BMP/JPG/JPEG/GIF
				x,y,width,height:坐标及显示区域尺寸

				GIF图片无缩减功能，根据图片实际大小（1:1）显示，若参数区域小于图片的实际大小，则会返回PIC_WINDOW_ERR
				其他图片可以根据区域大小进行缩减，显示在参数（x,y,width,height）区域的中间位置
输出参数:        无
返回值:         	0:     显示成功
                1~19:  文件打开错误
				PIC_FORMAT_ERR		0x27	//格式错误
				PIC_SIZE_ERR		0x28	//图片尺寸错误
				PIC_WINDOW_ERR		0x29	//窗口设定错误
				PIC_MEM_ERR			0x21	//内存错误

***********************************************************/
u8 ai_load_picfile(const u8 *filename,u16 x,u16 y,u16 width,u16 height);//智能画图


/***********************************************************
函数名:       	jpg_decode
主要功能:    	jpg/jpeg显示函数
输入参数:        FileName:要显示的图片文件  JPG/JPEG
				x,y,width,height:坐标及显示区域尺寸

				图片可以根据区域大小进行缩减，显示在参数（x,y,width,height）区域的中间位置
输出参数:        无
返回值:         	0:     显示成功
                1~19:  文件打开错误
				PIC_FORMAT_ERR		0x27	//格式错误
				PIC_SIZE_ERR		0x28	//图片尺寸错误
				PIC_WINDOW_ERR		0x29	//窗口设定错误
				PIC_MEM_ERR			0x21	//内存错误
***********************************************************/
u8 jpg_decode(const u8 *filename,u16 xx,u16 yy,u16 width,u16 height);


/***********************************************************
函数名:       	gif_decode
主要功能:    	gif显示函数
输入参数:        FileName:要显示的图片文件  JPG/JPEG
				x,y,width,height:坐标及显示区域尺寸

				根据图片实际大小（1:1）显示，若参数区域小于图片的实际大小，则返回PIC_WINDOW_ERR
				若参数区域大于图片的实际大小，则显示在参数（x,y,width,height）区域的中间位置
输出参数:        无
返回值:         	0:     显示成功
                1~19:  文件打开错误
				PIC_FORMAT_ERR		0x27	//格式错误
				PIC_SIZE_ERR		0x28	//图片尺寸错误
				PIC_WINDOW_ERR		0x29	//窗口设定错误
				PIC_MEM_ERR			0x21	//内存错误
***********************************************************/
u8 gif_decode(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 rate);//在指定区域解码一个GIF文件.


/***********************************************************
函数名:       	stdbmp_decode
主要功能:    	bmp显示函数
输入参数:        FileName:要显示的图片文件  bmp
				x,y,width,height:坐标及显示区域尺寸

				图片可以根据区域大小进行缩减，显示在参数（x,y,width,height）区域的中间位置
输出参数:        无
返回值:         	0:     显示成功
                1~19:  文件打开错误
				PIC_FORMAT_ERR		0x27	//格式错误
				PIC_SIZE_ERR		0x28	//图片尺寸错误
				PIC_WINDOW_ERR		0x29	//窗口设定错误
				PIC_MEM_ERR			0x21	//内存错误
***********************************************************/
u8 bmp_decode(const u8 *filename,u16 xx,u16 yy,u16 width,u16 height);














#endif /* PICTURE_H_ */
