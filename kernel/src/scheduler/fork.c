#include "scheduler/fork.h"
#include "mmio.h"
#include "scheduler/page_manager.h"
#include "scheduler/sched.h"

int fork_process(unsigned long fork_flags, unsigned long function,
                 unsigned long args, unsigned long stack_ptr) {
  preempt_disable();

  task_struct_t *new_task;
  new_task = (task_struct_t *)allocate_free_page();
  if (!new_task) {
    // preempt_enable();
    return -1;
  }

  proc_regs *regs_child = get_ptr_to_regs(new_task);
  mem_init_zero((unsigned long)regs_child, sizeof(*regs_child));
  mem_init_zero((unsigned long)&new_task->cpu_context, sizeof(cpu_context_t));

  if (fork_flags & PF_KTHREAD) {
    new_task->cpu_context.x19 = function;
    new_task->cpu_context.x20 = stack_ptr;
  } else {
    proc_regs *regs_parent = get_ptr_to_regs(current_task);
    *regs_child = *regs_parent;
    regs_child->gpr[0] = 0;
    regs_child->sp = stack_ptr + PAGE_SIZE;
    new_task->stack = stack_ptr;
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

int move_to_user_mode(unsigned long function) {
  proc_regs *regs = get_ptr_to_regs(current_task);
  mem_init_zero((unsigned long)regs, sizeof(*regs));

  regs->pc = function;
  regs->proc_state = PSR_EL0t;

  unsigned long stack_ptr = allocate_free_page();
  if (!stack_ptr)
    return -1;

  regs->sp = stack_ptr + PAGE_SIZE;
  current_task->stack = stack_ptr;

  return 0;
}

proc_regs *get_ptr_to_regs(task_struct_t *task) {
  unsigned long ptr = (unsigned long)task + THREAD_SIZE - sizeof(proc_regs);
  return (proc_regs *)ptr;
}
