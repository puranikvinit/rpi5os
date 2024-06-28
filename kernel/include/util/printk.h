#ifndef PRINTK_H
#define PRINTK_H

#include <stdarg.h>

typedef enum {
  INFO,
  DEBUG,
  WARN,
  ERROR,
  PANIC,
} log_level;

void printk(log_level lvl, char *fmt, ...);

#endif // !PRINTK_H
