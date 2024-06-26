#ifndef SYS_H
#define SYS_H

// Member APIs

/*! \brief System call to write a string to the console
 *
 * \param `text` The string to write to the console
 */
void sys_write(char *text);

/*! \brief System call to create a new process
 *
 * \return The process ID of the new process
 */
int sys_fork();

/*! \brief System call to allocate memory
 *
 * \return The address of the allocated memory
 */
unsigned long sys_malloc();

/*! \brief System call to exit the current process
 */
void sys_proc_exit();

// Userspace available APIs, to switch control to kernel mode, to execute the
// required system call.
extern void call_sys_write(char *);
extern int call_sys_fork();
extern unsigned long call_sys_malloc();
extern void call_sys_proc_exit();

// Kernel API to return control to userspace, after the system call has been
// serviced.
extern void return_from_syscall();

#endif // !SYS_H
