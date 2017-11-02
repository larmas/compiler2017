    .globl example
example:
    enter $0, $0
    mov 0(%ebp), %eax
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    call _example
    call _example
    leave
    ret

