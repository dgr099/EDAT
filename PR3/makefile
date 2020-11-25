CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi -g
resultado: prueba.o func.o
	$(CC) $(CFLAGS) -o $@ $^
prueba.o: prueba.c
	$(CC) $(CFLAGS) -c -o $@ $<
func.o: func.c func.h
	$(CC) $(CFLAGS) -c -o$@ $<