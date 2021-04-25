#ifndef		__DISPLAYDIR_H__
#define		__DISPLAYDIR_H__

#include	"rect.h"


#define ICO_WIDTH	48  												//ͼ���� ���ص�λ
#define ICO_HEIGH	48													//ͼ��߶� ���ص�λ
#define ICO_DATA_SIZE  ((((ICO_WIDTH * 24) + 31) & ~31) / 8)			//ͼ��ͼ�����ݿ��


typedef	struct _FileStruct
{
	INT16U 	x,y;														//��ʾλ��
	INT8S  	name[0x100];													//�ļ���Ŀ¼����
	INT8S	attrib;														//ָʾΪĿ¼�����ļ���0���ļ���1��Ŀ¼
	INT8U   nouse[3];
}FileEntity;

typedef struct _DIRFS
{
	INT8U rootPath[256];
 INT8U attrib;
	INT16U TotalItem; /* �ļ��б����ܹ��ж����� */
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


////////////////////�޸��ļ����Ʋ���///////////
BOOL 	CDispDir_CopyFattoFat(INT8S *dest,INT8S *src);
BOOL 	CDispDir_CopyYaffstoYaffs(INT8S *dest,INT8S *source);
BOOL	CDispDir_CopyFattoYaffs(INT8S *dest,INT8S *source);
BOOL 	CDispDir_CopyYaffstoFat(INT8S *dest,INT8S *source);
/////////////////��ȡ�ļ����ߴ��̿ռ�Ĵ�С///////////
void 	CDispDir_GetFileLength(INT8S *source,INT8S *dest);


void 	CDispDir_GetDiskSpace(INT8S *source,INT8S *StrTotalSpace,INT8S *StrFreeSpace);
///////////�ļ���ɾ������/////////////////////////////
void    CDispDir_DelDir(INT8S *path);
///////////��ȡ�ļ������ݺ���/////////////////////////
void 	CDispDir_ReadYaffsDir(INT8S *source);
void 	CDispDir_ReadFatDir(INT8S *source);
///////////��������޸Ĳ���////////////////////////////
void 	CDispDir_ProgressBarInit(INT32S FileLen);
void 	CDispDir_DrawProgressBar(void);
//////////���ƴ��̡��ļ��С��ļ���ͼ��////////////////
void    CDispDir_DrawPicture(INT16S Type,INT16S x,INT16S y);
//�����޸���Ӳ���


void    CDispDir_ButtonStatus( INT16S ibtn,INT16S init);

void 	CDispDir_UpDataView(INT16S  iPageStart);
void 	CDispDir_UpDataViewArea(BOOL bline);

void 	CDispDir_UpDataFilePath(INT8S *pach,BOOL bline);
void 	CDispDir_UpDataFileInfo(INT8S *name,INT8S *allsize,INT8S *filesize,BOOL bline);
void 	CDispDir_Message(INT8S *message,BOOL bline);

unsigned char OpenDir(void *DirHand,char *DirName);

unsigned char ReadDir(void *DirHand,char *DirName,DIRFS *DirInf);



#endif
