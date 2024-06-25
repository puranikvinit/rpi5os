#include "system_registers.h"
#include "mmu.h"

.section ".text.boot"

.global _start

_start:

    mrs x7, mpidr_el1 // Multi-Processor Identifier(EL1)
    lsr x7, x7, #8 // Shift the identifier to the right by 8 bits.
    and x7, x7, #7 // Last 3 bits of the identifier extracted. Indicates CPU ID.
    cbz x7, intr_init // Proceed if on primary core, else wait for the primary core to initialize.
    
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

// Initialise required system registers required before jumping into EL1 exception level.
intr_init:  ldr x0, =SCTLR_VALUE_MMU_DISABLED
            msr sctlr_el1, x0

            ldr x0, =HCR_VALUE
            msr hcr_el2, x0

            ldr x0, =SPSR_VALUE
            msr spsr_el2, x0

            ldr x0, =TCR_VALUE
            msr tcr_el1, x0

            ldr x0, =MAIR_VALUE
            msr mair_el1, x0

            adr x0, bss_init
            msr elr_el2, x0

            eret

bss_init: adrp x1, __bss_start // Load the start address of the BSS section, defined in the linker.
          adrp x2, __bss_size // Load the end address of the BSS section, defined in the linker.

bss_loop: cbz x2, stack_init // Skip initialization of BSS variables if the size is `0`

          str xzr, [x1], #8 // Initialize all the BSS variables to `0`
          sub x2, x2, #1
          cbnz x2, bss_loop // Iterate for all variables of the BSS section

stack_init: adrp x0, _start
            mov sp, x0

            bl map_identity
            bl map_high

            adrp x0, id_pg_dir
            msr ttbr0_el1, x0
            
            adrp x0, high_pg_dir
            msr ttbr1_el1, x0

            tlbi vmalle1
            dsb ish
            isb
            mrs x0, sctlr_el1
            ldr x1, =SCTLR_MMU_ENABLED
            orr x0, x0, x1
            msr sctlr_el1, x0
            
            isb
            dc ivac, xzr
            dsb ish
            isb

            ldr x0, =VA_START
            add sp, sp, x0

            adr x1, kernel_main
            add x1, x1, x0

            mov x0, #0 // Pass core ID as an argument to the `kernel_main()` routine of the kernel
            blr x1 // Jump to the `kernel_main()` routine of the kernel

            b core_hang // If the `kernel_main()` routine of the kernel returns, loop indefinitely


.macro create_table_entry, table, next_table, va_start, shift, t1, t2
lsr \t1, \va_start, #\shift // Shift the virtual address to the right by the shift value
and \t1, \t1, #ENTRIES_PER_TABLE - 1 // Extract the index of the entry in the table, by masking the bits to the left of the index.

mov \t2, \next_table // Load the address of the next table into a register
orr \t2, \t2, #MM_TYPE_PAGE_TABLE // Set the type of the table to a page table
str \t2, [\table, \t1, lsl #3] // Store the table descriptor entry in the table at the calculated index
.endm


.macro create_block_map, table, va_start, va_end, pa_start, flags, t1
lsr \va_start, \va_start, #SECTION_SHIFT // Shift the virtual address to the right by the section shift value
and \va_start, \va_start, #ENTRIES_PER_TABLE - 1

lsr \va_end, \va_end, #SECTION_SHIFT
sub \va_end, \va_end, #1
and \va_end, \va_end, #ENTRIES_PER_TABLE - 1

lsr \pa_start, \pa_start, #SECTION_SHIFT
lsl \pa_start, \pa_start, #SECTION_SHIFT

mov \t1, \flags
orr \pa_start, \pa_start, \t1

9999: str \pa_start, [\table, \va_start, lsl #3]
      add \va_start, \va_start, #1
      add \pa_start, \pa_start, #SECTION_SIZE
      cmp \va_start, \va_end
      b.le 9999b

.endm

map_identity:
  mov x29, x30

  adrp x0, id_pg_dir
  mov x1, #ID_MAP_TABLE_SIZE
  bl mem_init_zero

smi:  adrp x0, id_pg_dir
  add x1, x0, #PAGE_SIZE
  eor x4, x4, x4
  create_table_entry x0, x1, x4, PGD_SHIFT, x2, x3

  add x0, x0, #PAGE_SIZE
  add x1, x1, #PAGE_SIZE
  eor x4, x4, x4
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

smi1:  mov x0, x1
  eor x2, x2, x2
  ldr x3, =ID_MAP_SIZE
  eor x4, x4, x4
  ldr x6, =MMU_KERNEL_FLAGS
  create_block_map x0, x2, x3, x4, x6, x5

smi2:  mov x30, x29
  ret


map_high:
  mov x29, x30

  adrp x0, high_pg_dir
  mov x1, #HIGH_MAP_TABLE_SIZE
  bl mem_init_zero

smh:  adrp x0, high_pg_dir
  add x1, x0, #PAGE_SIZE

  ldr x4, =VA_START
  create_table_entry x0, x1, x4, PGD_SHIFT, x2, x3

  add x0, x0, #PAGE_SIZE
  add x1, x1, #PAGE_SIZE

  ldr x4, =VA_START
  ldr x5, =PUD_ENTRY_MAP_SIZE
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

  add x1, x1, #PAGE_SIZE
  add x4, x4, x5
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

  add x1, x1, #PAGE_SIZE
  add x4, x4, x5
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

  add x1, x1, #PAGE_SIZE
  add x4, x4, x5
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

  add x1, x1, #PAGE_SIZE
  ldr x4, =VA_START
  ldr x5, =PHYSICAL_SOC_START
  add x4, x4, x5
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3

  add x1, x1, #PAGE_SIZE
  ldr x4, =VA_START
  ldr x5, =PHYSICAL_DEVICE_START
  add x4, x4, x5
  create_table_entry x0, x1, x4, PUD_SHIFT, x2, x3


  ldr x10, =HIGH_MAP_FIRST_START
  ldr x11, =HIGH_MAP_FIRST_END

  ldr x12, =HIGH_MAP_SECOND_START
  ldr x13, =HIGH_MAP_SECOND_END

  ldr x14, =HIGH_MAP_THIRD_START
  ldr x15, =HIGH_MAP_THIRD_END

  ldr x16, =HIGH_MAP_FOURTH_START
  ldr x17, =HIGH_MAP_FOURTH_END

  ldr x18, =HIGH_MAP_SOC_START
  ldr x19, =HIGH_MAP_SOC_END

  ldr x20, =HIGH_MAP_DEVICE_START
  ldr x21, =HIGH_MAP_DEVICE_END

  ldr x22, =PHYSICAL_FIRST_START
  ldr x23, =PHYSICAL_SECOND_START
  ldr x24, =PHYSICAL_THIRD_START
  ldr x25, =PHYSICAL_FOURTH_START
  ldr x26, =PHYSICAL_SOC_START
  ldr x27, =PHYSICAL_DEVICE_START

smm1:  add x0, x0, #PAGE_SIZE
  mov x2, x10
  mov x3, x11
  mov x4, x22
  ldr x6, =MMU_KERNEL_FLAGS
  create_block_map x0, x2, x3, x4, x6, x5

smm2:  add x0, x0, #PAGE_SIZE
  mov x2, x12
  mov x3, x13
  mov x4, x23
  create_block_map x0, x2, x3, x4, x6, x5

smm3:  add x0, x0, #PAGE_SIZE
  mov x2, x14
  mov x3, x15
  mov x4, x24
  create_block_map x0, x2, x3, x4, x6, x5

smm4:  add x0, x0, #PAGE_SIZE
  mov x2, x16
  mov x3, x17
  mov x4, x25
  create_block_map x0, x2, x3, x4, x6, x5

smm7:  add x0, x0, #PAGE_SIZE
  mov x2, x18
  mov x3, x19
  mov x4, x26
  ldr x6, =MMU_PERIPHERALS_FLAGS
  create_block_map x0, x2, x3, x4, x6, x5

smm5:  add x0, x0, #PAGE_SIZE
  mov x2, x20
  mov x3, x21
  mov x4, x27
  create_block_map x0, x2, x3, x4, x6, x5

smm6:  mov x30, x29
  ret


