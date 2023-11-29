#include "pmm.h"

#include "../string.h"
#include "../video/vga.h"

static uint32_t pmm_memory_size = 0;
static uint32_t pmm_used_blocks = 0;
static uint32_t pmm_max_blocks = 0;
static uint32_t *pmm_memory_map = 0;

void pmm_mmap_set(int bit)
{
    pmm_memory_map[bit / PMM_TOTAL_ENTRIES] |= (1 << (bit % PMM_TOTAL_ENTRIES));
}

void pmm_mmap_unset(int bit)
{
    pmm_memory_map[bit / PMM_TOTAL_ENTRIES] |= ~(1 << (bit % PMM_TOTAL_ENTRIES));
}

int pmm_mmap_test(int bit)
{
    return pmm_memory_map[bit / PMM_TOTAL_ENTRIES] & (1 << (bit % PMM_TOTAL_ENTRIES));
}

#include "../video/vga.h"

int pmm_mmap_get_first_free()
{
    for (uint32_t i = 0; i < pmm_max_blocks / 32; ++i)
    {
        if (pmm_memory_map[i] != 0xffffffff)
        {
            for (int j = 0; j < 32; ++j)
            {
                int bit = 1 << j;
                if (!(pmm_memory_map[i] & bit))
                {
                    // bit integer * n of bytes in int * n blocks per byte * bit index;
                    uint32_t value = i * 4 * PMM_BLOCKS_PER_BYTE + j;
                    vga_puth(j);
                    return value;
                }
            }
        }
    }
}

void pmm_init(uint32_t mem_size, uint32_t *bitmap)
{
    vga_puts("Initialising physical memory with: ");
    vga_puth32(mem_size);
    vga_puts(" Kb");

    pmm_memory_size = mem_size;
    pmm_memory_map = bitmap;
    pmm_max_blocks = mem_size * 1024;
    pmm_used_blocks = pmm_max_blocks;

    memset(pmm_memory_map, 0xF, pmm_max_blocks / PMM_BLOCKS_PER_BYTE);
}

void pmm_init_region(uint32_t base_addr, uint32_t size)
{
    uint32_t align = base_addr / PMM_BLOCK_SIZE;
    uint32_t blocks = size / PMM_BLOCK_SIZE;

    for (; blocks > 0; --blocks)
    {
        pmm_mmap_unset(align++);
        pmm_used_blocks--;
    }

    pmm_mmap_set(0);
}

void pmm_deinit_region(uint32_t base_addr, uint32_t size)
{
    uint32_t align = base_addr / PMM_BLOCK_SIZE;
    uint32_t blocks = size / PMM_BLOCK_SIZE;

    for (; blocks > 0; --blocks)
    {
        pmm_mmap_set(align++);
        pmm_used_blocks++;
    }
}

void *pmm_alloc_block()
{
    uint32_t free_blocks = pmm_max_blocks - pmm_used_blocks;
    if (free_blocks < 0)
        return 0;

    uint32_t block_frame = pmm_mmap_get_first_free();
    if (block_frame == -1)
        return 0;

    pmm_mmap_set(block_frame);
    uint32_t address = block_frame * PMM_BLOCK_SIZE;
    pmm_used_blocks++;

    return (void *)address;
}

void pmm_free_block(void *paddr)
{
    uint32_t address = (uint32_t)paddr;
    int block_frame = address / PMM_BLOCK_SIZE;

    pmm_mmap_unset(block_frame);
    pmm_used_blocks--;
}

void pmm_init_regions(mboot_mmap_T *mmap)
{
    for (uint8_t i = 0; i < 15; ++i)
    {
        // Sanity Check: Mark it Reserved
        if (mmap[i].type > 4)
            mmap[i].type = 1;

        // No more entries
        if (i > 0 && mmap[i].base_low == 0)
            break;

        if (mmap[i].type == 1)
            pmm_init_region(mmap[i].base_low, mmap[i].size_low);
    }
}

void *pmm_alloc_blocks(uint32_t size)
{

    if (pmm_max_blocks - pmm_used_blocks <= size)
        return 0; // not enough space

    int frame = pmm_mmap_get_first_free(size);

    if (frame == -1)
        return 0; // not enough space

    for (uint32_t i = 0; i < size; i++)
        pmm_mmap_set(frame + i);

    uint32_t addr = frame * PMM_BLOCK_SIZE;
    pmm_used_blocks += size;

    return (void *)addr;
}