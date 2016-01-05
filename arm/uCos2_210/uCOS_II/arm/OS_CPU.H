#ifndef OS_CPU_H
#define OS_CPU_H

// OS_CPU.H 包括了用#defines 定义的与处理器相关的常量，宏和类型定义

/*
*********************************************************************************************************
*							    数据类型 
*                            (与编译器相关) 
*********************************************************************************************************
*/
/*
因为不同的微处理器有不同的字长，所以 μC/OS-Ⅱ的移植包括了一系列的类型定义以
确保其可移植性。尤其是，μC/OS-Ⅱ代码从不使用 C 的 short,int 和 long 等数据类型，因
为它们是与编译器相关的，不可移植。
*/
typedef 	unsigned char  BOOLEAN;                 			/* 布尔变量                                 */
typedef 	unsigned char  INT8U;                  			/* 无符号8位整型变量                        */
typedef 	signed   char  INT8S;                  			/* 有符号8位整型变量                        */
typedef 	unsigned short INT16U;                  			/* 无符号16位整型变量                       */
typedef 	signed   short INT16S;                  			/* 有符号16位整型变量                       */
typedef 	unsigned int   INT32U;                  			/* 无符号32位整型变量                       */
typedef 	signed   int   INT32S;                  			/* 有符号32位整型变量                       */
typedef 	float          FP32;                    			/* 单精度浮点数（32位长度）                 */
typedef 	double         FP64;                    			/* 双精度浮点数（64位长度）                 */

typedef 	INT32U         OS_STK;                  			/* 栈的宽度为32bit                          */
typedef 	INT32U   		OS_CPU_SR;                		/* 用于关闭中断时保存CPSR的值			    */

#define 	BYTE           INT8S								/* 以下是兼容UC/OS V1.XX的数据类型，在uC/OS-II没有使用  */
#define 	UBYTE          INT8U
#define 	WORD           INT16S
#define 	UWORD          INT16U
#define 	LONG           INT32S
#define 	ULONG          INT32U


/* 
************************************************************************* 
*                             与处理器相关的代码 
************************************************************************* 
*/ 
#define  	OS_CRITICAL_METHOD    3
#if      	OS_CRITICAL_METHOD == 3
#define  	OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR())  		/* Disable interrupts                        */
#define  	OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr))  		/* Restore  interrupts                       */
OS_CPU_SR 	OSCPUSaveSR(void);							// 函数声明   			
void      	OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif

// 栈增长的方向:ARM的栈从高到低
#define  	OS_STK_GROWTH        1                       		/* Stack grows from HIGH to LOW memory on ARM    */
#define  	OS_TASK_SW()         OSCtxSw()

#endif
