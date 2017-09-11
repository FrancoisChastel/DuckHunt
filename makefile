make:
	std=c++11 *pp
	mv a.out Skeleton
	mkfifo player2server server2player

run:
	./Skeleton server < player2server | ./Skeleton verbose > player2server	

clean:
	rm -rf *.gch
	rm -rf Skeleton
	rm -rf player2server 
	rm -rf server2player
