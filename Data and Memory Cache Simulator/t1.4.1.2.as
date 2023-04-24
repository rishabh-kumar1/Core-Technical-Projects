    lw 0 1 two
    lw 0 4 huge
high lw 0 2 list
    add 0 0 7
higher add 2 1 3
    lw 2 5 0
    lw 3 6 0
    beq 5 6 bound
    nor 5 0 5
    add 5 1 5
    add 5 6 5
    nor 5 4 5
    beq 0 5 bound
    lw 2 5 0
    sw 2 6 0
    sw 3 5 0
    add 0 1 7
bound lw 2 5 0
    beq 6 0 check
    add 1 2 2
    beq 0 0 higher
check beq 7 0 final
    halt
    beq 0 0 high
final halt
two .fill 1
list .fill list0
huge .fill 21736443
list0 .fill 10
list1 .fill 20
list2 .fill 2
list3 .fill 0