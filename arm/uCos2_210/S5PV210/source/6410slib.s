;=====================================================================
; File Name : 2440slib.s
; Function  : S3C2440  (Assembly)
; Date      : March 09, 2002
; Revision	: Programming start (February 26,2002) -> SOP
; Revision	: 03.11.2003 ver 0.0	Attatched for 2440
;=====================================================================

;Interrupt, FIQ/IRQ disable
NOINT  EQU 0xc0    ; 1100 0000

;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.
   GBLL    THUMBCODE
   [ {CONFIG} = 16
THUMBCODE SETL  {TRUE}
	 CODE32
   |
THUMBCODE SETL  {FALSE}
   ]

   MACRO
     MOV_PC_LR
     [ THUMBCODE
       bx lr
     |
       mov pc,lr
     ]
   MEND

   AREA |C$$code|, CODE, READONLY

;========================================
;Workaround of problem between LCD and Framebuffer
;========================================
	EXPORT	SDRAMtest
SDRAMtest
    	ldr r0,=0x31000000
   		ldr r1,=0x80000
   		mov r2,#0xf000000f
LB2 		str r2,[r0],#4
		str r2,[r0],#4
   		subs r1,r1,#4
   		bne LB2
   		mov pc,lr
   		

;==============
; CPSR I,F bit
;==============
;int SET_IF(void);
;The return value is current CPSR.
	EXPORT	SET_IF
SET_IF
    ;This function works only if the processor is in previliged mode.
   mrs r0,cpsr
   mov r1,r0
   orr r1,r1,#NOINT
   msr cpsr_cxsf,r1
   MOV_PC_LR

;void WR_IF(int cpsrValue);
   EXPORT WR_IF
WR_IF
    ;This function works only if the processor is in previliged mode.
   msr cpsr_cxsf,r0
   MOV_PC_LR


;void CLR_IF(void);
   EXPORT  CLR_IF
CLR_IF
    ;This function works only if the processor is in previliged mode.
   mrs r0,cpsr
   bic r0,r0,#NOINT
   msr cpsr_cxsf,r0
   MOV_PC_LR

;====================================
; MMU Cache/TLB/etc on/off functions
;====================================
R1_I	EQU	(1<<12)
R1_C	EQU	(1<<2)
R1_A	EQU	(1<<1)
R1_M    EQU	(1)
R1_iA	EQU	(1<<31)
R1_nF   EQU	(1<<30)

;void MMU_EnableICache(void)
   EXPORT MMU_EnableICache
MMU_EnableICache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableICache(void)
   EXPORT MMU_DisableICache
MMU_DisableICache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_EnableDCache(void)
   EXPORT MMU_EnableDCache
MMU_EnableDCache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableDCache(void)
   EXPORT MMU_DisableDCache
MMU_DisableDCache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_EnableAlignFault(void)
   EXPORT MMU_EnableAlignFault
MMU_EnableAlignFault
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableAlignFault(void)
   EXPORT MMU_DisableAlignFault
MMU_DisableAlignFault
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_EnableMMU(void)
   EXPORT MMU_EnableMMU
MMU_EnableMMU
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableMMU(void)
   EXPORT MMU_DisableMMU
MMU_DisableMMU
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_SetFastBusMode(void)
; FCLK:HCLK= 1:1
  EXPORT MMU_SetFastBusMode
MMU_SetFastBusMode
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_iA:OR:R1_nF
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_SetAsyncBusMode(void)
; FCLK:HCLK= 1:2
   EXPORT MMU_SetAsyncBusMode
MMU_SetAsyncBusMode
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_nF:OR:R1_iA
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;=========================
; Set TTBase
;=========================
;void MMU_SetTTBase(int base)
   EXPORT MMU_SetTTBase
MMU_SetTTBase
   ;ro=TTBase
   mcr p15,0,r0,c2,c0,0
   MOV_PC_LR

;=========================
; Set Domain
;=========================
;void MMU_SetDomain(int domain)
   EXPORT MMU_SetDomain
MMU_SetDomain
   ;ro=domain
   mcr p15,0,r0,c3,c0,0
   MOV_PC_LR

;=========================
; ICache/DCache functions
;=========================
;void MMU_InvalidateIDCache(void)
   EXPORT MMU_InvalidateIDCache
MMU_InvalidateIDCache
   mcr p15,0,r0,c7,c7,0
   MOV_PC_LR

;void MMU_InvalidateICache(void)
   EXPORT MMU_InvalidateICache
MMU_InvalidateICache
   mcr p15,0,r0,c7,c5,0
   MOV_PC_LR

;void MMU_InvalidateICacheMVA(U32 mva)
   EXPORT MMU_InvalidateICacheMVA
MMU_InvalidateICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c5,1
   MOV_PC_LR

;void MMU_PrefetchICacheMVA(U32 mva)
   EXPORT MMU_PrefetchICacheMVA
MMU_PrefetchICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c13,1
   MOV_PC_LR

;void MMU_InvalidateDCache(void)
   EXPORT MMU_InvalidateDCache
MMU_InvalidateDCache
   mcr p15,0,r0,c7,c6,0
   MOV_PC_LR

;void MMU_InvalidateDCacheMVA(U32 mva)
   EXPORT MMU_InvalidateDCacheMVA
MMU_InvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c6,1
   MOV_PC_LR

;void MMU_CleanDCacheMVA(U32 mva)
   EXPORT MMU_CleanDCacheMVA
MMU_CleanDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c10,1
   MOV_PC_LR

;void MMU_CleanInvalidateDCacheMVA(U32 mva)
   EXPORT MMU_CleanInvalidateDCacheMVA
MMU_CleanInvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c14,1
   MOV_PC_LR

;void MMU_CleanDCacheIndex(U32 index)
   EXPORT MMU_CleanDCacheIndex
MMU_CleanDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c10,2
   MOV_PC_LR

;void MMU_CleanInvalidateDCacheIndex(U32 index)
   EXPORT MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c14,2
   MOV_PC_LR

;void MMU_WaitForInterrupt(void)
   EXPORT MMU_WaitForInterrupt
MMU_WaitForInterrupt
   mcr p15,0,r0,c7,c0,4
   MOV_PC_LR

;===============
; TLB functions
;===============
;voic MMU_InvalidateTLB(void)
   EXPORT MMU_InvalidateTLB
MMU_InvalidateTLB
   mcr p15,0,r0,c8,c7,0
   MOV_PC_LR

;void MMU_InvalidateITLB(void)
   EXPORT MMU_InvalidateITLB
MMU_InvalidateITLB
   mcr p15,0,r0,c8,c5,0
   MOV_PC_LR

;void MMU_InvalidateITLBMVA(U32 mva)
   EXPORT MMU_InvalidateITLBMVA
MMU_InvalidateITLBMVA
   ;ro=mva
   mcr p15,0,r0,c8,c5,1
   MOV_PC_LR

;void MMU_InvalidateDTLB(void)
	EXPORT MMU_InvalidateDTLB
MMU_InvalidateDTLB
	mcr p15,0,r0,c8,c6,0
	MOV_PC_LR

;void MMU_InvalidateDTLBMVA(U32 mva)
	EXPORT MMU_InvalidateDTLBMVA
MMU_InvalidateDTLBMVA
	;r0=mva
	mcr p15,0,r0,c8,c6,1
	MOV_PC_LR

;=================
; Cache lock down
;=================
;void MMU_SetDCacheLockdownBase(U32 base)
   EXPORT MMU_SetDCacheLockdownBase
MMU_SetDCacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,0
   MOV_PC_LR

;void MMU_SetICacheLockdownBase(U32 base)
   EXPORT MMU_SetICacheLockdownBase
MMU_SetICacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,1
   MOV_PC_LR

;=================
; TLB lock down
;=================
;void MMU_SetDTLBLockdown(U32 baseVictim)
   EXPORT MMU_SetDTLBLockdown
MMU_SetDTLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,0
   MOV_PC_LR

;void MMU_SetITLBLockdown(U32 baseVictim)
   EXPORT MMU_SetITLBLockdown
MMU_SetITLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,1
   MOV_PC_LR

;============
; Process ID
;============
;void MMU_SetProcessId(U32 pid)
   EXPORT MMU_SetProcessId
MMU_SetProcessId
   ;r0= pid
   mcr p15,0,r0,c13,c0,0
   MOV_PC_LR

   END
