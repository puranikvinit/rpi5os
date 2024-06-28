#include "types.h"
#include "util/printk.h"

void display_message(int interrupt_type, u64 elr, unsigned long esr) {
  printk(PANIC, "INTERRUPT HANDLER NOT FOUND\0");

  printk(INFO, "interrupt type: %d\0", interrupt_type);
  printk(INFO, "ELR: %x\0", elr);
  printk(INFO, "ESR: %b\0", esr);
}
