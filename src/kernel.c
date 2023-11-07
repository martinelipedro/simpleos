#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"
#include "multiboot.h"

static void panic(const char *error_message);

void kmain(mboot_header_T *info)
{
    init_gdt();
    init_idt();
    init_keyboard();

    vga_init();
    if (!(info->flags >> 6 & 0x1))
    {
        panic("Invalid multiboot info provided!");
    }

    // vga_puth32(info->boot_device);

    while (1)
        ;
}

static void panic(const char *error_message)
{
    vga_puts(error_message);
    while (1)
        ;
}