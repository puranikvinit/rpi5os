.section ".text.user"

.set SYS_WRITE, 0
.set SYS_FORK, 1
.set SYS_MALLOC, 2
.set SYS_PROC_EXIT, 3

.globl user_call_sys_write
user_call_sys_write:
    mov w8, #SYS_WRITE
    svc #0
    ret

.globl user_call_sys_fork
user_call_sys_fork:
    mov w8, #SYS_FORK
    svc #0
    ret

.globl user_call_sys_malloc
user_call_sys_malloc:
    mov w8, #SYS_MALLOC
    svc #0
    ret

.globl user_call_sys_proc_exit
user_call_sys_proc_exit:
    mov w8, #SYS_PROC_EXIT
    svc #0
    ret
