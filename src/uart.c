#include <stdbool.h>
#include <stdint.h>

#include "uart.h"
#include "peripherals.h"

// List the registers we will primarily be using
static const device_register_t AUX_MU_IO_REG = 0x20215040;
static const device_register_t AUX_MU_STAT_REG = 0x20215064;

void uart_init(void) {
  device_barrier();

  // Setup the GPIO pins for UART1. We use pins 14 and 15 as the hardware
  // requires us to, and we set them to ALT5 as per the datasheet.
  static const device_register_t GPFSEL1 = 0x20200004;
  device_rmw(GPFSEL1, 0b111111 << 12, 0b010010 << 12);
  device_barrier();

  // Turn on UART1. It's an AUX peripheral, so we need to enable it there.
  static const device_register_t AUXENB = 0x20215004;
  device_rmw(AUXENB, 0b001, 0b001);
  device_barrier();

  // List all the registers we will use for configuration
  static const device_register_t AUX_MU_IIR_REG = 0x20215044;
  static const device_register_t AUX_MU_IER_REG = 0x20215048;
  static const device_register_t AUX_MU_LCR_REG = 0x2021504c;
  static const device_register_t AUX_MU_MCR_REG = 0x20215054;
  static const device_register_t AUX_MU_CNTL_REG = 0x20215060;
  static const device_register_t AUX_MU_BAUD_REG = 0x20215068;

  // Disable the UART while we configure it
  device_write(AUX_MU_CNTL_REG, 0x00);

  // Set the UART to 8-bit mode
  device_write(AUX_MU_LCR_REG, 0x03);
  // Set the baud rate to 115200. With a 250MHz clock, this is 270.
  device_write(AUX_MU_BAUD_REG, 270);

  // Disable interrupts
  device_write(AUX_MU_IIR_REG, 0x00);
  // Clear the FIFOs and the overrun flag
  device_write(AUX_MU_IER_REG, 0xc6);
  device_read(AUX_MU_MCR_REG);

  // Finally, enable the UART. We don't do any RTS/CTS since those pins aren't
  // connected.
  device_write(AUX_MU_CNTL_REG, 0x03);
  device_barrier();
}

void uart_putc(char c) {
  device_barrier();
  while (device_read(AUX_MU_STAT_REG) & 0x20)
    ;
  device_write(AUX_MU_IO_REG, c);
  device_barrier();
}

void uart_puts(const char *s) {
  while (*s)
    uart_putc(*s++);
}

void uart_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  uart_vprintf(fmt, args);
  va_end(args);
}

static void uart_emit_hex(uint32_t value);
static void uart_emit_decimal(int32_t value, bool is_signed);

void uart_vprintf(const char *fmt, va_list args) {
  while (*fmt) {
    // Normal case for no conversion specifier
    if (*fmt != '%') {
      uart_putc(*fmt++);
      continue;
    }

    // Skip the `%`
    fmt++;
    // Decide what to do based on the conversion specifier
    switch(*fmt) {
    default:
      // If we don't know what it is, just print the conversion specifier as-is
      uart_putc('%');
      uart_putc(*fmt);
      break;
    case '%':
      uart_putc('%');
      break;
    case 's':
      uart_puts(va_arg(args, const char *));
      break;
    case 'x':
      uart_emit_hex(va_arg(args, uint32_t));
      break;
    case 'u':
      uart_emit_decimal(va_arg(args, int32_t), false);
      break;
    case 'd':
      uart_emit_decimal(va_arg(args, int32_t), true);
      break;
    }
    // Move past the conversion specifier
    fmt++;
  }
}

static void uart_emit_hex(uint32_t value) {
  // Print the leading `0x`
  uart_puts("0x");
  // Isolate each of the eight nibbles, then print each of them out
  for (int i = 0; i < 8; i++) {
    uint8_t n = (value >> (28 - i * 4)) & 0xf;
    uart_putc(n < 10 ? '0' + n : 'a' + n - 10);
  }
}

static void uart_emit_decimal(int32_t value, bool is_signed) {
  // If the value is zero, just print a `0`
  if (value == 0) {
    uart_putc('0');
    return;
  }
  // If the value is negative, print a `-` and negate it
  if (is_signed && value < 0) {
    uart_putc('-');
    value = -value;
  }
  // Create a buffer to store the string representation of the number. It'll be
  // at most 10 digits long, plus a null terminator.
  char buffer[11];
  char *cur = buffer + 10;
  *cur-- = '\0';
  while (value != 0) {
    *cur-- = '0' + value % 10;
    value /= 10;
  }
  uart_puts(cur + 1);
}
