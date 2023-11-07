#include <stdint.h>
#include "video/vga.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "interrupts/isr.h"
#include "util.h"
#include "drivers/keyboard.h"

void kmain()
{
    init_gdt();
    init_idt();
    init_keyboard();

    vga_init();
    vga_puts("Hello, World!");

    asm volatile("int $0x3");

    while (1);
}
