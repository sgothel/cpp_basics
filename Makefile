# A simple Makefile for compiling small SDL projects

# set the compiler
CPP		:= g++ -x c++ -std=c++17 -c
LN	    := g++

# set the compiler flags
CPPFLAGS := -ggdb3 -O0 -Wall
LNFLAGS := -ggdb3 -O0 -Wall -lm

obj/%.o: src/%.cpp $(HEADERS) Makefile
	mkdir -p obj
	$(CPP) -o $@ $(CPPFLAGS) $<

bin/lesson00: obj/lesson00.o obj/lesson00_module.o
	mkdir -p bin
	$(LN) -o $@ $^ $(LNFLAGS)

# default recipe
all: bin/lessons00

clean:
	rm -rf obj bin Debug build

.PHONY: all clean
