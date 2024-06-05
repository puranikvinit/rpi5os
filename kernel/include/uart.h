#ifndef UART_H
#define UART_H

#include "core/mmio.h"

// ARM PrimeCell (PL011) UART (Revision r1p5) Implementation

// GPIO pin mapping:
// GPIO function selected for this implementation - a4
// UART1_TX - GPIO pin 14
// UART1_RX - GPIO pin 15
// UART1_CTS - GPIO pin 16
// UART1_RTS - GPIO pin 17

// Register addresses extracted from the Raspberry Pi 5 Devicetree

enum {
  UART_CLOCK_FREQ =
      0x2faf080, // Clock Frequency of dedicated UART clock (clk_uart) - 50MHz

  // Base address of the UART0 registers
  UART0_BASE = BAR1_BASE + (0x40030000 - MAP_START),
  UART0_CR = UART0_BASE + 0x030, // UART Control Register (WIDTH: 16 bits)
  // [9] RXE - RX enable
  // [8] TXE - TX enable
  // [0] UARTE - UART enable

  UART0_IBRD =
      UART0_BASE + 0x024, // Integer Baud Rate Register (WIDTH: 16 bits)
  UART0_FBRD =
      UART0_BASE + 0x028, // Fractional Baud Rate Register (WIDTH: 6 bits)
  UART0_LCR_H = UART0_BASE + 0x02c, // Line Control Register (WIDTH: 8 bits)

  UART0_DR = UART0_BASE + 0x000, // Data Register (WIDTH: 8 bits)
  UART0_FR = UART0_BASE + 0x018, // Flag Register (WIDTH: 9 bits)
                                 // [7] TXFE - TX FIFO empty
                                 // [6] RXFF - RX FIFO full
                                 // [5] TXFF - TX FIFO full
                                 // [4] RXFE - RX FIFO empty
                                 // [3] BUSY - UART busy
};

/*! \brief Structure to store the baud rate values.
 *
 *  \var `baud_rate_integer` The integer part of the baud rate divisor.
 *  \var `baud_rate_fraction` The fractional part of the baud rate divisor.
 * */

typedef struct {
  unsigned int baud_rate_integer;
  unsigned int baud_rate_fraction;
} uart_baud_rate_t;

// Member APIs

/*! \brief Calculate the baud rate divisor values for a given baud rate.
 *
 * \param `baud_rate` The baud rate to calculate the values for.
 *
 * \return The integer and fractional baud rate divisor values.
 */
uart_baud_rate_t _calculate_baud_values(long baud_rate);

/*! \brief Initialize the UART0 interface. */
void uart_init();

/*! \brief Transmit a character over the UART0 interface.
 *
 * \param `c` The character to transmit.
 */
void uart_putc(char c);

/*! \brief Receive a character over the UART0 interface.
 *
 * \return The character received.
 */
char uart_getc();

/*! \brief Transmit a string over the UART0 interface.
 *
 * \param `s` The pointer to the string to be transmitted.
 */
void uart_puts(const char *s);

#endif // !UART_H
