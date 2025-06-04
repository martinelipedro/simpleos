#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"
#include "multiboot.h"

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

    while (1)
        ;
}

static void panic(const char *error_message)
{
    vga_puts(error_message);
    while (1)
        ;
}