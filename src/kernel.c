#include <stdint.h>

void kmain()
{
    uint8_t *video_memory = (uint8_t *)0xb8000;
    const char *string = "Hello, World!";

    for (uint16_t i = 0; i < 80 * 25; ++i)
    {
        video_memory[i * 2] = 0;
        video_memory[i * 2 + 1] = 0;
    }

    for (uint8_t i = 0; string[i] != '\0'; ++i)
    {
        video_memory[i * 2] = string[i];
        video_memory[i * 2 + 1] = 0xF;
    }

    while (1)
        ;
}