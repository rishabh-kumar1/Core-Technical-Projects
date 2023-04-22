        lw      0       1       result      //load reg1 with result of multiplication
        lw      0       2       mcand       //load reg2 with the number in mcand
        lw      0       3       mplier      //load reg3 with the number in mplier
        lw      0       4       looper      //load looper into reg4
        lw      0       5       carry       //load carry into reg5
        lw      0       6       less        //load less into reg6
        lw      0       7       partition   //load partition into reg7
        lw      0       0       holder      //load looper into reg0
looper  beq     6       7       final
        nor     2       5       0
        beq     4       0       half2
half1   add     3       3       3
        add     6       5       5
        add     6       6       6
        beq     4       4       looper
half2   add     3       1       1
        jalr    4       4       half1
final        halt
result      .fill       0
mcand       .fill       1103
mplier      .fill       7043
holder      .fill       1
carry       .fill       -2
less        .fill       -1
partition   .fill       -32768