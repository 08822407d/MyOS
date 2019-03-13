; DEBUG日记
; 2018.01.01
; 编译链接之后，测试用的异常处理函数后半段屏幕显示总是乱码，跟踪了十几遍仔细对比才发现，
; 编译器把ebx也拿来寻址用（固定在字符串数据的指针列表开头），但是下面照搬教材示例代码，
; 并没有考虑到这个问题，所以没有保护ebx。加入pushad和popad后就能正常显示了

[BITS   32]
[SECTION    .text]
; 导入全局变量
extern	disp_pos


[SECTION .text]

; 导出函数
global	disp_str
global	disp_color_str

; ========================================================================
;                  void disp_str(char * str);
; ========================================================================
; 打印字符串（默认黑底白字）
disp_str:
	push ebp
	mov	ebp,esp
	pushad

	mov	esi,[ebp + 8]		; 字符串首地址
	mov	edi,[disp_pos]
	mov	ah,0x0F
.j1:
	lodsb
	test al,al
	jz .j2
	cmp	al,0x0A				; 判断回车
	jnz	.j3
	push eax
	mov	eax,edi
	mov	bl,160
	div	bl
	and	eax,0xFF
	inc	eax
	mov	bl,160
	mul	bl
	mov	edi,eax
	pop	eax
	jmp	.j1
.j3:
	mov	[gs:edi],ax
	add	edi,2
	jmp	.j1

.j2:
	mov	[disp_pos],edi

	popad
	pop	ebp
	ret

; ========================================================================
;                  void disp_color_str(char * str, u16_t color);
; ========================================================================
; 打印字符串（自定义颜色）
disp_color_str:
	push ebp
	mov	ebp,esp
	pushad

	mov	esi,[ebp + 8]		; 字符串首地址
	mov	edi,[disp_pos]
	mov	ah,[ebp + 12]		; 颜色参数
.j1:
	lodsb
	test al,al
	jz .j2
	cmp	al,0x0A				; 判断回车
	jnz	.j3
	push eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0xFF
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.j1
.j3:
	mov	[gs:edi],ax
	add	edi,2
	jmp	.j1

.j2:
	mov	[disp_pos],edi

	popad
	pop	ebp
	ret
