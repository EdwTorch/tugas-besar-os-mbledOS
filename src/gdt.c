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

void gdt_install(void){
    // WIP
}
