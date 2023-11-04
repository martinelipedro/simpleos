#include "vga.h"

#include <stdint.h>

volatile uint16_t *vga_buffer = (uint16_t *)0xb8000;
static uint8_t current_column = 0, current_row = 0;
static uint8_t current_color = 0x0F;

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

void vga_putc(char c)
{
    const uint16_t index = (VGA_COLS * current_row) + current_column;
    vga_buffer[index] = (((uint16_t)current_color << 8) | c);
    current_column++;
}

void vga_puts(const char *string)
{
    for (uint16_t i = 0; string[i] != '\0'; ++i)
    {
        vga_putc(string[i]);
    }
}