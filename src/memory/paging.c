#include "paging.h"
#include "pmm.h"

volatile pagedir_t *kernel_dir = 0;

void paging_init()
{
    uintptr_t dir_phys = (uintptr_t)pmm_alloc_frame();
    volatile pagedir_t *dir = (volatile pagedir_t *)dir_phys;
    memset((void *)dir, 0, FRAME_SIZE);

    uintptr_t first_pt_phys = (uintptr_t)pmm_alloc_frame();
    volatile pagetable_t *first_pt = (volatile pagetable_t *)first_pt_phys;
    memset((void *)first_pt, 0, FRAME_SIZE);

    // Identity mapping on first 4MB
    for (uint32_t i = 0; i < 1024; ++i)
    {
        first_pt->entries[i].present = 1;
        first_pt->entries[i].rw = 1;
        first_pt->entries[i].frame_addr = i;
    }

    dir->entries[0].present = 1;
    dir->entries[0].rw = 1;
    dir->entries[0].frame_addr = first_pt_phys >> 12;

    dir->entries[768].present = 1;
    dir->entries[768].rw = 1;
    dir->entries[768].frame_addr - first_pt_phys >> 12;
}