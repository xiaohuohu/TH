#ifndef		__DISPLAYDIR_H__
#define		__DISPLAYDIR_H__

#include	"rect.h"


#define ICO_WIDTH	48  												//图标宽度 像素单位
#define ICO_HEIGH	48													//图标高度 像素单位
#define ICO_DATA_SIZE  ((((ICO_WIDTH * 24) + 31) & ~31) / 8)			//图标图像数据宽度


typedef	struct _FileStruct
{
	INT16U 	x,y;														//显示位置
	INT8S  	name[0x100];													//文件或目录名称
	INT8S	attrib;														//指示为目录还是文件，0－文件，1－目录
	INT8U   nouse[3];
}FileEntity;

typedef struct _DIRFS
{
	INT8U rootPath[256];
 INT8U attrib;
	INT16U TotalItem; /* 文件列表中总共有多少项 */
	INT8U FileName[100];
}DIRFS;


void	CDispDir_Init(INT16S left,INT16S top,INT16S width,INT16S height,INT16S SysDiskNum);
void	CDispDir_Destroy(void);
void	CDispDir_Create(void);
void	CDispDir_OnKey(INT16U key,INT16U tkey);
void	CDispDir_OnButtonOk(void);
void	CDispDir_OnButtonCancel(void);
INT16S 	CDispDir_DoModal(void);

void	CDispDir_UpdateObject(INT16S dk);
void	CDispDir_BackPreDir(void);
void	CDispDir_GotoNextDir(void);
void	CDispDir_DelSpace(INT8S *str);
void	CDispDir_Sort(void);
void    CDispDir_GotoNextDir(void);


////////////////////修改文件复制部分///////////
BOOL 	CDispDir_CopyFattoFat(INT8S *dest,INT8S *src);
BOOL 	CDispDir_CopyYaffstoYaffs(INT8S *dest,INT8S *source);
BOOL	CDispDir_CopyFattoYaffs(INT8S *dest,INT8S *source);
BOOL 	CDispDir_CopyYaffstoFat(INT8S *dest,INT8S *source);
/////////////////读取文件或者磁盘空间的大小///////////
void 	CDispDir_GetFileLength(INT8S *source,INT8S *dest);


void 	CDispDir_GetDiskSpace(INT8S *source,INT8S *StrTotalSpace,INT8S *StrFreeSpace);
///////////文件夹删除函数/////////////////////////////
void    CDispDir_DelDir(INT8S *path);
///////////读取文件夹内容函数/////////////////////////
void 	CDispDir_ReadYaffsDir(INT8S *source);
void 	CDispDir_ReadFatDir(INT8S *source);
///////////界面绘制修改部分////////////////////////////
void 	CDispDir_ProgressBarInit(INT32S FileLen);
void 	CDispDir_DrawProgressBar(void);
//////////绘制磁盘、文件夹、文件的图标////////////////
void    CDispDir_DrawPicture(INT16S Type,INT16S x,INT16S y);
//代码修改添加部分


void    CDispDir_ButtonStatus( INT16S ibtn,INT16S init);

void 	CDispDir_UpDataView(INT16S  iPageStart);
void 	CDispDir_UpDataViewArea(BOOL bline);

void 	CDispDir_UpDataFilePath(INT8S *pach,BOOL bline);
void 	CDispDir_UpDataFileInfo(INT8S *name,INT8S *allsize,INT8S *filesize,BOOL bline);
void 	CDispDir_Message(INT8S *message,BOOL bline);

unsigned char OpenDir(void *DirHand,char *DirName);

unsigned char ReadDir(void *DirHand,char *DirName,DIRFS *DirInf);



#endif
