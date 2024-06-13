#include "interrupts/vector_table.h"

// Macro to define a vector entry in the vector table
.macro vector_entry type

// Align the vector table to 0x80 bytes, as required by the bootstub.
.balign 0x80
b \type

.endm

.globl interrupt_entry
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

.globl interrupt_exit
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

// Macro to initialise a valid defined interrupt handler
.macro handle_valid_interrupt_entry el, handler

interrupt_entry \el
bl \handler
interrupt_exit \el

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

// Align the vector table to 0x800 bytes, as required by the bootstub.
.balign 0x800

// Define the vector table
.globl vector_table
vector_table:
  vector_entry el1t_sync_exception
  vector_entry el1t_irq_exception
  vector_entry el1t_fiq_exception
  vector_entry el1t_serror_exception

  vector_entry el1h_sync_exception
  vector_entry el1h_irq_exception
  vector_entry el1h_fiq_exception
  vector_entry el1h_serror_exception

  vector_entry el0_64_sync_exception
  vector_entry el0_64_irq_exception
  vector_entry el0_64_fiq_exception
  vector_entry el0_64_serror_exception

  vector_entry el0_32_sync_exception
  vector_entry el0_32_irq_exception
  vector_entry el0_32_fiq_exception
  vector_entry el0_32_serror_exception

el1t_sync_exception:
  handle_invalid_interrupt_entry 1, EL1t_SYNC_EXCEPTION

el1t_irq_exception:
  handle_invalid_interrupt_entry 1, EL1t_IRQ_EXCEPTION

el1t_fiq_exception:
  handle_invalid_interrupt_entry 1, EL1t_FIQ_EXCEPTION

el1t_serror_exception:
  handle_invalid_interrupt_entry 1, EL1t_SERROR_EXCEPTION


el1h_sync_exception:
  handle_invalid_interrupt_entry 1, EL1h_SYNC_EXCEPTION

el1h_irq_exception:
  handle_valid_interrupt_entry 1, handle_irq

el1h_fiq_exception:
  handle_invalid_interrupt_entry 1, EL1h_FIQ_EXCEPTION

el1h_serror_exception:
  handle_invalid_interrupt_entry 1, EL1h_SERROR_EXCEPTION


el0_64_sync_exception:
  handle_valid_interrupt_entry 0, handle_sync

el0_64_irq_exception:
  handle_valid_interrupt_entry 0, handle_irq

el0_64_fiq_exception:
  handle_invalid_interrupt_entry 0, EL0_64_FIQ_EXCEPTION

el0_64_serror_exception:
  handle_invalid_interrupt_entry 0, EL0_64_SERROR_EXCEPTION


el0_32_sync_exception:
  handle_invalid_interrupt_entry 0, EL0_32_SYNC_EXCEPTION

el0_32_irq_exception:
  handle_invalid_interrupt_entry 0, EL0_32_IRQ_EXCEPTION

el0_32_fiq_exception:
  handle_invalid_interrupt_entry 0, EL0_32_FIQ_EXCEPTION

el0_32_serror_exception:
  handle_invalid_interrupt_entry 0, EL0_32_SERROR_EXCEPTION

// Loop indefinitely if an error occurs, especially when an invalid interrupt raised
.globl error_hang
error_hang: b error_hang

