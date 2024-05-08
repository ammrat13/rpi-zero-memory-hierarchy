#pragma once

#include <stdint.h>

typedef enum perfcount_event_t {
#  define XX(num, name) PERFCOUNT_EVENT_ ## name = num,
#  include "perfcount-types.h"
} perfcount_event_t;

const char *perfcount_event_name(perfcount_event_t event);

static inline void perfcount_set(perfcount_event_t event0, perfcount_event_t event1) {

  uint32_t val;
  asm ("mrc p15, 0, %[val], c15, c12, 0\n" : [val] "=r" (val) ::);

  val &= 0xf0000ffe;
  val |= event0 << 20;
  val |= event1 << 12;
  val |= 1;

  asm volatile (
    "mcr p15, 0, %[val], c15, c12, 0\n"
    "mcr p15, 0, r0, c7, c5, 4\n"
    :: [val] "r" (val) :);
}

static inline perfcount_event_t perfcount_get_event0(void) {
  uint32_t val;
  asm ("mrc p15, 0, %[val], c15, c12, 0\n" : [val] "=r" (val) ::);
  return (val >> 20) & 0xff;
}

static inline perfcount_event_t perfcount_get_event1(void) {
  uint32_t val;
  asm ("mrc p15, 0, %[val], c15, c12, 0\n" : [val] "=r" (val) ::);
  return (val >> 12) & 0xff;
}

static inline uint32_t perfcount_get_cr0(void) {
  uint32_t ret;
  asm volatile ("mrc p15, 0, %[ret], c15, c12, 2\n" : [ret] "=r" (ret) ::);
  return ret;
}

static inline uint32_t perfcount_get_cr1(void) {
  uint32_t ret;
  asm volatile ("mrc p15, 0, %[ret], c15, c12, 3\n" : [ret] "=r" (ret) ::);
  return ret;
}
