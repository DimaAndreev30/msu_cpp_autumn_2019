CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: calc

calc: main.o calc.o
	$(CC) -o calc main.o calc.o $(CFLAGS)

main.o: main.cpp calc.h
	$(CC) -c main.cpp $(CFLAGS)

calc.o: calc.cpp calc.h
	$(CC) -c calc.cpp $(CFLAGS)


clean:
	rm -rf *.o calc



run: all
	./calc "2+2"

test: all
	python3 test.py

