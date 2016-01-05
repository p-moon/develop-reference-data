#include "s5pv210.h"

void Clock_Init()
{
	// 设置各种时钟开关，暂时不使用PLL
	CLK_SRC0  = 0x0;		
	// 设置锁定时间，使用默认值即可		
	APLL_LOCK = 0x0000FFFF;																			
	MPLL_LOCK = 0x0000FFFF;	
	// 设置分频
	CLK_DIV0  = 0x14131440;																		
	// FOUT= MDIV * FIN / (PDIV*2^(SDIV-1)) = 0x7d*24/(0x3*2^(1-1))=1000 MHz
	APLL_CON0 = APLL_VAL;																		
	// FOUT = MDIV*FIN/(PDIV*2^SDIV)=0x29b*24/(0xc*2^1)= 667 MHz
	MPLL_CON  = MPLL_VAL;																		
	// 设置各种时钟开关，使用PLL
	CLK_SRC0  = 0x10001111;																		
}
