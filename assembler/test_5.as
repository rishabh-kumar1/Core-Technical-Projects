        lw      0       1       ArrayS
        lw      0       2       Num
        lw      0       3       Fcall
        jalr    3       7
        halt
Find    lw      0       6       NegOne  // r6=-1
        add     0       0       5       // r5=0
Top     add     2       6       2       // Decrement Num
        add     1       2       4       // 4 is address
        lw      4       3       0       // ld array element
        beq     3       0       skip    // is  element 0?
        lw      0       4       One     // r4=1
        add     5       4       5       // if not add 1
skip    beq     0       2       Done    // if Num=0 we are done
        beq     0       0       Top     // next iteration
Done    jalr    7       3               // return
Zero    .fill   0
NegOne  .fill   -1
One     .fill   1
Fcall   .fill   Find
ArrayS  .fill   Array
Num     .fill   9
Array   .fill   0
        .fill   3
        .fill   1
        .fill   -1
        .fill   0
        .fill   -1
        .fill   0
        .fill   1
        .fill   -2
