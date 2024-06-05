.globl get_current_exception_level
get_current_exception_level:
    mrs x0, CurrentEL
    lsr w0, w0, #2
    ret
