CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi

HEADERS = utils.h

EXE = tester
OBJ = $(EXE).o utils.o checkUtils.o checkCreateTable.o \
checkCreateIndex.o checkPrint.o checkFindKey.o checkAddIndexEntry.o
#all : $(EXE)


%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

$EXE: $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean :
	rm -f *.o core $(EXE)

programa: programa.o menu.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

programa.o: programa.c menu.h
	$(CC) $(CFLAGS) -c -o $@ $<

menu.o:	menu.c menu.h
	$(CC) $(CFLAGS) -c -o $@ $<


prueba: prueba.o utils.o
	$(CC) $(CFLAGS) -o $@ $^
prueba.o: prueba.c
	$(CC) $(CFLAGS) -c -o $@ $<
