#ifndef __FACECOLORSTYLE_H__
#define __FACECOLORSTYLE_H__
# include	"os_cpu.h"
//#include 	"rtcapi.h"
#include    "cdc.h"
#define SKYBLUECOLOR	0X004080 //蔚蓝
#define DARKGREENCOLOR4	0x137D5A
#define DARKGRAYCOLOR2	0x505050 //深灰


typedef struct FaceColorStyle
{
	INT32S fcsTextColor1 ;
	INT32S fcsTextColor2 ;
	INT32S fcsTextColor3 ;
	INT32S fcsTextColor4 ; 
	INT32S fcsTextColor5 ;
	INT32S fcsTextColor6 ;
	INT32S fcsTextColor7 ;
	INT32S fcsTextColor8 ;
	INT32S fcsTextColor9 ;
	INT32S fcsTextColor10 ;
	INT32S fcsTextColor11 ;
	
	INT32S fcsColumnBk;
	INT32S fcsTableLine ;			//主界面表格边线      
	INT32S fcsTechfrmBk ;			//主界面表格以外的底色
	INT32S fcsTitleBk ; 			//列表框列头文字底色，"10 X 2" 中乘号的颜色、16个小圈圈的颜色（探针气缸状态）
	INT32S fcsTableBk ; 			//主界面表格底色

}FACE_CLR_STYLE;


void SetFCS(INT16S mode);









#endif
