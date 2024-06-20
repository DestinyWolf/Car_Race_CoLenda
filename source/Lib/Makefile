all: gameboy_example

imagem: gameboy_example.c colenda.o
	gcc -o gameboy_example gameboy_example.c colenda.o -std=c99

colenda.o: colenda.c colenda.h
	gcc -c colenda.c -std=c99

clean:
	rm -f *.o