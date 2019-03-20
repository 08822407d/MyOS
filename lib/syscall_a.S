%include "sconst.inc"

extern  sys_call

global  get_ticks

[BITS	32]
[SECTION .text]

get_ticks:
    mov eax,SC_GET_TICKS
    int INTVEC_SYSCALL
    ret