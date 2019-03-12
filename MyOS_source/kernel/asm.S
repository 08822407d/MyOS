[BITS	32]
[SECTION .text]

global in_b
global out_b
global in_w
global out_w

global x86_lgdt
global x86_lidt
global x86_lldt
global x86_ltr
;==============================================================================
;in/out指令包装
;----------------------------------------------------------
in_b:
    mov	edx, [esp + 4]
	xor	eax, eax
	in	al, dx
	dw  0x00EB
	dw  0x00EB
;按__cdecl，返回值放在eax中
	ret

out_b:
    mov	edx, [esp + 4]              ;端口号
	mov	al, [esp + 4 + 4]           ;out的值
	out	dx, al
	dw  0x00EB
	dw  0x00EB
;无返回值
	ret

;----------------------------------------------------------
in_w:
    mov	edx, [esp + 4]
	xor	eax, eax
	in	ax, dx
	dw  0x00EB
	dw  0x00EB
;按__cdecl，返回值放在eax中
	ret

out_w:
    mov	edx, [esp + 4]              ;端口号
	mov	ax, [esp + 4 + 4]           ;out的值
	out	dx, ax
	dw  0x00EB
	dw  0x00EB
;无返回值
	ret
;==============================================================================
;保护模式指令包装
;----------------------------------------------------------
x86_lgdt:
	mov	edx, [esp + 4]
	lgdt [edx]
	ret

x86_lidt:
	mov	edx, [esp + 4]
	lidt [edx]
	ret

x86_lldt:
	mov	dx, [esp + 4]
	lldt dx
	ret

x86_ltr:
	mov	dx, [esp + 4]
	ltr dx
	ret
;==============================================================================