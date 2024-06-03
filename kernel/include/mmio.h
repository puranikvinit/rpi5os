#ifndef MMIO_H
#define MMIO_H

// Register addresses extracted from the Raspberry Pi 5 Devicetree

enum {
  BAR1_BASE = 0x1f00000000, // System Base address to which APB0 PERIPHERALS
                            // base address of RP1 are mapped
  MAP_START = 0x40000000, // Peripheral Base in the RP1 Processor Address space
};

// Member APIs

/*! \brief Read a value from a memory-mapped I/O register.
 *
 * \param `reg` The register to read from.
 *
 * \return The value read from the register.
 */
extern unsigned int mmio_read_32(unsigned long);

/*! \brief Write a value into a memory-mapped I/O register.
 *
 * \param `reg` The register to write into.
 * \param `data` The value to write into the register.
 */
extern void mmio_write_32(unsigned long, unsigned int);

#endif // !MMIO_H
