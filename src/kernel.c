#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"
#include "multiboot.h"
#include "memory/pmm.h"

static void panic(const char *error_message);

void kmain(mboot_header_T *info, uint32_t magic)
{
    init_gdt();

    init_idt();
    init_keyboard();

    vga_init();

    if (magic != 0x1badb002)
    {
        panic("Invalid multiboot magic!");
    }

    if (!CHECKFLAG(info->flags, 6))
    {
        panic("Invalid multiboot info provided!");
    }

    pmm_init(info);
    void *test = pmm_alloc_frame();

    vga_puts("--- Starting PMM Test---\n");
    void *initial_frames[10];
    for (int i = 0; i < 10; ++i)
    {
        initial_frames[i] = pmm_alloc_frame();
        if (initial_frames[i] == NULL)
        {
            vga_puts("Test failed.\n");
            panic("PMM ERROR");
        }
    }

    vga_puts("\nExhaust all remaining memory...");
    size_t allocs = 0;

    while (pmm_alloc_frame() != NULL)
    {
        allocs++;
    }

    vga_puts("\nMemory exhausted with x allocations: ");
    vga_puth32(allocs);

    while (1)
        ;
}

static void panic(const char *error_message)
{
    vga_puts(error_message);
    while (1)
        ;
}