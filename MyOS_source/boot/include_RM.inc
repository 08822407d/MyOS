;==================================================================================================
;实模式用到的一些函数，单独放一个文件里
;==================================================================================================
;利用BIOS INT0x10显示一个字符串
;调用者设置CX（字符串长度）、ES：BP（字符串地址）、DX（DH行号，DL列号）
str_print:
    mov ax,0x1301
    mov bx,0x0007                       ;0页，白底黑字
    int 0x10
    ret
;---------------------------------------------------------------------------
;INT0x15 E820获取内存分布信息
;ES:DI指向存储信息的缓冲区（0x9000：0x40），ES:SI指向存储RamMap项数的地址（0x9000：0x0E）
E820:
    push es

    xor edi,edi
    xor esi,esi
    mov ax,RM_Seg_SysInfo
    mov es,ax
    mov di,RM_off_RamMap
    mov si,RM_off_RamMap_Nr
    xor ebx,ebx

    mov WORD [es:si],0
.l0:
    mov eax,0xE820                          ;INT0x15 E820号功能
    mov ecx,0x14                            ;一条记录占20字节
    mov edx,0x534D4150                      ;验证内容‘SAMP’
    int 0x15
    jc .check_fail
    add di,0x14
    inc WORD [es:si]                        ;0x0E处存放的条目个数+1
    cmp ebx,0
    jne .l0
    jmp .j0
.check_fail:                                ;如果CF置位则显示”读取信息出错“并死循环
    mov cx,Len_Msg1
    mov bp,Msg1
    mov dx,[fs:0]
    call str_print
    inc dh
    mov [fs:0],dx
    jmp $
.j0:                                        ;读取成功则返回主程序
    pop es

    ret
;---------------------------------------------------------------------------
Get_HD_Info:
    push ds
    push es
 
    mov ax,RM_Seg_SysInfo
    mov ds,ax
    mov es,ax
    xor si,si
    xor di,di

    mov ax,0
    mov ds,ax
    lds si,[4 * 0x41]                       ;获取硬盘1参数
    mov ax,RM_Seg_SysInfo
    mov es,ax
    mov di,RM_off_DiskInfo
    mov cx,0x10
    cld
    rep
    movsb

    mov ax,0
    mov ds,ax
    lds si,[4 * 0x46]                       ;获取硬盘2参数
    mov ax,RM_Seg_SysInfo
    mov es,ax
    mov di,(RM_off_DiskInfo + 0x10)
    mov cx,0x10
    cld
    rep
    movsb

    pop es
    pop ds

    ret
;==================================================================================================