#ifndef __DEF_H__
#define __DEF_H__

#include "integer.h"
#include "adt_datatype.h"

typedef unsigned char  BOOLEAN;
#ifndef	BOOL
	#define	BOOL		   BOOLEAN
#endif

//typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
//typedef char  	       INT8S;                    /* Signed    8 bit quantity                           */
//typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
//typedef short          INT16S;                   /* Signed   16 bit quantity                           */
//typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
//typedef signed int     INT32S;                   /* Signed   32 bit quantity                           */
//typedef float          FP32;                     /* Single precision floating point                    */
//typedef double         FP64;                     /* Double precision floating point                    */

//typedef unsigned int   OS_STK;                   /* Each stack entry is 16-bit wide                    */
//typedef unsigned short OS_CPU_SR;                /* Define size of CPU status register (PSW = 16 bits) */

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16U                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

//typedef unsigned int	DWORD;
//#define	UINT	U32
#define	LPSTR	U8 *

//typedef unsigned char BOOL;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
//typedef unsigned long DWORD;
typedef DWORD *PDWORD;
//typedef unsigned char   BYTE;
//typedef unsigned short  WORD;

//typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;

typedef BYTE *LPBYTE;

typedef void *PVOID;


#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif

//#ifndef	U32
//#define U32 unsigned long
//#endif

#define U16 unsigned short
#define S32 long
#define S16 short int
#define U8  unsigned char
#define	S8  char

#ifndef	TRUE
	#define TRUE 	1   
	#define FALSE 	0
#endif


#ifndef	U32
	#define U32 unsigned long
#endif

#ifndef	U16
	#define U16 unsigned short
#endif

#ifndef	S32
	#define S32 long
#endif

#ifndef	S16
	#define S16 short int
#endif

#ifndef	U8
	#define U8  unsigned char
#endif


/*	

#ifndef	S8
	#define	S8  char
#endif

#ifndef	DWORD
	#define	DWORD	INT32U
#endif

#ifndef	LPSTR
	#define	LPSTR	U8 *
#endif

*/
#endif /*__DEF_H__*/

