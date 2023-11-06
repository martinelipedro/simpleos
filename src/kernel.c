#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "util.h"
#include "keyboard.h"

void kmain()
{
    init_gdt();
    init_idt();
    init_keyboard();

    vga_init();
    vga_puts("Hello, World!");

    asm volatile("int $0x3");

    while (1)
        ;
}
