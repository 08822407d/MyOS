	.file	"interrupt_c.c"
	.text
	.globl	set_int_gate
	.type	set_int_gate, @function
set_int_gate:
.LFB0:
	.cfi_startproc
	movl	12(%esp), %edx
	movl	8(%esp), %eax
	sall	$3, %eax
	addl	4(%esp), %eax
	movw	%dx, (%eax)
	movw	$16, 2(%eax)
	movl	16(%esp), %ecx
	movb	%cl, 5(%eax)
	shrl	$16, %edx
	movw	%dx, 6(%eax)
	ret
	.cfi_endproc
.LFE0:
	.size	set_int_gate, .-set_int_gate
	.globl	set_int_gate_idt
	.type	set_int_gate_idt, @function
set_int_gate_idt:
.LFB1:
	.cfi_startproc
	pushl	12(%esp)
	.cfi_def_cfa_offset 8
	pushl	12(%esp)
	.cfi_def_cfa_offset 12
	pushl	12(%esp)
	.cfi_def_cfa_offset 16
	pushl	$idt
	.cfi_def_cfa_offset 20
	call	set_int_gate
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE1:
	.size	set_int_gate_idt, .-set_int_gate_idt
	.globl	idt_veccpy
	.type	idt_veccpy, @function
idt_veccpy:
.LFB2:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ebx
.L4:
	movl	(%ebx), %edx
	testl	%edx, %edx
	je	.L7
	movzbl	5(%ebx), %eax
	sall	$5, %eax
	orb	$-114, %al
	pushl	%eax
	.cfi_def_cfa_offset 12
	pushl	%edx
	.cfi_def_cfa_offset 16
	movzbl	4(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 20
	pushl	$idt
	.cfi_def_cfa_offset 24
	call	set_int_gate
	addl	$8, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 8
	jmp	.L4
.L7:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE2:
	.size	idt_veccpy, .-idt_veccpy
	.globl	idt_veccpy_exception
	.type	idt_veccpy_exception, @function
idt_veccpy_exception:
.LFB3:
	.cfi_startproc
	pushl	$gate_table_exceptions
	.cfi_def_cfa_offset 8
	call	idt_veccpy
	addl	$4, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE3:
	.size	idt_veccpy_exception, .-idt_veccpy_exception
	.globl	idt_veccpy_pic
	.type	idt_veccpy_pic, @function
idt_veccpy_pic:
.LFB4:
	.cfi_startproc
	pushl	$gate_table_pic
	.cfi_def_cfa_offset 8
	call	idt_veccpy
	addl	$4, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE4:
	.size	idt_veccpy_pic, .-idt_veccpy_pic
	.globl	idt_reload
	.type	idt_reload, @function
idt_reload:
.LFB5:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$idt_desc
	.cfi_def_cfa_offset 32
	call	x86_lidt
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE5:
	.size	idt_reload, .-idt_reload
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	" "
.LC1:
	.string	"Exception! --> "
.LC2:
	.string	"\n\n"
.LC3:
	.string	"  EFLAGS:"
.LC4:
	.string	"  CS:"
.LC5:
	.string	"  EIP:"
.LC6:
	.string	"  Error code:"
	.text
	.globl	exception_handler
	.type	exception_handler, @function
exception_handler:
.LFB6:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	movw	$0, disp_pos
	movl	$0, %ebx
.L13:
	cmpl	$1999, %ebx
	jg	.L17
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC0
	.cfi_def_cfa_offset 32
	call	disp_color_str
	incl	%ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L13
.L17:
	movw	$0, disp_pos
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC1
	.cfi_def_cfa_offset 32
	call	disp_color_str
	addl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	movl	28(%esp), %eax
	pushl	excep_msg(,%eax,4)
	.cfi_def_cfa_offset 32
	call	disp_color_str
	movl	$.LC2, (%esp)
	call	disp_str
	addl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC3
	.cfi_def_cfa_offset 32
	call	disp_color_str
	addl	$4, %esp
	.cfi_def_cfa_offset 28
	pushl	44(%esp)
	.cfi_def_cfa_offset 32
	call	disp_int
	addl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC4
	.cfi_def_cfa_offset 32
	call	disp_color_str
	addl	$4, %esp
	.cfi_def_cfa_offset 28
	pushl	40(%esp)
	.cfi_def_cfa_offset 32
	call	disp_int
	addl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC5
	.cfi_def_cfa_offset 32
	call	disp_color_str
	addl	$4, %esp
	.cfi_def_cfa_offset 28
	pushl	36(%esp)
	.cfi_def_cfa_offset 32
	call	disp_int
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	$-1, 20(%esp)
	jne	.L18
.L12:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L18:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$31
	.cfi_def_cfa_offset 28
	pushl	$.LC6
	.cfi_def_cfa_offset 32
	call	disp_color_str
	addl	$4, %esp
	.cfi_def_cfa_offset 28
	pushl	32(%esp)
	.cfi_def_cfa_offset 32
	call	disp_int
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L12
	.cfi_endproc
.LFE6:
	.size	exception_handler, .-exception_handler
	.section	.rodata.str1.1
.LC7:
	.string	"spurious_irq: "
.LC8:
	.string	"\n"
	.text
	.globl	spurious_irq
	.type	spurious_irq, @function
spurious_irq:
.LFB7:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC7
	.cfi_def_cfa_offset 32
	call	disp_str
	addl	$4, %esp
	.cfi_def_cfa_offset 28
	pushl	28(%esp)
	.cfi_def_cfa_offset 32
	call	disp_int
	movl	$.LC8, (%esp)
	call	disp_str
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE7:
	.size	spurious_irq, .-spurious_irq
	.globl	excep_msg
	.section	.rodata.str1.1
.LC9:
	.string	"#DE Divide Error"
.LC10:
	.string	"#DB RESERVED"
.LC11:
	.string	"\342\200\224  NMI Interrupt"
.LC12:
	.string	"#BP Breakpoint"
.LC13:
	.string	"#OF Overflow"
.LC14:
	.string	"#BR BOUND Range Exceeded"
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align 4
.LC15:
	.string	"#UD Invalid Opcode (Undefined Opcode)"
	.align 4
.LC16:
	.string	"#NM Device Not Available (No Math Coprocessor)"
	.section	.rodata.str1.1
.LC17:
	.string	"#DF Double Fault"
	.section	.rodata.str1.4
	.align 4
.LC18:
	.string	"    Coprocessor Segment Overrun (reserved)"
	.section	.rodata.str1.1
.LC19:
	.string	"#TS Invalid TSS"
.LC20:
	.string	"#NP Segment Not Present"
.LC21:
	.string	"#SS Stack-Segment Fault"
.LC22:
	.string	"#GP General Protection"
.LC23:
	.string	"#PF Page Fault"
	.section	.rodata.str1.4
	.align 4
.LC24:
	.string	"\342\200\224  (Intel reserved. Do not use.)"
	.align 4
.LC25:
	.string	"#MF x87 FPU Floating-Point Error (Math Fault)"
	.section	.rodata.str1.1
.LC26:
	.string	"#AC Alignment Check"
.LC27:
	.string	"#MC Machine Check"
	.section	.rodata.str1.4
	.align 4
.LC28:
	.string	"#XF SIMD Floating-Point Exception"
	.data
	.align 32
	.type	excep_msg, @object
	.size	excep_msg, 80
excep_msg:
	.long	.LC9
	.long	.LC10
	.long	.LC11
	.long	.LC12
	.long	.LC13
	.long	.LC14
	.long	.LC15
	.long	.LC16
	.long	.LC17
	.long	.LC18
	.long	.LC19
	.long	.LC20
	.long	.LC21
	.long	.LC22
	.long	.LC23
	.long	.LC24
	.long	.LC25
	.long	.LC26
	.long	.LC27
	.long	.LC28
	.align 32
	.type	gate_table_exceptions, @object
	.size	gate_table_exceptions, 160
gate_table_exceptions:
	.long	divide_error
	.byte	0
	.byte	0
	.zero	2
	.long	single_step_exception
	.byte	1
	.byte	0
	.zero	2
	.long	nmi
	.byte	2
	.byte	0
	.zero	2
	.long	breakpoint_exception
	.byte	3
	.byte	3
	.zero	2
	.long	overflow
	.byte	4
	.byte	3
	.zero	2
	.long	bounds_check
	.byte	5
	.byte	0
	.zero	2
	.long	inval_opcode
	.byte	6
	.byte	0
	.zero	2
	.long	copr_not_available
	.byte	7
	.byte	0
	.zero	2
	.long	double_fault
	.byte	8
	.byte	0
	.zero	2
	.long	copr_seg_overrun
	.byte	9
	.byte	0
	.zero	2
	.long	inval_tss
	.byte	10
	.byte	0
	.zero	2
	.long	segment_not_present
	.byte	11
	.byte	0
	.zero	2
	.long	stack_exception
	.byte	12
	.byte	0
	.zero	2
	.long	general_protection
	.byte	13
	.byte	0
	.zero	2
	.long	page_fault
	.byte	14
	.byte	0
	.zero	2
	.long	copr_error
	.byte	16
	.byte	0
	.zero	2
	.long	alignment_check
	.byte	17
	.byte	0
	.zero	2
	.long	machine_check
	.byte	18
	.byte	0
	.zero	2
	.long	simd_exception
	.byte	19
	.byte	0
	.zero	2
	.long	0
	.byte	0
	.byte	0
	.zero	2
	.align 32
	.type	gate_table_pic, @object
	.size	gate_table_pic, 136
gate_table_pic:
	.long	hwint00
	.byte	32
	.byte	0
	.zero	2
	.long	hwint01
	.byte	33
	.byte	0
	.zero	2
	.long	hwint02
	.byte	34
	.byte	0
	.zero	2
	.long	hwint03
	.byte	35
	.byte	0
	.zero	2
	.long	hwint04
	.byte	36
	.byte	0
	.zero	2
	.long	hwint05
	.byte	37
	.byte	0
	.zero	2
	.long	hwint06
	.byte	38
	.byte	0
	.zero	2
	.long	hwint07
	.byte	39
	.byte	0
	.zero	2
	.long	hwint08
	.byte	40
	.byte	0
	.zero	2
	.long	hwint09
	.byte	41
	.byte	0
	.zero	2
	.long	hwint10
	.byte	42
	.byte	0
	.zero	2
	.long	hwint11
	.byte	43
	.byte	0
	.zero	2
	.long	hwint12
	.byte	44
	.byte	0
	.zero	2
	.long	hwint13
	.byte	45
	.byte	0
	.zero	2
	.long	hwint14
	.byte	46
	.byte	0
	.zero	2
	.long	hwint15
	.byte	47
	.byte	0
	.zero	2
	.long	0
	.byte	0
	.byte	0
	.zero	2
	.ident	"GCC: (GNU) 8.2.0"
