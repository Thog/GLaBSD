NAME=moe
SRCDIRS =  $(wildcard src/*)

$(foreach dir,$(SRCDIRS),$(eval include $(dir)/make.config))

ARCH = i686-elf
YASM_FORMAT = elf32

CC = clang
CXX = clang++
AS = llvm-mc

#TODO: Switch to ld.lld
LD = $(ARCH)-gcc
YASM = yasm
CC_FLAGS = -target i686-pc-none-elf -march=i686 -nostdlib -nostdlibinc -Wall -Wextra -Iincludes -std=gnu11 -g -ffreestanding -D SERIAL_LOGGING
CFLAGS=$(CC_FLAGS)
LDFLAGS = -nostdlib -T linker.ld 

%.o: %.c
	@echo "kernel> Building $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.S
	@echo "kernel> Building $<"
	@$(AS) -c $< -o $@

%.o: %.asm
	@echo "kernel> Building $<"
	@$(YASM) -g dwarf2 -f $(YASM_FORMAT) $< -o $@

$(NAME).bin: $(OBJS)
	@echo "kernel> Linking"
	@$(LD) $(LDFLAGS) -o $@ $(OBJS) -lgcc

all: iso

clean:
	@rm -f $(OBJS) $(NAME).bin $(NAME).iso
	@rm -rf isodir

iso: $(NAME).bin
	@mkdir -p isodir
	@mkdir -p isodir/boot
	@mkdir -p isodir/boot/grub

	@echo "Preparing ISO..."
	@cp $(NAME).bin isodir/boot/
	@echo "menuentry \"MOE\" {multiboot2 /boot/$(NAME).bin}" > isodir/boot/grub/grub.cfg
	@echo "Creating ISO..."
	@grub-mkrescue -o $(NAME).iso isodir
	@echo "Build complete!"

re: clean all
