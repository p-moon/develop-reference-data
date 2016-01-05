#include "s5pv210.h"
#include "int.h"

void  Timer0_ISR(void)
{
	unsigned long *p;
	unsigned long addr;
	// 清除TIMER0中断标志
	TINT_CSTAT |= (1<<5);
	// 清VIC
	intc_clearvectaddr();
	// UCOS timer tick
	OSTimeTick();
}

int Timer_Init(void)
{
	unsigned long temp0;
	unsigned long utimer = 0;
	unsigned long uprescaler = 65;
	unsigned long udivider = 4;
	unsigned long utcntb = 625/2;
	unsigned long utcmpb = 0;

	// 定时器的输入时钟 = PCLK / ( {prescaler value + 1} ) / {divider value} = PCLK/(65+1)/16=62500hz
	//设置预分频系数为66
	temp0 = TCFG0;
	temp0 = (temp0 & (~(0xff00ff))) | ((uprescaler)<<0);
	TCFG0 = temp0;

	// 16分频
	temp0 = TCFG1;
	temp0 = (temp0 & (~(0xf<<4*utimer))& (~(1<<20))) |(udivider<<4*utimer);
	TCFG1 = temp0;

	// 1s = 62500hz,1秒tick 200下?
	TCNTB0 = utcntb;
	TCMPB0 = utcmpb;

	// 手动更新
	TCON |= 1<<1;
	// 清手动更新位
	TCON &= ~(1<<1);
	// 自动加载和启动timer0
	TCON |= (1<<0)|(1<<3);

	// 使能timer0中断
	temp0 = TINT_CSTAT;
	temp0 = (temp0 & (~(1<<utimer)))|(1<<(utimer));
	TINT_CSTAT = temp0;

	intc_enable(NUM_TIMER0);
	pISR_TIMER0 = (unsigned long)Timer0_ISR;
	return 0;
}

extern char __ENTRY[];
void ISRInit(void)
{
	int i;
	unsigned long *p;

	// 所有中断的中断处理函数都初始化为HandlerIRQ
	for(i=NUM_EINT0; i<=NUM_PENDN1; i++)	
	{
		intc_setvectaddr(i, (void (*)(void))(__ENTRY+0x18));
	}
	intc_init();	
 }
 