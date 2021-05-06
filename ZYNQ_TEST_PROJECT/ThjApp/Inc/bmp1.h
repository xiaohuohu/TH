# ifndef 	___BMP_H___
# define 	___BMP_H___

/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  �ļ���:        bmp.h
  �����ˣ�       ��־��
  �������ڣ�		2005/08/15
  ��Ҫ��������:  ����16ɫ�Լ�256ɫλͼ����ʾ

  �޸ļ�¼��
   1.  ...
*************************************************/

# include 	<stdio.h>
# include 	<stdlib.h>
# include 	"os_cpu.h"
# include 	"rect.h"

#define	DARKGRAYCOLOR2	0x505050	 //���


#define FS_size_t       unsigned long       /* 32 bit unsigned */
#define FS_u32          unsigned long       /* 32 bit unsigned */
#define FS_i32          signed long         /* 32 bit signed */
#define FS_u16          unsigned short      /* 16 bit unsigned */
#define FS_i16          signed short        /* 16 bit signed */


typedef struct {
  FS_u32 fileid_lo;          /* unique id for file (lo)      */
  FS_u32 fileid_hi;          /* unique id for file (hi)      */
  FS_u32 fileid_ex;          /* unique id for file (ex)      */
  FS_i32 EOFClust;           /* used by FAT FSL only         */
  FS_u32 CurClust;           /* used by FAT FSL only         */
  FS_i32 filepos;            /* current position in file     */
  FS_i32 size;               /* size of file                 */
  int dev_index;             /* index in _FS_devinfo[]       */
  FS_i16 error;              /* error code                   */
  unsigned char inuse;       /* handle in use mark           */
  unsigned char mode_r;      /* mode READ                    */
  unsigned char mode_w;      /* mode WRITE                   */
  unsigned char mode_a;      /* mode APPEND                  */
  unsigned char mode_c;      /* mode CREATE                  */
  unsigned char mode_b;      /* mode BINARY                  */
} FS_FILE;


/*************************************************
  ������:     	 CBmp_Init
  ��Ҫ����:    	 λͼ����������ݳ�ʼ��
  �������:       ��
  �������:       ��
  ����ֵ:         ��
*************************************************/
void	CBmp_Init(void);

/*************************************************
  ������:     	 CBmp_Destroy
  ��Ҫ����:    	 λͼ������������ͷţ�����λͼ�ļ��Ĺر�
  �������:       ��
  �������:       ��
  ����ֵ:         ��
*************************************************/
void	CBmp_Destroy(void);

/*************************************************
  ������:     	 CBmp_SetRect
  ��Ҫ����:    	 �趨λͼ��ʾʱ��ʾ��λ�ã�������ʱδ�ã�
  �������:       rect:	�趨�ķ�Χ
  �������:       ��
  ����ֵ:         ��
*************************************************/
void 	CBmp_SetRect(CRect rect);//������ʾ��Χ
	
/*************************************************
  ������:     	 CBmp_ReadBmpFile
  ��Ҫ����:    	 �򿪲���ȡָ����λͼ�ļ�
  �������:       filename:	Ҫ��ȡ��λͼ�ļ���
  �������:       ��
  ����ֵ:         ��ȷ��ȡ�ļ�����TRUE�����򷵻�FALSE
*************************************************/
BOOL 	CBmp_ReadBmpFile(INT8S *filename);//��ȡָ���ļ�BMP����

/*************************************************
  ������:     	 CBmp_View
  ��Ҫ����:    	 ��ʾλͼ��ָ����λ��
  �������:       X:	ָ��λͼ��ʾ��ˮƽλ�ã� -1��ʾ������ʾ
  				 Y: ָʾλͼ��ʾ�Ĵ�ֱλ�ã� -1��ʾ������ʾ
  �������:       ��
  ����ֵ:         ���������Ҫ��ʾ�ļ���ʾ������TRUE�����򷵻�FALSE
*************************************************/
BOOL 	CBmp_View(INT16S x,INT16S y);//��ʾλ��,�������ʾ��Χ

/*************************************************
  ������:     	 CBmp_Put01
  ��Ҫ����:    	 ��ʾ��ɫλͼ��ָ����λ��
  �������:      X:   ָ��λͼ��ʾ��ˮƽλ��
				 Y:   ָʾλͼ��ʾ�Ĵ�ֱλ��
  �������:      ��
  ����ֵ:        ��
*************************************************/
void 	CBmp_Put01(INT16S x,INT16S y);

//void	CBmp_GetFullScr24Bit(char *filename);
void	CBmp_GetFullScr24Bit(char *filename,INT16S isVirtual);
void	CBmp_GetFullScrMono(char *filename);
void	CBmp_DispXMSBmp(INT16S x,INT16S y,INT16U handle);
/*************************************************
  ������:     	 CBmp_GetWidth
  ��Ҫ����:    	 ��ȡ��ǰ��ʾλͼ�Ŀ��
  �������:       ��
  �������:       ��
  ����ֵ:         ��ǰ��ʾλͼ���
*************************************************/
INT16S 	CBmp_GetWidth(void);

/*************************************************
  ������:     	 CBmp_GetHeight
  ��Ҫ����:    	 ��ȡ��ǰ��ʾλͼ�ĸ߶�
  �������:       ��
  �������:       ��
  ����ֵ:         ��ǰ��ʾλͼ�߶�
*************************************************/
INT16S 	CBmp_GetHeight(void);

char	CreatBMP(void *filename,INT16S Wide,INT16S Hight,INT16S color);

INT32S InitDispBMP(void *filename);

INT8U DispBMP(void *filename,char color,int x,int y);



#endif
