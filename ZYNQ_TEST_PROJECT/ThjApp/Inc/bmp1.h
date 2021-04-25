# ifndef 	___BMP_H___
# define 	___BMP_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        bmp.h
  创建人：       范志州
  创建日期：		2005/08/15
  主要功能描述:  负责16色以及256色位图的显示

  修改记录：
   1.  ...
*************************************************/

# include 	<stdio.h>
# include 	<stdlib.h>
# include 	"os_cpu.h"
# include 	"rect.h"

/*************************************************
  函数名:     	 CBmp_Init
  主要功能:    	 位图处理相关数据初始化
  输入参数:       无
  输出参数:       无
  返回值:         无
*************************************************/
void	CBmp_Init(void);

/*************************************************
  函数名:     	 CBmp_Destroy
  主要功能:    	 位图处理相关数据释放，包括位图文件的关闭
  输入参数:       无
  输出参数:       无
  返回值:         无
*************************************************/
void	CBmp_Destroy(void);

/*************************************************
  函数名:     	 CBmp_SetRect
  主要功能:    	 设定位图显示时显示的位置（保留暂时未用）
  输入参数:       rect:	设定的范围
  输出参数:       无
  返回值:         无
*************************************************/
void 	CBmp_SetRect(CRect rect);//设置显示范围
	
/*************************************************
  函数名:     	 CBmp_ReadBmpFile
  主要功能:    	 打开并读取指定的位图文件
  输入参数:       filename:	要读取的位图文件名
  输出参数:       无
  返回值:         正确读取文件返回TRUE，否则返回FALSE
*************************************************/
BOOL 	CBmp_ReadBmpFile(INT8S *filename);//读取指定文件BMP类型

/*************************************************
  函数名:     	 CBmp_View
  主要功能:    	 显示位图到指定的位置
  输入参数:       X:	指定位图显示的水平位置， -1表示居中显示
  				 Y: 指示位图显示的垂直位置， -1表示居中显示
  输出参数:       无
  返回值:         如果存在需要显示文件显示并返回TRUE，否则返回FALSE
*************************************************/
BOOL 	CBmp_View(INT16S x,INT16S y);//显示位置,相对于显示范围

/*************************************************
  函数名:     	 CBmp_Put01
  主要功能:    	 显示单色位图到指定的位置
  输入参数:      X:   指定位图显示的水平位置
				 Y:   指示位图显示的垂直位置
  输出参数:      无
  返回值:        无
*************************************************/
void 	CBmp_Put01(INT16S x,INT16S y);

//void	CBmp_GetFullScr24Bit(char *filename);
//void	CBmp_GetFullScr24Bit(char *filename,INT16S isVirtual);
void	CBmp_GetFullScrMono(char *filename);
void	CBmp_DispXMSBmp(INT16S x,INT16S y,INT16U handle);
/*************************************************
  函数名:     	 CBmp_GetWidth
  主要功能:    	 获取当前显示位图的宽度
  输入参数:       无
  输出参数:       无
  返回值:         当前显示位图宽度
*************************************************/
INT16S 	CBmp_GetWidth(void);

/*************************************************
  函数名:     	 CBmp_GetHeight
  主要功能:    	 获取当前显示位图的高度
  输入参数:       无
  输出参数:       无
  返回值:         当前显示位图高度
*************************************************/

INT16S 	CBmp_GetHeight(void);
char	CreatBMP(void *filename,INT16S Wide,INT16S Hight,INT16S color);

INT32S 	InitDispBMP(void *filename);

INT8U DispBMP(void *filename,char color,int x,int y);


BOOL  DispWords(int Num,int x,int y);
BOOL  DispWordsH(int Num,int x,int y);
BOOL  DispWordsS(int Num,int x,int y);


#endif
