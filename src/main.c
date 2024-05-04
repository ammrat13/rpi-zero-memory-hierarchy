#include <stddef.h>

#include "id.h"
#include "uart.h"
#include "mmu.h"

#include "test.h"

static volatile uint32_t *const translation_table = (void *)0x100000;
static volatile uint32_t *const dut = (void *)0x1000000;

int main(void) {

  // Initialize all the hardware
  uart_init();
  mmu_init();

  // Print a message to let us know that we're alive, then dump ID registers
  uart_puts("Booted!\n");
  uart_printf("Main ID Register: %x\n", id_main());
  uart_printf("Cache Type Register: %x\n", id_cache_type());
  uart_printf("TLB Type Register: %x\n", id_tlb_type());

  // Setup the translation table. We have this at a fixed address right after
  // the stack.
  {
    // Initially, zero out the table
    for (size_t i = 0; i < 4096; i++)
      translation_table[i] = 0;

    // Identity map the first two mebibytes. The first mebibyte is for the
    // kernel, and the second is for this translation table.
    for (size_t i = 0; i < 2; i++) {
      // Start off with the section base address
      uint32_t descriptor = i << 20;
      // Setup the options
      descriptor |= (0b0 << 15) | (0b11 << 10); // Full access
      descriptor |= (0b0 << 16) | (0b001 << 12) | (0b11 << 2); // WBWA, non-sharable
      descriptor |= 0b10;
      // Write
      translation_table[i] = descriptor;
    }

    // Map the device registers. These are from 0x20000000 to 0x20ffffff.
    for (size_t i = 0x200; i < 0x300; i++) {
      // Start off with the section base address
      uint32_t descriptor = i << 20;
      // Setup the options
      descriptor |= (0b0 << 15) | (0b11 << 10); // Full access
      descriptor |= (0b0 << 16) | (0b000 << 12) | (0b01 << 2); // Shared device
      descriptor |= (0b1 << 18); // Supersection
      descriptor |= 0b10;
      // Write
      translation_table[i] = descriptor;
    }

    // Map the DUT buffer itself. It starts at 16MiB, and is 256MiB in size
    for (size_t i = 0x10; i < 0x110; i++) {
      // Start off with the section base address
      uint32_t descriptor = i << 20;
      // Setup the options
      descriptor |= (0b0 << 15) | (0b11 << 10); // Full access
      descriptor |= (0b0 << 16) | (0b001 << 12) | (0b11 << 2); // WBWA, non-sharable
      descriptor |= (0b1 << 18); // Supersection
      descriptor |= 0b10;
      // Write
      translation_table[i] = descriptor;
    }
  }
  // Tell the MMU about the translation table. The translation table itself is
  // in cacheable non-shared memory. Also, we stay in Domain 0.
  mmu_set_attributes(
    (uint32_t)translation_table | 0b01 << 3 | 0b1 << 0,
    0b01
  );
  // Turn the MMU on
  uart_puts("About to enable MMU...\n");
  mmu_enable();
  uart_puts("MMU enabled!\n");

  // Run the test
  run_test(dut);

  // We're done, so just hang
  uart_puts("Done!\n");
  return 0;
}
