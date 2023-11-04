#include <stdint.h>
#include "vga.h"
#include "gdt.h"

void kmain()
{
    init_gdt();

    // uint8_t *video_memory = (uint8_t *)0xb8000;
    const char *string = "Hello, World!";

    vga_init();
    vga_puts(string);

    while (1)
        ;
}