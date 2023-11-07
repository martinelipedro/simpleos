#ifndef SIMPLEOS_GDT_H
#define SIMPLEOS_GDT_H

#include <stdint.h>

struct GDT_ENTRY_STRUCT
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;

} __attribute__((packed));
typedef struct GDT_ENTRY_STRUCT gdt_entry_T;

struct GDT_POINTER_STRUCT
{
    uint16_t limit;
    uint32_t base;

} __attribute__((packed));
typedef struct GDT_POINTER_STRUCT gdt_pointer_T;

void init_gdt(void);

#endif