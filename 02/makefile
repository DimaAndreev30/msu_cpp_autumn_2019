CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: alloc

alloc: main.o alloc.o
	$(CC) -o alloc main.o alloc.o $(CFLAGS)

main.o: main.cpp alloc.h
	$(CC) -c main.cpp $(CFLAGS)

alloc.o: alloc.cpp alloc.h
	$(CC) -c alloc.cpp $(CFLAGS)


clean:
	rm -rf *.o alloc


ARGS = 100 a50 a50 r a10
run: all
	./alloc $(ARGS)

test: all
	python3 test.py

