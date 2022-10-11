//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 00
//============================================================================

#include <cstdio>
#include <iostream>

/**
 * Lesson 00
 *
 * A C++ program consist out of
 * - things, like types, their methods and general functions
 * - flow, the program flow of things
 */

/**
 * Function declaration with function name `add`.
 *
 * No program code is given.
 *
 * A function declaration is used to let the compiler
 * know how to use a function.
 *
 * @param x int paramter 1
 * @param y int paramter 2
 * @return type int
 */
int add(const int x, const int y);

/**
 * Function definition with function name `add`
 * and given program code.
 *
 * A function definition must match a given function declaration, if exists.
 *
 * A function definition may be called if invoked.
 *
 * @param x int paramter 1
 * @param y int paramter 2
 * @return type int
 */
int add(const int x, const int y) {
    return x + y;
}

// Function declaration of a function defined in another module, see lesson00_module.cpp
// Linker has to link lesson00.o + lesson00_module.o to resolve this function!
int double_value(const int x);

int main(int argc, const char* argv[]) {
    // the program flow within this function definition
    {
        // entering scope-1, constructing `a` while initializing w/ `3`
        int a=3;
        {
            // entering this scope-2, constructing `b` w/ `4`
            int b=4;
            int r = a + b;
            (void)r; // avoid unused warnings
            // leaving this scope-2, b will be destructed
        }
        // int r = a + b; // b is out of scope
    }
    {
        // while loop, an exploded for-loop (see below)
        int i=0; /* loop variable */
        while( i < argc /* while condition */ ) {
            std::cout << "main.0.arg[" << i << "]: " << argv[i] << std::endl;
            i = i + 1; // ++i
        }
    }
    {
        // for loop
        for(int i=0 /* initialization */;
            i<argc  /* while condition */;
            ++i     /* tail expression */)
        {
            fprintf(stderr, "main.1.arg[%d]: %s\n", i, argv[i]);
        }
        for(int i=0; i<argc; ++i) {
            fprintf(stderr, "main.2.arg[%d]: %s\n", i, argv[i]);
        }
        // `i` is out of scope!
    }

	int error_counter = 0;
	{
        const int res = add(1, 2);
        if( 3 != res ) {
            ++error_counter;
        }
	}
	{
	    const int res = double_value(1);
	    if( 2 != res ) {
	        ++error_counter;
	    }
	}
	if( 0 == error_counter ) {
		std::cout << "No error" << std::endl;
		return 0;
	} else {
		std::cout << error_counter << " error(s)" << std::endl;
		return 1;
	}
}
