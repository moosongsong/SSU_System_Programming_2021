CC = gcc
CFLAGS = -I.
DEPS = assignment1.h
OBJ = assignment1.o

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)
assignment1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf $^

.PHONY: clean
clean: 
	rm -f *.o assignment1
