#include <stdint.h>
#include "vga.h"

void kmain()
{
    uint8_t *video_memory = (uint8_t *)0xb8000;
    const char *string = "Hello, World!";

    vga_init();
    vga_putc('H');
    vga_putc('e');

    while (1)
        ;
}