#ifndef SIMPLEOS_PMM_H
#define SIMPLEOS_PMM_H

#include <stdint.h>
#include "../multiboot.h"

// Each bit
#define PMM_BLOCKS_PER_BYTE 8

// 4096 bytes (4kb) blocks
#define PMM_BLOCK_SIZE 4096

// Block alignment
#define PMM_BLOCK_ALIGN PMM_BLOCK_SIZE

// Entry count on mmap array
#define PMM_TOTAL_ENTRIES 32

void pmm_mmap_set(int bit);
void pmm_mmap_unset(int bit);
int pmm_mmap_test(int bit);

int pmm_mmap_get_first_free();

void pmm_init(uint32_t mem_size, uint32_t *bitmap);
void pmm_init_regions(mboot_mmap_T *mmap);
void pmm_init_region(uint32_t base_addr, uint32_t size);
void pmm_deinit_region(uint32_t base_addr, uint32_t size);

void *pmm_alloc_block();
void *pmm_alloc_blocks(uint32_t size);
void pmm_free_block(void *paddr);

extern void load_pdbr(uint32_t address);
extern uint32_t get_pdbr(void);

#endif