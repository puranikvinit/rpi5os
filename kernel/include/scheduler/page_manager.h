#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "scheduler/sched.h"

// Member APIs

/*! \brief Allocate a free page from the page pool
 */
unsigned long allocate_free_page();

/*! \brief Free a page and add it back to the page pool
 *
 * \param page The page to be freed
 */
void free_page(unsigned long page);

/*! \brief Allocate a page for a kernel process, in the higher half memory
 */
unsigned long allocate_kernel_page();

/*! \brief Allocate a page for a user process, in the lower half memory
 *
 * \param task The task to which the page is to be allocated
 * \param virt_addr The virtual address to which the page is to be mapped
 */
unsigned long allocate_user_page(task_struct_t *task, unsigned long virt_addr);

/*! \brief Map a user process page to a virtual address
 *
 * \param task The task to which the page is to be mapped
 * \param virt_addr The virtual address to which the page is to be mapped
 * \param page The page to be mapped
 */
void map_page(task_struct_t *task, unsigned long virt_addr, unsigned long page);
unsigned long map_table(unsigned long *table, unsigned long shift,
                        unsigned long virt_addr, int *status_code);
void map_table_entry(unsigned long *pte, unsigned long virt_addr,
                     unsigned long physical_addr);

/*! \brief Copies the pages of a userspace process into the page allocated to
 * it, and prepares switch to user mode.
 *
 * \param dest The page to which the pages are to be copied
 */
int copy_virt_mem(task_struct_t *dest);

/*! \brief Handle memory aborts
 *
 * \param addr The address at which the memory abort occurred
 * \param esr_val The ESR value
 */
int mem_abort_handler(unsigned long addr, unsigned long esr_val);

#endif // !PAGE_MANAGER_H
