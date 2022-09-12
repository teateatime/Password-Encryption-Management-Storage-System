CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = main.o password.o manager.o support.o login.o pems.o utility.o store.o my_string.o

all: pass
pass: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o pass -lsqlite3 
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -lsqlite3 -o main.o
password.o: password.c
	$(CC) $(CFLAGS) -c password.c -lsqlite3 -o password.o -lm
support.o: support.c
	$(CC) $(CFLAGS) -c support.c -lsqlite3 -o support.o
login.o: login.c
	$(CC) $(CFLAGS) -c login.c -lsqlite3 -o login.o
pems.o: pems.c
	$(CC) $(CFLAGS) -c pems.c -lsqlite3 -o pems.o
my_string.o: my_string.c
	$(CC) $(CFLAGS) -c my_string.c -lsqlite3 -o my_string.o
manager.o: manager.c
	$(CC) $(CFLAGS) -c manager.c -lsqlite3 -o manager.o
utility.o: utility.c
	$(CC) $(CFLAGS) -c utility.c -lsqlite3 -o utility.o
store.o: store.c
	$(CC) $(CFLAGS) -c store.c -lsqlite3 -o store.o
clean:
	rm pass $(OBJECTS)
