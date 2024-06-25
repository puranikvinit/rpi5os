#include "user_code.h"
#include "sys_calls/sys.h"

void _loop(char *str) {
  char buff[2] = "";
  while (1) {
    for (int i = 0; i < 6; i++) {
      buff[0] = str[i];
      buff[1] = '\0';
      call_sys_write(buff);
    }
  }
}

void user_process() {
  call_sys_write("User Process\0");
  int pid = call_sys_fork();
  if (pid < 0) {
    call_sys_write("Fork failed\n\0");
    return;
  }
  if (pid == 0) {
    _loop("abcde\n");
  } else {
    _loop("12345\n");
  }
}
