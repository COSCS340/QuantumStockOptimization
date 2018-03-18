EXES = main
CC=g++

CFLAGS= -Wall -I/usr/include -march=native -O2
LDFLAGS= 

all: $(EXES)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

%: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f ${EXES} *.o 

main: stock_data.cpp main.cpp markowitz.cpp
