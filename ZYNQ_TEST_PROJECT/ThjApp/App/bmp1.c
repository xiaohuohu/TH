//�����涯̬�ڴ����뻻Ϊ��ʵ���õľ���
#include   "includes.h"

//*************************** ��ͼBMB *************************//

typedef struct 				//tagRGBQUADE
{
	INT8U   rgbBlue;    				// ��ɫ������(ֵ��ΧΪ0-255)
	INT8U   rgbGreen;   				// ��ɫ������(ֵ��ΧΪ0-255)
	INT8U   rgbRed;     				// ��ɫ������(ֵ��ΧΪ0-255)
	INT8U   rgbReserved;				// ����������Ϊ0
} __attribute__ ((packed)) RGBQUADE;



typedef struct 				//tagRGBQUADE
{
	INT8U   Blue;    					// ��ɫ������(ֵ��ΧΪ0-255)
	INT8U   Green;   					// ��ɫ������(ֵ��ΧΪ0-255)
	INT8U   Red;     					// ��ɫ������(ֵ��ΧΪ0-255)
} __attribute__ ((packed)) RGBDATA;


typedef  struct 				//tagBITMAPINFO
{
	INT16U  bfType; 					//λͼ�ļ������ͣ�����ΪBM
	INT32U  bfSize; 					//λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
	INT16U  bfReserved1; 				// λͼ�ļ������֣�����Ϊ0
	INT16U  bfReserved2; 				// λͼ�ļ������֣�����Ϊ0
	INT32U  bfOffBits; 					// λͼ���ݵ���ʼλ�ã��������λͼ


	INT32U  biSize;       				// ���ṹ��ռ���ֽ���
	INT32U  biWidth;      				// λͼ�Ŀ�ȣ�������Ϊ��λ
	INT32U  biHeight;     				// λͼ�ĸ߶ȣ�������Ϊ��λ
	INT16U  biPlanes;     				// Ŀ���豸�ļ��𣬱���Ϊ1
	INT16U  biBitCount;      			// ÿ�����������λ����������1(˫ɫ),
	                                	// 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	INT32U  biCompression;   			// λͼѹ�����ͣ������� 0(��ѹ��),
	                               		// 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	INT32U  biSizeImage;     			// λͼ�Ĵ�С�����ֽ�Ϊ��λ
	INT32U  biXPelsPerMeter; 			// λͼˮƽ�ֱ��ʣ�ÿ��������
	INT32U  biYPelsPerMeter; 			// λͼ��ֱ�ֱ��ʣ�ÿ��������
	INT32U  biClrUsed;       			// λͼʵ��ʹ�õ���ɫ���е���ɫ��
	INT32U  biClrImportant;  			// λͼ��ʾ��������Ҫ����ɫ��

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
��������:����λͼ�ļ�
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

		CBMP.CreatBmp.bfType=0x4D42; 				// λͼ�ļ������ͣ�����ΪBM
		CBMP.CreatBmp.bfSize=0x36+Wide*Hight*3; 	// λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
		CBMP.CreatBmp.bfReserved1=0; 				// λͼ�ļ������֣�����Ϊ0
		CBMP.CreatBmp.bfReserved2=0; 				// λͼ�ļ������֣�����Ϊ0

		CBMP.CreatBmp.bfOffBits=0x36; 				// λͼ���ݵ���ʼλ�ã��������λͼ
		CBMP.CreatBmp.biSize=   0x28;       		// ���ṹ��ռ���ֽ���

		CBMP.CreatBmp.biWidth=  Wide;      			// λͼ�Ŀ�ȣ�������Ϊ��λ
		CBMP.CreatBmp.biHeight= Hight;     			// λͼ�ĸ߶ȣ�������Ϊ��λ
		CBMP.CreatBmp.biPlanes= 1;     				// Ŀ���豸�ļ��𣬱���Ϊ1
		CBMP.CreatBmp.biBitCount=0x18;      		// ÿ�����������λ����������1(˫ɫ),
		                                			// 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
		CBMP.CreatBmp.biCompression=0x0;   			// λͼѹ�����ͣ������� 0(��ѹ��),
		                                			// 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
		CBMP.CreatBmp.biSizeImage=Wide*Hight*3;     	// λͼ�Ĵ�С�����ֽ�Ϊ��λ
		CBMP.CreatBmp.biXPelsPerMeter=0; 			// λͼˮƽ�ֱ��ʣ�ÿ��������
		CBMP.CreatBmp.biYPelsPerMeter=0; 			// λͼ��ֱ�ֱ��ʣ�ÿ��������
		CBMP.CreatBmp.biClrUsed=0;       			// λͼʵ��ʹ�õ���ɫ���е���ɫ��
		CBMP.CreatBmp.biClrImportant=0;  			// λͼ��ʾ��������Ҫ����ɫ��

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
	if(res != FR_OK)return(0);		//ͼƬ�ļ������ڷ���

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


