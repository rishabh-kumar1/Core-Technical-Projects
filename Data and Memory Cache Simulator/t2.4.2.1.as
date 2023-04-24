    lw 0 1 fifteen
    lw 0 2 thirty
    lw 0 3 three
    nor 1 1 1
    add 3 1 1
    add 2 1 1
loop beq 1 4 final
    add 2 2 2
    add 4 3 4
    beq 0 0 loop
final halt
thirty .fill 10
fifteen .fill 5
three .fill 1