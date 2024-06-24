#ifndef GIC_H
#define GIC_H

// Register addresses for the GIC-400 General Interrupt Controller

// Register base addresses extracted from the Raspberry Pi 5 Devicetree

// Register offsets are extracted from the "CoreLink GIC-400 Generic Interrupt
// Controller" Technical Reference Manual

#include "mmu.h"
enum {
  // Base address of the GIC-400 General Interrupt Controller
  GIC_BASE = 0x107fff8000 + VA_START,

  // GIC-400 Distributor Register
  GIC_D_BASE =
      GIC_BASE + 0x1000, // Base address of the GIC-400 Distributor Register

  GIC_D_CTLR = GIC_D_BASE + 0x000, // Distributor Control Register

  GIC_D_ENABLE = GIC_D_BASE + 0x100,  // Interrupt Set-Enable Register
  GIC_D_DISABLE = GIC_D_BASE + 0x180, // Interrupt Clear-Enable Register

  GIC_D_PENDING_CLR = GIC_D_BASE + 0x280, // Interrupt Clear-Pending Register
  GIC_D_ACTIVE_CLR = GIC_D_BASE + 0x380,  // Interrupt Clear-Active Register

  GIC_D_PRI = GIC_D_BASE + 0x400,    // Interrupt Priority Register
  GIC_D_TARGET = GIC_D_BASE + 0x800, // Interrupt Processor Targets Register
  GIC_D_CONFIG = GIC_D_BASE + 0xc00, // Interrupt Configuration Register

  // GIC-400 CPU Interface Register
  GIC_C_BASE =
      GIC_BASE + 0x2000, // Base address of the GIC-400 CPU Interface Register

  GIC_C_CTLR = GIC_C_BASE + 0x000,     // CPU Interface Control Register
  GIC_C_PRI_MASK = GIC_C_BASE + 0x004, // Interrupt Priority Mask Register
  GIC_C_ACK = GIC_C_BASE + 0x00c,      // Interrupt Acknowledge Register
  GIC_C_EOI = GIC_C_BASE + 0x010,      // End of Interrupt Register

  GIC_NUM_SPI = 192, // Number of SPIs configured in the GIC-400 in the SoC
  GIC_NUM_IRQ = GIC_NUM_SPI + 32, // Total number of IRQs in the GIC-400
  // The first 32 IRQs are reserved for the CPU internal exceptions (SGIs -
  // Software Generated Interrupts and PPIs - Private Peripheral Interrupts)

  GIC_SPURIOUS_INTERRUPT = 1023, // Spurious interrupt number
};

// Member APIs

/*! \brief Initialize the GIC-400 General Interrupt Controller. This function
 * initializes the GIC-400 Distributor and CPU Interface Registers to enable the
 * interrupt handling.
 */
void gic_init();

/*! \brief Enable an IRQ line in the GIC-400 Distributor Register.
 *
 * \param `irq` The IRQ line to enable.
 */
void enable_irq_line(unsigned int irq);

#endif // !GIC_H
