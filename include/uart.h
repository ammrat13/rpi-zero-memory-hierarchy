#pragma once

#include <stdarg.h>

void uart_init(void);

void uart_putc(char c);
void uart_puts(const char *s);

// Supported conversion specifiers:
//   %% - a literal '%'
//   %s - a null-terminated string
//   %x - a hex number
//   %u - an unsigned decimal number
//   %d - a signed decimal number
void uart_printf(const char *fmt, ...);
void uart_vprintf(const char *fmt, va_list args);
