CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: format test_


format: main.o
	$(CC) -o format main.o $(CFLAGS)

main.o: main.cpp format.h
	$(CC) -c main.cpp $(CFLAGS)


test_: test.o
	$(CC) -o test_ test.o $(CFLAGS)

test.o: test.cpp format.h
	$(CC) -c test.cpp $(CFLAGS)


clean:
	rm -rf *.o format test_ a.out


run: format
	./format

test: test_
	./test_
