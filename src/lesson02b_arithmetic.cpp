//============================================================================
// Author      : Svenson Han Gothel and Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 0.2b Applied arithmetic using C++
//============================================================================

#include <iostream>
#include <cstdio>
using namespace std;

/**
 * Returns sum of all integer between [a, b].
 *
 * Implementation utilizes a simple loop,
 * hence duration increases with increased distance between b - a.
 *
 * Implementation does not solve in O(1), i.e. constant time,
 * but linear time O(n).
 *
 * @param a
 * @param b
 * @return
 */
long summe_loop( long a, long b){
    long r = 0;
    /**
    for( long i = a; i <= b; ++i) {
        r = r + i;
    } */
    while( a <= b ) {
        r += a;
        ++a;
    }
    return r;
}


/**
 * Returns sum of all integer between [a, b].
 *
 * Implementation utilizes F Gaus sum equation,
 * hence operates in constant time O(1).
 *
 * @param a
 * @param b
 * @return
 */
long summe_gaus( long a, long b ){
   double n = b - a + 1;
   double r = ( a + b ) * ( n / 2.0 );
   return (long)r;
}

typedef long(*summe_func)(long a, long b);

bool test_summe0(summe_func func, const char* func_name,
                 long exp, long a, long b )
{
    long e = func(a, b);
    bool ok = exp == e;
    printf("%s [%ld..%ld] -> %ld: ok %d\n", func_name, a, b, e, ok);
    return ok;
}

bool test_summe( long exp, long a, long b ) {
    bool ok1 = test_summe0(summe_loop, "Summe Loop", exp, a, b );
    bool ok2 = test_summe0(summe_gaus, "Summe Gaus", exp, a, b );
    return ok1 && ok2;
}

int main() {
	cout << "Teste Summe LOOP" << endl;
	int r = 0;
	if( test_summe( 0, 0, 0 ) == false) {
	    r++;
	}
    if( test_summe( 1, 1, 1 ) == false) {
        r++;
    }
    if( test_summe( 3, 3, 3 ) == false) {
        r++;
    }
    if( test_summe( 4, 4, 4 ) == false) {
        r++;
    }
    if( test_summe( 3, 1, 2 ) == false) {
        r++;
    }
    if( test_summe( 6, 1, 3 ) == false) {
        r++;
    }
    if( test_summe( 55, 1, 10 ) == false) {
        r++;
    }
    if( test_summe( 500000000500000000L, 1, 1000000000L ) == false) {
        r++;
    }
    printf("Summe Fehler %d\n",r);

	return 0;
}
