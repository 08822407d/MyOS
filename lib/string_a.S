[BITS   32]
[SECTION .text]

; 导出函数
global	memcpy
global	memset
global	strcpy
global  strlen

; ========================================================================
; 			void memcpy(void * dest, void * src, u32_t size);
; ========================================================================
memcpy:
	push ebp
	mov	ebp,esp

	push esi
	push edi
	push ecx

	mov	edi,[ebp + 8]	    ; 目标地址
	mov	esi,[ebp + 12]	    ; 源地址
	mov	ecx,[ebp + 16]	    ; 长度
.j1:
;判断是否复制完，是则跳出
	cmp	ecx,0
	jz .j2
	mov	al,[ds:esi]		    
	inc	esi
	mov	byte [es:edi],al	
	inc	edi

	dec	ecx
	jmp	.j1
.j2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp,ebp
	pop	ebp

	ret
; ------------------------------------------------------------------------

; ========================================================================
; 				void memset(void* p_dst, char content, int size);
; ========================================================================
memset:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]		; Destination
	mov	edx, [ebp + 12]		; Char to be putted
	mov	ecx, [ebp + 16]		; Counter
.1:
	cmp	ecx, 0				; 判断计数器
	jz	.2					; 计数器为零时跳出

	mov	byte [edi], dl		; ┓
	inc	edi					; ┛

	dec	ecx					; 计数器减一
	jmp	.1					; 循环
.2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret
; ------------------------------------------------------------------------


; ========================================================================
; 				char* strcpy(char* p_dst, char* p_src);
; ========================================================================
strcpy:
	push    ebp
	mov     ebp, esp

	mov     esi, [ebp + 12] 	; Source
	mov     edi, [ebp + 8]  	; Destination

.1:
	mov     al, [esi]           ; ┓
	inc     esi                 ; ┃
								; ┣ 逐字节移动
	mov     byte [edi], al      ; ┃
	inc     edi                 ; ┛

	cmp     al, 0           	; 是否遇到 '\0'
	jnz     .1              	; 没遇到就继续循环，遇到就结束

	mov     eax, [ebp + 8]  	; 返回值

	pop     ebp
	
	ret
; ------------------------------------------------------------------------

; ------------------------------------------------------------------------
; int strlen(char* p_str);
; ------------------------------------------------------------------------
strlen:
        push    ebp
        mov     ebp, esp

        mov     eax, 0                  ; 字符串长度开始是 0
        mov     esi, [ebp + 8]          ; esi 指向首地址

.1:
        cmp     byte [esi], 0           ; 看 esi 指向的字符是否是 '\0'
        jz      .2                      ; 如果是 '\0'，程序结束
        inc     esi                     ; 如果不是 '\0'，esi 指向下一个字符
        inc     eax                     ;         并且，eax 自加一
        jmp     .1                      ; 如此循环

.2:
        pop     ebp
        ret                             ; 函数结束，返回
; ------------------------------------------------------------------------