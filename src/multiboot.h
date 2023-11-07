#ifndef SIMPLEOS_MULTIBOOT_H
#define SIMPLEOS_MULTIBOOT_H

#include <stdint.h>

struct MULTIBOOT_MEMORY_MAP_STRUCT
{
    uint32_t size;
    uint32_t base_addr;
    uint32_t length;
    uint32_t type;

} __attribute__((packed));
typedef struct MULTIBOOT_MEMORY_MAP_STRUCT mboot_memmap_T;

#endif