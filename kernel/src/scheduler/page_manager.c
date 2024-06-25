#include "scheduler/page_manager.h"
#include "mmio.h"
#include "mmu.h"
#include "peripherals/uart.h"
#include "scheduler/sched.h"

static unsigned short page_map[NUMBER_OF_PAGES] = {
    0,
};

unsigned long allocate_free_page() {
  for (int i = 0; i < NUMBER_OF_PAGES; i++) {
    if (page_map[i] == 0) {
      page_map[i] = 1;
      unsigned long allocated_page = PHYSICAL_SECOND_START + i * PAGE_SIZE;
      unsigned long page_size = PAGE_SIZE;
      mem_init_zero(allocated_page + VA_START, page_size);

      return allocated_page;
    }
  }
  return 0;
}

void free_page(unsigned long page) {
  page_map[(page - PHYSICAL_SECOND_START) / PAGE_SIZE] = 0;
}

unsigned long allocate_kernel_page() {
  unsigned long page = allocate_free_page();
  if (!page)
    return 0;

  return page + VA_START;
}

unsigned long allocate_user_page(task_struct_t *task, unsigned long virt_addr) {
  unsigned long page = allocate_free_page();
  if (!page)
    return 0;

  map_page(task, virt_addr, page);
  return page + VA_START;
}

void map_page(task_struct_t *task, unsigned long virt_addr,
              unsigned long page) {
  int status_code;

  if (!task->mm.pgd_address) {
    task->mm.pgd_address = allocate_free_page();
    task->mm.kernel_pages[++task->mm.kernel_pages_count] = task->mm.pgd_address;
  }

  unsigned long pgd = task->mm.pgd_address;

  unsigned long pud = map_table((unsigned long *)(pgd + VA_START), PGD_SHIFT,
                                virt_addr, &status_code);
  if (status_code) {
    task->mm.kernel_pages[++task->mm.kernel_pages_count] = pud;
  }

  unsigned long pmd = map_table((unsigned long *)(pud + VA_START), PUD_SHIFT,
                                virt_addr, &status_code);
  if (status_code) {
    task->mm.kernel_pages[++task->mm.kernel_pages_count] = pmd;
  }

  unsigned long pte = map_table((unsigned long *)(pmd + VA_START), PMD_SHIFT,
                                virt_addr, &status_code);
  if (status_code) {
    task->mm.kernel_pages[++task->mm.kernel_pages_count] = pte;
  }

  map_table_entry((unsigned long *)(pte + VA_START), virt_addr, page);

  user_page_t user_page = {page, virt_addr};
  task->mm.user_pages[task->mm.user_pages_count++] = user_page;
}

unsigned long map_table(unsigned long *table, unsigned long shift,
                        unsigned long virt_addr, int *status_code) {
  unsigned long index = (virt_addr >> shift) & (ENTRIES_PER_TABLE - 1);
  if (!table[index]) {
    *status_code = 1;
    unsigned long next_level_table = allocate_free_page();
    unsigned long table_entry = next_level_table | MM_TYPE_PAGE_TABLE;
    table[index] = table_entry;

    return next_level_table;
  }

  *status_code = 0;
  return table[index] & MM_PAGE_TABLE_ADDR_MASK;
}

void map_table_entry(unsigned long *pte, unsigned long virt_addr,
                     unsigned long physical_addr) {
  unsigned long index = (virt_addr >> PAGE_SHIFT) & (ENTRIES_PER_TABLE - 1);
  pte[index] = (physical_addr | MMU_USER_PAGE_FLAGS);
}

int copy_virt_mem(task_struct_t *dest) {
  task_struct_t *src = current_task;
  for (int i = 0; i < src->mm.user_pages_count; i++) {
    unsigned long page =
        allocate_user_page(dest, src->mm.user_pages[i].virtual_address);
    if (!page)
      return -1;

    mem_copy(page, src->mm.user_pages[i].virtual_address, PAGE_SIZE);
  }

  return 0;
}

static int ind = 1;

int mem_abort_handler(unsigned long addr, unsigned long esr_val) {
  unsigned long dfs = esr_val & 0b111111;
  if ((dfs & 0b111100) == 0b100) {
    uart_puts("Hey Sexy!\n\0");
    unsigned long page = allocate_free_page();
    if (!page)
      return -1;

    map_page(current_task, addr & MM_PAGE_TABLE_ADDR_MASK, page);
    ind++;
    if (ind > 2)
      return -1;

    return 0;
  }
  return -1;
}
