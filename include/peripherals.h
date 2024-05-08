#pragma once

#include <stdint.h>

typedef uint32_t device_register_t;

static uint32_t device_read(device_register_t addr) {
  return *(volatile uint32_t *)addr;
}

static void device_write(device_register_t addr, uint32_t data) {
  *(volatile uint32_t *)addr = data;
}

static void device_rmw(device_register_t addr, uint32_t mask, uint32_t value) {
  device_write(addr, (device_read(addr) & ~mask) | value);
}

static inline void device_barrier() {
  uint32_t scratch;
  asm volatile (
    "mcr p15, 0, %[scratch], c7, c10, 4\n"
    : [scratch] "=r" (scratch) :: "memory");
}
