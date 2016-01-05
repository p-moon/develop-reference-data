;=========================================
; NAME:210INIT.S
; DESC: C start up codes
;       Configure memory, ISR ,stacks
;		Initialize C-variables
;=========================================

_ISR_STARTADDRESS	EQU 	0x3fff0000	

;// 7种模式 
USERMODE    		EQU 	0x10
FIQMODE     		EQU 	0x11
IRQMODE     		EQU 	0x12
SVCMODE     		EQU 	0x13
ABORTMODE   		EQU 	0x17
UNDEFMODE   		EQU 	0x1b
MODEMASK    		EQU 	0x1f
NOINT       		EQU 	0xc0

;// 栈的分配
_STACK_BASEADDRESS	EQU 	0x3fff8000					
UserStack			EQU		(_STACK_BASEADDRESS - 0x5000)	
SVCStack			EQU		(_STACK_BASEADDRESS - 0x4000)	
UndefStack			EQU		(_STACK_BASEADDRESS - 0x3000)	
AbortStack			EQU		(_STACK_BASEADDRESS - 0x2000)	
IRQStack			EQU		(_STACK_BASEADDRESS - 0x1000)	
FIQStack			EQU		(_STACK_BASEADDRESS - 0x0)		

	;// 进栈:
	;// |HandleIRQ	|
	;// |r0			|
	;// 出栈:
	;// pc = HandleIRQ
	;// r0 = r0
 	MACRO
$HandlerLabel HANDLER $HandleLabel
$HandlerLabel
	sub	sp,sp,#4										; decrement sp(to store jump address)
	stmfd	sp!,{r0}									; PUSH the work register to stack(lr does''t push because it return to original address)
	ldr     r0,=$HandleLabel							; load the address of HandleXXX to r0
	ldr     r0,[r0]	 									; load the contents(service routine start address) of HandleXXX
	str     r0,[sp,#4]      							; store the contents(ISR) of HandleXXX to stack
	ldmfd   sp!,{r0,pc}     							; POP the work register and pc(jump to ISR)
	MEND

	AREA    Init,CODE,READONLY
	PRESERVE8

	IMPORT |Image$$RO$$Base|
	IMPORT	main										; The main entry of mon program 
	IMPORT	Led_Blink
	IMPORT  OS_CPU_IRQ_ISR 								; uCOS_II IrqISR
	EXPORT	HandleIRQ		
	EXPORT  __ENTRY


__ENTRY
	b	ResetHandler
	b	HandlerUndef									; handler for Undefined mode
	b	HandlerSWI	    								; handler for SWI interrupt
	b	HandlerPabort									; handler for PAbort
	b	HandlerDabort									; handler for DAbort
	b	.		        								; reserved
	b	HandlerIRQ										; handler for IRQ interrupt
	b	HandlerFIQ										; handler for FIQ interrupt
	b	HandlerUndef									; Must be @0x20.
	
HandlerFIQ      HANDLER HandleFIQ
HandlerIRQ      HANDLER HandleIRQ
HandlerUndef    HANDLER HandleUndef
HandlerSWI      HANDLER HandleSWI
HandlerDabort   HANDLER HandleDabort
HandlerPabort   HANDLER HandlePabort

ResetHandler
	bl	InitStacks									; Initialize stacks
	
	ldr		r0, =HandleIRQ       						; Setup IRQ handler
	ldr		r1, =OS_CPU_IRQ_ISR 				
	str		r1, [r0]

	ldr 	r0, =0xE2700000								; WatchDog timer disable
	mov 	r1, #0
	str 	r1, [r0]

	mov 	r0, #0x53									; allow irq
	msr	 	CPSR_cxsf, r0	

	ldr 	r0, =|Image$$RO$$Base|						; Image$$RO$$Base = entry point
	bl		CoSetExceptonVectoerBase					; set entry point as except vector
	
	ldr 	r0,=0xffffffff
	mcr 	p15,0,r0,c1,c0,2							; Enable coprocessors
	mov 	R0,#0x40000008
	vmsr	fpexc,r0									; Globally enable NEON & VFP

	IMPORT |Image$$ZI$$Base|							; clear ZI section (BSS section)
	IMPORT |Image$$ZI$$Limit|
	ldr r3, =|Image$$ZI$$Base|	
	ldr r1, =|Image$$ZI$$Limit| 						; Top of zero init segment
	mov r2, #0
loop_zero
	cmp r3, r1							
	str r2, [r3], #4
	bne loop_zero
	
 	bl	main											; jump to main

InitStacks
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1									; UndefMode
	ldr	sp,=UndefStack		 

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1									; AbortMode
	ldr	sp,=AbortStack		 

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1									; IRQMode
	ldr	sp,=IRQStack		 

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1									; FIQMode
	ldr	sp,=FIQStack		 

	bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1									; SVCMode
	ldr	sp,=SVCStack		 

	mov	pc,lr	

CoSetExceptonVectoerBase
	mcr 	p15,0,r0,c12,c0,0
	mov 	pc,lr
		
	ALIGN
	AREA IntVector, DATA, READWRITE

	^   _ISR_STARTADDRESS							
HandleReset 				#   	4
HandleUndef 				#   	4
HandleSWI					#   	4
HandlePabort    			#   	4
HandleDabort   				#   	4
HandleReserved  			#   	4
HandleIRQ					#  		4
HandleFIQ					#		4

	END
