linux64 = 0
#macos64 = 0

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
LASMFLAGS	= -I boot/
KASMFLAGS	= -I include/ -f elf
CFLAGS		= $(M32CFLAG) -I include/ -c -fno-builtin -w -fno-stack-protector
LDFLAGS		= $(M32LDFLAG) -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)
INCLUDEFLAGS =
#-----------------------------------------------------------------------------------------
# This Program
BOOTSEC	= bootsec.bin loader.bin
KERNEL	= kernel.bin

ASMOBJS		= kernel/kern_a.o kernel/asm.o kernel/interrupt_a.o lib/klib_a.o lib/string_a.o
COBJS		= kernel/global.o kernel/kmain.o kernel/init.o \
				kernel/interrupt_c.o kernel/protect.o kernel/i8259_pic.o \
				lib/klib_c.o kernel/proc.o kernel/clock.o kernel/test_procs.o
KOBJS		= $(ASMOBJS) $(COBJS)

DASMOUTPUT	= kernel.bin.asm

# All Phony Targets
.PHONY : every clean all buildimg 

every : $(BOOTSEC) $(KERNEL)

all : every clean

clean :
	rm -f $(KOBJS) $(COBJS:.o=.d)

buildimg : clean
	dd if=bootsec.bin of=myos.vhd bs=512 count=1 seek=0 conv=notrunc
	dd if=loader.bin of=myos.vhd bs=512 count=4 seek=1 conv=notrunc
	dd if=kernel.bin of=myos.vhd bs=512 count=256 seek=5 conv=notrunc

bootsec.bin : boot/bootsec.S boot/bootrc.inc
	$(ASM) $(LASMFLAGS) -o $@ $<

loader.bin : boot/loader.S boot/bootrc.inc boot/include_RM.inc boot/include_PM.inc 
	$(ASM) $(LASMFLAGS) -o $@ $<

kernel.bin : $(KOBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(KOBJS)

$(ASMOBJS) : %.o : %.S
	$(ASM) $(KASMFLAGS) -o $@ $<

$(COBJS) : %.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(COBJS:.o=.d) : %.d : %.c
	@set -e; rm -f $@; \
         $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

include $(COBJS:.o=.d)