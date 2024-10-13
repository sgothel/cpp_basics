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
 * Lesson 4.0
 *
 * Implementing binary search on a sorted array and sorted insert
 */

constexpr static const size_t no_index = std::numeric_limits<size_t>::max();

//
// full array value space using size_t, excluding lower and upper bounds initially
//
size_t binary_search(const std::vector<int>& array, int target_value) {
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
        // std::cout << "c " << c << " [" << l << ".." << h << "]: p " << i << std::endl;
        if ( array[i] < target_value ) {
            l = i;
        } else if ( array[i] > target_value ) {
            h = i;
        } else {
            return i;
        }
        ++c;
    }
    (void)c;
    return no_index;
}
size_t ordered_insert(std::vector<int>& array, int value) {
    if( array.size() < 1 ) {
        array.push_back(value);
        return 0;
    }
    // Because std::vector<>::begin() iterator performs arithmetic
    // using a signed difference_type, we need to use such a signed type
    // here to avoid `bugprone-narrowing-conversions` (LINT)
    //
    // Now, isn't this odd as std::vector<>::size() uses unsigned size_type,
    // aka size_t and mentioned iterator hence lose half the value range possible?
    typedef std::vector<int>::difference_type iterdiff_t;
    iterdiff_t l = 0;
    iterdiff_t h = array.cend() - array.cbegin() - 1;
    if ( array[l] >= value ) {
        array.insert(array.begin(), value);
        return l;
    } else if ( array[h] <= value ) {
        array.insert(array.end(), value);
        return h+1;
    }
    // size_t c = 0;
    while( h - l >= 2 ) {
        // iterdiff_t i = ( l + h ) / 2; // l+h too big?
        iterdiff_t i = l + ( h - l ) / 2; // better, also solved with std::midpoint(l, h)
        // std::cout << "c " << c << " (" << l << ".." << h << "): p " << i << std::endl;
        if ( array[i] < value ) {
            l = i;
        } else if ( array[i] > value ) {
            h = i;
        } else {
            array.insert(array.begin() + i, value);
            return i;
        }
        // ++c;
    }
    array.insert(array.begin() + h, value);
    return h;
}

bool test_binsearch(const std::vector<int>& array, int target_value, size_t exp_idx, size_t& has_idx) {
    has_idx = binary_search(array, target_value);
    if( exp_idx != has_idx ) {
        std::cerr << "Error: has " << has_idx << " != exp " << exp_idx << std::endl;
        return false;
    } else {
        return true;
    }
}
void test_binsearch(std::vector<int>& array_in, std::vector<int>& array_miss, int line) {
    for(size_t i=0; i < array_in.size(); ++i) {
        size_t idx;
        if( !test_binsearch(array_in, array_in[i], i, idx) ) {
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
            if( !test_binsearch(array_in, target, no_index, idx) ) {
                std::cout << "ERROR-2: " << target << " found at " << idx << std::endl;
                assert(false);
            } else {
                std::cout << "OK   : " << target << " not found, idx " << idx << std::endl;
            }
        }
    }
}

void printVec(const std::string& prefix, const std::vector<int>& v) {
    std::cout << prefix << ": Vec sz " << v.size() << ": ";
    for(size_t k=0; k<v.size(); ++k) {
        std::cout << "[" << k << "] " << v[k] << ", ";
    }
    std::cout << std::endl;
}
void test_ordered_insert(std::vector<int>& array_in, const std::vector<int>& array_exp) {
    printVec("INP", array_in);
    printVec("EXP", array_exp);
    std::vector<int> array;
    array.reserve( array_in.size() ); // be nice
    for(size_t i=0; i<array_in.size(); ++i) {
        const int v = array_in[i];
        const size_t p = ordered_insert(array, v);
        // printVec("added "+std::to_string(v)+" to "+std::to_string(p), array);
        assert(array[p] == v);
    }
    printVec("OUT", array);
    assert( array.size() == array_in.size() );
    assert( array.size() == array_exp.size() );
    assert( array == array_exp );
}

int main(int, const char**) {

    // test binary search
    {
        std::vector<int> array1_in = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::vector<int> array1_miss = { -1, 10 };

        std::vector<int> array2_in = { 1, 3, 4, 5, 6, 9, 11 };
        std::vector<int> array2_miss = { 0, 2, 7, 8, 10, 12 };

        test_binsearch(array1_in, array1_miss, __LINE__);
        test_binsearch(array2_in, array2_miss, __LINE__);
    }
    // test ordered insert
    {
        {
            std::vector<int> array_in  = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
            std::vector<int> array_exp = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            test_ordered_insert(array_in, array_exp);
        }
        {
            std::vector<int> array_in  = { 2, 8, 1, 6, 0, 9, 4, 7, 3, 5 };
            std::vector<int> array_exp = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            test_ordered_insert(array_in, array_exp);
        }

    }
    return 0;
}
