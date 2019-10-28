CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: matrix _test


matrix: main.o matrix.o
	$(CC) -o matrix main.o matrix.o $(CFLAGS)

main.o: main.cpp matrix.h
	$(CC) -c main.cpp $(CFLAGS)

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp $(CFLAGS)


_test: test.o matrix.o
	$(CC) -o _test test.o matrix.o $(CFLAGS)

test.o: test.cpp matrix.h
	$(CC) -c test.cpp $(CFLAGS)


clean:
	rm -rf *.o matrix


run: matrix
	./matrix

test: _test
	./_test

