/************************************************ 
  NAME    : MMU.H
  DESC    :
  Revision: 02.28.2002 ver 0.0
 ************************************************/

//#include "2440slib.h"

#ifndef __MMU_H__
#define __MMU_H__
#ifdef __cplusplus
extern "C" {
#endif



#define DESC_SEC	(0x2|(1<<4))
#define CB		(3<<2)  //cache_on, write_back
#define CNB		(2<<2)  //cache_on, write_through 
#define NCB             (1<<2)  //cache_off,WR_BUF on
#define NCNB		(0<<2)  //cache_off,WR_BUF off
#define AP_RW		(3<<10) //supervisor=RW, user=RW
#define AP_RO		(2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT	(0x0)
#define DOMAIN_CHK	(0x1) 
#define DOMAIN_NOTCHK	(0x3) 
#define DOMAIN0		(0x0<<5)
#define DOMAIN1		(0x1<<5)

#define DOMAIN0_ATTR	(DOMAIN_CHK<<0) 
#define DOMAIN1_ATTR	(DOMAIN_FAULT<<2) 

#define RW_CB		(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB		(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB		(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT	(AP_RW|DOMAIN1|NCNB|DESC_SEC)

void MMU_Init(void);
void MMU_SetMTT(unsigned int vaddrStart,unsigned int vaddrEnd,unsigned int paddrStart,unsigned int attr);
void ChangeRomCacheStatus(int attr);

#define _MMUTT_STARTADDRESS 0x33ff8000


#ifdef __cplusplus
}
#endif

/*
#define ASM		1
#define C_LANG	2
#define DMA		3
#define TRANS_MODE C_LANG
*/
#endif /*__MMU_H__*/
