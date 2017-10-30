main:

    enter $16, $0

    leave
    ret

before:

    enter $0, $0

    movq $1, %eax
    leave
    ret

after:

    enter $8, $0

    movq $1, %eax
    leave
    ret

