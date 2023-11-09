#ifndef SIMPLEOS_MULTIBOOT_H
#define SIMPLEOS_MULTIBOOT_H

#include <stdint.h>

#define MBOOT_MAGIC 0x1badb002
#define CHECKFLAG(flag, bit) (flag >> bit & 0x1)

struct MULTIBOOT_HEADER_STRUCT
{
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;
} __attribute__((packed));
typedef struct MULTIBOOT_HEADER_STRUCT mboot_header_T;

struct MULTIBOOT_MEMORY_MAP_STRUCT
{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;

} __attribute__((packed));
typedef struct MULTIBOOT_MEMORY_MAP_STRUCT mboot_mmap_T;

mboot_mmap_T *get_memory_map(mboot_header_T *mbt);

#endif