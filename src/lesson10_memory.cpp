//============================================================================
// Author      : Sven Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 1.0 Memory (data, references and pointer)
//============================================================================

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cassert>

#include <string>
#include <memory>

/**
 * Lesson 1.0
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
        // `incr_func` is a created type definition of a function
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

        // Leaving this scope destructs all automatic allocated resources of this block: `i`, only pointer itself w/o heap of `p_j` and `p_k`.
    }

    // Source of things, from stack to heap _with_ automatic destruction when running out-of-scope!
    {
        int i = 5; // stack automatic variable
        assert(5 == i);

        // Heap via C++ new including initialization invoking copy-ctor,
        // then assigned ownership to std::unique_ptr<int>.
        std::unique_ptr<int> p_j( new int(6) );
        assert(6 == *p_j);

        // Implicit heap via C++ new from `std::make_unique<int>` including initialization invoking copy-ctor.
        std::unique_ptr<int> p_k = std::make_unique<int>( 7 );
        assert(7 == *p_k);

        // Implicit heap via C++ new from `std::make_shared<int>` including initialization invoking copy-ctor.
        std::shared_ptr<int> p_l = std::make_shared<int>( 8 );
        assert(8 == *p_l);

        // Shared use of p_l and its heap, latter only gets destructed if both std::shared_ptr<int> instances are destructed.
        std::shared_ptr<int> p_l2 = p_l;
        assert(8 == *p_l2);

        // Leaving this scope destructs all automatic allocated resources of this block: `i`, as well as `p_j`, `p_k`, `p_l` and `p_l2` inclusive their heap.
    }

    // Arrays and a little piece of pointer-arithmetic
    {
        const size_t len = 10;

        // Using an array of int value via C malloc and manual initialization to desired values
        {
            // Heap for array via C malloc without initialization
            int* array = static_cast<int*>( std::malloc( sizeof(int) * len ) );

            // manual initialization of array to desired values
            for(size_t i=0; i<len; ++i) {
                // `array + i` increments the `int` pointer `array` by `i * sizeof(int)` bytes!
                *( array + i ) = static_cast<int>( i );
            }

            // read and validate array content
            for(size_t i=0; i<len; ++i) {
                // implicit pointer arithmetic of `array[i[` ==  `*( array + i )`
                const int v = array[i];
                assert( static_cast<int>( i ) == v );
            }

            // Demonstrate pointer distance by element-size: 1 == ( array + 1 ) - array
            {
                // p1 and p0 are pointer of type `int`,
                // hence all arithmetic operations on these pointers operate on int-size.
                int* p1_int = array + 1;
                int* p0_int = array;
                size_t element_distance = p1_int - p0_int;
                printf("1 element distance int int-size: %zu, p1 %p, p0 %p\n", element_distance, p1_int, p0_int);
                assert( 1 == ( p1_int - p0_int ) );
            }

            // Demonstrate pointer distance by byte-size: sizeof(int) == ( array + 1 ) - array
            {
                // p1 and p0 are pointer of type `uint8_t` (byte-sized unsigned int),
                // hence all arithmetic operations on these pointers operate on byte-size.
                uint8_t* p1_int = reinterpret_cast<uint8_t*>( array + 1 );
                uint8_t* p0_int = reinterpret_cast<uint8_t*>( array );
                size_t byte_distance = p1_int - p0_int;
                printf("1 element distance in byte-size: %zu, p1 %p, p0 %p\n", byte_distance, p1_int, p0_int);
                assert( sizeof(int) == ( p1_int - p0_int ) );
            }

            // Explicit destruction of allocated memory!
            std::free(array);
            array = nullptr; // convention, but not required here as impossible to reuse due to running out of scope
        }

        // Using an array of int value via C++ new[], its default initialization _and_ manual initialization to desired values
        {
            // Heap for array via C++ new[] with default constructor initialization
            int* array = new int[len];

            // manual initialization of array to desired values
            for(size_t i=0; i<len; ++i) {
                // implicit pointer arithmetic of `array[i[` ==  `*( array + i )`
                array[i] = static_cast<int>( i );
            }

            // read and validate array content
            for(size_t i=0; i<len; ++i) {
                // implicit pointer arithmetic of `array[i[` ==  `*( array + i )`
                const int v = array[i];
                assert( static_cast<int>( i ) == v );
            }

            // Explicit destruction of allocated memory!
            delete array;
            array = nullptr; // convention, but not required here as impossible to reuse due to running out of scope
        }

        // Using an array of int value via C malloc and C++ placement new to desired values
        {
            // Heap for array via C malloc without initialization
            int* array = static_cast<int*>( std::malloc( sizeof(int) * len ) );

            // manual initialization of array to desired values
            for(size_t i=0; i<len; ++i) {
                // placement-new with given memory-address and `int` copy-ctor
                new( array + i ) int( static_cast<int>( i ) );
            }

            // read and validate array content
            for(size_t i=0; i<len; ++i) {
                // implicit pointer arithmetic of `array[i[` ==  `*( array + i )`
                const int v = array[i];
                assert( static_cast<int>( i ) == v );
            }

            // Manual destructor call matching `placement-new` above,
            // only valid for non build-in types
            // and required in case type's constructor has side-effects, i.e. a non `TriviallyCopyable` type.
            //
            // This is not the case for `int`, as it is a `TriviallyCopyable` type and even has no destructor to call.
            //
            // for(size_t i=0; i<len; ++i) {
            //    (array + i)->~a_complex_type();
            // }

            // Explicit destruction of allocated memory!
            std::free(array);
            array = nullptr; // convention, but not required here as impossible to reuse due to running out of scope
        }
    }
    return 0;
}
