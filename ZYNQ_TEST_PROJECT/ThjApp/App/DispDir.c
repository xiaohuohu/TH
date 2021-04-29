#include	"includes.h"
#include	"base.h"
#include	"dispdir.h"

#define		MAXFILECOUNT		0x400                                   //最大文件存储量
#define		BGCOLOR				DARKGRAYCOLOR							//背景颜色 灰色
#define		DISKCOLOR 			DARKGRAYCOLOR							//绘制硬盘图标的颜色

#define 	DISPX				73										//文件或文件夹名字位置的起始X偏移量
#define 	DISPY				110										//文件或文件夹名字位置的起始Y偏移量

#define		DISPDIRITEMWIDTH	190 									//文件之间的横向偏移值
#define		DISPDIRITEMHEIGHT	68										//文件之间的纵向偏移值  

INT16S 		usb_max_devs;									//支持usb设备最大数目


TEST_EXT    OS_SEM 		FileSem;                                       //文件拷贝任务的信号量
TEST_EXT	INT16S 		g_iDlgLeft;										//边框的左侧坐标
TEST_EXT	INT16S		g_iDlgTop;										//边框顶端坐标
TEST_EXT	INT16S		g_iDlgWidth;									//边框的宽度
TEST_EXT	INT16S		g_iDlgHeight;									//边框的高度
TEST_EXT	INT16S		g_iDlgIDOk;										//Do_Modal程序结束的返回值
TEST_EXT	BOOL		Ok_Exit;										//退出标志量
TEST_EXT	BOOL		g_bEnglish;		
//extern 		FATFS		g_ufs;

INT16S      g_iPageStart = 0;
INT16S		g_iCurFocus=0;												//当前光标所在位置的标号

INT8S		g_cPath[0x80]="\0";									       	//当前所在界面的路径，例如进入ICO文件夹以后，g_cPath=0:\ADT\ICO
INT8S		g_cSrcPath[0x80]="\0";										//光标所在的位置对应的文件或文件夹的完整路径，例如进入ADT文件夹后，光标在ICO文件夹处，g_cSrcPath=0:\ADT\ICO
INT16S		g_iTotalCount=0;											//打开的一个磁盘或者文件夹的文件数目
BOOL		g_bDirCopy=FALSE;											//文件夹复制标志量
INT16S		g_iCurDisk=-1;												//-1为根目录，0为yaffs磁盘，1为fat磁盘，2为usb                                                  																
//FS_DIR		*g_pDir;      												//FAT文件系统结构体全局变量
DIR			*g_pDir;
//yaffs_DIR  	*g_pyaffsDir; 												//Yaffs文件系统结构体全局变量
FileEntity	*g_pFileEntity;  											//结构体，包含界面中文件或者文件夹的名字，位置，类型信息
FileEntity	g_FileEntityArray[MAXFILECOUNT];							//磁盘最大1024个目录以及文件项，根目录只有三个盘

INT16S 		g_iDispDirRowNum;	 										//行数
INT16S 		g_iDispDirColNum;											//列数	

static 		INT16S		g_iDispDirPageNum;											//显示每面显示的文件数											//是否处在根目录界面

BOOL        g_bFromFat=TRUE;											//true表示从FAT复制，false表示从Yaffs复制
BOOL        g_bToFat=TRUE;												//true表示粘贴到FAT，false表示粘贴到Yaffs 
INT32S 		g_iBufSize=1024*500;                                        //文件复制时，每一个块的容量，500Kb
INT32S 		g_iCurPercent=0,g_iPieceWidth=0,g_iPieceNum=0,g_iCurWidth=0;//已拷贝的百分比，每一个块宽度，块数，当前进度条宽度
BOOL        g_bUsbFirstRead=TRUE;										//第一次进入U盘的表质量，第一次进入之前true，进入之后置为false

static FATFS	g_ufs;

												
//各个磁盘界面信息参数初始化
void	CDispDir_Init(INT16S left,INT16S top,INT16S width,INT16S height,INT16S SysDiskNum)
{
	Ok_Exit=FALSE;
	g_iDlgIDOk=TRUE;
	g_iCurDisk=SysDiskNum;												//-1表示根目录，0表示Yaffs，1表示Fat，2表示外接的U盘
	g_iCurFocus=0;
	g_iPageStart = 0;
	
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;
	
	g_iDispDirColNum =(g_iDlgWidth-20)/DISPDIRITEMWIDTH;				//列数
 	g_iDispDirRowNum =(g_iDlgHeight-190)/DISPDIRITEMHEIGHT;				//行数					
	g_iDispDirPageNum=g_iDispDirRowNum*g_iDispDirColNum;				//每页显示的项目数
	
	memset(g_cPath,NULL,0x80);
	Uart_Printf("g_iCurDisk=%d\n",g_iCurDisk);
	
	switch(g_iCurDisk)
	{
		case -1:														//表示根目录												
			strcpy(g_cPath,g_bEnglish?"My Devices":tr("我的设备"));
			break;
		case 0:
			//现在没有YAFFS盘
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
	
	memset(g_cSrcPath,NULL,0x80);	 									//重点注意：涉及磁盘之间的拷贝，不能进行初始化。粘贴结束后才进行初始化
	memset(g_FileEntityArray,NULL,sizeof(FileEntity)*MAXFILECOUNT);

}

//创建界面，显示界面
void   CDispDir_Create()												
{
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"File Management":tr("文件管理"));

	//按钮状态设为凸起
	CDispDir_ButtonStatus(0, 1);
	
 	CDispDir_UpDataFilePath(g_bEnglish?"My Devices":tr("我的设备"),1);
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
		//磁盘的名称显示位置
		strcpy(g_FileEntityArray[0].name,g_bEnglish?"Local Disk(C:)":tr("本地磁盘(C:)"));
		strcpy(g_FileEntityArray[1].name,g_bEnglish?"Local Disk(D:)":tr("本地磁盘(D:)"));
		strcpy(g_FileEntityArray[2].name,g_bEnglish?"U Disk(E:)":tr("移动磁盘(E:)"));
		g_iTotalCount=3;
		strcpy(g_cPath,g_bEnglish?"My Devices":tr("我的设备"));
		
		for(DiskID=0;DiskID<g_iTotalCount;DiskID++)
		{
			g_FileEntityArray[DiskID].attrib=2;	
		}	
	}
	else if(g_iCurDisk==1||g_iCurDisk==2)							//fat盘和usb
	{
		CDispDir_ReadFatDir(g_cPath);								//读取fat磁盘文件或文件夹的函数
	}	
			
	if(g_iTotalCount<=0)
	{
		Uart_Printf(g_bEnglish?"Dir Empty":"目录为空,没有文件");
		CDispDir_UpDataFilePath(g_cPath,0);							//更新路径
		CDispDir_Message(g_bEnglish?"Dir Empty":tr("目录为空,没有文件"),0);
	}
	else
	{
		INT16S  i = 0;
		INT16S  dx = 0;
		INT16S  dy = 0;
		INT8S 	SrcPath[0x80]="\0";									//重点注意：子函数中尽量不要使用全局变量，对造成对全局变量的值的干扰，导致程序bug
		INT8S 	FileSize[0x30]="\0";

	
		CDispDir_UpDataFilePath(g_cPath,0);							//更新路径
		CDispDir_Sort();											//文件和文件夹按照名字排序

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
			
			if(g_pFileEntity->attrib==2)      						 	//显示磁盘      					
			{
				CDispDir_DrawPicture(0,dx,dy);
			}		
			else if(g_pFileEntity->attrib==1)							//显示文件夹图标
			{
				CDispDir_DrawPicture(3,dx,dy);
			}
			else if((strstr(g_pFileEntity->name,(char*)".DAT"))||(strstr(g_pFileEntity->name,(char*)".dat")))
			{
				CDispDir_DrawPicture(1,dx,dy);                 			//显示图片文件图标
			}
			else
			{
				CDispDir_DrawPicture(2,dx,dy);							//显示其他类型文件图标
			}
			
			CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,
					g_pFileEntity->name,BLACKCOLOR,WHITECOLOR);          //图标右侧显示文件名称

		}
				
		g_pFileEntity = &g_FileEntityArray[iPageStart+g_iCurFocus];																	
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,
						g_pFileEntity->name,WHITECOLOR,DARKBLUECOLOR);			//光标所在处，颜色加以区别
		
		//读取光标所在处的文件的大小，文件夹忽略
		if(g_pFileEntity->attrib==2)									//各个磁盘的空间								
		{
			INT8S  source[80]="";
			INT8S  StrTotalSpace[30]="";
			INT8S  StrFreeSpace[30]="";
			//Uart_Printf("here0=%s,here0=%s\n",StrTotalSpace,StrFreeSpace);
			//strcpy(source,"/flash");
			CDispDir_GetDiskSpace(source,StrTotalSpace,StrFreeSpace);			//获取磁盘的总空间和剩余空间并显示
			CDispDir_UpDataFileInfo(g_pFileEntity->name,StrTotalSpace,StrFreeSpace,0);
		}	
		else if(g_pFileEntity->attrib==0)								//文件（非文件夹）
		{
			INT8S buf[15]="\0";
			
			strcpy(SrcPath,g_cPath);
			
			if(g_iCurDisk==0)											//yaffs和Fat对于文件路径的存储方式不一样，加以区分
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
			CDispDir_GetFileLength(SrcPath,FileSize);					//获取文件的大小并显示
			
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
		CDC_PutColorBar(x1+1,y1+1,x2-x1-2,y2-y1-2,WHITECOLOR,0);				//目录栏区域绘制

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
		CDC_PutColorBar(x1+1,y1+1,x2-x1-2,y2-y1-2,WHITECOLOR,0);				//目录栏区域绘制
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
	
	CDC_PutColorBar(x1+1,y1+1,x2-x1-1,y2-y1-1,LIGHTGRAYCOLOR,0);				//文件大小显示区域绘制

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
		CDC_PutColorBar(x1+2,y1+2,x2-x1-2,y2-y1-2,BGCOLOR,0);				//提示信息区域绘制
		CDC_OutText_UTF(x1+6,y1+4,message,BLACKCOLOR,BGCOLOR,KZK_24);
	}
}



//按钮状态绘制
void CDispDir_ButtonStatus( INT16S ibtn,INT16S init)
{

	static	CRect 	rect[5];									//提示按扭的边框

	if(init)
	{
		INT16S	i=0;
		INT16S  ButtonWidth = 160;

		for(i=0;i<5;i++)										//界面按钮信息初始化
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
			CDC_OutCenTextKzk24(rect[0],tr("1-复制"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[1],tr("2-粘贴"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[2],tr("3-删除"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[3],tr("4-上一层"),YELLOWCOLOR,BLACKCOLOR);
			CDC_OutCenTextKzk24(rect[4],tr("5-根目录"),YELLOWCOLOR,BLACKCOLOR);

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

//文件系统退出时，释放资源
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

//去掉文件名中的空格
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

//刷新文件显示界面
void	CDispDir_UpdateObject(INT16S dk)
{
	INT8S FileSize[0x30]="\0";
	INT8S SrcPath[0x80]="\0";											//重点注意：子函数中尽量不要使用全局变量，对造成对全局变量的值的干扰，导致程序bug

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
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,g_pFileEntity->name,BLACKCOLOR,WHITECOLOR);					//使原光标所在处失去焦点

	}
	
	g_iCurFocus += dk;
	
	//系统根目录移动光标
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
		

		CDispDir_GetDiskSpace(source,StrTotalSpace,StrFreeSpace);		//获取磁盘的总空间和剩余空间并显示

		CDispDir_UpDataFileInfo(g_pFileEntity->name,StrTotalSpace,StrFreeSpace,0);

	}
	else														//进入各个磁盘后移动光标
	{
		g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];
		
		CDC_OutText(DISPX+g_pFileEntity->x,DISPY+g_pFileEntity->y,g_pFileEntity->name,WHITECOLOR,DARKBLUECOLOR);				//新的光标所在处获得焦点
		
		if(g_pFileEntity->attrib!=1)										//读取光标所在处的文件的大小，文件夹忽略
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
			CDispDir_GetFileLength(SrcPath,FileSize);						//文件的大小

			CDispDir_UpDataFileInfo(g_pFileEntity->name,"",FileSize,0);
		}
		else
		{
			CDispDir_UpDataFileInfo(g_pFileEntity->name,"","",0);
		}
	}
}



//返回上一页
void	CDispDir_BackPreDir(void)
{
	INT16S i=0;
	
	if(g_iCurDisk==0)
	{
		if(strlen(g_cPath)<7)											//当前界面为Yaffs磁盘的根目录，则返回到系统的根目录	
		{
			g_iCurDisk=-1;
		}
		else                                                            //如果不是yaffs的根目录，则退到上一级目录
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
	else if(g_iCurDisk==1||g_iCurDisk==2)               				//fat和usb
	{
		//if(strlen(g_cPath)<5)											//当前界面为fat磁盘的根目录，则返回到系统的根目录
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

//进入下一级目录
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
	strcat(g_cPath,g_pFileEntity->name);							//进入文件夹，这个函数可以改变  g_cPath 的值，
	
	g_iPageStart=0;
	g_iCurFocus =0;	
	CDispDir_UpDataView(g_iPageStart);

}

//根据文件或者文件夹的名字排序。文件夹放前面，文件放后面
void	CDispDir_Sort(void)
{
	INT16S 		i,j;
	INT16S 		dirnum=0;
	FileEntity	Tmp;

	for(i=0; i<g_iTotalCount; i++)										//该循环函数实现将文件夹放到前面文件放在后面
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
	
	for(i=0; i<g_iTotalCount; i++)										//计算文件夹目录的数量
	{										
		if(g_FileEntityArray[i].attrib==1)
			dirnum++;
	}
	
	for(i=0; i<dirnum; i++)												//文件夹按名字排序
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
	
	for(i=dirnum; i<g_iTotalCount; i++)								//文件按名字排序，文件排在文件夹后
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

//粘贴、删除等操作的"确定"与否选择
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

//检查当前目录里是否有要被复制的文件名。str为文件名，attr为1则是文件夹，为0是文件，path是当前界面的路径
BOOL	CDispDir_SearchFile(INT8S *str,INT8S attr,INT8S *path)					
{	
	FRESULT res;
#if FALSE

	if(g_iCurDisk==0)													//Yaffs盘
	{
		struct  yaffs_dirent *yaffsDirent;
		char   buf[0x10];
		yaffs_DIR *yaffsDir;											//重点注意：之前用的g_bYaffsDir，全局变量会引起其他引用全局变量的地方产生变化
		
		yaffsDir =yaffs_opendir(path);
		
		if(!yaffsDir)
		{
			Uart_Printf("Open Path Err !Path=%s\n",path);
		//	CDC_PutColorBar(11,76,g_iDlgWidth-21,g_iDlgHeight-191,WHITECOLOR,0);
			
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("目录打开错误"),0);
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
					case 1:												//attr为1，则选取查找文件夹
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
						
					case 0:												//attr为1，则选取查找文件
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
	else if(g_iCurDisk==1||g_iCurDisk==2)								//FAT盘
#endif

	if(g_iCurDisk==1||g_iCurDisk==2)								//FAT盘
	{
		//struct FS_DIRENT *direntp;
		FILINFO direntp;
		DIR pDir;													//注意使用局部变量
		char   buf[0x10];
		
		//pDir = FS_OpenDir(path);
		res = f_opendir (&pDir, &path);								/* Open a directory */
		
		if(res != FR_OK)
		{
			Uart_Printf("Open Path Err !Path=%s\n",path);
		//	CDC_PutColorBar(11,76,g_iDlgWidth-21,g_iDlgHeight-191,WHITECOLOR,0);
		
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("目录打开错误"),0);
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

//获取path的最后一个文件或者文件夹的名字  例如path=0:\ADT\ICO，则name=ICO
void	CDispDir_GetFileName(INT8S *name,INT8S *path)							
{
	INT16S i,j;
	
	if(g_bFromFat==FALSE)												//被复制的文件位于yaffs
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
	else if(g_bFromFat==TRUE)											//被复制的文件位于fat
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



//文件夹复制
BOOL	CDispDir_CopyDir(INT8S *destpath,INT8S *srcpath) 				//仅COPY当前相应目录下的文件，对于子目录不COPY
{


	char dest[0x80];
	char src[0x80];
#if FALSE
	if(g_bFromFat==FALSE&&g_bToFat==FALSE)								//从yaffs拷贝到yaffs
	{
		struct yaffs_dirent *yaffsDirent;
		g_pyaffsDir =yaffs_opendir(srcpath);

		//Uart_Printf("yaffs到yaffs的文件夹拷贝\n\r");
		if(!g_pyaffsDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"目录打开错误 Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"目录打开错误",0);
			return FALSE;
		}

		//循环遍历源文件夹下的所有文件（子文件夹除外）
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

				if(yaffsDirent->d_type!=0x03)							//源文件夹如果还有子文件夹，子文件夹则不属于拷贝范围，只拷贝文件							//目录里的文件复制
				{
					strcpy(src,srcpath);
					strcat(src,"/");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"/");
					strcat(dest,buf1);									//被复制的文件在目标文件夹的新路径

					CDispDir_GetFileName(name,dest);
					//下面的if判断语句层出现问题，被调用的子函数中尽量不要引入全局变量
					if(CDispDir_SearchFile(name,0,destpath))			//查看目标目录里是否以有重名的文件名
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"复制文件...",0);

							if(!CDispDir_CopyYaffstoYaffs(dest,src))				//yaffs到yaffs的文件拷贝函数
							{
								yaffs_closedir(g_pyaffsDir);
								g_pyaffsDir = NULL;
								Uart_Printf("Copy Dir Copy File Err1!\n");
								return FALSE;
							}
						}
						else
						{
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"取消文件覆盖",0);
						}
					}
					else												//无重复文件
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
	//else if(g_bFromFat==TRUE&&g_bToFat==TRUE)							//从fat拷贝到fat
	if(g_bFromFat==TRUE&&g_bToFat==TRUE)
	{
		FRESULT		res;

		//struct FS_DIRENT *direntp;
		FILINFO* direntp;
		//g_pDir = FS_OpenDir(srcpath);
		res=f_opendir (g_pDir, srcpath);

		if(res != FR_OK)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err\n":"目录打开错误 \n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":tr("目录打开错误"),0);
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

				if(direntp->fattrib!=0x10)						//源文件夹里的文件复制，子文件夹忽略
				{
					strcpy(src,srcpath);
					strcat(src,"\\");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"\\");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//查看目标目录里是否以有要自制的文件名
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)\n",buf1);
						//sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)",buf1);
						strcpy(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":tr("存在同名文件,确定覆盖?(Y/N)"));
						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":tr("复制文件..."),0);

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
							CDispDir_Message(g_bEnglish?"Overwrite cancel":tr("取消文件覆盖"),0);
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
	else if(g_bFromFat==TRUE&&g_bToFat==FALSE)							//从fat拷贝到yaffs
	{
		struct FS_DIRENT *direntp;
		g_pDir = FS_OpenDir(srcpath);

		if(!g_pDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"目录打开错误 Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"目录打开错误",0);
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

				if(direntp->FAT_DirAttr!=16)						//目录里的文件复制
				{
					strcpy(src,srcpath);
					strcat(src,"\\");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"/");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//查看目标目录里是否以有要自制的文件名
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"复制文件...",0);

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
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"取消文件覆盖",0);
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

	else if(g_bFromFat==FALSE&&g_bToFat==TRUE)							//从yaffs拷贝到fat
	{
		struct yaffs_dirent *yaffsDirent;
		g_pyaffsDir =yaffs_opendir(srcpath);
		//Uart_Printf("yaffs到Fat的文件夹拷贝\n");
		if(!g_pyaffsDir)
		{
			Uart_Printf(g_bEnglish?"Dir Open Err,Copy Dir":"目录打开错误 Copy Dir\n");
			CDispDir_Message(g_bEnglish?"Dir Open Err":"目录打开错误",0);
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

				if(yaffsDirent->d_type!=0x03)						//目标目录如果还有子目录，子目录则不属于拷贝范围，只拷贝文件							//目录里的文件复制
				{
					strcpy(src,srcpath);
					strcat(src,"/");
					strcat(src,buf1);

					strcpy(dest,destpath);
					strcat(dest,"\\");
					strcat(dest,buf1);

					CDispDir_GetFileName(name,dest);

					if(CDispDir_SearchFile(name,0,destpath))		//查看目标目录里是否以有重名的文件名
					{
						Uart_Printf(g_bEnglish?"Exist The Same File,Are You Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)\n",buf1);
						sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)",buf1);

						CDispDir_Message(buf,0);

						if(CDispDir_Confirm())
						{
							CDispDir_Message(g_bEnglish?"Copy file...":"复制文件...",0);

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
							CDispDir_Message(g_bEnglish?"Overwrite cancel":"取消文件覆盖",0);
						}
					}
					else											//无重复文件
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



//各个磁盘的按键响应
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
			CDispDir_UpdateObject(-g_iDispDirColNum);						//上移按键，光标位置值减少一行的文件数
			break;

		case TDOWN:
			CDispDir_UpdateObject(g_iDispDirColNum);						//上移按键，光标位置值增加一行的文件数
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
					if(g_iCurFocus==0)										//进入Yaffs
					{
						CDispDir_Message(g_bEnglish?"Disk C can not be Accessed!":tr("C盘无法访问                   "),0);
						/*
						g_iCurDisk=0;
						g_iCurFocus =0;
						g_iPageStart=0;
						strcpy(g_cPath,"/flash");
						CDispDir_UpDataView(g_iPageStart);
						*/
					}
					else if(g_iCurFocus==1)									//进入Fat盘
					{
						g_iCurDisk=1;
						g_iCurFocus =0;
						g_iPageStart=0;
						//strcpy(g_cPath,"smc:");
						strcpy(g_cPath,"0:");
						CDispDir_UpDataView(g_iPageStart);
					}
					else if(g_iCurFocus==2)									//进图U盘
					{

						if(g_bUsbFirstRead==TRUE)
						{
							INT16S usb_stor_curr_dev=-1;


							f_mount(0,"3:",0);

							UsbHostStop();
							
							Uart_Printf("正在查找U盘\n");
							usb_stor_curr_dev = UDiskScan();

							if(usb_stor_curr_dev == 0)
							{
								usb_max_devs=0;
								Uart_Printf("未找到U盘!\n");
							//	CDC_PutColorBar(11,g_iDlgHeight-74,g_iDlgWidth-21,29,BGCOLOR,0);
								CDispDir_Message(g_bEnglish?"NO USB Device":tr("没有插入U盘"),0);
								UsbHostStop();
								break;
							}
							else
							{
								usb_max_devs=1;
								Uart_Printf("找到U盘!\n");
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
					CDispDir_Message(g_bEnglish?"Can't Open this file!":tr("系统不能打开该文件"),0);
				}
			}
			break;

		case '1':														//复制功能，区分复制文件的情况：Yaffs盘中复制，FAT盘的复制，用标志量来区分
			{
				char buf[0x10];

				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Disk can't Be Copied":tr("磁盘不能被复制"),0);
					break;
				}

				g_pFileEntity = &g_FileEntityArray[g_iPageStart+g_iCurFocus];

				CDispDir_ButtonStatus(1,0);													//延时后显示弹起状态

				strcpy(g_cSrcPath,g_cPath);

				if(g_iCurDisk==0)
				{
					strcat(g_cSrcPath,"/");
					g_bFromFat=FALSE;									//置0，表示从Yaffs复制
				}
				else
				{
				    strcat(g_cSrcPath,"\\");
				    g_bFromFat=TRUE;									//置1，表示从fat复制
				}

				strcpy(buf,g_pFileEntity->name);
				CDispDir_DelSpace(buf);
				strcat(g_cSrcPath,buf);
				Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);				//被复制的文件名

				if(g_pFileEntity->attrib==1) 							//复制目录
				{
					INT8S tempBuf[0x30];
					g_bDirCopy=TRUE;									//复制文件夹标志位

					strcpy(tempBuf,g_bEnglish?"Copy Dir:":tr("复制目录:"));
					strcat(tempBuf,buf);
					CDispDir_Message(tempBuf,0);
				}
				else													//复制文件
				{
					INT8S tempBuf[0x30];

					g_bDirCopy=FALSE;									//复制文件标志位

					strcpy(tempBuf,g_bEnglish?"Copy File:":tr("复制文件:"));
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
					CDispDir_Message(g_bEnglish?"Disk can't Be Pasted":tr("磁盘不能被粘贴"),0);
					break;
				}

				CDispDir_ButtonStatus(2,0);

				if(g_iCurDisk==0)										//按下键值2时所在的界面为yaffs磁盘
				{
					g_bToFat=FALSE;										//置0，复制到yaffs
				}
				else if(g_iCurDisk==1||g_iCurDisk==2)
				{
					g_bToFat=TRUE;										//置1，复制到fat
				}

				if(g_bDirCopy) 	//目录粘贴
				{
					CDispDir_GetFileName(name,g_cSrcPath);				//获取最里子目录的名字，如果是g_cSrcPath=0:\ADT\ICO，则name=ICO
					//Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);
					//Uart_Printf("name=%s\n",name);

					if(CDispDir_SearchFile(name,1,g_cPath))				//g_cPath当前界面的路径。（name为文件夹，attr为1，且能搜索到返回true）
					{
						Uart_Printf(g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"存在同名目录%s,确定覆盖?(Y/N)\n",name);
						//sprintf(buf,g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"存在同名目录%s,确定覆盖?(Y/N)",name);
						strcpy(buf,g_bEnglish?"Exist Same Dir,Sure Overwrite(Y/N)":"存在同名目录,确定覆盖?(Y/N)");
						CDispDir_Message(buf,0);

						if(!CDispDir_Confirm())
						{
							Uart_Printf(g_bEnglish?"Cancel Paste File":tr("取消粘贴目录文件!"));

							CDispDir_Message(g_bEnglish?"Cancel Paste File":tr("取消粘贴目录文件!"),0);
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

							CDispDir_Message(g_bEnglish?"Copy Dir...":tr("复制目录..."),0);

							if(!CDispDir_CopyDir(curpath, g_cSrcPath))
							{
								Uart_Printf(g_bEnglish?"Paste Dir File Failed\n":"粘贴目录文件失败!\n");

								CDispDir_Message(g_bEnglish?"Paste Dir File Failed!":tr("粘贴目录文件失败!"),0);
							}
						}
					}
					else												//若无重名文件夹
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
							//if(FS_MkDir(curpath)!=0)					//Fat创建文件夹
							if(f_mkdir(curpath)!=FR_OK)
							{
								Uart_Printf("Dir Make Failed!\n");
							}
							Uart_Printf("curpath=%s\n",curpath);
						}
						/*
						else if(g_bToFat==FALSE)
						{
							if(yaffs_mkdir(curpath, S_IFDIR)!=0)		//yaffs创建文件夹
							{
								Uart_Printf("Dir Make Failed!\n");
							}
						}*/

						CDispDir_Message(g_bEnglish?"Copy Dir...":tr("复制目录..."),0);

						if(!CDispDir_CopyDir(curpath, g_cSrcPath))
						{
							Uart_Printf(g_bEnglish?"Paste Dir File Failed!\n":"粘贴目录文件失败!\n");

							CDispDir_Message(g_bEnglish?"Paste Dir File Failed!":tr("粘贴目录文件失败!"),0);
						}
					}

					Uart_Printf("Copy Dir OK!\n");
					CDispDir_UpDataView(g_iPageStart);
					CDispDir_Message(g_bEnglish?"Paste Dir File Complete!":tr("粘贴目录文件完成!"),0);
				}
				else													//文件粘贴
				{
					Uart_Printf("g_cSrcPath=%s\n",g_cSrcPath);
					Uart_Printf("g_cPath=%s\n",g_cPath);
					CDispDir_GetFileName(name,g_cSrcPath);
					Uart_Printf("name=%s\n",name);

					if(CDispDir_SearchFile(name,0,g_cPath))				//如果文件，且attr为0，且搜索得到name，返回true
					{
						Uart_Printf(g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)\n",name);
						//sprintf(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":"存在同名文件%s,确定覆盖?(Y/N)",name);
						strcpy(buf,g_bEnglish?"Exist Same File,Sure Overwrite(Y/N)":tr("存在同名文件,确定覆盖?(Y/N)"));
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
							//重名判断只是删除目标路径的重名文件，并不直接复制文件
							strcat(curpath,name);

							CDispDir_Message(g_bEnglish?"Delete File...":tr("删除旧文件..."),0);

							if(strcmp(curpath,g_cSrcPath)!=0)			//如果目标位置与源位置不同（如果相同不作处理，例如讲一个文件复制粘贴致当前位置）
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
							CDispDir_Message(g_bEnglish?"Cancel Paste File!":tr("取消粘贴文件!"),0);
							break;
						}
					}
					//else   											//else被注释掉，即无论是否文件重名，都将执行文件复制（重名判断函数中不包括复制文件）
					{
						//Uart_Printf("没有重名文件，进入else判断BGCOLOR=%d\n",BGCOLOR);
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

						CDispDir_Message(g_bEnglish?"Copy File...":tr("复制文件..."),0);
						#if(FALSE)
						if(g_bFromFat==TRUE&& g_bToFat==FALSE)
						{
							//Uart_Printf("从FAT复制，拷贝到yaffs\n");
							if(!CDispDir_CopyFattoYaffs(curpath,g_cSrcPath))
							{
								Uart_Printf("Copy File Err\n");
								CDispDir_Message(g_bEnglish?"Copy File Failed!":"复制文件失败!",0);
								break;
							}
						}
						#endif

						if(g_bFromFat==TRUE&& g_bToFat==TRUE)
						{
							//Uart_Printf("从FAT复制，拷贝到FAT\n");
							if(!CDispDir_CopyFattoFat(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":tr("复制文件失败!"),0);
								break;
							}
						}

						#if(FALSE)
						if(g_bFromFat==FALSE&&g_bToFat==FALSE)
						{
							//Uart_Printf("从yaffs复制，拷贝到yaffs\n");
							if(!CDispDir_CopyYaffstoYaffs(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":"复制文件失败!",0);
								break;
							}
						}

						if(g_bFromFat==FALSE&&g_bToFat==TRUE)
						{
							//Uart_Printf("从yaffs复制，拷贝到FAT\n");
							if(!CDispDir_CopyYaffstoFat(curpath,g_cSrcPath))
							{
								Uart_Printf("Paste File Err\n");
								CDispDir_Message(g_bEnglish?"Paste File Failed!":"复制文件失败!",0);
								break;
							}
						}
						#endif
					}

					CDispDir_UpDataView(g_iPageStart);
					CDispDir_Message(g_bEnglish?"Paste File Complete!":tr("粘贴文件完成!"),0);
				}
			}
			break;

		case '3':
			{
				char buf1[0x80];
				char buf[0x30];

				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Disk Can't Be Deleted":tr("磁盘不能被删除"),0);
					break;
				}


				CDispDir_ButtonStatus(3,0);

				//文件夹的删除
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
					Uart_Printf(g_bEnglish?"Delete File %s,Sure (Y/N)\n":"删除文件夹%s,确定删除??(Y/N)\n",g_pFileEntity->name);
					//sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":"删除文件夹%s,确定删除??(Y/N)",g_pFileEntity->name);
					strcpy(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":tr("删除文件夹,确定删除?(Y/N)"));
					CDispDir_Message(buf,0);

					if(CDispDir_Confirm())
					{
						Uart_Printf("Delete File ...\n");
						CDispDir_Message(g_bEnglish?"Delete File ...":tr("删除文件夹..."),0);
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
						CDispDir_Message(g_bEnglish?"Delete File Complete!":tr("删除文件夹完成!"),0);

						break;
					}
					else
					{
						Uart_Printf("Cancel Delete File!\n");
						CDispDir_Message(g_bEnglish?"Cancel Delete File!":tr("取消删除文件夹!"),0);
						break;
					}

				}
				else													//文件删除
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

					Uart_Printf(g_bEnglish?"Delete File %s,Sure (Y/N)\n":"删除文件%s,确定删除?(Y/N)\n",g_pFileEntity->name);
					//sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":"删除文件%s,确定删除??(Y/N)",g_pFileEntity->name);
					sprintf(buf,g_bEnglish?"Delete File %s,Sure (Y/N)":tr("删除文件,确定删除?(Y/N)"));
					CDispDir_Message(buf,0);

					if(CDispDir_Confirm())
					{
						Uart_Printf("Delete File ...\n");

						CDispDir_Message(g_bEnglish?"Delete File ...":tr("删除文件..."),0);

						if(g_iCurDisk==0)
						{
							//yaffs_unlink(buf1);							//Yaffs删除文件函数
						}
						else
						{
							//FS_Remove(buf1);							//FAT删除文件函数
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
						CDispDir_Message(g_bEnglish?"Delete File Complete!":tr("删除文件完成!"),0);
						break;
					}
					else
					{
						Uart_Printf("Cancel Delete File!\n");
						CDispDir_Message(g_bEnglish?"Cancel Delete File!":tr("取消删除文件!"),0);
						break;
					}
				}
			}
			break;

		case '4':														//返回上一级目录
			{
				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Already Root":tr("已经到根目录"),0);
					break;
				}


				CDispDir_ButtonStatus(4,0);

				CDispDir_BackPreDir();
			}
			break;
		case '5':														//返回根目录
			{
				if(g_iCurDisk==-1)
				{
					CDispDir_Message(g_bEnglish?"Already Root":tr("已经到根目录"),0);
					break;
				}


				CDispDir_ButtonStatus(5,0);

				g_iCurDisk=-1;
				g_iPageStart=0;
				g_iCurFocus =0;
				CDispDir_UpDataView(g_iPageStart);
			}

			break;
	#if	PRINTBMP													//屏幕截图 归零键
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


//各个磁盘的界面绘制以及执行操作的程序入口
INT16S    CDispDir_DoModal()
{
	INT16U key=0,tkey=0;

	CDispDir_Create();

	while(TRUE)
	{
		key = KeyMBoxGetKey();

		if(key != 0xffff)
		{
			CDispDir_OnKey(key,tkey);									//按键响应函数

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
//遍历目标目录，读取yaffs中的文件并赋值给数组
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

		Uart_Printf(g_bEnglish?"Dir Open Error!\n":"目录打开错误\n");
		CDispDir_Message(g_bEnglish?"Dir Open Error":"目录打开错误",0);
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
				Uart_Printf(g_bEnglish?"Dir too much over 1024\n":"目录和文件项超过1024个\n");
				CDispDir_Message(g_bEnglish?"Dir or File Too Much":"目录或文件项超过1024",0);
				break;
			}

			strcpy(buf,yaffsDirent->d_name);
			CDispDir_DelSpace(buf);
			strcpy(g_FileEntityArray[count].name,buf);
			g_FileEntityArray[count].x=(count%g_iDispDirRowNum)*DISPDIRITEMWIDTH;					//文件X坐标偏移量
			g_FileEntityArray[count].y=(count/g_iDispDirRowNum%g_iDispDirColNum)*DISPDIRITEMHEIGHT;	//文件Y坐标偏移量
			g_FileEntityArray[count].attrib=(yaffsDirent->d_type ==0x03?1:0);                   	//1表示文件夹，0表示文件
			count++;
		}

	}while(yaffsDirent);

	yaffs_closedir(yaffsDir);																//关闭打开的文件，释放资源
	yaffsDir=NULL;
	g_iTotalCount = count;

}
#endif


//遍历目标目录，读取fat中的文件并赋值给数组
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
			CDispDir_Message(g_bEnglish?"Dir Open Error":tr("目录打开错误"),0);
			return;
		}
		else if(g_iCurDisk==2)
		{
			g_bUsbFirstRead=TRUE;
			strcpy(g_cPath,oldbuf);
						
			CDispDir_Message(g_bEnglish?"NO USB Device":tr("没有插入U盘"),0);
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
					Uart_Printf(g_bEnglish?"Dir too much over 1024":tr("目录和文件项超过1024个"));
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


//文件夹或文件的删除
void	CDispDir_DelDir(INT8S *path)
{
#if 0

	if(g_iCurDisk==0)													//yaffs盘
	{
		char dest[0x80];
		char buf[0x10];
		struct  yaffs_dirent *yaffsDirent;
		yaffs_DIR  	*yaffsDir;
		yaffsDir= yaffs_opendir(path);
		//循环删除目标文件夹内部的文件或子文件夹
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
				if(yaffsDirent->d_type!=0x03)							//如果是文件直接删除
				{
					yaffs_unlink(dest);
				}
				else													//如果是文件夹，"递归调用"该函数
				{
					CDispDir_DelDir(dest);
				}
			}

		}while(yaffsDirent);

		yaffs_closedir(yaffsDir);
		yaffs_rmdir(path);												//删除目标文件夹自身（因为该函数只能删除空的文件夹，因此之前需要循环遍历来删除文件夹的内容）
	}
	else																//fat盘，原理同上
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
		//非常重要的函数，指针起始偏移
		res1=f_lseek(&fd1,0);
		res2=f_lseek(&fd2,0);
		FileCopyBuf=malloc(g_iBufSize);

		for(;;)
		{
			res1=f_read(&fd1,FileCopyBuf,g_iBufSize,&br);
			res2=f_write(&fd2,FileCopyBuf,br,&bw);
			CDispDir_DrawProgressBar();										//进度条加载
			if((br != bw) || (br != g_iBufSize))
			{
				break;
			}
		}

		free(FileCopyBuf);
		f_close(&fd1);												//必须关闭文件才能正真的创建文件。f_close()函数不能笔误写成f_closedir()
		f_close(&fd2);
//		OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);

		return 1;
	}
}


/*
//Fat盘文件拷贝至Fat盘 方法2
BOOL	CDispDir_CopyFattoFat(INT8S *dest,INT8S *source)
{
	Uart_Printf("test1\n");
	//FS_FILE 	*fp2;													//fat盘文件指针
	//FS_FILE 	*fp1;
	FIL		*fp1;
	FIL 	*fp2;
	FRESULT	res;
	UINT 	br;
	UINT 	bw;
	OS_ERR		os_err;
	CPU_TS    	p_ts;

	INT32S		lFileLen=0;												//拷贝的文件的大小
	//INT8U 		Semerr;													//信号量标志位
	INT32S j=0,len=0,len2=0;											//j存储拷贝完成后文件的大小，len是一次读入内存的大小，len2是一次写入内存的大小
	INT32S TimeBegin=0,TimeEnd=0;										//复制起始时间和完成时间，测试用
	char *FileCopyBuf=NULL;												//内存指针
	fp1=(FIL*)malloc_k(1024*1024*10,"fp1");										//函数结束时必须释放内存空间
	fp2=(FIL*)malloc_k(1024*1024*10,"fp2");
	OSSemPend(&FileSem,5000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);

	if(os_err!=0)
	{
		Uart_Printf("等待信号量故障\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"等待信号量超时",0);
		return FALSE;
	}

	if(strcmp(dest,source)==0)											//初始地址与目标地址重复
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

		//lFileLen=fp1->size;												//源文件大小
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
		//FS_FSeek(fp1, 0, SEEK_SET);										//非常重要的函数，指针起始偏移
		//FS_FSeek(fp2, 0, SEEK_SET);
		f_lseek(fp1, 0);
		f_lseek(fp2, 0);

		//TimeBegin = OSTimeGet();
		TimeBegin =OSTimeGet(&os_err);
		FileCopyBuf = malloc(g_iBufSize);									//动态申请内存空间

		for(;;)
		{
			//len=FS_FRead(FileCopyBuf,1,g_iBufSize,fp1);					//每次从fp1读取的长度为g_iBufSize，写入到FileCopyBuf
			res=f_read (fp1, FileCopyBuf, g_iBufSize,&br);
			Uart_Printf("res1=%d\n",res);
			//Uart_Printf("len=%d\n",len);
			//len2=FS_FWrite(FileCopyBuf,1,len,fp2);						//每次写入fp2的长度为len，
			res=f_write (fp2, FileCopyBuf, br,&bw);
			Uart_Printf("res2=%d\n",res);
			//Uart_Printf("len2=%d\n",len2);
			CDispDir_DrawProgressBar();										//进度条加载
			j+=br;
			Uart_Printf("test2=%d,test3=%d,test4=%d,test5=%d\n",br,bw,g_iBufSize,j);
			//if((fp1->fsize != fp2->fsize) || (fp1->fsize != g_iBufSize))	//读写完成时，len != g_iBufSize；读写错误时，len != len2
			if((br!=bw)||(br!=g_iBufSize))									//br和bw表示实际读写的时间
			{
				break;
			}
		}
		//Uart_Printf("CopyTime=%d\n",TimeEnd-TimeBegin);
		//Uart_Printf("j=%d\n",j);
		free(FileCopyBuf);													//释放申请内存空间
		//TimeEnd=OSTimeGet();
		TimeEnd =OSTimeGet(&os_err);
		//j表示总的写入长度
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
//Fat盘文件拷贝至Yaffs盘,基本结构同上，具体实现有差异
BOOL CDispDir_CopyFattoYaffs(INT8S *dest,INT8S *source)							//注意全局变量的使用！代码编写过程中，由于此函数调用的子函数包含有全局变量，导致文件的打开、关闭、读写出现错误。
{
	INT32S fw;															//yaffs磁盘文件指针
	FS_FILE *fp;														//fat磁盘文件指针
	INT32S err,len,j=0;
	INT32S lFileLen=0;													//拷贝的文件的大小
	INT8U *buf=NULL;													//申请的内存指针
	INT8U Semerr;
	OSSemPend(&FileSem,5000,&Semerr);

	if(Semerr!=OS_NO_ERR)
	{
		Uart_Printf("等待信号量故障\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"等待信号量超时",0);
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
		//非常重要的函数，指针起始偏移
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
		CDispDir_Message(g_bEnglish?"yaffs haven't be formated!!!":"yaffs区未格式化!!!",0);
		OSTimeDly(500);
		OSSemPost(FileSem);
		return FALSE;
	}

	//Uart_Printf("j=%d\n",j);											//写入文件的总大小
	yaffs_close(fw);
	FS_FClose(fp);
	OSSemPost(FileSem);
	return TRUE;
}
#endif


#if 0
//Yaffs盘文件拷贝至Yaffs盘,基本结构同上，具体实现有差异
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
		Uart_Printf("等待信号量故障\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"等待信号量超时",0);
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

		//err=yaffs_read(fw1,YaffsCopyBuf,err);							//err为Yaffs中目标文件的大小，使用该函数不能对err初始化
		//Uart_Printf("err=%d\n",err);
		CDispDir_ProgressBarInit(lFileLen);
		//非常重要的函数，指针起始偏移
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

		//Uart_Printf("j=%d\n",j);                    						//j为粘贴的结果的大小
		free(YaffsCopyBuf);
		yaffs_close(fw1);
		yaffs_close(fw2);
		OSSemPost(&FileSem);
		return TRUE;
	}


}
#endif

#if 0
//Yaffs盘文件拷贝至Fat盘,基本结构同上，具体实现有差异
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
		Uart_Printf("等待信号量故障\n");
		CDispDir_Message(g_bEnglish?"Copy File Sem Timeout!":"等待信号量超时",0);
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
	//非常重要的函数，指针起始偏移
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

//获取文件的大小
void CDispDir_GetFileLength(INT8S *source,INT8S *dest)
{
	#if FALSE
	struct yaffs_stat yaffsBuf[1];

	if(g_iCurDisk==0)													//yaffs
	{
		if(yaffs_stat(source, yaffsBuf) != -1)							//获取文件状态
		{
	 		if(yaffsBuf[0].st_size < 1024)
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%4uB":"文件大小:%4uB",yaffsBuf[0].st_size);
	 		}
	 		else if(yaffsBuf[0].st_size < 1024*1024)					//文件内容小于1024*1024Bit
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%.2fKB":"文件大小:%.2fKB",(FP32)yaffsBuf[0].st_size/1024);
	 		}
	 		else														//文件内容大于1024*1024Bit
	 		{
	 			sprintf(dest,g_bEnglish?"File Size:%.2fMB":"文件大小:%.2fMB",(FP32)yaffsBuf[0].st_size/(1024*1024));
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
		 //方法2
		 FIL File;
		 FRESULT res;
		 res=f_open(File,source,FA_OPEN_EXISTING|FA_READ);
		 len=File.fsize;

		 */

		strcpy(dest,g_bEnglish?"File Size:":tr("文件大小:"));

 		if(len< 1024)													//文件内容小于1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%4uB":tr("%4uB"),len);
 		}
 		else if(len < 1024*1024)										//文件内容小于1024*1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%.2fKB":tr("%.2fKB"),(FP32)len/1024);
 		}
 		else															//文件内容大于1024*1024Bit
 		{
 			sprintf(fileSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)len/(1024*1024));
 		}
 		strcat(dest,fileSize);
		//FS_FClose(fp);
 		res=f_close(fp);
 		fp=free_k(fp,"fp");
	}
}


//获取yaffs或者fat磁盘的空间和剩余空间
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
	 	sprintf(StrTotalSpace,g_bEnglish?"Total Size:%.2fMB":"总体空间:%.2fMB",(FP32)iTotalSpace/(1024*1024));
	 	sprintf(StrFreeSpace,g_bEnglish?"Free Size:%.2fMB":"剩余空间:%.2fMB",(FP32)iFreeSpace/(1024*1024));
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
		iFreeSpace=fre_clust*fs->csize*512;				//fre_clust与fs->free_clust数值相等
		iTotalSpace=(fs->n_fatent-2)*fs->csize*512;
		sprintf(totalSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)iTotalSpace/(1024*1024));
		strcpy(TotalSpace,g_bEnglish?"Total Size:":tr("总体空间:"));
		strcat(TotalSpace,totalSize);

		sprintf(leftSize,g_bEnglish?"%.2fMB":tr("%.2fMB"),(FP32)iFreeSpace/(1024*1024));
		strcpy(FreeSpace,g_bEnglish?"Free Size:":tr("剩余空间:"));
		strcat(FreeSpace,leftSize);

	}
}


//复制文件时进度条信息初始化
void 	CDispDir_ProgressBarInit(INT32S FileLen)
{

	INT16S x1=  g_iDlgLeft+270;
	INT16S x2=	g_iDlgLeft+g_iDlgWidth-330;

	INT16S y1=	g_iDlgTop+230;
	INT16S y2=	g_iDlgTop+255;


	CDC_DrawWin24(x1-20,y1-50,x2-x1+125,100,g_bEnglish?"Copying...":tr("文件拷贝中..."));
	CDC_OutText_UTF(x2+5,y1+1,"0.0%",BLUECOLOR,BGCOLOR,KZK_24);

	CDC_Line(x1,y1,x2,y1,BLACKCOLOR);
	CDC_Line(x1,y1,x1,y2,BLACKCOLOR);

	CDC_Line(x1,y2,x2,y2,WHITECOLOR);
	CDC_Line(x2,y1,x2,y2,WHITECOLOR);

	CDC_PutColorBar(x1+1,y1+1,x2-x1-1,y2-y1-1,WHITECOLOR,0);

	g_iCurPercent=0;													//当前复制百分比初始化0
	g_iPieceNum=FileLen/g_iBufSize;										//进度条块数
	g_iPieceWidth=(FP32)(x2-x1-1)/g_iPieceNum;								//每个块的宽度
}


//模拟绘制复制过程的进度条
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
	if(Type==-1)														//无用
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
	else if(Type==0)													//磁盘图标
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
	else if(Type==1)													//DAT文件图标
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
	else if(Type==2)													//未识别文件图标
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
	else if(Type==3)													//文件夹图标
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

		CDC_Line(x+10,y+5,x+34,y+5,BLACKCOLOR);		//  一
		CDC_Line(x+10,y+5,x+10,y+43,BLACKCOLOR);	//	1
		CDC_Line(x+10,y+43,x+38,y+43,BLACKCOLOR);	//  -

		CDC_Line(x+38,y+43,x+38,y+9,BLACKCOLOR);	//  1
		CDC_Line(x+34,y+5,x+38,y+9,BLACKCOLOR);		//  、

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

	if( 0 == strncmp(DirName,"0:",2) )		//C盘 fatfs文件系统
	{
		strcpy(DirNameTem,DirName);
		len = strlen(DirNameTem);
		
		if(DirNameTem[len-1] == '\\')
			DirNameTem[len-1] = '\0';
		
		res = f_opendir((DIR *)DirHand,(char *)DirNameTem);

		if(res != FR_OK)
			{Error = res;goto Err;}
	}
	else if( 0 == strncmp(DirName,"3:",2) )	//U盘 fatfs文件系统
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
	if( 0 == strncmp(DirName,"0:",2) )		//C盘 fatfs文件系统
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
		DirInf->attrib = fno2[0].fattrib;							//目录或文件类型

		
	}

	else if( 0 == strncmp(DirName,"3:",2) )	//U盘
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
		DirInf->attrib = fno2[3].fattrib;							//目录或文件类型


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


