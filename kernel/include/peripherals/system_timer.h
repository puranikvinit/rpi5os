#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include "ap.h"

#define SYSTEM_TIMER_TRIGGER_INTERVAL 200000

// Register addresses for the Raspberry Pi 5 System Timer

// Register base addresses extracted from the Raspberry Pi 5 Devicetree

// Register offsets are extracted from the "BCM2835 ARM Peripherals" Datasheet
// (The system timer used in the Pi 5 is compatible with the system timer used
// in BCM2835 SoC, as mentioned in the device tree)

enum {
  SYSTEM_TIMER_CTRL_STATUS =
      SOC_PERIPHERAL_BASE +
      0x3000, // Base address of the System Timer Register, also the Control &
              // Status register.

  SYSTEM_TIMER_LO =
      SOC_PERIPHERAL_BASE + 0x3004, // The lower 32 bits of the timer counter.
  // NOTE: The higher 32 bits of the timer counter are not accessible.

  // The lower 32 bits of the free running counter is matched against the
  // following compare registers, one for each line.
  SYSTEM_TIMER_C0 =
      SOC_PERIPHERAL_BASE + 0x300c, // System Timer Compare for channel 0
  SYSTEM_TIMER_C1 =
      SOC_PERIPHERAL_BASE + 0x3010, // System Timer Compare for channel 1
  SYSTEM_TIMER_C2 =
      SOC_PERIPHERAL_BASE + 0x3014, // System Timer Compare for channel 2
  SYSTEM_TIMER_C3 =
      SOC_PERIPHERAL_BASE + 0x3018, // System Timer Compare for channel 3

  // Status bit for channel 1
  SYSTEM_TIMER_MASKBIT = 1 << 1,

  SYSTEM_TIMER_IRQ_0 = 64 + 32, // The IRQ ID for channel 0
  SYSTEM_TIMER_IRQ_1 = 65 + 32, // The IRQ ID for channel 1
  SYSTEM_TIMER_IRQ_2 = 66 + 32, // The IRQ ID for channel 2
  SYSTEM_TIMER_IRQ_3 = 67 + 32, // The IRQ ID for channel 3
  // For the above channel lines, the GIC_SPI IDs have been extracted from the
  // devicetree of the Raspberry Pi 5 The IRQ IDs are calculated by adding 32 to
  // the GIC_SPI IDs (as the first 32 IRQs are reserved for SGIs and PPIs)
};

// Member APIs

/*! \brief Initialize the System Timer. This function initializes the System
 * Timer to generate an interrupt after 200ms from the time of execution of the
 * initialiser.
 */
void timer_init();

/*! \brief Handle the System Timer interrupt. This function is called when the
 * System Timer generates an interrupt.
 */
void handle_timer_irq();

#endif // !SYSTEM_TIMER_H
