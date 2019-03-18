;暂时完成
[BITS	32]
[SECTION .text]

; -----------------------------------------------------------------------------
; 接口
; ---------------------------------
extern	exception_handler
extern  irq_handler
extern	spurious_irq
extern	k_reenter
extern	stack_top
extern	p_proc_ready
extern	tss0

; ---------------------------------
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error
global	alignment_check
global	machine_check
global	simd_exception

global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15

global	undef_exception
global	undef_hwint

global restart
%include "sconst.inc"
; ---------------------------------------------------
;未定义异常和IRQ入口
undef_hwint:
undef_exception:

; =======================================================================================
; 中断和异常 -- 硬件中断
; --------------------------------
%macro  hwint_master 1
		sub esp,4
		pushad
		push ds
		push es
		push fs
		push gs

		mov dx,ss
		mov ds,dx
		mov es,dx
		mov esp,stack_top

		inc BYTE [gs:0]

		mov al,0x20
		out 0x20,al

		inc dWORD [k_reenter]
		cmp dWORD [k_reenter],0
		jne restart

        push %1
        call irq_handler
        pop  ecx

		sti
		jmp restart
%endmacro

restart:
		cli
		mov  esp,[p_proc_ready]
		lldt [esp + LDT_SEL]
		lea  eax,[esp + STACKTOP]
		mov  [tss0 + TSS_SP0],eax
re_enter:
		dec dWORD [k_reenter]
		pop gs
		pop fs
		pop es
		pop ds
		popad
		add esp,4

        iretd
; ---------------------------------
ALIGN   16
hwint00:                	; irq 0 (系统闹钟/定时器).
    hwint_master    0
ALIGN   16
hwint01:                	; irq 1 (键盘)
    hwint_master    1
ALIGN   16
hwint02:                	; irq 2 (接从片)
    hwint_master    2
ALIGN   16
hwint03:                	; irq 3 (第二串口)
    hwint_master    3
ALIGN   16
hwint04:                	; irq 4 (第一串口)
    hwint_master    4
ALIGN   16
hwint05:                	; irq 5 (XT盘/现在不使用)
    hwint_master    5
ALIGN   16
hwint06:                	; irq 6 (软盘驱动器)
    hwint_master    6
ALIGN   16
hwint07:                	; irq 7 (打印机)
    hwint_master    7

; ---------------------------------
%macro  hwint_slave		1
		pushad
		push ds
		push es
		push fs
		push gs

;        push    %1
;        call    irq_handler
;        add     esp, 4

		pop gs
		pop fs
		pop es
		pop ds
		popad

        iret
%endmacro
; ---------------------------------
ALIGN   16
hwint08:                	; irq 8 (RTC时钟).
    hwint_slave     8
ALIGN   16
hwint09:                	; irq 9 (irq 2 重定位)
    hwint_slave     9
ALIGN   16
hwint10:                	; irq 10 (未分配)
    hwint_slave     10
ALIGN   16
hwint11:                	; irq 11 (未分配)
    hwint_slave     11
ALIGN   16
hwint12:                	; irq 12 (PS/2鼠标)
    hwint_slave     12
ALIGN   16
hwint13:                	; irq 13 (浮点处理器异常)
    hwint_slave     13
ALIGN   16
hwint14:                	; irq 14 (AT硬盘/温彻斯特盘/主IDE盘)
    hwint_slave     14
ALIGN   16
hwint15:                	; irq 15 (次IDE盘)
    hwint_slave     15

; =======================================================================================
; 中断和异常 -- 异常
; ---------------------------------
;有错误码
%macro EXCEPTION_ERR_CODE 1
	push	%1
	jmp		exception_entry
%endmacro
;无错误码
%macro EXCEPTION_NO_ERR_CODE 1
	push	~0
	EXCEPTION_ERR_CODE %1
%endmacro
; ---------------------------------
; 异常处理入口
divide_error:						; #DE  0除
	EXCEPTION_NO_ERR_CODE	0
single_step_exception:				; #DB  单步中断
	EXCEPTION_NO_ERR_CODE	1
nmi:								; #NMI 不可屏蔽中断
	EXCEPTION_NO_ERR_CODE	2
breakpoint_exception:				; #BP  断点
	EXCEPTION_NO_ERR_CODE	3
overflow:							; #OF  溢出
	EXCEPTION_NO_ERR_CODE	4
bounds_check:						; #BR  数组超界
	EXCEPTION_NO_ERR_CODE	5
inval_opcode:						; #UD  无效操作
	EXCEPTION_NO_ERR_CODE	6
copr_not_available:					; #NM  协处理器不可用
	EXCEPTION_NO_ERR_CODE	7
double_fault:						; #DF  双重错误
	EXCEPTION_ERR_CODE 		8
copr_seg_overrun:					;      协处理器段超界
	EXCEPTION_NO_ERR_CODE	9
inval_tss:							; #TS  无效的TSS
	EXCEPTION_ERR_CODE 		10
segment_not_present:				; #NP  段不存在
	EXCEPTION_ERR_CODE 		11
stack_exception:					; #SS  栈段错误
	EXCEPTION_ERR_CODE 		12
general_protection:					; #GP  常规保
	EXCEPTION_ERR_CODE 		13
page_fault:							; #PF  页错误
	EXCEPTION_ERR_CODE 		14
copr_error:							; #MF  x87 FPU错
	EXCEPTION_NO_ERR_CODE	16
alignment_check:					; #AC  对齐检
	EXCEPTION_ERR_CODE 		17
machine_check:						; #MC  机器检查
	EXCEPTION_NO_ERR_CODE	18
simd_exception:						; #XM  SIMD浮点异常
	EXCEPTION_NO_ERR_CODE	19

; ---------------------------------
; 统一的异常处理函数
exception_entry:
	call	exception_handler
	add		esp, 4*2
	hlt
	iret

; -----------------------------------------------------------------------------

