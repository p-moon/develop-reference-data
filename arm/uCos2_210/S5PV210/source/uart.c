#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "s5pv210.h"

void Uart_Init()
{
	// 配置引脚用于RX/TX功能
	GPA0CON = 0x22222222;
	GPA1CON = 0x2222;

	// 设置数据格式等
	// 使能fifo
	UFCON0 = 0x1;
	// 无流控
	UMCON0 = 0x0;
	// 数据位:8, 无校验, 停止位: 1
	ULCON0 = 0x3;
	// 时钟：PCLK，禁止中断，使能UART发送、接收
	UCON0  = 0x5;  				

	// 设置波特率
	UBRDIV0 = UART_UBRDIV_VAL;
	UDIVSLOT0 = UART_UDIVSLOT_VAL;
}


// 接收一个字符 
char serial_getc(void)
{
	// 如果RX FIFO空，等待
	while(!(UTRSTAT0 & (1<<0)));
	
	// 取数据
	return URXH0;                   	
}

void serial_putc(const char c)
{
	// 如果TX FIFO满，等待	
	while(!(UTRSTAT0 & (1<<2)));	
	
	// 写数据	
	UTXH0 = c;   

	if(c == '\n')
	{
		serial_putc('\r');
	}
}   


void Uart_SendString(char *s)
{
	while(*s)
	{
		serial_putc(*s++);
	}
}

void Uart_Printf(const char *fmt,...)
{
    va_list ap;
    char string[50];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
    Uart_SendString(string);
}
