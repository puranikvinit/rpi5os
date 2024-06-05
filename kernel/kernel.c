#include "string.h"
#include "uart.h"

long semaphore = 0;

int kernel_main(unsigned int core_id) {
  if (core_id == 0) {
    uart_init();
  }

  uart_puts("Core ID, \0");
  uart_putc(core_id + '0');
  uart_putc('\n');

  char buff[5] = "";
  int_to_str(53, buff);
  uart_puts(buff);

  while (semaphore != core_id) {
  }

  semaphore++;

  while (1) {
  }

  return 0;
}
