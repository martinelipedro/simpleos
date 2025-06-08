#ifndef SIMPLEOS_PAGING_H
#define SIMPLEOS_PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../string.h"

#define PAGE_SIZE 0x1000

typedef struct PAGE_TABLE_ENTRY_STRUCT
{
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t pwt : 1;
    uint32_t pcd : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t unused : 5;
    uint32_t frame_addr : 20;

} __attribute__((packed)) pte_t;

typedef pte_t pde_t;

typedef struct
{
    pte_t entries[1024];
} __attribute__((aligned(PAGE_SIZE))) pagetable_t;

typedef struct
{
    pde_t entries[1024];
} __attribute__((aligned(PAGE_SIZE))) pagedir_t;

void paging_init();

#endif