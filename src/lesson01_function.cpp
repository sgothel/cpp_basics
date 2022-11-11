//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 0.1 Functions
//============================================================================

#include <iostream>
#include <cassert>

/**
 * Lesson 0.1 Functions
 *
 * This lesson demonstrates
 * - function declaration and definition in same or different modules
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

// Include header file, which contains declaration of function `double_value` as defined in module lesson03_module.cpp.
// Linker has to link lesson01_function.o + lesson03_module.o to resolve this function!
#include "cpp_basics/lesson01_module.hpp"

int main(int argc, const char* argv[]) {
    // A loop to print all program invocation arguments
    {
        for(int i=0; i<argc; ++i) {
            std::cout << "cmd_arg[" << std::to_string(i) << "]: " << argv[i] << std::endl;
        }
    }

	int error_counter = 0;

	// invoke local function `add`
	{
        const int res = add(1, 2);
        if( 3 != res ) {
            ++error_counter;
        }
	}

	// invoke external function `double_value`
	{
	    const int res = double_value(1);
	    if( 2 != res ) {
	        ++error_counter;
	    }
	}

    // Demonstrate using address of a function, i.e. a function-pointer.
    {
        // `add_func` is a created type definition of a function
        typedef int(*add_func)(const int, const int);
        printf("10: add: address %p\n", add);

        add_func f1 = add;
        const int i = 10;
        const int j = 20;
        const int r = f1(i, j);
        printf("11: i: %d, j %d: f1 %p, size f1 %zu, r %d\n", i, j, f1, sizeof(f1), r);
        assert( add == f1 );
        assert( i + j == r );
        assert( 10 == i );
        assert( 20 == j );

        // Leaving this scope destructs all automatic allocated resources of this block: `f1`, `i`, `r`.
    }

	if( 0 == error_counter ) {
		std::cout << "No error" << std::endl;
		return 0;
	} else {
		std::cout << error_counter << " error(s)" << std::endl;
		return 1;
	}
}
