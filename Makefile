CC = g++
CFLAGS = -Wall -pedantic -g

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).o sudoku.o
	$(CC) $(CFLAGS) -o main main.o sudoku.o

main.o : main.cpp sudoku.hpp
	$(CC) $(CFLAGS) -c main.cpp

sudoku.o : sudoku.hpp sudoku.cpp
	$(CC) $(CFLAGS) -c sudoku.cpp

clean:
	$(RM) $(TARGET) *.o
