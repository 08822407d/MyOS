
;==================================================================================================
[BITS 32]
;---------------------------------------------------------------------------
extern  disp_pos

extern  kernel_main
extern  sys_init
;---------------------------------------------------------------------------
[section .bss]
stackspace  resb 0x2000
stack_top:
;---------------------------------------------------------------------------
[section .text]

global  _start
global  stack_top
;---------------------------------------------------------------------------
_start:
; 转移栈到kernel中
    mov ax,ds
    mov ss,ax
    mov esp,stack_top
    cli
; 将汇编代码设置的参数转移到c文件中
    ;移交光标位置
    mov ax,[Phys_CursorPos]
    mov [disp_pos],ax

    call sys_init
;用长跳转强制刷新段寄存器
	jmp	SELECTOR_CS_KRNL:c_init
c_init:	
    call kernel_main

    sti
    jmp $
;---------------------------------------------------------------------------
Phys_CursorPos      EQU     0x90000             ;正式用途是存放光标位置，启动阶段用于临时存放
                                                        ;最后一条字符串的行列数
SELECTOR_CS_KRNL    EQU     0x10