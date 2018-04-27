CPP = g++
CFLAGS = -O3 -Wall -Wextra
LFLAGS = -lcurl


all: main

#Executables

main: main.cpp stock_data.o 
	$(CPP) $(CFLAGS) main.cpp stock_data.o -o main $(LFLAGS)

#0bject Files

stock_data.o: stock_data.cpp stock_data.h
	$(CPP) $(CFLAGS) stock_data.cpp -c -o stock_data.o $(LFLAGS)