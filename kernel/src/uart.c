#include "uart.h"
#include "core/mmio.h"
#include "gpio.h"

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
  gpio_init(14, 4, 0, 0, 0, 1);
  // GPIO15 - UART0_RX
  // GPIO Pin Function = a4
  // Pull-up = 0
  // Pull-down = 0
  // Output disable = 0
  // Input enable = 1
  gpio_init(15, 4, 0, 0, 0, 1);

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
