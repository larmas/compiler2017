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
    mov 0(%ebp), %edx
    mov 0(%ebp), %eax
    add %edx, %eax
    mov %eax ,-32(%ebp)
    mov -32(%ebp), %eax
    leave
    ret

    .globl hastaIgual
hastaIgual:
    enter $8, $0
.L4:
.L6:
    mov 0(%ebp), %edx
    mov 0(%ebp), %eax
    add %edx, %eax
    mov %eax ,-48(%ebp)
.L8:
.L7:
.L5:
    mov -8(%ebp), %eax
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    call _incr
    call _hastaIgual
    leave
    ret

