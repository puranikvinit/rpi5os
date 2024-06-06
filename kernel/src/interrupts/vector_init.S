.globl vector_table_init
vector_table_init:
  adr x0, vector_table
  msr vbar_el1, x0
  ret

.globl irq_enable
irq_enable:
  msr daifclr, #2
  ret

.globl irq_disable
isr_disable:
  msr daifset, #2
  ret
