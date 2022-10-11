//============================================================================
// Name        : arithmetic.cpp
// Author      : Svenson Han Gothel and Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : Applied artihmetic using C++
//============================================================================

#include <cstdio>
#include <iostream>

// function declaration
int add(const int x, const int y);

// function definition
int add(const int x, const int y) {
    return x + y;
}

// function declaration
int double_value(const int x);

int main(int argc, const char* argv[]) {
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
        // scope
        for(int i=0; i<argc; i++) {
            fprintf(stderr, "main.arg[%d]: %s\n", i, argv[i]);
        }
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
