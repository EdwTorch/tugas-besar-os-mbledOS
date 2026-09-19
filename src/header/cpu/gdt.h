#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include "interrupt.h"
// Some GDT Constant
#define GDT_MAX_ENTRY_COUNT 32
/**
 * As kernel SegmentDescriptor for code located at index 1 in GDT, 
 * segment selector is sizeof(SegmentDescriptor) * 1 = 0x8
*/ 
#define GDT_KERNEL_CODE_SEGMENT_SELECTOR 0x8
#define GDT_KERNEL_DATA_SEGMENT_SELECTOR 0x10

#define GDT_USER_CODE_SEGMENT_SELECTOR 0x18
#define GDT_USER_DATA_SEGMENT_SELECTOR 0x20
#define TASK_STATE_SEGMENT_SELECTOR 0x28

extern struct GDTR _gdt_gdtr;

/**
 * Segment Descriptor storing system segment information.
 * Struct defined exactly as Intel Manual Segment Descriptor definition (Figure 3-8 Segment Descriptor).
 * Manual can be downloaded at www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html/ 
 *
 * @param segment_low  16-bit lower-bit segment limit
 * @param base_low     16-bit lower-bit base address
 * @param base_mid     8-bit middle-bit base address
 * @param type_bit     4-bit contain type flags
 * @param non_system   1-bit contain system
 */
struct SegmentDescriptor {
    // First 32-bit
    uint16_t segment_low;
    uint16_t base_low;

    // Next 16-bit (Bit 32 to 47)
    uint8_t base_mid; //Base Address 16-23
    uint8_t type_bit   : 4; //Segment Type : (Code/Data/Read/Write)
    uint8_t non_system : 1; /* Descriptor Type : 0 = System; 1 = code / data */
    
    uint8_t DPL_bit : 2; /*Descriptor Previlege Level dari Level 0-3. Level 0 = Kernel, Level 3 = User */
    uint8_t segment_present_p_bit : 1; /*Set or Clear. Clear --> Not Valid [0], Set --> Valid/In Memory [1]*/
    
    uint8_t seg_high_limit_bit : 4; 
    uint8_t avl_bit : 1; /* Available for system usage*/
    uint8_t l_bit : 1;  /* Mode 32 Bit (0) dan Mode 64 bit (1)*/
    uint8_t db_bit : 1; /*  Default operation size (0 = 16-bit segment; 1 = 32-bit segment) */
    uint8_t granularity_bit : 1; /* (0 = Byte, 1 = 4KB page) */
    
    uint8_t base_high; //Base Address 24-31
    // TODO : Continue SegmentDescriptor definition (DONE YEY)
} __attribute__((packed));

/**
 * Global Descriptor Table containing list of segment descriptor. One GDT already defined in memory.c.
 * More details at https://wiki.osdev.org/GDT_Tutorial
 * @param table Fixed-width array of SegmentDescriptor with size GDT_MAX_ENTRY_COUNT
 */
struct GlobalDescriptorTable {
    struct SegmentDescriptor table[GDT_MAX_ENTRY_COUNT];
} __attribute__((packed));

/**
 * GDTR, carrying information where's the GDT located and GDT size.
 * Global kernel variable defined at memory.c.
 * 
 * @param size    Global Descriptor Table size, use sizeof operator
 * @param address GDT address, GDT should already defined properly
 */
struct GDTR {
    uint16_t                     size;
    struct GlobalDescriptorTable *address;
} __attribute__((packed));

void gdt_install_tss(void);
#endif
