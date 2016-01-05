#ifndef _TYPEDEF1_H_
#define _TYPEDEF1_H_

typedef unsigned char				uchar;
typedef volatile unsigned long		vu_long;
typedef volatile unsigned short	 	vu_short;
typedef volatile unsigned char		vu_char;
typedef volatile unsigned long		U32;
#define __REG(x)	(*(vu_long *)(x))

#endif
