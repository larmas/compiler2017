    .globl example
example:
    enter $8, $0
    mov  $1, %eax
    add  $1, %eax
    mov  %eax ,-24(%rbp)
.L0:
    mov $1, %eax
    leave
    ret

    .globl _main
_main:
    enter $0, $0
    mov $10, %eax
    leave
    ret

