SECTIONS
{
    . = 0x80000; /* Start address of the kernel image */

    .text : { 
        KEEP(*(.text.boot)) 

        kernel/src/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/interrupts/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/interrupts/handlers/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/peripherals/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/scheduler/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/sys_calls/*(.text .text.* .gnu.linkonce.t*)
        kernel/src/util/*(.text .text.* .gnu.linkonce.t*)
    }
    .rodata : {
        kernel/src/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/interrupts/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/interrupts/handlers/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/peripherals/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/scheduler/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/sys_calls/*(.rodata .rodata.* .gnu.linkonce.r*)
        kernel/src/util/*(.rodata .rodata.* .gnu.linkonce.r*)
    }

        /* PROVIDE -> for variables which are referenced but not initialized in the section */
    PROVIDE(_data = .); /* Initialize data start address to current location pointer */
    .data : { 
        kernel/src/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/interrupts/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/interrupts/handlers/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/peripherals/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/scheduler/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/sys_calls/*(.data .data.* .gnu.linkonce.d*)
        kernel/src/util/*(.data .data.* .gnu.linkonce.d*)
    } /* Section for initialized data */
    
    . = ALIGN(0x1000);
    .bss (NOLOAD) : { /* Section for uninitialized data; NOLOAD -> Don't allocate space in linking, will be allocated and initialized in runtime */
        . = ALIGN(16); /* Align the current location pointer to the next 16bit boundary */
        __bss_start = .; /* Start address of the BSS section */
        
        kernel/src/*(.bss .bss.*)
        kernel/src/interrupts/*(.bss .bss.*)
        kernel/src/interrupts/handlers/*(.bss .bss.*)
        kernel/src/peripherals/*(.bss .bss.*)
        kernel/src/scheduler/*(.bss .bss.*)
        kernel/src/sys_calls/*(.bss .bss.*)
        kernel/src/util/*(.bss .bss.*)

        __bss_end = .; /* End address of the BSS section */
    }

    . = ALIGN(0x1000);
    id_pg_dir = .;
    .data.id_pg_dir : { . += (3 * (1 << 12)); }

    . = ALIGN(0x1000);
    high_pg_dir = .;
    .data.high_pg_dir : { . += (8 * (1 << 12)); }

    . = ALIGN(0x1000);
    __user_begin = .;
    .text.user : { test/src*(.text .text.*) } 
    .rodata.user : { test/src*(.rodata .rodata.*) }
    .data.user : { test/src*(.data .data.*) }
    .bss.user : { test/src*(.bss .bss.*) }
    . = ALIGN(0x1000);
    __user_end = .;

    _end = .; /* End address of loaded program data */
    
    /* DISCARD certain sections from the final output file, like comments, metadata and debugging information */
   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}

/* .gnu.linkonce.d -> used by the GNU linker for certain optimizations
*  for example, merge identical constants across various files into a single section to save space and access times */
__bss_size = (__bss_end - __bss_start)>>3; /* Size of the BSS section */

