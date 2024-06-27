#ifndef SYSTEM_REGISTERS_H
#define SYSTEM_REGISTERS_H

// ******************************************************************************
//
// PREDEFINED CONFIGS FOR SCTRL_EL1 (SYSTEM CONTROL REGISTER FOR EL1)
//
// [29]LSMAOE (0b1) - Load Multiple and Store Multiple set to atomic; memory
// accesses are required to be ordered. (register is permitted to be cached in
// TLB)
//
// [28]nTLSMD (0b1) - All memory accesses by A32 and T32 Load Multiple and Store
// Multiple at EL0 that are marked at stage 1 as
// Device-nGRE/Device-nGnRE/Device-nGnRnE memory are not trapped.
//
// [25]EE (0b0) - Exception Endianness is Little-endian.
//
// [24]E0E (0b0) - Endianness of explicit data access at EL0 is Little-endian.
//
// [23]SPAN (0b1) - Value of PAN (Privileged Access Never) is unchanged when
// exception level is switched to L1.
//
// [22]EIS (0b1) - The taking of an exception to EL1 is a context synchronizing
// event.
//
// [20]TSCXT (0b1) - EL0 access to SCXTNUM_EL0 (EL0 Read/Write Software Context
// Number) is disabled, causing an exception to EL1, or to EL2 when it is
// implemented and enabled for the current Security state and HCR_EL2.TGE is 1.
//
// [12]I (0b0) - Instruction cache disabled.
//
// [11]EOS (0b1) - An exception return from EL1 is a context synchronizing
// event.
//
// [2]C (0b0) - Data cache disabled.
//
// [0]M (0b0) - MMU disabled.
// [0]M (0b1) - MMU enabled.
//
// ******************************************************************************
#define SCTLR_RESERVED ((3 << 28) | (3 << 22) | (1 << 20) | (1 << 11))
#define SCTLR_EE_LITTLE_ENDIAN (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN (0 << 24)
#define SCTLR_I_CACHE_DISABLED (0 << 12)
#define SCTLR_D_CACHE_DISABLED (0 << 2)
#define SCTLR_MMU_DISABLED (0 << 0)
#define SCTLR_MMU_ENABLED (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED                                               \
  (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED |          \
   SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// ******************************************************************************
//
// PREDEFINED CONFIGS FOR HCR_EL2 (Hypervisor Configuration Register for EL2)
//
// [31]RW (0b1) - Execution state for EL1 is AArch64.
//
// ******************************************************************************
#define HCR_RW (1 << 31)
#define HCR_VALUE (HCR_RW)

// ******************************************************************************
//
// PREDEFINED CONFIGS FOR SPSR_EL2 (Saved Program Status Register for EL2)
//
// [8:6]MASK_ALL (0b111) - Mask all interrupts after switch to EL1.
// [2:0]EL1h (0b101) - Use EL1 dedicated stack pointer.
//
// ******************************************************************************
#define SPSR_MASK_ALL (7 << 6)
#define SPSR_EL1h (5 << 0)
#define SPSR_VALUE (SPSR_MASK_ALL | SPSR_EL1h)

// ******************************************************************************
//
// CONSTANTS FOR ESR_EL1 (Exception Syndrome Register for EL1)
//
// ******************************************************************************
#define ESR_EL1_EC_SHIFT 26
#define ESR_EL1_EC_SVC_64B 0x15
#define ESR_EL0_EC_DATA_ABORT 0x24

// ******************************************************************************
//
// PREDEFINED CONFIGS FOR CPACR_EL1 (Architectural Feature Access Control
// Register)
//
// [21:20]FPEN (0b11) - Traps execution at EL0 and EL1, of instructions that
// access the advanced SIMD and floating-point registers.
// 0b00 -> Traps at both EL0 and EL1.
// 0b01 -> Traps at EL0, not at EL1.
// 0b10 -> Traps at both EL0 and EL1.
// 0b11 -> Traps disabled for both EL0 and EL1.
//
// ******************************************************************************

#define CPACR_FPEN (3 << 20)
#define CPACR_VALUE (CPACR_FPEN)

#endif // !SYSTEM_REGISTERS_H
