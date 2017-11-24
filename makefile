CC=gcc
CFLAGS=-I.

main: funciones.o main.o
	$(CC) -o main funciones.o main.o -lm -I.
	rm main.o funciones.o