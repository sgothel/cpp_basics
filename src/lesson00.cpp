//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 00
//============================================================================

#include <iostream>

/**
 * Lesson 00
 *
 * A C++ program consist out of
 * - things, like types, their methods and general functions
 * - flow, the program flow of things
 *
 * This lesson demonstrates
 * - function declaration and definition in same or different modules
 * - clarify loop expressions using different C++ constructs
 *   - if branch and goto
 *   - while loop
 *   - for loop
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

// Include header file, which contains declaration of function `double_value` as defined in module lesson00_module.cpp.
// Linker has to link lesson00.o + lesson00_module.o to resolve this function!
#include "cpp_basics/lesson00_module.hpp"

int main(int argc, const char* argv[]) {
    // the program flow within this function definition
    {
        // loop through all program invocation arguments and print them
        for(int i=0; i<argc; ++i) {
            std::cout << "cmd_arg[" << std::to_string(i) << "]: " << argv[i] << std::endl;
        }
    }
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

    const int loop_count = 3;
    // Same loop as if+goto
    {
        // while loop, an exploded for-loop (see below)
        std::cout << "loop0.0: ";
        int i=0;              /* instantiation and initialization of loop variable */
        mark:
        if( i < loop_count    /* while condition */ ) {
            std::cout << "i " << i << ", ";
            i = i + 1; // ++i
            goto mark;
        }
        std::cout << std::endl;
    }
    // Same loop as while
    {
        // while loop, an exploded for-loop (see below)
        std::cout << "loop1.0: ";
        int i=0;              /* instantiation and initialization of loop variable */
        while( i < loop_count /* while condition */ ) {
            std::cout << "i " << i << ", ";
            i = i + 1;        /* tail expression */
        }
        std::cout << std::endl;
    }
    // Same loop as for (1)
    {
        std::cout << "loop2.0: ";
        int i;                /* instantiation of loop variable*/
        for(i=0               /* initialization of loop variable*/;
            i<loop_count      /* while condition */;
            ++i               /* tail expression */)
        {
            std::cout << "i " << i << ", ";
        }
        // `i` is still in scope!
        std::cout << std::endl;
    }
    // Same loop as for (2)
    {
        std::cout << "loop2.1: ";
        /* instantiation and initialization of loop variable; while condition; tail expression */
        for(int i=0; i<loop_count; ++i) {
            std::cout << "i " << i << ", ";
        }
        // `i` is out of scope!
        std::cout << std::endl;
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
