#include "string.h"

void *memset(void *s, int val, uint32_t len)
{
    uint8_t *dst = s;
    while (len--)
    {
        *dst++ = (uint8_t)val;
    }
    return s;
}