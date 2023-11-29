#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"
#include "multiboot.h"
#include "mem/pmm.h"
#include "mem/vmm.h"

static void panic(const char *error_message);

// In KBytes
static uint32_t memory_size = 40000;

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_table[1024] __attribute__((aligned(4096)));

extern void load_dir(uint32_t *);
extern void enablep(void);

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

    for (uint16_t i = 0; i < 1024; ++i)
    {
        page_directory[i] = 0x00000002;
    }

    for (uint32_t i = 0; i < 1024; ++i)
    {
        page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = ((uint32_t)page_table) | 3;

    load_dir(page_directory);
    enablep();

    vga_puts("\nAt Paging World!");

    while (1)
        ;
}

static void panic(const char *error_message)
{
    vga_puts(error_message);
    while (1)
        ;
}