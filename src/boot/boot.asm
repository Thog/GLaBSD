extern _start_virtual
extern setup_pages
extern __va_start__

global _start
global page_flush
global jump_to_virtual
global page_directory
global _start_debug

; page directory
section .data
align 0x1000
page_directory:
    dd 0x83
    times (1023) dd 0

; stack setup
section .bss
align 32
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .boot
_start:

    ; quick setup a temp stack
    lea ecx, [stack_top]
    sub ecx, __va_start__
    mov esp, ecx

    ; backup multiboot2 info
    push eax
    push ebx

    ; calculate page_directory physical position
    lea ecx, [page_directory]
    sub ecx, __va_start__

    ; calculate page directory index of the virtual address
    lea eax, [__va_start__]
    shr eax, 22

    ; setup the virtual page
    mov ebx, 0x83
    mov [ecx + eax * 4], ebx

    ; restore multiboot2 info
    pop ebx
    pop eax

    ; load the page directory base
    mov cr3, ecx

    ; enable 4MB paging support
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    ; enable paging
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    ; setup virtual stack
    mov esp, stack_top

    ; jump to the second stage
    lea ecx, [_start_virtual]
    jmp ecx