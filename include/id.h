#pragma once

#include <stdint.h>

static uint32_t id_main(void) {
  uint32_t ret;
  asm ("mrc p15, 0, %[ret], c0, c0, 0" : [ret] "=r" (ret));
  return ret;
}

static uint32_t id_cache_type(void) {
  uint32_t ret;
  asm ("mrc p15, 0, %[ret], c0, c0, 1" : [ret] "=r" (ret));
  return ret;
}

static uint32_t id_tlb_type(void) {
  uint32_t ret;
  asm ("mrc p15, 0, %[ret], c0, c0, 3" : [ret] "=r" (ret));
  return ret;
}
