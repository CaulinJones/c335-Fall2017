CC = gcc

CFLAGS = -g

LDFLAGS = -g

poly: poly.o dump.o
	$(CC) $(LDFLAGS) poly.o dump.o -o poly

poly.o: poly.c
	$(CC) $(CFLAGS) -c poly.c

dump.o: dump.c
	$(CC) $(CFLAGS) -c dump.c

