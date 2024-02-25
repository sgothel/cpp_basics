//============================================================================
// Author      : Svenson Han Göthel and Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 4.0 Simple algorithms using C++
//============================================================================

#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>

#include <limits>

/**
 * Lesson 4.0
 *
 * Implementing simple algorithms
 */

int binary_search(const std::vector<int>& array, int target) {
    int l = 0;
    int h = array.size()-1;
    int i;
    while( l <= h ) {
        i = ( l + h ) / 2;
        if ( array[i] < target ) {
            l = i + 1;
        } else if ( array[i] > target ) {
            h = i - 1;
        } else {
            return i;
        }
    }
    return -1;
}

bool test_binsearch(const std::vector<int>& array, int target, int exp, int& has) {
    has = binary_search(array, target);
    if( exp != has ) {
        printf("Error-1: has %d != exp %d\n", has, exp);
        return false;
    } else {
        return true;
    }
}

int main(int, const char**) {
    int error_counter = 0;
    {
        std::vector<int> array = { 1, 3, 4, 5, 6, 9, 11 };
        for(size_t i=0; i < array.size(); ++i) {
            const int target = array[i];
            int idx;
            if( !test_binsearch(array, target, i, idx) ) {
                std::cout << "ERROR: array[" << i << "] = " << target << " found at " << idx << std::endl;
                ++error_counter;
            } else {
                std::cout << "OK   : array[" << i << "] = " << target << " found at " << idx << std::endl;
            }
        }
        {
            std::vector<int> targets2 = { 0, 2, 7, 8, 10, 12 };
            for(size_t i=0; i < targets2.size(); ++i) {
                const int target = targets2[i];
                int idx;
                if( !test_binsearch(array, target, -1, idx) ) {
                    std::cout << "ERROR: " << target << " found at " << idx << std::endl;
                    ++error_counter;
                } else {
                    std::cout << "OK   : " << target << " not found, idx " << idx << std::endl;
                }
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
