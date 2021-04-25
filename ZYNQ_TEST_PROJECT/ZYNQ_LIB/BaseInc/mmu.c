/*
 * mmu.c
 *
 *  Created on: 2014-5-13
 *      Author: Administrator
 */

#include <string.h>
#include "adt_datatype.h"
#include "xil_platform.h"
#include "system.h"
#include "mmu.h"

/* ZYNQ_LIB类型  */
extern int Zynq_Lib_Type;

extern void Xil_SetTlbAttributes(u32 Addr, u32 attrib);

#define MMU_ALIGN_SIZE		0x100000				//1M
#define MMU_SECTOR_SIZE		0x100000				//1M
#define MMU_MEM_AREA_SIZE	(MMU_SECTOR_SIZE*3)		//3M

u8  __attribute__((aligned(MMU_ALIGN_SIZE))) MMU_MEM_AREA[MMU_MEM_AREA_SIZE]; 	// Align: MMU_ALIGN_SIZE

#define MMU_VIDEO_AREA_SIZE (MMU_SECTOR_SIZE*6)		//6M 分辨率最高支持到 1280*1024
static u8 *MMU_VIDEO_MEMORY = (u8*)(0x1da00000);// 0x1da00000 - 0x1dffffff 6M固定地址空间为显存使用空间

#define MMU_VIDEO_AREA_BUS_SIZE (MMU_SECTOR_SIZE*16)		//16M 分辨率最高支持到 1280*1024
static u8 *MMU_VIDEO_BUS_MEMORY = (u8*)(0x1b000000);// 0x1b000000 - 0x1dffffff 16M固定地址空间为显存使用空间


/* mmu attribute - sector */
#define _NS(v) 		((v&0x01)<<19)
#define _nG(v) 		((v&0x01)<<17)
#define _S(v)  		((v&0x01)<<16)
#define _AP(v) 		((v&0x04)<<15) | ((v&0x03)<<10) //access permission
#define _TEX(v)		((v&0x07)<<12)
#define _Domain(v)	((v&0x0f)<<5)
#define _C(v)		((v&0x01)<<3)	// cache
#define _B(v)		((v&0x01)<<2)	// buffer

void MMU_Init(void)
{
	u32 tmp   = 0;
	u32 size;// = sizeof(MMU_MEM_AREA);
	u32 attribute;

	memset(MMU_MEM_AREA,		0x00, MMU_MEM_AREA_SIZE);

	attribute = _NS(0) | _nG(0) | _S(0) | _AP(3) | _TEX(0) | _Domain(0) | _C(0) | _B(0) | 2;


	Xil_SetTlbAttributes(0xFFFF0000,0x14de2);           // S=b1 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0

	/* 显存 */
	if (ZYNQ_LIB_TYPE == Zynq_Lib_Type){
		size = MMU_VIDEO_AREA_SIZE;
	}
	else {
		size = MMU_VIDEO_AREA_BUS_SIZE;
	}

	for(tmp=0; size>0; tmp++){

		if(ZYNQ_LIB_TYPE == Zynq_Lib_Type){
			Xil_SetTlbAttributes((u32)(MMU_VIDEO_MEMORY+(MMU_SECTOR_SIZE*tmp)),attribute);
		}
		else{
			Xil_SetTlbAttributes((u32)(MMU_VIDEO_BUS_MEMORY+(MMU_SECTOR_SIZE*tmp)),attribute);
		}

		if(size>MMU_SECTOR_SIZE) size -= MMU_SECTOR_SIZE;
		else size = 0;
	}

	size = MMU_MEM_AREA_SIZE;
	for(tmp=0;size>0;tmp++)	{

		Xil_SetTlbAttributes((u32)(MMU_MEM_AREA+(MMU_SECTOR_SIZE*tmp)),attribute);
		if(size>MMU_SECTOR_SIZE) size -= MMU_SECTOR_SIZE;
		else size = 0;
	}

	return;
}


u32 MMU_MallocAddr(MMU_TYPE type)
{
	u32 addr = 0;

	switch(type)
	{
	case MMU_ADDR_VIEDO:
		if(ZYNQ_LIB_TYPE == Zynq_Lib_Type){
			addr = (u32)(MMU_VIDEO_MEMORY);
		}
		else{
			addr = (u32)(MMU_VIDEO_BUS_MEMORY);
		}
		break;

	case MMU_ADDR_ETHERNET0:
		addr = (u32)(((u32)MMU_MEM_AREA) + 0);
		break;

	case MMU_ADDR_ETHERNET1:
		addr = (u32)(((u32)MMU_MEM_AREA) + 0x80000*1);
		break;

	case MMU_ADDR_USB_DEVICE:
		addr = (u32)(((u32)MMU_MEM_AREA) + 0x80000*2);
		break;

	case MMU_ADDR_USB_HOST:
		addr = (u32)(((u32)MMU_MEM_AREA) + 0x80000*3);
		break;

	case MMU_ADDR_MMC:
		addr = (u32)(((u32)MMU_MEM_AREA) + 0x80000*4);
		break;

	default:
		addr = 0x0;
		break;
	}

	return addr;
}
