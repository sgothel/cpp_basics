//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 01
//============================================================================

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <string>

/**
 * Lesson 02
 *
 * Things occupying space, actual memory for data and code accessible via references and pointers.
 *
 * Functions may use such references and pointers to utilize call-by-reference
 * instead of call-by-value, as used in our lessons before.
 */

/**
 * Call by value example.
 *
 * One is added to the copied value `v` and returned.
 *
 * Notable: `const int v` is equivalent to `int const v`.
 *
 * @param v const imutable copied int value (r-value)
 * @return incremented value
 */
int return_incremented_value(const int v) { return v+1; }

/**
 * Call by refence example 01.
 *
 * The referenced int variable is incremented by one and its value returned.
 *
 * Notable: Because a reference is always valid, no checks are required.
 *
 * @param v mutable reference to int variable (l-value)
 * @return incremented value
 */
int increment_reference_and_return_01(int& v) { return ++v; }

/**
 * Call by refence example 02.
 *
 * The int variable referened by given pointer is incremented by one and its value returned.
 *
 * Notable-01: The given pointer is immutable, but the variable it references is mutable.
 *
 * Notable-02: Because a pointer may be nullptr, it shall be checked before usage.
 *
 * @param p_v immutable pointer to a mutable int variable (l-value)
 * @return incremented value
 */
int increment_reference_and_return_02(int* const p_v) {
    if( nullptr == p_v ) { // ensure p_v is valid
        // p_v is null, pointing to invalid memory
        return -1;
    } else {
        // p_v is valid
        return ++(*p_v);
    }
}

int main(int argc, const char* argv[]) {
    {
        // loop through all program invocation arguments and print them
        for(int i=0; i<argc; ++i) {
            printf("cmd_arg[%d]: %s\n", i, argv[i]);
        }
    }
    // Call-by-value and call-by-reference
    {
        // Instantiate variable `i` and initialize with value 5.
        int i = 5;

        // Assign address of variable `i` to int-pointer `p_i`.
        int* p_i = &i;

        printf("00: i: value %d, size %zu, address %p\n", i, sizeof(i), p_i);

        // Assign 6 to content of int-pointer `p_i`, aka int-variable `i`.
        *p_i = 6;
        printf("01: i: value %d, size %zu, address %p\n", i, sizeof(i), p_i);

        {
            const int r = return_incremented_value(i);
            printf("02: i: value %d, size %zu, address %p; r %d\n", i, sizeof(i), p_i, r);
            assert(7 == r);
            assert(6 == i);
            // Leaving this scope destructs all automatic allocated resources of this block: `r`.
        }

        {
            const int r = increment_reference_and_return_01(i);
            printf("03: i: value %d, size %zu, address %p; r %d\n", i, sizeof(i), p_i, r);
            assert(7 == r);
            assert(7 == i);
            // Leaving this scope destructs all automatic allocated resources of this block: `r`.
        }

        {
            const int r = increment_reference_and_return_02(&i);
            printf("04: i: value %d, size %zu, address %p; r %d\n", i, sizeof(i), p_i, r);
            assert(8 == r);
            assert(8 == i);
            // Leaving this scope destructs all automatic allocated resources of this block: `r`.
        }
        // Leaving this scope destructs all automatic allocated resources of this block: `i`, `p_i`.
    }

    // Demonstrate using address of a function, i.e. a function-pointer.
    {
        typedef int(*incr_func)(const int);
        printf("10: return_incremented_value: address %p\n",
                return_incremented_value);

        incr_func f1 = return_incremented_value;
        const int i = 10;
        const int r = f1(i);
        printf("11: i: value %d, size %zu; f1 %p, size f1 %zu, r %d\n", i, sizeof(i), f1, sizeof(f1), r);
        assert( return_incremented_value == f1 );
        assert( 11 == r );
        assert( 10 == i );
        // Leaving this scope destructs all automatic allocated resources of this block: `f1`, `i`, `r`.
    }

    // Source of things, from stack to heap w/o automatic destruction when running out-of-scope!
    {
        int i = 5; // stack automatic variable
        assert(5 == i);

        // Heap via C malloc without initialization
        int* p_j = static_cast<int*>( std::malloc( sizeof(int) ) );
        {
            *p_j = 6; // post allocation initialization
        }
        assert(6 == *p_j);
        // Explicit destruction of allocated memory!
        std::free(p_j);
        p_j = nullptr; // convention, but not required here as impossible to reuse due to running out of scope

        // Heap via C++ new including initialization invoking copy-ctor
        int* p_k = new int(7);
        assert(7 == *p_k);
        // Explicit destruction of allocated memory!
        delete p_k;
        p_k = nullptr; // convention, but not required here as impossible to reuse due to running out of scope
    }

    // Pointer arithmetic
    {
        //const int len = 10;

        // Using an array of int value via C malloc
        {
            // int* big
        }
    }
    return 0;
}
