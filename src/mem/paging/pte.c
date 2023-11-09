#include "pte.h"

void pte_add_attrib(pte_T *entry, uint32_t attr_mask)
{
    *(entry) |= attr_mask;
}

void pte_remove_attrib(pte_T *entry, uint32_t attr_mask)
{
    *(entry) &= ~attr_mask;
}

void pte_set_frame(pte_T *entry, uint32_t address)
{
    *(entry) |= ((*entry) & ~PTE_F_FRAME) | address;
}

int pte_is_present(pte_T entry)
{
    return entry & PTE_F_PRESENT;
}

int pte_is_writable(pte_T entry)
{
    return entry & PTE_F_WRITABLE;
}

uint32_t pte_physical_addr(pte_T entry)
{
    return entry & PTE_F_FRAME;
}