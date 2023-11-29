#ifndef SIMPLEOS_PDE_H
#define SIMPLEOS_PDE_H

#include <stdint.h>

enum PAGING_PDE_FLAGS
{
    PDE_F_PRESENT = 0x1,
    PDE_F_WRITABLE = 0x2,
    PDE_F_USER = 0x4,
    PDE_F_PWT = 0x8,
    PDE_F_PCD = 0x10,
    PDE_F_ACCESSED = 0x20,
    PDE_F_DIRTY = 0x40,
    PDE_F_4MB = 0x80,
    PDE_F_LV4_GLOBAL = 0x200,
    PDE_F_CPU_GLOBAL = 0x100,
    PDE_F_FRAME = 0x7FFFF000,
};

typedef uint32_t pde_T;

void pde_add_attrib(pde_T *entry, uint32_t attr_mask);
void pde_del_attrib(pde_T *entry, uint32_t attr_mask);
void pde_set_frame(pde_T *entry, uint32_t address);
int pde_is_present(pde_T entry);
int pde_is_user(pde_T entry);
int pde_is_4mb(pde_T entry);
int pde_is_writable(pde_T entry);
uint32_t pde_physical_addr(pde_T entry);
void pde_enable_global(pde_T entry);

#endif