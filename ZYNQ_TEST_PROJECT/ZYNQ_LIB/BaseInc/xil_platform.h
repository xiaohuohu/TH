#ifndef __XIL_PLATFORM_H__
#define __XIL_PLATFORM_H__

#include "adt_datatype.h"


#define readb(addr) \
  ({ unsigned char __v = (*(volatile unsigned char *) (addr)); __v; })
#define readw(addr) \
  ({ unsigned short __v = (*(volatile unsigned short *) (addr)); __v; })
#define readl(addr) \
	({ unsigned int __v = (*(volatile unsigned long *) (addr)); __v; })

#define writeb(b, addr) \
  (void)((*(volatile unsigned char *) (addr)) = (b))
#define writew(b, addr) \
  (void)((*(volatile unsigned short *) (addr)) = (b))
#define writel(b,addr) \
  (void)((*(volatile unsigned long *) (addr)) =(b))



/* ******************Common *********************** */
#define BIT_MASK(bit)       (0x1<<0)

#define SLCR_LOCK_MASK		0x767B
#define SLCR_UNLOCK_MASK 	0xDF0D
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_100MBPS_DIV0 	8
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_100MBPS_DIV1 	5
#define XPAR_PS7_ETHERNET_1_ENET_SLCR_100MBPS_DIV0 	8
#define XPAR_PS7_ETHERNET_1_ENET_SLCR_100MBPS_DIV1 	5

/* System Level Control Registers (slcr)  */
#define rSLCR_LOCK			(*(volatile u32*)0xF8000004)
#define rSLCR_UNLOCK		(*(volatile u32*)0xF8000008)
#define rSLCR_LOCKSTA		(*(volatile u32*)0xF800000c)
#define rSLCR_GEM0_CLK_CTRL (*(volatile u32*)0xF8000140)
#define rSLCR_GEM1_CLK_CTRL (*(volatile u32*)0xF8000144)
#define rUART_RST_CTRL  	(*(volatile u32*)0xF8000228)
#define rPSS_RST_CTRL		(*(volatile u32*)0xF8000200)
#define rA9_CPU_RST_CTRL 	(*(volatile u32*)0xF8000244)
 #define PERI_RST_MASK		0x0100
 #define A9_CLKSTOP1_MASK	0x0020
 #define A9_CLKSTOP0_MASK	0x0010
 #define A9_RST1_MASK		0x0002
 #define A9_RST0_MASK		0x0001
#define rREBOOT_STATUS   	(*(volatile u32*)0xF8000258)

#define rMIO_PIN_07		(*(volatile u32*)0xF800071C)
#define rMIO_PIN_08		(*(volatile u32*)0xF8000720)
#define rMIO_PIN_09		(*(volatile u32*)0xF8000724)

/* Application Processing Unit (mpcore) */
#define rICCICR  		(*(volatile u32*)(0xF8F00100+0x00))
#define rICCPMR  		(*(volatile u32*)(0xF8F00100+0x04))
#define rICCBPR  		(*(volatile u32*)(0xF8F00100+0x08))
#define rICCIAR  		(*(volatile u32*)(0xF8F00100+0x0C))
#define rICCEOIR 		(*(volatile u32*)(0xF8F00100+0x10))
#define rICCRPR  		(*(volatile u32*)(0xF8F00100+0x14))
#define rICCHPIR 		(*(volatile u32*)(0xF8F00100+0x18))
#define rICCABPR 		(*(volatile u32*)(0xF8F00100+0x1c))
#define rICCIDR			(*(volatile u32*)(0xF8F001fc))

#define rICDICER0 		(*(volatile u32*)(0xF8F01180))
#define rICDICER1 		(*(volatile u32*)(0xF8F01184))
#define rICDICER2 		(*(volatile u32*)(0xF8F01188))

#define rICDICPR0 		(*(volatile u32*)(0xF8F01280))
#define rICDICPR1 		(*(volatile u32*)(0xF8F01284))
#define rICDICPR2 		(*(volatile u32*)(0xF8F01288))

#define rICDIPTR0 		(*(volatile u32*)0xF8F01800)
#define rICDIPTR1 		(*(volatile u32*)0xF8F01804)
#define rICDIPTR2 		(*(volatile u32*)0xF8F01808)
#define rICDIPTR3 		(*(volatile u32*)0xF8F0180c)
#define rICDIPTR4 		(*(volatile u32*)0xF8F01810)
#define rICDIPTR5 		(*(volatile u32*)0xF8F01814)
#define rICDIPTR6 		(*(volatile u32*)0xF8F01818)
#define rICDIPTR7 		(*(volatile u32*)0xF8F0181c)
#define rICDIPTR8 		(*(volatile u32*)0xF8F01820)
#define rICDIPTR9 		(*(volatile u32*)0xF8F01824)
#define rICDIPTR10 		(*(volatile u32*)0xF8F01828) /* ID#42 ID#43 */
#define rICDIPTR11 	 	(*(volatile u32*)0xF8F0182c) /* ID#44 */
#define rICDIPTR12 		(*(volatile u32*)0xF8F01830)
#define rICDIPTR13		(*(volatile u32*)0xF8F01834)
#define rICDIPTR14		(*(volatile u32*)0xF8F01838)
#define rICDIPTR15		(*(volatile u32*)0xF8F0183c)
#define rICDIPTR16		(*(volatile u32*)0xF8F01840)
#define rICDIPTR17 		(*(volatile u32*)0xF8F01844) /* ID#69 ID#70 ID#71 */
#define rICDIPTR18		(*(volatile u32*)0xF8F01848)
#define rICDIPTR19 		(*(volatile u32*)0xF8F0184c)
#define rICDIPTR20 		(*(volatile u32*)0xF8F01850)
#define rICDIPTR21 		(*(volatile u32*)0xF8F01854)
#define rICDIPTR22 		(*(volatile u32*)0xF8F01858)
#define rICDIPTR23 		(*(volatile u32*)0xF8F0185c)

#define rICDICFR0  		(*(volatile u32*)0xF8F01C00)
#define rICDICFR1  		(*(volatile u32*)0xF8F01C04)
#define rICDICFR2  		(*(volatile u32*)0xF8F01C08)
#define rICDICFR3  		(*(volatile u32*)0xF8F01C0c)
#define rICDICFR4  		(*(volatile u32*)0xF8F01C10)
#define rICDICFR5  		(*(volatile u32*)0xF8F01C14)

/* General Purpose I/O (gpio)  */
#define rGPIO_MASK_DATA_0_LSW	(*(volatile u32*)0xE000A000)
#define rGPIO_MASK_DATA_0_MSW	(*(volatile u32*)0xE000A004)
#define rGPIO_MASK_DATA_1_LSW	(*(volatile u32*)0xE000A008)
#define rGPIO_MASK_DATA_1_MSW	(*(volatile u32*)0xE000A00C)
#define rGPIO_MASK_DATA_2_LSW	(*(volatile u32*)0xE000A010)
#define rGPIO_MASK_DATA_2_MSW	(*(volatile u32*)0xE000A014)
#define rGPIO_MASK_DATA_3_LSW	(*(volatile u32*)0xE000A018)
#define rGPIO_MASK_DATA_3_MSW	(*(volatile u32*)0xE000A01C)

#define rGPIO_DATA_0_RO 		(*(volatile u32*)0xE000A060)
#define rGPIO_DATA_1_RO 		(*(volatile u32*)0xE000A064)
#define rGPIO_DATA_2_RO 		(*(volatile u32*)0xE000A068)
#define rGPIO_DATA_3_RO 		(*(volatile u32*)0xE000A06C)

#define rGPIO_DIRM_0 			(*(volatile u32*)0xE000A204)
#define rGPIO_OEN_0 			(*(volatile u32*)0xE000A208)
#define rGPIO_DIRM_1 			(*(volatile u32*)0xE000A244)
#define rGPIO_OEN_1 			(*(volatile u32*)0xE000A248)
#define rGPIO_DIRM_2 			(*(volatile u32*)0xE000A284)
#define rGPIO_OEN_2 			(*(volatile u32*)0xE000A288)
#define rGPIO_DIRM_3 			(*(volatile u32*)0xE000A2c4)
#define rGPIO_OEN_3 			(*(volatile u32*)0xE000A2c8)

/* Definition for CPU ID */
#ifndef XPAR_CPU_ID
#define XPAR_CPU_ID 0
#endif

/* Definitions for peripheral PS7_CORTEXA9_0 */
#ifndef XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ
#define XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ 666666687
#endif

/******************************************************************/

/* Canonical definitions for peripheral PS7_CORTEXA9_0 */
#ifndef XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ
#define XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ 666666687
#endif



/* Definitions for peripheral PS7_SCUGIC_0 */
#ifndef XPAR_PS7_SCUGIC_0_DEVICE_ID
#define XPAR_PS7_SCUGIC_0_DEVICE_ID 0
#endif
#ifndef XPAR_PS7_SCUGIC_0_BASEADDR
#define XPAR_PS7_SCUGIC_0_BASEADDR 0xF8F00100
#endif
#ifndef XPAR_PS7_SCUGIC_0_HIGHADDR
#define XPAR_PS7_SCUGIC_0_HIGHADDR 0xF8F001FF
#endif
#ifndef XPAR_PS7_SCUGIC_0_DIST_BASEADDR
#define XPAR_PS7_SCUGIC_0_DIST_BASEADDR 0xF8F01000
#endif

/* Definitions for peripheral PS7_SCUC_0 */
#ifndef XPAR_PS7_SCUC_0_S_AXI_BASEADDR
#define XPAR_PS7_SCUC_0_S_AXI_BASEADDR 0xF8F00000
#endif

#ifndef XPAR_PS7_SCUC_0_S_AXI_HIGHADDR
#define XPAR_PS7_SCUC_0_S_AXI_HIGHADDR 0xF8F000FC
#endif

/* Definitions for peripheral PS7_L2CACHEC_0 */
#ifndef XPAR_PS7_L2CACHEC_0_S_AXI_BASEADDR
#define XPAR_PS7_L2CACHEC_0_S_AXI_BASEADDR 0xF8F02000
#endif

#ifndef XPAR_PS7_L2CACHEC_0_S_AXI_HIGHADDR
#define XPAR_PS7_L2CACHEC_0_S_AXI_HIGHADDR 0xF8F02FFF
#endif

/* Definitions for peripheral PS7_SCUTIMER_0 */
#ifndef XPAR_PS7_SCUTIMER_0_DEVICE_ID
#define XPAR_PS7_SCUTIMER_0_DEVICE_ID 0
#endif
#ifndef XPAR_PS7_SCUTIMER_0_BASEADDR
#define XPAR_PS7_SCUTIMER_0_BASEADDR 0xF8F00600
#endif
#ifndef XPAR_PS7_SCUTIMER_0_HIGHADDR
#define XPAR_PS7_SCUTIMER_0_HIGHADDR 0xF8F0061F
#endif



/* UART
 *
 * */
#define UART0_BASE_ADDR		0xE0000000
#define UART1_BASE_ADDR		0xE0001000
#define UART_CLK_FREQ_HZ	100000000
#define UART0_IRQ_ID		59
#define UART1_IRQ_ID		82

typedef struct uart_regs{
	u32 control;	/*0x00*/
	u32 mode;		/*0x04*/
	u32 intr_en;	/*0x08*/
	u32 intr_dis;	/*0x0c*/
	u32 intr_mask;	/*0x10*/
	u32 intr_sts;	/*0x14*/
	u32 baud_rate_gen;/*0x18*/
	u32 recv_timeout;/*0x1C*/
	u32 recv_fifo_trigger;	/*0x20*/
	u32 modem_ctrl;	/*0x24*/
	u32 modem_sts;	/*0x28*/
	u32 channel_sts;/*0x2c*/
	u32 tx_rx_fifo;	/*0x30*/
	u32 baud_rate_div;	/*0x34*/
	u32 flow_delay;	/*0x38*/
	u32 reserved0[2];
	u32 tx_fifo_trigger;/*0x44*/
}__attribute__((__packed__)) uart_controller_regs;


/* UartLite
 *
 * */
#define UARTLITE2_BASE_ADDR		0x82c00000
#define UARTLITE2_HIGH_ADDR		0x82c00fff
#define UARTLITE2_IRQ_ID 		62 /* INTERRUPT ID */

#define UARTLITE3_BASE_ADDR		0x82c10000
#define UARTLITE3_HIGH_ADDR		0x82c10fff
#define UARTLITE3_IRQ_ID 		67 /* INTERRUPT ID */

#define UARTLITE4_BASE_ADDR		0x82c20000
#define UARTLITE4_HIGH_ADDR		0x82c20fff
#define UARTLITE4_IRQ_ID 		68 /* INTERRUPT ID */

typedef struct uartlite_regs{
	u32 rx_fifo;	/*0x00*/
	u32 tx_fifo;	/*0x04*/
	u32 stat_reg;	/*0x08*/
	u32 ctrl_reg;	/*0x0c*/
}__attribute__((__packed__)) uartlite_controller_regs;



/* TTC
 *  Triple Timer Counter (ttc)
 * */
#define TTC0_BASE_ADDR	0xF8001000
#define TTC1_BASE_ADDR 	0xF8002000
#define TTC0_1_IRQ_ID	42
#define TTC0_2_IRQ_ID	43
#define TTC0_3_IRQ_ID	44
#define TTC1_1_IRQ_ID	69
#define TTC1_2_IRQ_ID	70
#define TTC1_3_IRQ_ID	71
#define TTC_CLK_FREQ_HZ	111111115

typedef struct ttc_regs{
	u32 clk_ctrl_1;	/*0x00*/
	u32 clk_ctrl_2;	/*0x04*/
	u32 clk_ctrl_3;	/*0x08*/
	u32 count_ctrl_1;	/*0x0c*/
	u32 count_ctrl_2;	/*0x10*/
	u32 count_ctrl_3;	/*0x14*/
	u32 count_val_1;	/*0x18*/
	u32 count_val_2;	/*0x1C*/
	u32 count_val_3;	/*0x20*/
	u32 interval_count_1;	/*0x24*/
	u32 interval_count_2;	/*0x28*/
	u32 interval_count_3;	/*0x2c*/
	u32 match_1_count_1;	/*0x30*/
	u32 match_1_count_2;	/*0x34*/
	u32 match_1_count_3;	/*0x38*/
	u32 match_2_count_1;	/*0x3c*/
	u32 match_2_count_2;	/*0x40*/
	u32 match_2_count_3;	/*0x44*/
	u32 match_3_count_1;	/*0x48*/
	u32 match_3_count_2;	/*0x4c*/
	u32 match_3_count_3;	/*0x50*/
	u32 intr_reg_1;			/*0x54*/
	u32 intr_reg_2;			/*0x58*/
	u32 intr_reg_3;			/*0x5c*/
	u32 intr_en_1;			/*0x60*/
	u32 intr_en_2;			/*0x64*/
	u32 intr_en_3;			/*0x68*/
	u32 event_count_timer1;	/*0x6c*/
	u32 event_count_timer2;	/*0x70*/
	u32 event_count_timer3;	/*0x74*/
	u32 event_reg_1;		/*0x78*/
	u32 event_reg_2;		/*0x7c*/
	u32 event_reg_3;		/*0x80*/
}__attribute__((__packed__)) ttc_controller_regs;


/* QSPI
 * Quad-SPI Flash Controller (qspi)
 *  */
/* Definitions for peripheral PS7_QSPI_0 */
#define QSPI_DEVICE_ID		0
#define QSPI_BASE_ADDR		0xE000D000
#define QSPI_CLK_FREQ_HZ	200000000  /* 200 MHz */
#define QSPI_MODE			0

/* QSPI register offsets */
typedef struct qspi_dr_dev {
	u32 config;	/* 0x00 */
	u32 isr;	/* 0x04 */
	u32 ier;	/* 0x08 */
	u32 idr;	/* 0x0C */
	u32 imr;	/* 0x10 */
	u32 en;		/* 0x14 */
	u32 delay;	/* 0x18 */
	u32 txd0;	/* 0x1C */
	u32 rxdata;	/* 0x20 */
	u32 slaveidle;	/* 0x24 */
	u32 txfifo;	/* 0x28 */
	u32 rxfifo;	/* 0x2C */
	u32 gpio;	/* 0x30 */
	u32 res0;	/* 0x34 */
	u32 lpbk;	/* 0x38 */
	u32 reserved0[17];
	u32 txd1;	/* 0x80 */
	u32 txd2;	/* 0x84 */
	u32 txd3;	/* 0x88 */
	u32 reserved1[5];
	u32 lqspicfg;	/* 0xA0 */
	u32 lqspists;	/* 0xA4 */
	u32 reserved2[21];
	u32 modid;		/* 0xFC */
}__attribute__((__packed__)) qspi_controller_regs;


/* SPI
 * SPI Controller (SPI)
 *  */
#define SPI_DEVICE_ID		0
#define SPI_0_BASE_ADDR		0xE0006000
#define SPI_1_BASE_ADDR		0xE0007000
#define SPI_CLK_FREQ_HZ		166666672
#define SPI_MODE			0

/* SPI register offsets */
typedef struct spi_dr_dev {
	u32 config;	/* 0x00 */
	u32 isr;	/* 0x04 */
	u32 ier;	/* 0x08 */
	u32 idr;	/* 0x0C */
	u32 imr;	/* 0x10 */
	u32 en;		/* 0x14 */
	u32 delay;	/* 0x18 */
	u32 txdata;	/* 0x1C */
	u32 rxdata;	/* 0x20 */
	u32 slaveidle;	/* 0x24 */
	u32 txfifo;	/* 0x28 */
	u32 rxfifo;	/* 0x2C */
	u32 reserved0[51];
	u32 modid;	/* 0xFC */
}__attribute__((__packed__)) spi_controller_regs;

/* I2C Controller (IIC)
 *
 * */
#define I2C_0_BASE_ADDR		0xE0004000
#define I2C_1_BASE_ADDR		0xE0005000
#define I2C_CLK_FREQ_HZ 	111111115

/* I2C Controller (IIC) */
typedef struct i2c_dr_dev {
	u32 control;			/* 0x00 */
	u32 status;				/* 0x04 */
	u32 address;			/* 0x08 */
	u32 data;				/* 0x0C */
	u32 interrupt_status;	/* 0x10 */
	u32 transfer_size;		/* 0x14 */
	u32 slave_mon_pause;	/* 0x18 */
	u32 timeout;			/* 0x1C */
	u32 interrupt_mask;		/* 0x20 */
	u32 interrupt_enable;	/* 0x24 */
	u32 interrupt_disable;	/* 0x28 */
}__attribute__((__packed__)) i2c_controller_regs;



/* SD Controller (sdio)
 *
 * */
#define SD_BASE_ADDR		0xE0100000



/* USB Controller (usb)
 *
 * */
#define USB0_BASE_ADDR		0xE0002000
#define USB1_BASE_ADDR		0xE0003000
#define USB0_IRQ_ID			53
#define USB1_IRQ_ID			76

/* USB DR device mode registers (Little Endian) */
typedef struct usb_dr_device {
	u32 id;				/* 0x00	*/
	u32 hwgeneral;		/* 0x04	*/
	u32 hwhost;			/* 0x08	*/
	u32 hwdevice;		/* 0x0c	*/
	u32 hwtxbuf;		/* 0x10	*/
	u32 hwrxbuf;		/* 0x14 */

	u8 res0[104];	/* */

	u32 gpTimeR0ld;  	/* 0x80	*/
	u32 gpTimeR0ctrl;	/* 0x84	*/
	u32 gpTimeR1ld;		/* 0x88	*/
	u32 gpTimeR1Ctrl;	/* 0x8c	*/

	u32 sbuscfg;		/* 0x90	*/
	u8 res1[108];

	/* Capability register */
	u16 caplength;			/* 0x100 	Capability Register Length */
	u16 hciversion;			/* 0x102	Host Controller Interface Version */
	u32 hcsparams;			/* 0x104	Host Controller Structual Parameters */
	u32 hccparams;			/* 0x108	Host Controller Capability Parameters */
	u8 res2[20];
	u32 dciversion;			/* 0x120  	Device Controller Interface Version */
	u32 dccparams;			/* 0x124	Device Controller Capability Parameters */
	u8 res3[24];
	/* Operation register */
	u32 usbcmd;				/* 0x140 	USB Command Register */
	u32 usbsts;				/* 0x144 	USB Status Register */
	u32 usbintr;			/* 0x148 	USB Interrupt Enable Register */
	u32 frindex;			/* 0x14c 	Frame Index Register */
	u8 res4[4];
	u32 deviceaddr;			/* 0x154 	Device Address */
	u32 endpointlistaddr;	/* 0x158 	Endpoint List Address Register */
	u32 ttctrl;				/* 0x15c 	*/
	u32 burstsize;			/* 0x160	Master Interface Data Burst Size Register */
	u32 txfilltuning;		/* 0x164 	*/
	u32 txttfilltuning;		/* 0x168	Transmit FIFO Tuning Controls Register */
	u32 ic_usb;				/* 0x16c	*/
	u32 ulpi_viewport;		/* 0x170	*/
	u8 res5[4];
	u32 endptnak;   		/* 0x178	*/
	u32 endptnaken;			/* 0x17c	*/
	u32 configflag;			/* 0x180	Configure Flag Register */
	u32 portsc1;			/* 0x184	Port 1 Status and Control Register */
	u8 res7[28];
	u32 otgsc;				/* 0x1a4    On-The-Go Status and Control */
	u32 usbmode;			/* 0x1a8	USB Mode Register */
	u32 endptsetupstat;		/* 0x1ac 	Endpoint Setup Status Register */
	u32 endpointprime;		/* 0x1b0	Endpoint Initialization Register */
	u32 endptflush;			/* 0x1b4	Endpoint Flush Register */
	u32 endptstatus;		/* 0x1b8	Endpoint Status Register */
	u32 endptcomplete;		/* 0x1bc	Endpoint Complete Register */
	u32 endptctrl[12];		/* 0x1c0	Endpoint Control Registers */
}__attribute__((__packed__)) usb_controller_regs;



/* Gigabit Ethernet Controller (GEM)
 *
 * */
#define GEM0_BASE_ADDR		0xE000B000
#define GEM1_BASE_ADDR		0xE000C000
#define GEM0_IRQ_ID			54
#define GEM1_IRQ_ID			77

/* Device registers */
typedef struct gem_regs {
	u32 net_ctrl; 	/* 0x00 Network Control reg */
	u32 net_cfg; 	/* 0x04 Network Config reg */
	u32 net_status;	/* 0x08 Network Status reg */
	u32 reserved0;
	u32 dma_cfg;	/* 0x10 DMA Control reg */
	u32 tx_status; 	/* 0x14 TX Status reg */
	u32 rx_qbar; 	/* 0x18 RX Q Base address reg */
	u32 tx_qbar; 	/* 0x1c TX Q Base address reg */
	u32 rx_status; 	/* 0x20 RX Status reg */
	u32 intr_status;/* 0x24 Interrupt Status reg */
	u32 intr_en;	/* 0x28 Interrupt Enable reg */
	u32 intr_dis;	/* 0x2c Interrupt Disable reg */
	u32 intr_mask;	/* 0x30 Interrupt Mask Status reg */
	u32 phy_maint; 	/* 0x34 Phy Maintaince reg */
	u32 reserved1[18];
	u32 hash_bot;	/* 0x80 Hash Low address reg */
	u32 hash_top;	/* 0x84 Hash High address reg */
	u32 spec_addr1_bot;		 	/* 0x88  */
	u32 spec_addr1_top; 		/* 0x8c  */
	u32 spec_addr2_bot;			/* 0x90  */
	u32 spec_addr2_top; 		/* 0x94  */
	u32 spec_addr3_bot; 		/* 0x98  */
	u32 spec_addr3_top; 		/* 0x9c  */
	u32 spec_addr4_bot; 		/* 0xa0  */
	u32 spec_addr4_top; 		/* 0xa4  */
	u32 type_id_match1; 		/* 0xa8  */
	u32 type_id_match2; 		/* 0xac  */
	u32 type_id_match3; 		/* 0xb0  */
	u32 type_id_match4; 		/* 0xb4  */
	u32 wake_on_lan;			/* 0xb8 */
	u32 ipg_stretch;			/* 0xbc */
	u32 stacked_vlan;			/* 0xc0 */
	u32 tx_pfc_pause;			/* 0xc4 */
	u32 spec_addr1_mask_bot;	/* 0xc8 */
	u32 spec_addr1_mask_top;	/* 0xcc */
	u32 reserved2[11];
	u32 module_id;				/* 0xfc */
	u32 octets_tx_bot;			/* 0x100 */
	u32 octets_tx_top;			/* 0x104 */
	u32 frames_tx;				/* 0x108 */
	u32 broadcaset_frames_tx;	/* 0x10c */
	u32 multi_frames_tx;		/* 0x110 */
	u32 pause_frames_tx;		/* 0x114 */
	u32 frames_64b_tx;			/* 0x118 */
	u32 frames_65to127b_tx;		/* 0x11c */
	u32 frames_128to255b_tx;	/* 0x120 */
	u32 frames_256to511b_tx;	/* 0x124 */
	u32 frames_512to1023b_tx;	/* 0x128 */
	u32 frames_1024to1518b_tx;	/* 0x12c */
	u32 reverved3;
	u32 tx_under_runs;			/* 0x134 */
	u32 single_collisn_frames;	/* 0x138 */
	u32 multi_collisn_frames;	/* 0x13c */
	u32 excessive_collisns;		/* 0x140 */
	u32 late_collisns;			/* 0x144 */
	u32 deferred_tx_frames;		/* 0x148 */
	u32 carrier_sense_errs;		/* 0x14c */
	u32 octets_rx_bot;			/* 0x150 */
	u32 octets_rx_top;			/* 0x154 */
	u32 frames_rx;				/* 0x158 */
	u32 bdcase_frames_rx;		/* 0x15c */
	u32 multi_frames_rx;		/* 0x160 */
	u32 pause_rx;				/* 0x164 */
	u32 frames_64b_rx;			/* 0x168 */
	u32 frames_65to127b_rx;		/* 0x16c */
	u32 frames_128to255b_rx;	/* 0x170 */
	u32 frames_256to511b_rx;	/* 0x174 */
	u32 frames_512to1023b_rx;	/* 0x178 */
	u32 frames_1024to1518b_rx;	/* 0x17c */
	u32 reverved4;
	u32 undersz_rx;				/* 0x184 */
	u32 oversz_rx;				/* 0x188 */
	u32 jab_rx;					/* 0x18c */
	u32 fcs_errors;				/* 0x190 */
	u32 length_field_errors;	/* 0x194 */
	u32 rx_symbol_errors;		/* 0x198 */
	u32 align_errors;			/* 0x19c */
	u32 rx_resource_errors;		/* 0x1a0 */
	u32 rx_overrun_errors;		/* 0x1a4 */
	u32 ip_hdr_csum_errors;		/* 0x1a8 */
	u32 tcp_csum_errors;		/* 0x1ac */
	u32 udp_csum_errors;		/* 0x1b0 */
	u32 reverved5[5];
	u32 timer_strobe_s;			/* 0x1c8 */
	u32 timer_strobe_ns;		/* 0x1cc */
	u32 timer_s;				/* 0x1d0 */
	u32 timer_ns;				/* 0x1d4 */
	u32 timer_adjust;			/* 0x1d8 */
	u32 timer_incr;				/* 0x1dc */
	u32 ptp_tx_s;				/* 0x1e0 */
	u32 ptp_tx_ns;				/* 0x1e4 */
	u32 ptp_rx_s;				/* 0x1e8 */
	u32 ptp_rx_ns;				/* 0x1ec */
	u32 ptp_peer_tx_s;			/* 0x1f0 */
	u32 ptp_peer_tx_ns;			/* 0x1f4 */
	u32 ptp_peer_rx_s;			/* 0x1f8 */
	u32 ptp_peer_rx_ns;			/* 0x1fc */
	u32 reverved6[33];
	u32 design_cfg2;			/* 0x284 */
	u32 design_cfg3;			/* 0x288 */
	u32 design_cfg4;			/* 0x28c */
	u32 design_cfg5;			/* 0x290 */
}__attribute__((__packed__)) gem_controller_regs;

/* Definitions for peripheral REAL_TIME_0 */
#define XPAR_REAL_TIME_0_S00_AXI_BASEADDR 0x43C00000
#define XPAR_REAL_TIME_0_S00_AXI_HIGHADDR 0x43C00FFF
#define AXI_PANELLEN_BASEADDR		XPAR_REAL_TIME_0_S00_AXI_BASEADDR

/* Definitions for peripheral ADT_PARLLEL_BUS_0 */
#define XPAR_ADT_PARLLEL_BUS_0_S01_AXI_BASEADDR 0x83C00000
#define XPAR_ADT_PARLLEL_BUS_0_S01_AXI_HIGHADDR 0x83C3FFFF



/* CAN Controller (can)
 *
 * */
#define CAN_0_BASE_ADDR		0xE0008000
#define CAN_1_BASE_ADDR		0xE0009000
#define CAN_0_IRQ_ID		60
#define CAN_1_IRQ_ID		83

/* Device registers */
typedef struct can_regs {
	u32 SRR; 			/* 0x00 Software Reset Register */
	u32 MSR; 			/* 0x04 Mode Select Register */
	u32 BRPR;			/* 0x08 Baud Rate Prescaler Register */
	u32 BTR;			/* 0x0c Bit Timing Register */
	u32 ECR;			/* 0x10 Error Counter Register */
	u32 ESR; 			/* 0x14 Error Status Register */
	u32 SR; 			/* 0x18 Status Register */
	u32 ISR; 			/* 0x1c Interrupt Status Register */
	u32 IER; 			/* 0x20 Interrupt Enable Register */
	u32 ICR;			/* 0x24 Interrupt Clear Register */
	u32 TCR;			/* 0x28 Timestamp Control Register */
	u32 WIR;			/* 0x2c Watermark Interrupt Register */
	u32 TXFIFO_ID;		/* 0x30 transmit message fifo message identifier */
	u32 TXFIFO_DLC; 	/* 0x34 transmit message fifo data length code */
	u32 TXFIFO_DATA1;	/* 0x38 transmit message fifo data word 1 */
	u32 TXFIFO_DATA2;	/* 0x3c transmit message fifo data word 2 */
	u32 TXHPB_ID;		/* 0x40 transmit high priority buffer message identifier */
	u32 TXHPB_DLC;		/* 0x44 transmit high priority buffer data length code */
	u32 TXHPB_DATA1; 	/* 0x48 transmit high priority buffer data word 1 */
	u32 TXHPB_DATA2;	/* 0x4c transmit high priority buffer data word 2 */
	u32 RXFIFO_ID; 		/* 0x50 receive message fifo message identifier */
	u32 RXFIFO_DLC; 	/* 0x54 receive message fifo data length code */
	u32 RXFIFO_DATA1; 	/* 0x58 receive message fifo data word 1 */
	u32 RXFIFO_DATA2; 	/* 0x5c receive message fifo data word 2 */
	u32 AFR; 			/* 0x60 Acceptance Filter Register */
	u32 AFMR1; 			/* 0x64 Acceptance Filter Mask Register 1 */
	u32 AFIR1; 			/* 0x68 Acceptance Filter ID Register 1 */
	u32 AFMR2; 			/* 0x6c Acceptance Filter Mask Register 2 */
	u32 AFIR2; 			/* 0x70 Acceptance Filter ID Register 2 */
	u32 AFMR3;			/* 0x74 Acceptance Filter Mask Register  3 */
	u32 AFIR3;			/* 0x78 Acceptance Filter ID Register 3 */
	u32 AFMR4;			/* 0x7c Acceptance Filter Mask Register 4 */
	u32 AFIR4;			/* 0x80 Acceptance Filter ID Register 4 */
}__attribute__((__packed__)) can_controller_regs;


/* @@ 对应 FPGA 读写地址
 * */
#define	AXI_FRAM_BASEADDR 			XPAR_ADT_PARLLEL_BUS_0_S01_AXI_BASEADDR
#define	AXI_MOTION_BASEADDR 		(XPAR_ADT_PARLLEL_BUS_0_S01_AXI_BASEADDR + 0x10000)
#define	AXI_IO_BASEADDR 		    (XPAR_ADT_PARLLEL_BUS_0_S01_AXI_BASEADDR + 0x20000)
#define	AXI_REALTIME_BASEADDR 		XPAR_REAL_TIME_0_S00_AXI_BASEADDR
#define AXI_PANELLEN_BASEADDR		XPAR_REAL_TIME_0_S00_AXI_BASEADDR
#define	AXI_XADC_BASEADDR 			XPAR_XADC_WIZ_0_BASEADDR
#define	AXI_VDMA_BASEADDR			XPAR_AXI_VDMA_0_BASEADDR

#define AXI_TOUACH_BESEADDR  		0x43C90000

/* 6001按键LED灯读、写地址 */
#define AXI_KEYBOARD_ADDR			(0x43c20000)	//6001手持盒读按键寄存器
#define KEY_OUT_OFFSET				(0) 	//0*4
#define EM_KEY_OFFSET				(4)		//1*4	//bit0		[正常为1,按下为0]
#define SAFE_KEY_OFFSET				(8)		//2*4	//bit0  	[正常为1,按下为0]
#define SWTICH_KEY_OFFSET			(12)	//3*4	//bit0 bit1	[正常为1,按下为0]
#define LED_CTRL_OFFSET				(16)	//4*4


/*@@ 显示部分相关地址
 *
 * */
/* Definitions for peripheral AXI_VDMA_DDR_0 */
#define XPAR_AXI_VDMA_DDR_0_S_AXI_GP_BASEADDR 0x83C40000
#define XPAR_AXI_VDMA_DDR_0_S_AXI_GP_HIGHADDR 0x83C40FFF

/* Definitions for peripheral ADT_CLK_GEN_0 */
#define XPAR_ADT_CLK_GEN_0_BASEADDR 0x83000000
#define XPAR_ADT_CLK_GEN_0_HIGHADDR 0x83007FFF

#define XPAR_AXI_VDMA_0_BASEADDR 0x43000000
#define XPAR_AXI_VDMA_0_HIGHADDR 0x4300FFFF

//#define DDR_BASEADDR 				0
#define CFV_BASEADDR 				XPAR_AXI_VDMA_DDR_0_S_AXI_GP_BASEADDR
#define	VDMA_BASEADDR 				XPAR_AXI_VDMA_0_BASEADDR
//#define VIDEO_BASEADDR 				(DDR_BASEADDR + 0x12000000)


//触摸控制 xpt2046_ip
//寄存器偏移地址
#define     POSITION     0         //触摸位置，[7:0]若为0x81,有接触；若为0x80，无接触。  [19:8]为y坐标。  [31:20]为x坐标。
#define     PRESSURE     1           //压力检测，[11:0] 是z2值，[23:12]是z1值 R值 1.1 以下表示ok. R X面板为1.
#define     TEMP         2          //温度电压测量，[11:0]是temp1值，[23:12]是temp0值
#define     TIMEOUT1     3           //滤波时间 ，默认50ms  单位 10ns
#define     TIMEOUT2     4           //滤波时间 ，默认200ms 单位 10ns
#define     TOUCH_FLAG   5         //触摸标志，1表示有触摸，这时数据有效；0表示没有触摸
#define     COMMTEST     6


/* @@ 寄存器偏移地址
 *  基地址:AXI_REALTIME_BASEADDR
 *
 * */
#define DEVTYPE			0   //R     0xa501      读取设别系列号
#define	COMTEST			4	//R/W	0x 0		读写测试使用
#define	VERDATA      	8	//R		0x102		版本信息
#define	REAL_TIME1		12	//R/W	0x 0		实时时钟值，具体见库
#define	REAL_TIME2		16	//R/W	0x 0		实时时钟值，具体见库
#define	TIMER_COUNT1	20	//R		0x 0		计数器1计数值
#define	DEVIDE_DATA1	24	//R/W	0x 64		计数器1分频值
#define	SET_COUNT1		28	//W		0x 0		设置计计数器1数初值
#define	TIMER_COUNT2	32	//R		0x 0		计数器2计数值
#define	DEVIDE_DATA2	36	//R/W	0x 64		计数器2分频值
#define	SET_COUNT2		40	//W		0x 0		设置计数器2计数初值
#define	LED_CTL			44  //R/W	0x 0		指示灯控制
#define	KBDO_KBSTS		48  //R		0x 0		键盘检测值bit0-bit14 KBDO ; bit15 KBSTS
#define GPIO_OUT		52	//R/W   0xffffffff


/* GPIO_OUT DESC:
 * bit0-复位网络
 * bit1-复位从USB
 * bit2-复位主USB
 * bit3-复位FPGA
 * bit4-复位CPLD
 */
#define GPIO_IN			56	//R		0x0




#endif
