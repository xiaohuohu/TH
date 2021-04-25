#ifndef _MMU_H_
#define _MMU_H_


/* 各外设分配MMU空间大小  */
#define MMU_GEM_DMA_MEMORY_SIZE  		0x80000 	//512K[暂时需要128K]
#define MMU_USB_DEVICE_DMA_MEMORY_SIZE	0x80000		//512K
#define MMU_USB_HOST_DMA_MEMORY_SIZE	0x80000		//512K
#define MMU_MMC_DMA_MEMORY_SIZE			0x80000		//512K




typedef enum _MMU_TYPE{
	MMU_ADDR_VIEDO,
	MMU_ADDR_ETHERNET0,
	MMU_ADDR_ETHERNET1,
	MMU_ADDR_USB_DEVICE,
	MMU_ADDR_USB_HOST,
	MMU_ADDR_MMC
}MMU_TYPE;



void MMU_Init(void);
u32  MMU_MallocAddr(MMU_TYPE type);


#endif
