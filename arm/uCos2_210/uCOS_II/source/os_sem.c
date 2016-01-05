/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                          SEMAPHORE MANAGEMENT
*
*                          (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File    : OS_SEM.C
* By      : Jean J. Labrosse
* Version : V2.83
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include <ucos_ii.h>
#endif

#if OS_SEM_EN > 0
/*
*********************************************************************************************************
*                                           ACCEPT SEMAPHORE
*
* Description: This function checks the semaphore to see if a resource is available or, if an event
*              occurred.  Unlike OSSemPend(), OSSemAccept() does not suspend the calling task if the
*              resource is not available or the event did not occur.
*
* Arguments  : pevent     is a pointer to the event control block
*
* Returns    : >  0       if the resource is available or the event did not occur the semaphore is
*                         decremented to obtain the resource.
*              == 0       if the resource is not available or the event did not occur or,
*                         if 'pevent' is a NULL pointer or,
*                         if you didn't pass a pointer to a semaphore
*********************************************************************************************************
*/
// 无等待地请求一个信号量。
// 当一个任务请求一个信号量时，如果该信号量暂时无效，
// 也可以让该任务简单地返回，而不是进入睡眠等待状态
#if OS_SEM_ACCEPT_EN > 0
INT16U  OSSemAccept (OS_EVENT *pevent)
{
    INT16U     cnt;
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) 					  /* Validate 'pevent'                             */
    {                    
        return (0);
    }
#endif
	// 事件类型是信号量?
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) 	  /* Validate event block type                     */
    {   
        return (0);
    }

    OS_ENTER_CRITICAL();
	// 获得计数器
    cnt = pevent->OSEventCnt;
    // 可用资源大于0?
    if (cnt > 0) 									  /* See if resource is available                  */
    {                                    
        pevent->OSEventCnt--;                         /* Yes, decrement semaphore and notify caller    */
    }
    OS_EXIT_CRITICAL();
    return (cnt);                                     /* Return semaphore count                        */
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                           CREATE A SEMAPHORE
*
* Description: This function creates a semaphore.
*
* Arguments  : cnt           is the initial value for the semaphore.  If the value is 0, no resource is
*                            available (or no event has occurred).  You initialize the semaphore to a
*                            non-zero value to specify how many resources are available (e.g. if you have
*                            10 resources, you would initialize the semaphore to 10).
*
* Returns    : != (void *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                            created semaphore
*              == (void *)0  if no event control blocks were available
*********************************************************************************************************
*/
// 创建一个信号量
OS_EVENT  *OSSemCreate (INT16U cnt)
{
    OS_EVENT  *pevent;
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

    if (OSIntNesting > 0) 								   /* See if called from ISR ...               */
    {                                
        return ((OS_EVENT *)0);                            /* ... can't CREATE from an ISR             */
    }
    
    OS_ENTER_CRITICAL();
    // 获得ECB
    pevent = OSEventFreeList;                              /* Get next free event control block        */
    if (OSEventFreeList != (OS_EVENT *)0) 				   /* See if pool of free ECB pool was empty   */
    {                
    	// 更新OSEventFreeList
        OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
    }
    OS_EXIT_CRITICAL();
    
    if (pevent != (OS_EVENT *)0) 						   /* Get an event control block               */
    {               
    	// 设置事件类型
        pevent->OSEventType    = OS_EVENT_TYPE_SEM;
        // 设置计数器
        pevent->OSEventCnt     = cnt;                      /* Set semaphore value                      */
        pevent->OSEventPtr     = (void *)0;                /* Unlink from ECB free list                */
#if OS_EVENT_NAME_SIZE > 1
        pevent->OSEventName[0] = '?';                      /* Unknown name                             */
        pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
		// 初始化一个空的等待任务列表
        OS_EventWaitListInit(pevent);                      /* Initialize to 'nobody waiting' on sem.   */
    }

    // 返回ECB
    return (pevent);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         DELETE A SEMAPHORE
*
* Description: This function deletes a semaphore and readies all tasks pending on the semaphore.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete semaphore ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the semaphore even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the semaphore was deleted
*                            OS_ERR_DEL_ISR          If you attempted to delete the semaphore from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the semaphore
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a semaphore
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the semaphore was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the semaphore MUST check the return code of OSSemPend().
*              2) OSSemAccept() callers will not know that the intended semaphore has been deleted unless
*                 they check 'pevent' to see that it's a NULL pointer.
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the semaphore.
*              4) Because ALL tasks pending on the semaphore will be readied, you MUST be careful in
*                 applications where the semaphore is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the semaphore.
*********************************************************************************************************
*/
// 删除信号量
#if OS_SEM_DEL_EN > 0
OS_EVENT  *OSSemDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
    BOOLEAN    tasks_waiting;
    OS_EVENT  *pevent_return;
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif



#if OS_ARG_CHK_EN > 0
    if (err == (INT8U *)0) {                               /* Validate 'err'                           */
        return (pevent);
    }
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return (pevent);
    }
#endif
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {        /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);
    }
    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0) {                         /* See if any tasks waiting on semaphore    */
        tasks_waiting = OS_TRUE;                           /* Yes                                      */
    } else {
        tasks_waiting = OS_FALSE;                          /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete semaphore only if no task waiting */
             if (tasks_waiting == OS_FALSE) {
#if OS_EVENT_NAME_SIZE > 1
                 pevent->OSEventName[0] = '?';             /* Unknown name                             */
                 pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
                 pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
                 pevent->OSEventPtr     = OSEventFreeList; /* Return Event Control Block to free list  */
                 pevent->OSEventCnt     = 0;
                 OSEventFreeList        = pevent;          /* Get next free event control block        */
                 OS_EXIT_CRITICAL();
                 *err                   = OS_NO_ERR;
                 pevent_return          = (OS_EVENT *)0;   /* Semaphore has been deleted               */
             } else {
                 OS_EXIT_CRITICAL();
                 *err                   = OS_ERR_TASK_WAITING;
                 pevent_return          = pevent;
             }
             break;

        case OS_DEL_ALWAYS:                                /* Always delete the semaphore              */
             while (pevent->OSEventGrp != 0) {             /* Ready ALL tasks waiting for semaphore    */
             	  // 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
                 (void)OS_EventTaskRdy(pevent, (void *)0, OS_STAT_SEM);
             }
#if OS_EVENT_NAME_SIZE > 1
             pevent->OSEventName[0] = '?';                 /* Unknown name                             */
             pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
             pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
             pevent->OSEventPtr     = OSEventFreeList;     /* Return Event Control Block to free list  */
             pevent->OSEventCnt     = 0;
             OSEventFreeList        = pevent;              /* Get next free event control block        */
             OS_EXIT_CRITICAL();
             if (tasks_waiting == OS_TRUE) {               /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err                   = OS_NO_ERR;
             pevent_return          = (OS_EVENT *)0;       /* Semaphore has been deleted               */
             break;

        default:
             OS_EXIT_CRITICAL();
             *err                   = OS_ERR_INVALID_OPT;
             pevent_return          = pevent;
             break;
    }
    return (pevent_return);
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                           PEND ON SEMAPHORE
*
* Description: This function waits for a semaphore.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for the resource up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever at the specified
*                            semaphore or, until the resource becomes available (or the event occurs).
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task owns the resource
*                                                or, the event you are waiting for occurred.
*                            OS_TIMEOUT          The semaphore was not received within the specified
*                                                timeout.
*                            OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a semaphore.
*                            OS_ERR_PEND_ISR     If you called this function from an ISR and the result
*                                                would lead to a suspension.
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer.
*
* Returns    : none
*********************************************************************************************************
*/
// 要求获得一个信号量
void  OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (err == (INT8U *)0)							  /* Validate 'err'                                */
    {                          
        return;
    }
    if (pevent == (OS_EVENT *)0)  					  /* Validate 'pevent'                             */
    {                   
        *err = OS_ERR_PEVENT_NULL;
        return;
    }
#endif
	// 确定事件类型是信号量
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) 	  /* Validate event block type                     */
    {   
        *err = OS_ERR_EVENT_TYPE;
        return;
    }
    // 中断处理里不允许挂起
    if (OSIntNesting > 0) 							  /* See if called from ISR ...                    */
    {                           
        *err = OS_ERR_PEND_ISR;                       /* ... can't PEND from an ISR                    */
        return;
    }
    
    // 调度禁止时不允许挂起
    if (OSLockNesting > 0) 							  /* See if called with scheduler locked ...       */
    {                          
        *err = OS_ERR_PEND_LOCKED;                    /* ... can't PEND when locked                    */
        return;
    }
    
    OS_ENTER_CRITICAL();
    // 如果信号量可用
    if (pevent->OSEventCnt > 0) 					  /* If sem. is positive, resource available ...   */
    {        
    	// 获得信号量，计数器减一，即可用的资源减少一份
        pevent->OSEventCnt--;                         /* ... decrement semaphore only if positive.     */
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        // 返回
        return;
    }

    // 否则，等待信号量 	                          /* Otherwise, must wait until event occurs       */
    // 更新任务状态标志
    OSTCBCur->OSTCBStat   |= OS_STAT_SEM;             /* Resource not available, pend on semaphore     */
    // 延时等待最大时间未到
    OSTCBCur->OSTCBPendTO  = OS_FALSE;
	// 设置延时等待最大时间
    OSTCBCur->OSTCBDly     = timeout;                 /* Store pend timeout in TCB                     */
    // 将任务添加到事件控制块的等待任务表
    OS_EventTaskWait(pevent);                         /* Suspend task until event or timeout occurs    */
    OS_EXIT_CRITICAL();

	// 找出优先级最高的任务并进行任务切换
    OS_Sched();                                       /* Find next highest priority task ready         */

    // 当信号量有效或者等待时间到后，
    // 并且调用 OSSemPend()函数的任务再一次成为最高优先级任务,
    // 就会回到这里继续执行
    OS_ENTER_CRITICAL();
    // 是否延时等待最大时间到了?
    if (OSTCBCur->OSTCBPendTO == OS_TRUE) 			  /* See if we timedout                            */
    {           
	    // 由于等待超时而将任务置为就绪态。
        OS_EventTO(pevent);
        OS_EXIT_CRITICAL();
        *err = OS_TIMEOUT;                            /* Indicate that didn't get event within TO      */
        return;
    }

    // 释放TCB里的ECB
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         POST TO A SEMAPHORE
*
* Description: This function signals a semaphore
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
* Returns    : OS_NO_ERR           The call was successful and the semaphore was signaled.
*              OS_SEM_OVF          If the semaphore count exceeded its limit.  In other words, you have
*                                  signalled the semaphore more often than you waited on it with either
*                                  OSSemAccept() or OSSemPend().
*              OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a semaphore
*              OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer.
*********************************************************************************************************
*/
// 释放信号量
INT8U  OSSemPost (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) 						   /* Validate 'pevent'                        */
    {                         
        return (OS_ERR_PEVENT_NULL);
    }
#endif
	// 检查事件类型是信号量
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) 		   /* Validate event block type                */
    {        
        return (OS_ERR_EVENT_TYPE);
    }
    
    OS_ENTER_CRITICAL();
    // 有任务正在等待该信号量?
    if (pevent->OSEventGrp != 0) 						   /* See if any task waiting for semaphore*/
    {                     
    	// 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
        (void)OS_EventTaskRdy(pevent, (void *)0, OS_STAT_SEM); /* Ready HPT waiting on event           */
        OS_EXIT_CRITICAL();
        // 找出优先级最高的任务并进行任务切换
        OS_Sched();                                            /* Find HPT ready to run                */
        return (OS_NO_ERR);
    }

    // 没有任务在等待信号量
    if (pevent->OSEventCnt < 65535u) 				  /* Make sure semaphore will not overflow         */
    {             
    	// 计算器加一
        pevent->OSEventCnt++;                         /* Increment semaphore count to register event   */
        OS_EXIT_CRITICAL();
        return (OS_NO_ERR);
    }

    OS_EXIT_CRITICAL();                               /* Semaphore value has reached its maximum       */
    return (OS_SEM_OVF);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          QUERY A SEMAPHORE
*
* Description: This function obtains information about a semaphore
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore
*
*              p_sem_data    is a pointer to a structure that will contain information about the
*                            semaphore.
*
* Returns    : OS_NO_ERR           The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE   If you are attempting to obtain data from a non semaphore.
*              OS_ERR_PEVENT_NULL  If 'pevent'     is a NULL pointer.
*              OS_ERR_PDATA_NULL   If 'p_sem_data' is a NULL pointer
*********************************************************************************************************
*/
// 查询信号量的当前状态
#if OS_SEM_QUERY_EN > 0
INT8U  OSSemQuery (OS_EVENT *pevent, OS_SEM_DATA *p_sem_data)
{
#if OS_LOWEST_PRIO <= 63
    INT8U     *psrc;
    INT8U     *pdest;
#else
    INT16U    *psrc;
    INT16U    *pdest;
#endif
    INT8U      i;
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) 						   /* Validate 'pevent'                        */
    {                         
        return (OS_ERR_PEVENT_NULL);
    }
    if (p_sem_data == (OS_SEM_DATA *)0) 				   /* Validate 'p_sem_data'                    */
    {                  
        return (OS_ERR_PDATA_NULL);
    }
#endif
	// 事件类型是信号量?
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) 		   /* Validate event block type                */
    {        
        return (OS_ERR_EVENT_TYPE);
    }
    
    OS_ENTER_CRITICAL();
    // 设置信号量信息的结构体
    // 等待任务所在的组
    p_sem_data->OSEventGrp = pevent->OSEventGrp;           /* Copy message mailbox wait list           */
    // 等待任务表
    psrc                   = &pevent->OSEventTbl[0];
    pdest                  = &p_sem_data->OSEventTbl[0];
    for (i = 0; i < OS_EVENT_TBL_SIZE; i++) 
    {
        *pdest++ = *psrc++;
    }
    // 计数器
    p_sem_data->OSCnt = pevent->OSEventCnt;                /* Get semaphore count                      */
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_SEM_QUERY_EN                          */

/*$PAGE*/
/*
*********************************************************************************************************
*                                              SET SEMAPHORE
*
* Description: This function sets the semaphore count to the value specified as an argument.  Typically,
*              this value would be 0.
*
*              You would typically use this function when a semaphore is used as a signaling mechanism
*              and, you want to reset the count value.
*
* Arguments  : pevent     is a pointer to the event control block
*
*              cnt        is the new value for the semaphore count.  You would pass 0 to reset the
*                         semaphore count.
*
*              err        is a pointer to an error code returned by the function as follows:
*
*                            OS_NO_ERR            The call was successful and the semaphore value was set.
*                            OS_ERR_EVENT_TYPE    If you didn't pass a pointer to a semaphore.
*                            OS_ERR_PEVENT_NULL   If 'pevent' is a NULL pointer.
*                            OS_ERR_TASK_WAITING  If tasks are waiting on the semaphore.
*********************************************************************************************************
*/

#if OS_SEM_SET_EN > 0
void  OSSemSet (OS_EVENT *pevent, INT16U cnt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif



#if OS_ARG_CHK_EN > 0
    if (err == (INT8U *)0) {                          /* Validate 'err'                                */
        return;
    }
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        *err = OS_ERR_PEVENT_NULL;
        return;
    }
#endif
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {   /* Validate event block type                     */
        *err = OS_ERR_EVENT_TYPE;
        return;
    }
    OS_ENTER_CRITICAL();
    *err = OS_NO_ERR;
    if (pevent->OSEventCnt > 0) {                     /* See if semaphore already has a count          */
        pevent->OSEventCnt = cnt;                     /* Yes, set it to the new value specified.       */
    } else {                                          /* No                                            */
        if (pevent->OSEventGrp == 0) {                /*      See if task(s) waiting?                  */
            pevent->OSEventCnt = cnt;                 /*      No, OK to set the value                  */
        } else {
            *err               = OS_ERR_TASK_WAITING;
        }
    }
    OS_EXIT_CRITICAL();
}
#endif

#endif                                                /* OS_SEM_EN                                     */
