#include "interrupts/gic.h"
#include "mmio.h"

void gic_init() {
  // Disable all interrupt lines.
  for (int i = 0; i < GIC_NUM_IRQ >> 5; i++) {
    mmio_write_32(GIC_D_DISABLE + 4 * i, 0xffffffff);
  }

  // Set priority mask register to the lowest value possible, to allow all
  // interrupts to be raised.
  mmio_write_32(GIC_C_PRI_MASK, 0xff);

  // Set the priority of all interrupts to be greater than the priority mask, or
  // else they will not be raised.
  for (int i = 0; i < GIC_NUM_IRQ >> 2; i++) {
    mmio_write_32(GIC_D_PRI + 4 * i, 0x7f7f7f7f);
  }

  // BIT[1] Set all interrupts to be level sensitive; active high.
  // BIT[0] RES1 (Specified handling mode of peripheral interrupts in primitive
  // architectures).
  for (int i = 0; i < GIC_NUM_IRQ >> 4; i++) {
    mmio_write_32(GIC_D_CONFIG + 4 * i, 0x55555555);
  }

  // Set all interrupts to be handled by all CPUs. (Specify target to be all the
  // CPU cores).
  // The first 32 bits are RES0.
  for (int i = 0; i < GIC_NUM_IRQ >> 2; i++) {
    if (i < 8)
      continue;
    mmio_write_32(GIC_D_TARGET + 4 * i, 0xffffffff);
  }

  // Enable the GIC Distributor and CPU Interface.
  mmio_write_32(GIC_D_CTLR, 0x1);
  mmio_write_32(GIC_C_CTLR, 0x1);
}

void enable_irq_line(unsigned int irq) {
  // Calculate the bit position of the interrupt line.
  unsigned int enable_bit = 0x1 << (irq & 0x1f);

  // Calculate the register offset of the interrupt line.
  unsigned int register_offset = irq >> 5;

  mmio_write_32(GIC_D_ENABLE + 4 * register_offset, enable_bit);
}
