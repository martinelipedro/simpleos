#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"

void kmain()
{
    init_gdt();
    init_idt();

    const char *string = "Hello, World!";

    vga_init();
    vga_puts(string);

    asm volatile("int $0x3");
    vga_puts(string);

    while (1)
        ;
}