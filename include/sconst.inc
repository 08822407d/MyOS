STACKBASE	equ	0
GSREG		equ	STACKBASE
FSREG		equ	GSREG		+ 4
ESREG		equ	FSREG		+ 4
DSREG		equ	ESREG		+ 4
EDIREG		equ	DSREG		+ 4
ESIREG		equ	EDIREG		+ 4
EBPREG		equ	ESIREG		+ 4
K_ESPREG	equ	EBPREG		+ 4
EBXREG		equ	K_ESPREG	+ 4
EDXREG		equ	EBXREG		+ 4
ECXREG		equ	EDXREG		+ 4
EAXREG		equ	ECXREG		+ 4
RETADDR		equ	EAXREG		+ 4
EIPREG		equ	RETADDR		+ 4
CSREG		equ	EIPREG		+ 4
EFLAGSREG	equ	CSREG		+ 4
ESPREG		equ	EFLAGSREG	+ 4
SSREG		equ	ESPREG		+ 4
STACKTOP	equ	SSREG		+ 4
LDT_SEL	    equ	STACKTOP
LDT		    equ	LDT_SEL	+ 4

TSS_SP0	 equ	4

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

INTVEC_SYSCALL   equ    0x80

SC_GET_TICKS     equ    0x00
SC_WRITE		 equ	0x01