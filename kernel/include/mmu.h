#ifndef MMU_H
#define MMU_H

// #define VA_START 0xffff000000000000
#define VA_START 0xffff000000000000
#define PHYSICAL_MEM_SIZE 0x100000000 // 4GiB

#define ID_MAP_PAGES 3
#define ID_MAP_TABLE_SIZE (ID_MAP_PAGES * PAGE_SIZE)

#define HIGH_MAP_PAGES 8
#define HIGH_MAP_TABLE_SIZE (HIGH_MAP_PAGES * PAGE_SIZE)

#define HIGH_MAP_FIRST_START (0x0 + VA_START)
#define HIGH_MAP_FIRST_END (0x3f800000 + VA_START)

#define HIGH_MAP_SECOND_START (0x40000000 + VA_START)
#define HIGH_MAP_SECOND_END (0x80000000 + VA_START)

#define HIGH_MAP_THIRD_START (0x80000000 + VA_START)
#define HIGH_MAP_THIRD_END (0xc0000000 + VA_START)

#define HIGH_MAP_FOURTH_START (0xc0000000 + VA_START)
#define HIGH_MAP_FOURTH_END (0x100000000 + VA_START)

#define HIGH_MAP_SOC_START (0x1040000000 + VA_START)
#define HIGH_MAP_SOC_END (0x1080000000 + VA_START)

#define HIGH_MAP_DEVICE_START (0x1f00000000 + VA_START)
#define HIGH_MAP_DEVICE_END (0x1f40000000 + VA_START)

#define PHYSICAL_FIRST_START 0x0
#define PHYSICAL_FIRST_END 0x3f800000

#define PHYSICAL_SECOND_START 0x40000000
#define PHYSICAL_SECOND_END 0x80000000

#define PHYSICAL_THIRD_START 0x80000000
#define PHYSICAL_THIRD_END 0xc0000000

#define PHYSICAL_FOURTH_START 0xc0000000
#define PHYSICAL_FOURTH_END 0x100000000

#define PHYSICAL_SOC_START 0x1040000000

#define PHYSICAL_DEVICE_START 0x1f00000000

#define PAGE_SHIFT 12
#define TABLE_SHIFT                                                            \
  9 // Width of index in each of the page tables, in the virtual address
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT) // 4KiB -> Size of a page
#define SECTION_SIZE                                                           \
  (1 << SECTION_SHIFT) // 2MiB -> Size of a section mapped by a single entry in
                       // the PMD table

#define ID_MAP_SIZE (8 * SECTION_SIZE)

#define LOW_MEMORY                                                             \
  (2 * SECTION_SIZE) // Start of the low memory region to be mapped

// NOTE: In the Raspberry Pi 5, the address to peripheral control registers need
// not be mapped to virtual addresses, neither does the `HIGH_MEMORY` have to
// subtract the offset of the PERIPHERAL_BASE from itself for the mapping.
// Hence, we can use the entire 4GiB of addressable memory for Virtual
// Addressing.

#define PAGING_MEMORY (PHYSICAL_FOURTH_END - PHYSICAL_SECOND_START)
#define NUMBER_OF_PAGES (PAGING_MEMORY / PAGE_SIZE)

#define ENTRIES_PER_TABLE (1 << TABLE_SHIFT)

#define PGD_SHIFT (PAGE_SHIFT + 3 * TABLE_SHIFT)
#define PUD_SHIFT (PAGE_SHIFT + 2 * TABLE_SHIFT)
#define PMD_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PG_DIR_SIZE                                                            \
  (6 * PAGE_SIZE) // 6 pages for the PGD_DIR, as explained in the linker

#define PUD_ENTRY_MAP_SIZE (1 << PUD_SHIFT)

// ************************************************************
//
// Translation Table Descriptors
//
// [0] -> Valid bit (to state whether the translation entry is valid or not)
// [1] -> Block/Table bit (to state whether the entry is a table index or a
// section map) (0b0 -> Block, 0b1 -> Table)
// [11:2] -> Lower attributes
// [47:12] -> Address bits
// [63:48] -> Upper attributes
//
// ************************************************************

// Predefined values for translation table descriptors
#define MM_TYPE_PAGE_TABLE 0x3
#define MM_TYPE_PAGE 0x3
#define MM_TYPE_BLOCK 0x1
#define MM_ACCESS (1 << 10)
#define MM_ACCESS_PERMISSION (1 << 6)
#define MM_KERNEL_PERMS (1 << 54)
#define MM_USER_PERMS (1 << 6)

#define MM_PAGE_TABLE_ADDR_MASK 0xfffffffffffff000

#define MM_INNER_SHAREABLE (3 << 8)

// ************************************************************
//
// Memory Attribute Indirection Register (MAIR_EL1) attributes
//
// attribute_index = 0 -> DEVICE_nGnRnE; MAIR = 0b00000000 (0x00)
// attribute_index = 1 -> NORMAL_NC; MAIR = 0b01000100 (0x44)
//
// DEVICE MEMORY - Must be used for memory regions that cover peripheral control
// registers. It is the strongest memory type, and it is non-cacheable,
// non-shareable, and non-allocatable.
//
// nGnRnE -> Non-gathering, non-reordering, non-early write acknowledgment.
//
// NORMAL MEMORY - Must be used for memory regions that cover normal RAM.
// It is cacheable, shareable, and allocatable.
//
// NC -> Non-cacheable.
// ************************************************************

#define DEVICE_nGnRnE_ATTR_INDX 0x00
#define NORMAL_NC_ATTR_INDX 0x01

#define DEVICE_nGnRnE_ATTR 0x00
#define NORMAL_NC_ATTR 0x44

#define MAIR_VALUE                                                             \
  ((DEVICE_nGnRnE_ATTR << (DEVICE_nGnRnE_ATTR_INDX * 8)) |                     \
   (NORMAL_NC_ATTR << (NORMAL_NC_ATTR_INDX * 8)))

#define MMU_KERNEL_FLAGS                                                       \
  (MM_KERNEL_PERMS | MM_INNER_SHAREABLE | MM_TYPE_BLOCK |                      \
   (NORMAL_NC_ATTR_INDX << 2) | MM_ACCESS)

#define MMU_PERIPHERALS_FLAGS                                                  \
  (MM_KERNEL_PERMS | MM_INNER_SHAREABLE | MM_TYPE_BLOCK |                      \
   (DEVICE_nGnRnE_ATTR_INDX << 2) | MM_ACCESS)

#define MMU_PTE_FLAGS                                                          \
  (MM_TYPE_PAGE | (NORMAL_NC_ATTR_INDX << 2) | MM_ACCESS_PERMISSION | MM_ACCESS)

#define MMU_USER_PAGE_FLAGS                                                    \
  (MM_USER_PERMS | MM_INNER_SHAREABLE | MM_TYPE_PAGE |                         \
   (NORMAL_NC_ATTR_INDX << 2) | MM_ACCESS)

// NOTE: In the flags and attributes that are stored in the descriptor entries,
// the index to the attribute with respect to the `MAIR_EL1` register is stored,
// not the attributes themselves.

// ************************************************************
//
// Translation Control Register (TCR_EL1) configurations
// (for stage 1 translations)
//
// [5:0] -> T0SZ (Size offset of the memory region addressed by TTBR0_EL1)
// [21:16] -> T1SZ (Size offset of the memory region addressed by TTBR1_EL1)
//
// [15:14] -> TG0 (Granule size for TTBR0_EL1)
// 0b00 -> 4KiB
// 0b01 -> 64KiB
// 0b10 -> 16KiB
//
// [31:30] -> TG1 (Granule size for TTBR1_EL1)
// 0b01 -> 16KiB
// 0b10 -> 4KiB
// 0b11 -> 64KiB
//
// ************************************************************

#define TCR_T0SZ (64 - 48)
#define TCR_T1SZ ((64 - 48) << 16)

#define TCR_TG0_4K (0 << 14)
#define TCR_TG1_4K (2 << 30)

#define TCR_IPS (2 << 32)

#define TCR_VALUE (TCR_T0SZ | TCR_T1SZ | TCR_TG0_4K | TCR_TG1_4K | TCR_IPS)

#define DACR_VALUE 0xffffffff

#define CPACR_EL1_FPEN (1 << 21) | (1 << 20) // don't trap SIMD/FP registers
#define CPACR_EL1_ZEN (1 << 17) | (1 << 16)  // don't trap SVE instructions
#define CPACR_EL1_VAL (CPACR_EL1_FPEN | CPACR_EL1_ZEN)

#endif // !MMU_H
