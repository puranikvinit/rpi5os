#include "peripherals/system_timer.h"
#include "mmio.h"
#include "peripherals/uart.h"
#include "scheduler/sched.h"
#include "util/printk.h"

unsigned int curVal = 0;

void timer_init() {
  // Set the interval for the timer.
  curVal = mmio_read_32(SYSTEM_TIMER_LO);
  curVal += SYSTEM_TIMER_TRIGGER_INTERVAL;
  mmio_write_32(SYSTEM_TIMER_C1, curVal);
}

void handle_timer_irq() {
  // Increment the current value of the timer by the interval, and set the
  // timer.
  curVal += SYSTEM_TIMER_TRIGGER_INTERVAL;
  mmio_write_32(SYSTEM_TIMER_C1, curVal);
  mmio_write_32(SYSTEM_TIMER_CTRL_STATUS, SYSTEM_TIMER_MASKBIT);
  printk(INFO, "timer interrupt received\0");
  timer_tick();
}
