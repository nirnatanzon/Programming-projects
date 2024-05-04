run: set.o main.o
	gcc -ansi -Wall -pedantic -g set.o main.o -o run
set.o: set.c set.h
	gcc -c -ansi -Wall -pedantic set.c -o set.o
main.o:main.c set.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
	
