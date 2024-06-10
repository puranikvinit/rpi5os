#include "interrupts/gic.h"
#include "mmio.h"
#include "peripherals/system_timer.h"
#include "peripherals/uart.h"
#include "util/string.h"

void gic_init() {
  for (int i = 0; i < GIC_NUM_IRQ >> 5; i++) {
    // mmio_write_32(GIC_D_DISABLE + 4 * i, 0xffffffff);

    mmio_write_32(GIC_D_DISABLE + 4 * i, 0);
  }
  uart_puts("a\n\0");

  mmio_write_32(GIC_C_PRI_MASK, 0xff);
  uart_puts("b\n\0");

  for (int i = 0; i < GIC_NUM_IRQ >> 2; i++) {
    mmio_write_32(GIC_D_PRI + 4 * i, 0x7f7f7f7f);
  }
  uart_puts("c\n\0");

  for (int i = 0; i < GIC_NUM_IRQ >> 4; i++) {
    mmio_write_32(GIC_D_CONFIG + 4 * i, 0x55555555);
  }
  uart_puts("d\n\0");

  for (int i = 0; i < GIC_NUM_IRQ >> 2; i++) {
    if (i < 8)
      continue;
    mmio_write_32(GIC_D_TARGET + 4 * i, 0xffffffff);
  }
  uart_puts("e\n\0");

  mmio_write_32(GIC_D_CTLR, 0x1);
  uart_puts("f\n\0");

  mmio_write_32(GIC_C_CTLR, 0x1);
  uart_puts("g\n\0");
}

void register_irq_handler(unsigned int irq) {
  unsigned int val = 0x1 << (irq & 0x1f);
  unsigned int n = irq >> 5;

  mmio_write_32(GIC_D_ENABLE + 4 * n, val);
}

void gic_dispatch() {
  while (1) {
    unsigned int interrupt_spi = mmio_read_32(GIC_C_ACK);
    unsigned int interrupt_id = interrupt_spi & 0x3FF;
    uart_puts("Dispatch Triggered!\n\0");

    if (interrupt_id >= GIC_NUM_IRQ) {
      uart_puts("Spurious interrupt received: \0");
      char buff[9] = "";
      int_to_str(interrupt_id, buff);
      uart_puts(buff);
      uart_putc('\n');
      timer_init();
      uart_puts("Timer Initialized!\n\0");

      __asm__("msr daif, xzr\n\t"
              "wfi\n\t");
      continue;
    }

    if (interrupt_id == 30) {
      uart_puts("Timer interrupt received!\n\0");
    } else {
      uart_puts("Unknown interrupt received!\n\0");
    }
    mmio_write_32(GIC_C_EOI, interrupt_spi);
  }
}
