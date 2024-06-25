#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "scheduler/sched.h"

// Member APIs

unsigned long allocate_free_page();
void free_page(unsigned long page);

unsigned long allocate_kernel_page();
unsigned long allocate_user_page(task_struct_t *task, unsigned long virt_addr);

void map_page(task_struct_t *task, unsigned long virt_addr, unsigned long page);
unsigned long map_table(unsigned long *table, unsigned long shift,
                        unsigned long virt_addr, int *status_code);
void map_table_entry(unsigned long *pte, unsigned long virt_addr,
                     unsigned long physical_addr);

int copy_virt_mem(task_struct_t *dest);

int mem_abort_handler(unsigned long addr, unsigned long esr_val);

#endif // !PAGE_MANAGER_H
