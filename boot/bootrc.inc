;==================================================================================================
;程序说明（2017/12/25）
;
;之前计划的版本有些复杂，到后来自己的实力跟不上，没法debug，只好照着成熟的设计简化自己的结构
;==================================================================================================
;---------------------------------------------------------------------------
;定义文件尺寸变量(扇区为单位)
FileSize_loader             EQU     0x04                ;loader固定4扇区
FileSize_kernel             EQU     0x64                ;kernel暂定16扇区
Sec_kernel_LBA              EQU     0x05                ;kernel文件在硬盘中存放的起始扇区
;实模式下的参数
;一些启动用的常量
RM_Seg_boot                 EQU     0x7c0               ;MBR加载到物理地址0x7c00
RM_Seg_SysInfo              EQU     0x9000              ;然后bootsec把自己复制到0x90000处
RM_base_loader              EQU     0x9020              ;loader起始段基址
RM_base_stack               EQU     0x9100              ;（系统）栈段基址
RM_off_MBRPE                EQU     0x1BE               ;分区表起始偏移

;一些loader使用的常数
Phys_base_Stack	            EQU		0x91000				;栈底0x90000，向下扩展的栈

Phys_CursorPos              EQU     0x90000             ;正式用途是存放光标位置，启动阶段用于临时存放
                                                        ;最后一条字符串的行列数
RM_off_RamMap               EQU     0x40                ;INT15 E820获取的RAM分布表写入的位置
RM_off_RamMap_Nr            EQU     0x0E                ;E820的记录条目个数写入偏移0xE处的2字节内
RM_off_DiskInfo             EQU     0x1D0               ;IVT 0x41和0x46指向的硬盘信息复制到此处


;机器信息表存放的内容和格式同LINUX，前64字节预留给各种硬件参数，从偏移0x40起预留240字节，
;用于另加入由E820功能读取的内存分布信息（20字节一条记录，预留12条记录的空间），结尾处0x1D0的2个16字节存放IVT中41h
;和46h指向的主硬盘和次硬盘信息表，然后是根设备号

;---------------------------------------------------------------------------
;保护模式下使用的参数
;PM下一些段的基址
_segbase_SysStack           EQU             0x98000         ;系统栈段,向下扩展段中，基址字段实际充当上界
                                                            ;界限字段是esp数值的最小值界限
_segbase_PTE                EQU             0x100000        ;PTE段
_segbase_GlbCode            EQU             0x0             ;全局代码段
_segbase_GlbData            EQU             0x0             ;全局数据段
_segbase_VgaRAM             EQU             0xB8000         ;VGA字符模式显存段基址

;PM下一些段的偏移
_off_MachinInfo             EQU             0x90000         ;同LINUX机器信息放在此处
_off_LoaderSetup            EQU             0x90200         ;全局段中loader的偏移
_off_PDE                    EQU             0x9D000         ;PDE的起始物理地址
_off_PTE                    EQU             0x100000        ;PTE的起始物理地址接着
_off_KernelRead             EQU             0x500000        ;内核文件被放置的位置
_off_KernelArrange          EQU             0x0             ;内核文件被重定位的位置

;---------------------------------------------------------------------------
;ELF_header_32各项偏移值
elf32_h_ident             EQU             0x0
elf32_h_type              EQU             elf32_h_ident + 0x10
elf32_h_machine           EQU             elf32_h_type + 0x02
elf32_h_version           EQU             elf32_h_machine + 0x02
elf32_h_entry             EQU             elf32_h_version + 0x04
elf32_h_phoff             EQU             elf32_h_entry + 0x04
elf32_h_shoff             EQU             elf32_h_phoff + 0x04
elf32_h_flages            EQU             elf32_h_shoff + 0x04
elf32_h_ehsize            EQU             elf32_h_flages + 0x04
elf32_h_phentsize         EQU             elf32_h_ehsize + 0x02
elf32_h_phnum             EQU             elf32_h_phentsize + 0x02
elf32_h_shentsize         EQU             elf32_h_phnum + 0x02
elf32_h_shnum             EQU             elf32_h_shentsize + 0x02
elf32_h_shstrndx          EQU             elf32_h_shnum + 0x02

;program_header(ELF_32)各项偏移值
elf32_ph_type              EQU             0x0
elf32_ph_offset            EQU             elf32_ph_type + 0x04
elf32_ph_vaddr             EQU             elf32_ph_offset + 0x04
elf32_ph_paddr             EQU             elf32_ph_vaddr + 0x04
elf32_ph_filesz            EQU             elf32_ph_paddr + 0x04
elf32_ph_memsz             EQU             elf32_ph_filesz + 0x04
elf32_ph_flags             EQU             elf32_ph_memsz + 0x04
elf32_ph_align             EQU             elf32_ph_flags + 0x04
;---------------------------------------------------------------------------
;==================================================================================================
; 描述符类型值说明
; 其中:
;       DA_  : 描述符属性
;       D    : 数据段
;       C    : 代码段
;       S    : 系统段
;       R    : 只读
;       RW   : 读写
;       A    : 已访问
;		uw	 :向上扩展
;		dw	 :向下扩展的
;       其它 : 可按照字面意思理解
;----------------------------------------------------------------------------
DA_32		EQU	4000h		; 32 位段
DA_Gr_4K	EQU	8000h		; 段界限粒度为 4K 字节

DA_DPL0		EQU	  00h		; DPL = 0
DA_DPL1		EQU	  20h		; DPL = 1
DA_DPL2		EQU	  40h		; DPL = 2
DA_DPL3		EQU	  60h		; DPL = 3
;----------------------------------------------------------------------------
; 存储段描述符类型值说明
;----------------------------------------------------------------------------
DA_DRu		EQU	90h			; 存在的只读数据段类型值	向上扩展
DA_DRWu		EQU	92h			; 存在的可读写数据段属性值	向上扩展
DA_DRd		EQU	94h			; 存在的只读数据段类型值	向下扩展
DA_DRWd		EQU	96h			; 存在的可读写数据段属性值	向下扩展
DA_C		EQU	98h			; 存在的只执行代码段属性值
DA_CR		EQU	9Ah			; 存在的可执行可读代码段属性值
DA_CCO		EQU	9Ch			; 存在的只执行一致代码段属性值
DA_CCOR		EQU	9Eh			; 存在的可执行可读一致代码段属性值
;----------------------------------------------------------------------------
; 系统段描述符类型值说明
;----------------------------------------------------------------------------
DA_LDT		EQU	  82h		; 局部描述符表段类型值
DA_TaskGate	EQU	  85h		; 任务门类型值
DA_386TSS	EQU	  89h		; 可用 386 任务状态段类型值
DA_386CGate	EQU	  8Ch		; 386 调用门类型值
DA_386IGate	EQU	  8Eh		; 386 中断门类型值
DA_386TGate	EQU	  8Fh		; 386 陷阱门类型值
;----------------------------------------------------------------------------
; 选择子类型值说明
; 其中:
;       SA_  : Selector Attribute
SA_RPL0		EQU	0			; ┓
SA_RPL1		EQU	1			; ┣ RPL
SA_RPL2		EQU	2			; ┃
SA_RPL3		EQU	3			; ┛

SA_TIG		EQU	0			; ┓TI
SA_TIL		EQU	4			; ┛

;----------------------------------------------------------------------------
; 分页机制使用的常量说明
;----------------------------------------------------------------------------
PG_P		EQU	1			; 页存在属性位
PG_RWR		EQU	0			; R/W 属性位值, 读/执行
PG_RWW		EQU	2			; R/W 属性位值, 读/写/执行
PG_USS		EQU	0			; U/S 属性位值, 系统级
PG_USU		EQU	4			; U/S 属性位值, 用户级

;==================================================================================================
;MBR分区表,16字节
%macro MBRPE 0
    db  0
    db  0
    db  0
    db  0
    db  0
    db  0
    db  0
    db  0
    dd  0
    dd  0
%endmacro

;描述符定义宏，三个参数依次为段基址、段界限、段属性
%macro Descriptor 3
	dw	%2 & 0FFFFh									; 段界限 1							(2 字节)
	dw	%1 & 0FFFFh									; 段基址 1							(2 字节)
	db	(%1 >> 16) & 0FFh							; 段基址 2							(1 字节)
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)			; 属性 1 + 段界限 2 + 属性 2		 (2 字节)
	db	(%1 >> 24) & 0FFh							; 段基址 3							(1 字节)
%endmacro ; 共 8 字节
;
; 门定义宏，
%macro Gate 4
	dw	(%2 & 0FFFFh)								; 偏移 1							(2 字节)
	dw	%1											; 选择子							(2 字节)
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)			; 属性							    (2 字节)
	dw	((%2 >> 16) & 0FFFFh)						; 偏移 2							(2 字节)
%endmacro ; 共 8 字节
;==================================================================================================