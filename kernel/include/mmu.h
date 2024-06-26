#ifndef MMU_H
#define MMU_H

#define VA_START 0xffff000000000000
#define PHYSICAL_MEM_SIZE 0x100000000 // 4GiB

// ************************************************************
//
// *********** Memory Mapping for the Raspberry Pi 5 **********
//
// --- IDENTITY MAPPING ---
//
// 0x0 -- 0x1000000 (16MiB) -> 1:1 mapping
//
// Allotted to TTBR0_EL1. Contains the syscall table and a few other variables
// and structs required for the complete intialization of the kernel and its
// scheduler.
//
//
// --- HIGH MEMORY MAPPING ---
//
// 0x0 -- 0x3f800000 (1016GiB) -> 0xffff000000000000 -- 0xffff00003f800000
//
// 0x3f800000 -- 0x40000000 (8MiB) -> Reserved for VC
//
// 0x40000000 -- 0x80000000 (1GiB) -> 0xffff000040000000 -- 0xffff000080000000
// 0x80000000 -- 0xc0000000 (1GiB) -> 0xffff000080000000 -- 0xffff0000c0000000
// 0xc0000000 -- 0x100000000 (1GiB) -> 0xffff0000c0000000 -- 0xffff000100000000
//
// 0x1040000000 -- 0x1080000000 (1GiB) -> 0xffff010400000000 --
// 0xffff010800000000 (SoC peripherals)
// 0x1f00000000 -- 0x1f40000000 (1GiB) -> 0xffff1f00000000 --
// 0xffff1f40000000 (RP1 peripherals)
//
// Allotted to TTBR1_EL1. Contains the kernel code and data, and the memory
// allotted to kernel processes.
//
// ************************************************************

// 3 Pages -> 1 PGD, 1 PUD, 1 PMD tables
#define ID_MAP_PAGES 3
#define ID_MAP_TABLE_SIZE (ID_MAP_PAGES * PAGE_SIZE)

// 8 Pages -> 1 PGD, 1 PUD, 6 PMD tables
#define HIGH_MAP_PAGES 8
#define HIGH_MAP_TABLE_SIZE (HIGH_MAP_PAGES * PAGE_SIZE)

// Virtual address range for PMD_1 table entries
#define HIGH_MAP_FIRST_START (0x0 + VA_START)
#define HIGH_MAP_FIRST_END (0x3f800000 + VA_START)

// Virtual address range for PMD_2 table entries
#define HIGH_MAP_SECOND_START (0x40000000 + VA_START)
#define HIGH_MAP_SECOND_END (0x80000000 + VA_START)

// Virtual address range for PMD_3 table entries
#define HIGH_MAP_THIRD_START (0x80000000 + VA_START)
#define HIGH_MAP_THIRD_END (0xc0000000 + VA_START)

// Virtual address ange for PMD_4 table entries
#define HIGH_MAP_FOURTH_START (0xc0000000 + VA_START)
#define HIGH_MAP_FOURTH_END (0x100000000 + VA_START)

// Virtual address range for PMD_5 table entries
#define HIGH_MAP_SOC_START (0x1040000000 + VA_START)
#define HIGH_MAP_SOC_END (0x1080000000 + VA_START)

// Virtual address range for PMD_6 table entries
#define HIGH_MAP_DEVICE_START (0x1f00000000 + VA_START)
#define HIGH_MAP_DEVICE_END (0x1f40000000 + VA_START)

// Physical address range for PMD_1 table entries
#define PHYSICAL_FIRST_START 0x0
#define PHYSICAL_FIRST_END 0x3f800000

// Physical address range for PMD_2 table entries
#define PHYSICAL_SECOND_START 0x40000000
#define PHYSICAL_SECOND_END 0x80000000

// Physical address range for PMD_3 table entries
#define PHYSICAL_THIRD_START 0x80000000
#define PHYSICAL_THIRD_END 0xc0000000

// Physical address range for PMD_4 table entries
#define PHYSICAL_FOURTH_START 0xc0000000
#define PHYSICAL_FOURTH_END 0x100000000

// Physical address range for PMD_5 table entries
#define PHYSICAL_SOC_START 0x1040000000
#define PHYSICAL_SOC_END 0x108000000

// Physical address range for PMD_6 table entries
#define PHYSICAL_DEVICE_START 0x1f00000000
#define PHYSICAL_DEVICE_END 0x1f40000000

// Size of Page table -> 4KiB (2^12 bytes)
#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << PAGE_SHIFT) // 4KiB -> Size of a page

// Number of entries in each of the translation tables -> 512 (2^9)
#define TABLE_SHIFT                                                            \
  9 // Width of index in each of the page tables, in the virtual address
#define ENTRIES_PER_TABLE (1 << TABLE_SHIFT)

#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)
#define SECTION_SIZE                                                           \
  (1 << SECTION_SHIFT) // 2MiB -> Size of a section mapped by a single entry in
                       // the PMD table

// Size of the ID map -> 16MiB (8 * 2MiB)
#define ID_MAP_SIZE (8 * SECTION_SIZE)

// Total memory size that is available for kernel processes.
#define PAGING_MEMORY (PHYSICAL_FOURTH_END - PHYSICAL_SECOND_START)
#define NUMBER_OF_PAGES (PAGING_MEMORY / PAGE_SIZE)

// Number of bits to be shifted to the right to obtain the index in the PGD
// table
#define PGD_SHIFT (PAGE_SHIFT + 3 * TABLE_SHIFT)

// Number of bits to be shifted to the right to obtain the index in the PUD
// table
#define PUD_SHIFT (PAGE_SHIFT + 2 * TABLE_SHIFT)

// Number of bits to be shifted to the right to obtain the index in the PMD
// table
#define PMD_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

// Size of the memory that can be covered in one entry of the PUD table (1GiB =
// 1 << 30)
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
#define MM_INNER_SHAREABLE (3 << 8)

#define MM_ACCESS_PERMISSION (1 << 6)
#define MM_KERNEL_PERMS (1 << 54)
#define MM_USER_PERMS (1 << 6)

#define MM_PAGE_TABLE_ADDR_MASK 0xfffffffffffff000

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
// [34:32] -> IPS (Physical address space size)
// 0b000 -> 32 bits
// 0b001 -> 36 bits
// 0b010 -> 40 bits (for the Raspberry Pi 5)
// 0b011 -> 42 bits
// 0b100 -> 44 bits
// 0b101 -> 48 bits
// 0b110 -> 52 bits
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

#endif // !MMU_H
