#ifndef	__SERIAL__H___
#define	__SERIAL__H___


#define COM1	0
#define	COM2	1
#define	COM3	2

#define	DATABITS7	0X02
#define DATABITS8	0X03
/*
#define	STOPBIT1	0X00
#define	STOPBIT2	0X04

#define	NOPARITY	0x00
#define	ODDPARITY	0X08
#define	EVENPARITY	0X18

#define	BAUD1200	0X80
#define	BAUD2400	0XA0
#define	BAUD4800	0XC0
#define	BAUD9600	0XE0
*/
#define Com1	  0x3f0
#define Com2	  0x2f0
#define Com3	  0x3e0
#define Com4	  0x2e0

#define Txbuf	  0x08	   /* tx buffer */
#define Rxbuf	  0x08	   /* rx buffer */
#define Dll		  0x08	   /* baud lsb */
#define Dlh	  	  0x09	   /* baud msb */
#define Ier	  	  0x09	   /* int enable reg */
#define Fcr	  	  0x0a	   /* FIFO control register */
#define Lcr	      0x0b	   /* line control reg */
#define Dfr	  	  0x0b	   /* Data format  reg */
#define Mcr	  	  0x0c	   /* modem control reg */
#define Lsr	  	  0x0d	   /* line status reg */

/*
#define	COM1BASE	0X3F8
#define	COM2BASE	0X2F8

#define	TXDATA1		COM1BASE
#define	RXR1		COM1BASE
#define	IER1		(COM1BASE+1)
#define	IIR1		(COM1BASE+2)
#define	LCR1		(COM1BASE+3)
#define	MCR1		(COM1BASE+4)
#define	LSR1		(COM1BASE+5)

#define	TXDATA2		COM2BASE
#define	RXR2		COM2BASE
#define	IER2		(COM2BASE+1)
#define	IIR2		(COM2BASE+2)
#define	LCR2		(COM2BASE+3)
#define	MCR2		(COM2BASE+4)
#define	LSR2		(COM2BASE+5)
*/

#define	RX_ID		0X04
#define	RX_MASK		0X07
#define	MC_INT		0X08

#define	IMR			0X21
#define	ICR			0X20

#define	EOI			0X20

#define	IRQ5		0XDF
#define	IRQ4		0XEF
#define	IRQ3		0XF7

#endif
