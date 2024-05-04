#pragma once

#include <stddef.h>
#include <stdint.h>

void run_test(volatile uint32_t *dut);
uint32_t run_test_asm(volatile uint32_t *dut, size_t arr_mask, size_t arr_stride, size_t count);
