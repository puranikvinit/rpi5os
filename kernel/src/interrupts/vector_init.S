// Initialise the vector table
.globl vector_table_init
vector_table_init:
  adr x0, vector_table
  msr vbar_el1, x0
  ret

// Enable interrupts
// [D] - Debug Interrupt Mask
// [A] - Synchronous Interrupt Mask
// [I] - IRQ Mask
// [F] - FIQ Mask
.globl interrupts_enable
irq_enable:
  msr daifclr, #7
  ret

// Disable interrupts
.globl interrupts_disable
irq_disable:
  msr daifset, #7
  ret

