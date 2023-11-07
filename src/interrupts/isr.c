#include "isr.h"

#include "../video/vga.h"

#include "../util.h"
#include <stdint.h>

static isr_T interrupt_handlers[256];

extern void isr_handler()
{
    vga_puts("Received Interrupt\n");
}

extern void irq_handler(int_registers_T regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_T handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t index, isr_T handler)
{
    interrupt_handlers[index] = handler;
}
