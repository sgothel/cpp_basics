//============================================================================
// Author      : Svenson Han Göthel and Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 4.0 Simple binary search algorithms using C++
//============================================================================

#include <cstdio>
#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>

#include <limits>
#include <cassert>

/**
 * Lesson 4.0
 *
 * Implementing binary search on a sorted array
 */

typedef ssize_t (*binary_search_func0_t)(const std::vector<int>& array, int target_value);
typedef size_t (*binary_search_func1_t)(const std::vector<int>& array, int target_value);

bool test_binsearch0(binary_search_func0_t binary_search, const std::vector<int>& array, int target_value, ssize_t exp_idx, ssize_t& has_idx) {
    has_idx = binary_search(array, target_value);
    if( exp_idx != has_idx ) {
        std::cerr << "Error: has " << has_idx << " != exp " << exp_idx << std::endl;
        return false;
    } else {
        return true;
    }
}
bool test_binsearch1(binary_search_func1_t binary_search, const std::vector<int>& array, int target_value, size_t exp_idx, size_t& has_idx) {
    has_idx = binary_search(array, target_value);
    if( exp_idx != has_idx ) {
        std::cerr << "Error: has " << has_idx << " != exp " << exp_idx << std::endl;
        return false;
    } else {
        return true;
    }
}

//
// naive implementation halving array value space using ssize_t with negative indices
//
constexpr static const ssize_t no_index_0 = -1;

ssize_t binary_search00(const std::vector<int>& array, int target_value) {
    // Because std::vector<>::begin() iterator performs arithmetic
    // using a signed difference_type, we need to use such a signed type
    // here to avoid `bugprone-narrowing-conversions` (LINT)
    //
    // Now, isn't this odd as std::vector<>::size() uses unsigned size_type,
    // aka size_t and mentioned iterator hence lose half the value range possible?
    typedef std::vector<int>::difference_type iterdiff_t;
    iterdiff_t l = 0;
    iterdiff_t h = array.cend() - array.cbegin() - 1;
    iterdiff_t c = 0;
    while( l <= h ) {
        // iterdiff_t i = ( l + h ) / 2; // l+h too big?
        iterdiff_t i = l + ( h - l ) / 2; // better, also solved with std::midpoint(l, h)
        std::cout << "c " << c << " [" << l << ".." << h << "]: p " << i << std::endl;
        if ( array[i] < target_value ) {
            l = i + 1;
        } else if ( array[i] > target_value ) {
            h = i - 1;
        } else {
            return i;
        }
        ++c;
    }
    return no_index_0;
}

//
// full array value space using size_t, but additional limit check avoiding underflow
//
constexpr static const size_t no_index = std::numeric_limits<size_t>::max();

size_t binary_search10(const std::vector<int>& array, int target_value) {
    size_t l = 0;
    size_t h = array.size()-1;
    size_t c = 0;
    while( l <= h ) {
        // size_t i = ( l + h ) / 2; // l+h too big?
        size_t i = l + ( h - l ) / 2; // better, also solved with std::midpoint(l, h)
        std::cout << "c " << c << " [" << l << ".." << h << "]: p " << i << std::endl;
        if ( array[i] < target_value ) {
            l = i + 1;
        } else if ( array[i] > target_value ) {
            if( h == 0 ) {
                return no_index;
            }
            h = i - 1;
        } else {
            return i;
        }
        ++c;
    }
    return no_index;
}

//
// full array value space using size_t, excluding lower and upper bounds initially
//

size_t binary_search11(const std::vector<int>& array, int target_value) {
    size_t l = 0;
    size_t h = array.size()-1;
    if ( array[l] == target_value ) {
        return l;
    } else if ( array[h] == target_value ) {
        return h;
    }
    size_t c = 0;
    while( h - l >= 2 ) {
        // size_t i = ( l + h ) / 2; // l+h too big?
        size_t i = l + ( h - l ) / 2; // better, also solved with std::midpoint(l, h)
        std::cout << "c " << c << " [" << l << ".." << h << "]: p " << i << std::endl;
        if ( array[i] < target_value ) {
            l = i;
        } else if ( array[i] > target_value ) {
            h = i;
        } else {
            return i;
        }
        ++c;
    }
    return no_index;
}

void test_binsearch0(binary_search_func0_t binary_search, std::vector<int>& array_in, std::vector<int>& array_miss, int line) {
    // Because std::vector<>::begin() iterator performs arithmetic
    // using a signed difference_type, we need to use such a signed type
    // here to avoid `bugprone-narrowing-conversions` (LINT)
    //
    // Now, isn't this odd as std::vector<>::size() uses unsigned size_type,
    // aka size_t and mentioned iterator hence lose half the value range possible?
    const ssize_t ain_sz = array_in.cend() - array_in.cbegin();
    for(ssize_t i=0; i < ain_sz; ++i) {
        ssize_t idx;
        if( !test_binsearch0(binary_search, array_in, array_in[i], i, idx) ) {
            std::cout << "ERROR-1 @ " << line << ": array_in[" << i << "] = " << array_in[i] << " found at " << idx << std::endl;
            assert(false);
        } else {
            std::cout << "OK   : array_in[" << i << "] = " << array_in[i] << " found at " << idx << std::endl;
        }
    }

    const ssize_t amiss_sz = array_miss.cend() - array_miss.cbegin();
    for(ssize_t i=0; i < amiss_sz; ++i) {
        const int target = array_miss[i];
        ssize_t idx;
        if( !test_binsearch0(binary_search, array_in, target, no_index_0, idx) ) {
            std::cout << "ERROR-2: " << target << " found at " << idx << std::endl;
            assert(false);
        } else {
            std::cout << "OK   : " << target << " not found, idx " << idx << std::endl;
        }
    }
}

void test_binsearch1(binary_search_func1_t binary_search, std::vector<int>& array_in, std::vector<int>& array_miss, int line) {
    for(size_t i=0; i < array_in.size(); ++i) {
        size_t idx;
        if( !test_binsearch1(binary_search, array_in, array_in[i], i, idx) ) {
            std::cout << "ERROR-1 @ " << line << ": array_in[" << i << "] = " << array_in[i] << " found at " << idx << std::endl;
            assert(false);
        } else {
            std::cout << "OK   : array_in[" << i << "] = " << array_in[i] << " found at " << idx << std::endl;
        }
    }
    {
        for(size_t i=0; i < array_miss.size(); ++i) {
            const int target = array_miss[i];
            size_t idx;
            if( !test_binsearch1(binary_search, array_in, target, no_index, idx) ) {
                std::cout << "ERROR-2: " << target << " found at " << idx << std::endl;
                assert(false);
            } else {
                std::cout << "OK   : " << target << " not found, idx " << idx << std::endl;
            }
        }
    }
}

int main(int, const char**) {
    std::vector<int> array1_in = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> array1_miss = { -1, 10 };

    std::vector<int> array2_in = { 1, 3, 4, 5, 6, 9, 11 };
    std::vector<int> array2_miss = { 0, 2, 7, 8, 10, 12 };

    // test impl00
    {
        test_binsearch0(binary_search00, array1_in, array1_miss, __LINE__);
        test_binsearch0(binary_search00, array2_in, array2_miss, __LINE__);
    }
    // test impl10
    {
        test_binsearch1(binary_search10, array1_in, array1_miss, __LINE__);
        test_binsearch1(binary_search10, array2_in, array2_miss, __LINE__);
    }
    // test impl11
    {
        test_binsearch1(binary_search11, array1_in, array1_miss, __LINE__);
        test_binsearch1(binary_search11, array2_in, array2_miss, __LINE__);
    }
    return 0;
}
