#include"header/cpu/gdt.h"

/**
 * global_descriptor_table, predefined GDT.
 * Initial SegmentDescriptor already set properly according to Intel Manual & OSDev.
 * Table entry : [{Null Descriptor}, {Kernel Code}, {Kernel Data (variable, etc)}, ...].
 */
struct GlobalDescriptorTable global_descriptor_table = {
    .table = {
        /* Null Descriptor*/
        [0] = { 
            .segment_low =0,
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0,
            .non_system = 0,
            .DPL_bit = 0,
            .segment_present_p_bit = 0,
            .seg_high_limit_bit =0,
            .avl_bit =0,
            .l_bit =0,
            .db_bit =0,
            .granularity_bit =0,
            .base_high = 0
        },
        /* Kernel Code */
        [1] = {
            .segment_low =0xFFFF,
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0xA,
            .non_system = 1,
            .DPL_bit = 0,
            .segment_present_p_bit = 1,
            .seg_high_limit_bit =0xF,
            .avl_bit =0,
            .l_bit =0,
            .db_bit =1,
            .granularity_bit =1,
            .base_high = 0            
        },
        /* Kernel Data Segment*/
        [2] = {
            .segment_low =0xFFFF,
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0x2,
            .non_system = 1,
            .DPL_bit = 0,
            .segment_present_p_bit = 1,
            .seg_high_limit_bit =0xF,
            .avl_bit =0,
            .l_bit =0,
            .db_bit =1,
            .granularity_bit =1,
            .base_high = 0        
        },
        /* User Mode Code Segment */
        [3] = {
            .segment_low =0xFFFF,
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0xA,
            .non_system = 1,
            .DPL_bit = 0b11,
            .segment_present_p_bit = 1,
            .seg_high_limit_bit =0xF,
            .avl_bit =0,
            .l_bit =0,
            .db_bit =1,
            .granularity_bit =1,
            .base_high = 0
        },
        /* User Mode Data Segment */
        [4] = {
            .segment_low =0xFFFF,
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0x2,
            .non_system = 1,
            .DPL_bit = 0x3,
            .segment_present_p_bit = 1,
            .seg_high_limit_bit =0xF,
            .avl_bit =0,
            .l_bit =0,
            .db_bit =1,
            .granularity_bit =1,
            .base_high = 0
        },
        /* Task State Segment */
        [5] = {
            .segment_low = sizeof(struct TSSEntry),
            .base_low = 0,
            .base_mid = 0,
            .type_bit = 0x9,
            .non_system = 0,
            .DPL_bit = 0,
            .segment_present_p_bit = 1,
            .seg_high_limit_bit =((sizeof(struct  TSSEntry) >>16)&0xF),
            .avl_bit =0,
            .l_bit =0,
            .db_bit =0,
            .granularity_bit =0,
            .base_high = 0
        }
    }
};

/**
 * _gdt_gdtr, predefined system GDTR. 
 * GDT pointed by this variable is already set to point global_descriptor_table above.
 * From: https://wiki.osdev.org/Global_Descriptor_Table, GDTR.size is GDT size minus 1.
 */
struct GDTR _gdt_gdtr = {
    // TODO : Implement, this GDTR will point to global_descriptor_table. 
    //        Use sizeof operator
    .size = sizeof(global_descriptor_table)-1,
    .address  = &global_descriptor_table
};

void gdt_install_tss(void){
    uint32_t tss_base = (uint32_t)&_interrupt_tss_entry;

    global_descriptor_table.table[5].base_low = (tss_base & 0xFFFF);
    global_descriptor_table.table[5].base_mid = (tss_base>>16) & 0xFF;
    global_descriptor_table.table[5].base_high = (tss_base>>24)&0xFF;
    global_descriptor_table.table[5].segment_low = sizeof(struct TSSEntry) -1;

    _interrupt_tss_entry.ss0 = 0x10; // Kernel Data Selector (Index 2 -> Offset 0x10)
    _interrupt_tss_entry.IOmap_base_address = sizeof(struct TSSEntry);

    // 3. Load GDT (Instruksi lgdt)
    __asm__ __volatile__("lgdt %0" : : "m"(_gdt_gdtr));

    // 4. Flush Segment Registers & Reload CS via Far Jump
    __asm__ __volatile__(
        "mov $0x10, %%ax   \n" // Kernel Data Selector
        "mov %%ax, %%ds    \n"
        "mov %%ax, %%es    \n"
        "mov %%ax, %%fs    \n"
        "mov %%ax, %%gs    \n"
        "mov %%ax, %%ss    \n"
        "ljmp $0x08, $1f   \n" // Far Jump ke Kernel Code Selector (0x08)
        "1:                \n"
        : 
        : 
        : "ax", "memory"
    );

    // 5. Load Task Register / TSS (Instruksi ltr)
    // 0x28 adalah TSS Selector (Index 5 * 8)
    __asm__ __volatile__(
        "mov $0x28, %%ax   \n"
        "ltr %%ax          \n"
        : 
        : 
        : "ax"
    );
}
