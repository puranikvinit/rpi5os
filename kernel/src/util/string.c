#include "util/string.h"
#include "peripherals/uart.h"

int str_length(const char *s) {
  register const char *str;

  for (str = s; *str; ++str)
    ;

  return (str - s);
}

void str_concat(char *src, char *dest) {
  int src_pointer = 0;
  for (; src[src_pointer] != '\0'; src_pointer++)
    ;

  for (int dest_pointer = 0; dest[dest_pointer] != '\0'; dest_pointer++)
    src[src_pointer++] = dest[dest_pointer];
  src[src_pointer] = '\0';
}

int str_to_int(char *s) {
  int res = 0;
  int is_negative = 0;

  for (int i = 0; i < str_length(s); i++) {
    if (s[i] == '-' && i == 0)
      is_negative = 1;
    else if (s[i] >= 48 && s[i] <= 57)
      res = (res * 10) + (s[i] - 48);
    else
      return -1;
  }

  return is_negative ? (-1 * res) : res;
}

void int_to_str(long num, char *buffer, int base) {
  if (num == 0) {
    str_concat(buffer, "0\0");
    return;
  }

  int is_negative = 0;
  if (num < 0 && base == 10) {
    num *= -1;
    is_negative = 1;
  }

  while (num != 0) {
    int digit = num % base;
    char c[2] = "";
    c[0] = (char)((digit < 10) ? (digit + 48) : (digit - 10 + 'a'));
    c[1] = '\0';
    str_concat(buffer, c);
    num = num / base;
  }

  switch (base) {
  case 2:
    str_concat(buffer, "b0\0");
    break;

  case 10:
    if (is_negative)
      str_concat(buffer, "-\0");
    break;

  case 16:
    str_concat(buffer, "x0\0");
    break;
  }

  for (int i = 0; i < str_length(buffer) / 2; i++) {
    char temp = buffer[i];
    buffer[i] = buffer[str_length(buffer) - 1 - i];
    buffer[str_length(buffer) - 1 - i] = temp;
  }

  buffer[str_length(buffer)] = '\0';
}

void str_remove_trailing_spaces(char *str) {
  int str_ptr = 0;
  for (; str[str_ptr] != '\0'; str_ptr++)
    ;

  while (str[str_ptr - 1] == 32)
    str[--str_ptr] = '\0';
}

void str_empty(char *str) {
  for (int i = 0; i < str_length(str); i++)
    str[i] = '\0';
}
