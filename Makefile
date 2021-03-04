dict: dict.o list.o
	gcc -Wall -o dict dict.o list.o -g

list.o: list.c list.h
	gcc -Wall -o list.o -c list.c -g

dict.o: dict.c list.h
	gcc -Wall -o dict.o -c dict.c -g
