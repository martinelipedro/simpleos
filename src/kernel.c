#include <stdint.h>
#include "vga.h"

void kmain()
{
    uint8_t *video_memory = (uint8_t *)0xb8000;
    const char *string = "Hello, World!";

    vga_init();

    for (uint8_t i = 0; string[i] != '\0'; ++i)
    {
        video_memory[i * 2] = string[i];
        video_memory[i * 2 + 1] = 0xF;
    }

    while (1)
        ;
}