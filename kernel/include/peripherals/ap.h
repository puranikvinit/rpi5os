#ifndef AP_H
#define AP_H

// Register addresses extracted from the Raspberry Pi 5 Devicetree

enum {
  SOC_PERIPHERAL_BASE = 0x107c000000, // Base address of the peripherals which
                                      // are on the SoC directly
};

#endif // !AP_H
