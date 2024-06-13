#ifndef SCHED_H
#define SCHED_H

#define THREAD_SIZE 4096

#define MAX_TASKS 64

#define FIRST_TASK task[0]
#define LAST_TASK task[MAX_TASKS - 1]

#define PF_KTHREAD 0x2

typedef enum {
  TASK_RUNNING,
  TASK_INTERRUPTIBLE,
  TASK_UNINTERRUPTIBLE,
  TASK_STOPPED,
} task_state_t;

typedef struct {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp; // x29
  unsigned long pc; // x30
  unsigned long sp;
} cpu_context_t;

typedef struct {
  cpu_context_t cpu_context;
  task_state_t state;
  long counter;
  long priority;
  long skip_preempt_count;

  unsigned long stack;
  unsigned long flags;
} task_struct_t;

extern task_struct_t *current_task;
extern task_struct_t *task[MAX_TASKS];
extern int number_of_tasks;

#define INIT_TASK                                                              \
  {                                                                            \
      .cpu_context =                                                           \
          {                                                                    \
              .x19 = 0,                                                        \
              .x20 = 0,                                                        \
              .x21 = 0,                                                        \
              .x22 = 0,                                                        \
              .x23 = 0,                                                        \
              .x24 = 0,                                                        \
              .x25 = 0,                                                        \
              .x26 = 0,                                                        \
              .x27 = 0,                                                        \
              .x28 = 0,                                                        \
              .fp = 0,                                                         \
              .pc = 0,                                                         \
              .sp = 0,                                                         \
          },                                                                   \
      .state = TASK_RUNNING,                                                   \
      .counter = 0,                                                            \
      .priority = 1,                                                           \
      .skip_preempt_count = 0,                                                 \
  }

// Member APIs

void scheduler_init();
void schedule();

void preempt_enable();
void preempt_disable();

void switch_task(task_struct_t *next);
extern void switch_context(task_struct_t *prev, task_struct_t *next);

void timer_tick();

void task_init();
void task_exit();

#endif // !SCHED_H
