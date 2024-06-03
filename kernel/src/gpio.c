#include "gpio.h"
#include "mmio.h"

int gpio_init(int gpio_pin_number, int gpio_function, int gpio_pull_up,
              int gpio_pull_down, int gpio_disable_output,
              int gpio_enable_input) {
  long gpio_pin_funcsel = GPIO_BASE + GPIO0_FUNCSEL_OFFSET +
                          (gpio_pin_number)*GPIO_FUNCSEL_OFFSET_INCREMENT;
  long gpio_pin_pad =
      GPIO_PAD + GPIO_PAD0_OFFSET + (gpio_pin_number)*GPIO_PAD_OFFSET_INCREMENT;

  // Set the GPIO pin function
  unsigned int func = mmio_read_32(gpio_pin_funcsel);
  func &= ~(0x1f); // Clear the function select bits [4:0] for the pin.

  // Check if the GPIO function provided is valid
  if (gpio_function > 31)
    return -1;
  // set the function select bits [4:0] for the pin.
  func |= gpio_function;

  mmio_write_32(gpio_pin_funcsel, func);

  // Set GPIO pad values
  unsigned int pad = mmio_read_32(gpio_pin_pad);
  // Clear the output disable[7], input enable[6], pull-up[3], and pull-down[2]
  // bits
  pad &= ~((1 << 7) | (1 << 6) | (1 << 3) | (1 << 2));

  // Check if the params provided are valid.
  if (gpio_pull_up != 1 && gpio_pull_up != 0)
    return -1;
  if (gpio_pull_down != 1 && gpio_pull_down != 0)
    return -1;
  if (gpio_disable_output != 1 && gpio_disable_output != 0)
    return -1;
  if (gpio_enable_input != 1 && gpio_enable_input != 0)
    return -1;

  // Set the input values at the appropriate bit positions.
  pad |= ((gpio_disable_output << 7) | (gpio_enable_input << 6) |
          (gpio_pull_up << 3) | (gpio_pull_down << 2));

  mmio_write_32(gpio_pin_pad, pad);

  return 0;
}
