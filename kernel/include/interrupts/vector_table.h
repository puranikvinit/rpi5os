#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

#define STACK_FRAME_SIZE                                                       \
  256 // Size of the stack fram required to store the current processor state
      // before entering into the exception handler (32 registers, 8 bytes
      // each).

// EL1t - Exception Level 1, shares stack pointer with Exception Level 0
// (SPSel=0)
// EL1h - Exception Level 1, has its own stack pointer (SPSel=1)
// EL0_64 - Excpetion taken from EL0, 64-bit mode
// EL0_32 - Exception taken from EL0, 32-bit mode
//
// Types of Exceptions:
// 1. Synchronous Exceptions
// 2. IRQ
// 3. FIQ
// 4. SError
//
// Hence, we have 16 exception handlers in total, 4 for each type of exexution
// state of the processor.

#define EL1t_SYNC_EXCEPTION 0
#define EL1t_IRQ_EXCEPTION 1
#define EL1t_FIQ_EXCEPTION 2
#define EL1t_SERROR_EXCEPTION 3

#define EL1h_SYNC_EXCEPTION 4
#define EL1h_IRQ_EXCEPTION 5
#define EL1h_FIQ_EXCEPTION 6
#define EL1h_SERROR_EXCEPTION 7

#define EL0_64_SYNC_EXCEPTION 8
#define EL0_64_IRQ_EXCEPTION 9
#define EL0_64_FIQ_EXCEPTION 10
#define EL0_64_SERROR_EXCEPTION 11

#define EL0_32_SYNC_EXCEPTION 12
#define EL0_32_IRQ_EXCEPTION 13
#define EL0_32_FIQ_EXCEPTION 14
#define EL0_32_SERROR_EXCEPTION 15

#define SYSCALL_ERROR 16
#define DATA_ABORT_ERROR 17

#endif // !VECTOR_TABLE_H
