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

/*! \brief Fetch the current exception level of the processor, at the time of
 * calling this function.
 *
 * \return The value extracted from the CurrentEL register, which specifies the
 * current exception level.
 */
extern int get_current_exception_level();

/*! \brief Initialise the memory region with zeroes.
 *
 * \param `start` The start address of the memory region.
 * \param `size` The size of the memory region.
 */
extern void mem_init_zero(unsigned long, unsigned long);

/*! \brief Copy the contents of one memory region to another.
 *
 * \param `dst` The destination address of the memory region.
 * \param `src` The source address of the memory region.
 * \param `size` The size of the memory region.
 */
extern void mem_copy(unsigned long, unsigned long, unsigned long);

/*! \brief Set the value of the Translation Table Base Register 0 (TTBR0_EL1)
 * for the upcoming userspace process.
 *
 * \param The base address of the translation table for the userspace process.
 */
extern void set_pgd(unsigned long);

#endif // !MMIO_H
