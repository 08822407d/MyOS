
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               sconst.inc
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

STACKBASE	 equ	0
GS		 equ	STACKBASE
FS		 equ	GS		+ 4
ES	 	 equ	FS		+ 4
DS	 	 equ	ES		+ 4
EDI	 	 equ	DS		+ 4
ESI		 equ	EDI		+ 4
EBP		 equ	ESI		+ 4
K_ESP    equ	EBP		+ 4
EBX		 equ	K_ESP	+ 4
EDX		 equ	EBX		+ 4
ECX		 equ	EDX		+ 4
EAX		 equ	ECX		+ 4
RETADDR	 equ	EAX		+ 4
EIP		 equ	RETADDR	+ 4
CS		 equ	EIP		+ 4
EFLAGS	 equ	CS		+ 4
ESP		 equ	EFLAGS	+ 4
SS		 equ	ESP		+ 4
STACKTOP equ	SS 		+ 4
LDT_SEL	 equ	STACKTOP
LDT		 equ	LDT_SEL	+ 4

TSS3_S_SP0	 equ	4

INT_CTL      equ	0x20	; I/O port for interrupt controller         <Master>
INT_CTLMASK  equ	0x21	; setting bits in this port disables ints   <Master>
INT2_CTL     equ	0xA0	; I/O port for second interrupt controller  <Slave>
INT2_CTLMASK equ	0xA1	; setting bits in this port disables ints   <Slave>

END_OF_INT	 equ	0x20

; 以下选择子值必须与 protect.h 中保持一致!!!
SELECTOR_CS_KERN equ	0x10
SELECTOR_DS_KERN equ	0x18
SELECTOR_TSS0    equ    0x30		; TSS. 从外层跳到内存时 SS 和 ESP 的值从里面获得.
SELECTOR_LDT0    equ    0x38