#include "scheduler/fork.h"
#include "mmio.h"
#include "mmu.h"
#include "scheduler/page_manager.h"
#include "scheduler/sched.h"

int fork_process(unsigned long fork_flags, unsigned long function,
                 unsigned long args) {
  preempt_disable();

  task_struct_t *new_task;
  new_task = (task_struct_t *)allocate_kernel_page();
  if (!new_task) {
    // preempt_enable();
    return -1;
  }

  proc_regs *regs_child = get_ptr_to_regs(new_task);
  mem_init_zero((unsigned long)regs_child, sizeof(*regs_child));
  mem_init_zero((unsigned long)&new_task->cpu_context, sizeof(cpu_context_t));

  if (fork_flags & PF_KTHREAD) {
    new_task->cpu_context.x19 = function;
    new_task->cpu_context.x20 = args;
  } else {
    proc_regs *regs_parent = get_ptr_to_regs(current_task);
    *regs_child = *regs_parent;
    regs_child->gpr[0] = 0;
    copy_virt_mem(new_task);
  }

  new_task->flags = fork_flags;
  new_task->priority = current_task->priority;
  new_task->state = TASK_RUNNING;
  new_task->counter = new_task->priority;
  new_task->skip_preempt_count = 1;

  new_task->cpu_context.pc = (unsigned long)return_from_fork;
  // new_task->cpu_context.sp = (unsigned long)new_task + THREAD_SIZE;
  new_task->cpu_context.sp = (unsigned long)regs_child;

  task[++number_of_tasks] = new_task;

  preempt_enable();

  return number_of_tasks;
}

int move_to_user_mode(unsigned long start, unsigned long size,
                      unsigned long function) {
  proc_regs *regs = get_ptr_to_regs(current_task);
  mem_init_zero((unsigned long)regs, sizeof(*regs));

  regs->pc = function;
  regs->proc_state = PSR_EL0t;
  regs->sp = 2 * PAGE_SIZE;

  unsigned long code_page = allocate_user_page(current_task, 0);
  if (!code_page)
    return -1;

  mem_copy(code_page, start, size);
  set_pgd(current_task->mm.pgd_address);

  return 0;
}

proc_regs *get_ptr_to_regs(task_struct_t *task) {
  unsigned long ptr = (unsigned long)task + THREAD_SIZE - sizeof(proc_regs);
  return (proc_regs *)ptr;
}
