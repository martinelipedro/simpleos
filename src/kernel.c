#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"
#include "multiboot.h"
#include "mem/pmm.h"

static void panic(const char *error_message);

// In KBytes
static uint32_t memory_size = 40000;

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

    pmm_init(memory_size, (uint32_t *)(0xFFFFF * 512));
    pmm_init_regions((mboot_mmap_T *)info->mmap_addr);

    vga_putc('\n');
    vga_puth32((uint32_t)pmm_alloc_block());

    while (1);
}

static void panic(const char *error_message)
{
    vga_puts(error_message);
    while (1);
}