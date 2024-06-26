#ifndef AP_H
#define AP_H

// Register addresses extracted from the Raspberry Pi 5 Devicetree

#include "mmu.h"
enum {
  SOC_PERIPHERAL_BASE =
      0x107c000000 + VA_START, // Base address of the peripherals which
                               // are on the SoC directly (added with the start
                               // of higher half memory region)
};

#endif // !AP_H
