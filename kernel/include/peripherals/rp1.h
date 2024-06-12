#ifndef RP1_H
#define RP1_H

// Register addresses extracted from the Raspberry Pi 5 Devicetree

enum {
  BAR1_BASE = 0x1f00000000, // System Base address to which APB0 PERIPHERALS
                            // base address of RP1 are mapped
  MAP_START = 0x40000000, // Peripheral Base in the RP1 Processor Address space

  PCIE_IRQ_1 = 229 + 32, // PCIe IRQ 1
  PCIE_IRQ_2 = 230 + 32, // PCIe IRQ 2
  PCIE_IRQ_3 = 231 + 32, // PCIe IRQ 3
  PCIE_IRQ_4 = 232 + 32, // PCIe IRQ 4
};

#endif // !RP1_H
