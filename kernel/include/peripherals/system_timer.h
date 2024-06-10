#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include "mmio.h"
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
  SYSTEM_TIMER_C1 = SOC_PERIPHERAL_BASE + 0x3010,
  SYSTEM_TIMER_C3 = SOC_PERIPHERAL_BASE + 0x3018, // System Timer Compare 3

  SYSTEM_MASKBIT = 3,

  SYSTEM_TIMER_IRQ_0 = 1 << 0,
  SYSTEM_TIMER_IRQ_1 = 1 << 1,
  SYSTEM_TIMER_IRQ_2 = 1 << 2,
  SYSTEM_TIMER_IRQ_3 = 1 << 3,
};

// Member APIs
extern void timer_init();

#endif // !SYSTEM_TIMER_H
