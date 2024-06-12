#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#define PAGE_SIZE (1 << 12)
#define SECTION_SIZE (1 << 21)

#define LOW_MEMORY (2 * SECTION_SIZE)
#define HIGH_MEMORY (512 * SECTION_SIZE)

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define NUMBER_OF_PAGES (PAGING_MEMORY / PAGE_SIZE)

// Member APIs

unsigned long allocate_free_page();
void free_page(unsigned long page);

#endif // !PAGE_MANAGER_H
