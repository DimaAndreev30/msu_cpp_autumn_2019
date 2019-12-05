CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: vector test_


vector: main.o
	$(CC) -o vector main.o $(CFLAGS)

main.o: main.cpp vector.h
	$(CC) -c main.cpp $(CFLAGS)


test_: test.o
	$(CC) -o test_ test.o $(CFLAGS)

test.o: test.cpp vector.h
	$(CC) -c test.cpp $(CFLAGS)


clean:
	rm -rf *.o vector test_ a.out


run: vector
	./vector

test: test_
	./test_
