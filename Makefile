# Directories
BIN := bin
BUILD := build
INCLUDE := include
SRC := examples

# Compiler
CC := g++
CFLAGS := -std=c++14 -O2 -I $(INCLUDE) -pthread

all: http tcp

setup:
	mkdir -p bin
	scripts/include

http: setup
	$(CC) $(CFLAGS) $(SRC)/http-server.cc -o $(BIN)/warp-http

tcp: setup
	$(CC) $(CFLAGS) $(SRC)/tcp-server.cc -o $(BIN)/warp-tcp

clean:
	mkdir -p bin
	rm -f bin/*