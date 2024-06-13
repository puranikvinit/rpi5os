#include "scheduler/sched.h"
#include "interrupts/vector_init.h"
#include "scheduler/page_manager.h"

static task_struct_t init_task = INIT_TASK;
task_struct_t *current_task = &(init_task);
task_struct_t *task[MAX_TASKS] = {
    &init_task,
};
int number_of_tasks = 1;

void preempt_enable() { current_task->skip_preempt_count--; }

void preempt_disable() { current_task->skip_preempt_count++; }

void scheduler_init() {
  current_task->counter = 0;
  schedule();
}

void schedule() {
  preempt_disable();

  int c, next;
  task_struct_t *p;

  while (1) {
    c = -1;
    next = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
      p = task[i];
      if (p && p->state == TASK_RUNNING && p->counter > c) {
        c = p->counter;
        next = i;
      }
    }

    if (c) {
      break;
    }

    for (int i = 0; i < MAX_TASKS; i++) {
      p = task[i];
      if (p)
        p->counter = (p->counter >> 1) + p->priority;
    }
  }
  switch_task(task[next]);
  preempt_enable();
}

void switch_task(task_struct_t *next) {
  if (current_task == next)
    return;

  task_struct_t *prev = current_task;
  current_task = next;

  switch_context(prev, current_task);
}

void timer_tick() {
  --current_task->counter;
  if (current_task->counter > 0 || current_task->skip_preempt_count > 0)
    return;

  current_task->counter = 0;
  interrupts_enable();
  schedule();
  interrupts_disable();
}

void task_init() {
  // Any other initialisation that needs to be necessarily done before the task
  // is preempted.
  preempt_enable();
}

void task_exit() {
  preempt_disable();

  for (int i = 0; i < MAX_TASKS; i++) {
    if (task[i] == current_task) {
      task[i]->state = TASK_STOPPED;
      break;
    }
  }
  if (current_task->stack) {
    free_page(current_task->stack);
  }

  preempt_enable();
  scheduler_init();
}
