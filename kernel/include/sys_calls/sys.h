#ifndef SYS_H
#define SYS_H

// Member APIs

void sys_write(char *text);
int sys_fork(unsigned long stack);
unsigned long sys_malloc();
void sys_proc_exit();

extern void call_sys_write(char *);
extern int call_sys_fork(unsigned long, unsigned long, unsigned long);
extern unsigned long call_sys_malloc();
extern void call_sys_proc_exit();

extern void return_from_syscall();

#endif // !SYS_H
