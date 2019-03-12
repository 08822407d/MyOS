ENTRYPOINT	= 0x0
ENTRYOFFSET	=   0x0
#-----------------------------------------------------------------------------------------
# Programs, flags, etc.
ASM			= nasm
DASM		= ndisasm
CROSSCOMPILE = i386-elf-
CC		= $(CROSSCOMPILE)gcc
LD		= $(CROSSCOMPILE)ld
LASMFLAGS	= -I boot/
KASMFLAGS	= -I include/ -f elf
CFLAGS		= -I include/ -c -fno-builtin -w
LDFLAGS		= -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)
INCFLAGS	=

#-----------------------------------------------------------------------------------------
# This Program
BOOTSEC	= bootsec.bin loader.bin
KERNEL	= kernel.bin

AOBJS		= kernel/kern_a.o kernel/asm.o kernel/interrupt_a.o lib/klib_a.o lib/string_a.o

COBJS		= kernel/global.o kernel/init.o kernel/kmain.o \
				kernel/i8259_pic.o kernel/interrupt_c.o kernel/protect.o \
				lib/klib_c.o

KOBJS		= $(AOBJS) $(COBJS)

#-----------------------------------------------------------------------------------------
# All Phony Targets
.PHONY : every clean all buildimg 

every : $(BOOTSEC) $(KERNEL)

all : clean every 

clean :
	rm -f $(KOBJS) $(COBJS:.o=.d)

# We assume that "a.img" exists in current folder
buildimg : clean
	dd if=bootsec.bin of=../myos.vhd bs=512 count=1 seek=0 conv=notrunc
	dd if=loader.bin of=../myos.vhd bs=512 count=4 seek=1 conv=notrunc
	dd if=kernel.bin of=../myos.vhd bs=512 count=64 seek=5 conv=notrunc
	
#-----------------------------------------------------------------------------------------
# Rules
bootsec.bin : boot/bootsec.S boot/bootrc.inc
	$(ASM) $(LASMFLAGS) -o $@ $<

loader.bin : boot/loader.S boot/bootrc.inc boot/include_RM.inc boot/include_PM.inc 
	$(ASM) $(LASMFLAGS) -o $@ $<

$(KERNEL) : $(KOBJS)
	$(LD) $(LDFLAGS) -o $@ $(KOBJS)

$(AOBJS) : %.o : %.S
	$(ASM) $(KASMFLAGS) -o $@ $<

$(COBJS) : %.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(COBJS:.o=.d) : %.d : %.c
	@set -e; rm -f $@; \
         $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

include $(COBJS:.o=.d)

