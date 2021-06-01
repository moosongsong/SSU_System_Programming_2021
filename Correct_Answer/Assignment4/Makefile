CC=gcc
CFLAGS= -Wall -g -I.

DEPS = assignment4.h
OBJ = assignment4.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assignment4: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o assignment4

