.SILENT:
.PHONY: run

CC=g++

all: cols_slow rows_fast

cols_slow: cols_slow.o timer.o
	$(CC) -o cols_slow cols_slow.o timer.o -O2
	
rows_fast: rows_fast.o timer.o
	$(CC) -o rows_fast rows_fast.o timer.o -O2
	
timer.o: timer.cpp timer.h
	$(CC) -c timer.cpp
	
cols_slow.o: cols_slow.cpp timer.h
	$(CC) -c cols_slow.cpp
	
rows_fast.o: rows_fast.cpp timer.h
	$(CC) -c rows_fast.cpp
	
clean:
	rm -rf *.o cols_slow rows_fast
	
run: all
	./cols_slow
	./rows_fast