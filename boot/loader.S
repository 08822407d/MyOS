%include    "bootrc.inc"

[BITS 16]
jmp loader_start
;==================================================================================================
;loader使用的GDT，也作为kernel使用的GDT的基础
;会被复制到kernel的GDT空间里，然后会把GDTptr也复制给c语言编写的kernel的gdt_ptr
GDTStart:
;                                       段基址               ，段界限             ，段属性
DESC_NULL:              Descriptor      0x0                 ,0x0                ,0x0
DESC_VgaRAM:            Descriptor      _segbase_VgaRAM     ,0x8000 - 1         ,DA_32 | DA_DRWu
DESC_GlbCode:           Descriptor      _segbase_GlbCode    ,0xFFFFF            ,DA_32 | DA_Gr_4K | DA_CR
DESC_GlbData:           Descriptor      _segbase_GlbData    ,0xFFFFF            ,DA_32 | DA_Gr_4K | DA_DRWu
DESC_SysStack:          Descriptor      _segbase_SysStack   ,0x4000 - 1         ,DA_32 | DA_DRWd
GDTEnd:

GDTLen          EQU     GDTEnd  -   GDTStart
GDTptr          dw      GDTLen  -   1
                dd      (GDTStart + _off_LoaderSetup)

;段选择符
sele_VgaRAM     EQU     DESC_VgaRAM     -   GDTStart
sele_GlbCode    EQU     DESC_GlbCode    -   GDTStart
sele_GlbData    EQU     DESC_GlbData    -   GDTStart
sele_SysStack   EQU     DESC_SysStack   -   GDTStart

IDTptr          dw      0
                dd      0
;---------------------------------------------------------------------------

;---------------------------------------------------------------------------
;一些显示信息用的字符串
;实模式下
Msg1        db      "Memory Check Fail!"
;保护模式下的字符串，C风格
Msg2        db      "Now In Protect Mode",0x0A,0
Msg3        db      "Reading Kernel...",0x0A,0
Msg4		db		"Kernel File Format Error!",0x0A,0
Msg5        db      "Finish.",0x0A,0

Len_Msg1    EQU     Msg2    -   Msg1

;---------------------------------------------------------------------------
;检查elf头的前四个字节是否匹配，以及elf头长度是否是32位版的长度0x34
ELF_32_check:
	db 0x7F,"ELF",0x34
;---------------------------------------------------------------------------
;存放安置好的kernenl的入口点
ELF_32_ENTRY:
    dd 0x0
;==================================================================================================

%include    "include_RM.inc"

loader_start:
;初始化段寄存器
    mov ax,cs
    mov ds,ax
    mov es,ax

;E820获取内存分布
    call E820
;复制IVT 0x41和0x46的硬盘信息
    call Get_HD_Info

;---------------------------------------------------------------------------
;进保护模式
	lgdt [GDTptr]
    lidt [IDTptr]

	cli

	in	al, 92h
	or	al, 00000010b
	out	92h, al

	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	jmp	dWORD sele_GlbCode:(_off_LoaderSetup + PMode_Start)
;---------------------------------------------------------------------------
[BITS 32]
%include    "include_PM.inc"
PMode_Start:
    mov ax,sele_GlbData
    mov ds,ax
    mov es,ax
    mov ax,sele_SysStack
    mov ss,ax
    mov esp,0x0
    mov ax,sele_VgaRAM
    mov gs,ax

;将光标位置参数由之前的INT15规格转换为线性地址
    mov dx,[Phys_CursorPos]
    add dl,dl
    mov al,dh
    mov bl,160
    mul bl
    xor dh,dh
    add ax,dx
    mov [Phys_CursorPos],ax

    push dword (_off_LoaderSetup + Msg2)
    call printstr_tmp
    add esp,0x04

;计算内存大小
    call calc_MemSize
;设置内存分页
    call setup_page
;从硬盘读取kernel
    push dWORD (_off_LoaderSetup + Msg3)
    call printstr_tmp
    add esp,0x04

    call read_kernel_HDD
;检查kernel文件格式
    call check_kfile_format
;重新安置kernel各section
    call arrange_elf32

    push dWORD (_off_LoaderSetup + Msg5)
    call printstr_tmp
    add esp,0x04
;统一段寄存器，控制转移到kernel
    xor eax,eax
    mov ax,sele_GlbData
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov ax,sele_VgaRAM
    mov gs,ax

    push DWORD sele_GlbCode
    push DWORD [_off_LoaderSetup + ELF_32_ENTRY]
    retf
;==================================================================================================