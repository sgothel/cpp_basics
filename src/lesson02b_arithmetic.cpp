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
 * Friedrich Gauss Summenformel 'Little Gauss'
 *
 *   1+2+3+4+5+6
 *
 *   a         b
 *   1 2 3 4 5 6
 *       3+4      =  7
 *     2  +  5    =  7
 *   1    +    6  =  7
 *   a    +    b
 *   1+2+3+4+5+6  = 21
 *
 *   6 / 2 = 3
 *   b / 2
 *
 *   (a + b) * b/2
 *
 *   1+2+3+4+5+6  = 3 * 7
 *
 *   a    +    b
 *   1    +    6  = 7
 *
 * --------------------
 *
 *   1+2+3+4+5+6+7+8
 *
 *   a             b
 *   1 2 3 4 5 6 7 8
 *         4+5       =  9
 *       3  +  6     =  9
 *     2    +    7   =  9
 *   1      +      8 =  9
 *   a      +      b
 *   1+2+3+4+5+6+7+8 = 36
 *
 *   8 / 2 = 4
 *   b / 2
 *
 *   (a + b) * b/2
 *
 * --------------------
 *
 *   1+2+3+4+5 = 15
 *
 *   a       b
 *   1 2 3 4 5
 *
 *       3        =  3
 *     2 + 4      =  6
 *   1   +   5    =  6
 *   a       b
 *   1+2+3+4+5    = 15
 *
 *   n = b-a+1 == b, mit a==1
 *   n = 6 -> n/2 = 6/2 = 3
 *
 *   5 / 2 = 2 + 1/2
 *   b / 2
 *
 *   (a + b) * b/2
 *
 *   = 6 * ( 2 + 1/2 )
 *   = 6 * 2 + 6 * 1/2
 *   = 12 + 3
 *   = 15
 *
 * --------------------
 * --------------------
 *
 *   3+4+5+6+7+8
 *
 *   a         b
 *   3 4 5 6 7 8
 *       5+6       = 11
 *     4  +  7     = 11
 *   3    +    8   = 11
 *   a    +    b
 *   3+4+5+6+7+8   = 33
 *
 *   n = b-a+1
 *   n = 6 -> n/2 = 6/2 = 3
 *
 *   (3 + 8) * 3   = 33
 *
 *   (a + b) * (b-a+1)/2
 *
 * --------------------
 *
 *   3+4+5+6+7
 *
 *   a       b
 *   3 4 5 6 7
 *       5        =  5
 *     4 + 6      = 10
 *   3   +   7    = 10
 *   a   +   b
 *   3+4+5+6+7    = 25
 *
 *   n = b-a+1
 *   n = 7-3+1 = 5
 *   n = 5 -> n/2 = 5/2 = 2 + 1/2
 *
 *  Would require floating type for 5/2
 *  1:   (3 + 7) * 5/2
 *  2: ( (3 + 7) * 5 ) /2
 *
 *  Works with int type, but vulnerable to overflow
 *  1:   (3 + 7) *   5 / 2     = 25
 *  1:   (3 + 7) * ( 2 + 1/2 ) = 25
 *  1:        10 * ( 2 + 1/2 ) = 25
 *  1:       10 * 2 + 10 * 1/2 = 25
 *  1:                  20 + 5 = 25
 *  1:   (a + b) * (b-a+1)/2
 *
 *  2: ( (3 + 7) * 5 ) / 2     = 25
 *  2: (      10 * 5 ) / 2     = 25
 *  2: (          50 ) / 2     = 25
 *  2:                  25     = 25
 *  2: ( (a + b) * (b-a+1) ) / 2
 *
 * @param a
 * @param b
 * @return
 */
long summe_gaus( long a, long b ){
   // would require floating type for n/2: ( a + b ) * ( n / 2 )
   const int n = b - a + 1;
   return ( ( a + b ) * n ) / 2;
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
