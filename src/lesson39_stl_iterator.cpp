//============================================================================
// Author      : Svenson Han Göthel and Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 4.0 Simple binary search and sorted insert algorithm using C++
//============================================================================

#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>

#include <limits>
#include <cassert>

/**
 * Lesson 3.9 STL iterator and difference_type limitations (signed integer)
 */

int main(int, const char**) {
    // regular pointer arithmetic adding usigned integer size_t,
    // i.e. using maximum value range
    {
        int array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int* plala = &array[0];
        for(size_t i=0; i<9l; ++i) {
            array[i] = *(plala + i) + 1;
        }        
    }

    // Because std::vector<>::begin() iterator performs arithmetic 
    // using a signed difference_type, we need to use such a signed type 
    // here to avoid `bugprone-narrowing-conversions` (LINT)
    //
    // Now, isn't this odd as std::vector<>::size() uses unsigned size_type,
    // aka size_t and mentioned iterator hence lose half the value range possible?
    {        
        // index operator[] can use unsigned size_t, full range
        std::vector<int> array(10, 0);
        for(size_t i=0; i<array.size(); ++i) {
            // using unsigned int as index is legal, also full range
            array[i] += 1; 
        }
        // now iterating via iterator, loss of full range
        typedef std::vector<int>::difference_type iterdiff_t;
        iterdiff_t sz = array.cend() - array.cbegin() - 1;
        for(iterdiff_t i=0; i<sz; ++i) {
            // begin() iterator is signed hence adding signed difference_type, loss of full range
            // same is true w/ all other operations like insert where an iterator is being used
            *(array.begin() + i) += 1;             
        }
        // or .. same issues
        typedef std::vector<int>::iterator iter_t;
        for(iter_t i=array.begin(); i < array.end(); ++i) {
            *i += 1;
        }
    }

    return 0;
}
