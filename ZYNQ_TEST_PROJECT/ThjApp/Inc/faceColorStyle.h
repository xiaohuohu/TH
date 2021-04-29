#ifndef __FACECOLORSTYLE_H__
#define __FACECOLORSTYLE_H__
# include	"os_cpu.h"
//#include 	"rtcapi.h"
#include    "cdc.h"
#define SKYBLUECOLOR	0X004080 //ε��
#define DARKGREENCOLOR4	0x137D5A
#define DARKGRAYCOLOR2	0x505050 //���


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
	INT32S fcsTableLine ;			//�����������      
	INT32S fcsTechfrmBk ;			//������������ĵ�ɫ
	INT32S fcsTitleBk ; 			//�б����ͷ���ֵ�ɫ��"10 X 2" �г˺ŵ���ɫ��16��СȦȦ����ɫ��̽������״̬��
	INT32S fcsTableBk ; 			//���������ɫ

}FACE_CLR_STYLE;


void SetFCS(INT16S mode);









#endif
