#ifndef __DAT_DATATYPE_H__
#define __DAT_DATATYPE_H__



#if 0
#ifndef   s8
#define   s8   signed char
#endif

#ifndef   s16
#define   s16  signed short
#endif

#ifndef   s32
#define   s32  signed long
#endif

#ifndef   u8
#define   u8   unsigned char
#endif

#ifndef   u16
#define   u16  unsigned short
#endif

#ifndef   u32
#define   u32  unsigned long
#endif

#ifndef   u64
#define   u64  unsigned long long
#endif

#ifndef   U8
#define   U8   unsigned char
#endif

#ifndef  U16
#define unsigned short U16
#endif
//typedef unsigned long  U32;

#ifndef  U32
#define U32 unsigned long
#endif

#ifndef  U64
#define U64 unsigned long long
#endif

#ifndef  uchar
#define uchar unsigned char
#endif

#ifndef  ushort
#define	ushort unsigned short
#endif

#ifndef  ulong
#define ulong unsigned long
#endif

#ifndef  uint
#define	uint unsigned int
#endif

#ifndef  lbaint_t
#define lbaint_t ulong
#endif

#ifndef  BYTE
#define BYTE unsigned char
#endif

#ifndef  BOOLEAN
#define BOOLEAN unsigned char
#endif

#ifndef  INT8U
#define INT8U unsigned char
#endif

#ifndef  INT8S
#define  INT8S signed char
#endif

#ifndef  INT16U
#define INT16U unsigned short
#endif

#ifndef  INT16S
#define  INT16S signed short
#endif

#ifndef  INT32U
#define INT32U unsigned int
#endif

#ifndef  INT32S
#define  INT32S signed int
#endif

#ifndef  FP32
#define FP32 float          		                     /* Single precision floating point                    */
#endif

#ifndef   FP64
#define  FP64 double         	                    /* Double precision floating point                    */
#endif

#if !defined(LONG)
#define long LONG;
#endif

#if !defined(ULONG)
#define unsigned long ULONG;
#endif

#ifndef	BOOL
	#define	BOOL	BOOLEAN
#endif

#ifndef bool
#define bool BOOLEAN
#endif

#ifndef NULL
#define NULL 		((void*)0)
#endif

#ifndef TRUE
#  define TRUE		1U
#endif

#ifndef FALSE
#  define FALSE		0U
#endif

#ifndef true
#define true		1u
#endif

#ifndef false
#define false		0u
#endif

#endif

#if 1
typedef   signed char  s8;
typedef   signed short s16;
typedef   signed long  s32;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;
typedef unsigned long long u64;

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;

typedef unsigned long long U64;

typedef unsigned char	uchar;
typedef	unsigned short	ushort;
typedef unsigned long	ulong;
typedef	unsigned int	uint;
typedef ulong lbaint_t;


typedef unsigned char  	 BOOLEAN;
typedef unsigned char    INT8U;
typedef   signed char    INT8S;
typedef unsigned short   INT16U;
typedef   signed short   INT16S;
typedef unsigned int     INT32U;
typedef   signed int     INT32S;

//typedef INT8S    BYTE;
//#ifndef BYTE
//#define BYTE  INT8S
//#endif

typedef float          		FP32;                     /* Single precision floating point                    */
typedef double         		FP64;                     /* Double precision floating point                    */


#if !defined(LONG)
typedef long LONG;
#endif

#if !defined(ULONG)
typedef unsigned long ULONG;
#endif

#ifndef	BOOL
	#define	BOOL	BOOLEAN
#endif

#ifndef bool
#define bool BOOLEAN
#endif

#ifndef NULL
#define NULL 		((void*)0)
#endif

#ifndef TRUE
#  define TRUE		1U
#endif

#ifndef FALSE
#  define FALSE		0U
#endif

#ifndef true
#define true		1u
#endif

#ifndef false
#define false		0u
#endif

#endif

#endif

