            lw      0       1       bottom
            lw      0       2       top
            lw      0       3       two
repeat      add     2       1        2
            beq     2       0       finish
            beq     0       0       repeat
finish      halt
bottom    .fill       -1
top     .fill       10
two     .fill       1