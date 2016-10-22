HOST = $(shell ./default-host.sh)
AR = $(HOST)-ar
AS = $(HOST)-as
PREFIX = /usr
EXEC_PREFIX = /usr
BOOTDIR = /boot
LIBDIR = $(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include
CC = ${HOST}-gcc --sysroot=$(PWD)/sysroot -isystem=$(INCLUDEDIR)
AS = ${HOST}-as
CFLAGS = -O2 -g
export

headers:
	@mkdir -p sysroot
	@(DESTDIR="${PWD}/sysroot" make -C libc install-headers)
	@(DESTDIR="${PWD}/sysroot" make -C kernel install-headers)

install: headers
	@(DESTDIR="$(PWD)/sysroot" make -C libc install)
	@(DESTDIR="$(PWD)/sysroot" make -C kernel install)

iso: install
	@mkdir -p isodir
	@mkdir -p isodir/boot
	@mkdir -p isodir/boot/grub

	@echo "global> Preparing ISO..."
	@cp sysroot/boot/glabsd.kernel isodir/boot/glabsd.kernel
	@echo "menuentry \"GLaBSD\" {multiboot /boot/glabsd.kernel}" > isodir/boot/grub/grub.cfg
	@echo "global> Creating ISO..."
	@grub-mkrescue -o glabsd.iso isodir
	@echo "global> Build complete!"

clean:
	@(DESTDIR="$PWD/sysroot" make -C libc clean)
	@(DESTDIR="$PWD/sysroot" make -C kernel clean)
	@rm -rfv sysroot
	@rm -rfv isodir
	@rm -rfv glabsd.iso

all: iso
re: clean all

.PHONY: clean re