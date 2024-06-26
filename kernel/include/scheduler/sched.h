#ifndef SCHED_H
#define SCHED_H

// Size of the stack for each task
#define THREAD_SIZE PAGE_SIZE

// Maximum number of tasks that can be created
#define MAX_TASKS 64

// Maximum number of pages that can be allocated for each task
#define MAX_PAGES_PER_PROCESS 16

// Task macros
#define FIRST_TASK task[0]
#define LAST_TASK task[MAX_TASKS - 1]

// Task flags
#define PF_KTHREAD 0x2

/*!
 * \brief Task states
 */
typedef enum {
  TASK_RUNNING,
  TASK_INTERRUPTIBLE,
  TASK_UNINTERRUPTIBLE,
  TASK_STOPPED,
} task_state_t;

/*!
 * \brief CPU context
 */
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

/*!
 * \brief User page address
 *
 * \var `physical_address` Physical address of the page
 * \var `virtual_address` Virtual address of the page
 */
typedef struct {
  unsigned long physical_address;
  unsigned long virtual_address;
} user_page_t;

/*!
 * \brief Memory management structure
 *
 * \var `pgd_address` PGD translation table address for the current task
 * \var `user_pages_count` Number of user pages
 * \var `user_pages` User pages
 * \var `kernel_pages_count` Number of kernel pages
 * \var `kernel_pages` Kernel pages
 */
typedef struct {
  unsigned long pgd_address;
  int user_pages_count;
  user_page_t user_pages[MAX_PAGES_PER_PROCESS];
  int kernel_pages_count;
  unsigned long kernel_pages[MAX_PAGES_PER_PROCESS];
} mm_struct_t;

/*!
 * \brief Task structure, containing all the information about a task
 *
 * \var `cpu_context` CPU context
 * \var `state` Task state
 * \var `counter` Task counter
 * \var `priority` Task priority
 * \var `skip_preempt_count` Number of times a task can skip preemption
 * \var `flags` Task flags
 * \var `mm` Memory management structure
 */
typedef struct {
  cpu_context_t cpu_context;
  task_state_t state;
  long counter;
  long priority;
  long skip_preempt_count;

  unsigned long flags;
  mm_struct_t mm;
} task_struct_t;

// Global variables, for task management
extern task_struct_t *current_task;
extern task_struct_t *task[MAX_TASKS];
extern int number_of_tasks;

// Initial task (even the first task created by the kernel is forked from this
// init task)
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
      .flags = PF_KTHREAD,                                                     \
      .mm = {.pgd_address = 0,                                                 \
             .user_pages_count = 0,                                            \
             .user_pages = {{0}},                                              \
             .kernel_pages_count = 0,                                          \
             .kernel_pages = {0}},                                             \
  }

// Member APIs

/*!
 * \brief Initialize the scheduler
 */
void scheduler_init();

/*! \brief prepare the scheduler */
void schedule();

/*!
 * \brief Schedule the next task in the task queue
 * based on the basic scheduling algorithm used in the initial release of the
 * Linux Kernel
 */
void _schedule();

/*!
 * \brief Enable preemption of the current task
 */
void preempt_enable();

/*!
 * \brief Disable preemption of the current task
 */
void preempt_disable();

/*!
 * \brief Switch the current task with the next task
 *
 * \param next Next task that has to be scheduled by the scheduler.
 */
void switch_task(task_struct_t *next);

/*!
 * \brief Switch the context of the current task with the next task
 *
 * \param prev Previous task
 * \param next Next task
 */
extern void switch_context(task_struct_t *prev, task_struct_t *next);

/*!
 * \brief Prepare the kernel, and call the scheduler when the system timer
 * raises an interrupt
 */
void timer_tick();

/*!
 * \brief Prepare the kernel and the CPU to execute the upcoming task scheduled.
 */
void task_init();

/*!
 * \brief Gracefully exit the current task
 */
void task_exit();

#endif // !SCHED_H
