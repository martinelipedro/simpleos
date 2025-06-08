/*
 * linker.ld - Linker Script for a Higher-Half Kernel
 * This script sets the VMA (Virtual Memory Address) high and the
 * LMA (Load Memory Address) low, as required by the boot.s code.
 */

ENTRY(_start)

SECTIONS
{
    /* Set the starting Virtual Memory Address (VMA) to 3GB + 1MB */
    . = 1M + 0xC0000000;
    
    _kernel_virt_start = .;

    /*
     * The '.boot' section contains the multiboot header and the early assembly.
     * The AT() directive sets the Load Memory Address (LMA). We tell the
     * linker to load this section at its virtual address minus the 3GB offset,
     * which results in a physical load address of 1MB.
     */
    .boot ALIGN(4K) : AT(ADDR(.boot) - 0xC0000000)
    {
        /* The multiboot header must be at the very start of the file. */
        *(.multiboot)
        /* Followed by our early boot assembly code. */
        *(.boot.text)
    }

    /* Standard .text section for compiled C and Assembly code. */
    .text ALIGN(4K) :
    {
        *(.text)
    }
    
    /* Read-only data section. */
    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    /* Initialized data section. */
    .data ALIGN(4K) :
    {
        *(.data)
    }

    /* Uninitialized data section (BSS). */
    .bss ALIGN(4K) :
    {
        *(.bss)
        *(COMMON)
    }

    _kernel_virt_end = .;

    /*
     * Define a symbol that gives the physical end address of the kernel.
     * This is crucial for the Physical Memory Manager (PMM) to know
     * which memory frames are already occupied by the kernel itself.
     */
    _kernel_phys_end = _kernel_virt_end - 0xC0000000;
}