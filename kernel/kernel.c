#include "interrupts/gic.h"
#include "interrupts/vector_init.h"
#include "mmio.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "scheduler/fork.h"
#include "scheduler/sched.h"
#include "sys_calls/sys.h"
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

void user_process() {
  char *buff = "User Process - Parent\n\0";
  call_sys_write(buff);

  unsigned long child_stack = call_sys_malloc();
  if (child_stack < 0) {
    char *err = "Error allocating memory for child stack!\n\0";
    call_sys_write(err);
  }

  int err_code = call_sys_fork((unsigned long)&test_process,
                               (unsigned long)"12345", child_stack);
  if (err_code < 0) {
    char *err = "Error forking child process!\n\0";
    call_sys_write(err);
  }

  child_stack = call_sys_malloc();
  if (child_stack < 0) {
    char *err = "Error allocating memory for child stack!\n\0";
    call_sys_write(err);
  }

  err_code = call_sys_fork((unsigned long)&test_process, (unsigned long)"ABCDE",
                           child_stack);
  if (err_code < 0) {
    char *err = "Error forking child process!\n\0";
    call_sys_write(err);
  }

  call_sys_proc_exit();
}

void kernel_process() {
  uart_puts("Kernel Process\n\0");

  int err_code = move_to_user_mode((unsigned long)&user_process);
  if (err_code) {
    uart_puts("Error moving to user mode!\n\0");
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

  int res = fork_process(PF_KTHREAD, (unsigned long)&kernel_process, 0, 0);
  if (res < 0) {
    uart_puts("Error forking kernel process!\n\0");
    return 1;
  }

  while (1) {
    scheduler_init();
  }

  return 0;
}
