CC=gcc
CFLAGS= -Wall -g -I.

DEPS = assignment2.h
OBJ = assignment2.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Change the name of the output binary file to your student ID #.
assignment2: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o assignment2

