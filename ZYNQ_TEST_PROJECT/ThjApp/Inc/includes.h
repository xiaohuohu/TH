#ifndef   __INCLUDES_H__
#define   __INCLUDES_H__

//标准头文件
#include    <stdio.h>
#include 	<stdarg.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include 	<math.h>
#include  	<locale.h>		//让sprintf函数能处理浮点数 setlocale(LC_ALL,"C");   //让sprintf函数能处理浮点数，在<locale.h>中


// ZYNQ_LIB 头文件

//OS
#include    "ucos_ii.h"
#include    "os_cpu.h"
#include    "os_cfg.h"
//#include 	"target.h"

//平台
//#include	"A9_2440addr.h"
//#include	"nand.h"
//#include 	"usb.h"
//#include	"part.h"
//#include 	"2410IIC.h"
//#include	"option.h"
#include   "A9-2440common.h"
#include	"LibFunc.h"
#include 	"adt_datatype.h"
#include 	"system.h"
#include	"CTimer.h"

//MOTION
#include	"adt4860.h"

//LCD
#include	"cdc.h"
#include	"lcdlib.h"
#include	"rect.h"
#include	"point.h"
//#include	"bmp.h"

//FS
#include	"ff.h"
#include 	"mmu.h"

//#include 	"fs_api.h"  //
//#include	"yaffsfs.h"
//#include 	"yaffs_guts.h"
//#include 	"sd.h"
//#include 	"sdcard.h"

//KEY
//#include	"keylib.h"
#include	"key.h"
//定时器
#include 	"rtc/rtc.h"
//串口
#include	"uart/uart.h"
//WLIP
#include 	"cc.h"
#include 	"ip_addr.h"
#include	"def.h"
//USB
#include		"usb_device.h"
#include		"usb_host.h"



// INC头文件
#include	"afxmsg.h"
#include	"base.h"
#include	"bmp1.h"
#include	"cbutton.h"
#include	"cedit.h"
#include	"control.h"
//#include	"ctimer.h"
#include	"ctrlcard.h"
#include	"data.h"
#include	"DISPdir.h"

#include	"dispmsg.h"
#include	"disptime.h"
#include	"dog.h"
//#include	"filefrm.h"
#include	"ilist.h"
#include 	"InputValueMap.h"
#include    "keyboard.h"
#include	"Language.h"
#include	"ledout.h"
//#include	"machpara.h"
#include	"mainfrm.h"
#include	"MergePack.h"
#include	"msgdlg.h"
#include	"netset.h"
#include	"paraset.h"
#include	"password.h"
#include	"runtask.h"
#include	"settime.h"
#include	"setdog.h"
#include	"status.h"
#include	"StrTool.h"
#include	"syspara.h"

#include	"techfrm.h"
#include	"testfrm.h"
#include	"testkey.h"
#include	"userdog.h"
#include	"work.h"
//#include	"xms.h"  无用

//#include 	"Hzk48Code.h"

//#include    "nat.h"
#include	"keyinfo.h"
//#include	"telcom.h"

//#include	"graphics.h"


//VMotion 头文件
#include 	"THMotion.h"

#define 	PROJECT_ID	"DB016A027B"
#define		VER			101
//#define		PRINTBMP

#define		DISPSTYLE	1														//字符显示对齐模式（0：居中 2：左对齐 1：右对齐）
#define		EDITHEIGHT  40														//主界面显示框高度
#define		EDITWIDTH	345														//主界面显示框宽度
#define		EDITLEFT	43														//主界面显示框X起点
#define		EDITTOP		23														//主界面显示框Y起点
#define		DISTANCE	20														//显示框之间的距离




#endif
