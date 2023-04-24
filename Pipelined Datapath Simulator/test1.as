		lw		0	1   one
		lw		0	2   top
		lw 		0	3   bottom
begin	add		3   2   2
        nor     2   1   4
		beq	    1	2	begin
        lw      0   5   rt 
        nor     5   5   5
		halt
one	    .fill	0
top		.fill	10
bottom	.fill 	-1
rt      .fill    3