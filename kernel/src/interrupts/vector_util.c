#include "peripherals/uart.h"
#include "util/string.h"

void display_message(int interrupt_type, unsigned long elr, unsigned long esr) {
  uart_puts("Interrupt Type: \0");
  char buff[3] = "";
  int_to_str(interrupt_type, buff,
             10); // TODO: Show name rather than number of the interrupt
  uart_puts(buff);
  uart_puts("\n\0");

  char buff2[20] = "";
  uart_puts("ELR: \0");
  int_to_str(elr, buff2, 16);
  uart_puts(buff2);
  uart_puts("\n\0");

  char buff3[20] = "";
  uart_puts("ESR: \0");
  int_to_str(esr, buff3, 2);
  uart_puts(buff3);
  uart_puts("\n\0");
}
