#include "int.h"
#include "s5pv210.h"

#define 	GPH2CON 				(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT					(*(volatile unsigned long *) 0xE0200C44)
#define 	GPH2_0_EINT16 			(0xf<<(0*4))
#define 	GPH2_1_EINT17 			(0xf<<(1*4))
#define 	GPH2_2_EINT18 			(0xf<<(2*4))
#define 	GPH2_3_EINT19 			(0xf<<(3*4))
#define		EXT_INT_0_CON  			( *((volatile unsigned long *)0xE0200E00) )
#define		EXT_INT_1_CON  			( *((volatile unsigned long *)0xE0200E04) )
#define		EXT_INT_2_CON  			( *((volatile unsigned long *)0xE0200E08) )
#define		EXT_INT_3_CON  			( *((volatile unsigned long *)0xE0200E0C) )
#define		EXT_INT_0_MASK   		( *((volatile unsigned long *)0xE0200F00) )
#define		EXT_INT_1_MASK   		( *((volatile unsigned long *)0xE0200F04) )
#define		EXT_INT_2_MASK   		( *((volatile unsigned long *)0xE0200F08) )
#define		EXT_INT_3_MASK   		( *((volatile unsigned long *)0xE0200F0C) )
#define		EXT_INT_0_PEND   		( *((volatile unsigned long *)0xE0200F40) )
#define		EXT_INT_1_PEND   		( *((volatile unsigned long *)0xE0200F44) )
#define		EXT_INT_2_PEND   		( *((volatile unsigned long *)0xE0200F48) )
#define		EXT_INT_3_PEND   		( *((volatile unsigned long *)0xE0200F4C) )

#define RGB(r,g,b)   				(unsigned int)( (r << 16) + (g << 8) + b )
extern int gRow;

void isr_key(void)
{
	static int counter;
	Uart_Printf("isr_key\n");

	Lcd_Printf(gRow/10*3+25*3, 0, RGB( 0xFF,0xFF,0xFF), RGB( 0x00,0x00,0x00),0,"Key interrupt times:%d\n",++counter);
	
	// clear VIC0ADDR
	intc_clearvectaddr();
	
	// clear pending bit	
	EXT_INT_2_PEND |= 1<<0;					
}

// 初始化按键,仅在测试中断时使用
int Button_Init(void)
{
	int c = 0;

	// 外部中断相关的设置
	// 1111 = EXT_INT[16] 
	GPH2CON |= 0xF;							
	// 010 = Falling edge triggered
	EXT_INT_2_CON |= 1<<1;			
	// unmasked
	EXT_INT_2_MASK &= ~(1<<0);

	// 使能中断EINT16_31
	intc_enable(NUM_EINT16_31);
	
	// 设置中断EINT16_31的处理函数
	pISR_EINT16_31 = (unsigned long)isr_key;
	
}
