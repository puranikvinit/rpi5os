#ifndef FORK_H
#define FORK_H

#include "scheduler/sched.h"
#define PSR_EL0t 0x00000000

#define PSR_EL1t 0x00000004
#define PSR_EL1h 0x00000005

#define PSR_EL2t 0x00000008
#define PSR_EL2h 0x00000009

#define PSR_EL3t 0x0000000c
#define PSR_EL3h 0x0000000d

typedef struct {
  unsigned long gpr[31];
  unsigned long sp;
  unsigned long pc;
  unsigned long proc_state;
} proc_regs;

// Member APIs

int fork_process(unsigned long fork_flags, unsigned long function,
                 unsigned long args, unsigned long stack_ptr);

int move_to_user_mode(unsigned long function);

proc_regs *get_ptr_to_regs(task_struct_t *task);

extern void return_from_fork();

#endif // !FORK_H
