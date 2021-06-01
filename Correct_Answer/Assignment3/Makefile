CC=gcc
CFLAGS= -Wall -g -I.

DEPS = assignment3.h
OBJ = assignment3.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Change the name of the output binary file to your student ID #.
assignment3: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o assignment3

