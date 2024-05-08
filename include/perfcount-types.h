#ifndef XX
#error "Must define XX to include this file"
#endif

XX(0x00, icache_miss)
XX(0x01, frontend_stall)
XX(0x02, true_stall)
XX(0x03, iutlb_miss)
XX(0x04, dutlb_miss)
XX(0x05, branch_count)
XX(0x06, branch_mispredict)
XX(0x07, instr_count)
XX(0x09, dcache_count)
XX(0x0b, dcache_miss)
XX(0x0c, dcache_wb)
XX(0x0d, pc_change)
XX(0x0f, tlb_miss)
XX(0x10, data_count)
XX(0x11, lsu_stall)
XX(0x12, sb_drain_count)
XX(0x23, call_count)
XX(0x24, ret_count)
XX(0x25, ras_predict)
XX(0x26, ras_mispredict)
XX(0xff, cycle_count)

#undef XX
