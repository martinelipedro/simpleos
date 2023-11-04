#include "vga.h"

#include <stdint.h>

volatile uint16_t *vga_buffer = (uint16_t *)0xb8000;

void vga_init(void)
{
    vga_clear();
}

void vga_clear(void)
{
    for (uint16_t i = 0; i < VGA_COLS * VGA_ROWS; ++i)
    {
        vga_buffer[i] = 0;
    }
}
