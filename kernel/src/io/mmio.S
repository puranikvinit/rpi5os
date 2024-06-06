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
