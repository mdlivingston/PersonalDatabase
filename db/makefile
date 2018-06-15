final: final.o database.o document.o assemblage.o scanner.o array.o dictionary.o doublylinked.o node.o
	gcc -Wall -g final.o database.o document.o assemblage.o scanner.o array.o dictionary.o doublylinked.o node.o -o final

final.o: final.c document.h assemblage.h
	gcc -Wall -g -c final.c

database.o: database.c database.h assemblage.h document.h scanner.h doublylinked.h array.h
	gcc -Wall -g -c database.c

document.o: document.c document.h dictionary.h scanner.h 
	gcc -Wall -g -c document.c

assemblage.o: assemblage.c assemblage.h document.h scanner.h doublylinked.h array.h
	gcc -Wall -g -c assemblage.c

scanner.o: scanner.c scanner.h
	gcc -Wall -g -c scanner.c

array.o: array.c array.h scanner.h
	gcc -Wall -g -c array.c

dictionary.o: dictionary.c dictionary.h array.h scanner.h
	gcc -Wall -g -c dictionary.c

doublylinked.o: doublylinked.c doublylinked.h scanner.h node.h
	gcc -Wall -g -c doublylinked.c

node.o: node.c node.h scanner.h
	gcc -Wall -g -c node.c

clean:
	rm -f *.o final
