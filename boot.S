#include "interrupts/system_registers.h"

// Bootloader at the start of the kernel image
.section ".text.boot"

.global _start

_start:
    mrs x5, mpidr_el1 // Multi-Processor Identifier(EL1)
    lsr x5, x5, #8 // Shift the identifier to the right by 8 bits.
    and x5, x5, #7 // Last 3 bits of the identifier extracted. Indicates CPU ID.
    cbz x5, intr_init // Proceed if on primary core, else wait for the primary core to initialize.
    
    // The default stub for the Raspberry Pi 5 looks for the first instruction at x80000, 
    // and runs it on the primary core of the processor (generally core 0), and parks all 
    // the remaining cores. If the address of a function is written onto the register at 
    // which the respective core expects it, the core is then woken up from sleep to 
    // execute that function.
  
    // delay for the secondary cores to wait for the primary core to initialize
                  mov x0, #10000
core_init_delay:  sub x0, x0, #1
                  cbz x0, intr_init
                  b core_init_delay

core_hang:  wfe // Wait for event, and loop indefinitely
            b core_hang


intr_init:  ldr x0, =SCTLR_VALUE_MMU_DISABLED
            msr sctlr_el1, x0
 
            ldr x0, =SPSR_VALUE
            msr spsr_el2, x0

            adr x0, bss_init
            msr elr_el2, x0

            eret

bss_init: adr x1, __bss_start // Load the start address of the BSS section, defined in the linker.
          adr x2, __bss_size // Load the size of the BSS section, calculated and defined in the linker.

bss_loop: cbz x2, stack_init // Skip initialization of BSS variables if the size is `0`

          str xzr, [x1], #8 // Initialize all the BSS variables to `0`
          sub x2, x2, #1
          cbnz x2, bss_loop // Iterate for all variables of the BSS section

stack_init: adr x1, __stack_end // Load the end address of the stack for the primary core, 
                        // defined in the linker (stack grows downwards)
    
            adr x3, __stack_size // Load the size of the stack, calculated and defined in the linker
            mul x4, x5, x3 // Calculate the offset for the current core
            add x1, x4, x1 // Calculate the end address of the stack for the current core.

            mov sp, x1 // Store the start address in the stack ptr.
    // The first element of the stack contains the start address, and from the
    // next contiguous address, the stack grows.

    mov x0, x5 // Pass core ID as an argument to the `kernel_main()` routine of the kernel
    bl kernel_main // Link and jump to the `kernel_main()` routine of the kernel

    b core_hang // If the `kernel_main()` routine of the kernel returns, loop indefinitely

