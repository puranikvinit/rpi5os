#include "sys_calls/sys_wrappers.h"
#include "interrupts/vector_table.h"

// Macro to define instruction sequence after the processor exits the interrupt handler
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

.globl return_from_syscall
return_from_syscall:
  bl interrupts_disable
  str x0, [sp]
  interrupt_exit 0



