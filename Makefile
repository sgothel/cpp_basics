# A simple Makefile for compiling small SDL projects

# set the compiler
CPP		:= g++ -x c++ -std=c++17 -c -I include -Wall -Wextra -Werror
LN	    := g++

# set the compiler flags
CPPFLAGS := -ggdb3 -O0 -Wall
LNFLAGS := -ggdb3 -O0 -Wall -lm

TEMPDIRS = obj bin

obj/%.o: src/%.cpp $(HEADERS) Makefile
	$(CPP) -o $@ $(CPPFLAGS) $<

all: tempdirs bin/lesson00_basic \
     bin/lesson01_function bin/lesson02_arithmetic bin/lesson03_geometry0 bin/lesson03_geometry1 \
     bin/lesson10_memory \
     bin/lesson25_oop_virtl01

.PHONY: all clean tempdirs obj

bin/lesson00_basic: obj/lesson00_basic.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson01_function: obj/lesson01_function.o obj/lesson01_module.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson02_arithmetic: obj/lesson02_arithmetic.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson03_geometry0: obj/lesson03_geometry0.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson03_geometry1: obj/lesson03_geometry1.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson10_memory: obj/lesson10_memory.o
	$(LN) -o $@ $^ $(LNFLAGS)

bin/lesson25_oop_virtl01: obj/lesson25_oop_virtl01.o
	$(LN) -o $@ $^ $(LNFLAGS)

tempdirs:
	for dir in $(TEMPDIRS); do \
	    mkdir -p $$dir; \
	done

clean:
	rm -rf obj bin Debug build

