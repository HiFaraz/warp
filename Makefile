# Directories
BIN := bin
BUILD := build
INCLUDE := include
SRC := examples

# Compiler
CC := g++
CFLAGS := -std=c++14 -O2 -I $(INCLUDE) -pthread

all: http tcp

http:
	$(CC) $(CFLAGS) $(SRC)/http-server.cc -o $(BIN)/ftl-http

tcp:
	$(CC) $(CFLAGS) $(SRC)/tcp-server.cc -o $(BIN)/ftl-tcp

clean:
	rm -f bin/*