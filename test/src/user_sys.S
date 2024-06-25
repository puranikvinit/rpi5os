#include "sys_calls/sys_wrappers.h"

.globl call_sys_write
call_sys_write:
  mov w8, #SYS_WRITE
  svc #0
  ret

.globl call_sys_fork
call_sys_fork:
  mov w8, #SYS_FORK
  svc #0
  ret

.globl call_sys_malloc
call_sys_malloc:
  mov w8, #SYS_MALLOC
  svc #0
  ret

.globl call_sys_proc_exit
call_sys_proc_exit:
  mov w8, #SYS_PROC_EXIT
  svc #0
  ret


