NAME=moe
SRCDIRS = boot src

$(foreach dir,$(SRCDIRS),$(eval include $(dir)/make.config))

ARCH = i686-elf
YASM_FORMAT = elf32

CC = $(ARCH)-gcc
AS = $(ARCH)-gcc
YASM = yasm
CFLAGS = -Iincludes -std=gnu11 -g -ffreestanding -Wall -Wextra
LDFLAGS = -n -T linker.ld -ffreestanding -O2 -nostdlib -lgcc -Wl,--print-map

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
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

all: $(NAME).bin

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