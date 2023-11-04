#include "vga.h"

extern void isr_handler()
{
    vga_puts("Received Interrupt\n");
}