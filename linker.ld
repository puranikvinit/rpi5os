SECTIONS
{
    . = 0x80000;     /* Address of first instruction */
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) } /* Ensure `.text` section starts with the bootloader code */
    
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }

        /* PROVIDE -> for variables which are referenced but not initialized in the section */
    PROVIDE(_data = .); /* Initialize data start address to current location pointer */
    .data : { *(.data .data.* .gnu.linkonce.d*) } /* Section for initialized data */
    
    .bss (NOLOAD) : { /* Section for uninitialized data; NOLOAD -> Don't allocate space in linking, will be allocated and initialized in runtime */
        . = ALIGN(16); /* Align the current location pointer to the next 16bit boundary */
        __bss_start = .; /* Start address of the BSS section */
        *(.bss .bss.*)
        *(COMMON) /* COMMON -> used for uninitialized global variables that are declared without an explicit section attribute in multiple files. */
        __bss_end = .; /* End address of the BSS section */
    }

    .stack (NOLOAD) : { /* Section for stack */
        . = ALIGN(16); /* Align the current location pointer to the next 8bit boundary */
        __stack_start = .; /* Start address of the stack */
        . += 0x1000; /* Reserve 4KiB for the stack */
        . = ALIGN(16);
        __stack_end = .; /* End address of the stack */
    }

    _end = .; /* End address of loaded program data */
    
    /* DISCARD certain sections from the final output file, like comments, metadata and debugging information */
   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}

/* .gnu.linkonce.d -> used by the GNU linker for certain optimizations
*  for example, merge identical constants across various files into a single section to save space and access times */

__bss_size = (__bss_end - __bss_start)>>3; /* Size of the BSS section */
__stack_size = (__stack_end - __stack_start); /* Size of the stack */
