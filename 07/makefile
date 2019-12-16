CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: vector


vector: test.o
	$(CC) -o vector test.o $(CFLAGS)

test.o: test.cpp vector.h allocator.h iterator.h
	$(CC) -c test.cpp $(CFLAGS)


clean:
	rm -rf *.o vector a.out


run: vector
	./vector

test: vector
	./vector
