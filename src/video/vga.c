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
    current_row = 0;
    current_column = 0;
}

void vga_putc(char c)
{
    switch (c)
    {
    case '\n':
        current_row++;
        current_column = 0;
        break;

    default:
        const uint16_t index = (VGA_COLS * current_row) + current_column;
        vga_buffer[index] = (((uint16_t)current_color << 8) | c);
        current_column++;
        break;
    }
}

void vga_puts(const char *string)
{
    for (uint16_t i = 0; string[i] != '\0'; ++i)
    {
        vga_putc(string[i]);
    }
}

static char hexadecimal_index[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void vga_puth(uint8_t hex)
{
    vga_puts("0x");
    vga_putc(hexadecimal_index[(hex & 0xF0) >> 4]);
    vga_putc(hexadecimal_index[(hex & 0x0F)]);
    vga_putc('\n');
}

void vga_puth32(uint32_t hex)
{
    vga_puts("0x");
    vga_putc(hexadecimal_index[(hex & 0xF0000000) >> 28]);
    vga_putc(hexadecimal_index[(hex & 0xF000000) >> 24]);
    vga_putc(hexadecimal_index[(hex & 0xF00000) >> 20]);
    vga_putc(hexadecimal_index[(hex & 0xF0000) >> 16]);
    vga_putc(hexadecimal_index[(hex & 0xF000) >> 12]);
    vga_putc(hexadecimal_index[(hex & 0xF00) >> 8]);
    vga_putc(hexadecimal_index[(hex & 0xF0) >> 4]);
    vga_putc(hexadecimal_index[(hex & 0x0F)]);
    vga_putc('\n');
}

void vga_puth64(uint64_t hex)
{
    vga_puts("0x");
    vga_putc(hexadecimal_index[(hex & 0xF000000000000000) >> 60]);
    vga_putc(hexadecimal_index[(hex & 0xF00000000000000) >> 56]);
    vga_putc(hexadecimal_index[(hex & 0xF0000000000000) >> 52]);
    vga_putc(hexadecimal_index[(hex & 0xF000000000000) >> 48]);
    vga_putc(hexadecimal_index[(hex & 0xF00000000000) >> 44]);
    vga_putc(hexadecimal_index[(hex & 0xF0000000000) >> 40]);
    vga_putc(hexadecimal_index[(hex & 0xF000000000) >> 36]);
    vga_putc(hexadecimal_index[(hex & 0xF00000000) >> 32]);
    vga_putc(hexadecimal_index[(hex & 0xF0000000) >> 28]);
    vga_putc(hexadecimal_index[(hex & 0xF000000) >> 24]);
    vga_putc(hexadecimal_index[(hex & 0xF00000) >> 20]);
    vga_putc(hexadecimal_index[(hex & 0xF0000) >> 16]);
    vga_putc(hexadecimal_index[(hex & 0xF000) >> 12]);
    vga_putc(hexadecimal_index[(hex & 0xF00) >> 8]);
    vga_putc(hexadecimal_index[(hex & 0xF0) >> 4]);
    vga_putc(hexadecimal_index[(hex & 0x0F)]);
    vga_putc('\n');
}