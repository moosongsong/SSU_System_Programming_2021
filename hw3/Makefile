CC=gcc
CFLAGS= -Wall -g -I.

DEPS = 20170767.h
OBJ = 20170767.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Change the name of the output binary file to your student ID #.
20170767: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o 20170767

