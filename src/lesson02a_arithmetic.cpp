//============================================================================
// Author      : Svenson Han Gothel and Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 0.2a Applied arithmetic using C++
//============================================================================

#include <cstdio>
#include <iostream>
#include <cmath>

#include <limits>

/**
 * Lesson 0.2
 *
 * Implementing simple arithmetic functionality
 */

struct DivisionResult {
	int factor;
	int remainder;
};
DivisionResult divide(const int z, const int n) {
	int r=0;
    int c=z;
    while ( c >= n ) {
    	c -= n; // c = c - n;
    	++r; // r = r + 1;
    }
	return DivisionResult { .factor=r, .remainder=c };
}

int mult(const int a, const int b) {
	int r=0;
	for (int i=0; i<a; ++i) {
		r += b; // r=r+b;
	}
	return r;
}

int summe_iter(const int a, const int b) {
	int r=0;
	for (int i=a; i<=b; ++i) {
		r += i; // r=r+i;
	}
	return r;
}

int summe(const int a, const int b) {
	// would require floating type for n/2: ( a + b ) * ( n / 2 )
	const int n = b - a + 1;
	return ( ( a + b ) * n ) / 2;
}

// function declaration
int my_pow(const int x, const int y);

// function definition
int my_pow(const int x, const int y) {
	int r=1;
	for (int i=0; i<y; ++i){
		r=mult(r, x); // r = r * x
	}
	return r;
}

int factorial(int v) {
    // factorial(5) = 1*2*3*4*5
    int r = v;
    while( v > 1 ) {
        // v = v - 1;
        // r = r * v;
        r *= --v;
    }
    return r;
}

float machineEpsilon() {
    const float one(1);
    const float two(2);
    float x = 1;
    float res;
    // float t;
    int i=0;
    do {
        res = x;
        x = x / two;
        // t = one + x;
        // fprintf(stderr, "%3d: %.48f -> %.48f, %.48f\n", i, res, x, t);
        // std::cerr << i << ": " << res << " -> " << x << std::endl;
        ++i;
    } while ( one + x  > one );
    (void)i;
    return res;
}

void test_limits() {
    unsigned int i = std::numeric_limits<unsigned int>::max() - 10;
    do {
        fprintf(stderr, "%u, bytes %zu\n", i, sizeof(i));
        if( i == std::numeric_limits<unsigned int>::max() ) {
            break; // end outter loop
        }
        ++i;
    } while( true );
}

int main(int argc, const char* argv[]) {
    {
        // loop through all program invocation arguments and print them
        for(int i=0; i<argc; ++i) {
            printf("cmd_arg[%d]: %s\n", i, argv[i]);
        }
    }
    {
        float e = machineEpsilon();
        fprintf(stderr, "machineEpsilon: %.48f\n", e);
    }
    {
        test_limits();
    }

    int error_counter = 0;

	{
		for(int numerator = 0; numerator < 100; ++numerator) {
			for(int denominator=1; denominator<=10; ++denominator) {
				const int expected = numerator / denominator;
				const int expected_r = numerator % denominator;
				const DivisionResult has = divide(numerator, denominator);

				if( expected != has.factor || expected_r != has.remainder ) {
					printf("Error-1: %d / %d != %d + %d / %d, error result %d + %d / %d\n",
							numerator, denominator,
							expected, expected_r, denominator,
							has.factor, has.remainder, denominator);
					++error_counter;
				}

				const int test_numerator = denominator*has.factor+has.remainder;
				if( test_numerator != numerator ) {
                    printf("Error-2: %d * %d + %d != %d, error result %d + %d / %d\n",
                            denominator, has.factor, has.remainder,
                            numerator,
                            has.factor, has.remainder, denominator);
                    ++error_counter;
				}
			}
		}
	}
	{
		for(int a = 0; a <= 10; ++a) {
			for(int b = 0; b <= 10; ++b) {
				const int expected = a*b;
				const int has = mult(a, b);
				if( expected != has ) {
					printf("Error: %d * %d != %d, error result %d\n",
							a, b, expected, has);
					++error_counter;
				}
			}
		}
	}
	if( true ) {
		for(int a = 0; a <= 10; ++a) {
			for(int b = a; b <= a + 10; ++b) {
				const int expected = summe_iter(a, b);
				const int has = summe(a, b);
				if( expected != has ) {
					printf("Error: Sum[%d .. %d] != %d, error result %d\n",
							a, b, expected, has);
					++error_counter;
				}
			}
		}
	}
	{
		for(int a = 0; a <= 10; ++a) {
			for(int b=0; b <= 5; ++b) {
				const int expected = (int)std::pow(a, b);
				const int has = my_pow(a, b);
				if( expected != has ) {
					printf("Error: %d ** %d != %d, error result %d ? ",
							a, b, expected, has);
					++error_counter;
				}
			}
		}
    }
	{
	    int last_exp = 1;
        for(int a = 0; a <= 10; ++a) {
            const int expected = last_exp * a;
            if( 0 < expected) {
                last_exp = expected;
            }
            const int has = factorial(a);
            if( expected != has ) {
                printf("Error: factorial(%d) != %d, error result %d ? ",
                        a, expected, has);
                ++error_counter;
            } else {
                printf("factorial(%d) = %d\n", a, has);
            }
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
