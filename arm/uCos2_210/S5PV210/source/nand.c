
#include <string.h>

#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "Nand.h"

//suppport boot params


#define	EnNandFlash()	(rNFCONT |= 1)
#define	DsNandFlash()	(rNFCONT &= ~1)
#define	NFChipEn()		(rNFCONT &= ~(1<<1))
#define	NFChipDs()		(rNFCONT |= (1<<1))
#define	InitEcc()		(rNFCONT |= (1<<4))
#define	MEccUnlock()	(rNFCONT &= ~(1<<5))
#define	MEccLock()		(rNFCONT |= (1<<5))
#define	SEccUnlock()	(rNFCONT &= ~(1<<6))
#define	SEccLock()		(rNFCONT |= (1<<6))

#define	WrNFDat8(dat)	(rNFDATA8 = (dat))
#define	WrNFDat32(dat)	(rNFDATA = (dat))
#define	RdNFDat8()		(rNFDATA8)	//byte access
#define	RdNFDat32()		(rNFDATA)	//word access

#define	WrNFCmd(cmd)	(rNFCMD = (cmd))
#define	WrNFAddr(addr)	(rNFADDR = (addr))
#define	WrNFDat(dat)	WrNFDat8(dat)
#define	RdNFDat()		RdNFDat8()	//for 8 bit nand flash, use byte access

#define	RdNFMEcc()		(rNFMECC0)	//for 8 bit nand flash, only use NFMECC0
#define	RdNFSEcc()		(rNFSECC)	//for 8 bit nand flash, only use low 16 bits

#define	RdNFStat()		(rNFSTAT)
#define	NFIsBusy()		(!(rNFSTAT&1))
#define	NFIsReady()		(rNFSTAT&1)

//#define	WIAT_BUSY_HARD	1
//#define	ER_BAD_BLK_TEST
//#define	WR_BAD_BLK_TEST

#define	READCMD0	0
#define	READCMD1	1
#define	READCMD2	0x50
#define	ERASECMD0	0x60
#define	ERASECMD1	0xd0
#define	PROGCMD0	0x80
#define	PROGCMD1	0x10
#define	QUERYCMD	0x70
#define	RdIDCMD		0x90

static U16 NandAddr;

// HCLK=100Mhz
#define TACLS		1//7	// 1-clk(0ns) 
#define TWRPH0		4//7	// 3-clk(25ns)
#define TWRPH1		0//7	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns

static void InitNandCfg(void)
{
	// for S3C2440

	rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);	
	// TACLS		[14:12]	CLE&ALE duration = HCLK*TACLS.
	// TWRPH0		[10:8]	TWRPH0 duration = HCLK*(TWRPH0+1)
	// TWRPH1		[6:4]	TWRPH1 duration = HCLK*(TWRPH1+1)
	// AdvFlash(R)	[3]		Advanced NAND, 0:256/512, 1:1024/2048
	// PageSize(R)	[2]		NAND memory page size
	//						when [3]==0, 0:256, 1:512 bytes/page.
	//						when [3]==1, 0:1024, 1:2048 bytes/page.
	// AddrCycle(R)	[1]		NAND flash addr size
	//						when [3]==0, 0:3-addr, 1:4-addr.
	//						when [3]==1, 0:4-addr, 1:5-addr.
	// BusWidth(R/W) [0]	NAND bus width. 0:8-bit, 1:16-bit.
	
	rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
	// Lock-tight	[13]	0:Disable lock, 1:Enable lock.
	// Soft Lock	[12]	0:Disable lock, 1:Enable lock.
	// EnablillegalAcINT[10]	Illegal access interupt control. 0:Disable, 1:Enable
	// EnbRnBINT	[9]		RnB interrupt. 0:Disable, 1:Enable
	// RnB_TrandMode[8]		RnB transition detection config. 0:Low to High, 1:High to Low
	// SpareECCLock	[6]		0:Unlock, 1:Lock
	// MainECCLock	[5]		0:Unlock, 1:Lock
	// InitECC(W)	[4]		1:Init ECC decoder/encoder.
	// Reg_nCE		[1]		0:nFCE=0, 1:nFCE=1.
	// NANDC Enable	[0]		operating mode. 0:Disable, 1:Enable.

//	rNFSTAT = 0;
    
//    Nand_Reset();
}

#ifdef	WIAT_BUSY_HARD
#define	WaitNFBusy()	while(NFIsBusy())
#else
static U32 WaitNFBusy(void)	// R/B 未接好?
{
	U8 stat;
	
	WrNFCmd(QUERYCMD);
	do {
		stat = RdNFDat();
		//Uart_Printf("%x\n", stat);
	}while(!(stat&0x40));
	WrNFCmd(READCMD0);
	return stat&1;
}
#endif



U32 ReadChipId(void)
{
	U32 id;
	
	NFChipEn();	
	WrNFCmd(RdIDCMD);
	WrNFAddr(0);
	while(NFIsBusy());	
	id  = RdNFDat()<<8;
	id |= RdNFDat();		
	NFChipDs();		
	
	return id;
}

U16 ReadStatus(void)
{
	U16 stat;
	
	NFChipEn();	
	WrNFCmd(QUERYCMD);		
	stat = RdNFDat();	
	NFChipDs();
	
	return stat;
}

static U32 EraseBlock(U32 addr)
{
	U8 stat;

	addr &= ~0x1f;
		
	NFChipEn();	
	WrNFCmd(ERASECMD0);		
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WrNFCmd(ERASECMD1);		
	stat = WaitNFBusy();
	NFChipDs();
	
#ifdef	ER_BAD_BLK_TEST
	if(!((addr+0xe0)&0xff)) stat = 1;	//just for test bad block
#endif
	
	Uart_Printf("Erase block 0x%x %s\n", addr, stat?"fail":"ok");
		
	return stat;
}

//addr = page address
void ReadPage(U32 addr, U8 *buf)
{
	U16 i;
	
	NFChipEn();
	WrNFCmd(READCMD0);
	WrNFAddr(0);
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	InitEcc();
	WaitNFBusy();
	for(i=0; i<512; i++)
		buf[i] = RdNFDat();
	NFChipDs();
}

static U32 WritePage(U32 addr, U8 *buf)
{
	U32 i, mecc;
	U8 stat, tmp[7];
	
	
	NFChipEn();
	WrNFCmd(PROGCMD0);
	WrNFAddr(0);
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	InitEcc();	//reset mecc and secc
	MEccUnlock();
	for(i=0; i<512; i++)
		WrNFDat(buf[i]);
	MEccLock();
	
	mecc = RdNFMEcc();
		
	tmp[0] = mecc&0xff;
    tmp[1] = (mecc>>8)&0xff;
    tmp[2] = (mecc>>16)&0xff;
    tmp[3] = (mecc>>24)&0xff;
    tmp[5] = 0xff;	//mark good block
    
    SEccUnlock();
	WrNFDat(tmp[0]);
	WrNFDat(tmp[1]);
	WrNFDat(tmp[2]);
	WrNFDat(tmp[3]);
	SEccLock();
	WrNFDat(tmp[4]);
	WrNFDat(tmp[5]);
    	
	WrNFCmd(PROGCMD1);
	stat = WaitNFBusy();
	NFChipDs();
	
#ifdef	WR_BAD_BLK_TEST
	if((addr&0xff)==0x17) stat = 1;	//just for test bad block
#endif
		
	if(stat)
		Uart_Printf("Write nand flash 0x%x fail\n", addr);
	else {	
		U8 RdDat[512];
		
		ReadPage(addr, RdDat);		
		for(i=0; i<512; i++)
			if(RdDat[i]!=buf[i]) {
				Uart_Printf("Check data at page 0x%x, offset 0x%x fail\n", addr, i);
				stat = 1;
				break;
			}
	}
		
	return stat;	
}

static void MarkBadBlk(U32 addr)
{
	addr &= ~0x1f;
	
	NFChipEn();
	
	WrNFCmd(READCMD2);	//point to area c
	
	WrNFCmd(PROGCMD0);
	WrNFAddr(4);		//mark offset 4,5,6,7
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WrNFDat(0);			//mark with 0
	WrNFDat(0);
	WrNFDat(0);			//mark with 0
	WrNFDat(0);
	WrNFCmd(PROGCMD1);
	WaitNFBusy();		//needn't check return status
	
	WrNFCmd(READCMD0);	//point to area a
		
	NFChipDs();
}

int CheckBadBlk(U32 addr)
{
	U8 dat;
	
	addr &= ~0x1f;
	
	NFChipEn();
	
	WrNFCmd(READCMD2);	//point to area c
	WrNFAddr(5);		//mark offset 4,5,6,7
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WaitNFBusy();
	dat = RdNFDat();
	
	WrNFCmd(READCMD0);	//point to area a
	
	NFChipDs();

	return (dat!=0xff);
}


/************************************************************/
void InitNandFlash(int info)
{	
	U32 i;
	
	InitNandCfg();
	i = ReadChipId();
	if(info)
		Uart_Printf("Read chip id = %x\n", i);	
	if((i==0x9873)||(i==0xec75))	
		NandAddr = 0;
	else if(i==0xec76)
		NandAddr = 1;
	else {	
		//Uart_Printf("Chip id error!!!\n");
		return;
	}
	
}




