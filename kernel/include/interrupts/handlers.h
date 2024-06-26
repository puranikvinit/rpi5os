#ifndef HANDLERS_H
#define HANDLERS_H

/*! \brief Handle IRQs when the corresponding interrupt is raised.
 * This function (as of now) handles EL1h_IRQ and EL0_64_IRQ interrupts.
 */
void handle_irq();

/*! \brief Handle Synchronous exceptions when the corresponding exception is
 * raised. This function (as of now) handles EL1h_Synchronous exceptions.
 */
extern void handle_sync();

#endif // !HANDLERS_H
