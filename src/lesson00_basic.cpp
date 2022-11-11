//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 0.0 Basic Language Statements
//============================================================================

#include <iostream>

/**
 * Lesson 0.0 Basic Language Statements
 *
 * A C++ program consist out of
 * - things, like types, their methods and general functions
 * - flow, the program flow of things
 *
 * This lesson demonstrates
 * - branch statements
 *   - if-else branch
 *   - switch branch, incl. break
 *   - conditional operator branch
 * - loop statements using
 *   - if branch and goto
 *   - while loop
 *   - do-while loop
 *   - for loop
 *   - break within a loop
 */
int main(int argc, const char* argv[]) {
    // the program flow within this function definition

    // Annotates scope and lifecycle within block-statements `{}`
    { // block-1 open
        // constructing `a` and initialize w/ `3, valid in scope of block-1
        int a=3;
        { // block-2 open
            // constructing `b` w/ `4`, valid in scope of block-2
            int b=4;
            int r = a + b;
            (void)r; // avoid unused warnings
        } // block-2 closed, left scope, 'b' is destructed
        // int r = a + b; // error: b is out of scope
    } // block-1 closed, left scope, `a` is destructed

    // branches: if
    {
        const int a = 0;

        // Note that we place the immutable literal (r-value) on the left-side
        // of the equality operation '0 == a',
        // which avoids accidental assignment of a mutable l-value if typo 'a = 0'.

        if( 0 == a ) {
            // executed if 'a' contains '0'
            std::cout << "branch0.0a";
        } else if( 1 == a ) {
            // executed if 'a' contains '1'
            std::cout << "branch0.1a";
        }
        std::cout << std::endl;

        // Note that the expression `0 == a` is a boolean expression,
        // i.e. resolved to either `true` or `false`.
        const bool b0 = 0 == a;
        const bool b1 = 1 == a;

        // Note that the expression for `if` and `while` are boolean expressions.
        //
        // Below we use the pre-computed boolean results.

        if( b0 ) {
            // executed if b0 is true, i.e. 'a' contains '0'
            std::cout << "branch0.0b";
        } else if( b1 ) {
            // executed if b1 is true, i.e. 'a' contains '1'
            std::cout << "branch0.1b";
        }
        std::cout << std::endl;
    }

    // branches: switch
    {
        int a = 0;

        switch( a ) {
            case 0:
                // executed if 'a' contains '0'
                std::cout << "branch1.0";
                break; // ends code for this case
            case 1:
                // executed if 'a' contains '0'
                {
                    // use an inner block-statement to allow local case resources
                    int v = 1;
                    std::cout << "branch1." << v;
                }
                break; // ends code for this case
            case 2:
                // executed if 'a' contains '2'
                // and falls through to default case code
                [[fallthrough]];
            default:
                // executed if none of the above cases matches
                std::cout << "branch1.2";
                break; // ends code for this case
        }
        std::cout << std::endl;
    }

    // branches: conditional operator
    {
        int a = 0;

        // initialized with '0' if 'a' contains '0', otherwise initialized with '1'
        char c = ( 0 == a ) ? '0' : '1';

        std::cout << "branch2." << c << std::endl;
    }

    const int loop_count = 3;
    // Same loop as if+goto
    {
        // manual if-goto loop, an exploded for-loop (see below)
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
        std::cout << "loop1.1: ";
        int i=0;              /* instantiation and initialization of loop variable */
        while( i < loop_count /* while condition */ ) {
            std::cout << "i " << i << ", ";
            i = i + 1;        /* tail expression */
        }
        std::cout << std::endl;
    }
    // Same loop as do-while
    {
        // do-while loop - executed at least once
        std::cout << "loop1.2: ";
        int i=0;              /* instantiation and initialization of loop variable */
        do {
            std::cout << "i " << i << ", ";
            i = i + 1;        /* tail expression */
        } while( i < loop_count /* while condition */ );
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
    // Using break within a loop
    {
        std::cout << "loop3.1: ";
        int i=0;              /* instantiation and initialization of loop variable */
        while( true /* while condition: forever */ ) {
            if( i >= loop_count ) {
                break; // exit loop
            }
            std::cout << "i " << i << ", ";
            i = i + 1;        /* tail expression */
        }
        std::cout << std::endl;
    }

    // A loop to print all program invocation arguments
    {
        for(int i=0; i<argc; ++i) {
            std::cout << "cmd_arg[" << std::to_string(i) << "]: " << argv[i] << std::endl;
        }
    }

    // Return `0` for no error to invoking shell
    return 0;
}
