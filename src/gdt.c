#include "gdt.h"

gdt_entry_T gdt_entries[5];
gdt_pointer_T gdt_pointer;

static void gdt_set_entry(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

extern void gdt_flush(void);

void init_gdt(void)
{
    gdt_pointer.limit = (sizeof(gdt_entry_T) * 5) - 1;
    gdt_pointer.base = (uint32_t)&gdt_entries;

    gdt_set_entry(0, 0, 0, 0, 0);                // Null segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush();
}

static void gdt_set_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt_entries[index].base_low = (base & 0xFFFF);
    gdt_entries[index].base_middle = (base >> 16) & 0xFF;
    gdt_entries[index].base_high = (base >> 24) & 0xFF;

    gdt_entries[index].limit_low = (limit & 0xFFFF);
    gdt_entries[index].granularity = ((limit >> 16) & 0x0F);

    gdt_entries[index].granularity |= granularity & 0xF0;
    gdt_entries[index].access = access;
}