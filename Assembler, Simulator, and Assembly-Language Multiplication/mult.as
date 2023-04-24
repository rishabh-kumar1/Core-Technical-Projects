        lw      0       3       mcand       //loading registers - Final submission for multiplication assembly program
        lw      0       4       mplier      //r2 - r5
        lw      0       5       cover       
        lw      0       2       most
        beq     0       0       looper      //branch to loop  
looper  nor     4       5       6
        beq     0       6       do
        beq     0       0       tor     //branches to tor
do      add     1       3       1           //adding
tor     lw      0       6       move
        add     2       6       2
        beq     0       2       final
        add     3       3       3       //bit shifting
        add     5       5       5       //continued
        lw      0       6       mult                          
        add     5       6       5
        beq     0       0       looper      //continue loop
final       halt
mcand       .fill       1103 //works with any two postitive 15 bit numbers
mplier      .fill       7043
most        .fill       15
move .fill       -1
mult    .fill       1
cover       .fill       -2