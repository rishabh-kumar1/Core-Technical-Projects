        lw 0 1 zero
        lw 0 5 neg
        lw 0 6 lower
        lw 0 7 one
        lw 0 2 fifteen
        lw 0 3 ten
        lw 0 4 neg2
carry    beq 5 6 final
        nor 2 4 7
        beq 0 7 bound
cool    add 3 3 3
        add 5 4 4
        add 5 5 5
        beq 0 0 carry
bound     add 3 1 1
        beq 0 0 cool
final     halt
ten  .fill 10
neg  .fill -1
fifteen   .fill 15
neg2   .fill -2
zero  .fill 0
one     .fill 1
lower     .fill -32768