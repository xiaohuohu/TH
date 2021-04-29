#include	"includes.h"
#include	"base.h"
#include	"dispdir.h"

#define		MAXFILECOUNT		0x400                                   //����ļ��洢��
#define		BGCOLOR				DARKGRAYCOLOR							//������ɫ ��ɫ
#define		DISKCOLOR 			DARKGRAYCOLOR							//����Ӳ��ͼ�����ɫ

#define 	DISPX				73										//�ļ����ļ�������λ�õ���ʼXƫ����
#define 	DISPY				110										//�ļ����ļ�������λ�õ���ʼYƫ����

#define		DISPDIRITEMWIDTH	190 									//�ļ�֮��ĺ���ƫ��ֵ
#define		DISPDIRITEMHEIGHT	68										//�ļ�֮�������ƫ��ֵ  

INT16S 		usb_max_devs;									//֧��usb�豸�����Ŀ


TEST_EXT    OS_SEM 		FileSem;                                       //�ļ�����������ź���
TEST_EXT	INT16S 		g_iDlgLeft;										//�߿���������
TEST_EXT	INT16S		g_iDlgTop;										//�߿򶥶�����
TEST_EXT	INT16S		g_iDlgWidth;									//�߿�Ŀ��
TEST_EXT	INT16S		g_iDlgHeight;									//�߿�ĸ߶�
TEST_EXT	INT16S		g_iDlgIDOk;										//Do_Modal��������ķ���ֵ
TEST_EXT	BOOL		Ok_Exit;										//�˳���־��
TEST_EXT	BOOL		g_bEnglish;		
//extern 		FATFS		g_ufs;

INT16S      g_iPageStart = 0;
INT16S		g_iCurFocus=0;												//��ǰ�������λ�õı��

INT8S		g_cPath[0x80]="\0";									       	//��ǰ���ڽ����·�����������ICO�ļ����Ժ�g_cPath=0:\ADT\ICO
INT8S		g_cSrcPath[0x80]="\0";										//������ڵ�λ�ö�Ӧ���ļ����ļ��е�����·�����������ADT�ļ��к󣬹����ICO�ļ��д���g_cSrcPath=0:\ADT\ICO
INT16S		g_iTotalCount=0;											//�򿪵�һ�����̻����ļ��е��ļ���Ŀ
BOOL		g_bDirCopy=FALSE;											//�ļ��и��Ʊ�־��
INT16S		g_iCurDisk=-1;												//-1Ϊ��Ŀ¼��0Ϊyaffs���̣�1Ϊfat���̣�2Ϊusb                                                  																
//FS_DIR		*g_pDir;      												//FAT�ļ�ϵͳ�ṹ��ȫ�ֱ���
DIR			*g_pDir;
//yaffs_DIR  	*g_pyaffsDir; 												//Yaffs�ļ�ϵͳ�ṹ��ȫ�ֱ���
FileEntity	*g_pFileEntity;  											//�ṹ�壬�����������ļ������ļ��е����֣�λ�ã�������Ϣ
FileEntity	g_FileEntityArray[MAXFILECOUNT];							//�������1024��Ŀ¼�Լ��ļ����Ŀ¼ֻ��������

INT16S 		g_iDispDirRowNum;	 										//����
INT16S 		g_iDispDirColNum;											//����	

static 		INT16S		g_iDispDirPageNum;											//��ʾÿ����ʾ���ļ���											//�Ƿ��ڸ�Ŀ¼����

BOOL        g_bFromFat=TRUE;											//true��ʾ��FAT���ƣ�false��ʾ��Yaffs����
BOOL        g_bToFat=TRUE;												//true��ʾճ����FAT��false��ʾճ����Yaffs 
INT32S 		g_iBufSize=1024*500;                                        //�ļ�����ʱ��ÿһ�����������500Kb
INT32S 		g_iCurPercent=0,g_iPieceWidth=0,g_iPieceNum=0,g_iCurWidth=0;//�ѿ����İٷֱȣ�ÿһ�����ȣ���������ǰ���������
BOOL        g_bUsbFirstRead=TRUE;										//��һ�ν���U�̵ı���������һ�ν���֮ǰtrue������֮����Ϊfalse

static FATFS	g_ufs;

												
//�������̽�����Ϣ������ʼ��
void	CDispDir_Init(INT16S left,INT16S top,INT16S width,INT16S height,INT16S SysDiskNum)
{
	Ok_Exit=FALSE;
	g_iDlgIDOk=TRUE;
	g_iCurDisk=SysDiskNum;												//-1��ʾ��Ŀ¼��0��ʾYaffs��1��ʾFat��2��ʾ��ӵ�U��
	g_iCurFocus=0;
	g_iPageStart = 0;
	
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	
	g_iDispDirColNum =(g_iDlgWidth-20)/DISPDIRITEMWIDTH;				//����
 	g_iDispDirRowNum =(g_iDlgHeight-190)/DISPDIRITEMHEIGHT;				//����					
	g_iDispDirPageNum=g_iDispDirRowNum*g_iDispDirColNum;				//ÿҳ��ʾ����Ŀ��
	
	memset(g_cPath,NULL,0x80);
	Uart_Printf("g_iCurDisk=%d\n",g_iCurDisk);
	
	switch(g_iCurDisk)
	{
		case -1:														//��ʾ��Ŀ¼												
			strcpy(g_cPath,g_bEnglish?"My Devices":tr("�ҵ��豸"));
			break;
		case 0:
			//����û��YAFFS��
			break;
			
		case 1:	
			strcpy(g_cPath,"0:");
			break;	
					
		case 2:
			strcpy(g_cPath,"3:");
			break;
			
		default:
		    break;
	}
	
	memset(g_cSrcPath,NULL,0x80);	 									//�ص�ע�⣺�漰����֮��Ŀ��������ܽ��г�ʼ����ճ��������Ž��г�ʼ��
	memset(g_FileEntityArray,NULL,sizeof(FileEntity)*MAXFILECOUNT);

}

//�������棬��ʾ����
void   CDispDir_Create()												
{
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"File Management":tr("�ļ�����"));

	//��ť״̬��Ϊ͹��
	CDispDir_ButtonStatus(0, 1);
	
 	CDispDir_UpDataFilePath(g_bEnglish?"My Devices":tr("�ҵ��豸"),1);
	CDispDir_UpDataViewArea(1);
 	CDispDir_UpDataFileInfo("","","",1);
 	CDispDir_Message(NULL,1);
	CDispDir_UpDataView(g_iPageStart);
}


void CDispDir_UpDataView(INT16S  iPageStart)
{

//	CDC_PutColorBar(11,76,g_iDlgWidth-21,g_iDlgHeight-191,WHITECOLOR,0);

	memset(g_FileEntityArray,NULL,sizeof(FileEntity)*MAXFILECOUNT);
	CDispDir_UpDataViewArea(0);
	
	if(g_iCurDisk==-1)
	{
		INT16S  DiskID=0;
		//���̵�������ʾλ��
		strcpy(g_FileEntityArray[0].name,g_bEnglish?"Local Disk(C:)":tr("���ش���(C:)"));
		strcpy(g_FileEntityArray[1].name,g_bEnglish?"Local Disk(D:)":tr("���ش���(D:)"));
		strcpy(g_FileEntityArray[2].name,g_bEnglish?"U Disk(E:)":tr("�ƶ�����(E:)"));
		g_iTotalCount=3;
		strcpy(g_cPath,g_bEnglish?"My Devices":tr("�ҵ��豸"));
		
		for(DiskID=0;DiskID<g_iTotalCount;DiskID++)
		{
			g_FileEntityArray[DiskID].attrib=2;	
		}	
	}
	else if(g_iCurDisk==1||g_iCurDisk==2)							//fat�̺�usb
	{
		CDispDir_ReadFatDir(g_cPath);								//��ȡfat�����ļ����ļ��еĺ���
	}	
			
	if(g_iTotalCount<=0)
	{
		Uart_Printf(g_bEnglish?"Dir Empty":"Ŀ¼Ϊ��,û���ļ�");
		CDispDir_UpDataFilePath(g_cPath,0);							//����·��
		CDispDir_Message(g_bEnglish?"Dir Empty":tr("Ŀ¼Ϊ��,û���ļ�"),0);
	}
	else
	{
		INT16S  i = 0;
		INT16S  dx = 0;
		INT16S  dy = 0;
		INT8S 	SrcPath[0x80]="\0";									//�ص�ע�⣺�Ӻ����о�����Ҫʹ��ȫ�ֱ���������ɶ�ȫ�ֱ�����ֵ�ĸ��ţ����³���bug
		INT8S 	FileSize[0x30]="\0";

	
		CDispDir_UpDataFilePath(g_cPath,0);							//����·��
		CDispDir_Sort();											//�ļ����ļ��а�����������

	//printf("g_iDispDirPageNum1=%d\n",g_iDispDirPageNum);
	//printf("g_iDispDirRowNum=%d\n",g_iDispDirRowNum);
	//printf("g_iDispDirColNum=%d\n",g_iDispDirColNum);
	
		for(i=0; i<g_iDispDirPageNum; i++)		
		{	
			
			//printf("here3\n");
			if(iPageStart+i>= g_iTotalCount)
			{	
				break;
			}	
			
			g_pFileEntity = &g_FileEntityArray[iPageStart+i];
			
			dx = 20 + g_pFileEntity->x;
			dy = 90 + g_pFileEntity->y;
			
			if(g_pFileEntity->attrib==2)      						 	//��ʾ����      					
			{
				CDispDir_DrawPicture(0,dx,dy);
			}		
			else if(g_pFileEntity->attrib==1)							//��ʾ�ļ���ͼ��
			{
				CDispDir_DrawPicture(3,dx,dy);
			}
			else if((strstr(g_pFileEntity->name,(char*)".DAT"))||(strstr(g_pFileEntity->name,(char*)".dat")))
			{
				CDispDir_DrawPicture(1,dx,dy);                 			//��ʾͼƬ�ļ�ͼ��
			}
			else
			{
				CDispDir_DrawPicture(2,dx,dy);							//��ʾ���������ļ�ͼ��
			}
			
			CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,
					g_pFileEntity->name,BLACKCOLOR,WHITECOLOR);          //ͼ���Ҳ���ʾ�ļ�����

		}
				
		g_pFileEntity = &g_FileEntityArray[iPageStart+g_iCurFocus];																	
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,
						g_pFileEntity->name,WHITECOLOR,DARKBLUECOLOR);			//������ڴ�����ɫ��������
		
		//��ȡ������ڴ����ļ��Ĵ�С���ļ��к���
		if(g_pFileEntity->attrib==2)									//�������̵Ŀռ�								
		{
			INT8S  source[80]="";
			INT8S  StrTotalSpace[30]="";
			INT8S  StrFreeSpace[30]="";
			//Uart_Printf("here0=%s,here0=%s\n",StrTotalSpace,StrFreeSpace);
			//strcpy(source,"/flash");
			CDispDir_GetDiskSpace(source,StrTotalSpace,StrFreeSpace);			//��ȡ���̵��ܿռ��ʣ��ռ䲢��ʾ
			CDispDir_UpDataFileInfo(g_pFileEntity->name,StrTotalSpace,StrFreeSpace,0);
		}	
		else if(g_pFileEntity->attrib==0)								//�ļ������ļ��У�
		{
			INT8S buf[15]="\0";
			
			strcpy(SrcPath,g_cPath);
			
			if(g_iCurDisk==0)											//yaffs��Fat�����ļ�·���Ĵ洢��ʽ��һ������������
			{					
				strcat(SrcPath,"/");
			}
			else
			{
			    strcat(SrcPath,"\\");
			}
			
			strcpy(buf,g_pFileEntity->name);
			CDispDir_DelSpace(buf);
			strcat(SrcPath,buf);	
			CDispDir_GetFileLength(SrcPath,FileSize);					//��ȡ�ļ��Ĵ�С����ʾ
			
			CDispDir_UpDataFileInfo(g_pFileEntity->name,"",FileSize,0);

		}
		else
		{
			CDispDir_UpDataFileInfo(g_pFileEntity->name,"","",0);
		}
		
		CDispDir_Message(" ",0);
	}
}


void CDispDir_UpDataFilePath(INT8S *pach,BOOL bline)
{
	INT8S buf[0x80];
	INT16S x1=  g_iDlgLeft+10;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-10;
	INT16S y1=	g_iDlgTop+43;
	INT16S y2=	g_iDlgTop+68;
	
	memset(buf,NULL,sizeof(buf));

	if(bline)
	{
		CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
		CDC_Line(x1,y1,x1,y2,BLACKCOLOR);
		
		CDC_Line(x1,y2,x2,y2,WHITECOLOR);
		CDC_Line(x2,y1,x2,y2,WHITECOLOR);
	}
	
	if(pach !=NULL)
	{
		CDC_PutColorBar(x1+1,y1+1,x2-x1-2,y2-y1-2,WHITECOLOR,0);				//Ŀ¼���������

		switch(pach[1])
		{	
		case 'f'://
			strcpy(buf,"C:");
			strcat(buf,pach+6);
			CDC_OutText(x1+6,y1+4,buf,BLACKCOLOR,WHITECOLOR);
			break;
		case 'm':
			strcpy(buf,"D:");
			strcat(buf,pach+4);
			CDC_OutText(x1+6,y1+4,buf,BLACKCOLOR,WHITECOLOR);
			break;
			
		case 's':
			strcpy(buf,"E:");
			strcat(buf,pach+4);
			CDC_OutText(x1+6,y1+4,buf,BLACKCOLOR,WHITECOLOR);
			break;
			
		default:
			strcpy(buf,pach);
		//	CDC_OutText_UTF(x1+6,y1+4,buf,BLACKCOLOR,WHITECOLOR,KZK_16);
			CDC_OutText(x1+6,y1+4,buf,BLACKCOLOR,WHITECOLOR);
			break;
		}
	}
}


void CDispDir_UpDataViewArea(BOOL bline)
{
	INT16S x1=  g_iDlgLeft+10;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-10;
	
	INT16S y1=	g_iDlgTop+75;
	INT16S y2=	g_iDlgTop+g_iDlgHeight-115;
	

	if(bline)
	{
		CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
		CDC_Line(x1,y1,x1,y2,BLACKCOLOR);
		
		CDC_Line(x1,y2,x2,y2,WHITECOLOR);
		CDC_Line(x2,y1,x2,y2,WHITECOLOR);
	}
	else
		CDC_PutColorBar(x1+1,y1+1,x2-x1-2,y2-y1-2,WHITECOLOR,0);				//Ŀ¼���������
}


void CDispDir_UpDataFileInfo(INT8S *name,INT8S *allsize,INT8S *filesize,BOOL bline)
{
	INT16S x1=  g_iDlgLeft+10;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-10;
	INT16S y1=	g_iDlgTop+g_iDlgHeight-107;
	INT16S y2=	g_iDlgTop+g_iDlgHeight-82;


	if(bline)
	{
		CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
		CDC_Line(x1,y1,x1,y2,BLACKCOLOR);
		
		CDC_Line(x1,y2,x2,y2,WHITECOLOR);
		CDC_Line(x2,y1,x2,y2,WHITECOLOR);
	}
	
	CDC_PutColorBar(x1+1,y1+1,x2-x1-1,y2-y1-1,LIGHTGRAYCOLOR,0);				//�ļ���С��ʾ�������

	//CDC_OutText_UTF(x1+6,y1+4,name,BLACKCOLOR,LIGHTGRAYCOLOR,KZK_16);
	//CDC_OutText_UTF(x1+300,y1+4,allsize,BLACKCOLOR,LIGHTGRAYCOLOR,KZK_16);
	//CDC_OutText_UTF(x1+600,y1+4,filesize,BLACKCOLOR,LIGHTGRAYCOLOR,KZK_16);
	
	CDC_OutText(x1+6,y1+4,name,BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText(x1+300,y1+4,allsize,BLACKCOLOR,LIGHTGRAYCOLOR);
	CDC_OutText(x1+600,y1+4,filesize,BLACKCOLOR,LIGHTGRAYCOLOR);
}


void CDispDir_Message(INT8S *message,BOOL bline)
{
	INT16S x1=  g_iDlgLeft+10;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-10;
	INT16S y1=	g_iDlgTop+g_iDlgHeight-75;
	INT16S y2=	g_iDlgTop+g_iDlgHeight-45;

	if(bline)
	{
		CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
		CDC_Line(x1,y1,x1,y2,BLACKCOLOR);
		
		CDC_Line(x1+1,y1+1,x2,y1+1,BLACKCOLOR);
		CDC_Line(x1+1,y1+1,x1+1,y2,BLACKCOLOR);
		
		CDC_Line(x1,y2,x2,y2,WHITECOLOR);
		CDC_Line(x2,y1,x2,y2,WHITECOLOR);
	}
	
	if(message)
	{
		CDC_PutColorBar(x1+2,y1+2,x2-x1-2,y2-y1-2,BGCOLOR,0);				//��ʾ��Ϣ�������
		CDC_OutText_UTF(x1+6,y1+4,message,BLACKCOLOR,BGCOLOR,KZK_24);
	}
}



//��ť״̬����
void CDispDir_ButtonStatus( INT16S ibtn,INT16S init)
{

	static	CRect 	rect[5];									//��ʾ��Ť�ı߿�

	if(init)
	{
		INT16S	i=0;
		INT16S  ButtonWidth = 160;

		for(i=0;i<5;i++)										//���水ť��Ϣ��ʼ��
		{
			rect[i].left=g_iDlgLeft+10+i*ButtonWidth;
			rect[i].top=g_iDlgTop+g_iDlgHeight-40;
			rect[i].right=rect[i].left+130;
			rect[i].bottom=g_iDlgTop+g_iDlgHeight-5;
			
			CDC_Line( rect[i].left+1,rect[i].top+1,rect[i].right-1,rect[i].top+1,WHITECOLOR);
			CDC_Line( rect[i].left+1,rect[i].top+1,rect[i].left+1,rect[i].bottom-1,WHITECOLOR);
			CDC_Line( rect[i].left+2,rect[i].bottom-1,rect[i].right-1,rect[i].bottom-1,BLACKCOLOR);
			CDC_Line( rect[i].right-1,rect[i].top+1,rect[i].right-1,rect[i].bottom-1,BLACKCOLOR);
		}

		if(g_bEnglish)
		{
			CDC_OutCenTextKzk24(rect[0]," 1-Copy",YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[1],"2-Paste",YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[2]," 3-Del",YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[3]," 4-Back",YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[4]," 5-Root",YELLOWCOLOR,BLACKCOLOR);
		}
		else
		{
			CDC_OutCenTextKzk24(rect[0],tr("1-����"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[1],tr("2-ճ��"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[2],tr("3-ɾ��"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[3],tr("4-��һ��"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[4],tr("5-��Ŀ¼"),YELLOWCOLOR,BLACKCOLOR);

		}
	}
	
	if(ibtn)
	{
		ibtn -=1;
		 
		CDC_Line( rect[ibtn].left+1,rect[ibtn].top+1,rect[ibtn].right-1,rect[ibtn].top+1,BLACKCOLOR);
		CDC_Line( rect[ibtn].left+1,rect[ibtn].top+1,rect[ibtn].left+1,rect[ibtn].bottom-1,BLACKCOLOR);
		CDC_Line( rect[ibtn].left+2,rect[ibtn].bottom-1,rect[ibtn].right-1,rect[ibtn].bottom-1,WHITECOLOR);
		CDC_Line( rect[ibtn].right-1,rect[ibtn].top+1,rect[ibtn].right-1,rect[ibtn].bottom-1,WHITECOLOR);
		OSTimeDly(200);
		CDC_Line( rect[ibtn].left+1,rect[ibtn].top+1,rect[ibtn].right-1,rect[ibtn].top+1,WHITECOLOR);
		CDC_Line( rect[ibtn].left+1,rect[ibtn].top+1,rect[ibtn].left+1,rect[ibtn].bottom-1,WHITECOLOR);
		CDC_Line( rect[ibtn].left+2,rect[ibtn].bottom-1,rect[ibtn].right-1,rect[ibtn].bottom-1,BLACKCOLOR);
		CDC_Line( rect[ibtn].right-1,rect[ibtn].top+1,rect[ibtn].right-1,rect[ibtn].bottom-1,BLACKCOLOR);
	}
}

//�ļ�ϵͳ�˳�ʱ���ͷ���Դ
void	CDispDir_Destroy()												
{
	FRESULT res;
	switch (g_iCurDisk)
	{
			
		case 1:
			{
				if(g_pDir)
				{
					//FS_CloseDir(g_pDir);
					res=f_closedir (g_pDir);
					g_pDir=NULL;
				}
			}
			break;
			
		case 2:
			{
				if(g_pDir)
				{
					//FS_CloseDir(g_pDir);
					res=f_closedir (g_pDir);
					g_pDir=NULL;
				}
			}	
			break;
		
		default:
			break;			
	}
	
	g_bUsbFirstRead=TRUE;
	f_mount(0,"3:",1);
	//usb_stop();


}

//ȥ���ļ����еĿո�
void	CDispDir_DelSpace(INT8S *str)															
{
	INT16S i,j;
	
	for(i=0; i<strlen(str); i++)
	{
		if(str[i] == ' ')
		{
			for(j=i; j<strlen(str); j++)
			{
				str[j] = str[j+1];
			}
			
			str[j] = '\0';
			i--;
		}
	}

	if(str[strlen(str)-1]=='.')
		str[strlen(str)-1]='\0';
}

//ˢ���ļ���ʾ����
void	CDispDir_UpdateObject(INT16S dk)
{
	INT8S FileSize[0x30]="\0";
	INT8S SrcPath[0x80]="\0";											//�ص�ע�⣺�Ӻ����о�����Ҫʹ��ȫ�ֱ���������ɶ�ȫ�ֱ�����ֵ�ĸ��ţ����³���bug

	if(g_iPageStart+g_iCurFocus+dk<0)
		return;
		
	if(g_iPageStart+g_iCurFocus+dk>=g_iTotalCount)
		return;

	if(g_iCurFocus+dk>=g_iDispDirPageNum)
	{
		g_iCurFocus =0;
		g_iPageStart +=g_iDispDirPageNum;
		CDispDir_UpDataView(g_iPageStart);
		return;
	}
	
	if(g_iCurFocus+dk<0)
	{
		g_iCurFocus =g_iDispDirPageNum-1;
		g_iPageStart -=g_iDispDirPageNum;
		CDispDir_UpDataView(g_iPageStart);
		return;
	}


	{
		g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,g_pFileEntity->name,BLACKCOLOR,WHITECOLOR);					//ʹԭ������ڴ�ʧȥ����

	}
	
	g_iCurFocus += dk;
	
	//ϵͳ��Ŀ¼�ƶ����
	if(g_iCurDisk==-1)
	{
		INT8S  source[80]="";
		INT8S  StrTotalSpace[30]="";
		INT8S  StrFreeSpace[30]="";
		
		g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];	
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,g_pFileEntity->name,WHITECOLOR,DARKBLUECOLOR);
		
		switch(g_iPageStart+g_iCurFocus)
		{
			case 0:	
				strcpy(source,"/flash");
				break;
					
			case 1:	
				//strcpy(source,"smc:");
				strcpy(source,"0:");
				break;
				
			case 2:
				strcpy(source,"3:");	
				break;	

			default:
				strcpy(source,"3:");		
				break;
		}
		

		CDispDir_GetDiskSpace(source,StrTotalSpace,StrFreeSpace);		//��ȡ���̵��ܿռ��ʣ��ռ䲢��ʾ

		CDispDir_UpDataFileInfo(g_pFileEntity->name,StrTotalSpace,StrFreeSpace,0);

	}
	else														//����������̺��ƶ����
	{
		g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,g_pFileEntity->name,WHITECOLOR,DARKBLUECOLOR);				//�µĹ�����ڴ���ý���
		
		if(g_pFileEntity->attrib!=1)										//��ȡ������ڴ����ļ��Ĵ�С���ļ��к���
		{
			INT8S buf[0x30]="\0";
			strcpy(SrcPath,g_cPath);
			
			if(g_iCurDisk==0)
			{					
				strcat(SrcPath,"/");
			}
			else
			{
			    strcat(SrcPath,"\\");
			}
			
			strcpy(buf,g_pFileEntity->name);
			CDispDir_DelSpace(buf);
			strcat(SrcPath,buf);	
			CDispDir_GetFileLength(SrcPath,FileSize);						//�ļ��Ĵ�С

			CDispDir_UpDataFileInfo(g_pFileEntity->name,"",FileSize,0);
		}
		else
		{
			CDispDir_UpDataFileInfo(g_pFileEntity->name,"","",0);
		}
	}
}



//������һҳ
void	CDispDir_BackPreDir(void)
{
	INT16S i=0;
	
	if(g_iCurDisk==0)
	{
		if(strlen(g_cPath)<7)											//��ǰ����ΪYaffs���̵ĸ�Ŀ¼���򷵻ص�ϵͳ�ĸ�Ŀ¼	
		{
			g_iCurDisk=-1;
		}
		else                                                            //�������yaffs�ĸ�Ŀ¼�����˵���һ��Ŀ¼
		{
			for(i=strlen(g_cPath)-1; i>0; i--)
			{
				if(g_cPath[i] == '/')
				{
					g_cPath[i] = 0;
					break;
				}
			}
			
			if(i==0)
			{
				switch(g_iCurDisk)
				{
					case 0:	
						strcpy(g_cPath,"/flash");
					break;
					
					case 1:	
						//strcpy(g_cPath,"smc:");
						strcpy(g_cPath,"0:");
					break;
					
					case 2:	
						strcpy(g_cPath,"3:");
					break;
					
					default:
					break;
				}
			}
		}
	}
	else if(g_iCurDisk==1||g_iCurDisk==2)               				//fat��usb
	{
		//if(strlen(g_cPath)<5)											//��ǰ����Ϊfat���̵ĸ�Ŀ¼���򷵻ص�ϵͳ�ĸ�Ŀ¼
		if(strlen(g_cPath)<3)
		{
			g_iCurDisk=-1;
		}
		else
		{
			for(i=strlen(g_cPath)-1; i>0; i--)
			{
				if(g_cPath[i] == '\\')
				{
					g_cPath[i] = 0;
					break;
				}
			}
			
			if(i==0)
			{
				switch(g_iCurDisk)
				{
					case 0:	strcpy(g_cPath,"/flash");break;
					//case 1:	strcpy(g_cPath,"smc:");break;
					case 1:	strcpy(g_cPath,"0:");break;
					default:strcpy(g_cPath,"3:");break;
				}
			}
		}
	}


	g_iPageStart=0;
	g_iCurFocus =0;	
	CDispDir_UpDataView(g_iPageStart);

}

//������һ��Ŀ¼
void	CDispDir_GotoNextDir(void)
{
		
	if(g_iCurDisk==0) 
	{
		strcat(g_cPath,"/");
	}
	else if	(g_iCurDisk==1||g_iCurDisk==2)
	{
		strcat(g_cPath,"\\");
	} 
	 
	g_pFileEntity = &g_FileEntityArray[g_iCurFocus];
	strcat(g_cPath,g_pFileEntity->name);							//�����ļ��У�����������Ըı�  g_cPath ��ֵ��
	
	g_iPageStart=0;
	g_iCurFocus =0;	
	CDispDir_UpDataView(g_iPageStart);

}

//�����ļ������ļ��е����������ļ��з�ǰ�棬�ļ��ź���
void	CDispDir_Sort(void)
{
	INT16S 		i,j;
	INT16S 		dirnum=0;
	FileEntity	Tmp;

	for(i=0; i<g_iTotalCount; i++)										//��ѭ������ʵ�ֽ��ļ��зŵ�ǰ���ļ����ں���
	{
		if(g_FileEntityArray[i].attrib ==0 )								
		{
			for(j=i; j<g_iTotalCount; j++)
			{
				if(g_FileEntityArray[j].attrib==1)
					break;
			}
			
			if(j < g_iTotalCount)
			{
				memcpy(&Tmp,&g_FileEntityArray[j],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[j],&g_FileEntityArray[i],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[i],&Tmp,sizeof(FileEntity));
			}
		}
	}
	
	for(i=0; i<g_iTotalCount; i++)										//�����ļ���Ŀ¼������
	{										
		if(g_FileEntityArray[i].attrib==1)
			dirnum++;
	}
	
	for(i=0; i<dirnum; i++)												//�ļ��а���������
	{
		for(j=i+1; j<dirnum; j++)
		{
			if(strcmp(g_FileEntityArray[j].name,g_FileEntityArray[i].name) < 0)
			{
				memcpy(&Tmp,&g_FileEntityArray[j],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[j],&g_FileEntityArray[i],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[i],&Tmp,sizeof(FileEntity));
			}
		}
	}
	
	for(i=dirnum; i<g_iTotalCount; i++)								//�ļ������������ļ������ļ��к�
	{
		for(j=i+1; j<g_iTotalCount; j++)
		{
			if(strcmp(g_FileEntityArray[j].name,g_FileEntityArray[i].name) < 0)
			{
				memcpy(&Tmp,&g_FileEntityArray[j],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[j],&g_FileEntityArray[i],sizeof(FileEntity));
				memcpy(&g_FileEntityArray[i],&Tmp,sizeof(FileEntity));
			}
		}
	}

	for(i=0; i<g_iTotalCount; i++)
	{
		g_FileEntityArray[i].x=(i%g_iDispDirColNum)*DISPDIRITEMWIDTH;
		g_FileEntityArray[i].y=(i/g_iDispDirColNum%g_iDispDirRowNum)*DISPDIRITEMHEIGHT;
	}
}

//ճ����ɾ���Ȳ�����"ȷ��"���ѡ��
BOOL	CDispDir_Confirm(void)
{
	INT16U key;
	
	while(1)
	{
		key = KeyMBoxGetKey();
		
		if(key != 0xffff)
		{
			if(key==TYSWITCH ||key==TENTER)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		
		OSTimeDly(10);
	}

	return FALSE;
}

//��鵱ǰĿ¼���Ƿ���Ҫ�����Ƶ��ļ�����strΪ�ļ�����attrΪ1�����ļ��У�Ϊ0���ļ���path�ǵ�ǰ�����·��
BOOL	CDispDir_SearchFile(INT8S *str,INT8S attr,INT8S *path)					
{	
	FRESULT res;
#if FALSE

	if(g_iCurDisk==0)													//Yaffs��
	{
		struct  yaffs_dirent *yaffsDirent;
		char   buf[0x10];
		yaffs_DIR *yaffsDir;											//�ص�ע�⣺֮ǰ�õ�g_bYaffsDir��ȫ�ֱ�����������������ȫ�ֱ����ĵط������仯
		
		yaffsDir =yaffs_opendir(path);
		
		if(!yaffsDir)
		{
			Uart_Printf("Open Path Err !Path=%s\n",path);
		//	CDC_PutColorBar(11,76,g_iDlgWidth-21,g_iDlgHeight-191,WHITECOLOR,0);
			
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("Ŀ¼�򿪴���"),0);
			return FALSE;
		}

		do
		{
			yaffsDirent=yaffs_readdir(yaffsDir);	
			
			if(yaffsDirent)
			{
				strcpy(buf,yaffsDirent->d_name);
				CDispDir_DelSpace(buf);

				switch(attr)
				{
					case 1:												//attrΪ1����ѡȡ�����ļ���
						{
							if(yaffsDirent->d_type==0x03) 				//dir
							{
								if(strcmp(str,buf)==0)
								{
									yaffs_closedir(yaffsDir);
									yaffsDir = NULL;
									return TRUE;
								}
							}
						}
						break;
						
					case 0:												//attrΪ1����ѡȡ�����ļ�
						{
							if(yaffsDirent->d_type!=0x03) 				//file
							{						
								if(strcmp(str,buf)==0)
								{
									yaffs_closedir(yaffsDir);
									yaffsDir = NULL;
									return TRUE;
								}
							}
						}
						break;
						
					default:
						break;
				}
			}
			
		}while(yaffsDirent);
			
		yaffs_closedir(yaffsDir);
		yaffsDir = NULL;
	}
	else if(g_iCurDisk==1||g_iCurDisk==2)								//FAT��
#endif

	if(g_iCurDisk==1||g_iCurDisk==2)								//FAT��
	{
		//struct FS_DIRENT *direntp;
		FILINFO direntp;
		DIR pDir;													//ע��ʹ�þֲ�����
		char   buf[0x10];
		
		//pDir = FS_OpenDir(path);
		res = f_opendir (&pDir, &path);								/* Open a directory */
		
		if(res != FR_OK)
		{
			Uart_Printf("Open Path Err !Path=%s\n",path);
		//	CDC_PutColorBar(11,76,g_iDlgWidth-21,g_iDlgHeight-191,WHITECOLOR,0);
		
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("Ŀ¼�򿪴���"),0);
			return FALSE;
		}

		do
		{
			//direntp = FS_ReadDir(pDir);
			res = f_readdir (&pDir, &direntp);
			if(res== FR_OK)
			{
				//strcpy(buf,direntp->d_name);
				strcpy(buf,direntp.fname);
				CDispDir_DelSpace(buf);
				switch(attr)
				{
					case 1:
						{
							//if(direntp->FAT_DirAttr==16) 				//dir
							if(direntp.fattrib==0x10)
							{
								if(strcmp(str,buf)==0)
								{
									//FS_CloseDir(pDir);
									res=f_closedir (&pDir);
									//pDir = NULL;
									return TRUE;
								}
							}
						}
						break;
						
					case 0:
						{
							//if(direntp->FAT_DirAttr!=16) 				//file
							if(direntp.fattrib!=0x10)
							{						
								if(strcmp(str,buf)==0)
								{
									//FS_CloseDir(pDir);
									res=f_closedir (&pDir);
									//pDir = NULL;
									return TRUE;
								}
							}
						}
						break;
						
					default:
						break;
				}
			}
			
		}while(res!=FR_OK);
			
		//FS_CloseDir(pDir);
		res=f_closedir (&pDir);
		//pDir = NULL;
	}

	return FALSE;


}

//��ȡpath�����һ���ļ������ļ��е�����  ����path=0:\ADT\ICO����name=ICO
void	CDispDir_GetFileName(INT8S *name,INT8S *path)							
{
	INT16S i,j;
	
	if(g_bFromFat==FALSE)												//�����Ƶ��ļ�λ��yaffs
	{
		for(i=strlen(path)-1; i>=0; i--)
		{
			if(path[i]=='/')
				break;
		}
		
		if(i != 0)
		{
			for(j=i+1; j<strlen(path); j++)
			{
				name[j-(i+1)] = path[j];
			}
			
			name[j-(i+1)] = NULL;
		}
		else
			strcpy(name,"");				
	}
	else if(g_bFromFat==TRUE)											//�����Ƶ��ļ�λ��fat
	{
		for(i=strlen(path)-1; i>=0; i--)
		{
			if(path[i]=='\\')
				break;
		}
		
		if(i != 0)
		{
			for(j=i+1; j<strlen(path); j++)
			{
				name[j-(i+1)] = path[j];
			}
			
			name[j-(i+1)] = NULL;
		}
		else
			strcpy(name,"");
	}
}



//�ļ��и���
BOOL	CDispDir_CopyDir(INT8S *destpath,INT8S *srcpath) 				//��COPY��ǰ��ӦĿ¼�µ��ļ���������Ŀ¼��COPY
{


	char dest[0x80];
	char src[0x80];
#if FALSE
	if(g_bFromFat==FALSE&&g_bToFat==FALSE)								//��yaffs������yaffs
	{
		struct yaffs_dirent *yaffsDirent;
		g_pyaffsDir =yaffs_opendir(srcpath);

		//Uart_Printf("yaffs��yaffs���ļ��п���\n\r");
		if(!g_pyaffsDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"Ŀ¼�򿪴��� Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"Ŀ¼�򿪴���",0);
			return FALSE;
		}

		//ѭ������Դ�ļ����µ������ļ������ļ��г��⣩
		do
		{
			char buf[0x30];
			char buf1[0x10];
			char name[0x20];
			yaffsDirent= yaffs_readdir(g_pyaffsDir);

			if(yaffsDirent)
			{
				strcpy(buf1,yaffsDirent->d_name);
				CDispDir_DelSpace(buf1);

				if(yaffsDirent->d_type!=0x03)							//Դ�ļ�������������ļ��У����ļ��������ڿ�����Χ��ֻ�����ļ�							//Ŀ¼����ļ�����
				{
					strcpy(src,srcpath);
					strcat(src,"/");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"/");
					strcat(dest,buf1);									//�����Ƶ��ļ���Ŀ���ļ��е���·��

					CDispDir_GetFileName(name,dest);
					//�����if�ж�����������⣬�����õ��Ӻ����о�����Ҫ����ȫ�ֱ���
					if(CDispDir_SearchFile(name,0,destpath))			//�鿴Ŀ��Ŀ¼���Ƿ������������ļ���
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"�����ļ�...",0);

							if(!CDispDir_CopyYaffstoYaffs(dest,src))				//yaffs��yaffs���ļ���������
							{
								yaffs_closedir(g_pyaffsDir);
								g_pyaffsDir = NULL;
								Uart_Printf("Copy Dir Copy File Err1!\n");
								return FALSE;
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"ȡ���ļ�����",0);
						}
					}
					else												//���ظ��ļ�
					{

						if(!CDispDir_CopyYaffstoYaffs(dest,src))
						{
							yaffs_closedir(g_pyaffsDir);
							g_pyaffsDir = NULL;
							Uart_Printf("Copy Dir Copy File Err2!\n");
							return FALSE;
						}
					}
				}
			}
		}while(yaffsDirent);

		yaffs_closedir(g_pyaffsDir);
		g_pyaffsDir=NULL;
		return TRUE;
	}
#endif
	//else if(g_bFromFat==TRUE&&g_bToFat==TRUE)							//��fat������fat
	if(g_bFromFat==TRUE&&g_bToFat==TRUE)
	{
		FRESULT		res;

		//struct FS_DIRENT *direntp;
		FILINFO* direntp;
		//g_pDir = FS_OpenDir(srcpath);
		res=f_opendir (g_pDir, srcpath);

		if(res != FR_OK)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err\n":"Ŀ¼�򿪴��� \n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":tr("Ŀ¼�򿪴���"),0);
			return FALSE;
		}

		do
		{
			char buf[0x30];
			char buf1[0x10];
			char name[0x10];

			//direntp = FS_ReadDir(g_pDir);
			res=f_readdir (g_pDir, direntp);

			if(res==FR_OK)
			{
				strcpy(buf1,direntp->fname);
				CDispDir_DelSpace(buf1);

				if(direntp->fattrib!=0x10)						//Դ�ļ�������ļ����ƣ����ļ��к���
				{
					strcpy(src,srcpath);
					strcat(src,"\\");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"\\");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//�鿴Ŀ��Ŀ¼���Ƿ�����Ҫ���Ƶ��ļ���
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)\n",buf1);
						//sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)",buf1);
						strcpy(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":tr("����ͬ���ļ�,ȷ������?(Y/N)"));
						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":tr("�����ļ�..."),0);

							if(!CDispDir_CopyFattoFat(dest,src))
							{
								//FS_CloseDir(g_pDir);
								f_closedir (g_pDir);
								g_pDir=NULL;
								Uart_Printf("Copy Dir Copy File Err1!\n");
								return FALSE;
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Overwrite cancel":tr("ȡ���ļ�����"),0);
						}
					}
					else
					{
						if(!CDispDir_CopyFattoFat(dest,src))
						{
							//FS_CloseDir(g_pDir);
							f_closedir (g_pDir);
							g_pDir = NULL;
							Uart_Printf("Copy Dir Copy File Err2!\n");
							return FALSE;
						}
					}
				}
			}
		}while(res==FR_OK);

		//FS_CloseDir(g_pDir);
		f_closedir (g_pDir);
		g_pDir=NULL;
		return TRUE;
	}

#if(0)
	else if(g_bFromFat==TRUE&&g_bToFat==FALSE)							//��fat������yaffs
	{
		struct FS_DIRENT *direntp;
		g_pDir = FS_OpenDir(srcpath);

		if(!g_pDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"Ŀ¼�򿪴��� Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"Ŀ¼�򿪴���",0);
			return FALSE;
		}

		do
		{
			char buf[0x30];
			char buf1[0x10];
			char name[0x10];

			direntp = FS_ReadDir(g_pDir);

			if(direntp)
			{
				strcpy(buf1,direntp->d_name);
				CDispDir_DelSpace(buf1);

				if(direntp->FAT_DirAttr!=16)						//Ŀ¼����ļ�����
				{
					strcpy(src,srcpath);
					strcat(src,"\\");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"/");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//�鿴Ŀ��Ŀ¼���Ƿ�����Ҫ���Ƶ��ļ���
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"�����ļ�...",0);

							if(!CDispDir_CopyFattoYaffs(dest,src))
							{
								FS_CloseDir(g_pDir);
								g_pDir=NULL;
								Uart_Printf("Copy Dir Copy File Err1!\n");
								return FALSE;
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"ȡ���ļ�����",0);
						}
					}
					else
					{
						if(!CDispDir_CopyFattoYaffs(dest,src))
						{
							FS_CloseDir(g_pDir);
							g_pDir = NULL;
							Uart_Printf("Copy Dir Copy File Err2!\n");
							return FALSE;
						}
					}
				}
			}
		}while(direntp);

		FS_CloseDir(g_pDir);
		g_pDir=NULL;
		return TRUE;
	}

	else if(g_bFromFat==FALSE&&g_bToFat==TRUE)							//��yaffs������fat
	{
		struct yaffs_dirent *yaffsDirent;
		g_pyaffsDir =yaffs_opendir(srcpath);
		//Uart_Printf("yaffs��Fat���ļ��п���\n");
		if(!g_pyaffsDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"Ŀ¼�򿪴��� Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"Ŀ¼�򿪴���",0);
			return FALSE;
		}

		do
		{
			char buf[0x30];
			char buf1[0x10];
			char name[0x10];
			yaffsDirent= yaffs_readdir(g_pyaffsDir);
			Uart_Printf("yaffsDirent=%d\n",yaffsDirent);

			if(yaffsDirent)
			{
				strcpy(buf1,yaffsDirent->d_name);
				CDispDir_DelSpace(buf1);

				if(yaffsDirent->d_type!=0x03)						//Ŀ��Ŀ¼���������Ŀ¼����Ŀ¼�����ڿ�����Χ��ֻ�����ļ�							//Ŀ¼����ļ�����
				{
					strcpy(src,srcpath);
					strcat(src,"/");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"\\");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//�鿴Ŀ��Ŀ¼���Ƿ������������ļ���
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"�����ļ�...",0);

							if(!CDispDir_CopyYaffstoFat(dest,src))
							{
								yaffs_closedir(g_pyaffsDir);
								g_pyaffsDir = NULL;
								Uart_Printf("Copy Dir Copy File Err1!\n");
								return FALSE;
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"ȡ���ļ�����",0);
						}
					}
					else											//���ظ��ļ�
					{
						if(!CDispDir_CopyYaffstoFat(dest,src))
						{
							yaffs_closedir(g_pyaffsDir);
							g_pyaffsDir = NULL;
							Uart_Printf("Copy Dir Copy File Err2!\n");
							return FALSE;
						}
					}
				}
			}
		}while(yaffsDirent);

		yaffs_closedir(g_pyaffsDir);
		g_pyaffsDir=NULL;
		return TRUE;
	}

#endif

	return TRUE;
}


void	CDispDir_OnButtonOk()
{
	Ok_Exit=TRUE;
	g_iDlgIDOk=1;
}


void	CDispDir_OnButtonCancel()
{
   Ok_Exit = TRUE;
   g_iDlgIDOk=0;
}



//�������̵İ�����Ӧ
void	CDispDir_OnKey(INT16U key,INT16U tkey)
{
	switch(key)
	{

		case TLEFT:
			CDispDir_UpdateObject(-1);
			break;

		case TRIGHT:
			CDispDir_UpdateObject(1);
			break;

		case TUP:
			CDispDir_UpdateObject(-g_iDispDirColNum);						//���ư��������λ��ֵ����һ�е��ļ���
			break;

		case TDOWN:
			CDispDir_UpdateObject(g_iDispDirColNum);						//���ư��������λ��ֵ����һ�е��ļ���
			break;

//		case TSAVE:
//			CDispDir_OnButtonOk();
//			break;

		case TCANCEL:
			{
				CDispDir_OnButtonCancel();
			}
			break;

		case TENTER:
			{
				g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];

				if(g_pFileEntity->attrib==1)
				{
					CDispDir_GotoNextDir();
				}
				else if(g_pFileEntity->attrib==2)
				{
					if(g_iCurFocus==0)										//����Yaffs
					{
						CDispDir_Message(g_bEnglish?"Disk C can not be Accessed!":tr("C���޷�����                   "),0);
						/*
						g_iCurDisk=0;
						g_iCurFocus =0;
						g_iPageStart=0;
						strcpy(g_cPath,"/flash");
						CDispDir_UpDataView(g_iPageStart);
						*/
					}
					else if(g_iCurFocus==1)									//����Fat��
					{
						g_iCurDisk=1;
						g_iCurFocus =0;
						g_iPageStart=0;
						//strcpy(g_cPath,"smc:");
						strcpy(g_cPath,"0:");
						CDispDir_UpDataView(g_iPageStart);
					}
					else if(g_iCurFocus==2)									//��ͼU��
					{

						if(g_bUsbFirstRead==TRUE)
						{
							INT16S usb_stor_curr_dev=-1;


							f_mount(0,"3:",0);

							UsbHostStop();
							
							Uart_Printf("���ڲ���U��\n");
							usb_stor_curr_dev = UDiskScan();

							if(usb_stor_curr_dev == 0)
							{
								usb_max_devs=0;
								Uart_Printf("δ�ҵ�U��!\n");
							//	CDC_PutColorBar(11,g_iDlgHeight-74,g_iDlgWidth-21,29,BGCOLOR,0);
								CDispDir_Message(g_bEnglish?"NO USB Device":tr("û�в���U��"),0);
								UsbHostStop();
								break;
							}
							else
							{
								usb_max_devs=1;
								Uart_Printf("�ҵ�U��!\n");
								g_bUsbFirstRead=FALSE;

							}
						}

						g_iCurDisk=2;

						g_iCurFocus =0;
						g_iPageStart=0;
						strcpy(g_cPath,"3:");
						
						CDispDir_UpDataView(g_iPageStart);

					}
				}
				else
				{
					CDispDir_Message(g_bEnglish?"Can't Open this file!":tr("ϵͳ���ܴ򿪸��ļ�"),0);
				}
			}
			break;

		case '1':														//���ƹ��ܣ����ָ����ļ��������Yaffs���и��ƣ�FAT�̵ĸ��ƣ��ñ�־��������
			{
				char buf[0x10];

				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Disk can't Be Copied":tr("���̲��ܱ�����"),0);
					break;
				}

				g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];

				CDispDir_ButtonStatus(1,0);													//��ʱ����ʾ����״̬

				strcpy(g_cSrcPath,g_cPath);

				if(g_iCurDisk==0)
				{
					strcat(g_cSrcPath,"/");
					g_bFromFat=FALSE;									//��0����ʾ��Yaffs����
				}
				else
				{
				    strcat(g_cSrcPath,"\\");
				    g_bFromFat=TRUE;									//��1����ʾ��fat����
				}

				strcpy(buf,g_pFileEntity->name);
				CDispDir_DelSpace(buf);
				strcat(g_cSrcPath,buf);
				Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);				//�����Ƶ��ļ���

				if(g_pFileEntity->attrib==1) 							//����Ŀ¼
				{
					INT8S tempBuf[0x30];
					g_bDirCopy=TRUE;									//�����ļ��б�־λ

					strcpy(tempBuf,g_bEnglish?"Copy Dir:":tr("����Ŀ¼:"));
					strcat(tempBuf,buf);
					CDispDir_Message(tempBuf,0);
				}
				else													//�����ļ�
				{
					INT8S tempBuf[0x30];

					g_bDirCopy=FALSE;									//�����ļ���־λ

					strcpy(tempBuf,g_bEnglish?"Copy File:":tr("�����ļ�:"));
					strcat(tempBuf,buf);
					CDispDir_Message(tempBuf,0);
				}
			}
			break;

		case '2':
			{
				char name[0x10];
				char curpath[0x80];
				char buf[0x30];

				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Disk can't Be Pasted":tr("���̲��ܱ�ճ��"),0);
					break;
				}

				CDispDir_ButtonStatus(2,0);

				if(g_iCurDisk==0)										//���¼�ֵ2ʱ���ڵĽ���Ϊyaffs����
				{
					g_bToFat=FALSE;										//��0�����Ƶ�yaffs
				}
				else if(g_iCurDisk==1||g_iCurDisk==2)
				{
					g_bToFat=TRUE;										//��1�����Ƶ�fat
				}

				if(g_bDirCopy) 	//Ŀ¼ճ��
				{
					CDispDir_GetFileName(name,g_cSrcPath);				//��ȡ������Ŀ¼�����֣������g_cSrcPath=0:\ADT\ICO����name=ICO
					//Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);
					//Uart_Printf("name=%s\n",name);

					if(CDispDir_SearchFile(name,1,g_cPath))				//g_cPath��ǰ�����·������nameΪ�ļ��У�attrΪ1����������������true��
					{
						Uart_Printf(g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"����ͬ��Ŀ¼%s,ȷ������?(Y/N)\n",name);
						//sprintf(buf,g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"����ͬ��Ŀ¼%s,ȷ������?(Y/N)",name);
						strcpy(buf,g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"����ͬ��Ŀ¼,ȷ������?(Y/N)");
						CDispDir_Message(buf,0);

						if(!CDispDir_Confirm())
						{
							Uart_Printf(g_bEnglish?"Cancel Paste File":tr("ȡ��ճ��Ŀ¼�ļ�!"));

							CDispDir_Message(g_bEnglish?"Cancel Paste File":tr("ȡ��ճ��Ŀ¼�ļ�!"),0);
							break;
						}
						else
						{
							strcpy(curpath,g_cPath);

							if(g_iCurDisk==0)
							{
								strcat(curpath,"/");
							}
							else if(g_iCurDisk==1||g_iCurDisk==2)
							{
								strcat(curpath,"\\");
							}

							strcat(curpath,name);

							CDispDir_Message(g_bEnglish?"Copy Dir...":tr("����Ŀ¼..."),0);

							if(!CDispDir_CopyDir(curpath, g_cSrcPath))
							{
								Uart_Printf(g_bEnglish?"Paste Dir File Failed\n":"ճ��Ŀ¼�ļ�ʧ��!\n");

								CDispDir_Message(g_bEnglish?"Paste Dir File Failed!":tr("ճ��Ŀ¼�ļ�ʧ��!"),0);
							}
						}
					}
					else												//���������ļ���
					{
						strcpy(curpath,g_cPath);

						if(g_iCurDisk==0)
						{
							strcat(curpath,"/");
						}
						else if(g_iCurDisk==1||g_iCurDisk==2)
						{
							strcat(curpath,"\\");
						}

						strcat(curpath,name);
						Uart_Printf("Mk Cur Dir:%s\n",curpath);

						if(g_bToFat==TRUE)
						{
							//if(FS_MkDir(curpath)!=0)					//Fat�����ļ���
							if(f_mkdir(curpath)!=FR_OK)
							{
								Uart_Printf("Dir Make Failed!\n");
							}
							Uart_Printf("curpath=%s\n",curpath);
						}
						/*
						else if(g_bToFat==FALSE)
						{
							if(yaffs_mkdir(curpath, S_IFDIR)!=0)		//yaffs�����ļ���
							{
								Uart_Printf("Dir Make Failed!\n");
							}
						}*/

						CDispDir_Message(g_bEnglish?"Copy Dir...":tr("����Ŀ¼..."),0);

						if(!CDispDir_CopyDir(curpath, g_cSrcPath))
						{
							Uart_Printf(g_bEnglish?"Paste Dir File Failed!\n":"ճ��Ŀ¼�ļ�ʧ��!\n");

							CDispDir_Message(g_bEnglish?"Paste Dir File Failed!":tr("ճ��Ŀ¼�ļ�ʧ��!"),0);
						}
					}

					Uart_Printf("Copy Dir OK!\n");
					CDispDir_UpDataView(g_iPageStart);
					CDispDir_Message(g_bEnglish?"Paste Dir File Complete!":tr("ճ��Ŀ¼�ļ����!"),0);
				}
				else													//�ļ�ճ��
				{
					Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);
					Uart_Printf("g_cPath=%s\n",g_cPath);
					CDispDir_GetFileName(name,g_cSrcPath);
					Uart_Printf("name=%s\n",name);

					if(CDispDir_SearchFile(name,0,g_cPath))				//����ļ�����attrΪ0���������õ�name������true
					{
						Uart_Printf(g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)\n",name);
						//sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"����ͬ���ļ�%s,ȷ������?(Y/N)",name);
						strcpy(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":tr("����ͬ���ļ�,ȷ������?(Y/N)"));
						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							strcpy(curpath,g_cPath);

							if(g_iCurDisk==0)
							{
								strcat(curpath,"/");
							}
							else if(g_iCurDisk==1||g_iCurDisk==2)
							{
								strcat(curpath,"\\");
							}
							//�����ж�ֻ��ɾ��Ŀ��·���������ļ�������ֱ�Ӹ����ļ�
							strcat(curpath,name);

							CDispDir_Message(g_bEnglish?"Delete File...":tr("ɾ�����ļ�..."),0);

							if(strcmp(curpath,g_cSrcPath)!=0)			//���Ŀ��λ����Դλ�ò�ͬ�������ͬ�����������署һ���ļ�����ճ���µ�ǰλ�ã�
							{
								if(g_iCurDisk==0)
								{
									//yaffs_unlink(curpath);
								}
								else if(g_iCurDisk==1||g_iCurDisk==2)
								{
									//FS_Remove(curpath);
									FRESULT res;
									res=f_unlink (curpath);
								}
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Cancel Paste File!":tr("ȡ��ճ���ļ�!"),0);
							break;
						}
					}
					//else   											//else��ע�͵����������Ƿ��ļ�����������ִ���ļ����ƣ������жϺ����в����������ļ���
					{
						//Uart_Printf("û�������ļ�������else�ж�BGCOLOR=%d\n",BGCOLOR);
						strcpy(curpath,g_cPath);

						if(g_iCurDisk==0)
						{
							strcat(curpath,"/");
						}
						else if(g_iCurDisk==1||g_iCurDisk==2)
						{
							strcat(curpath,"\\");
						}

						strcat(curpath,name);

						CDispDir_Message(g_bEnglish?"Copy File...":tr("�����ļ�..."),0);
						#if(FALSE)
						if(g_bFromFat==TRUE&& g_bToFat==FALSE)
						{
							//Uart_Printf("��FAT���ƣ�������yaffs\n");
							if(!CDispDir_CopyFattoYaffs(curpath,g_cSrcPath))
							{
								Uart_Printf("Copy File Err\n");
								CDispDir_Message(g_bEnglish?"Copy File Failed!":"�����ļ�ʧ��!",0);
								break;
							}
						}
						#endif

						if(g_bFromFat==TRUE&& g_bToFat==TRUE)
						{
							//Uart_Printf("��FAT���ƣ�������FAT\n");
							if(!CDispDir_CopyFattoFat(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":tr("�����ļ�ʧ��!"),0);
								break;
							}
						}

						#if(FALSE)
						if(g_bFromFat==FALSE&&g_bToFat==FALSE)
						{
							//Uart_Printf("��yaffs���ƣ�������yaffs\n");
							if(!CDispDir_CopyYaffstoYaffs(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":"�����ļ�ʧ��!",0);
								break;
							}
						}

						if(g_bFromFat==FALSE&&g_bToFat==TRUE)
						{
							//Uart_Printf("��yaffs���ƣ�������FAT\n");
							if(!CDispDir_CopyYaffstoFat(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":"�����ļ�ʧ��!",0);
								break;
							}
						}
						#endif
					}

					CDispDir_UpDataView(g_iPageStart);
					CDispDir_Message(g_bEnglish?"Paste File Complete!":tr("ճ���ļ����!"),0);
				}
			}
			break;

		case '3':
			{
				char buf1[0x80];
				char buf[0x30];

				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Disk Can't Be Deleted":tr("���̲��ܱ�ɾ��"),0);
					break;
				}


				CDispDir_ButtonStatus(3,0);

				//�ļ��е�ɾ��
				if(g_pFileEntity->attrib == 1)
				{
					strcpy(buf1,g_cPath);

					if(g_iCurDisk==0)
					{
						strcat(buf1,"/");
					}
					else
					{
						strcat(buf1,"\\");
					}

					strcpy(buf,g_pFileEntity->name);
					CDispDir_DelSpace(buf);
					strcat(buf1,buf);
					Uart_Printf(g_bEnglish?"Delete File %s,Sure (Y/N)\n":"ɾ���ļ���%s,ȷ��ɾ��??(Y/N)\n",g_pFileEntity->name);
					//sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":"ɾ���ļ���%s,ȷ��ɾ��??(Y/N)",g_pFileEntity->name);
					strcpy(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":tr("ɾ���ļ���,ȷ��ɾ��?(Y/N)"));
					CDispDir_Message(buf,0);

					if(CDispDir_Confirm())
					{
						Uart_Printf("Delete File ...\n");
						CDispDir_Message(g_bEnglish?"Delete File ...":tr("ɾ���ļ���..."),0);
						CDispDir_DelDir(buf1);

						if(g_iCurFocus-1 < 0)
						{
							if( g_iPageStart>0)
							{
							 	g_iPageStart -=g_iDispDirPageNum;
							 	g_iCurFocus = g_iDispDirPageNum-1;
							}
							else
								g_iCurFocus =0;
						}
						else
							g_iCurFocus--;

						CDispDir_UpDataView(g_iPageStart);

						Uart_Printf("Delete File OK!\n");
						CDispDir_Message(g_bEnglish?"Delete File Complete!":tr("ɾ���ļ������!"),0);

						break;
					}
					else
					{
						Uart_Printf("Cancel Delete File!\n");
						CDispDir_Message(g_bEnglish?"Cancel Delete File!":tr("ȡ��ɾ���ļ���!"),0);
						break;
					}

				}
				else													//�ļ�ɾ��
				{
					strcpy(buf1,g_cPath);

					if(g_iCurDisk==0)									//Yaffs
					{
						strcat(buf1,"/");
					}
					else
					{
						strcat(buf1,"\\");								//FAT
					}

					strcpy(buf,g_pFileEntity->name);
					CDispDir_DelSpace(buf);
					strcat(buf1,buf);

					Uart_Printf(g_bEnglish?"Delete File %s,Sure (Y/N)\n":"ɾ���ļ�%s,ȷ��ɾ��?(Y/N)\n",g_pFileEntity->name);
					//sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":"ɾ���ļ�%s,ȷ��ɾ��??(Y/N)",g_pFileEntity->name);
					sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":tr("ɾ���ļ�,ȷ��ɾ��?(Y/N)"));
					CDispDir_Message(buf,0);

					if(CDispDir_Confirm())
					{
						Uart_Printf("Delete File ...\n");

						CDispDir_Message(g_bEnglish?"Delete File ...":tr("ɾ���ļ�..."),0);

						if(g_iCurDisk==0)
						{
							//yaffs_unlink(buf1);							//Yaffsɾ���ļ�����
						}
						else
						{
							//FS_Remove(buf1);							//FATɾ���ļ�����
							FRESULT res;
							res=f_unlink (buf1);
						}

						if(g_iCurFocus-1 < 0)
						{
							if( g_iPageStart>0)
							{
							 	g_iPageStart -=g_iDispDirPageNum;
							 	g_iCurFocus = g_iDispDirPageNum-1;
							}
							else
								g_iCurFocus =0;
						}
						else
							g_iCurFocus--;

						CDispDir_UpDataView(g_iPageStart);

						Uart_Printf("Delete File OK!\n");
						CDispDir_Message(g_bEnglish?"Delete File Complete!":tr("ɾ���ļ����!"),0);
						break;
					}
					else
					{
						Uart_Printf("Cancel Delete File!\n");
						CDispDir_Message(g_bEnglish?"Cancel Delete File!":tr("ȡ��ɾ���ļ�!"),0);
						break;
					}
				}
			}
			break;

		case '4':														//������һ��Ŀ¼
			{
				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Already Root":tr("�Ѿ�����Ŀ¼"),0);
					break;
				}


				CDispDir_ButtonStatus(4,0);

				CDispDir_BackPreDir();
			}
			break;
		case '5':														//���ظ�Ŀ¼
			{
				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Already Root":tr("�Ѿ�����Ŀ¼"),0);
					break;
				}


				CDispDir_ButtonStatus(5,0);

				g_iCurDisk=-1;
				g_iPageStart=0;
				g_iCurFocus =0;
				CDispDir_UpDataView(g_iPageStart);
			}

			break;
	#if	PRINTBMP													//��Ļ��ͼ �����
		case F5:case TDELALL:
			{
				char   filename[0x20];
				INT16S i=0;

				//CBmp_Init();
				strcpy(filename,"DspDir");

				for(i=0; i<strlen(filename); i++)
				{
					if(filename[i] == '.')
					{
						filename[i]='\0';
						break;
					}
				}

				strcat(filename,".bmp");
				CreatBMP(filename,1280,720,1);
				//CBmp_Destroy();
			}

			break;
	#endif

		default:
			break;
	}
}


//�������̵Ľ�������Լ�ִ�в����ĳ������
INT16S    CDispDir_DoModal()
{
	INT16U key=0,tkey=0;

	CDispDir_Create();

	while(TRUE)
	{
		key = KeyMBoxGetKey();

		if(key != 0xffff)
		{
			CDispDir_OnKey(key,tkey);									//������Ӧ����

			if(Ok_Exit)
				break;
		}

		/*if(RealIO(STOPIO,0,2))
		{
			Ok_Exit=TRUE;
			break;
		}*/

		OSTimeDly(10);
	}

	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}

#if 0
//����Ŀ��Ŀ¼����ȡyaffs�е��ļ�����ֵ������
void CDispDir_ReadYaffsDir(INT8S *source)
{


	INT16S 		count=0;
	char 	oldbuf[0x80]="\0";
	yaffs_DIR  	*yaffsDir;
	struct yaffs_dirent *yaffsDirent;

	strcpy(oldbuf,source);
	yaffsDir = yaffs_opendir(source);

	if(!yaffsDir)
	{
		strcpy(source,oldbuf);

		Uart_Printf(g_bEnglish?"Dir Open Error!\n":"Ŀ¼�򿪴���\n");
		CDispDir_Message(g_bEnglish?"Dir Open Error":"Ŀ¼�򿪴���",0);
		return;
	}

	do
	{
		yaffsDirent= yaffs_readdir(yaffsDir);

		if(yaffsDirent)
		{
			char   buf[0x20];

			memset(buf,NULL,0x20);

			if(count>=MAXFILECOUNT)
			{
				Uart_Printf(g_bEnglish?"Dir too much over 1024\n":"Ŀ¼���ļ����1024��\n");
				CDispDir_Message(g_bEnglish?"Dir or File Too Much":"Ŀ¼���ļ����1024",0);
				break;
			}

			strcpy(buf,yaffsDirent->d_name);
			CDispDir_DelSpace(buf);
			strcpy(g_FileEntityArray[count].name,buf);
			g_FileEntityArray[count].x=(count%g_iDispDirRowNum)*DISPDIRITEMWIDTH;					//�ļ�X����ƫ����
			g_FileEntityArray[count].y=(count/g_iDispDirRowNum%g_iDispDirColNum)*DISPDIRITEMHEIGHT;	//�ļ�Y����ƫ����
			g_FileEntityArray[count].attrib=(yaffsDirent->d_type ==0x03?1:0);                   	//1��ʾ�ļ��У�0��ʾ�ļ�
			count++;
		}

	}while(yaffsDirent);

	yaffs_closedir(yaffsDir);																//�رմ򿪵��ļ����ͷ���Դ
	yaffsDir=NULL;
	g_iTotalCount = count;

}
#endif


//����Ŀ��Ŀ¼����ȡfat�е��ļ�����ֵ������
void CDispDir_ReadFatDir(INT8S *source)
{

	INT16S 		count=0;
	char 	oldbuf[0x80]="\0";
	//struct FS_DIRENT *direntp;
	FILINFO direntp;
	DIR FatDir;
	FRESULT		res;

	DIRFS	dirifo;

	strcpy(oldbuf,source);

	res=OpenDir(&FatDir, source);

	//printf("res1=%d\n",res);

	if(res!=FR_OK)
	{
		if(g_iCurDisk==1)
		{
			strcpy(g_cPath,oldbuf);
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("Ŀ¼�򿪴���"),0);
			return;
		}
		else if(g_iCurDisk==2)
		{
			g_bUsbFirstRead=TRUE;
			strcpy(g_cPath,oldbuf);
						
			CDispDir_Message(g_bEnglish?"NO USB Device":tr("û�в���U��"),0);
			return;
		}
	}

	do
	{				
		res=ReadDir(&FatDir, source, &dirifo);
		
		if(res == FR_OK)
		{
			char   buf[0x100];

			memset(buf,NULL,0x100);

			strcpy(buf,dirifo.FileName);

			CDispDir_DelSpace(buf);

			if(strcmp(buf,".")!=0 && strcmp(buf,"..")!=0 && strcmp(buf,"")!=0)
			{

				if(count>=MAXFILECOUNT)
				{
					Uart_Printf(g_bEnglish?"Dir too much over 1024":tr("Ŀ¼���ļ����1024��"));
					break;
				}

				strcpy(g_FileEntityArray[count].name,buf);
				g_FileEntityArray[count].x=(count%g_iDispDirRowNum)*DISPDIRITEMWIDTH;
				g_FileEntityArray[count].y=(count/g_iDispDirRowNum%g_iDispDirColNum)*DISPDIRITEMHEIGHT;
				g_FileEntityArray[count].attrib=(dirifo.attrib==16?1:0);
				count++;
				
			}
		}
	}while(res == FR_OK);

	res= f_closedir (&FatDir);
	
	g_iTotalCount = count;

}


//�ļ��л��ļ���ɾ��
void	CDispDir_DelDir(INT8S *path)
{
#if 0

	if(g_iCurDisk==0)													//yaffs��
	{
		char dest[0x80];
		char buf[0x10];
		struct  yaffs_dirent *yaffsDirent;
		yaffs_DIR  	*yaffsDir;
		yaffsDir= yaffs_opendir(path);
		//ѭ��ɾ��Ŀ���ļ����ڲ����ļ������ļ���
		do
		{
			yaffsDirent=yaffs_readdir(yaffsDir);

			if(yaffsDirent)
			{
				strcpy(buf,yaffsDirent->d_name);
				CDispDir_DelSpace(buf);
				strcpy(dest,path);
				strcat(dest,"/");
				strcat(dest,buf);
				//Uart_Printf("dest=%s\n",dest);
				if(yaffsDirent->d_type!=0x03)							//������ļ�ֱ��ɾ��
				{
					yaffs_unlink(dest);
				}
				else													//������ļ��У�"�ݹ����"�ú���
				{
					CDispDir_DelDir(dest);
				}
			}

		}while(yaffsDirent);

		yaffs_closedir(yaffsDir);
		yaffs_rmdir(path);												//ɾ��Ŀ���ļ���������Ϊ�ú���ֻ��ɾ���յ��ļ��У����֮ǰ��Ҫѭ��������ɾ���ļ��е����ݣ�
	}
	else																//fat�̣�ԭ��ͬ��
#endif
	{
		char dest[0x80];
		char buf[0x10];
		//struct  FS_DIRENT *fsDirent;
		FILINFO	*fsDirent;
		DIR	*fsDir;
		FRESULT		res;
		//fsDir=  FS_OpenDir(path);
		res=f_opendir (fsDir, path);

		do
		{
			//fsDirent=FS_ReadDir(fsDir);
			res=f_readdir (fsDir, fsDirent);

			if(res == FR_OK)
			{
				//strcpy(buf,fsDirent->d_name);
				strcpy(buf,fsDirent->fname);
				CDispDir_DelSpace(buf);
				strcpy(dest,path);
				strcat(dest,"\\");
				strcat(dest,buf);

				if(fsDirent->fattrib!=16)
				{
					//FS_Remove(dest);
					res=f_unlink (dest);
				}
				else
				{
					CDispDir_DelDir(dest);
				}
			}

		}while(res == FR_OK);

		//FS_CloseDir(fsDir);
		//FS_RmDir(path);
		res = f_closedir(fsDir);
		res=f_unlink (path);
	}


}


//INT16S  CopyFatToFatFile(INT16S FileD, INT16S FileS)
BOOL	CDispDir_CopyFattoFat(INT8S *dest,INT8S *source)
{
	FIL 	fd1,fd2;
	FRESULT	res1,res2;
	UINT	br,bw;
	OS_ERR	os_err;
//	CPU_TS  p_ts;
	INT8U	*FileCopyBuf=NULL;
	DWORD	lFileLen=0;

//	OSSemPend(&FileSem,5000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);

	if(os_err!=OS_ERR_NONE)
	{
		CMsg_QPost("ReadData FromFile Sem Timeout!");
		OSTimeDly(100);
		return FALSE;
	}

	if(strcmp(dest,source)==0)
	{
//		OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
		return 0;
	}
	else
	{
		res1 = f_open(&fd1,source,FA_OPEN_EXISTING|FA_WRITE|FA_READ);

		if( res1!=FR_OK )
		{
//			OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
			return 0;
		}

		res2 = f_open(&fd2,dest,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);

		if( res2!=FR_OK )
		{
//			OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
			return 0;
		}

		lFileLen=fd1.fsize;
		CDispDir_ProgressBarInit(lFileLen);
		//�ǳ���Ҫ�ĺ�����ָ����ʼƫ��
		res1=f_lseek(&fd1,0);
		res2=f_lseek(&fd2,0);
		FileCopyBuf=malloc(g_iBufSize);

		for(;;)
		{
			res1=f_read(&fd1,FileCopyBuf,g_iBufSize,&br);
			res2=f_write(&fd2,FileCopyBuf,br,&bw);
			CDispDir_DrawProgressBar();										//����������
			if((br != bw) || (br != g_iBufSize))
			{
				break;
			}
		}

		free(FileCopyBuf);
		f_close(&fd1);												//����ر��ļ���������Ĵ����ļ���f_close()�������ܱ���д��f_closedir()
		f_close(&fd2);
//		OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);

		return 1;
	}
}


/*
//Fat���ļ�������Fat�� ����2
BOOL	CDispDir_CopyFattoFat(INT8S *dest,INT8S *source)
{
	Uart_Printf("test1\n");
	//FS_FILE 	*fp2;													//fat���ļ�ָ��
	//FS_FILE 	*fp1;
	FIL		*fp1;
	FIL 	*fp2;
	FRESULT	res;
	UINT 	br;
	UINT 	bw;
	OS_ERR		os_err;
	CPU_TS    	p_ts;

	INT32S		lFileLen=0;												//�������ļ��Ĵ�С
	//INT8U 		Semerr;													//�ź�����־λ
	INT32S j=0,len=0,len2=0;											//j�洢������ɺ��ļ��Ĵ�С��len��һ�ζ����ڴ�Ĵ�С��len2��һ��д���ڴ�Ĵ�С
	INT32S TimeBegin=0,TimeEnd=0;										//������ʼʱ������ʱ�䣬������
	char *FileCopyBuf=NULL;												//�ڴ�ָ��
	fp1=(FIL*)malloc_k(1024*1024*10,"fp1");										//��������ʱ�����ͷ��ڴ�ռ�
	fp2=(FIL*)malloc_k(1024*1024*10,"fp2");
	OSSemPend(&FileSem,5000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);

	if(os_err!=0)
	{
		Uart_Printf("�ȴ��ź�������\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"�ȴ��ź�����ʱ",0);
		return FALSE;
	}

	if(strcmp(dest,source)==0)											//��ʼ��ַ��Ŀ���ַ�ظ�
	{
		OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
		return TRUE;
	}
	else
	{
		//fp1 = FS_FOpen(source,"rb");
		res=f_open(fp1,source,FA_OPEN_EXISTING|FA_READ);
		if(res != FR_OK)
		{
			Uart_Printf("fp1 err\n");
			OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
			return FALSE;
		}

		//lFileLen=fp1->size;												//Դ�ļ���С
		lFileLen=fp1->fsize;

		//fp2 = FS_FOpen(dest,"wb");
		res=f_open(fp2,dest,FA_CREATE_ALWAYS|FA_WRITE);

		if(res != FR_OK)
		{
			Uart_Printf("fp2 err\n");
			OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
			return FALSE;
		}

		CDispDir_ProgressBarInit(lFileLen);
		//FS_FSeek(fp1, 0, SEEK_SET);										//�ǳ���Ҫ�ĺ�����ָ����ʼƫ��
		//FS_FSeek(fp2, 0, SEEK_SET);
		f_lseek(fp1, 0);
		f_lseek(fp2, 0);

		//TimeBegin = OSTimeGet();
		TimeBegin =OSTimeGet(&os_err);
		FileCopyBuf = malloc(g_iBufSize);									//��̬�����ڴ�ռ�

		for(;;)
		{
			//len=FS_FRead(FileCopyBuf,1,g_iBufSize,fp1);					//ÿ�δ�fp1��ȡ�ĳ���Ϊg_iBufSize��д�뵽FileCopyBuf
			res=f_read (fp1, FileCopyBuf, g_iBufSize,&br);
			Uart_Printf("res1=%d\n",res);
			//Uart_Printf("len=%d\n",len);
			//len2=FS_FWrite(FileCopyBuf,1,len,fp2);						//ÿ��д��fp2�ĳ���Ϊlen��
			res=f_write (fp2, FileCopyBuf, br,&bw);
			Uart_Printf("res2=%d\n",res);
			//Uart_Printf("len2=%d\n",len2);
			CDispDir_DrawProgressBar();										//����������
			j+=br;
			Uart_Printf("test2=%d,test3=%d,test4=%d,test5=%d\n",br,bw,g_iBufSize,j);
			//if((fp1->fsize != fp2->fsize) || (fp1->fsize != g_iBufSize))	//��д���ʱ��len != g_iBufSize����д����ʱ��len != len2
			if((br!=bw)||(br!=g_iBufSize))									//br��bw��ʾʵ�ʶ�д��ʱ��
			{
				break;
			}
		}
		//Uart_Printf("CopyTime=%d\n",TimeEnd-TimeBegin);
		//Uart_Printf("j=%d\n",j);
		free(FileCopyBuf);													//�ͷ������ڴ�ռ�
		//TimeEnd=OSTimeGet();
		TimeEnd =OSTimeGet(&os_err);
		//j��ʾ�ܵ�д�볤��
		//FS_FClose(fp1);
		//FS_FClose(fp2);
		res=f_close(fp1);
		res=f_close(fp2);
		fp1=free_k(fp1,"fp1");
		fp2=free_k(fp2,"fp2");
		OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
		return TRUE;
	}
}
*/

#if 0
//Fat���ļ�������Yaffs��,�����ṹͬ�ϣ�����ʵ���в���
BOOL CDispDir_CopyFattoYaffs(INT8S *dest,INT8S *source)							//ע��ȫ�ֱ�����ʹ�ã������д�����У����ڴ˺������õ��Ӻ���������ȫ�ֱ����������ļ��Ĵ򿪡��رա���д���ִ���
{
	INT32S fw;															//yaffs�����ļ�ָ��
	FS_FILE *fp;														//fat�����ļ�ָ��
	INT32S err,len,j=0;
	INT32S lFileLen=0;													//�������ļ��Ĵ�С
	INT8U *buf=NULL;													//������ڴ�ָ��
	INT8U Semerr;
	OSSemPend(&FileSem,5000,&Semerr);

	if(Semerr!=OS_NO_ERR)
	{
		Uart_Printf("�ȴ��ź�������\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"�ȴ��ź�����ʱ",0);
		return FALSE;
	}

	fp=FS_FOpen(source,"r");

	if(!fp)
	{
		Uart_Printf("err@11\r\n");
		OSSemPost(&FileSem);
		return FALSE;
	}

	lFileLen=fp->size;
	CDispDir_ProgressBarInit(lFileLen);
	fw = yaffs_open(dest,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

	if(fw!=-1)
	{
		buf = malloc(g_iBufSize);
		//�ǳ���Ҫ�ĺ�����ָ����ʼƫ��
		err=yaffs_lseek(fw,0,SEEK_SET);
		FS_FSeek(fp, 0, SEEK_SET);

		for(;;)
		{
			len = FS_FRead(buf,1,g_iBufSize,fp);
			err=yaffs_write(fw,buf,len);
			CDispDir_DrawProgressBar();
			j=j+len;
			if((err != len) || (len != g_iBufSize))
			{
				break;
			}
		}

		free(buf);
	}

	else
	{
		Uart_Printf("err@2\r\n");
		CDispDir_Message(g_bEnglish?"yaffs haven't be formated!!!":"yaffs��δ��ʽ��!!!",0);
		OSTimeDly(500);
		OSSemPost(FileSem);
		return FALSE;
	}

	//Uart_Printf("j=%d\n",j);											//д���ļ����ܴ�С
	yaffs_close(fw);
	FS_FClose(fp);
	OSSemPost(FileSem);
	return TRUE;
}
#endif


#if 0
//Yaffs���ļ�������Yaffs��,�����ṹͬ�ϣ�����ʵ���в���
BOOL CDispDir_CopyYaffstoYaffs(INT8S *dest,INT8S *source)
{


	INT32S err,err2,fw1,fw2,j=0;
	INT8U 	Semerr;
	INT8U 	*YaffsCopyBuf=NULL;
	INT32S lFileLen=0;
	struct yaffs_stat yaffsBuf[1];

	OSSemPend(&FileSem,5000,&Semerr);

	if(Semerr!=OS_NO_ERR)
	{
		Uart_Printf("�ȴ��ź�������\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"�ȴ��ź�����ʱ",0);
		return FALSE;
	}

	if(strcmp(dest,source)==0)
	{
		OSSemPost(&FileSem);
		return TRUE;
	}
	else
	{
		yaffs_stat(source,yaffsBuf);
		lFileLen=yaffsBuf[0].st_size;
		fw1 = yaffs_open(source,O_RDONLY, S_IREAD | S_IWRITE);

		if( fw1==-1 )
		{
			OSSemPost(&FileSem);
			return FALSE;
		}

		fw2 = yaffs_open(dest,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

		if( fw2==-1 )
		{
			OSSemPost(&FileSem);
			return FALSE;
		}

		//err=yaffs_read(fw1,YaffsCopyBuf,err);							//errΪYaffs��Ŀ���ļ��Ĵ�С��ʹ�øú������ܶ�err��ʼ��
		//Uart_Printf("err=%d\n",err);
		CDispDir_ProgressBarInit(lFileLen);
		//�ǳ���Ҫ�ĺ�����ָ����ʼƫ��
		err=yaffs_lseek(fw1,0,SEEK_SET);
		err2=yaffs_lseek(fw2,0,SEEK_SET);
		YaffsCopyBuf=malloc(g_iBufSize);

		for(;;)
		{
			err=yaffs_read(fw1,YaffsCopyBuf,g_iBufSize);
			err2=yaffs_write(fw2,YaffsCopyBuf,err);
			CDispDir_DrawProgressBar();
			j=j+err2;

			if((err != err2) || (err != g_iBufSize))
			{
				break;
			}
		}

		//Uart_Printf("j=%d\n",j);                    						//jΪճ���Ľ���Ĵ�С
		free(YaffsCopyBuf);
		yaffs_close(fw1);
		yaffs_close(fw2);
		OSSemPost(&FileSem);
		return TRUE;
	}


}
#endif

#if 0
//Yaffs���ļ�������Fat��,�����ṹͬ�ϣ�����ʵ���в���
BOOL CDispDir_CopyYaffstoFat(INT8S *dest,INT8S *source)
{


	INT32S err,fw,len,j=0;
	FS_FILE *fp;
	INT8U 	Semerr;
	INT8U *buf=NULL;
	INT32S lFileLen=0;
	struct yaffs_stat yaffsBuf[1];

	OSSemPend(&FileSem,5000,&Semerr);

	if(Semerr!=OS_NO_ERR)
	{
		Uart_Printf("�ȴ��ź�������\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"�ȴ��ź�����ʱ",0);
		return FALSE;
	}

	fw = yaffs_open(source,O_RDONLY, S_IREAD | S_IWRITE);

	if( fw==-1 )
	{
		OSSemPost(&FileSem);
		return FALSE;
	}

	fp = FS_FOpen(dest,"wb");

	if( !fp )
	{
		OSSemPost(&FileSem);
		return FALSE;
	}

	buf = malloc(g_iBufSize);
	yaffs_stat(source,yaffsBuf);
	lFileLen=yaffsBuf[0].st_size;
	CDispDir_ProgressBarInit(lFileLen);
	//�ǳ���Ҫ�ĺ�����ָ����ʼƫ��
	err=yaffs_lseek(fw,0,SEEK_SET);
	FS_FSeek(fp, 0, SEEK_SET);

	for(;;)
	{
		err=yaffs_read(fw,buf,g_iBufSize);
		len = FS_FWrite(buf,1,err,fp);
		CDispDir_DrawProgressBar();
		j=j+len;

		if((err != len) || (len != g_iBufSize))
		{
			break;
		}
	}
	//Uart_Printf("j=%d\n",j);
	//Uart_Printf("fp->size=%d\n",fp->size);
	free(buf);
	yaffs_close(fw);
	FS_FClose(fp);
	OSSemPost(&FileSem);
	return TRUE;


}
#endif

//��ȡ�ļ��Ĵ�С
void CDispDir_GetFileLength(INT8S *source,INT8S *dest)
{
	#if FALSE
	struct yaffs_stat yaffsBuf[1];

	if(g_iCurDisk==0)													//yaffs
	{
		if(yaffs_stat(source, yaffsBuf) != -1)							//��ȡ�ļ�״̬
		{
	 		if(yaffsBuf[0].st_size < 1024)
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%4uB":"�ļ���С:%4uB",yaffsBuf[0].st_size);
	 		}
	 		else if(yaffsBuf[0].st_size < 1024*1024)					//�ļ�����С��1024*1024Bit
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%.2fKB":"�ļ���С:%.2fKB",(FP32)yaffsBuf[0].st_size/1024);
	 		}
	 		else														//�ļ����ݴ���1024*1024Bit
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%.2fMB":"�ļ���С:%.2fMB",(FP32)yaffsBuf[0].st_size/(1024*1024));
	 		}
		}
	}
	else
	#endif

	{
		//FS_FILE *fp=NULL;
		FIL		*fp=NULL;
		FRESULT	res;
		INT32U 	len=0;
		INT8S*  fileSize="\0";
		//fp = FS_FOpen(source,"rb");
		fp=(FIL*)malloc_k(1024*1024*10,"fp");
		res=f_open(fp,source,FA_OPEN_EXISTING|FA_READ);
		len=fp->fsize;
		//Uart_Printf("len=%d\n",len);

		/*
		 //����2
		 FIL File;
		 FRESULT res;
		 res=f_open(File,source,FA_OPEN_EXISTING|FA_READ);
		 len=File.fsize;

		 */

		strcpy(dest,g_bEnglish?"File Size:":tr("�ļ���С:"));

 		if(len< 1024)													//�ļ�����С��1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%4uB":tr("%4uB"),len);
 		}
 		else if(len < 1024*1024)										//�ļ�����С��1024*1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%.2fKB":tr("%.2fKB"),(FP32)len/1024);
 		}
 		else															//�ļ����ݴ���1024*1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)len/(1024*1024));
 		}
 		strcat(dest,fileSize);
		//FS_FClose(fp);
 		res=f_close(fp);
 		fp=free_k(fp,"fp");
	}
}


//��ȡyaffs����fat���̵Ŀռ��ʣ��ռ�
void CDispDir_GetDiskSpace(INT8S *source,INT8S *TotalSpace,INT8S *FreeSpace)
{
	FRESULT	res;
	FATFS	*fs;
	DWORD	fre_clust;
	DWORD   iTotalSpace=0,iFreeSpace=0;
	INT8S	*totalSize="\0",*leftSize="\0";
#if FALSE
	if(g_iCurFocus==0)
	{
		iFreeSpace  = yaffs_freespace("/flash");
		iTotalSpace = yaffs_totalspace("/flash");
	 	sprintf(StrTotalSpace,g_bEnglish?"Total Size:%.2fMB":"����ռ�:%.2fMB",(FP32)iTotalSpace/(1024*1024));
	 	sprintf(StrFreeSpace,g_bEnglish?"Free Size:%.2fMB":"ʣ��ռ�:%.2fMB",(FP32)iFreeSpace/(1024*1024));
	}
	else if(g_iCurFocus==1)
#endif

	if(g_iCurFocus==1)
	{
		//FS_DISKFREE_T info;
	 	//FS_IoCtl(source, FS_CMD_GET_DISKFREE, 0, &info);
	 	//iTotalSpace = info.total_clusters * info.sectors_per_cluster * info.bytes_per_sector;
	 	//iFreeSpace = info.avail_clusters * info.sectors_per_cluster * info.bytes_per_sector;
		res = f_getfree(source,&fre_clust,&fs);
		iFreeSpace=fre_clust*fs->csize*512;				//fre_clust��fs->free_clust��ֵ���
		iTotalSpace=(fs->n_fatent-2)*fs->csize*512;
		sprintf(totalSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)iTotalSpace/(1024*1024));
		strcpy(TotalSpace,g_bEnglish?"Total Size:":tr("����ռ�:"));
		strcat(TotalSpace,totalSize);

		sprintf(leftSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)iFreeSpace/(1024*1024));
		strcpy(FreeSpace,g_bEnglish?"Free Size:":tr("ʣ��ռ�:"));
		strcat(FreeSpace,leftSize);

	}
}


//�����ļ�ʱ��������Ϣ��ʼ��
void 	CDispDir_ProgressBarInit(INT32S FileLen)
{

	INT16S x1=  g_iDlgLeft+270;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-330;

	INT16S y1=	g_iDlgTop+230;
	INT16S y2=	g_iDlgTop+255;


	CDC_DrawWin24(x1-20,y1-50,x2-x1+125,100,g_bEnglish?"Copying...":tr("�ļ�������..."));
	CDC_OutText_UTF(x2+5,y1+1,"0.0%",BLUECOLOR,BGCOLOR,KZK_24);

	CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
	CDC_Line(x1,y1,x1,y2,BLACKCOLOR);

	CDC_Line(x1,y2,x2,y2,WHITECOLOR);
	CDC_Line(x2,y1,x2,y2,WHITECOLOR);

	CDC_PutColorBar(x1+1,y1+1,x2-x1-1,y2-y1-1,WHITECOLOR,0);

	g_iCurPercent=0;													//��ǰ���ưٷֱȳ�ʼ��0
	g_iPieceNum=FileLen/g_iBufSize;										//����������
	g_iPieceWidth=(FP32)(x2-x1-1)/g_iPieceNum;								//ÿ����Ŀ��
}


//ģ����Ƹ��ƹ��̵Ľ�����
void 	CDispDir_DrawProgressBar(void)
{
	INT8S Percent[10]="\0";

	INT16S x1=  g_iDlgLeft+270;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-330;

	INT16S y1=	g_iDlgTop+230;
	INT16S y2=	g_iDlgTop+255;


	g_iCurPercent++;
	g_iCurWidth=(g_iCurPercent*g_iPieceWidth);

	if(g_iCurWidth>=(x2-x1-1))
	{
		g_iCurWidth=(x2-x1-1);
	}

	sprintf(Percent,"%.1f%%",(FP32)g_iCurWidth*100/(x2-x1-1));
	CDC_OutText_UTF(x2+5,y1+1,Percent,BLUECOLOR,BGCOLOR,KZK_24);

	CDC_PutColorBar(x1+1,y1+1,(INT32S)g_iCurWidth,y2-y1-1,BLUECOLOR,1);

}


void CDispDir_DrawPicture(INT16S Type,INT16S x,INT16S y)
{
	if(Type==-1)														//����
	{
		CDC_Line(x+6,y+16,x+24,y+8,BLACKCOLOR);
		CDC_Line(x+6,y+16,x+28,y+30,BLACKCOLOR);
		CDC_Line(x+28,y+30,x+44,y+19,BLACKCOLOR);
		CDC_Line(x+44,y+19,x+24,y+8,BLACKCOLOR);
		CDC_Line(x+6,y+16,x+6,y+24,BLACKCOLOR);
		CDC_Line(x+28,y+30,x+28,y+40,BLACKCOLOR);
		CDC_Line(x+44,y+19,x+44,y+27,BLACKCOLOR);
		CDC_Line(x+6,y+24,x+28,y+40,BLACKCOLOR);
		CDC_Line(x+28,y+40,x+44,y+27,BLACKCOLOR);
	}
	else if(Type==0)													//����ͼ��
	{
		CDC_PutColorBar(x+5,y,38,48,DISKCOLOR,0);
		CDC_PutColorBar(x+5,y,6,6,WHITECOLOR,0);
		CDC_PutColorBar(x+5,y+42,6,6,WHITECOLOR,0);
		CDC_PutColorBar(x+37,y,6,6,WHITECOLOR,0);
		CDC_PutColorBar(x+37,y+42,6,6,WHITECOLOR,0);
		CDC_Fillellipse(x+11,y+6,6,6,DISKCOLOR);
		CDC_Fillellipse(x+11,y+42,6,6,DISKCOLOR);
		CDC_Fillellipse(x+37,y+6,6,6,DISKCOLOR);
		CDC_Fillellipse(x+37,y+42,6,6,DISKCOLOR);
		CDC_Fillellipse(x+11,y+6,2,2,WHITECOLOR);
		CDC_Fillellipse(x+11,y+42,2,2,WHITECOLOR);
		CDC_Fillellipse(x+37,y+6,2,2,WHITECOLOR);
		CDC_Fillellipse(x+37,y+42,2,2,WHITECOLOR);
		CDC_Arc(x+11,y+6,  6,180,270,1,BLACKCOLOR);
		CDC_Arc(x+11,y+42, 6,90, 180,1,BLACKCOLOR);
		CDC_Arc(x+37,y+6,  6,270,360,1,BLACKCOLOR);
		CDC_Arc(x+37,y+42, 6,0,  90, 1,BLACKCOLOR);
		CDC_Line(x+11,y,x+37,y,BLACKCOLOR);
		CDC_Line(x+11,y+48,x+37,y+48,BLACKCOLOR);
		CDC_Line(x+5,y+6,x+5,y+42,BLACKCOLOR);
		CDC_Line(x+43,y+6,x+43,y+42,BLACKCOLOR);
		CDC_Fillellipse(x+24,y+20,14,14,WHITECOLOR);
		CDC_Fillellipse(x+24,y+20,7,7,DISKCOLOR);
		CDC_Fillellipse(x+24,y+20,4,4,WHITECOLOR);
		CDC_Fillellipse(x+24,y+20,2,2,DISKCOLOR);

	}
	else if(Type==1)													//DAT�ļ�ͼ��
	{
	//	CDC_PutColorBar(x+10,y+9,28,34,BGCOLOR,0);
	//	CDC_Line(x+10,y+6,x+35,y+6,BGCOLOR);
	//	CDC_Line(x+10,y+7,x+36,y+7,BGCOLOR);
	//	CDC_Line(x+10,y+8,x+37,y+8,BGCOLOR);

		CDC_Line(x+10,y+5,x+34,y+5,BLACKCOLOR);
		CDC_Line(x+10,y+5,x+10,y+43,BLACKCOLOR);
		CDC_Line(x+10,y+43,x+38,y+43,BLACKCOLOR);
		CDC_Line(x+38,y+43,x+38,y+9,BLACKCOLOR);
		CDC_Line(x+34,y+5,x+38,y+9,BLACKCOLOR);
		CDC_Line(x+14,y+15,x+34,y+15,BLACKCOLOR);
		CDC_Line(x+14,y+20,x+34,y+20,BLACKCOLOR);
		CDC_Line(x+14,y+25,x+34,y+25,BLACKCOLOR);
		CDC_Line(x+14,y+30,x+34,y+30,BLACKCOLOR);
	}
	else if(Type==2)													//δʶ���ļ�ͼ��
	{
	//	CDC_PutColorBar(x+10,y+9,28,34,BGCOLOR,0);
	//	CDC_Line(x+10,y+6,x+35,y+6,BGCOLOR);
	//	CDC_Line(x+10,y+7,x+36,y+7,BGCOLOR);
	//	CDC_Line(x+10,y+8,x+37,y+8,BGCOLOR);
		CDC_Line(x+10,y+5,x+34,y+5,BLACKCOLOR);
		CDC_Line(x+10,y+5,x+10,y+43,BLACKCOLOR);
		CDC_Line(x+10,y+43,x+38,y+43,BLACKCOLOR);
		CDC_Line(x+38,y+43,x+38,y+9,BLACKCOLOR);
		CDC_Line(x+34,y+5,x+38,y+9,BLACKCOLOR);
		CDC_OutText_UTF(x+18,y+12,"?",REDCOLOR,WHITECOLOR,KZK_24);
	}
	else if(Type==3)													//�ļ���ͼ��
	{
	/*
		CDC_PutColorBar(x+5,y+8,38,32,YELLOWCOLOR,0);

		CDC_PutColorBar(x+5,y+8,4,9,WHITECOLOR,0);
		CDC_PutColorBar(x+28,y+8,15,5,WHITECOLOR,0);

		CDC_Line(x+28,y+13,x+31,y+13,WHITECOLOR);
		CDC_Line(x+28,y+14,x+30,y+14,WHITECOLOR);

		CDC_Line(x+5,y+40,x+43,y+40,BLACKCOLOR);
		CDC_Line(x+43,y+40,x+43,y+13,BLACKCOLOR);
		CDC_Line(x+5,y+40,x+5,y+18,BLACKCOLOR);
		CDC_Line(x+5,y+18,x+30,y+18,BLACKCOLOR);
		CDC_Line(x+30,y+18,x+35,y+13,BLACKCOLOR);
		CDC_Line(x+43,y+13,x+35,y+13,BLACKCOLOR);
		CDC_Line(x+10,y+18,x+10,y+8,BLACKCOLOR);
		CDC_Line(x+10,y+8,x+27,y+8,BLACKCOLOR);
		CDC_Line(x+27,y+8,x+27,y+15,BLACKCOLOR);
		CDC_Line(x+27,y+15,x+33,y+15,BLACKCOLOR);
		*/

		CDC_Line(x+10,y+5,x+34,y+5,BLACKCOLOR);		//  һ
		CDC_Line(x+10,y+5,x+10,y+43,BLACKCOLOR);	//	1
		CDC_Line(x+10,y+43,x+38,y+43,BLACKCOLOR);	//  -

		CDC_Line(x+38,y+43,x+38,y+9,BLACKCOLOR);	//  1
		CDC_Line(x+34,y+5,x+38,y+9,BLACKCOLOR);		//  ��

		CDC_Line(x+14,y+10,x+25,y+10,BLACKCOLOR);
		CDC_Line(x+14,y+15,x+34,y+15,BLACKCOLOR);

		CDC_PutColorBar(x+5,y+18,38,26,YELLOWCOLOR,0);
		CDC_Line(x+5,y+18,x+5,y+18+26,BLACKCOLOR);
		CDC_Line(x+5,y+18,x+5+38,y+18,BLACKCOLOR);

		CDC_Line(x+5,y+18+26,x+5+38,y+18+26,BLACKCOLOR);
		CDC_Line(x+5+38,y+18,x+5+38,y+18+26,BLACKCOLOR);


	}
}

unsigned char OpenDir(void *DirHand,char *DirName)
{
	static char DirNameTem[100];
	int len;
	INT8U Error = 0;
	FRESULT res;

	if( 0 == strncmp(DirName,"0:",2) )		//C�� fatfs�ļ�ϵͳ
	{
		strcpy(DirNameTem,DirName);
		len = strlen(DirNameTem);
		
		if(DirNameTem[len-1] == '\\')
			DirNameTem[len-1] = '\0';
		
		res = f_opendir((DIR *)DirHand,(char *)DirNameTem);

		if(res != FR_OK)
			{Error = res;goto Err;}
	}
	else if( 0 == strncmp(DirName,"3:",2) )	//U�� fatfs�ļ�ϵͳ
	{
		strcpy(DirNameTem,DirName);
		len = strlen(DirNameTem);
		
		if(DirNameTem[len-1] == '\\')
			DirNameTem[len-1] = '\0';

		
		//printf("g_iDispDirPageNum2=%d\n",g_iDispDirPageNum);

		res = f_mount(&g_ufs,"3:",1);

		//printf("g_iDispDirPageNum2=%d\n",g_iDispDirPageNum);
		
		if(res != FR_OK)
			{Error = res;goto Err;}
		
		res = f_opendir((DIR *)DirHand,(char *)DirNameTem);
		
		if(res != FR_OK)
			{Error = res;goto Err;}
	}
	else
	{
		Error = 0xff;
		goto Err;
	}
Err:
	return Error;
}

static char lDirFileName[100];


unsigned char ReadDir(void *DirHand,char *DirName,DIRFS *DirInf)
{
	static FILINFO fno2[4];
	INT8U Error = 0;
	FRESULT res;
	int i;
#if 1

	memset(lDirFileName,0x00,100);
	if( 0 == strncmp(DirName,"0:",2) )		//C�� fatfs�ļ�ϵͳ
	{
		fno2[0].lfname = lDirFileName;
		fno2[0].lfsize = 100;
		res = f_readdir((DIR *)DirHand,&fno2[0]);

		if(res != FR_OK)
			{Error = res;goto Err;}

		//printf("res2=%d\n",res);
		
		if(strlen(lDirFileName) != 0)
		{
			strcpy((char *)DirInf->FileName,fno2[0].lfname);
		}
		else
		{
			strcpy((char *)DirInf->FileName,fno2[0].fname);
		}
		DirInf->attrib = fno2[0].fattrib;							//Ŀ¼���ļ�����

		
	}

	else if( 0 == strncmp(DirName,"3:",2) )	//U��
	{
		fno2[3].lfname = lDirFileName;
		fno2[3].lfsize = 100;
		
		res = f_readdir((DIR *)DirHand,&fno2[3]);

		if(res != FR_OK)
			{Error = res;goto Err;}
		
		//printf("res2=%d\n",res);
		
		if(strlen(lDirFileName) != 0)
		{
			strcpy((char *)DirInf->FileName,fno2[3].lfname);
		}
		else
		{
			strcpy((char *)DirInf->FileName,fno2[3].fname);
		}
		DirInf->attrib = fno2[3].fattrib;							//Ŀ¼���ļ�����


	}
	else
	{
		Error = 0xff;
		goto Err;
	}
#endif
Err:

	return Error;
}


