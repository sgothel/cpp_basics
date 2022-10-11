# A simple Makefile for compiling small SDL projects

# set the compiler
CPP		:= g++ -x c++ -std=c++17 -c
LN	    := g++

# set the compiler flags
CPPFLAGS := -ggdb3 -O0 -Wall
LNFLAGS := -ggdb3 -O0 -Wall -lm

TEMPDIRS = obj bin

obj/%.o: src/%.cpp $(HEADERS) Makefile
	$(CPP) -o $@ $(CPPFLAGS) $<

all: tempdirs bin/lesson00

.PHONY: all clean tempdirs obj

bin/lesson00: obj/lesson00.o obj/lesson00_module.o
	$(LN) -o $@ $^ $(LNFLAGS)

tempdirs:
	for dir in $(TEMPDIRS); do \
	    mkdir -p $$dir; \
	done

clean:
	rm -rf obj bin Debug build

