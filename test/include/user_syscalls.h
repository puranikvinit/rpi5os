#ifndef _USER_SYS_H
#define _USER_SYS_H

void user_call_sys_write(char *buf);
int user_call_sys_fork();
void user_call_sys_proc_exit();

#endif // !_USER_SYS_H
