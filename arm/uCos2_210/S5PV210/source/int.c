#include "int.h"
#include "s5pv210.h"

// 初始化中断控制器  
void intc_init(void)
{
	// 禁止所有中断
	VIC0INTENCLEAR = 0xffffffff;
	VIC1INTENCLEAR = 0xffffffff;
	VIC2INTENCLEAR = 0xffffffff;
	VIC3INTENCLEAR = 0xffffffff;

	// 选择中断类型为IRQ
	VIC0INTSELECT = 0x0;
	VIC1INTSELECT = 0x0;
	VIC2INTSELECT = 0x0;
	VIC3INTSELECT = 0x0;

	// 清VICxADDR
	intc_clearvectaddr();

}

// 保存需要处理的中断的中断处理函数的地址  
void intc_setvectaddr(unsigned long intnum, void (*handler)(void))
{
	//VIC0
	if(intnum<32)			
	{
		*( (volatile unsigned long *)(VIC0VECTADDR + 4*intnum) ) = (unsigned)handler;
	}
	//VIC1
	else if(intnum<64) 		
	{
		*( (volatile unsigned long *)(VIC1VECTADDR + 4*(intnum-32)) ) = (unsigned)handler;
	}
	//VIC2
	else if(intnum<96) 			
	{
		*( (volatile unsigned long *)(VIC2VECTADDR + 4*(intnum-64)) ) = (unsigned)handler;
	}
	//VIC3
	else	
	{
		*( (volatile unsigned long *)(VIC3VECTADDR + 4*(intnum-96)) ) = (unsigned)handler;
	}
	return;
}

// 清除需要处理的中断的中断处理函数的地址  
void intc_clearvectaddr(void)
{
	// VICxADDR:当前正在处理的中断的中断处理函数的地址
	VIC0ADDR = 0;
	VIC1ADDR = 0;
	VIC2ADDR = 0;
	VIC3ADDR = 0;
}

// 使能中断  
void intc_enable(unsigned long intnum)
{
	unsigned long temp;
	if(intnum<32)
	{
		temp = VIC0INTENABLE;
		temp |= (1<<intnum);
		VIC0INTENABLE = temp;
	}
	else if(intnum<64)
	{
		temp = VIC1INTENABLE;
		temp |= (1<<(intnum-32));
		VIC1INTENABLE = temp;
	}
	else if(intnum<96)
	{
		temp = VIC2INTENABLE;
		temp |= (1<<(intnum-64));
		VIC2INTENABLE = temp;
	}
	else if(intnum<NUM_ALL)
	{
		temp = VIC3INTENABLE;
		temp |= (1<<(intnum-96));
		VIC3INTENABLE = temp;
	}
	// NUM_ALL : enable all interrupt
	else 
	{
		VIC0INTENABLE = 0xFFFFFFFF;
		VIC1INTENABLE = 0xFFFFFFFF;
		VIC2INTENABLE = 0xFFFFFFFF;
		VIC3INTENABLE = 0xFFFFFFFF;
	}

}

// 禁止中断  
void intc_disable(unsigned long intnum)
{
	unsigned long temp;

	if(intnum<32)
	{
		temp = VIC0INTENCLEAR;
		temp |= (1<<intnum);
		VIC0INTENCLEAR = temp;
	}
	else if(intnum<64)
	{
		temp = VIC1INTENCLEAR;
		temp |= (1<<(intnum-32));
		VIC1INTENCLEAR = temp;
	}
	else if(intnum<96)
	{
		temp = VIC2INTENCLEAR;
		temp |= (1<<(intnum-64));
		VIC2INTENCLEAR = temp;
	}
	else if(intnum<NUM_ALL)
	{
		temp = VIC3INTENCLEAR;
		temp |= (1<<(intnum-96));
		VIC3INTENCLEAR = temp;
	}
	// NUM_ALL : disable all interrupt
	else 
	{
		VIC0INTENCLEAR = 0xFFFFFFFF;
		VIC1INTENCLEAR = 0xFFFFFFFF;
		VIC2INTENCLEAR = 0xFFFFFFFF;
		VIC3INTENCLEAR = 0xFFFFFFFF;
	}

	return;
}


// 读中断状态  
unsigned long intc_getvicirqstatus()
{
	if(VIC0IRQSTATUS)
		return	VIC0IRQSTATUS;
	else if(VIC1IRQSTATUS)
		return 	VIC1IRQSTATUS+32;
	else if(VIC2IRQSTATUS)
		return 	VIC2IRQSTATUS+64;
	else if(VIC3IRQSTATUS)
		return 	VIC3IRQSTATUS+96;
	else
		return -1;
}



unsigned int get_x(unsigned long x)
{
	int i;
	for(i=0;i<32;i++)
	{	
		if(x&(1<<i))
			return i;
	}
	return 0;
}


unsigned int get_int_vect()
{
	unsigned int ret = -1;

	if(VIC0IRQSTATUS)
	{
		ret = get_x(VIC0IRQSTATUS);
	}
	else if(VIC1IRQSTATUS)
	{
		ret = get_x(VIC1IRQSTATUS)+32;
	}
	else if(VIC1IRQSTATUS)
	{
		ret = get_x(VIC2IRQSTATUS)+64;
	}
		else if(VIC1IRQSTATUS)
	{
		ret = get_x(VIC3IRQSTATUS)+96;
	}


	return ret;
}