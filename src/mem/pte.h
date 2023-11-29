#ifndef SIMPLEOS_PTE_H
#define SIMPLEOS_PTE_H

#include <stdint.h>

enum PAGING_PTE_FLAGS
{
    PTE_F_PRESENT = 0x1,
    PTE_F_WRITABLE = 0x2,
    PTE_F_USER = 0x4,
    PTE_F_WRITETOUGH = 0x8,
    PTE_F_NOT_CACHEABLE = 0x10,
    PTE_F_ACCESSED = 0x20,
    PTE_F_DIRTY = 0x40,
    PTE_F_PAT = 0x80,
    PTE_F_CPU_GLOBAL = 0x100,
    PTE_F_LV4_GLOBAL = 0x200,
    PTE_F_FRAME = 0x7FFFF000,
};

typedef uint32_t pte_T;

void pte_add_attrib(pte_T *entry, uint32_t attr_mask);
void pte_remove_attrib(pte_T *entry, uint32_t attr_mask);
void pte_set_frame(pte_T *entry, uint32_t address);
int pte_is_present(pte_T entry);
int pte_is_writable(pte_T entry);
uint32_t pte_physical_addr(pte_T entry);

#endif