    .globl example
example:
    enter $16, $0
    mov $1, %eax
    add $1, %eax
    mov %eax ,-16(%ebp)
    mov 0(%ebp), %eax
    add $1, %eax
    mov %eax ,-16(%ebp)
    mov $1, %eax
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    mov $10, %eax
    leave
    ret

