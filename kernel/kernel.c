#include "io/mmio.h"
#include "io/uart.h"
#include "util/string.h"

long semaphore = 0;

int kernel_main(unsigned int core_id) {
  if (core_id == 0) {
    uart_init();
  }

  uart_puts("Core ID, \0");
  uart_putc(core_id + '0');
  uart_putc('\n');

  char buff[5] = "";
  int_to_str(get_current_exception_level(), buff);
  uart_puts(buff);

  while (1) {
  }

  return 0;
}
