CC = g++
CFLAGS = -std=c++14 -O2

.SILENT:
.PHONY: run test


all: serializer test_prog


serializer: main.cpp serializer.h
	$(CC) -o serializer main.cpp $(CFLAGS)

test_prog: test.cpp serializer.h
	$(CC) -o test_prog test.cpp $(CFLAGS)


clean:
	rm -rf *.o serializer test_prog a.out


run: serializer
	./serializer

test: serializer
	./serializer
