%include "sconst.inc"

extern  sys_call

global  get_ticks
global  write

[BITS	32]
[SECTION .text]

get_ticks:
    mov eax,SC_GET_TICKS
    int INTVEC_SYSCALL
    ret

write:
    mov eax,SC_WRITE
    mov ebx,[esp + 4]
    mov ecx,[esp + 8]
    int INTVEC_SYSCALL
    ret