assembler: myfile.o Opcode.o function.o  main.o 
	gcc -ansi -Wall -pedantic -g myfile.o Opcode.o function.o main.o -o assembler
	
myfile.o: myfile.c myfile.h
	gcc -c -ansi -Wall -pedantic myfile.c -o myfile.o
	
Opcode.o: Opcode.c Opcode.h
	gcc -c -ansi -Wall -pedantic Opcode.c -o Opcode.o
	
function.o: function.c function.h myfile.h Opcode.h
	gcc -c -ansi -Wall -pedantic function.c -o function.o
	
main.o:main.c myfile.h Opcode.h function.h 
	gcc -c -ansi -Wall -pedantic main.c -o main.o


