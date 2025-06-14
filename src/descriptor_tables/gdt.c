#include "gdt.h"

gdt_entry_T gdt_entries[3];
gdt_pointer_T gdt_pointer;

static void gdt_set_entry(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

extern void gdt_flush(uint32_t gdtp);

void init_gdt(void)
{
    gdt_pointer.limit = (sizeof(gdt_entry_T) * 3) - 1;
    gdt_pointer.base = (uint32_t)&gdt_entries;

    gdt_set_entry(0, 0, 0, 0, 0);                // Null segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    // gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    // gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_pointer);
}

// Eu amo minha namorada :)

static void gdt_set_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt_entry_T *entry = &gdt_entries[index];

    entry->base_low = (base & 0xFFFF);
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->limit_low = (limit & 0xFFFF);
    entry->granularity = ((limit >> 16) & 0x0F);

    entry->granularity |= granularity & 0xF0;
    entry->access = access;
}