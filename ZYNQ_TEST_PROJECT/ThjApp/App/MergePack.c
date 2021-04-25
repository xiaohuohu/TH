/*
*********************************************************************************************************
*MergePack Tool
*              -For Ken of Dev3                            	
*
*
* File    : MergaPack.c
* By      : Ken,xuyuwen
* Version : 
*********************************************************************************************************
*/

#include "includes.h"


static TMF NCPINFO ;
static TAreaInfo *pAREA=NULL;
static unsigned char *pAreaInfo=NULL;
static 	FIL 		FileNcp_fd;
static char *ResFilePath="0:\\ADT\\NC_RES.NCP";
//static char *ResFilePathTem="\\ADT\\NC_RESTem.NCP";//该系统均为用到
//static char *ResFileYaffs="/flash/ADT/NC_RES.NCP";
//static char ResFileBuf[50];
//static char ResFileBufNew[50];
//TEST_EXT	OS_SEM		FileSem;

unsigned long crc_tab_32[] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};


void *malloc_k(int size,char *ptr)
{
	return malloc(size);
}


void *free_k(void *p,char *ptr)
{
	free(p);
	return 0;
}


unsigned long MG_CRC32(unsigned char *s, unsigned n)
{
    unsigned long c;         								/* temporary variable */

    static unsigned long crc = (unsigned long)0xffffffffL; 	/* shift register contents */

    if (s == NULL) 
    {
		c = 0xffffffffL;
    } 
    else 
    {
		c = crc;
		
        if (n)
        { 
	        do 
	        {
	            c = crc_tab_32[((int)c ^ (*s++)) & 0xff] ^ (c >> 8);
	        } while (--n);
        }
    }
    
    crc = c;
    return c ^ 0xffffffffL;       							/* (instead of ~c for 64-bit machines) */
}


//static FS_FILE *fi;
//static int yaffs_f;
/*
static int MG_SEEK(unsigned int offset,INT8U DEVICE)
{
	int ErrNo;
	ErrNo=MG_NO_ERR;
	if(DEVICE==1)	//LOCAL FAT
	{
		FS_FSeek(fi,offset,FS_SEEK_SET);
	}
	else				//LOCAL YAFFS II
	{
	}
	return ErrNo;
}*/

#if(FALSE)
static unsigned int MG_FDELETE(char *path,INT8U DEVICE)
{
	int i;
	char buf[50];
	int ErrNo;
	
	ErrNo = MG_NO_ERR;
	
	if(strlen((char *)path)==0)
	{
		ErrNo = MG_FILEDELETEERR;
		goto Err;
	}
	
	strcpy(ResFileBuf,path);
	
	if(DEVICE == 1)
	{
//		Uart_Printf("ResFileBuf %s\r\n",ResFileBuf);
		ErrNo = FS_Remove(ResFileBuf);
	}
	else
	{
		for(i=0;;i++)
		{
			if(!ResFileBuf[i])
				break;
				
			if(ResFileBuf[i] == '\\')
				ResFileBuf[i]='/';
		}
		sprintf(buf,"/flash%s",ResFileBuf);
//		Uart_Printf("buf %s\r\n",buf);
		ErrNo = yaffs_unlink(buf);
	}
Err:
	return ErrNo;
}
#endif


#if(FALSE)
static unsigned int MG_FRENAME(char *oldPath,char *newPath)
{
	int i;
	char OldBuf[50];
	char NewBuf[50];
	int ErrNo;
	
	if(strlen(oldPath) == 0 || strlen(newPath) == 0)
	{
		ErrNo = MG_FREEMEMFAIL;
		goto Err;
	}
	
	strcpy(ResFileBuf,oldPath);
	strcpy(ResFileBufNew,newPath);
	
	for(i=0;;i++)
	{
		if(!ResFileBuf[i])
			break;
		
		if(ResFileBuf[i]=='\\')
			ResFileBuf[i]='/';
	}
	
	for(i=0;;i++)
	{
		if(!ResFileBufNew[i])
			break;
		
		if(ResFileBufNew[i]=='\\')
			ResFileBufNew[i]='/';
	}
	
	sprintf(OldBuf,"/flash%s",ResFileBuf);
	sprintf(NewBuf,"/flash%s",ResFileBufNew);
//	Uart_Printf("OldBuf %s NewBuf %s\r\n",OldBuf,NewBuf);
	ErrNo = yaffs_rename(OldBuf,NewBuf);
	ErrNo = MG_NO_ERR;
	
Err:
	return ErrNo;
}
#endif

#if(FALSE)
static unsigned int MG_FWRITE(void *p,unsigned int size,char *path,INT8U DEVICE)
{
	if(DEVICE==1)									//LOCAL FAT
	{
	    if((p==NULL) && (path!=NULL))				//打开文件
	    {
		    fi = FS_FOpen(path,"w");
		    
		    if(!fi)
		    {
			    size=0;
			    goto Err;
		    }
		    
		    size=1;
	    }
	    else if(p)
	    {
		    size = FS_FWrite(p,1,size,fi);
	    }
	    else if((p==NULL) && (path==NULL))			//关闭文件
	    {
	    	FS_FClose(fi);
		    size = 1;
	    }
	
	}
	else											//LOCAL YAFFS II
	{
	    if((p==NULL) && (path!=NULL))				//打开文件
	    {	
	    	int i;
	    	char buf[50];
	    	
	    	strcpy(ResFileBuf,path);
	    	
			for(i=0;;i++)
			{
				if(!ResFileBuf[i])
					break;
				
				if(ResFileBuf[i]=='\\')
					ResFileBuf[i]='/';
			}
			
			sprintf(buf,"/flash%s",ResFileBuf);
			Uart_Printf("buf %s\r\n",buf);

			yaffs_f = yaffs_open(buf,O_WRONLY | O_CREAT, S_IREAD | S_IWRITE );

			if(yaffs_f == -1)
			{
				size=0;
				goto Err;
			}
			
		    size = 1;
	    }
	    else if(p)
	    {
		    size = yaffs_write(yaffs_f,(char *)p,size);
	    }
	    else if((p==NULL) && (path==NULL))			//关闭文件
	    {
	    	yaffs_close(yaffs_f);
		    size = 1;
	    }
	
	}
Err:
	return size;	
}
#endif



static unsigned int MG_FREAD(void *p,unsigned int size,char *path,INT8U DEVICE)
{
	FRESULT		res;
	UINT		br;
//	OS_ERR		os_err;
//	CPU_TS    	p_ts;

//	OSSemPend(&FileSem,10000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);

	if(DEVICE==1)									//LOCAL FAT
	{
	    if((p==NULL) && (path!=NULL))				//打开文件
	    {
	    	res=f_open(&FileNcp_fd,"0:\\ADT\\NC_RES.NCP",FA_OPEN_EXISTING|FA_READ);

		    if(res!=FR_OK)
		    {
		    	Uart_Printf("test3 err\n");
		    	size = 0;
			    goto Err;
		    }
		    
		    size = 1;
	    }
	    else if(p)
	    {
	    	//size为准备读取的空间，br为实际读取的空间
	    	res = f_read(&FileNcp_fd,p,size,&br);
		    size=br;
	    }
	    else if((p==NULL) && (path==NULL))			//关闭文件
	    {
	    	f_close(&FileNcp_fd);
		    size = 1;
	    }
	
	}
	/***********************本系统只用到了fat磁盘，没用到yaffs磁盘***********************/
	/*
	else											//LOCAL YAFFS II
	{
	    if((p==NULL) && (path!=NULL))				//打开文件
	    {	
	    	int i;
	    	char buf[50];
	    	
	    	strcpy(ResFileBuf,path);
	    	
			for(i=0;;i++)
			{
				if(!ResFileBuf[i])
					break;
				
				if(ResFileBuf[i]=='\\')
					ResFileBuf[i]='/';
			}
			
			sprintf(buf,"/flash%s",ResFileBuf);
			Uart_Printf("ResFileBuf %s\r\n",ResFileBuf);

			yaffs_f = yaffs_open(buf,O_RDONLY , S_IREAD );

			if(yaffs_f == -1)
			{
				size=0;
				goto Err;
			}
			
		    size = 1;
	    }
	    else if(p)
	    {
		    size=yaffs_read(yaffs_f,(char *)p,size);
	    }
	    else if((p==NULL) && (path==NULL))			//关闭文件
	    {
	    	yaffs_close(yaffs_f);
		    size = 1;
	    }
	
	}*/
Err:
//	OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
	return size;	
}


/*
|---------------------|
|        TMF
|---------------------|
|   TAreaInfo[array]
|---------------------|
|      pAreaInfo
|---------------------|
*/

#define IndexCount 50
static int PointIndex = 0;

typedef struct _TPointData
{
	int		PointIndex;
	INT32U	*Pointer;
	char	name[255];
}TPointData;

static TPointData PointData[IndexCount];
static INT8U DEVICE;


/*BOOL FATCOPYTOYAFFS(char *path,char *path1)
{
	if(!MG_FREAD(NULL,NULL,path,DEVICE))			//搜索FAT
	{	
		DEVICE = 0;
		
		if(!MG_FREAD(NULL,NULL,path,DEVICE))
		{
			ErrNo = MG_FILEREADERR;
			goto Err;
		}
	}

    if((size=MG_FREAD(&NCPINFO,sizeof(NCPINFO),"",DEVICE))!=sizeof(NCPINFO))
    {
	    ErrNo=MG_FILEREADERR;
	    goto Err;
    }
	
	DEVICE = 0;
	
	if(!MG_FWRITE(NULL,NULL,path1,DEVICE))		//
	{
		ErrNo=MG_FILEWRITEERR;
		Uart_Printf("1=%d\n",ErrNo);
		goto Err;
	}
	
	if((size=MG_FWRITE(&NCPINFO,sizeof(NCPINFO),"",DEVICE))!=sizeof(NCPINFO))
	{
		ErrNo=MG_FILEWRITEERR;
		Uart_Printf("2=%d\n",ErrNo);
		goto Err;
	}
    
    
    if(!MG_FREAD(NULL,NULL,path,DEVICE))			//搜索YAFFS
	{	
		DEVICE = 1;
		
		if(!MG_FREAD(NULL,NULL,path,DEVICE))
		{
			ErrNo = MG_FILEREADERR;
			goto Err;
		}
	}

    if((size=MG_FREAD(&NCPINFO,sizeof(NCPINFO),"",DEVICE))!=sizeof(NCPINFO))
    {
	    ErrNo=MG_FILEREADERR;
	    goto Err;
    }
}*/

#if(FALSE)
void YAFFSRWFILE()
{
	int a;
	int i,err;
	char buffer1[100];
	struct yaffs_DeviceStruct yd;

	memset(buffer1,0,sizeof(buffer1));
	
	yaffs_StartUp(); 
	
	yd.startBlock=1024;
	yd.endBlock=2048;
	yd.nChunksPerBlock=64;
	
	for(i=1024;i<2048;i++)
	ynandif_EraseBlockInNAND(&yd,i);    
	err=yaffs_mount("/flash");

	//loop: 
	Uart_Printf("**************************err:%d\r\n",err);

	a = yaffs_open("/flash/ADT/NC_RES.NCP",O_CREAT | O_RDWR , S_IREAD | S_IWRITE);
	
	Uart_Printf("a:%d\r\n",a);
	
	if(a!=-1)
	{
		err=yaffs_lseek(a,0,SEEK_SET);
		Uart_Printf("seekerr:%d\r\n",err);  
		   
		for(i=0;i<0x1000;i++)
		{
			err=yaffs_write(a,&NCPINFO,sizeof(NCPINFO));
			
			if(err!=sizeof(NCPINFO))
			{
				Uart_Printf("writeerr:%d\r\n",err);
				break;
			}
		}
		err=yaffs_lseek(a,0,SEEK_SET);
			
		Uart_Printf("lseekerr:%d\r\n",err);   
			//  OSTimeDly(10000);
		for(i=0;i<0x1000;i++)
		{
			err=yaffs_read(a,buffer1,sizeof(NCPINFO));
			
			if(err!=sizeof(NCPINFO))
			{
				Uart_Printf("yaffs_read:%d\r\n",err);
				break;
			} 
			   
			/*if(strcmp(buffer1,"abcdefghijklmnopqrst"))
			{
				Uart_Printf("cmp err:%s\r\n",buffer1);
				break;
			}*/
		}
		
		err=yaffs_totalspace("/flash");
		Uart_Printf("totalspace %d\n",err);
		err=yaffs_freespace("/flash");
		Uart_Printf("free space %d\n",err);
	
	}
	// if you want the data from cache to the nand, must remember yaffs_close    yaffs_close(a);
	OSTimeDly(10);
	//goto loop;   
}
#endif

int MG_UNMERGE(char *path)
{
	unsigned char *pf;
	int i,j,ErrNo,size,offset;
	unsigned long *pcrc;
	unsigned long pcrcTem;
	
    ErrNo = MG_NO_ERR;
    pAREA = NULL;
    pAreaInfo = NULL;
	pf = NULL;
	
	ErrNo = MG_NO_ERR;
	memset(PointData,0x00,sizeof(PointData));
	/*
	DEVICE = 0;
	
	if(!MG_FREAD(NULL,NULL,path,DEVICE))			//搜索FAT 或 YAFFS
	{	
		DEVICE = 1;
		
		if(!MG_FREAD(NULL,NULL,"\\ADT\\NC_RES.NCP",DEVICE))
		{
			ErrNo = MG_FILEREADERR;
			goto Err;
		}
	}
	 */
	/*********************只读取fat磁盘，不读取yaffs****************************/
	{
		DEVICE = 1;

		if(!MG_FREAD(NULL,0,"0:\\ADT\\NC_RES.NCP",DEVICE))
		{
			ErrNo = MG_FILEREADERR;
			goto Err;
		}
	}


    if((size=MG_FREAD(&NCPINFO,sizeof(NCPINFO),"",DEVICE))!=sizeof(NCPINFO))
    {
    	ErrNo=MG_FILEREADERR;
	    goto Err;
    }


    MG_CRC32(0,0);
    MG_CRC32((unsigned char*)&NCPINFO,size);
    
	Uart_Printf("filecount:%d DEVICE %d\r\n",NCPINFO.FileCount,DEVICE);

    //pAREA = (TAreaInfo *)malloc_k(sizeof(TAreaInfo)*NCPINFO.FileCount,"pAREA");

	pAREA = (TAreaInfo *)malloc(sizeof(TAreaInfo)*(NCPINFO.FileCount));
	Uart_Printf("pAREA:%x %d\n",pAREA,sizeof(TAreaInfo)*NCPINFO.FileCount);
	
    if(!pAREA)
    {
	    ErrNo=MG_GETMEMFAIL;
	    goto Err;
    }
    
    if((size=MG_FREAD(pAREA,sizeof(TAreaInfo)*NCPINFO.FileCount,"",DEVICE))!=(sizeof(TAreaInfo)*NCPINFO.FileCount))
    {
	    ErrNo=MG_FILEREADERR;
	    goto Err;
    }
    
    MG_CRC32((unsigned char*)pAREA,size);
    
    for(size=0,i=0; i<NCPINFO.FileCount; i++)
    {
        if(pAREA[i].CompileType==0)
        	break;
        else 
       		size += pAREA[i].filesize;
        
        if(size & 0x03)
        	size = size+0x04-(size & 0x03);
    }
    
    pAreaInfo = (unsigned char *)malloc_k(size,"pAreaInfo");
    
	Uart_Printf("pAreaInfo:%x size:%d\r\n",pAreaInfo,size);

    if(!pAreaInfo)
    {
	    ErrNo=MG_GETMEMFAIL;
	    goto Err;
    }

    if(MG_FREAD(pAreaInfo,size,"",DEVICE)!=size)
    {
	    ErrNo=MG_FILEREADERR;
	    goto Err;
    }
    
    MG_CRC32(pAreaInfo,size);

    for(offset=0,i=0; i<NCPINFO.FileCount; i++)
    {
        j = strlen(pAREA[i].name);
        
        if(pAREA[i].name[j-1]==' ')
        	pAREA[i].name[j-1]=NULL;
        	
        if(pAREA[i].CompileType)
        {
            pAREA[i].pInfo = &pAreaInfo[offset];
            size = pAREA[i].filesize;
            
            if(size & 0x03)
            	offset = offset+size+0x04-(size & 0x03);
            else 
            	offset = offset+size;
        }
        else
        {
            pAREA[i].pInfo=NULL;
        }
    }

    pf = (unsigned char *)malloc_k(1024*1024*10,"pf");
    
    if(!pf)
    {
	    ErrNo=MG_GETMEMFAIL;
	    goto Err;
    }
    
    size = MG_FREAD(pf,1024*1024*10,NULL,DEVICE);
    pcrc = (unsigned long*)&pf[size-4];
    pcrcTem = MG_CRC32(pf,size-4);
    
	Uart_Printf("size %d pcrc %d pcrcTem %d\r\n",size,*pcrc,pcrcTem);

/*
    if(*pcrc != pcrcTem)
    {
        ErrNo = MG_FILECRCERR;
        goto Err;
    }
*/
    MG_FREAD(NULL,0,NULL,DEVICE);
    pf = free_k(pf,"pf");
    
Err:
	switch(ErrNo)
	{
		case MG_FILECRCERR:
			Uart_Printf("文件校验不通过");
			CDC_OutText24(20,75,"文件校验不通过!",REDCOLOR,0);
			break;

		case MG_GETMEMFAIL:
			Uart_Printf("错误：申请内存不成功!");
			CDC_OutText24(20,75,"错误：申请内存不成功!!",REDCOLOR,0);
			break;

		case MG_FILEREADERR:
			Uart_Printf("错误：文件读取操作不成功!");
			CDC_OutText24(20,75,"错误：文件读取操作不成功!!!",REDCOLOR,0);
			break;

		default:
			if(ErrNo)
				Uart_Printf("err:%d\r\n",ErrNo);
			break;
	}

	if(ErrNo!=0)
	{
		if(DEVICE==0)
			CDC_OutText24(20,100,"DEVICE=0!",REDCOLOR,0);
		else
			CDC_OutText24(20,100,"DEVICE=1!",REDCOLOR,0);
	}
    
    return ErrNo;
}


/*******************************************
函数功能:获取资源文件属性包括文件名 文件大小 加载方式等
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
	AreaInfo:返回资源属性数据结构指针
返回值:正确返回零 错误返回非零
*******************************************/

int MG_GetTAreaInfo(char *pathname,TAreaInfo *AreaInfo)
{
	int i;
	int ErrNo;

	ErrNo = MG_NO_ERR;
	
	if(*pathname==0x00)
	{
		ErrNo=MG_FILENOEXIST;
		goto Err;
	}
	
	for(i=0;i<NCPINFO.FileCount;i++)
	{
		if(strcmp(pAREA[i].name,pathname)==0)
			break;
	}
	
	if(i>=NCPINFO.FileCount)
	{
		ErrNo=MG_FILENOEXIST;
		goto Err;
	}
	
	memcpy(AreaInfo,&pAREA[i],sizeof(TAreaInfo));
	
Err:
	return ErrNo;
}


/*******************************************
函数功能:静态或动态获取资源内容
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
	paddr:返回资源内容数据指针
返回值:正确返回零 错误返回非零
*******************************************/

int MG_GetMem(char *pathname,INT32U *paddr)
{
	FIL 		fd;
	FRESULT		res;
	UINT		br;
//	OS_ERR		os_err;
//	CPU_TS    	p_ts;

	int i;
	//int ret
	int ErrNo;
	int size,sizeTem,AddrOffs;
	//FS_FILE *fp;
	//int yaffs_fp;

	ErrNo = MG_NO_ERR;

	if(*pathname==0x00)
	{
		ErrNo = MG_FILENOEXIST;
		goto Err;
	}
	
	sizeTem = sizeof(NCPINFO);
	sizeTem = sizeTem + sizeof(TAreaInfo)*NCPINFO.FileCount;
	size = 0;
	AddrOffs = 0;
	
	for(i=0;i<NCPINFO.FileCount;i++)
	{
		if(strcmp(pAREA[i].name,pathname)==0)
			break;
			
		/********************************************************/
		size += pAREA[i].filesize;
		
		if(size&0x03)
			size = size + 0x04 - (size&0x03);
		/********************************************************/
	}
	
	AddrOffs = sizeTem + size;

	if(i>=NCPINFO.FileCount)
	{
		ErrNo=MG_FILENOEXIST;
		goto Err;
	}

	if(pAREA[i].CompileType)						//静态加载
	{
		*paddr=(INT32U)(pAREA[i].pInfo);
	}
	else											//动态加载
	{
		Uart_Printf("dynamic load\r\n");
			
		if(PointIndex >= IndexCount)
		{
			ErrNo=MG_GETMEMFAIL;
			goto Err;
		}
		
		PointData[PointIndex].Pointer = malloc_k(pAREA[i].filesize,"PointData");
		
		if(!PointData[PointIndex].Pointer)
		{
			ErrNo=MG_GETMEMFAIL;
			goto Err;
		}
		
		strcpy(PointData[PointIndex].name,pAREA[i].name);
		PointData[PointIndex].PointIndex = PointIndex;
		
		/***********************************************/
		
		Uart_Printf("AddrOffs %d sizeTem %d size %d\r\n",AddrOffs,sizeTem,size);
//		OSSemPend(&FileSem,10000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);
		if(DEVICE == 1)
		{
			res = f_open(&fd,ResFilePath,FA_CREATE_ALWAYS|FA_WRITE);//fp = FS_FOpen(ResFilePath,"r");
			
			if(res != FR_OK)
			{
				f_close(&fd) ;
				ErrNo=MG_GETMEMFAIL;
				goto Err;
			}
			
			//FS_FSeek(fp,AddrOffs,FS_SEEK_SET);
			//ret = FS_FRead(PointData[PointIndex].Pointer,1,pAREA[i].filesize,fp);
			res = f_lseek(&fd,AddrOffs);
			res = f_read(&fd,PointData[PointIndex].Pointer,pAREA[i].filesize,&br);
			
			if((&fd)->fsize != pAREA[i].filesize)
			{
				f_close(&fd) ;
				ErrNo=MG_GETMEMFAIL;
				goto Err;
			}
			
			f_close(&fd) ;
		}
		/*
		else
		{
			yaffs_fp = yaffs_open(ResFileYaffs,O_RDONLY,S_IREAD | S_IWRITE);
			
			if(yaffs_fp == -1)
			{
				yaffs_close(yaffs_fp);
				ErrNo=MG_GETMEMFAIL;
				goto Err;
			}
			
			yaffs_lseek(yaffs_fp,AddrOffs,SEEK_SET);
			
			ret = yaffs_read(yaffs_fp,PointData[PointIndex].Pointer,pAREA[i].filesize);
			
			if(ret != pAREA[i].filesize)
			{
				FS_FClose(fp);
				ErrNo=MG_GETMEMFAIL;
				goto Err;
			}
			
			yaffs_close(yaffs_fp);
		}*/
		
		*paddr = (INT32S)PointData[PointIndex].Pointer;
		
		/***********************************************/
		PointIndex++;
	}
	
Err:
	if(ErrNo)
	{
		paddr = NULL;//*paddr = NULL;
		Uart_Printf("can't load@@%s ErrNo %d\r\n",pathname,ErrNo);
	}
//	OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);
	return ErrNo;
}



/*******************************************
函数功能:动态获取资源内容内存释放
参数:pathname:文件名及路径(如:"ADT\\Test333.txt")指针
返回值:正确返回零 错误返回非零
*******************************************/

int MG_FreeMem(char *pathname)
{
	int PointIndexTem;
	int ErrNo;

	ErrNo = MG_NO_ERR;
	PointIndexTem = PointIndex - 1;
	
	if(PointIndexTem < 0)
	{
		ErrNo = MG_FREEMEMFAIL;
		goto Err;
	}
	
	if(strcmp(PointData[PointIndexTem].name,pathname) != 0)
	{
		ErrNo = MG_FREEMEMFAIL;
		goto Err;
	}
	
	memset(PointData[PointIndexTem].name,0x00,255);
	
	PointData[PointIndexTem].Pointer = free_k(PointData[PointIndexTem].Pointer,"PointData");
	
	Uart_Printf("free_k mem :%s\r\n",pathname);
	
	PointIndex--;
Err:
	return ErrNo;
}


/*******************************************
函数功能:更新资源文件NC_RES.NCP
参数:path:文件名及路径(如:"ADT\\Test333.txt")指针
	size:更新保存文件的大小
	s:更新保存文件内容指针
返回值:正确返回零 错误返回非零
*******************************************/
#if(FALSE)
int MG_Update(char *path,int size,char *s)
{
	TAreaInfo *pAREA=NULL;
	TAreaInfo *pAREATem=NULL;
	unsigned long pcrc;
	int ErrNo;
	int filesize;
	unsigned char *pf[3];
	int i,j,sizeTem;
	int offset=0,IndexNo=0;
	int startaddr[3];
	int datasize[3];
	int UpdateFlag=1;
	
	pAREA = NULL;
	pAREATem = NULL;
	//pAreaInfo = NULL;
	
	pf[0] = pf[1] = pf[2] = NULL;
	i = strlen(path);
	
	if(i)
	{
		path[i] = ' ';
		path[i+1] = 0x00;
	}
	
	ErrNo=MG_NO_ERR;
	/*
	{
		char *strTem1="qwert";
		char *strTem2="12345";
		char bufTem[15];
		
		memset(bufTem,0x00,15);
		filesize = MG_FWRITE(NULL,NULL,ResFilePathTem,0);
		Uart_Printf("filesize1 %d\r\n",filesize);
		filesize = MG_FWRITE(strTem1,5,NULL,0);
		Uart_Printf("filesize2 %d\r\n",filesize);
		filesize = MG_FWRITE(strTem2,5,NULL,0);
		Uart_Printf("filesize2 %d\r\n",filesize);
		MG_FWRITE(NULL,NULL,NULL,0);
		filesize = MG_FREAD(NULL,NULL,ResFilePathTem,0);
		Uart_Printf("filesize3 %d\r\n",filesize);
		filesize = MG_FREAD(bufTem,5,NULL,0);
		Uart_Printf("filesize4 %d bufTem %s\r\n",filesize,bufTem);
		filesize = MG_FREAD(bufTem,5,NULL,0);
		Uart_Printf("filesize4 %d bufTem %s\r\n",filesize,bufTem);
		filesize = MG_FREAD(bufTem,5,NULL,0);
		Uart_Printf("filesize4 %d bufTem %s\r\n",filesize,bufTem);
		filesize = MG_FREAD(NULL,NULL,NULL,0);
		Uart_Printf("filesize5 %d\r\n",filesize);
	}
	return ErrNo;
	*/
	
	Uart_Printf("DEVICE %d filename %s size %d\r\n",DEVICE,path,size);
	
	if(*path == 0 || size == 0 || s == 0)
	{
		ErrNo = MG_FREEMEMFAIL;
	}
	
	filesize = MG_FREAD(NULL,NULL,ResFilePath,DEVICE);
	
	if(filesize == 0)
	{
		MG_FREAD(NULL,NULL,NULL,DEVICE);			//关闭文件
		ErrNo=MG_FILEREADERR;
		goto Err;
	}
	
//x	Uart_Printf("filesize1 %d\r\n",filesize);
	
	if((filesize=MG_FREAD(&NCPINFO,sizeof(NCPINFO),NULL,DEVICE))!=sizeof(NCPINFO))
	{
		ErrNo=MG_FILEREADERR;
		goto Err;
	}
	
//x	Uart_Printf("filesize %d %d %d\r\n",filesize,sizeof(NCPINFO),NCPINFO.STLSIZE);

	Uart_Printf("sizeof(NCPINFO) %d NCPINFO.FileCount %d %d %d\r\n",sizeof(NCPINFO),NCPINFO.FileCount,sizeof(TAreaInfo),sizeof(TAreaInfo)*NCPINFO.FileCount);
	
	pAREA=(TAreaInfo *)malloc_k(sizeof(TAreaInfo)*NCPINFO.FileCount,"pAREA");
	
	if(!pAREA)
	{
		ErrNo=MG_GETMEMFAIL;
		goto Err;
	}
	
	if((filesize=MG_FREAD(pAREA,sizeof(TAreaInfo)*NCPINFO.FileCount,NULL,DEVICE))!=(sizeof(TAreaInfo)*NCPINFO.FileCount))
	{
		ErrNo=MG_FILEREADERR;
		goto Err;
	}
	
	Uart_Printf("filesize %d\r\n",filesize);
	
    for(sizeTem=0,i=0;i<NCPINFO.FileCount;i++)
    {   
        sizeTem += pAREA[i].filesize;
        
        if(sizeTem&0x03)
        	sizeTem = sizeTem+0x04-(sizeTem&0x03); 
//x Uart_Printf("i %d filesize %d sizeTem %d filename %s CompileType %d\r\n",i,pAREA[i].filesize,sizeTem,pAREA[i].name,pAREA[i].CompileType);
    }
    
    filesize = sizeTem;
    
    Uart_Printf("sizeTem %d\r\n",sizeTem);
    
    j = 0;
    offset = 0;
    startaddr[0] = startaddr[1] = startaddr[2] = 0;
    datasize[0] = datasize[1] = datasize[1] = 0;
    
    for(sizeTem=0,i=0; i<NCPINFO.FileCount; i++)
    {
    	if(strcmp(pAREA[i].name,path)==0 && (i > 0))
    	{
    		datasize[0] = offset;
    		startaddr[1] = offset;
    		
    		if(pAREA[i].filesize&0x03)
    			datasize[1] = pAREA[i].filesize + 0x04 - (pAREA[i].filesize&0x03);
    		else 
    			datasize[1] = pAREA[i].filesize;
    			
    		startaddr[2] = startaddr[1] + datasize[1];
    		datasize[2] = filesize - datasize[0] - datasize[1];
    		IndexNo = i;UpdateFlag = 0;
    		
    		pAREATem = (TAreaInfo *)malloc_k(sizeof(TAreaInfo)*NCPINFO.FileCount,"pAREATem");
    		memcpy(pAREATem,pAREA,sizeof(TAreaInfo)*NCPINFO.FileCount);
    		
//x    		Uart_Printf("pAREA.filesize %d startaddr0 %d startaddr1 %d startaddr2 %d\r\n",pAREA[i].filesize,startaddr[0],startaddr[1],startaddr[2]);
//x    		Uart_Printf("IndexNo %d offset1 %d datasize0 %d datasize1 %d datasize2 %d\r\n",IndexNo,offset,datasize[0],datasize[1],datasize[2]);
    	}
    	
    	if((strcmp(pAREA[i].name,path)==0) && (i == 0))
    	{
    		if(pAREA[i].filesize&0x03)
    			datasize[0] = pAREA[i].filesize + 0x04 - (pAREA[i].filesize&0x03);
    		else 
    			datasize[0] = pAREA[i].filesize;
    			
    		startaddr[1] = datasize[0];
    		datasize[1]  = filesize - datasize[0];
    		startaddr[2] = startaddr[1] + datasize[1];
    		datasize[2]  = filesize - datasize[0] - datasize[1];
    	}
    	
    	sizeTem = pAREA[i].filesize;
    	
//x    	Uart_Printf("i %d offset %d filesize %d\r\n",i,offset,filesize);
    	
    	if(sizeTem & 0x03)
    		offset = offset+sizeTem+0x04-(sizeTem&0x03);
    	else 
    		offset = offset+sizeTem;
    }
    
    if(UpdateFlag != 0)
    	datasize[0] = offset;
    	
    Uart_Printf("offsetEnd %d\r\n",offset);
    
    if(UpdateFlag == 0)								//更新已有文件
    {
    	if(IndexNo == 0)							//更新第一个文件
    	{
    		pAREATem[IndexNo].filesize = size;
    		
    		if(size & 0x03)
    			size = size + 0x04 - (size&0x03);
    			
    		pf[0] = (unsigned char *)malloc_k(size,"pf[0]");
    		
    		if(!pf[0])
    		{
	    		ErrNo=MG_GETMEMFAIL;
	    		goto Err;
    		}
    		
    		if(datasize[0] != 0)
    		{
    			filesize = MG_FREAD(pf[0],datasize[0],NULL,DEVICE);
    			
    			if(filesize != datasize[0])
    			{
	    			ErrNo = MG_FILEREADERR;
	    			goto Err;
    			}
    		}
    		
    		memset(pf[0],0x00,size);
    		memcpy(pf[0],s,pAREATem[IndexNo].filesize);
    		datasize[0] = size;
    	}
    	else if(datasize[0] != 0)
    	{
	    	pf[0] = (unsigned char *)malloc_k(datasize[0],"pf[0]");
	    	
	    	if(!pf[0])
	    	{
		    	ErrNo=MG_GETMEMFAIL;
		    	goto Err;
	    	}
	    	
	    	filesize = MG_FREAD(pf[0],datasize[0],NULL,DEVICE);
	    	
	    	if(filesize != datasize[0])
	    	{
		    	ErrNo = MG_FILEREADERR;
		    	goto Err;
	    	}
    	}
    	
//x    	Uart_Printf("filesize1 %d\r\n",filesize);
    	/***************************************************/
    	if(IndexNo == 0 && datasize[1] != 0)
    	{
    		pf[1] = (unsigned char *)malloc_k(datasize[1],"pf[1]");
    		
    		if(!pf[1])
    		{
	    		ErrNo=MG_GETMEMFAIL;
	    		goto Err;
    		}
    		
    		filesize = MG_FREAD(pf[1],datasize[1],NULL,DEVICE);
    		
    		if(filesize != datasize[1])
    		{
	    		ErrNo = MG_FILEREADERR;
	    		goto Err;
    		}
    	}
    	else
    	{
	    	pAREATem[IndexNo].filesize = size;
	    	
	    	if(size & 0x03)
	    		size = size + 0x04 - (size&0x03);
	    		
	    	pf[1] = (unsigned char *)malloc_k(size,"pf[1]");
	    	
	    	if(!pf[1])
	    	{
		    	ErrNo=MG_GETMEMFAIL;
		    	goto Err;
	    	}
	    	
	    	if(datasize[1] != 0)
	    	{
	    		filesize = MG_FREAD(pf[1],datasize[1],NULL,DEVICE);
	    		
	    		if(filesize != datasize[1])
	    		{
		    		ErrNo = MG_FILEREADERR;
		    		goto Err;
	    		}
	    	}
	    	
	    	memset(pf[1],0x00,size);
	    	memcpy(pf[1],s,pAREATem[IndexNo].filesize);
	    	datasize[1] = size;
    	}
    	
//x    	Uart_Printf("filesize2 %d size %d\r\n",filesize,size);
    	/*****************************************************/
    	if(datasize[2] != 0)
    	{
	    	pf[2] = (unsigned char *)malloc_k(datasize[2],"pf[2]");
	    	
	    	if(!pf[2])
	    	{
		    	ErrNo=MG_GETMEMFAIL;
		    	goto Err;
	    	}
	    	
	    	filesize = MG_FREAD(pf[2],datasize[2],NULL,DEVICE);
	    	
	    	if(filesize != datasize[2])
	    	{
		    	ErrNo = MG_FILEREADERR;
		    	goto Err;
	    	}
    	}
//x    	Uart_Printf("filesize3 %d\r\n",filesize);
    	MG_FREAD(NULL,NULL,NULL,DEVICE);			//关闭文件
    }
    else											//添加新的文件
    {
    	NCPINFO.FileCount = NCPINFO.FileCount + 1;
    	
    	pAREATem = (TAreaInfo *)malloc_k(sizeof(TAreaInfo)*NCPINFO.FileCount,"pAREATem");
    	
    	memset(pAREATem,0x00,sizeof(TAreaInfo)*NCPINFO.FileCount);
    	
    	IndexNo = NCPINFO.FileCount-1;
    	
    	memcpy(pAREATem,pAREA,sizeof(TAreaInfo)*(NCPINFO.FileCount-1));
    	
    	strcpy(pAREATem[IndexNo].name,path);
    	
    	pAREATem[IndexNo].filesize = size;
    	pAREATem[IndexNo].CompileType = 0;			//新增加的文件为动态加载 可释放
    	pAREATem[IndexNo].offset = 0;
    	pAREATem[IndexNo].pInfo = NULL;
    	/***********************************************************/
    	if(datasize[0] != 0)
    	{
	    	pf[0] = (unsigned char *)malloc_k(datasize[0],"pf[0]");
	    	
	    	if(!pf[0])
	    	{
		    	ErrNo=MG_GETMEMFAIL;
		    	goto Err;
	    	}
	    	
	    	filesize = MG_FREAD(pf[0],datasize[0],NULL,DEVICE);
	    	
	    	if(filesize != datasize[0])
	    	{
		    	ErrNo = MG_FILEREADERR;
		    	goto Err;
	    	}
	    	
	    	Uart_Printf("addnew filesize %d datasize[0] %d\r\n",filesize,datasize[0]);
    	}
    	
    	/***********************************************************/
    	
    	pAREATem[IndexNo].filesize = size;
    	
    	if(size & 0x03)
    		size = size + 0x04 - (size&0x03);
    		
    	pf[1] = (unsigned char *)malloc_k(size,"pf[1]");
    	
    	memset(pf[1],0x00,size);
    	memcpy(pf[1],s,pAREATem[IndexNo].filesize);
    	datasize[1] = size;
    	
    	MG_FREAD(NULL,NULL,NULL,DEVICE);			//关闭文件
    }
    
    /*************************************************
    注:文件更新保存到Yaffs文件系统，同时删除FAT文件系统中的文件
    **************************************************/
    
    MG_CRC32(0,0);
    filesize = MG_FWRITE(NULL,NULL,ResFilePathTem,0);
    
    if(filesize <= 0)
    {
	    ErrNo = MG_FILEWRITEERR;
	    goto Err;
    }
    
	pcrc = MG_CRC32((unsigned char *)&NCPINFO,sizeof(NCPINFO));
	filesize = MG_FWRITE(&NCPINFO,sizeof(NCPINFO),NULL,0);
	Uart_Printf("filesize1 %d pcrc %d\r\n",filesize,pcrc);
	
	if(filesize <= 0)
	{
		ErrNo = MG_FILEWRITEERR;
		goto Err;
	}
	
	pcrc = MG_CRC32((unsigned char *)pAREATem,sizeof(TAreaInfo)*NCPINFO.FileCount);
	filesize = MG_FWRITE(pAREATem,sizeof(TAreaInfo)*NCPINFO.FileCount,NULL,0);
	
	Uart_Printf("filesize2 %d pcrc %d\r\n",filesize,pcrc);
	
	if(filesize <= 0)
	{
		ErrNo = MG_FILEWRITEERR;
		goto Err;
	}
	
	if(datasize[0] != 0)
	{
		pcrc = MG_CRC32(pf[0],datasize[0]);
		filesize = MG_FWRITE(pf[0],datasize[0],NULL,0);
		
		Uart_Printf("filesize3 %d pcrc %d\r\n",filesize,pcrc);
		
		if(filesize <= 0)
		{
			ErrNo = MG_FILEWRITEERR;
			goto Err;
		}
	}
	
	if(datasize[1] != 0)
	{
		pcrc = MG_CRC32(pf[1],datasize[1]);
		filesize = MG_FWRITE(pf[1],datasize[1],NULL,0);
		
		Uart_Printf("filesize4 %d pcrc %d\r\n",filesize,pcrc);
		
		if(filesize <= 0)
		{
			ErrNo = MG_FILEWRITEERR;
			goto Err;
		}
	}
	
	if((UpdateFlag == 0) && (datasize[2] != 0))
	{
		pcrc = MG_CRC32(pf[2],datasize[2]);
		filesize = MG_FWRITE(pf[2],datasize[2],NULL,0);
		
		Uart_Printf("filesize5 %d pcrc %d\r\n",filesize,pcrc);
		
		if(filesize <= 0)
		{
			ErrNo = MG_FILEWRITEERR;
			goto Err;
		}
	}
	
	filesize = MG_FWRITE(&pcrc,4,NULL,0);
	
	Uart_Printf("filesize6 %d pcrc %d\r\n",filesize,pcrc);
	
	if(filesize <= 0)
	{
		ErrNo = MG_FILEWRITEERR;
		goto Err;
	}
	
	MG_FWRITE(NULL,NULL,NULL,0);
	
	filesize = MG_FDELETE(ResFilePath,0);
	
//x	Uart_Printf("filesize7 %d\r\n",filesize);

	filesize = MG_FDELETE(ResFilePath,1);
	
//x	Uart_Printf("filesize8 %d\r\n",filesize);

	filesize = MG_FRENAME(ResFilePathTem,ResFilePath);
	
//x	Uart_Printf("filesize9 %d\r\n",filesize);

Err:
	MG_FREAD(NULL,NULL,NULL,DEVICE);				//关闭文件
	MG_FWRITE(NULL,NULL,NULL,DEVICE);
	MG_FWRITE(NULL,NULL,NULL,0);
	
	pf[2] = free_k(pf[2],"pf[2]");
	pf[1] = free_k(pf[1],"pf[1]");
	pf[0] = free_k(pf[0],"pf[0]");
	
	pAREATem = free_k(pAREATem,"pAREATem");
	
	pAREA = free_k(pAREA,"pAREA");
	
	return ErrNo;
}
#endif

/*
int MG_Update(char *path,int size,char *s)
{
	int i;
	int ErrNo;
	int offset;

	ErrNo=MG_NO_ERR;
	
	if(*path==0x00)
	{
		ErrNo=MG_FILENOEXIST;
		goto Err;
	}
	
	for(i=0;i<NCPINFO.FileCount;i++)
	{
		Uart_Printf("name %s i %d FileCount %d\r\n",pAREA[i].name,i,NCPINFO.FileCount);
		
		if(strstr(pAREA[i].name,path)==pAREA[i].name)
			break;
	}
	
	Uart_Printf("i1:%d\r\n",i);
	if(i>=NCPINFO.FileCount)
	{
		ErrNo=MG_FILENOEXIST;
		goto Err;
	}
	
	Uart_Printf("i2:%d\r\n",i);
	
	if(pAREA[i].CompileType)
		NCPINFO.STLSIZE=NCPINFO.STLSIZE+size-pAREA[i].filesize;
	
	if(size & 0x3)
		size=size+0x04-size&0x03;
		
	pAREA[i].filesize=size;
	pAREA[i].pInfo=s;

	offset=pAREA[i].offset+pAREA[i].filesize;
	
	for(i++;i<NCPINFO.FileCount;i++)
	{
        pAREA[i].offset=offset;
		offset=pAREA[i].offset+pAREA[i].filesize;
	}
	
	if(!MG_FWRITE(NULL,NULL,path,DEVICE))
	{
		ErrNo=MG_FILEREADERR;
		goto Err;
	}

Err:
	if(ErrNo)
	{
		Uart_Printf("can't load@@%s\r\n",path);
	}
	return ErrNo;

}
*/
