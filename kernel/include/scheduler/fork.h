#ifndef FORK_H
#define FORK_H

#include "scheduler/sched.h"

// Processor State Register (PSR), EL0t
#define PSR_EL0t 0x00000000

// Processor State Register (PSR), EL1t
#define PSR_EL1t 0x00000004
// Processor State Register (PSR), EL1h
#define PSR_EL1h 0x00000005

// Processor State Register (PSR), EL2t
#define PSR_EL2t 0x00000008
// Processor State Register (PSR), EL2h
#define PSR_EL2h 0x00000009

// Processor State Register (PSR), EL3t
#define PSR_EL3t 0x0000000c
// Processor State Register (PSR), EL3h
#define PSR_EL3h 0x0000000d

/*! \brief Structure to store the register values of a process.
 *
 * \var gpr General Purpose Registers
 * \var sp Stack Pointer
 * \var pc Program Counter
 * \var proc_state Processor State
 */
typedef struct {
  unsigned long gpr[31];
  unsigned long sp;
  unsigned long pc;
  unsigned long proc_state;
} proc_regs;

// Member APIs

/*! \brief Create a new process by forking the current process.
 *
 * \param `fork_flags` Flags to be set for the new process
 * \param `function` Function pointer to the function to be executed by the new
 * process
 * \param `args` Arguments to be passed to the function
 *
 * \return The process ID of the new process
 */
int fork_process(unsigned long fork_flags, unsigned long function,
                 unsigned long args);

/*! \brief Move the processor to user mode.
 *
 * \param `start` Start address of the code section to be executed in the
 * userspace
 * \param `size` Size of the code section
 * \param `function` Function pointer to the function to be executed in the
 * userspace
 *
 * \return Status code of the operation
 */
int move_to_user_mode(unsigned long start, unsigned long size,
                      unsigned long function);

/*! \brief Get the pointer to the register values of the current process.
 *
 * \param `task` The task structure of the current process
 *
 * \return The pointer to the register values of the current process
 */
proc_regs *get_ptr_to_regs(task_struct_t *task);

/*! \brief Setup the kernel to switch to the newly forked process.
 */
extern void return_from_fork();

#endif // !FORK_H
