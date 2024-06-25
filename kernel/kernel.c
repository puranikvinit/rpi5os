#include "interrupts/gic.h"
#include "interrupts/vector_init.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "scheduler/fork.h"
#include "scheduler/sched.h"
#include "user_code.h"

long semaphore = 0;

void kernel_process() {
  uart_puts("Kernel Process\n\0");

  extern unsigned long __user_begin;
  extern unsigned long __user_end;

  unsigned long user_size =
      ((unsigned long)(&__user_end)) - ((unsigned long)(&__user_begin));

  int err_code = move_to_user_mode(
      (unsigned long)(&__user_begin), user_size,
      ((unsigned long)user_process - (unsigned long)(&__user_begin)));
  if (err_code) {
    uart_puts("Error moving to user mode!\n\0");
  }
}

int kernel_main(unsigned int core_id) {
  uart_init();
  uart_puts("UART Initialized!\n\0");

  uart_puts("Core ID, \0");
  uart_putc(core_id + '0');
  uart_putc('\n');

  // char buff[5] = "";
  // int_to_str(get_current_exception_level(), buff);
  // uart_puts("Current Exception Level: \0");
  // uart_puts(buff);
  // uart_putc('\n');

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

  scheduler_init();
  int res = fork_process(PF_KTHREAD, (unsigned long)&kernel_process, 0);
  if (res < 0) {
    uart_puts("Error forking kernel process!\n\0");
    return 1;
  }

  while (1) {
    schedule();
  }

  return 0;
}
