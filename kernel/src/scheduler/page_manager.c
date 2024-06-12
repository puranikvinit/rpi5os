#include "scheduler/page_manager.h"

static unsigned short page_map[NUMBER_OF_PAGES] = {
    0,
};

unsigned long allocate_free_page() {
  for (int i = 0; i < NUMBER_OF_PAGES; i++) {
    if (page_map[i] == 0) {
      page_map[i] = 1;
      return LOW_MEMORY + i * PAGE_SIZE;
    }
  }
  return 0;
}

void free_page(unsigned long page) {
  page_map[(page - LOW_MEMORY) / PAGE_SIZE] = 0;
}
