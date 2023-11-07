#include "multiboot.h"

// mboot_mmap_T *get_memory_map(mboot_header_T *mbt)
// {
//     uint32_t i = 0;
//     mboot_mmap_T *mmap = (mboot_mmap_T *)mbt->mmap_addr;
//     uint32_t map_limit = mbt->mmap_addr + mbt->mmap_length;

//     while ((uint32_t)mmap < map_limit)
//     {
//         mem_map[i].size = mmap->size;
//         mem_map[i].length = mmap->length;
//         mem_map[i].base_addr = mmap->base_addr;
//         mem_map[i].type = mmap->type;
//         i++;
//         mmap = (mboot_mmap_T *)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
//     }

//     return (mboot_mmap_T *)&mem_map;
// }