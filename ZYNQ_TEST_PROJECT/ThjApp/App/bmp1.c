//把里面动态内存申请换为你实际用的就行
#include   "includes.h"

//*************************** 截图BMB *************************//

typedef struct 				//tagRGBQUADE
{
	INT8U   rgbBlue;    				// 蓝色的亮度(值范围为0-255)
	INT8U   rgbGreen;   				// 绿色的亮度(值范围为0-255)
	INT8U   rgbRed;     				// 红色的亮度(值范围为0-255)
	INT8U   rgbReserved;				// 保留，必须为0
} __attribute__ ((packed)) RGBQUADE;



typedef struct 				//tagRGBQUADE
{
	INT8U   Blue;    					// 蓝色的亮度(值范围为0-255)
	INT8U   Green;   					// 绿色的亮度(值范围为0-255)
	INT8U   Red;     					// 红色的亮度(值范围为0-255)
} __attribute__ ((packed)) RGBDATA;


typedef  struct 				//tagBITMAPINFO
{
	INT16U  bfType; 					//位图文件的类型，必须为BM
	INT32U  bfSize; 					//位图文件的大小，以字节为单位
	INT16U  bfReserved1; 				// 位图文件保留字，必须为0
	INT16U  bfReserved2; 				// 位图文件保留字，必须为0
	INT32U  bfOffBits; 					// 位图数据的起始位置，以相对于位图


	INT32U  biSize;       				// 本结构所占用字节数
	INT32U  biWidth;      				// 位图的宽度，以像素为单位
	INT32U  biHeight;     				// 位图的高度，以像素为单位
	INT16U  biPlanes;     				// 目标设备的级别，必须为1
	INT16U  biBitCount;      			// 每个像素所需的位数，必须是1(双色),
	                                	// 4(16色)，8(256色)或24(真彩色)之一
	INT32U  biCompression;   			// 位图压缩类型，必须是 0(不压缩),
	                               		// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	INT32U  biSizeImage;     			// 位图的大小，以字节为单位
	INT32U  biXPelsPerMeter; 			// 位图水平分辨率，每米像素数
	INT32U  biYPelsPerMeter; 			// 位图垂直分辨率，每米像素数
	INT32U  biClrUsed;       			// 位图实际使用的颜色表中的颜色数
	INT32U  biClrImportant;  			// 位图显示过程中重要的颜色数

	RGBQUADE bmiColors[1];

}__attribute__ ((packed)) BITMAPINFO;


typedef struct
{

	BITMAPINFO  CreatBmp;
	RGBDATA cdata[800*600];

}__attribute__ ((packed)) CBMPDATA;



	static CBMPDATA  CBMP;
//	extern U32 (*GetPixel)(U32,U32);
//	FS_FILE *pfile;
//	BITMAPINFO bmpinfo;
//	static	char RGBbuf[800*600*3];
//	RGBQUADE  Colrlist[256];

INT32U GetPixelTem(INT16S x,INT16S y)
{
	INT32U Color;

	Color = CDC_GetPixel(x,y);

	return Color;
}





/*****************************************************************
函数功能:创建位图文件
******************************************************************/
char CreatBMP(void *filename,INT16S Wide,INT16S Hight,INT16S color)
{
	FIL 		pfile;
	FRESULT		res;
	UINT		bw;

	INT16S 		W,H1;
	INT8U 		*temp;
	INT32U 		data;

	{

		Wide=Wide-Wide%4;

		CBMP.CreatBmp.bfType=0x4D42; 				// 位图文件的类型，必须为BM
		CBMP.CreatBmp.bfSize=0x36+Wide*Hight*3; 	// 位图文件的大小，以字节为单位
		CBMP.CreatBmp.bfReserved1=0; 				// 位图文件保留字，必须为0
		CBMP.CreatBmp.bfReserved2=0; 				// 位图文件保留字，必须为0

		CBMP.CreatBmp.bfOffBits=0x36; 				// 位图数据的起始位置，以相对于位图
		CBMP.CreatBmp.biSize=   0x28;       		// 本结构所占用字节数

		CBMP.CreatBmp.biWidth=  Wide;      			// 位图的宽度，以像素为单位
		CBMP.CreatBmp.biHeight= Hight;     			// 位图的高度，以像素为单位
		CBMP.CreatBmp.biPlanes= 1;     				// 目标设备的级别，必须为1
		CBMP.CreatBmp.biBitCount=0x18;      		// 每个像素所需的位数，必须是1(双色),
		                                			// 4(16色)，8(256色)或24(真彩色)之一
		CBMP.CreatBmp.biCompression=0x0;   			// 位图压缩类型，必须是 0(不压缩),
		                                			// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
		CBMP.CreatBmp.biSizeImage=Wide*Hight*3;     	// 位图的大小，以字节为单位
		CBMP.CreatBmp.biXPelsPerMeter=0; 			// 位图水平分辨率，每米像素数
		CBMP.CreatBmp.biYPelsPerMeter=0; 			// 位图垂直分辨率，每米像素数
		CBMP.CreatBmp.biClrUsed=0;       			// 位图实际使用的颜色表中的颜色数
		CBMP.CreatBmp.biClrImportant=0;  			// 位图显示过程中重要的颜色数

		for(H1=0;H1<Hight;H1++)
		{
			for(W=0;W<Wide;W++)
			{
				data = GetPixelTem(W,Hight-H1-1);

				CBMP.cdata[W+H1*Wide].Red = data&0x00ff;
				CBMP.cdata[W+H1*Wide].Blue =  (data&0xff00)>>8;
				CBMP.cdata[W+H1*Wide].Green =  (data&0xff0000)>>16;

				if(!color)
				{
					CBMP.cdata[W+H1*Wide].Red  = (CBMP.cdata[W+H1*Wide].Red&0xff)*0x10000+
												(CBMP.cdata[W+H1*Wide].Green&0xff)*0x100+
												(CBMP.cdata[W+H1*Wide].Blue&0xff);

					CBMP.cdata[W+H1*Wide].Blue  = CBMP.cdata[W+H1*Wide].Red;
					CBMP.cdata[W+H1*Wide].Green = CBMP.cdata[W+H1*Wide].Red;

				}

			}
		}

		//pfile=FS_FOpen(filename,"w");
		//FS_FWrite((INT8U *)&CBMP,1,CBMP.CreatBmp.bfSize,pfile);

		res = f_open(&pfile,filename,FA_CREATE_ALWAYS|FA_WRITE);
		res = f_write(&pfile,(INT8U *)&CBMP,CBMP.CreatBmp.bfSize,&bw);
	}

	f_close(&pfile) ;

	return 0;
}



unsigned char ReadFileOffset(unsigned int offset, char *strFileName,void *buf,unsigned int BufSize,unsigned int *FileSize)
{
	INT8U Error = 0;
	//static FIL fd;
	static FIL fd2[4];
	FRESULT res;
	UINT	br;


	res = f_open(&fd2[2],(char*)strFileName,FA_OPEN_EXISTING|FA_READ);
	if(res == FR_OK)
	{
		res = f_lseek(&fd2[2],offset);
		if(res != FR_OK){Error = res;goto Err;}
		res = f_read(&fd2[2],buf,BufSize,&br);
		*FileSize = br;
		if(res != FR_OK){Error = res;goto Err;}
		f_close(&fd2[2]);
	}
	else
	{
		f_close(&fd2[2]) ;
		Error = 1;
		goto Err;
	}


Err:
	return Error;
}


INT32S InitDispBMP(void *filename)
{
	FIL 		fd;
	FRESULT		res;
	UINT		br;

	res= f_open(&fd, filename, FA_OPEN_EXISTING|FA_READ);

	if(res != FR_OK)
	{
		f_close(&fd) ;
		return FALSE;
	}

	f_close(&fd) ;
	return TRUE;
}


INT8U DispBMP(void *filename,char color,int x,int y)
{
	BITMAPINFO bmpinfo;
	FRESULT res=FR_OK;
	UINT	br;

	static	char *RGBbuf;
	RGBQUADE  Colrlist[256];
	INT8U Red=0,Green=0,Blue=0;
	int W,H,i,k;
	unsigned long data=0,ColorCell=0;

	res = ReadFileOffset(0,filename,(char *)&bmpinfo,sizeof(bmpinfo),&br);
	if(res != FR_OK)return(0);		//图片文件不存在返回

	RGBbuf = malloc_k(800*600*3,"RGBbuf");
	if (!RGBbuf) return 0;


	res = ReadFileOffset(bmpinfo.bfOffBits,filename,RGBbuf,bmpinfo.biSizeImage,&br);

	res = ReadFileOffset(bmpinfo.biSize+8,filename,(char *)&Colrlist,sizeof(Colrlist),&br);

	for(i=0;i<4;i++)
	for(H=0;H<bmpinfo.biHeight;H++)
	for(W=0;W<bmpinfo.biWidth/4;W++)
		{
			switch(bmpinfo.biBitCount)
			{
				case 1:
					ColorCell = (W*4+i+H*(bmpinfo.biWidth))-H*(bmpinfo.biWidth%4);
					for(k=0;k<8;k++)
					{
						if((RGBbuf[ColorCell]>>k)&0x01)
							CDC_PutPoint(W*4+i+k,H,0xffffff);
						else
							CDC_PutPoint(W*4+i+k,H,0);
					}
					///W +=8;
					break;
				case 4:



					break;
				case 8:

					ColorCell = (W*4+i+H*(bmpinfo.biWidth))+H*(bmpinfo.biWidth%4);
					//data = RGBT(Colrlist[RGBbuf[ColorCell]].rgbBlue,Colrlist[RGBbuf[ColorCell]].rgbReserved,Colrlist[RGBbuf[ColorCell]].rgbRed);
					CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);

					break;
				case 16:
					break;
				case 24:

					ColorCell = (W*4+i+H*(bmpinfo.biWidth))*3+H*(bmpinfo.biWidth%4);

					switch(color)
					{
					case 0:

						/*
						if((RGBbuf[ColorCell+0]<0x80)||
						   (RGBbuf[ColorCell+1]<0x80)||
						   (RGBbuf[ColorCell+2]<0x80)
						  )

							CDC_PutPoint(W+x,(bmpinfo.biHeight-H)+y,0);
						else
							CDC_PutPoint(W+x,(bmpinfo.biHeight-H)+y,1);

							*/
						if((RGBbuf[ColorCell+0])!=
						   (RGBbuf[ColorCell+1])!=
						   (RGBbuf[ColorCell+2])
						  )
						  {
						Red   = RGBbuf[ColorCell+2];
						Green = RGBbuf[ColorCell+1];
						Blue  = RGBbuf[ColorCell+0];
						data=Red*0x10000+Green*0x100+Blue;
						////if((Red<0xf0)||(Green<0xf0)||(Blue<0xf0))
						CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);
						}

					break;
					case 1:
						Red   = RGBbuf[ColorCell+2];
						Green = RGBbuf[ColorCell+1];
						Blue  = RGBbuf[ColorCell+0];
						data=Red*0x10000+Green*0x100+Blue;
						////if((Red<0xf0)||(Green<0xf0)||(Blue<0xf0))
						CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);
					break;
					case 2:
						Red   = RGBbuf[ColorCell+0];
						Green = RGBbuf[ColorCell+1];
						Blue  = RGBbuf[ColorCell+2];
						data=Red*0x10000+Green*0x100+Blue;
						CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);
						break;
					case 4:
					/*
						Red   = RGBbuf[ColorCell+2];
						Green = RGBbuf[ColorCell+1];
						Blue  = RGBbuf[ColorCell+0];
						data=Red*0x10000+Green*0x100+Blue;
						data=OLDcolor(data);
						*/

						Red   = RGBbuf[ColorCell+1];
						Green = RGBbuf[ColorCell+2];
						Blue  = RGBbuf[ColorCell+0];
						data=Red*0x10000+Green*0x100+Blue;

						CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);
					break;
					case 3:
						Red   = RGBbuf[ColorCell+2];
						Green = RGBbuf[ColorCell+0];
						Blue  = RGBbuf[ColorCell+1];
						data=Red*0x10000+Green*0x100+Blue;
					/*
						Red   = RGBbuf[ColorCell+2];
						Green = RGBbuf[ColorCell+1];
						Blue  = RGBbuf[ColorCell+0];
						data=Red*0x10000+Green*0x100+Blue;
						data=ASH(data);
						*/
						CDC_PutPoint(W*4+i+x,(bmpinfo.biHeight-H)+y,data);
					break;
					}


					break;
				default :
					break;
			}

		}
	RGBbuf = free_k(RGBbuf,"RGBbuf");
	return(TRUE);
}


