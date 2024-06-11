#ifndef VECTOR_INIT_H
#define VECTOR_INIT_H

/*! \brief Initialize the vector table.
 *
 * This function initializes the vector table with the exception handlers for
 * the processor.
 */
extern void vector_table_init();

/*! \brief Enable the interrupts.
 *
 * This function enables the interrupts in the processor.
 */
extern void interrupts_enable();

/*! \brief Disable the interrupts.
 *
 * This function disables the interrupts in the processor.
 */
extern void interrupts_disable();

#endif // !VECTOR_INIT_H
