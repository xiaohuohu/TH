#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_


#define trtag	"\x7f\x8f\x9f"

//#define trtags	trtag##"\""
//#define tr(a)	trtag## #a##"\x00"## #a
#define trtags	trtag"\""
//#define tr(a)	trtag #a"\x00" #a
#define tr(a)	(a)


#define IMAGEROMBASE 0x00100000
#define IMAGEZILIMIT 0x00100000+0x04000000


/*************************************************
	1,trtags���ڱ�ʾ��Ҫ������ַ���
	2��Ϊ����ַ������������⣬����UTF8��ʽ
	3��
*************************************************/

extern char	*CN_SYSAlarmMenu[];
extern char	*CN_GLIBAlarmMenu[];/*G����ⶨ�屨����Ϣ*/
extern char *LatheErrStr[];		/*�����������*/
extern char *CustomErrStr[]; 	/*�Զ��屨����Ϣ*/

typedef struct LanguageStruct{
	struct LanguageStruct *next;
	INT16U No;
	char *str;
}T_LanguageStruct;

typedef struct TagLanguage{
	char Tag[8];
	INT32U Capacity;
	INT32U NextTag;
}T_TagLang;


void LanguagePack(INT8U pack);

INT16U TransferLanguagePack(INT32U LangCode);


#endif
