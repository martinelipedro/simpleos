#ifndef SIMPLEOS_VGA_H
#define SIMPLEOS_VGA_H

#define VGA_ROWS 25
#define VGA_COLS 80

void vga_init(void);
void vga_clear(void);
void vga_putc(char c);
void vga_puts(const char *string);

#endif