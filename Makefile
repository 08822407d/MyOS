#linux64 = 0
macos64 = 0

ENTRYPOINT	= 0x0
ENTRYOFFSET	=   0x0
#-----------------------------------------------------------------------------------------
# Programs, flags, etc.
ASM		= nasm
DASM		= ndisasm
ifdef linux64
	M32CFLAG = -m32
	M32LDFLAG = -m elf_i386
endif
ifdef macos64
	CROSSCOMPILE = i386-elf-
endif
CC		= $(CROSSCOMPILE)gcc
LD		= $(CROSSCOMPILE)ld
LASMFLAGS	= -I boot/include/
KASMFLAGS	= -I include/ -f elf
CFLAGS		= $(M32CFLAG) -I include/ -c -fno-builtin
LDFLAGS		= $(M32LDFLAG) -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)
INCLUDEFLAGS =
#-----------------------------------------------------------------------------------------
# This Program
BOOTSEC	= boot/boot.bin boot/loader.bin
KERNEL	= kernel.bin

ASMOBJS		= kernel/kern_a.o kernel/interrupt_a.o lib/klib_a.o lib/string_a.o

COBJS		= kernel/global.o kernel/main.o kernel/init.o \
				kernel/interrupt_c.o kernel/protect.o kernel/i8259_pic.o \
				lib/klib_c.o kernel/proc.o

OBJS		= $(ASMOBJS) $(COBJS)

DASMOUTPUT	= kernel.bin.asm

# All Phony Targets
.PHONY : every final clean disasm all buildimg 

every : $(ORANGESBOOT) $(ORANGESKERNEL)

all : clean every

clean :
	rm -f $(OBJS) $(COBJS:.o=.d)

# We assume that "a.img" exists in current folder
buildimg :
	dd if=bootsec.bin of=X86ASM.vhd bs=512 count=1 seek=0 conv=notrunc
	dd if=loader.bin of=X86ASM.vhd bs=512 count=4 seek=1 conv=notrunc
	dd if=kernel.bin of=X86ASM.vhd bs=512 count=64 seek=5 conv=notrunc

bootsec.bin : boot/bootsec.asm boot/bootrc.inc
	$(ASM) $(LASMFLAGS) -o $@ $<

loader.bin : boot/loader.asm boot/bootrc.inc boot/include_RM.inc boot/include_PM.inc 
	$(ASM) $(LASMFLAGS) -o $@ $<

kernel.bin : $(KObjs)
	$(LD) $(LDFLAGS) -o kernel.bin $(KObjs)

$(AObjs) : %.o : %.S
	$(ASM) $(KASMFLAGS) -o $@ $<

$(COBJS) : %.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(COBJS:.o=.d) : %.d : %.c
	@set -e; rm -f $@; \
         $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

include $(COBJS:.o=.d)