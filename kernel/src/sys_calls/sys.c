#include "sys_calls/sys.h"
#include "mmu.h"
#include "peripherals/uart.h"
#include "scheduler/fork.h"
#include "scheduler/page_manager.h"
#include "scheduler/sched.h"
#include "sys_calls/sys_wrappers.h"

void sys_write(char *text) { uart_puts(text); }

int sys_fork() { return fork_process(0, 0, 0); }

unsigned long sys_malloc() {
  unsigned long alloc_addr = allocate_free_page();
  if (!alloc_addr)
    return -1;

  return alloc_addr;
}

void sys_proc_exit() { task_exit(); }

void const *sys_call_table[] = {
    &sys_write,
    &sys_fork,
    &sys_malloc,
    &sys_proc_exit,
};
