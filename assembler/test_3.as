 	lw  	 0   	5   	642     comment
loop   beq 	 5   	6      done
       jalr	 4   	7   	3 (Think about how that 3 is treated...)
       sw      6     4      5
	nor 	 1   	3   	1
       add 	 4   	5   	3     
	beq 	 0   	0   	loop
       noop	 2   	0   	2
done   halt  another comment
       .fill   loop
       .fill   6
