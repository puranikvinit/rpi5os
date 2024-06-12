#ifndef MMIO_H
#define MMIO_H

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

extern int get_current_exception_level();

#endif // !MMIO_H
