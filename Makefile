# Directories
BIN := bin
BUILD := build
DEPS := deps
INCLUDE := include
LIB := lib
SRC := examples
TESTS := tests

# Compiler
CC := g++
CFLAGS := -std=c++14 -O2 -I $(INCLUDE) -pthread
LFLAGS := -L $(LIB) -lpicohttpparser

all: warp

help:
	@echo "\n\033[1mWarp Makefile usage:\033[0m\n"
	@echo "\033[1mmake\033[0m\t\tBuild Warp\n\t\t(install dependencies and create Warp include files)"
	@echo "\033[1mmake clean\033[0m\tRemove all build artifacts"
	@echo "\033[1mmake examples\033[0m\tBuild examples"
	@echo "\033[1mmake test\033[0m\tRuns tests"
	@echo

##
# EXAMPLES
##

examples: http tcp
	@echo "\n\033[0;32mExamples built.\033[0m"

http: warp
	$(CC) $(CFLAGS) $(SRC)/http-server.cc $(LFLAGS) -o $(BIN)/warp-http

tcp: warp
	$(CC) $(CFLAGS) $(SRC)/tcp-server.cc $(LFLAGS) -o $(BIN)/warp-tcp

##
# TESTS
##

test: buffer
	@echo "\n\033[0;32mTests passed.\033[0m"

buffer: warp
	$(CC) $(CFLAGS) $(TESTS)/buffer.cc $(LFLAGS) -o $(BIN)/tests/buffer
	$(BIN)/tests/buffer
##
# SETUP
##

warp: dirs deps includes
	@echo "\n\033[1;32mWarp build complete.\033[0m\n"

includes:
	@scripts/include
	@echo "\033[0;32mInclude files created.\033[0m"

dirs:
	@mkdir -p $(BIN)
	@mkdir -p $(BIN)/tests
	@mkdir -p $(DEPS)
	@mkdir -p $(INCLUDE)
	@mkdir -p $(LIB)
	@echo "\033[0;32mBuild directories added.\033[0m"

deps: picohttpparser
	@echo "\033[0;32mDependencies added.\033[0m"

picohttpparser:
	@scripts/deps add picohttpparser https://github.com/h2o/picohttpparser 2a16b2365ba30b13c218d15ed9991576358a6337
	@echo "Building library"
	@cd $(DEPS)/picohttpparser; $(CC) $(CFLAGS) -c picohttpparser.c
	@ar -cvq $(LIB)/libpicohttpparser.a $(DEPS)/picohttpparser/picohttpparser.o
	@cp deps/picohttpparser/picohttpparser.h src
	@echo "\033[0;32mpicohttpparser added.\033[0m"

##
# TEARDOWN
##

clean: rm-dirs rm-deps
	@echo "\033[0;32mBuild files removed.\033[0m"
	
rm-dirs:
	@rm -r -f bin
	@rm -r -f include
	@rm -r -f lib
	@echo "\033[0;32mBuild directories removed.\033[0m"

rm-deps:
	@scripts/deps rm picohttpparser
	@rm src/picohttpparser.h
	@rm -r -f deps
	@echo "\n\033[0;32mDependencies removed.\033[0m"