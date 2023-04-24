    lw 0 1 expression
    lw 0 2 m
    lw 0 3 mult
    lw 0 4 odd
    lw 0 5 away 
    lw 0 6 bound
    lw 0 7 two
back beq 5 6 down
    nor 2 4 7
    beq 0 7 odd
close add 3 3 3
    add 5 4 4
    add 5 5 5 
    beq 0 0 back
odd add 3 1 1
    beq 0 0 close
down halt
m .fill 15
mult .fill 10
away .fill -1
bound .fill -32768
odd .fill -2
expression .fill 0
two .fill 1