;==================================================================================================
;保护模式下要使用的函数,另单独放一个文件里
;==================================================================================================
;从硬盘读取kernel,简易版的读硬盘功能
read_kernel_HDD:
    push ds
    push es

;写入要读取的扇区数目
    mov dx,0x01F2
    mov al,FileSize_kernel
    out dx,al
;写入LBA和参数
    xor eax,eax
    mov eax,Sec_kernel_LBA                          ;kernel的起始LBA写入eax
    xor edx,edx
    mov dl,0xE0                                     ;再将参数（11100000b）写入eax25-32位
    shl edx,24
    or eax,edx
    mov ecx,0x04
;把LBA和参数写入端口
    xor edx,edx
    mov dx,0x01F3
.l0:
    out dx,al
    inc dx
    shr eax,8
    loop .l0
;向端口写入读请求
    mov dx,0x01F7
    mov al,0x20
    out dx,al
;检查硬盘是否准备好读出数据
    mov dx,0x01F7
.waits:
    in al,dx
    and al,0x88
    cmp al,0x08
    jnz .waits
;以下开始读出数据
    mov ax,sele_GlbData
    mov es,ax
    mov edi,_off_KernelRead

    mov ax,FileSize_kernel
    mov bx,0x0100
    mul bx
    mov cx,dx
    shl ecx,16
    mov cx,ax

    mov dx,0x01F0
.l1:
    in ax,dx
    stosw
    loop .l1

    pop es
    pop ds

    ret
;---------------------------------------------------------------------------
;检查ELF头确认是ELF_32文件
;若两处检查任意不通过则报错并进入死循环
;ds:si指向e_ident，然后指向e_ehsize，es:di指向正确格式的信息
check_kfile_format:
    mov ax,sele_GlbData
    mov ds,ax
    mov es,ax
    mov esi,(_off_KernelRead + elf32_h_ident)         ;ELF格式的kernel文件elf头的ident位置
    mov eax,[ds:esi]
    mov edi,(_off_LoaderSetup + ELF_32_check)       ;loader中存放的检验信息的位置
    mov ebx,[es:edi]
    cmp dWORD eax,ebx
    jnz .err

    ret

.err:
    push dword (_off_LoaderSetup + Msg4)
    call printstr_tmp
    jmp $
;---------------------------------------------------------------------------
;安置elf32文件，void arrange_elf32(uchar* src_addr)
;esp->文件源地址 ;ebx=文件源地址 ;edx=phoff ;ecx(0)=phnum ;ecx(1)=filesize ;eax->phtype ;esi->phoffset ;edi->vaddr
arrange_elf32:
    pushad
    xor ecx,ecx                         ;所有非字宽大小的数据传送要用到的寄存器都清零
    xor edx,edx
    mov ebx,_off_KernelRead             ;ss:ebp指向参数一src_addr，参数赋给ebx
    mov edx,[ebx + elf32_h_phoff]       ;edx指向p_header_table起始，每读完一个段，更新edx指向下一条p_header
    add edx,ebx
    mov cx,[ebx + elf32_h_phnum]       ;cx(0)存储p_header数目,注意phnum是16位成员
.l0:
    mov eax,[ebx + elf32_h_type]        ;eax存储p_header_type
    cmp eax,0
    je .no_act                          ;ph_type = 0则不复制

    push ecx
    mov ecx,[edx + elf32_ph_filesz]     ;设置内存移动参数
    mov esi,[edx + elf32_ph_offset]
    add esi,_off_KernelRead
    mov edi,[edx + elf32_ph_vaddr]
    add edi,_off_KernelArrange
    rep
    movsb
    
    pop ecx
    add dx,[ebx + elf32_h_phentsize]
.no_act:
    loop .l0

    popad
    ret
;---------------------------------------------------------------------------
;利用RAMMap计算内存大小
calc_MemSize:
    push ds
    push esi
    push es
    push edi
    push ebx
    push ecx
    push edx
;根据RamMap的type计算内存大小
    mov edi,(_off_MachinInfo + RM_off_RamMap)       ;0x90040
    mov esi,(_off_MachinInfo + RM_off_RamMap_Nr)    ;0x9000E
    mov ecx,[ds:esi]                                ;把信息条目数提取出来
    mov esi,(_off_MachinInfo + 0x02)
.l1:
    cmp ecx,0
    je .end
    add edi,20                                      ;跳过必然是可用内存（IBMPC）的第一段（640KB)
    cmp dword [edi+16],0x01                         ;因为IBMPC前1MB的内存分布都是一样的（大概）
    jne .j1                                         ;所以从1MB开始寻找可用内存
    cmp dword [edi],0x100000
    je .j2
.j1:
    loop .l1
.j2:    
    mov ebx,[edi]                                   ;这里开始累加每段可用内存的大小
    add ebx,[edi + 8]
    mov [esi],ebx
    dec ecx
.l3:
    add edi,20
    cmp dword [edi + 16],0x01
    jne .j3
    add ebx,[edi + 8]
    mov [esi],ebx
.j3:
    loop .l3
    add ebx,0x10000                                 ;最末端的64KB总是用于BIOS映射
    mov [esi],ebx

.end:
    pop edx
    pop ecx
    pop ebx
    pop edi
    pop es
    pop esi
    pop ds
    ret
;---------------------------------------------------------------------------
;保护模式下临时的字符串显示功能
;一个参数：字符串地址（偏移量），压栈传参，C风格字符串结束
printstr_tmp:
    push ebp
    mov ebp,esp

    mov esi,[ebp + 8]                               ;字符串地址传送到esi
    mov di,[_off_MachinInfo + 0x00]                 ;将光标位置放入di
.l0:
    lodsb
    test al,al
    jz .l1
    cmp al,0x0A                                     ;判断是否回车
    jnz .l2
    push eax                                        ;是的话就更新显示位置
    mov ax,di
    mov bl,160
    div bl
    and ax,0xFF
    inc ax
    mov bl,160
    mul bl
    mov di,ax
    pop eax
    jmp .l0
.l2:
    mov ah,0x07
    mov [gs:di],ax
    add di,2
    jmp .l0
.l1:
    mov [_off_MachinInfo + 0x00],di                ;存入最新的光标位置

    pop ebp
    ret
;---------------------------------------------------------------------------
;安装PDE和PTE
setup_page:
    push ds
    push es

    mov esi,(_off_MachinInfo + 0x02)                ;0x90002处4字节存放内存大小
    xor edx,edx
    mov eax,[esi]
    mov ebx,0x400000
    div ebx
    mov ecx,eax                                     ;ecx存放页表数目（等于页目录中要初始化的条数）
    test edx,edx
    jz no_riminder
    inc ecx                                         ;有余数则多算一个页表
no_riminder:
    push ecx
;初始化PDE
    mov ax,sele_GlbData
    mov es,ax
    mov edi,_off_PDE
    mov eax,(_off_PTE | PG_P  | PG_USU | PG_RWW)
.l0:
    stosd
	add	eax,0x1000		                            ;PTE连续放置
	loop .l0
;初始化PTE
    mov ax,sele_GlbData
    mov es,ax
    mov edi,_off_PTE
    pop eax
    mov ebx,0x400
    mul ebx                                         ;计算页表项总数
    mov ecx,eax
    mov eax,(PG_P  | PG_USU | PG_RWW)
.l1:
    stosd
    add eax,0x1000
    loop .l1

    mov	eax,_off_PDE
	mov	cr3,eax
	mov	eax,cr0
	or	eax,0x80000000
	mov	cr0,eax
	jmp	short .j0
.j0:
	nop

    pop es
    pop ds
	ret
;==================================================================================================