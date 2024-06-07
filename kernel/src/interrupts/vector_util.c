#include "peripherals/uart.h"
#include "util/string.h"

void display_message(int interrupt_type, unsigned long elr, unsigned long esr) {
  uart_puts("Interrupt Type: \0");
  char buff[9] = "";
  int_to_str(interrupt_type,
             buff); // TODO: Show name rather than number of the interrupt
  uart_puts(buff);
  uart_puts("\n\0");

  str_empty(buff);
  uart_puts("ELR: \0");
  int_to_str(elr, buff);
  uart_puts(buff);
  uart_puts("\n\0");

  str_empty(buff);
  uart_puts("ESR: \0");
  int_to_str(esr, buff);
  uart_puts(buff);
  uart_puts("\n\0");
}
