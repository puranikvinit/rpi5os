#ifndef GPIO_H
#define GPIO_H

#include "io/mmio.h"

// Register addresses extracted from the Raspberry Pi 5 Devicetree

enum {
  // Base address of the GPIO registers for each of the GPIO pins
  GPIO_BASE = BAR1_BASE + (0x400d0000 - MAP_START),
  GPIO0_FUNCSEL_OFFSET = 0x04, // [4:0] for pin function select
  GPIO_FUNCSEL_OFFSET_INCREMENT = 0x08,

  // Base address of the Registered I/O registers
  SYS_RIO0 = 0x400e0000,
  RIO0_XOR = SYS_RIO0 + 0x1000, // XOR register alias
  RIO0_SET = SYS_RIO0 + 0x2000, // SET register alias
  RIO0_CLR = SYS_RIO0 + 0x3000, // CLR register alias

  // Base address of the GPIO PAD registers for each of the GPIO pins
  GPIO_PAD = BAR1_BASE + (0x400f0000 - MAP_START),
  // [7] output disable
  // [6] input enable
  // [3] pull-up enable
  // [2] pull-down enable

  GPIO_PAD0_OFFSET = 0x04,
  GPIO_PAD_OFFSET_INCREMENT = 0x04
};

// Member APIs

/*! \brief Initialize the GPIO pin.
 *
 * \param `gpio_pin_number` The GPIO pin number to initialize.
 * \param `gpio_function` The GPIO function to select for the pin.
 * \param `gpio_pull_up` Enable pull-up resistor for the pin.
 * \param `gpio_pull_down` Enable pull-down resistor for the pin.
 * \param `gpio_disable_output` Disable output for the pin.
 * \param `gpio_enable_input` Enable input for the pin.
 *
 * \return `0` if the GPIO pin is initialized successfully, `-1` otherwise.
 */
int gpio_init(int gpio_pin_number, int gpio_function, int gpio_pull_up,
              int gpio_pull_down, int gpio_disable_output,
              int gpio_enable_input);
// NOTE: If both pull-up and pull-down are disabled, then the pin is in a
// pull-none state.

#endif // !GPIO_H
