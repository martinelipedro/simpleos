#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "util.h"

void keyboard_callback(registers_T regs);

int tick = 0;

void kmain()
{
    init_gdt();

    init_idt();
    register_interrupt_handler(IRQ1, keyboard_callback);

    const char *string = "Hello, World!";

    vga_init();
    vga_puts(string);

    asm volatile("int $0x3");
    vga_puts(string);

    while (1)
        ;
}

void keyboard_callback(registers_T regs)
{
    uint8_t scancode = inb(0x60);
    vga_puts("Key Pressed");
}