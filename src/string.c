#include "string.h"

void *memset(void *s, int val, uint32_t len)
{
    uint8_t *dst = s;
    while (len > 0)
    {
        *dst = (uint8_t)val;
        dst++;
        len--;
    }
    return s;
}