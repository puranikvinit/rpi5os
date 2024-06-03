#include "uart.h"

int main() {
  uart_init();
  char *s = "Hello, from Raspberry Pi 5!\n";

  while (1) {
    uart_puts(s);
  }

  return 0;
}
