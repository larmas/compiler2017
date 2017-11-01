    .globl _example
_example:

    enter $0, $0

    movq $1, %rax
    leave
    ret

    .globl _main
_main:

    enter $0, $0

    movq $10, %rax
    leave
    ret

