    .globl example
example:
    enter $0, $0
    mov $1, %eax
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    mov $10, %eax
    leave
    ret

