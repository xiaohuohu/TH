#ifndef _MPACK_H_
	#define _MPACK_H_



typedef struct MERGEFORMAT{

    char STU_HEAD[30];              //�ļ�ͷ����Ϣ
    unsigned int Ver;
    unsigned int BuildData;
    unsigned int BuildTime;
    unsigned int ProgType;          //�������ͣ�ucos��linux
    unsigned int FileCount;         //�ļ�����
    unsigned int isCompress;        //ѹ����־��NULLΪ��ѹ��
    unsigned int STLSIZE;           //��̬��ߴ�
}TMF;
typedef struct _TAreainfo{
    char name[255];
    unsigned int CompileType;       //�������ͣ�1��̬���أ�0��̬���أ����ͷ�
    unsigned int BuildData;
    unsigned int BuildTime;
    unsigned int offset;
    unsigned int filesize;
    void *pInfo;
}TAreaInfo;

#define MaxFileCount    1000
#define MaxFileSize     (1024*1024*10)

enum MergeErrList{
    MG_NO_ERR =0,
    MG_FILETOOMUCH,
    MG_FILETOOBIG,
    MG_FILENOEXIST,
    MG_FILECRCERR,
    MG_FILEREADERR,
    MG_FILEWRITEERR,
    MG_FILEDELETEERR,
    MG_GETMEMFAIL,
    MG_FREEMEMFAIL
};
/*******************************************
��������:��ȡ��Դ�ļ����԰����ļ��� �ļ���С��
����:pathname:�ļ�����·��(��:"ADT\\Test333.txt")ָ��
	AreaInfo:������Դ�������ݽṹָ��
����ֵ:��ȷ������ ���󷵻ط���
*******************************************/
int MG_GetTAreaInfo(char *pathname,TAreaInfo *AreaInfo);
/*******************************************
��������:��̬��̬��ȡ��Դ����
����:pathname:�ļ�����·��(��:"ADT\\Test333.txt")ָ��
	paddr:������Դ��������ָ��
����ֵ:��ȷ������ ���󷵻ط���
*******************************************/
int MG_GetMem(char *pathname,INT32U *paddr);
/*******************************************
��������:��̬��ȡ��Դ�����ڴ��ͷ�
����:pathname:�ļ�����·��(��:"ADT\\Test333.txt")ָ��
����ֵ:��ȷ������ ���󷵻ط���
*******************************************/
int MG_FreeMem(char *pathname);
/*******************************************
��������:������Դ�ļ�NC_RES.NCP
����:path:�ļ�����·��(��:"ADT\\Test333.txt")ָ��
	size:���±����ļ��Ĵ�С
	s:���±����ļ�����ָ��
����ֵ:��ȷ������ ���󷵻ط���
*******************************************/
int MG_Update(char *path,int size,char *s);



int MG_UNMERGE(char *path);



#endif
