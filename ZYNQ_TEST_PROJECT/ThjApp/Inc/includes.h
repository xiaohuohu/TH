#ifndef   __INCLUDES_H__
#define   __INCLUDES_H__

//��׼ͷ�ļ�
#include    <stdio.h>
#include 	<stdarg.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include 	<math.h>
#include  	<locale.h>		//��sprintf�����ܴ������� setlocale(LC_ALL,"C");   //��sprintf�����ܴ�����������<locale.h>��


// ZYNQ_LIB ͷ�ļ�

//OS
#include    "ucos_ii.h"
#include    "os_cpu.h"
#include    "os_cfg.h"
//#include 	"target.h"

//ƽ̨
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
//��ʱ��
#include 	"rtc/rtc.h"
//����
#include	"uart/uart.h"
//WLIP
#include 	"cc.h"
#include 	"ip_addr.h"
#include	"def.h"
//USB
#include		"usb_device.h"
#include		"usb_host.h"



// INCͷ�ļ�
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
//#include	"xms.h"  ����

//#include 	"Hzk48Code.h"

//#include    "nat.h"
#include	"keyinfo.h"
//#include	"telcom.h"

//#include	"graphics.h"


//VMotion ͷ�ļ�
#include 	"THMotion.h"

#define 	PROJECT_ID	"DB016A027B"
#define		VER			101
//#define		PRINTBMP

#define		DISPSTYLE	1														//�ַ���ʾ����ģʽ��0������ 2������� 1���Ҷ��룩
#define		EDITHEIGHT  40														//��������ʾ��߶�
#define		EDITWIDTH	345														//��������ʾ����
#define		EDITLEFT	43														//��������ʾ��X���
#define		EDITTOP		23														//��������ʾ��Y���
#define		DISTANCE	20														//��ʾ��֮��ľ���




#endif
