#ifndef SIMPLEOS_VGA_H
#define SIMPLEOS_VGA_H

#define VGA_ROWS 25
#define VGA_COLS 80

#include <stdint.h>

void vga_init(void);
void vga_clear(void);
void vga_putc(char c);
void vga_puts(const char *string);
void vga_puth(uint8_t hex);
void vga_puth32(uint32_t hex);

#endif