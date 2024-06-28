#include "interrupts/gic.h"
#include "mmio.h"
#include "peripherals/rp1.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "util/printk.h"

void handle_irq() {
  // Read the interrupt ID from the GIC CPU Interface interrupt ACKNOWLEDGE
  // register.
  unsigned int interrupt_spi = mmio_read_32(GIC_C_ACK);
  unsigned int interrupt_id =
      interrupt_spi & 0x3ff; // Extract the interrupt ID, ignoring the CPU ID.

  // Trigger the respective interrupt handlers.
  switch (interrupt_id) {
  case SYSTEM_TIMER_IRQ_1:
    mmio_write_32(GIC_C_EOI, interrupt_id);
    handle_timer_irq();
    break;

  case PCIE_IRQ_4:
    mmio_write_32(GIC_C_EOI, interrupt_id);
    handle_uart_irq();
    break;

  default:
    mmio_write_32(GIC_C_EOI, interrupt_id);
    printk(ERROR, "unknown interrupt received\0");
    break;
  }
}
