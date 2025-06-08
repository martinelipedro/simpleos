#ifndef SIMPLEOS_PMM_H
#define SIMPLEOS_PMM_H

#include "../multiboot.h"
#include <stddef.h>

#define FRAME_SIZE 4096

void pmm_init(mboot_header_T *mbi);
void *pmm_alloc_frame();
void pmm_free_frame(void *paddr);

#endif