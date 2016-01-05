#include "s5pv210.h"

#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// 延时函数
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

void Led_Blink()				//LED 闪烁
{
	GPJ2CON = 0x00001111;		// 配置引脚
	while(1)					// 闪烁
	{
		GPJ2DAT = 0;			// LED on
		delay(0x1000000);
		GPJ2DAT = 0xf;			// LED off
		delay(0x1000000);
	}
}

void Led_On()				
{
	GPJ2CON = 0x00001111;		
	GPJ2DAT = 0;			
}

void Led_OFF()				
{
	GPJ2CON = 0x00001111;		
	GPJ2DAT = 0xf;			
}

void Printf_Regs(int r0,int r1,int r2,int r3)				
{
	int *addr;
	Uart_Printf("R0-R4:%x %x %x %x\n",r0,r1,r2,r3);
	addr = (int *)(r1+r0*4);
	Uart_Printf("addr:%x,[addr]:%x\n",addr,*addr);

	Uart_Printf("&pISR_TIMER0:%x pISR_TIMER0:%x\n",&pISR_TIMER0,pISR_TIMER0);
}

