# Directories
BIN := bin
BUILD := build
DEPS := deps
INCLUDE := include
LIB := lib
SRC := examples

# Compiler
CC := g++
CFLAGS := -std=c++14 -O2 -I $(INCLUDE) -pthread
LFLAGS := -L $(LIB) -lpicohttpparser

all: setup

##
# EXAMPLES
##

examples: http tcp
	@echo "\n\033[0;32mExamples built\033[0m"

http: setup
	$(CC) $(CFLAGS) $(SRC)/http-server.cc $(LFLAGS) -o $(BIN)/warp-http

tcp: setup
	$(CC) $(CFLAGS) $(SRC)/tcp-server.cc $(LFLAGS) -o $(BIN)/warp-tcp

##
# SETUP
##

setup: add-dirs add-deps
	@scripts/include
	@echo "\n\033[0;32mSetup complete\033[0m"

add-dirs:
	@mkdir -p bin
	@mkdir -p deps
	@mkdir -p include
	@mkdir -p lib
	@echo "\033[0;32mBuild directories added\033[0m"

add-deps: picohttpparser
	@echo "\033[0;32mDependencies added\033[0m"

picohttpparser:
	@scripts/deps add picohttpparser https://github.com/h2o/picohttpparser 2a16b2365ba30b13c218d15ed9991576358a6337
	@echo "Building library"
	@cd $(DEPS)/picohttpparser; $(CC) $(CFLAGS) -c picohttpparser.c
	@ar -cvq $(LIB)/libpicohttpparser.a $(DEPS)/picohttpparser/picohttpparser.o
	@cp deps/picohttpparser/picohttpparser.h src
	@echo "\033[0;32mpicohttpparser added\033[0m"

##
# TEARDOWN
##

clean: rm-dirs rm-deps
	@echo "\033[0;32mBuild files removed\033[0m"
	
rm-dirs:
	@rm -r -f bin
	@rm -r -f include
	@rm -r -f lib
	@echo "\033[0;32mBuild directories removed\033[0m"

rm-deps:
	@scripts/deps rm picohttpparser
	@rm src/picohttpparser.h
	@rm -r -f deps
	@echo "\n\033[0;32mDependencies removed\033[0m"