void Target_Init()
{
	// 初始化时钟
	Clock_Init();	
	// 初始化串口
    Uart_Init();
    // 测试串口
    Uart_Printf("Hello UCOS2\n");																
}
