CC = gcc
CCOPTS = -c -g -W -Wall
LINKOPTS = -g -W -Wall

all: shell

testvector: testvector.o vector.o
	$(CC) $(LINKOPTS) -o $@ $^

testvector.o: testvector.c
	$(CC) $(CCOPTS) -o $@ testvector.c

shell: shell.o vector.o
	$(CC) $(LINKOPTS) -o $@ $^

shell.o: shell.c
	$(CC) $(CCOPTS) -o $@ shell.c

vector.o: vector.c
	$(CC) $(CCOPTS) -o $@ vector.c

clean:
	rm -rf *.o testvector testvector.exe shell shell.exe*~
