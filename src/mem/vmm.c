#include "vmm.h"

#include "pmm.h"
#include "../video/vga.h"
#include "../string.h"

page_dir_T *current_directory = 0;

int vmm_alloc_page(pte_T *entry)
{
    void *physical_frame = pmm_alloc_block();
    if (!physical_frame)
        return 0;

    pte_set_frame(entry, (uint32_t)physical_frame);
    pte_add_attrib(entry, PTE_F_PRESENT);
}

void vmm_free_page(pte_T *entry)
{
    void *physical_frame = (void *)pte_physical_addr(*entry);
    if (physical_frame)
        pmm_free_block(physical_frame);

    pte_remove_attrib(entry, PTE_F_PRESENT);
}

pte_T *vmm_page_table_get_entry(page_table_T *table, virtual_addr addr)
{
    if (table)
        return &table->entries[PAGE_TABLE_INDEX(addr)];

    return 0;
}

pde_T *vmm_page_directory_get_entry(page_dir_T *directory, virtual_addr addr)
{
    if (directory)
        return &directory->entries[PAGE_DIRECTORY_INDEX(addr)];

    return 0;
}

extern void load_dir(uint32_t directory);

int vmm_switch_page_directory(page_dir_T *directory)
{
    if (!directory)
        return 0;

    current_directory = directory;
    load_dir((uint32_t)current_directory);
    return 1;
}

page_dir_T *vmm_get_page_directory(void)
{
    return current_directory;
}

void vmm_map_page(void *phys_addr, void *virt_addr)
{
    page_dir_T *directory = vmm_get_page_directory();

    pte_T *entry = &directory->entries[PAGE_DIRECTORY_INDEX((uint32_t)virt_addr)];
    if (((*entry) & PTE_F_PRESENT) != PTE_F_PRESENT)
    {
        // Page table not present
        page_table_T *table = (page_table_T *)pmm_alloc_block();
        if (!table)
            return;

        memset(table, 0x0, sizeof(page_table_T));

        pde_T *dir_entry = &directory->entries[PAGE_DIRECTORY_INDEX((uint32_t)virt_addr)];

        pde_add_attrib(dir_entry, PDE_F_PRESENT);
        pde_add_attrib(dir_entry, PDE_F_WRITABLE);
        pde_set_frame(dir_entry, (uint32_t)table);
    }

    page_table_T *table = (page_table_T *)PAGE_GET_PHYSICAL_ADDRESS(entry);

    pte_T *page = &table->entries[PAGE_TABLE_INDEX((uint32_t)virt_addr)];

    pte_set_frame(page, (uint32_t)phys_addr);
    pte_add_attrib(page, PTE_F_PRESENT);
}

extern void enablep(void);

void vmm_enable_paging(void)
{
    enablep();
}

void vmm_initialize(void)
{
    page_table_T *table = (page_table_T *)pmm_alloc_block();
    if (!table)
    {
        return;
    }

    page_table_T *table2 = (page_table_T *)pmm_alloc_block();
    if (!table2)
        return;

    // Clear the page table
    memset(table, 0, sizeof(page_table_T));
    for (int i = 0, frame = 0x0, virtual = 0x00000000; i < 1024; ++i, frame += 4096, virtual += 4096)
    {
        pte_T page = 0;
        pte_add_attrib(&page, PTE_F_PRESENT);
        pte_set_frame(&page, frame);

        table2->entries[PAGE_TABLE_INDEX(virtual)] = page;
    }

    for (int i = 0, frame = 0x100000, virtual = 0xc0000000; i < 1024; i++, frame += 4096, virtual += 4096)
    {

        pte_T page = 0;
        pte_add_attrib(&page, PTE_F_PRESENT);
        pte_set_frame(&page, frame);

        table->entries[PAGE_TABLE_INDEX(virtual)] = page;
    }

    page_dir_T *directory = (page_dir_T *)pmm_alloc_blocks(3);
    if (!directory)
        return;

    memset(directory, 0, sizeof(page_dir_T));

    pde_T *entry = &directory->entries[PAGE_DIRECTORY_INDEX(0xc0000000)];
    pde_add_attrib(entry, PDE_F_PRESENT);
    pde_add_attrib(entry, PDE_F_WRITABLE);
    pde_set_frame(entry, (uint32_t)table);

    pde_T *entry2 = &directory->entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    pde_add_attrib(entry2, PDE_F_PRESENT);
    pde_add_attrib(entry2, PDE_F_WRITABLE);
    pde_set_frame(entry2, (uint32_t)table2);

    current_directory = (page_dir_T *)&directory->entries;
    vmm_switch_page_directory(directory);

    vmm_enable_paging();
}