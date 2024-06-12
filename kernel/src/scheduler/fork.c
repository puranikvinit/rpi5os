#include "scheduler/fork.h"
#include "scheduler/page_manager.h"
#include "scheduler/sched.h"

int fork_process(unsigned long function, unsigned long args) {
  preempt_disable();

  task_struct_t *new_task;
  new_task = (task_struct_t *)allocate_free_page();
  if (!new_task) {
    // preempt_enable();
    return 1;
  }

  new_task->priority = current_task->priority;
  new_task->state = TASK_RUNNING;
  new_task->counter = new_task->priority;
  new_task->skip_preempt_count = 1;

  new_task->cpu_context.x19 = function;
  new_task->cpu_context.x20 = args;
  new_task->cpu_context.pc = (unsigned long)return_from_fork;
  new_task->cpu_context.sp = (unsigned long)new_task + THREAD_SIZE;

  task[++number_of_tasks] = new_task;

  preempt_enable();

  return 0;
}
