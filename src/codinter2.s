    .globl incr
incr:
    enter $0, $0
.L0:
    mov 0(%ebp), %edx
    mov 0(%ebp), %eax
    add %edx, %eax
    mov %eax ,-8(%ebp)
.L2:
    mov 0(%ebp), %edx
    mov 0(%ebp), %eax
    add %edx, %eax
    mov %eax ,-24(%ebp)
.L3:
.L1:
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    call _incr
    leave
    ret

