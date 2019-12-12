CC = g++
CFLAGS = -std=c++14 -pthread

.SILENT:
.PHONY: run test


all: sort


sort: main.cpp sort.o
	$(CC) -o sort main.cpp sort.o $(CFLAGS)

sort.o: sort.cpp sort.h
	$(CC) -c sort.cpp $(CFLAGS)

clean:
	rm -rf *.o sort temp_* a.out


run: sort
	./sort

test: sort
	./sort
