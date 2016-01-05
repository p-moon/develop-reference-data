/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                        MESSAGE QUEUE MANAGEMENT
*
*                          (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File    : OS_Q.C
* By      : Jean J. Labrosse
* Version : V2.83
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include <ucos_ii.h>
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
/*
*********************************************************************************************************
*                                      ACCEPT MESSAGE FROM QUEUE
*
* Description: This function checks the queue to see if a message is available.  Unlike OSQPend(),
*              OSQAccept() does not suspend the calling task if a message is not available.
*
* Arguments  : pevent        is a pointer to the event control block
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task received a
*                                                message.
*                            OS_ERR_EVENT_TYPE   You didn't pass a pointer to a queue
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
*                            OS_Q_EMPTY          The queue did not contain any messages
*
* Returns    : != (void *)0  is the message in the queue if one is available.  The message is removed
*                            from the so the next time OSQAccept() is called, the queue will contain
*                            one less entry.
*              == (void *)0  if you received a NULL pointer message
*                            if the queue is empty or,
*                            if 'pevent' is a NULL pointer or,
*                            if you passed an invalid event type
*
* Note(s)    : As of V2.60, you can now pass NULL pointers through queues.  Because of this, the argument
*              'err' has been added to the API to tell you about the outcome of the call.
*********************************************************************************************************
*/
// 无等待地从一个消息队列中取得消息
#if OS_Q_ACCEPT_EN > 0
void  *OSQAccept (OS_EVENT *pevent, INT8U *err)
{
    void      *msg;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (err == (INT8U *)0)						 /* Validate 'err'                                     */
    {                     
        return ((void *)0);
    }
    if (pevent == (OS_EVENT *)0)				 /* Validate 'pevent'                                  */
    {               
        *err = OS_ERR_PEVENT_NULL;
        return ((void *)0);
    }
#endif
	// 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) /* Validate event block type                          */
    {
        *err = OS_ERR_EVENT_TYPE;
        return ((void *)0);
    }
    
    OS_ENTER_CRITICAL();
	// 获得队列控制块
    pq = (OS_Q *)pevent->OSEventPtr;             /* Point at queue control block                       */
	// 消息数不为0?
    if (pq->OSQEntries > 0) 					 /* See if any messages in the queue                   */
    { 
    	// 取出消息，更新OSQOut
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
		// 消息个数减一
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
		// 消息被取完了?
        if (pq->OSQOut == pq->OSQEnd)			 /* Wrap OUT pointer if we are at the end of the queue */
        {          
            pq->OSQOut = pq->OSQStart;
        }
        *err = OS_NO_ERR;
    }
    // 消息个数为0，则:
    else 
    {
        *err = OS_Q_EMPTY;
        msg  = (void *)0;                        /* Queue is empty                                     */
    }
    OS_EXIT_CRITICAL();
	// 返回消息或空
    return (msg);                                /* Return message received (or NULL)                  */
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        CREATE A MESSAGE QUEUE
*
* Description: This function creates a message queue if free event control blocks are available.
*
* Arguments  : start         is a pointer to the base address of the message queue storage area.  The
*                            storage area MUST be declared as an array of pointers to 'void' as follows
*
*                            void *MessageStorage[size]
*
*              size          is the number of elements in the storage area
*
* Returns    : != (OS_EVENT *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                                created queue
*              == (OS_EVENT *)0  if no event control blocks were available or an error was detected
*********************************************************************************************************
*/
// 创建一个消息队列
OS_EVENT  *OSQCreate (void **start, INT16U size)
{
    OS_EVENT  *pevent;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif

    if (OSIntNesting > 0) 						 /* See if called from ISR ...                         */
    {                      
        return ((OS_EVENT *)0);                  /* ... can't CREATE from an ISR                       */
    }

    OS_ENTER_CRITICAL();
    // 获得事件控制块
    pevent = OSEventFreeList;                    /* Get next free event control block                  */
    if (OSEventFreeList != (OS_EVENT *)0) 		 /* See if pool of free ECB pool was empty             */
    {      
    	// 更新OSEventFreeList
        OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
    }
    OS_EXIT_CRITICAL();

    if (pevent != (OS_EVENT *)0) 				 /* See if we have an event control block              */
    {               
        OS_ENTER_CRITICAL();
        // 获得队列控制块
        pq = OSQFreeList;                        /* Get a free queue control block                     */
        if (pq != (OS_Q *)0) 					 /* Were we able to get a queue control block ?        */
        {         
        	// 更新OSEventFreeList
            OSQFreeList            = OSQFreeList->OSQPtr; /* Yes, Adjust free list pointer to next free*/
            OS_EXIT_CRITICAL();
			// 初始化队列控制块，包括:
			// 队列头
            pq->OSQStart           = start;               /*      Initialize the queue                 */
			// 队列尾
            pq->OSQEnd             = &start[size];
			// 新消息插入的地方
            pq->OSQIn              = start;
			// 下一个被取出的消息
            pq->OSQOut             = start;
			// 队列长度
            pq->OSQSize            = size;
			// 消息个数
            pq->OSQEntries         = 0;

			// 初始化事件控制块，包括:
			// 事件类型为消息队列
            pevent->OSEventType    = OS_EVENT_TYPE_Q;
			// 计算器
            pevent->OSEventCnt     = 0;
            pevent->OSEventPtr     = pq;
#if OS_EVENT_NAME_SIZE > 1
            pevent->OSEventName[0] = '?';                  /* Unknown name                             */
            pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
			// 初始化一个空的等待任务列表
            OS_EventWaitListInit(pevent);                 /*      Initalize the wait list              */
        } 
        else 
        {
            pevent->OSEventPtr = (void *)OSEventFreeList; /* No,  Return event control block on error  */
            OSEventFreeList    = pevent;
            OS_EXIT_CRITICAL();
            pevent = (OS_EVENT *)0;
        }
    }
    return (pevent);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        DELETE A MESSAGE QUEUE
*
* Description: This function deletes a message queue and readies all tasks pending on the queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            queue.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete the queue ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the queue even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the queue was deleted
*                            OS_ERR_DEL_ISR          If you tried to delete the queue from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the queue
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a queue
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the queue was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the queue MUST check the return code of OSQPend().
*              2) OSQAccept() callers will not know that the intended queue has been deleted unless
*                 they check 'pevent' to see that it's a NULL pointer.
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the queue.
*              4) Because ALL tasks pending on the queue will be readied, you MUST be careful in
*                 applications where the queue is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the queue.
*              5) If the storage for the message queue was allocated dynamically (i.e. using a malloc()
*                 type call) then your application MUST release the memory storage by call the counterpart
*                 call of the dynamic allocation scheme used.  If the queue storage was created statically
*                 then, the storage can be reused.
*********************************************************************************************************
*/

#if OS_Q_DEL_EN > 0
OS_EVENT  *OSQDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
    BOOLEAN    tasks_waiting;
    OS_EVENT  *pevent_return;
    OS_Q      *pq;
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
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);
    }
    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0) {                         /* See if any tasks waiting on queue        */
        tasks_waiting = OS_TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = OS_FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete queue only if no task waiting     */
             if (tasks_waiting == OS_FALSE) {
#if OS_EVENT_NAME_SIZE > 1
                 pevent->OSEventName[0] = '?';             /* Unknown name                             */
                 pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
                 pq                     = (OS_Q *)pevent->OSEventPtr;  /* Return OS_Q to free list     */
                 pq->OSQPtr             = OSQFreeList;
                 OSQFreeList            = pq;
                 pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
                 pevent->OSEventPtr     = OSEventFreeList; /* Return Event Control Block to free list  */
                 pevent->OSEventCnt     = 0;
                 OSEventFreeList        = pevent;          /* Get next free event control block        */
                 OS_EXIT_CRITICAL();
                 *err                   = OS_NO_ERR;
                 pevent_return          = (OS_EVENT *)0;   /* Queue has been deleted                   */
             } else {
                 OS_EXIT_CRITICAL();
                 *err                   = OS_ERR_TASK_WAITING;
                 pevent_return          = pevent;
             }
             break;

        case OS_DEL_ALWAYS:                                /* Always delete the queue                  */
             while (pevent->OSEventGrp != 0)
             {             /* Ready ALL tasks waiting for queue        */
             	  // 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
                 (void)OS_EventTaskRdy(pevent, (void *)0, OS_STAT_Q);
             }
#if OS_EVENT_NAME_SIZE > 1
             pevent->OSEventName[0] = '?';                 /* Unknown name                             */
             pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
             pq                     = (OS_Q *)pevent->OSEventPtr;   /* Return OS_Q to free list        */
             pq->OSQPtr             = OSQFreeList;
             OSQFreeList            = pq;
             pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
             pevent->OSEventPtr     = OSEventFreeList;     /* Return Event Control Block to free list  */
             pevent->OSEventCnt     = 0;
             OSEventFreeList        = pevent;              /* Get next free event control block        */
             OS_EXIT_CRITICAL();
             if (tasks_waiting == OS_TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err                   = OS_NO_ERR;
             pevent_return          = (OS_EVENT *)0;       /* Queue has been deleted                   */
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
*                                             FLUSH QUEUE
*
* Description : This function is used to flush the contents of the message queue.
*
* Arguments   : none
*
* Returns     : OS_NO_ERR           upon success
*               OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a queue
*               OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
*
* WARNING     : You should use this function with great care because, when to flush the queue, you LOOSE
*               the references to what the queue entries are pointing to and thus, you could cause
*               'memory leaks'.  In other words, the data you are pointing to that's being referenced
*               by the queue entries should, most likely, need to be de-allocated (i.e. freed).
*********************************************************************************************************
*/
// 清空一个消息队列
#if OS_Q_FLUSH_EN > 0
INT8U  OSQFlush (OS_EVENT *pevent)
{
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0)					  /* Validate 'pevent'                             */
    {                    
        return (OS_ERR_PEVENT_NULL);
    }
    // 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) 	  /* Validate event block type                     */
    {     
        return (OS_ERR_EVENT_TYPE);
    }
#endif
    OS_ENTER_CRITICAL();
    // 获得队列控制块
    pq             = (OS_Q *)pevent->OSEventPtr;      /* Point to queue storage structure              */
	// 清空队列
    pq->OSQIn      = pq->OSQStart;
    pq->OSQOut     = pq->OSQStart;
    pq->OSQEntries = 0;
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                     PEND ON A QUEUE FOR A MESSAGE
*
* Description: This function waits for a message to be sent to a queue
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for a message to arrive at the queue up to the amount of time
*                            specified by this argument.  If you specify 0, however, your task will wait
*                            forever at the specified queue or, until a message arrives.
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task received a
*                                                message.
*                            OS_TIMEOUT          A message was not received within the specified timeout
*                            OS_ERR_EVENT_TYPE   You didn't pass a pointer to a queue
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
*                            OS_ERR_PEND_ISR     If you called this function from an ISR and the result
*                                                would lead to a suspension.
*                            OS_ERR_PEND_LOCKED  If you called this function with the scheduler is locked
*
* Returns    : != (void *)0  is a pointer to the message received
*              == (void *)0  if you received a NULL pointer message or,
*                            if no message was received or,
*                            if 'pevent' is a NULL pointer or,
*                            if you didn't pass a pointer to a queue.
*
* Note(s)    : As of V2.60, this function allows you to receive NULL pointer messages.
*********************************************************************************************************
*/
// 等待一个消息队列中的消息
void  *OSQPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
    void      *msg;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (err == (INT8U *)0)  					/* Validate 'err'                                     */
    {                    
        return ((void *)0);
    }
    if (pevent == (OS_EVENT *)0)				/* Validate 'pevent'                                  */
    {               
        *err = OS_ERR_PEVENT_NULL;
        return ((void *)0);
    }
   	// 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) /* Validate event block type                          */
    {
        *err = OS_ERR_EVENT_TYPE;
        return ((void *)0);
    }
#endif
	// 不允许在中断中挂起
    if (OSIntNesting > 0) 						 /* See if called from ISR ...                         */
    {                      
        *err = OS_ERR_PEND_ISR;                  /* ... can't PEND from an ISR                         */
        return ((void *)0);
    }
    // 锁定调度时不允许挂起 
    if (OSLockNesting > 0) 						 /* See if called with scheduler locked ...            */
    {                     
        *err = OS_ERR_PEND_LOCKED;               /* ... can't PEND when locked                         */
        return ((void *)0);
    }
    
    OS_ENTER_CRITICAL();
    // 获得队列控制块
    pq = (OS_Q *)pevent->OSEventPtr;             /* Point at queue control block                       */
	// 消息个数大于0?
    if (pq->OSQEntries > 0) 					 /* See if any messages in the queue                   */
    {                   
    	// 取出消息，并且OSQOut指向下一条消息
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
        // 消息数减一
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
        // 达到队列尾?
        if (pq->OSQOut == pq->OSQEnd)			 /* Wrap OUT pointer if we are at the end of the queue */
        {          
            pq->OSQOut = pq->OSQStart;
        }
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        // 返回消息
        return (msg);                            /* Return message received                            */
    }
    // 如果获取不到消息:
   	
	// 更新任务状态标志
    OSTCBCur->OSTCBStat   |= OS_STAT_Q;          /* Task will have to pend for a message to be posted  */
	// 延时等待最大时间未到
    OSTCBCur->OSTCBPendTO  = OS_FALSE;
	// 设置延时等待最大时间
    OSTCBCur->OSTCBDly     = timeout;            /* Load timeout into TCB                              */
	// 将任务添加到事件控制块的等待任务表
    OS_EventTaskWait(pevent);                    /* Suspend task until event or timeout occurs         */
    OS_EXIT_CRITICAL();

	// 找出优先级最高的任务并进行任务切换
    OS_Sched();                                  /* Find next highest priority task ready         */

    // 当消息队列有效或者等待时间到后，
    // 并且调用 OSQPend()函数的任务再一次成为最高优先级任务,
    // 就会回到这里继续执行
    OS_ENTER_CRITICAL();
    // 等待最大时间到了?
    if (OSTCBCur->OSTCBPendTO == OS_TRUE) 		 /* Was task readied because of a timeout?             */
    {         
	    // 由于等待超时而将任务置为就绪态。
        OS_EventTO(pevent);                      /* Yes                                                */
        OS_EXIT_CRITICAL();
        *err = OS_TIMEOUT;                       /*     Indicate a timeout occured                     */
        return ((void *)0);                      /*     No message received                            */
    }
    // 否则，获得消息
    msg                     = OSTCBCur->OSTCBMsg;/* No, Extract message from TCB (Put there by QPost)  */
    OSTCBCur->OSTCBMsg      = (void *)0;
    OSTCBCur->OSTCBStat     = OS_STAT_RDY;
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;     /*     No longer waiting for event                    */
    OS_EXIT_CRITICAL();
    *err                    = OS_NO_ERR;
	// 返回消息
    return (msg);                                /*     Return message received                        */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        POST MESSAGE TO A QUEUE
*
* Description: This function sends a message to a queue
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*
* Note(s)    : As of V2.60, this function allows you to send NULL pointer messages.
*********************************************************************************************************
*/
// 向消息队列发送一个消息,先进先出的方式
#if OS_Q_POST_EN > 0
INT8U  OSQPost (OS_EVENT *pevent, void *msg)
{
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) 					   /* Validate 'pevent'                            */
    {                     
        return (OS_ERR_PEVENT_NULL);
    }
#endif
	// 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q)		  /* Validate event block type                    */
    {      
        return (OS_ERR_EVENT_TYPE);
    }
    OS_ENTER_CRITICAL();
    // 有任务在等待消息队列?
    if (pevent->OSEventGrp != 0)					   /* See if any task pending on queue             */
    {          
    	// 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
        (void)OS_EventTaskRdy(pevent, msg, OS_STAT_Q); /* Ready highest priority task waiting on event */
        OS_EXIT_CRITICAL();
		// 找出优先级最高的任务并进行任务切换
        OS_Sched();                                    /* Find highest priority task ready to run      */
        return (OS_NO_ERR);
    }
    // 没有任务在等待消息队列，则
    // 获得队列控制块
    pq = (OS_Q *)pevent->OSEventPtr;                   /* Point to queue control block                 */
    // 消息个数满了?
    if (pq->OSQEntries >= pq->OSQSize) 				   /* Make sure queue is not full                  */
    {               
        OS_EXIT_CRITICAL();
        return (OS_Q_FULL);
    }
    // 消息个数没满，则
    // 将消息插入到队列中
    *pq->OSQIn++ = msg;                                /* Insert message into queue                    */
    // 消息数加一
    pq->OSQEntries++;                                  /* Update the nbr of entries in the queue       */
    // 到了队列尾?
    if (pq->OSQIn == pq->OSQEnd) 					   /* Wrap IN ptr if we are at end of queue        */
    {                     
        pq->OSQIn = pq->OSQStart;
    }
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                   POST MESSAGE TO THE FRONT OF A QUEUE
*
* Description: This function sends a message to a queue but unlike OSQPost(), the message is posted at
*              the front instead of the end of the queue.  Using OSQPostFront() allows you to send
*              'priority' messages.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*
* Note(s)    : As of V2.60, this function allows you to send NULL pointer messages.
*********************************************************************************************************
*/
// 向消息队列发送一个消息，后进先出的方式
#if OS_Q_POST_FRONT_EN > 0
INT8U  OSQPostFront (OS_EVENT *pevent, void *msg)
{
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0)					  /* Validate 'pevent'                             */
    {                    
        return (OS_ERR_PEVENT_NULL);
    }
#endif
	// 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) 	  /* Validate event block type                     */
    {    
        return (OS_ERR_EVENT_TYPE);
    }
    OS_ENTER_CRITICAL();
    // 有任务在等待消息队列?
    if (pevent->OSEventGrp != 0)					  /* See if any task pending on queue              */
    {                    
    	// 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。/
        (void)OS_EventTaskRdy(pevent, msg, OS_STAT_Q);/* Ready highest priority task waiting on event  */
        OS_EXIT_CRITICAL();
		// 找出优先级最高的任务并进行任务切换
        OS_Sched();                                   /* Find highest priority task ready to run       */
        return (OS_NO_ERR);
    }
    // 没有任务在等待消息队列,则
    // 获得队列控制块
    pq = (OS_Q *)pevent->OSEventPtr;                  /* Point to queue control block                  */
	// 队列满了?
    if (pq->OSQEntries >= pq->OSQSize) 				  /* Make sure queue is not full                   */
    {              
        OS_EXIT_CRITICAL();
        return (OS_Q_FULL);
    }
    // 消息个数没满，则
    // 我们是第一个消息?
    if (pq->OSQOut == pq->OSQStart) 				  /* Wrap OUT ptr if we are at the 1st queue entry */
    {                 
        pq->OSQOut = pq->OSQEnd;
    }
    pq->OSQOut--;
    // 插入消息到队列的最后面
    *pq->OSQOut = msg;                                /* Insert message into queue                     */
    // 消息数加一
    pq->OSQEntries++;                                 /* Update the nbr of entries in the queue        */
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        POST MESSAGE TO A QUEUE
*
* Description: This function sends a message to a queue.  This call has been added to reduce code size
*              since it can replace both OSQPost() and OSQPostFront().  Also, this function adds the
*              capability to broadcast a message to ALL tasks waiting on the message queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.
*
*              opt           determines the type of POST performed:
*                            OS_POST_OPT_NONE         POST to a single waiting task
*                                                     (Identical to OSQPost())
*                            OS_POST_OPT_BROADCAST    POST to ALL tasks that are waiting on the queue
*                            OS_POST_OPT_FRONT        POST as LIFO (Simulates OSQPostFront())
*
*                            Below is a list of ALL the possible combination of these flags:
*
*                                 1) OS_POST_OPT_NONE
*                                    identical to OSQPost()
*
*                                 2) OS_POST_OPT_FRONT
*                                    identical to OSQPostFront()
*
*                                 3) OS_POST_OPT_BROADCAST
*                                    identical to OSQPost() but will broadcast 'msg' to ALL waiting tasks
*
*                                 4) OS_POST_OPT_FRONT + OS_POST_OPT_BROADCAST  is identical to
*                                    OSQPostFront() except that will broadcast 'msg' to ALL waiting tasks
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*
* Warning    : Interrupts can be disabled for a long time if you do a 'broadcast'.  In fact, the
*              interrupt disable time is proportional to the number of tasks waiting on the queue.
*********************************************************************************************************
*/

#if OS_Q_POST_OPT_EN > 0
INT8U  OSQPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)
{
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif



#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }
#endif
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {     /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on queue              */
        if ((opt & OS_POST_OPT_BROADCAST) != 0x00) {  /* Do we need to post msg to ALL waiting tasks ? */
            while (pevent->OSEventGrp != 0) {         /* Yes, Post to ALL tasks waiting on queue       */
				// 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
                (void)OS_EventTaskRdy(pevent, msg, OS_STAT_Q);
            }
        } else {
        	// 从等待列表中找出使最高优先级任务（Highest Priority Task – HPT），并将其置于就绪态。
            (void)OS_EventTaskRdy(pevent, msg, OS_STAT_Q);  /* No,  Post to HPT waiting on queue       */
        }
        OS_EXIT_CRITICAL();
        if ((opt & OS_POST_OPT_NO_SCHED) == 0) {
            OS_Sched();                                     /* Find highest priority task ready to run */
        }
        return (OS_NO_ERR);
    }
    pq = (OS_Q *)pevent->OSEventPtr;                  /* Point to queue control block                  */
    if (pq->OSQEntries >= pq->OSQSize) {              /* Make sure queue is not full                   */
        OS_EXIT_CRITICAL();
        return (OS_Q_FULL);
    }
    if ((opt & OS_POST_OPT_FRONT) != 0x00) {          /* Do we post to the FRONT of the queue?         */
        if (pq->OSQOut == pq->OSQStart) {             /* Yes, Post as LIFO, Wrap OUT pointer if we ... */
            pq->OSQOut = pq->OSQEnd;                  /*      ... are at the 1st queue entry           */
        }
        pq->OSQOut--;
        *pq->OSQOut = msg;                            /*      Insert message into queue                */
    } else {                                          /* No,  Post as FIFO                             */
        *pq->OSQIn++ = msg;                           /*      Insert message into queue                */
        if (pq->OSQIn == pq->OSQEnd) {                /*      Wrap IN ptr if we are at end of queue    */
            pq->OSQIn = pq->OSQStart;
        }
    }
    pq->OSQEntries++;                                 /* Update the nbr of entries in the queue        */
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        QUERY A MESSAGE QUEUE
*
* Description: This function obtains information about a message queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              p_q_data      is a pointer to a structure that will contain information about the message
*                            queue.
*
* Returns    : OS_NO_ERR           The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE   If you are attempting to obtain data from a non queue.
*              OS_ERR_PEVENT_NULL  If 'pevent'   is a NULL pointer
*              OS_ERR_PDATA_NULL   If 'p_q_data' is a NULL pointer
*********************************************************************************************************
*/
// 查询一个消息队列的状态
#if OS_Q_QUERY_EN > 0
INT8U  OSQQuery (OS_EVENT *pevent, OS_Q_DATA *p_q_data)
{
    OS_Q      *pq;
    INT8U      i;
#if OS_LOWEST_PRIO <= 63
    INT8U     *psrc;
    INT8U     *pdest;
#else
    INT16U    *psrc;
    INT16U    *pdest;
#endif
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif

#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) 					   /* Validate 'pevent'                            */
    {                     
        return (OS_ERR_PEVENT_NULL);
    }
    if (p_q_data == (OS_Q_DATA *)0) 				   /* Validate 'p_q_data'                          */
    {                  
        return (OS_ERR_PDATA_NULL);
    }
#endif
	// 事件类型是消息队列?
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) 	   /* Validate event block type                    */
    {      
        return (OS_ERR_EVENT_TYPE);
    }
    
    OS_ENTER_CRITICAL();
    // 获得邮箱的状态,包括:
    // 等待任务表	
    p_q_data->OSEventGrp = pevent->OSEventGrp;         /* Copy message queue wait list                 */
    psrc                 = &pevent->OSEventTbl[0];
    pdest                = &p_q_data->OSEventTbl[0];
    for (i = 0; i < OS_EVENT_TBL_SIZE; i++) 
    {
        *pdest++ = *psrc++;
    }
    // 队列控制块
    pq = (OS_Q *)pevent->OSEventPtr;
	// 消息个数大于0?
    if (pq->OSQEntries > 0) 						  /* Get next message to return if available      */
    {
    	// 提取消息
        p_q_data->OSMsg = *pq->OSQOut;                 
    } 
    else 
    {
        p_q_data->OSMsg = (void *)0;
    }
    // 消息个数
    p_q_data->OSNMsgs = pq->OSQEntries;
	// 队列长度
    p_q_data->OSQSize = pq->OSQSize;
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                 /* OS_Q_QUERY_EN                                */

/*$PAGE*/
/*
*********************************************************************************************************
*                                      QUEUE MODULE INITIALIZATION
*
* Description : This function is called by uC/OS-II to initialize the message queue module.  Your
*               application MUST NOT call this function.
*
* Arguments   :  none
*
* Returns     : none
*
* Note(s)    : This function is INTERNAL to uC/OS-II and your application should not call it.
*********************************************************************************************************
*/

void  OS_QInit (void)
{
#if OS_MAX_QS == 1
    OSQFreeList         = &OSQTbl[0];                /* Only ONE queue!                                */
    OSQFreeList->OSQPtr = (OS_Q *)0;
#endif

#if OS_MAX_QS >= 2
    INT16U  i;
    OS_Q   *pq1;
    OS_Q   *pq2;



    OS_MemClr((INT8U *)&OSQTbl[0], sizeof(OSQTbl));  /* Clear the queue table                          */
    pq1 = &OSQTbl[0];
    pq2 = &OSQTbl[1];
    for (i = 0; i < (OS_MAX_QS - 1); i++) {          /* Init. list of free QUEUE control blocks        */
        pq1->OSQPtr = pq2;
        pq1++;
        pq2++;
    }
    pq1->OSQPtr = (OS_Q *)0;
    OSQFreeList = &OSQTbl[0];
#endif
}
#endif                                               /* OS_Q_EN                                        */
