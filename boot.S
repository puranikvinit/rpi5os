// Bootloader at the start of the kernel image
.section ".text.boot"

.global _start

_start:
    mrs x1, mpidr_el1 // multi-processor identifier
    and x1, x1, #3 // Last 2 bits of the identifier extracted. `0` indicates primary core
    cbz x1, 2f // Proceed if on primary core, else loop while waiting for event

1:  wfe
    b 1b

2:  ldr x1, =_start // Load the start address of the boot file.
    mov sp, x1 // Store the start address in the stack ptr.
    // The first element of the stack contains the start address, and from the
    // next contiguous address, the stack grows.

    ldr x1, =__bss_start // Load the start address of the BSS section, defined in the linker
    ldr x2, =__bss_size // Load the size of the BSS section, calculated and defined in the linker
    
3:  cbz x2, 4f // Skip initialization of BSS variables

    str xzr, [x1], #8 // Initialize all the BSS variables to `0`
    sub x2, x2, #1
    cbnz x2, 3b // Iterate for all variables of the BSS section

4:  bl main // Link and jump to the `main()` routine of the kernel

    b 1b // If the `main()` routine of the kernel returns, loop indefinitely



    
