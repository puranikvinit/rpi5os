#include "interrupts/gic.h"
#include "interrupts/vector_init.h"
#include "mmio.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "scheduler/fork.h"
#include "scheduler/sched.h"
#include "util/string.h"

long semaphore = 0;

void test_process(char *text) {
  while (1) {
    for (int i = 0; i < 5; i++) {
      uart_putc(text[i]);
    }
    uart_putc('\n');
  }
}

int kernel_main(unsigned int core_id) {
  if (core_id == 0) {
    uart_init();
  }

  uart_puts("Core ID, \0");
  uart_putc(core_id + '0');
  uart_putc('\n');

  char buff[5] = "";
  int_to_str(get_current_exception_level(), buff);
  uart_puts("Current Exception Level: \0");
  uart_puts(buff);
  uart_putc('\n');

  vector_table_init();
  uart_puts("Vector Table Initialized!\n\0");

  gic_init();
  uart_puts("GIC Initialized!\n\0");

  timer_init();
  uart_puts("Timer Initialized!\n\0");

  interrupts_enable();
  uart_puts("IRQs Enabled!\n\0");

  enable_irq_line(SYSTEM_TIMER_IRQ_1);
  uart_puts("IRQ Handler Registered!\n\0");

  // enable_irq_line(PCIE_IRQ_1);
  // enable_irq_line(PCIE_IRQ_2);
  // enable_irq_line(PCIE_IRQ_3);
  // enable_irq_line(PCIE_IRQ_4);
  // uart_puts("PCIe IRQ Handler Registered!\n\0");

  int res = fork_process((unsigned long)&test_process, (unsigned long)"12345");
  if (res) {
    uart_puts("Error forking process 1!\n\0");
    return 1;
  }

  res = fork_process((unsigned long)&test_process, (unsigned long)"ABCDE");
  if (res) {
    uart_puts("Error forking process 2!\n\0");
    return 1;
  }

  while (1) {
    scheduler_init();
  }

  return 0;
}
