#include "user_code.h"
#include "user_syscalls.h"

void loop(char *str) {
  char buf[2] = {""};
  while (1) {
    for (int i = 0; i < 5; i++) {
      buf[0] = str[i];
      user_call_sys_write(buf);
    }
  }
}

void user_process() {
  user_call_sys_write("User process\n\r");
  int pid = user_call_sys_fork();
  if (pid < 0) {
    user_call_sys_write("Error during fork\n\r");
    user_call_sys_proc_exit();
    return;
  }
  if (pid == 0) {
    loop("abcde");
  } else {
    loop("12345");
  }
}
