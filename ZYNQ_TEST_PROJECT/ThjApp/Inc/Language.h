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
	1,trtags用于表示需要翻译的字符串
	2，为解决字符串处理函数问题，采用UTF8格式
	3，
*************************************************/

extern char	*CN_SYSAlarmMenu[];
extern char	*CN_GLIBAlarmMenu[];/*G代码库定义报警信息*/
extern char *LatheErrStr[];		/*车床错误代码*/
extern char *CustomErrStr[]; 	/*自定义报警信息*/

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
