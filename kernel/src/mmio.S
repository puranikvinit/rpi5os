.globl mmio_read_32 // Function to read a 32-bit unsigned integer from the specified memory-mapped register
mmio_read_32:
	ldr w0,[x0]
	ret

.globl mmio_write_32 // Function to write a 32-bit unsigned integer into the specified memory-mapped register.
mmio_write_32:
	str w1,[x0]
	ret

.globl get_current_exception_level // Function to get the current exception level
get_current_exception_level:
  mrs x0, CurrentEL
  lsr x0, x0, #2
  ret

.globl mem_init_zero
mem_init_zero:
  str xzr, [x0], #8
  subs x1, x1, #8
  b.gt mem_init_zero
  ret

.globl memcpy
memcpy:
	ldr x3, [x1], #8
	str x3, [x0], #8
	subs x2, x2, #8
	b.gt memcpy
	ret

.globl mem_copy
mem_copy:
	ldr x3, [x1], #8
	str x3, [x0], #8
	subs x2, x2, #8
	b.gt memcpy
	ret

.globl set_pgd
set_pgd:
  msr ttbr0_el1, x0
  tlbi vmalle1is
  DSB ISH
  isb
  ret

