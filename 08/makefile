CC = g++
CFLAGS = -std=c++14 -pthread

.SILENT:
.PHONY: run test


all: pool


pool: main.cpp pool.h
	$(CC) -o pool main.cpp $(CFLAGS)


clean:
	rm -rf *.o pool a.out


run: pool
	./pool

test: pool
	./pool
