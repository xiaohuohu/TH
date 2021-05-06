#include "includes.h"

//#include "Language_EN.c"

//extern Image$$RO$$Base;
//extern Image$$RO$$Limit;
//extern Image$$RW$$Base;
//extern Image$$ZI$$Base;
//extern Image$$ZI$$Limit;
//TEST_EXT	OS_SEM		FileSem;												//文件操作信号量,防止同时进行磁盘IO
void *malloc_k(int size,char *ptr);
void *free_k(void *p,char *ptr);

#ifndef true
	#define true	1
#endif
#ifndef false
	#define false	0
#endif
BOOL bLoadZDSuc=FALSE;															//加载字典文件成功标志量


char *GetPos_TR(INT32U *pos)
{
	char *str;
	char *pend,*sk=trtags;	
	char next[50];
	int i,j;
	int lensk;

	//str = (char*)(&Image$$RO$$Base);
	//pend  = (char*)&Image$$ZI$$Limit;
	str=(char*)(IMAGEROMBASE);
	pend=(char*)(IMAGEZILIMIT);

	if((str+*pos)>=pend)
		goto Err;

    lensk=sizeof(sk);

/****************KMP搜索算法********************************/

	for(i=0;i<lensk;i++)
		next[i]=0;   

	i=1;
	j=0;
	next[0]=0;
	
	while(i<=lensk)
	{
		if(j==0 || sk[i]==sk[j])
		{
			++i;
			++j;
			
			if(sk[i]!=sk[j]) 
				next[i]=j;
				
			else 
				next[i]=next[j];								//消去多余的可能的比较,next再向前跳
		}
		else 
			j=next[j];
	}


	i = *pos;
	j = 0;
	
	while((str+i)<pend && j<lensk)
	{
		if(j==0||str[i]==sk[j])
		{
			++i;
			++j;
		}
		else 
			j=next[j];
	}
	
	if(j>=lensk)
	{
		*pos=i-lensk;
		return &str[i-lensk];
	}

Err:
	return NULL;
	
}

INT16U TransferLanguagePack(INT32U LangCode)					//LangCode为0时，不翻译
{

	INT16U ErrNo;
	INT32U pos;
	char *p,*p1,*p2;
	int len,size;
	int i,j,textcount,loop,iTotal;
	char utf8[250];
	//int fd;		//原来用于读写yaffs，因为与FIL 		fd;重名了，所以注释掉
	//off_t err;	//与yaffs相关注释
	char *pBuff=NULL;
	int offset=1024*2*10*sizeof(int);
	INT32U *pTRANSTEXT;
	char *ptemp;

	//FS_FILE *pfile=NULL;
	FIL 		fd;
	FRESULT		res;
	UINT		br;
//	OS_ERR		os_err;
//	CPU_TS    	p_ts;

	INT8U ExportTag;
	char *pArray=NULL;
	char *pStart=NULL;
	TAreaInfo NCPFileInfo;
	char buf[20];
	p2=(char*)(IMAGEROMBASE);
	p1=(char*)(IMAGEZILIMIT);
//	OSSemPend(&FileSem,5000,OS_OPT_PEND_BLOCKING,&p_ts,&os_err);

 	res=f_open(&fd, "\\ADT\\ZIDIAN.ZD", FA_OPEN_EXISTING|FA_READ);     //pfile = FS_FOpen("\\ADT\\ZIDIAN.ZD","r");

 	if(res!=FR_OK)//if (!pfile)									//如果读取失败
 	{
 		switch(LangCode)										//多国语言时，将未翻译的字符输入保存，文件名要与.NCP文件中的文件名相同，很多语言时可自己在下面代码中添加文件 		
 		{
 			case 2:
	 			strcpy(buf,"ZIDIAN_trad.ZD");
	 			break;
	 		case 1:
	 			strcpy(buf,"ZIDIAN.ZD");
	 			break;
	 		
 		}

		if (!MG_GetTAreaInfo(buf,&NCPFileInfo))
		{
			if(!MG_GetMem(buf,(INT32U*)&pArray))
			{
				char *pfilebuf;

				pStart = pArray;
				memcpy(utf8,pArray,3);
				iTotal = 0;
				
				if(utf8[0]==0xef && utf8[1]==0xbb && utf8[2]==0xbf)
				{
					pBuff=malloc_k(offset+NCPFileInfo.filesize+10,"pBuff");
					
					if(pBuff)
					{
						memset(pBuff,0x00,offset+NCPFileInfo.filesize+10);
						pfilebuf = &pBuff[offset];
						pTRANSTEXT = (INT32U *)pBuff;
						
						pArray += 3;
						memcpy(pfilebuf,pArray,NCPFileInfo.filesize-3);
						
						pTRANSTEXT[0]=(INT32U)&pfilebuf[0];

						for(i=0;i<NCPFileInfo.filesize-3;i++)
						{
							if(pfilebuf[i]==0x09 && pfilebuf[i+1]==':')
							{
								pTRANSTEXT[iTotal*2+1]=(INT32U)&pfilebuf[i+2];
								pfilebuf[i]=0x00;
								i++;
								pfilebuf[i]=0x00;

							}
							
							if(pfilebuf[i]==0x0d && pfilebuf[i+1]==0x0a)
							{
								iTotal++;
								pTRANSTEXT[iTotal*2]=(INT32U)&pfilebuf[i+2];
								pfilebuf[i]=0x00;
								i++;
								pfilebuf[i]=0x00;
							}
						}
						pfilebuf[i]=0x00;
					}
				}
			    
				MG_FreeMem(buf);
			}
//		Uart_Printf("字典文件读操作失败\r\n");
		}
		
	}
	else
	{
		char *pfilebuf;
		INT16S 		ibufcount = 0;
		bLoadZDSuc=TRUE;
		//err=FS_FSeek(pfile,0,SEEK_SET);
		//size=FS_FRead(utf8,1,3,pfile);
		res = f_lseek(&fd,ibufcount);
		res = f_read(&fd,utf8,3,&br);
		size=3;											//暂未处理好size的算法
		iTotal=0;

		if(size==3 && utf8[0]==0xef && utf8[1]==0xbb && utf8[2]==0xbf)
		{
			pBuff=malloc_k(offset+(&fd)->fsize+10,"pBuff");
			
//			Uart_Printf("pbuf:%x %d %d\r\n",pBuff,offset,pfile->size);

			if(pBuff)
			{
				memset(pBuff,0x00,offset+(&fd)->fsize+10);
				pfilebuf = &pBuff[offset];
				pTRANSTEXT = (INT32U *)pBuff;

				res = f_lseek(&fd,ibufcount);
				res = f_read(&fd,pfilebuf,(&fd)->fsize,&br);//size = FS_FRead(pfilebuf,1,(&fd)->fsize,pfile);

				size=(&fd)->fsize;
				
				pTRANSTEXT[0] = (INT32U)&pfilebuf[0];

				for(i=0;i<size;i++)
				{
					if(pfilebuf[i]==0x09 && pfilebuf[i+1]==':')
					{
						pTRANSTEXT[iTotal*2+1]=(INT32U)&pfilebuf[i+2];
						pfilebuf[i]=0x00;
						i++;
						pfilebuf[i]=0x00;
						
//						Uart_Printf("99999999%d %s ",iTotal,(char*)pTRANSTEXT[iTotal*2+1]);
					}

					if(pfilebuf[i]==0x0d && pfilebuf[i+1]==0x0a)
					{
						iTotal++;
						pTRANSTEXT[iTotal*2]=(INT32U)&pfilebuf[i+2];
						pfilebuf[i]=0x00;
						i++;
						pfilebuf[i]=0x00;
						
//						Uart_Printf("99999999@%d %s:%s",iTotal,(char*)pTRANSTEXT[iTotal*2-1],(char*)pTRANSTEXT[iTotal*2]);
					}
				}
				
				pfilebuf[i]=0x00;
			}
		}

		f_close(&fd) ;
	}
// 	OSSemPost(&FileSem,OS_OPT_POST_1,&os_err);

/*yaffs相关处理全部注释
	yaffs_unlink((char*)"/flash/TEMP.ZD");
	fd = yaffs_open("/flash/TEMP.ZD",O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
	
	if(fd == -1)
	{
		Uart_Printf("yaffs文件写操作失败\r\n");
	}
	else
	{
		err = yaffs_lseek(fd,0,SEEK_SET);
		size = yaffs_write(fd,"\xEF\xBB\xBF",3);
		
	}
*/
	Uart_Printf("***********************\r\n");
	
	pos=0;
	textcount=0;
	
	for(textcount=0;;)
	{
		p = GetPos_TR(&pos);
		
		if(!p)
			break;

		p1 = p+4;
		pos += 4;
		len = strlen(p1);
		p2 = p1+len+1+1;
		
//		Uart_Printf("%x\r\n",p-0xc008000);

		if(memcmp(p1,p2,len)==0 && len)
		{
			len--;
			p1[len] = 0x00;
			ExportTag = NULL;

			if(LangCode==0 || !pBuff)
			{
				GBK2313ToUTF8(p1,utf8);
				
				if(utf8[0])
					sprintf(p,"\xEF\xBB\xBF%s",utf8);
				else
					p[0]=NULL;
			}
			else
			{
//				Uart_Printf("\taddr:%x offset:%x @%s@\r\n",p,pos,p+4);

				GBK2313ToUTF8(p1,utf8);
				p1 = utf8;
				
				if(!utf8[0])
				{
					p[0]=NULL;
					continue;
				}

				j = iTotal;
				i = textcount;
				
				if(textcount>=iTotal)
					i=0;
					
				if(i!=0)
					loop=true;
				else 
					loop=false;

				for(;;)
				{
//					Uart_Printf("11111111111111111111%d %s:%s",i,p1,(char*)pTRANSTEXT[i*2]);
//					Uart_Printf("%d %d\r\n",i,j);

					if(pTRANSTEXT[i*2]==0x00)
					{
						i=j=0;
						break;
					}
					
					if((*(char*)pTRANSTEXT[i*2]) && (strcmp(p1,(char*)pTRANSTEXT[i*2])==0))
					{
//						Uart_Printf("@%d %d\r\n",textcount,i);

						ptemp = (char*)pTRANSTEXT[i*2+1];
						
						if(strlen(ptemp)>(len*2-0))				//翻译的词汇比原空间大，不予翻译
						{	
							//Uart_Printf("@%d %d\r\n",strlen(ptemp),len*2-3);
							
							i=j=0;								//将无效翻译导出
							ExportTag=1;
							break;
						}

						if(ptemp[0])							//不存在翻译，则取原文
						{
							sprintf(p,"\xEF\xBB\xBF%s",ptemp);
						}
						else
							sprintf(p,"\xEF\xBB\xBF%s",p1);
							
						break;
					}
					
					if(i>=j)
					{
						if(loop)
						{
							i=0;
							j=textcount;
							loop=false;
						}
						else
						{
							break;
						}
					}
					else
						i++;
				}
				
				if(i==j)										//无匹配字段
				{
//					Uart_Printf("%d %d %s@\r\n",len*2+3,strlen(utf8),p1);

					if(utf8[0])
						sprintf(p,"\xEF\xBB\xBF%s",utf8);
					else
						p[0]=NULL;
					/*
					if((fd!=-1) && utf8[0])
					{
						//Uart_Printf("#w...\r\n");
						
						if(ExportTag==1)						//翻译长度太长标识@1@
							len=sprintf(utf8,"@1@>>>>>>%s\x09\x3a\r\n",&p[3]);
						else
							len=sprintf(utf8,"%s\x09\x3a\r\n",utf8);
							
						size=yaffs_write(fd,utf8,len);
						
						if(size!=len)
							Uart_Printf("write error!");
					}
					*/
					//Uart_Printf("#%d %d\r\n",i,j);
				}
				
				textcount++;
			}
			
		}
		
	}
	/*涉及yaffs注释
	if(fd!=-1)
		err=yaffs_close(fd);
	*/

	//Uart_Printf("***********************\r\n");
	
	ErrNo=NULL;

	if(pBuff)
	{
		pBuff = free_k(pBuff,"pBuff");
//		Uart_Printf("pBuff\r\n");
	}
/********************************************************/	
/*
	fd = yaffs_open("/flash/TEMP.ZD",O_WRONLY, S_IREAD);
	
	if(fd == -1)
	{
		Uart_Printf("yaffs文件读操作失败\r\n");
		goto Err;
	}
	
 	pfile = FS_FOpen("\\ADT\\TEMP.ZD","w");
 	
 	if(!pfile)
 	{
		Uart_Printf("FAT文件写操作失败\r\n");
		goto Err;
 	}
 	
 	for(;;)
 	{
		size=yaffs_read(fd,utf8,sizeof(utf8));
		
		FS_FWrite(utf8,size,1,pfile);
		
		if(size!=sizeof(utf8))
			break;
	
 	}
Err:	
	if(fd!=-1)
		err=yaffs_close(fd);
		
 	if(pfile)
 		FS_FClose(pfile);
	*/
	return ErrNo;
}


