#include "peripherals/uart.h"
#include "mmio.h"
#include "peripherals/gpio.h"

uart_baud_rate_t _calculate_baud_rate(long baud_rate) {
  double baud_rate_divisor = (double)UART_CLOCK_FREQ / (0x10 * baud_rate);
  unsigned int baud_rate_integer = (unsigned int)baud_rate_divisor;
  unsigned int baud_rate_fraction =
      (unsigned int)((baud_rate_divisor - baud_rate_integer) * 0x40 + 0.5);

  uart_baud_rate_t uart_baud_rate = {.baud_rate_integer = baud_rate_integer,
                                     .baud_rate_fraction = baud_rate_fraction};
  return uart_baud_rate;
}

void uart_init() {
  // GPIO14 - UART0_TX
  // GPIO Pin Function = a4
  // Pull-up = 0
  // Pull-down = 0
  // Output disable = 0
  // Input enable = 1
  // Interrupt enable = 1
  // Interrupt force = 0
  gpio_init(14, 4, 0, 0, 0, 1, 1, 0);
  // GPIO15 - UART0_RX
  // GPIO Pin Function = a4
  // Pull-up = 0
  // Pull-down = 0
  // Output disable = 0
  // Input enable = 1
  // Interrupt enable = 1
  // Interrupt force = 0
  gpio_init(15, 4, 0, 0, 0, 1, 1, 0);

  // NOTE: Flow control not yet implemented, hence the flow control pins are not
  // initialized.

  // Disable UART interface
  unsigned int uart_control = mmio_read_32(UART0_CR);
  uart_control &= ~(1); // Clear the UART enable[0] bit
  mmio_write_32(UART0_CR, uart_control);

  // Set the baud rate values
  uart_baud_rate_t uart_baud_rate = _calculate_baud_rate(9600);
  mmio_write_32(UART0_IBRD, uart_baud_rate.baud_rate_integer);
  mmio_write_32(UART0_FBRD, uart_baud_rate.baud_rate_fraction);

  // Set the line control register
  unsigned int uart_lcr_h = mmio_read_32(UART0_LCR_H);
  uart_lcr_h &= ~(7 << 4);
  uart_lcr_h |= (3 << 5); // 8-bit word length
  uart_lcr_h |= (1 << 4); // enable FIFO buffer
  mmio_write_32(UART0_LCR_H, uart_lcr_h);

  // Set the Interrupt FIFO level select register
  unsigned int uart_ifls = mmio_read_32(UART0_IFLS);
  uart_ifls &= ~(7 << 3); // Clear RX FIFO interrupt level bits
  uart_ifls |= (0 << 3);  // Set RX FIFO interrupt level to 1/8 full
  mmio_write_32(UART0_IFLS, uart_ifls);

  // Set the Interrupt Mask Set/Clear Register
  unsigned int uart_imsc = mmio_read_32(UART0_IMSC);
  uart_imsc &= ~(3 << 4); // Unmask TX and RX interrupt
  mmio_write_32(UART0_IMSC, uart_imsc);

  // Enable UART interface
  uart_control &= ~(1 | (3 << 8));
  uart_control |= (1 | (3 << 8)); // Set UART enable[0], TX[8] and RX[9] bits.
  mmio_write_32(UART0_CR, uart_control);
}

void uart_putc(char c) {
  // Wait until the UART is not busy
  // while (mmio_read(UART0_FR) & (1 << 3))
  // ;
  // Wait until the TX FIFO is not full
  while (mmio_read_32(UART0_FR) & (1 << 5))
    ;

  mmio_write_32(UART0_DR, c);
}

char uart_getc() {
  // Wait until the UART is not busy
  // while (mmio_read(UART0_FR) & (1 << 3))
  // ;
  // Wait until the RX FIFO is not empty
  while (mmio_read_32(UART0_FR) & (1 << 4))
    ;

  return (mmio_read_32(UART0_DR) & 0xff);
}

void uart_puts(const char *s) {
  for (int i = 0; s[i] != '\0'; i++)
    uart_putc((char)s[i]);
}

void handle_uart_irq() {
  mmio_write_32(UART0_ICR, 1 << 4); // Clear the RX interrupt
  uart_puts("UART Interrupt Received!\n\0");

  uart_putc(
      uart_getc()); // Echo the received character and clear the RX FIFO queue.
}
