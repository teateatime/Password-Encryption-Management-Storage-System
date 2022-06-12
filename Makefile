CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = main.o password.o manager.o support.o my_string.o

all: pass
pass: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o pass -lsqlite3 
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -lsqlite3 -o main.o
password.o: password.c
	$(CC) $(CFLAGS) -c password.c -lsqlite3 -o password.o -lm
support.o: support.c
	$(CC) $(CFLAGS) -c support.c -lsqlite3 -o support.o
my_string.o: my_string.c
	$(CC) $(CFLAGS) -c my_string.c -lsqlite3 -o my_string.o
manager.o: manager.c
	$(CC) $(CFLAGS) -c manager.c -lsqlite3 -o manager.o
clean:
	rm pass $(OBJECTS)
