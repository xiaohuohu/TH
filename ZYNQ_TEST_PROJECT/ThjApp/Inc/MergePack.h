#ifndef _MPACK_H_
	#define _MPACK_H_



typedef struct MERGEFORMAT{

    char STU_HEAD[30];              //文件头部信息
    unsigned int Ver;
    unsigned int BuildData;
    unsigned int BuildTime;
    unsigned int ProgType;          //程序类型，ucos或linux
    unsigned int FileCount;         //文件数量
    unsigned int isCompress;        //压缩标志，NULL为不压缩
    unsigned int STLSIZE;           //静态库尺寸
}TMF;
typedef struct _TAreainfo{
    char name[255];
    unsigned int CompileType;       //编译类型，1静态加载，0动态加载，可释放
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
函数功能:获取资源文件属性包括文件名 文件大小等
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
	AreaInfo:返回资源属性数据结构指针
返回值:正确返回零 错误返回非零
*******************************************/
int MG_GetTAreaInfo(char *pathname,TAreaInfo *AreaInfo);
/*******************************************
函数功能:静态或动态获取资源内容
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
	paddr:返回资源内容数据指针
返回值:正确返回零 错误返回非零
*******************************************/
int MG_GetMem(char *pathname,INT32U *paddr);
/*******************************************
函数功能:动态获取资源内容内存释放
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
返回值:正确返回零 错误返回非零
*******************************************/
int MG_FreeMem(char *pathname);
/*******************************************
函数功能:更新资源文件NC_RES.NCP
参数:path:文件名及路径(如:"ADT\\Test333.txt")指针
	size:更新保存文件的大小
	s:更新保存文件内容指针
返回值:正确返回零 错误返回非零
*******************************************/
int MG_Update(char *path,int size,char *s);



int MG_UNMERGE(char *path);



#endif
