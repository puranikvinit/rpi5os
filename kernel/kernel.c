#include "interrupts/vector_init.h"
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

  vector_table_init();
  uart_puts("Vector Table Initialized\n\0");

  irq_enable();
  uart_puts("IRQs Enabled\n\0");

  while (1) {
  }

  return 0;
}
