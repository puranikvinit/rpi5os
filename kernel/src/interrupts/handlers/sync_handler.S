#include "system_registers.h"
#include "interrupts/vector_table.h"
#include "sys_calls/sys_wrappers.h"
#include "mmu.h"

// Macro to define instruction sequence before the peocessor enters the interrupt handler
.macro interrupt_entry el

sub sp, sp, #STACK_FRAME_SIZE

// Save registers x0-x30
stp x0, x1, [sp, #16*0]
stp x2, x3, [sp, #16*1]
stp x4, x5, [sp, #16*2]
stp x6, x7, [sp, #16*3]
stp x8, x9, [sp, #16*4]
stp x10, x11, [sp, #16*5]
stp x12, x13, [sp, #16*6]
stp x14, x15, [sp, #16*7]
stp x16, x17, [sp, #16*8]
stp x18, x19, [sp, #16*9]
stp x20, x21, [sp, #16*10]
stp x22, x23, [sp, #16*11]
stp x24, x25, [sp, #16*12]
stp x26, x27, [sp, #16*13]
stp x28, x29, [sp, #16*14]

.if \el == 0
mrs x21, sp_el0
.else
add x21, sp, #STACK_FRAME_SIZE
.endif

// Save ELR_EL1 and SPSR_EL1
mrs x22, ELR_EL1
mrs x23, SPSR_EL1

stp x30, x21, [sp, #16*15]
stp x22, x23, [sp, #16*16]

.endm

.macro interrupt_exit el

// Restore ELR_EL1 and SPSR_EL1
ldp	x22, x23, [sp, #16 * 16]
ldp	x30, x21, [sp, #16 * 15]

.if \el == 0
msr sp_el0, x21
.endif

msr	elr_el1, x22
msr	spsr_el1, x23

// Restore registers x0-x30
ldp x0, x1, [sp, #16*0]
ldp x2, x3, [sp, #16*1]
ldp x4, x5, [sp, #16*2]
ldp x6, x7, [sp, #16*3]
ldp x8, x9, [sp, #16*4]
ldp x10, x11, [sp, #16*5]
ldp x12, x13, [sp, #16*6]
ldp x14, x15, [sp, #16*7]
ldp x16, x17, [sp, #16*8]
ldp x18, x19, [sp, #16*9]
ldp x20, x21, [sp, #16*10]
ldp x22, x23, [sp, #16*11]
ldp x24, x25, [sp, #16*12]
ldp x26, x27, [sp, #16*13]
ldp x28, x29, [sp, #16*14]

add sp, sp, #STACK_FRAME_SIZE

eret

.endm

// Macro to initialise an invalid interrupt handler
.macro handle_invalid_interrupt_entry el, type

interrupt_entry \el

mov x0, #\type
mrs x1, ELR_EL1
mrs x2, ESR_EL1 // EXCEPTION_SYNDROME register

bl display_message
b error_hang

.endm

.globl handle_sync
handle_sync:
  mrs x25, esr_el1
  lsr x25, x25, #ESR_EL1_EC_SHIFT
  cmp x25, #ESR_EL1_EC_SVC_64B
  beq handle_svc
  cmp x25, #ESR_EL0_EC_DATA_ABORT
  beq handle_data_abort
  handle_invalid_interrupt_entry  0, EL0_64_SYNC_EXCEPTION

// x25 -> System call table
// x26 -> System call number
// x27 -> Number of system calls
handle_svc:
  adr x25, sys_call_table
  eor x26, x26, x26
  mov w26, w8
  mov x27, #NUMBER_SYSCALLS
  // bl interrupts_enable
  cmp x26, x27
  b.hs handle_invalid_syscall

  ldr x16, [x25, x26, lsl #3]
  ldr x17, =VA_START
  add x16, x16, x17
  blr x16
  b return_from_syscall

handle_invalid_syscall:
  handle_invalid_interrupt_entry 0, SYSCALL_ERROR
  
handle_data_abort:
  bl interrupts_enable
  mrs x0, far_el1
  mrs x1, esr_el1
  bl mem_abort_handler
  cmp x0, 0
  b.eq x
  handle_invalid_interrupt_entry 0, DATA_ABORT_ERROR
x: bl interrupts_disable
   interrupt_exit 0

