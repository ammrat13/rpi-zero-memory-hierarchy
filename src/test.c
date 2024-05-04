#include "test.h"
#include "uart.h"

void run_test(volatile uint32_t *dut) {
  uart_puts("arr_size,arr_stride,step_count,elapsed_usec\n");

  // Loop over all the array sizes from 1KiB to 256MiB
  for (size_t arr_size = 256 * 1024 * 1024; arr_size >= 1 * 1024; arr_size >>= 1) {
    // Loop over all strides from 4 up to the array size
    for (size_t arr_stride = 4; arr_stride < arr_size; arr_stride <<= 1) {
      // Check for timeout every 256MiB of data, and keep track of how many
      // "steps" elapsed
      const size_t step = 256 * 1024 * 1024 / 4;
      const size_t timeout_usec = 90000000;
      size_t step_count = 0;
      size_t elapsed_usec = 0;

      // Warm up the caches before doing the real test
      run_test_asm(dut, arr_size - 1, arr_stride, arr_size / 4);

      // Run the real test
      while (elapsed_usec < timeout_usec) {
        step_count++;
        elapsed_usec += run_test_asm(dut, arr_size - 1, arr_stride, step);
      }
      uart_printf("%u,%u,%u,%u\n", arr_size, arr_stride, step_count, elapsed_usec);
    }
  }
}
