#include "util/printk.h"
#include "peripherals/uart.h"
#include "util/string.h"

char *log_levels[] = {
    "INFO: \0", "DEBUG: \0", "WARN: \0", "ERROR: \0", "PANIC: \0",
};

void printk(log_level lvl, char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  uart_puts(log_levels[lvl] + VA_START);

  for (const char *f = fmt; *f != '\0'; f++) {
    if (*f != '%') {
      uart_putc(*f);
      continue;
    }

    f++;
    char buff[40] = "";
    switch (*f) {
    case 'c': {
      uart_putc((char)va_arg(va, int));
      break;
    }

    case 's': {
      uart_puts(va_arg(va, char *));
      break;
    }

    case 'd': {
      int_to_str(va_arg(va, int), buff, 10);
      uart_puts(buff);
      break;
    }

    case 'x': {
      int_to_str(va_arg(va, int), buff, 16);
      uart_puts(buff);
      break;
    }

    case 'l': {
      int_to_str(va_arg(va, long), buff, 10);
      uart_puts(buff);
      break;
    }

    case 'b': {
      int_to_str(va_arg(va, int), buff, 2);
      uart_puts(buff);
      break;
    }
    }
  }
  uart_putc('\n');

  va_end(va);
}
