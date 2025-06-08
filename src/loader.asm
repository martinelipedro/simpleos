%define ALIGN    1 << 0
%define MEMINFO  1 << 1
%define FLAGS    (ALIGN | MEMINFO)
%define MAGIC    0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)
%define KERNEL_VIRT_BASE 0xC0000000

[bits 32]
section .multiboot
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .boot.text
    global _start
    extern kmain
    
_start:
    ; We are now running at physical address ~0x100000. Paging is OFF.
    cli ; Disable interrupts until our IDT is ready.

    ; CRITICAL: Save the multiboot info from GRUB before it gets clobbered.
    ; We'll use two "safe" registers, ecx and edx, to hold the values.
    mov ecx, ebx  ; ecx = pointer to multiboot info struct
    mov edx, eax  ; edx = multiboot magic number

    ; Calculate the physical address of our paging structures.
    mov ebx, page_directory
    sub ebx, KERNEL_VIRT_BASE ; ebx now holds the physical address

    ; Call the paging setup routine, passing the physical address.
    push ebx
    call setup_paging
    pop ebx

    ; Load CR3 with the PHYSICAL address of the page directory.
    mov cr3, ebx
    
    ; Enable paging by setting the PG bit (bit 31) in CR0.
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    ; Far jump to our higher-half code to reload CS and EIP.
    jmp 0x08:high_start

high_start:
    ; We are now running in the higher half at a virtual address ~0xC000xxxx.
    
    ; Set up the stack pointer to its new, high-memory location.
    mov esp, stack_top

    ; Now, push the saved multiboot info and magic number onto the new stack
    ; and call our C kernel's main function.
    push edx  ; Push the magic number
    push ecx  ; Push the multiboot info pointer
    call kmain

    ; If kmain ever returns, hang the system.
hang:
    cli
    hlt
    jmp hang

; This function sets up the initial page tables.
; It expects the physical address of the page directory in [esp+4].
setup_paging:
    mov edi, [esp+4] ; edi = physical address of page_directory

    ; Calculate the physical address of the first page table.
    mov esi, page_table1
    sub esi, KERNEL_VIRT_BASE ; esi = physical address of page_table1

    ; Map the first page table into the page directory for both identity and higher-half mapping.
    mov eax, esi
    or eax, 0x3 ; Present, Read/Write
    mov [edi], eax ; PDE[0] -> maps virtual 0x0...
    mov [edi + 768 * 4], eax ; PDE[768] -> maps virtual 0xC0000000...

    ; Now, fill in the page table itself (which maps the first 4MB of physical RAM).
    mov edi, esi ; edi = physical address of page_table1
    mov ecx, 0   ; Loop counter
.map_loop:
    mov eax, ecx
    imul eax, 0x1000 ; Address = index * 4096
    or eax, 0x3      ; Present, Read/Write
    mov [edi + ecx * 4], eax ; Map the page
    
    inc ecx
    cmp ecx, 1024
    jne .map_loop
    
    ret


section .bss
    align 4096 ; Page tables must be page-aligned
page_directory:
    resb 4096
page_table1:
    resb 4096

stack_bottom:
    resb 4 * 1024 * 16 ; 64KB stack
stack_top: