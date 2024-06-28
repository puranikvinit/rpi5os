#include "interrupts/gic.h"
#include "interrupts/vector_init.h"
#include "mmio.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "scheduler/fork.h"
#include "scheduler/sched.h"
#include "user_code.h"
#include "util/printk.h"

void kernel_process() {
  printk(DEBUG, "kernel process started\0");

  extern unsigned long __user_begin;
  extern unsigned long __user_end;

  unsigned long user_size =
      ((unsigned long)(&__user_end)) - ((unsigned long)(&__user_begin));

  int err_code = move_to_user_mode(
      (unsigned long)(&__user_begin), user_size,
      ((unsigned long)user_process - (unsigned long)(&__user_begin)));
  if (err_code) {
    printk(ERROR, "move to user mode failed\0");
  }
}

int kernel_main(unsigned int core_id) {
  uart_init();
  printk(INFO, "uart initialized\0");

  printk(INFO, "core id: %d\0", core_id);

  printk(INFO, "current exception level: %d\0", get_current_exception_level());

  vector_table_init();
  printk(INFO, "vector table initialized\0");

  gic_init();
  printk(INFO, "gic initialized\0");

  timer_init();
  printk(INFO, "timer initialized\0");

  interrupts_enable();
  printk(INFO, "irqs enabled\0");

  enable_irq_line(SYSTEM_TIMER_IRQ_1);

  // enable_irq_line(PCIE_IRQ_1);
  // enable_irq_line(PCIE_IRQ_2);
  // enable_irq_line(PCIE_IRQ_3);
  // enable_irq_line(PCIE_IRQ_4);

  scheduler_init();
  int res = fork_process(PF_KTHREAD, (unsigned long)&kernel_process, 0);
  if (res < 0) {
    printk(ERROR, "fork kernel process failed\0");
    return 1;
  }

  while (1) {
    schedule();
  }

  return 0;
}
