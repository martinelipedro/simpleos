#include "pde.h"

void pde_add_attrib(pde_T *entry, uint32_t attr_mask)
{
    (*entry) |= attr_mask;
}

void pde_del_attrib(pde_T *entry, uint32_t attr_mask)
{
    (*entry) &= ~attr_mask;
}

void pde_set_frame(pde_T *entry, uint32_t address)
{
    *(entry) |= ((*entry) & ~PDE_F_FRAME) | address;
}

int pde_is_present(pde_T entry)
{
    return entry & PDE_F_PRESENT;
}

int pde_is_user(pde_T entry)
{
    return entry & PDE_F_USER;
}

int pde_is_4mb(pde_T entry)
{
    return entry & PDE_F_4MB;
}

int pde_is_writable(pde_T entry)
{
    return entry & PDE_F_WRITABLE;
}

uint32_t pde_physical_addr(pde_T entry)
{
    return entry & PDE_F_FRAME;
}

void pde_enable_global(pde_T entry)
{
}
