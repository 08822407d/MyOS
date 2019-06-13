INTVEC_SYSCALL  equ     0x80
SC_SENDRECV     equ     0x00
SC_PRINTX       equ     0x01

global  sendrec
global  printx

[BITS	32]
[SECTION .text]
; ====================================================================================
;                  sendrec(int function, int src_dest, MESSAGE* msg);
; ====================================================================================
; Never call sendrec() directly, call send_recv() instead.
sendrec:
	mov	eax, SC_SENDRECV
	mov	ebx, [esp + 4]	; function
	mov	ecx, [esp + 8]	; src_dest
	mov	edx, [esp + 12]	; p_msg
	int	INTVEC_SYSCALL
	ret

; ====================================================================================
;                          void printx(char* s);
; ====================================================================================
printx:
	mov	eax, SC_PRINTX
	mov	edx, [esp + 4]
	int	INTVEC_SYSCALL
	ret