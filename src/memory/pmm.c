#include "pmm.h"
#include "../string.h"
#include "../video/vga.h"

static uint32_t *pmm_bitmap = (void *)0;
static size_t pmm_total_frames = 0;
static size_t pmm_used_frames = 0;
static size_t pmm_last_index;

extern uintptr_t _kernel_phys_end[];

static void mmap_set(size_t bit)
{
    pmm_bitmap[bit / 32] |= (1 << (bit % 32));
}

static void mmap_unset(size_t bit)
{
    pmm_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static int mmap_test(size_t bit)
{
    return pmm_bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_init(mboot_header_T *mbi)
{
    // Get total memory size
    uintptr_t highest_addr = 0;
    mboot_mmap_T *mmap = (mboot_mmap_T *)mbi->mmap_addr;

    while ((uintptr_t)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1)
        {
            uint64_t region_end = mmap->base_low + mmap->size_low;

            if (region_end > highest_addr)
            {
                highest_addr = region_end;
            }
        }

        mmap = (mboot_mmap_T *)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
    }

    pmm_total_frames = highest_addr / FRAME_SIZE;

    size_t bitmap_size = pmm_total_frames / 8;

    // Find a place for the bitmap itself (right after the kernel)
    pmm_bitmap = (uint32_t *)(((uint32_t)_kernel_phys_end + 3) & ~3);

    memset(pmm_bitmap, 0xFF, bitmap_size);

    pmm_used_frames = pmm_total_frames;

    // Mark available memory regions as free iterating mmap again
    mmap = (mboot_mmap_T *)mbi->mmap_addr;
    while ((uintptr_t)mmap < (uintptr_t)mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1)
        {

            uintptr_t addr = mmap->base_low;
            size_t len = mmap->size_low;

            size_t region_base_frame = addr / FRAME_SIZE;
            size_t region_num_frames = len / FRAME_SIZE;

            for (size_t i = 0; i < region_num_frames; ++i)
            {
                if ((region_base_frame + i) < pmm_total_frames)
                {
                    mmap_unset(region_base_frame + i);
                    pmm_used_frames--;
                }
            }
        }
        mmap = (mboot_mmap_T *)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
    }

    // Mark the kernel and bitmap regions as USED
    size_t kernel_start_frame = 0x100000 / FRAME_SIZE;
    size_t kernel_end_frame = ((uintptr_t)pmm_bitmap / FRAME_SIZE);

    size_t bitmap_frames = (bitmap_size + FRAME_SIZE - 1) / FRAME_SIZE;

    for (size_t i = kernel_start_frame; i < kernel_end_frame; ++i)
    {
        if (!mmap_test(i))
        {
            mmap_set(i);
            pmm_used_frames++;
        }
    }

    for (size_t i = 0; i < bitmap_frames; ++i)
    {
        mmap_set(kernel_end_frame + i);
        pmm_used_frames++;
    }
}

void *pmm_alloc_frame()
{
    // First loop: search from the last known index to the end
    for (size_t i = pmm_last_index; i < pmm_total_frames; ++i)
    {
        if (!mmap_test(i))
        {
            pmm_used_frames++;
            pmm_last_index = i + 1;
            return (void *)(i * FRAME_SIZE);
        }
    }

    // Second loop (wrap-around): search from the beginning to the last known index
    for (size_t i = 0; i < pmm_last_index; i++)
    {
        if (!mmap_test(i))
        {
            mmap_set(i);
            pmm_used_frames++;
            pmm_last_index = i + 1;
            return (void *)(i * FRAME_SIZE);
        }
    }

    // If we get here, there's no free memory
    return NULL;
}

void pmm_free_frame(void *paddr)
{
    if (paddr == NULL)
        return;

    size_t frame_index = (uintptr_t)paddr / FRAME_SIZE;
    if (frame_index >= pmm_total_frames)
        return;

    if (mmap_test(frame_index))
    {
        mmap_unset(frame_index);
        pmm_used_frames--;

        if (frame_index < pmm_last_index)
            pmm_last_index = frame_index;
    }
}