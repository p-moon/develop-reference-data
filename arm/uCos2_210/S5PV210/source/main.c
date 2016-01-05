#include "ucos_ii.h"
#include "app_cfg.h"
#include "lcd.h"
#include "s5pv210.h"
#include "Printf.h"

// 设置栈
OS_STK  MainTaskStk[MainTaskStkLengh];
OS_STK	Task0Stk[Task0StkLengh];       															
OS_STK	Task1Stk[Task1StkLengh];      															
OS_STK	Task2Stk[Task2StkLengh];       														

// s5pv210相关
#define RGB(r,g,b)   (unsigned int)( (r << 16) + (g << 8) + b )
int rYear, rMonth,rDay,rDayOfWeek,rHour,rMinute,rSecond;
int gLCD_Type;
extern unsigned char *gImage_bmp;
extern int gRow,gCol;

int main(void)
{
	// 初始化单板，主要就是初始化时钟和串口
	Target_Init();
	// 初始化内核，主要就是初始化所有的变量和数据结构
    OSInit();
    // 初始化tick time
    OSTimeSet(0);	
    // 将MainTask由睡眠态变成就绪态
    OSTaskCreate(MainTask,(void *)0, &MainTaskStk[MainTaskStkLengh - 1], MainTaskPrio);
    // 开始多任务调度，OSStart之前用户至少创建一个任务。
    // 此时有三个任务:空闲任务idle、统计任务stat、maintask(优先级最高)
    OSStart();																					
    for(;;)
    	;
}

void MainTask(void *pdata) 
{
#if OS_CRITICAL_METHOD == 3                                										
    OS_CPU_SR  cpu_sr;
#endif
	// 关闭中断
	OS_ENTER_CRITICAL();
	// 初始化中断
	ISRInit();																					
	// 初始化定时器,应该在开始多任务之后再初始化时钟节拍中断
    Timer_Init();		
    // 打开中断
	OS_EXIT_CRITICAL();
	
	// 初始化按键
    Button_Init();	
    // 初始化一线触摸
    TS_1wire_Init(10000);																			
	// 初始化LCD
	while(!gLCD_Type);																			
	Lcd_Init(gLCD_Type);
	Lcd_Draw_Bmp(gImage_bmp, gLCD_Type);

	// 初始化print 任务
    OSPrintfInit();			
    // 初始化统计任务。
    // 如果要使用统计任务，则在用户的第一个任务里需调用OSStatInit函数
    OSStatInit();																				
    // 创建用户应用程序任务
    OSTaskCreate(Task0,(void *)0, &Task0Stk[Task0StkLengh - 1], Task0Prio);			
    OSTaskCreate(Task1,(void *)0, &Task1Stk[Task1StkLengh - 1], Task1Prio);
    OSTaskCreate(Task2,(void *)0, &Task2Stk[Task2StkLengh - 1], Task2Prio);
 
    while(1)
    {
	    OSPrintf("Enter Maintask\n");	
        OSTimeDly(OS_TICKS_PER_SEC);
    }
}

void Task0(void *pdata)
{
	Lcd_Printf(gRow/10*3+25*3, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Key interrupt times:0\n");

    while(1)
    {
	    OSPrintf("Enter Task0\n");
        One_Wire_Timer_Proc();																	 // 一线触摸通讯

		// 由于tick中断里只会选出所有就绪任务中优先级最高的任务而不会进行调度，所以任意任务都必须自己主动调用OSTimeDly()，
		// OSTimeDly()里会调用任务调度函数OS_Sched()。
        OSTimeDly(OS_TICKS_PER_SEC/5);
    }
}

void Task1(void *pdata)
{
	int TestCnt=0;
	int Version;
			
	while (1)
	{
		TestCnt++;
	    OSPrintf("Enter Task1\n");	
	     
	    //led闪烁 
	    if(TestCnt%2)
	    	Led_On();
	    else
	    	Led_OFF();
		Lcd_Printf(gRow/10*3+25*2,0,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"LED blink times:%d",TestCnt/2);	      	
	    OSTimeDly(OS_TICKS_PER_SEC/5);
	}
}

void Task2(void *pdata)
{
	unsigned int i;
	int offset = 0;
	char   s[80];

	if(gLCD_Type == 0x3 || gLCD_Type == 0x4)
		offset = 2;
	else if(gLCD_Type == 0xe || gLCD_Type == 0x1)
		offset = -2;
	else if(gLCD_Type == 0x13)
		offset = -2;
	else 
		offset = 0;
		

	// RTC初始化
	Rtc_Init();
	while(1)
	{
	    OSPrintf("Enter Task2\n");	
		i++;
		if(i>99)
			i=0;

		rYear	 = (2000 + rBCDYEAR);	
		rMonth	 	= FROM_BCD(rBCDMON & 0x1f);
		rDay		= FROM_BCD(rBCDDAY & 0x03f);
		rDayOfWeek 	= rBCDDATE-1;
		rHour	 	= FROM_BCD(rBCDHOUR & 0x3f);
		rMinute 	= FROM_BCD(rBCDMIN & 0x7f);
		rSecond 	= FROM_BCD(rBCDSEC & 0x7f);

	   	//OSTimeDly( 5 );
	   	Uart_Printf("rtc:%d:%d:%d\n",rHour,rMinute,rSecond);
		// RTC
	   	Lcd_Printf(gRow/10*0, gCol/10*0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0, "%02d:%02d:%02d", rHour, rMinute, rSecond);
	   	Lcd_Printf(gRow/10*0, gCol/10*5-5*8, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0, "%4d-%02d-%02d", rYear, rMonth, rDay);
	   	Lcd_Printf(gRow/10*0, gCol/10*10-6*8, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0, "Day%d", rDayOfWeek);
		Lcd_Printf(gRow/10*3, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Task number:%-2d",OSTaskCtr);
		Lcd_Printf(gRow/10*3+25, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"CPU use rate:%%%-2d",OSCPUUsage);
	   	Lcd_Printf(gRow/10*10-16-offset, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"FriendARM uC/OS2 Mutiltask running:%-2d", i);
	   	OSTimeDly(OS_TICKS_PER_SEC/5);

	  }
}


