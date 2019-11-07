CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: bigint test_prog


bigint: main.o bigint.o
	$(CC) -o bigint main.o bigint.o $(CFLAGS)

main.o: main.cpp bigint.h
	$(CC) -c main.cpp $(CFLAGS)

bigint.o: bigint.cpp bigint.h
	$(CC) -c bigint.cpp $(CFLAGS)


test_prog: test.o bigint.o
	$(CC) -o test_prog test.o bigint.o $(CFLAGS)

test.o: test.cpp bigint.h
	$(CC) -c test.cpp $(CFLAGS)


clean:
	rm -rf *.o bigint test_prog a.out


run: bigint
	./bigint

test: test_prog bigint
	python test.py
	./test_prog
