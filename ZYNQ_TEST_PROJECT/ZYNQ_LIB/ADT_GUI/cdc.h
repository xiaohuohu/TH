/*
 * CDC.H
 *
 *  Created on: 2014-2-11
 *      Author: Administrator
 */

#ifndef CDC_H_
#define CDC_H_

// -- INCLUDES -- //
#include "adt_datatype.h"
#include "rect.h"
#include "point.h"

#define GUI_LIB_VER			106
#define GUI_LIB_NAME   		"LCDGUI_LIB"
#define GUI_LIB_BUS_NAME	"LCDGUI_BUS_LIB"

#define GUI_LIB_SUFFIX 		"Release"

#define	DARKBROWNCOLOR1		0x7f1f00	 //棕红
#define	DARKBROWNCOLOR		0x7f3f00	 //深棕
//#define	DARKBROWNCOLOR		0x7f3f00	 //深棕

#define	REDCOLOR		0xff0000     //红
#define	GREENCOLOR		0x00ff00     //绿
#define	BLUECOLOR		0x0000ff     //蓝
#define	BLACKCOLOR		0x000000     //黑
#define	GRAYCOLOR		0xa0a0a0     //灰
#define	CYANCOLOR		0x00ffff     //青
#define	WHITECOLOR		0xffffff     //白
#define	YELLOWCOLOR		0xffff00     //黄
#define	MAGENTACOLOR	0x8B008B     //洋红
#define	BROWNCOLOR		0x4a4613
#define	PINKCOLOR		0xff00ff     //粉色

#define	LIGHTREDCOLOR	0xff8080
#define	LIGHTGREENCOLOR	0x20ff00
#define	LIGHTBLUECOLOR	0x779FEA
#define	LIGHTCYANCOLOR	0xffff80
#define	LIGHTGRAYCOLOR	0xD3D3D3	 //浅灰
#define	LIGHTBLACKCOLOR 0x202020	 //浅黑

#define	DARKBLUECOLOR	0x004080     //深蓝
#define	DARKGRAYCOLOR	0x808080	 //深灰
#define	DARKGREENCOLOR	0x008020     //深绿1
#define DARKGREENCOLOR2	0x008060     //深绿2
#define	DARKYELLOWCOLOR 0xD2C919

#define		TEXT_HEIGHT  16		//默认西文字符高度
#define	 	TEXT_WIDTH   8		//默认西文字符宽度

# define 	CLIP_ON 	1   // 截边操作标志，暂时未用
# define	CLIP_OFF	0

#define	CDC_Power		Lcd_Power


INT16S		getmaxx(void);
INT16S		getmaxy(void);

INT16U	Gui_Lib_Ver(void); 			/* 获取GUI开发库版本 */
void	Gui_Lib_Name(INT8S *str);	/* 获取库版本和名称  */

/******************************************************************************
	Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.

	文件名:		cdc.h
	创建者:		范志州
	创建日期:	2005/10/13
	版本:		1.0

	主要功能描述:    此文件主要负责屏幕绘图操作，包括画点，画线，画矩形框，画圆，
			画对话框，画按钮，显示字体以及进行屏幕COPY等的相关操作。
	修改记录:
		1.
*******************************************************************************/


/********************************************
函数名:		Init
主要功能:	对LCD显示进行初始化
输入参数:
		-2    只初始化CDC内部资源，不初始化LCD控制器(建议使用此参数，BIOS到应用程序图像可平滑过渡，没有黑屏)
		-1    库根据机器类型自动选择
		>=0 请参考lcdlib.h中Resolutions定义

输出参数:		无
返回值:		无
注意：		应用程序如想使用自定义配置或不初始化LCD控制器，
			需将mode配为-2
********************************************/
void	CDC_Init(int mode);

/********************************************
函数名:		Desstroy
主要功能:	释放LCD显示资源
输入参数:	无
输出参数:	无
返回值:		无
********************************************/
void	CDC_Destroy(void);

/*********************************************
函数名:       	CDC_LOCKSRC,CDC_UNLOCKSRC,CDC_GETLOCKSRCSTA
主要功能:    	锁住屏幕,改为后台显示，在解锁后快速刷新显示
输入参数:        无
输出参数:        无
返回值:         	无
*********************************************/
void CDC_LOCKSRC(void);
void CDC_UNLOCKSRC(void);
INT16U CDC_GETLOCKSRCSTA(void);

/*********************************************
函数名:       	InvertArea_Cn()
主要功能:    	对指定区域取反
输入参数:       (x,y,ws,hs决定的取反的显示区域)
输出参数:		无
返回值:         无
*********************************************/
void InvertArea_Cn(INT16S x,INT16S y,INT16S ws,INT16S hs);

/*********************************************
函数名:       	Lock
主要功能:    	LCD显示信号量获取函数，内部使用，返回TRUE时可以使用CDC_开始的绘图函数
输入参数:       无
输出参数:		无
返回值:         无
*********************************************/
BOOL	CDC_Lock(void);

/*********************************************
函数名:       	UnLock
主要功能:    	复位（释放）信号量函数
输入参数:       无
输出参数:		无
返回值:         无
*********************************************/
void	CDC_UnLock(void);

/*********************************************
函数名:       	SaveScreen
主要功能:    		保存当前显示内容到内存中
输入参数:       	无
输出参数:			无
返回值:        	无
*********************************************/
void	CDC_SaveScreen(void);

/*********************************************
函数名:       	RestoreScreen
主要功能:    	恢复刚才在SaveScreen中保存的显示内容到屏幕上
输入参数:       无
输出参数:		无
返回值:         无
*********************************************/
void	CDC_RestoreScreen(void);

/*********************************************
函  数  名:    	SaveScreenXY
主要功能:    	保存当前显示内容(x,y,w,h决定的显示区域)到内存中
输入参数:		x,y 保存屏幕起始位置坐标
			w,h 保存屏幕矩形区域的宽和高
输出参数:		无
返  回  值:    无
*********************************************/
void	CDC_SaveScreenXY(INT16S x,INT16S y,INT16S w,INT16S h);

/*********************************************
函数名:      RestoreScreenXY
主要功能:    	恢复刚才在SaveScreenXY中保存的显示内容到屏幕上,恢复位置大小为x,y,w,h指定的块
输入参数:		x,y 恢复屏幕起始位置坐标
			w,h 恢复屏幕矩形区域的宽和高
输出参数:		无
返回值:      无
*********************************************/
void	CDC_RestoreScreenXY(INT16S x,INT16S y,INT16S w,INT16S h);

/*----------------------------------------------------------------------------------------------*/
/*----------------------------------------  画点和着色部分 ----------------------------------------*/

/*********************************************
函数名:       	PutPoint
主要功能:    	放置象素点
输入参数:        x,y--------放置象素点的位置坐标
				color------象素点的颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_PutPoint(INT16S x,INT16S y,INT32S color);
/*********************************************
函数名:       	PutPixel
主要功能:    	放置象素点,同上一函数，其目的是为了与graphics.lib函数名兼容
输入参数:        x,y--------放置象素点的位置坐标
				color------象素点的颜色
输出参数:        无
返回值:			无
*********************************************/
void	CDC_PutPixel(INT16S x,INT16S y,INT32S color);
void	CDC_PutPixel2(CPoint point,INT32S color);
/*********************************************
函数名:       	GetPixel
主要功能:    	获取象素点的颜色
输入参数:        x,y--------象素点的位置坐标
输出参数:        无
返回值:          x,y位置象素点的颜色值
*********************************************/
INT32U		CDC_GetPixel(INT16S x,INT16S y);
INT32U		CDC_GetPixel2(CPoint point);

/*********************************************
函数名:       	PutColorBar
主要功能:    	绘制Bar（填充矩形）图形
输入参数:        x,y,w,h-------需绘制Bar大小
				color---------Bar的填充颜色
				style---------Bar的填充类型
							  0,1:在矩形框内以指定颜色填充，填充方法为绘点的方式
							  100:同0,1的填充方式相同，但填充速度较快(对于需要较大填充区域时)
							  其它值同0,1方式相同
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_PutColorBar(INT16S x,INT16S y,INT16S w,INT16S h,INT32S color,INT16S style);
void	CDC_PutColorBar2(CRect rect,INT32S color,INT16S style);
/*********************************************
函数名:       	Fillellipse
主要功能:    	绘制填充椭圆，暂时只支持圆的绘制，即长短轴一样的情况
输入参数:        x,y--------椭圆的中心坐标
				xr,yr------椭圆的X轴半径，Y轴半径
				color------椭圆的填充颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_Fillellipse(INT16S x,INT16S y,INT16S xr,INT16S yr,INT32S color);
/*********************************************
函数名:       	Fillellipse
主要功能:    	绘制填充椭圆，暂时只支持圆的绘制，即长短轴一样的情况
输入参数:        point------椭圆的中心坐标
				xr,yr------椭圆的X轴半径，Y轴半径
				color------椭圆的填充颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_Fillellipse2(CPoint point,INT16S xr,INT16S yr,INT32S color);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画线部分 -------------------------------------------*/
/*********************************************
函数名:       	Line
主要功能:    	两点直接画直线
输入参数:        x1,y1------直线的起点位置
				x2,y2------直线的终点位置
				color------直线的颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_Line(INT16S x1,INT16S y1,INT16S x2,INT16S y2,INT32S color);
void	CDC_Line2(CPoint start,CPoint end,INT32S color);

/*********************************************
函数名:       	LineTo
主要功能:    	画直线到指定的位置
输入参数:        x,y--------画线到的位置
				color------线的颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_LineTo(INT16S x,INT16S y,INT32S color);
void	CDC_LineTo2(CPoint point,INT32S color);

/*********************************************
函数名:       	MoveTo
主要功能:    	移动绘图点到指定的位置
输入参数:       x,y--------移动到的位置
输出参数:       无
返回值:        	无
*********************************************/
void	CDC_MoveTo(INT16S x,INT16S y);
void	CDC_MoveTo2(CPoint point);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画圆（圆弧）部分 ------------------------------------*/
/*********************************************
函数名:       	Arc
主要功能:    	绘制圆弧（包括圆）
输入参数:		ox,oy------圆弧的圆心位置，使用浮点数是为了在内部处理除法
				r----------圆弧半径
				start,end--圆弧的开始角和结束角（0～360）
				dir--------圆弧的绘制方向
				color------圆弧颜色
输出参数:        无
返回值:         	无
*********************************************/
void    CDC_Arc(FP32 ox, FP32 oy, FP32 r,FP32 start, FP32 end, INT16S dir,INT32S color);
/*********************************************
函数名:       	CompArc
主要功能:    	计算整个圆弧的最大最小位置坐标
输入参数:        ox,oy------圆弧的圆心位置，使用浮点数是为了在内部处理除法
				r----------圆弧半径
				start,end--圆弧的开始角和结束角（0～360）
				dir--------圆弧的绘制方向
输出参数:        limit------整个圆弧的最大最小位置指针，limit[0],limit[1]为
		X的最大最小位置，limit[2],limit[3]为Y的最大最小位置
返回值:         	无
*********************************************/
void 	CDC_CompArc(FP32 ox, FP32 oy, FP32 r,FP32 start, FP32 end, INT16S dir,FP32 *limit);

/*********************************************
函数名:       	Circle
主要功能:    	绘制圆
输入参数:       	ox,oy------圆心位置
				r----------圆的半径
				color------圆的颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_Circle(INT16S ox,INT16S oy,INT16S r,INT32S color);
void	CDC_Circle2(CPoint point,INT16S r,INT32S color);
/*********************************************
函数名:       	Ellipse
主要功能:    	绘制椭圆
输入参数:      	x,y--------椭圆中心位置
				xr,yr------椭圆的X,Y半径
				color------椭圆的颜色
输出参数:       无
返回值:        	无
*********************************************/
void	CDC_Ellipse(INT16S x,INT16S y,INT16S xr,INT16S yr,INT32S color);
void	CDC_Ellipse2(CPoint point,INT16S xr,INT16S yr,INT32S color);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画按钮部分 -----------------------------------------*/
/*********************************************
函数名:       	DrawButton
主要功能:    	绘制按钮
输入参数:        rect----按钮矩形框大小
				color--按钮颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_DrawButton(CRect  rect,INT32S color);
void	CDC_DrawTButton(CRect   rect,INT32S color );
/*********************************************
函数名:       	PressButton
主要功能:    	绘制按钮按下时的形状
输入参数:        rect----按钮矩形框大小
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_PressButton(CRect m_rect);
/*********************************************
函数名:       	RlsButton
主要功能:    	绘制按钮释放时的形状
输入参数:       	rect----按钮矩形框大小
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_RlsButton(CRect	m_rect);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画框部分 -------------------------------------------*/
/*********************************************
函数名:       	DrawBox
主要功能:    	绘制编辑框
输入参数:       	rect-------需绘制编辑框大小
				color------编辑框的填充颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_DrawEdit(CRect	rect,INT32S color);
/*********************************************
函数名:       	DrawBox
主要功能:    	绘制BOX图形
输入参数:       	rect-------需绘制BOX大小
				color------BOX的填充颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_DrawBox(CRect	m_rect,INT32S color);
/*********************************************
函数名:       	DrawRect
主要功能:    	绘制矩形框
输入参数:       	rect-------需绘制矩形框大小
				color------矩形框的填充颜色
				edgecolor--矩形框的边界颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_DrawRect(CRect m_rect,INT32S color,INT32S edgecolor);
/*********************************************
函数名:       	Rectangle
主要功能:    	绘制非填充之矩形框
输入参数:        rect-------矩形框大下
				color------矩形框颜色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_Rectangle2(CRect rect,INT32S color);
void	CDC_Rectangle(INT16S x1,INT16S y1,INT16S x2,INT16S y2,INT32S color);
/*********************************************
函数名:       	SetRectangle
主要功能:    	绘制静态矩形框
输入参数:        x,y-------矩形框起始位置
				w,h-------矩形框的宽度，长度
				string----矩形框标题（左上角显示内容）
				color-----矩形框标题的背景色
				style-----矩形框标题填充风格（暂时只有1有效）
输出参数:        无
返回值:         	无
*********************************************/
void 	CDC_SetRectangle(INT16S x,INT16S y,INT16S w,INT16S h,INT8S *string,INT32S color,INT16S style);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画窗口部分 -----------------------------------------*/
/*********************************************
函数名:       	DrawWin
主要功能:    	绘制对话框
输入参数:       x,y----对话框左上角位置
				w,h----对话框的宽度和高度
				title--对话框标题内容
输出参数:       无
返回值:         无
*********************************************/
void	CDC_DrawWin(INT16S x,INT16S y,INT16S w,INT16S h,INT8S *title);
/*********************************************
函数名:       	DrawWin
主要功能:    	绘制对话框（带颜色跟填充类型）
输入参数:       	x,y----对话框左上角位置
				w,h----对话框的宽度和高度
				title--对话框标题内容
				color--对话框主体颜色
				style--对话框背景色的填充类型，暂时只有1有效
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_DrawWin2(INT16S x,INT16S y,INT16S w,INT16S h,INT8S *title,INT32S color,INT16S style);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 画布部分 -------------------------------------------*/
/*********************************************
函数名:       	Width
主要功能:    	获取当前画布的宽度
输入参数:        无
输出参数:		无
返回值:         	返回当前画布的宽度
*********************************************/
INT16S	CDC_Width(void);
/*********************************************
函数名:       	Height
主要功能:    	获取当前画布的高度
输入参数:        无
输出参数:		无
返回值:         	返回当前画布的高度
*********************************************/
INT16S	CDC_Height(void);
/*********************************************
函数名:       	InView
主要功能:    	判断一点是否在画布之内
输入参数:        point-----需要判断的位置
输出参数:	无
返回值:         	无
*********************************************/
BOOL	CDC_InView(CPoint point);
/*********************************************
函  数  名:       	ClearScreen
主要功能:		清除整个画布颜色为指定颜色
输入参数:        color-----清除整个画布为该颜色
输出参数:		无
返  回  值:        无
*********************************************/
void	CDC_ClearScreen(INT32S color);
/********************************************
函数名:		SetRect
主要功能:	设置画布区域(相对于全屏幕)，保留暂时未用
输入参数:	rect---画布区域矩形框
输出参数:	无
返回值:		无
********************************************/
void	CDC_SetRect(INT16S x1,INT16S y1,INT16S x2,INT16S y2);
void	CDC_SetRect2(CRect rect);

/*----------------------------------------------------------------------------------------------*/
/*------------------------------------------- 文字显示部分 ---------------------------------------*/
typedef enum ZKTYPE{
	HZK20X20_GBK2313,		//汉字库
	AZK20X20_ASCII,			//ascii 字库
	MZK24X24_UNICODE,		//多国语言字库
	MZK20X20_UNICODE,		//多国语言字库
	MZK16X16_UNICODE		//多国语言字库

}T_ZKTYPE;
	/*********************************************
	函数名:       	CDC_LoadZKLIB()
	主要功能:    	字库加载
	输入参数:       (对应的字库类型,并提供加载后的内存地址给CDC)
	输出参数:		无
	返回值:         无
	*********************************************/
INT16U CDC_LoadZKLIB(T_ZKTYPE type,INT8U *pZKLIB);

/*********************************************
函数名:       	AsciiChar
主要功能:    	快速显示单个8×16点阵字符
输入参数:        x,y--------显示字符的起点位置
		ch---------要显示的字符
		color------字符的颜色
		bk---------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_AsciiChar(INT16S x,INT16S y, INT8S ch,INT32S color,INT32S bk);

/*********************************************
函数名:       	OutText
主要功能:    	显示8×16点阵字符串，汉字为16×16
输入参数:       	x,y--------显示字符串的起点位置
				string-----要显示的字符串
				color------字符的颜色
				bk---------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_OutText(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void 	CDC_OutText2(CPoint point,INT8S *string,INT32S color,INT32S bk);

/*********************************************
函数名:       	OutCenText
主要功能:    	居中显示8×16点阵字符串，汉字为16×16
输入参数:        x1,y--------显示字符串的起点位置
				x2----------显示字符串的X终点位置
				string------要显示的字符串
				color-------字符的颜色
				bk----------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_OutCenText(INT16S x1,INT16S x2,INT16S y,INT8S *string,INT32S color,INT32S bk);
void	CDC_OutCenText2(CRect rect,INT8S *string,INT32S color,INT32S bk);

/*********************************************
函数名:       	OutText24
主要功能:    	显示16×24点阵字符串，汉字为24×24
输入参数:        x,y--------显示字符串的起点位置
				string-----要显示的字符串
				color------字符的颜色
				bk---------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:         	无
*********************************************/
void	CDC_OutText24(INT16S x,INT16S y,char *string,INT32S color, INT32S bk);
void	CDC_OutText32(INT16S x,INT16S y,char *string,INT32S color, INT32S bk);	//note: 只支持数字和大写英文字符显示
void	CDC_OutText20(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);	//需要外置字库支持,要调用CDC_LoadZKLIB进行字库加载

/*********************************************
函数名:       	CDC_OutText_24
主要功能:    	显示16×24点阵字符串，汉字为24×24
输入参数:        x,y--------显示字符串的起点位置
				 string-----要显示的字符串
				 color------字符的颜色
				 bk---------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:          无
*********************************************/
void CDC_OutText_24(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void CDC_OutText_20(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void CDC_OutText_16(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);

/*********************************************
函数名:       	CDC_OutText_UTF
主要功能:    	显示n×n点阵字符串
输入参数:        x,y--------显示字符串的起点位置
				string-----要显示的字符串
				color------字符的颜色
				bk---------字符的背景色,-1表示不刷新背景色
				zktype-----字库类型
							MZK24X24_UNICODE,		//多国语言字库
							MZK20X20_UNICODE,		//多国语言字库
							MZK16X16_UNICODE		//多国语言字库
							..<2~9>					//自定义字库序号，最多可以用8种字库

输出参数:        无
返回值:          无
*********************************************/
void CDC_OutText_UTF(INT16S x,INT16S y,char *string,INT32S color,INT32S bk,int zktype);

/*********************************************
函数名:       	CDC_OutCRText_24 UNICODE字符显示函数带自动换行
主要功能:    	居中显示12×24点阵字符串，汉字为24×24
输入参数:
				x,y--------显示字符串的起点位置
				width----------显示字符串的画板宽度
				string------要显示的字符串
				color-------字符的颜色
				bk----------字符的背景色,-1表示不刷新背景色
输出参数:        无
返回值:         	无
*********************************************/
void CDC_OutCRText_16(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);
void CDC_OutCRText_20(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);
void CDC_OutCRText_24(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);


/*********************************************
函数名:       	CDC_GetUCSTextWidth UNICODE字符宽度函数
主要功能:    	计算UCS字符的总宽度
输入参数:
				string------要显示的字符串
				zktype------字库类型：
								MZK24X24_UNICODE,		//多国语言字库
								MZK20X20_UNICODE,		//多国语言字库
								MZK16X16_UNICODE		//多国语言字库

输出参数:        无
返回值:         像素宽度
*********************************************/
int	CDC_GetUCSTextWidth(char *string,int zktype);


/****************************************************************/
/*																*/
/*				中断服务程序中可使用以下函数显示字体				*/
/*																*/
/****************************************************************/
void CDC_OutText_UTF_Intrpt(INT16S x,INT16S y,char *string,INT32S color,INT32S bk,int zktype);

void CDC_OutText_24_Intrpt(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void CDC_OutText_20_Intrpt(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void CDC_OutText_16_Intrpt(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);

void CDC_OutCRText_16_Intrpt(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);
void CDC_OutCRText_20_Intrpt(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);
void CDC_OutCRText_24_Intrpt(INT16S x,INT16S y,INT16S width,char *string,INT32S color,INT32S bk);


void CDC_OutText_Intrpt(INT16S x,INT16S y,INT8S *string,INT32S color,INT32S bk);
void CDC_OutText2_Intrpt(CPoint point,INT8S *string,INT32S color,INT32S bk);
void CDC_OutCenText_Intrpt(INT16S x1,INT16S x2,INT16S y,INT8S *string,INT32S color,INT32S bk);
void CDC_OutCenText2_Intrpt(CRect rect,INT8S *string,INT32S color,INT32S bk);
void CDC_AsciiChar_Intrpt(INT16S x,INT16S y, INT8S ch,INT32S color,INT32S bk);


void CDC_OutText20_Intrpt(INT16S x,INT16S y,char *string,INT32S color,INT32S bk);
void CDC_OutText24_Intrpt(INT16S x,INT16S y,INT8S *string,INT32S color, INT32S bk);
void CDC_OutText32_Intrpt(INT16S x,INT16S y,  INT8S *string,INT32S color,INT32S bk);


#endif /* CDC_H_ */
