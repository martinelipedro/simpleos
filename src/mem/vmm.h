#ifndef SIMPLEOS_VMM_H
#define SIMPLEOS_VMM_H

#include <stdint.h>
#include "pte.h"
#include "pde.h"

#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIRECTORY 1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) ((*x) & ~0xfff)

// In KBytes
#define PAGE_TABLE_ADDR_SPACE_SIZE 0x400000        // 4MB
#define PAGE_DIRECTORY_ADDR_SPACE_SIZE 0x100000000 // 4GB

// IN KBytes (4K)
#define PAGE_SIZE 4096

typedef uint32_t virtual_addr;

typedef struct PAGE_TABLE_STRUCT
{
    pte_T entries[PAGES_PER_TABLE];

} page_table_T;

typedef struct PAGE_DIRECTORY_STRUCT
{
    pde_T entries[PAGES_PER_DIRECTORY];

} page_dir_T;

int vmm_alloc_page(pte_T *entry);
void vmm_free_page(pte_T *entry);

pte_T *vmm_page_table_get_entry(page_table_T *table, virtual_addr addr);
pde_T *vmm_page_directory_get_entry(page_dir_T *directory, virtual_addr addr);
int vmm_switch_page_directory(page_dir_T *directory);
page_dir_T *vmm_get_page_directory(void);

void vmm_map_page(void *phys_addr, void *virt_addr);
void vmm_enable_paging(void);

void vmm_initialize(void);

#endif