#include "core.h"
#include "string.h"
#include "uart.h"

char s[40] = "Hello, from Raspberry Pi 5!\n";
char buff[32] = "";

int main() {
  uart_init();

  uart_puts(s);

  int_to_str(get_current_exception_level(), buff);
  uart_puts(buff);

  while (1) {
  }

  return 0;
}
