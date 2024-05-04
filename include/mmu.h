#pragma once

#include <stdint.h>

void mmu_init(void);
void mmu_set_attributes(uint32_t ttbr0, uint32_t dac);
void mmu_enable(void);
