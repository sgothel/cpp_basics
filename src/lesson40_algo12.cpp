//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 4.0 A divide-and-conquer algorithm (quicksort)
//===============================================================================
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

#include <cassert>

namespace test_env {
    /**
     * Test Value value type implementing constraints
     *
     * Properties:
     * - copyable
     * - assignable
     * - comparable
     *
     * NOT properties (not implement):
     * - arithmetic operators
     */
    class ValueType {
        private:
            int64_t store;

        public:
            constexpr ValueType() noexcept
            : store(0) { }

            constexpr ValueType(int64_t v) noexcept
            : store(v) { }

            constexpr ValueType(const ValueType& o) noexcept = default;
            constexpr ValueType(ValueType&& o) noexcept = default;
            constexpr ValueType& operator=(const ValueType&) noexcept = default;
            constexpr ValueType& operator=(ValueType&&) noexcept = default;

            constexpr int64_t value() const noexcept { return store; }

            constexpr bool operator==(const ValueType& b) const noexcept {
                return store == b.store;
            }

            std::strong_ordering operator<=>(const ValueType& b) const noexcept {
                return store == b.store ? std::strong_ordering::equal :
                                          ( store < b.store ? std::strong_ordering::less : std::strong_ordering::greater );
            }

            std::string toString() const noexcept { return std::to_string(store); }
    };
} // namespace test_env

namespace std {
    inline std::string to_string(const test_env::ValueType& v) noexcept { return std::to_string(v.value()); }

    inline std::ostream& operator<<(std::ostream& out, const test_env::ValueType& v) {
        return out << v.toString();
    }
}

namespace impl_common {

    template<typename V>
    void printVec(const std::vector<V>& v, size_t b, size_t e, size_t p) {
        const size_t range = e - b;
        std::cout << "Vec sz " << v.size() << ": [" << b << ".." << e << ") " << range << ", p " << p << ": ";
        for(size_t k=0; k<v.size(); ++k) {
            std::cout << "[" << k << "] " << v[k] << ", ";
        }
        std::cout << std::endl;
    }

    template<typename V>
    using VecIterator = typename std::vector<V>::iterator;

}

namespace hoare0 {

    using namespace impl_common;

    /**
     * Hoare partitioning of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return pivot index
     */
    template<typename V>
    size_t hoare_partition(std::vector<V>& array, size_t b, size_t e) {
        // stick with using references for comparison, no copy
        size_t lo = b; // pivot index

        size_t l=b;   // left index
        size_t r=e-1; // right index
        while( true ) {
            while(array[l] < array[lo]) { ++l; }

            while(array[r] > array[lo]) { --r; }

            if(l >= r) {
                return r;
            }
            std::swap(array[l], array[r]);
        }
        return l;
    }

    /**
     * Hoare quicksort of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return number of partitioning
     */
    template<typename V>
    size_t qsort(std::vector<V>& array, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        size_t pivot = hoare_partition<V>(array, b, e);
        // printVec(array, b, e, pivot);
        size_t c = 1;
        c += qsort(array, b, pivot+1); // left side of pivot, pivot included
        c += qsort(array, pivot+1, e); // right side of pivot
        return c;
    }
    template<typename V>
    size_t qsort(std::vector<V>& array) {
        return qsort(array, 0, array.size());
    }
}

namespace hoare1 {

    using namespace impl_common;

    /**
     * Hoare-Sedgewick partitioning of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return pivot index
     */
    template<typename V>
    size_t sedgewick_partition(std::vector<V>& array, size_t b, size_t e) {
        // stick with using references for comparison, no copy
        size_t hi = e-1; // pivot index

        size_t l=b;   // left index
        size_t r=e-2; // right index
        while( true ) {
            while(array[l] < array[hi]) { ++l; }

            while(r > 0 && array[r] > array[hi]) { --r; }

            // std::cout << ": [" << l << ".." << r << "]" << std::endl;

            if( r > l ) {
                std::swap(array[l], array[r]);
            } else {
                std::swap(array[l], array[hi]); // move pivot to final position
                return l;
            }
        }
    }

    /**
     * Hoare-Sedgewick quicksort of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return number of partitioning
     */
    template<typename V>
    size_t qsort(std::vector<V>& array, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        size_t pivot = sedgewick_partition<V>(array, b, e);
        // printVec(array, b, e, pivot);
        size_t c = 1;
        c += qsort(array, b, pivot);   // left side of pivot
        c += qsort(array, pivot+1, e); // right side of pivot
        return c;
    }
    template<typename V>
    size_t qsort(std::vector<V>& array) {
        return qsort(array, 0, array.size());
    }
}

namespace lumoto {

    using namespace impl_common;

    /**
     * Lomuto partitioning of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return pivot index
     */
    template<typename V>
    size_t lumoto_partition(std::vector<V>& array, size_t b, size_t e) {
        // stick with using references for comparison, no copy
        const size_t hi = e - 1; // pivot index
        size_t i = b; // temp pivot index

        for(size_t j = b; j < hi; ++j) {
            if( array[j] <= array[hi] ) { // pivot value array[hi]
                std::swap(array[i], array[j]);
                ++i; // move temp pivot index forward
            }
        }
        std::swap(array[i], array[hi]); // move pivot to final position
        return i; // pivot index
    }

    /**
     * Hoare-Lomuto quicksort of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return number of partitioning
     */
    template<typename V>
    size_t qsort(std::vector<V>& array, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        size_t pivot = lumoto_partition<V>(array, b, e);
        // printVec(array, b, e, pivot);
        size_t c = 1;
        c += qsort(array, b, pivot);   // left side of pivot
        c += qsort(array, pivot+1, e); // right side of pivot
        return c;
    }
    template<typename V>
    size_t qsort(std::vector<V>& array) {
        return qsort(array, 0, array.size());
    }
}

namespace hoare2 {

    using namespace impl_common;

    /**
     * Hoare alike partitioning of range [b..e).
     *
     * Difference to Hoare's partitioning is using dedicated loops for each side of the pivot
     * to move the element over to the other side.
     * It uses a middle index pivot.
     *
     * The pivot element is sorted in place and not included in the recursion like
     * Lumoto and Sedgewick but unlike Hoare.
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return pivot index
     */
    template<typename V>
    size_t partition(std::vector<V>& array, size_t b, size_t e) {
        // stick with using references for comparison, no copy
        size_t p_idx = (b + e - 1 ) / 2; // pivot index
        for(size_t i = b; i < p_idx; ) {
            if( array[i] > array[p_idx] ) {
                array.insert(array.begin() + e, array[i]);
                array.erase( array.begin() + i );
                --p_idx;
            } else {
                ++i;
            }
        }
        for(size_t i = p_idx+1; i < e; ) {
            if( array[i] < array[p_idx] ) {
                array.insert(array.begin() + b, array[i]);
                ++i;
                ++p_idx;
                array.erase( array.begin() + i );
            } else {
                ++i;
            }
        }
        return p_idx;
    }

    /**
     * Hoare alike quicksort of range [b..e).
     *
     * Quicksort by Tony Hoare in 1959, published 1961.
     *
     * @tparam V
     * @param array
     * @param b left start index, inclusive
     * @param e right end index, exclusive
     * @return number of partitioning
     */
    template<typename V>
    size_t qsort(std::vector<V>& array, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        array.reserve( array.size() + 1 );
        size_t pivot = partition<V>(array, b, e);
        // printVec(array, b, e, pivot);
        size_t c = 1;
        c += qsort(array, b, pivot);   // left side of pivot
        c += qsort(array, pivot+1, e); // right side of pivot
        return c;
    }
    template<typename V>
    size_t qsort(std::vector<V>& array) {
        return qsort(array, 0, array.size());
    }
}

//
// test code
//

typedef std::vector<test_env::ValueType> test_vector_t;

typedef size_t (*qsort_func)(test_vector_t& array);

void dumpVec(const std::string& prefix, size_t c, const test_vector_t& v) {
    std::cout << prefix << ": sz " << v.size() << ", qs-c " << c << ": ";
    for(size_t k=0; k<v.size(); ++k) {
        std::cout << "[" << k << "] " << v[k] << ", ";
    }
    std::cout << std::endl;
}

void test_qsort(const std::string& prefix, qsort_func qsort, test_vector_t has, const test_vector_t& exp) {
    size_t c = qsort(has);
    dumpVec(prefix+"a", c, has);
    assert( exp == has );
    c = qsort(has);
    dumpVec(prefix+"b", c, has);
    assert( exp == has );
}
void test_qsort(const std::string& prefix, test_vector_t has, const test_vector_t& exp) {
    test_qsort("qsort-hoare_goth-"+prefix, hoare2::qsort, has, exp);
    test_qsort("qsort-hoare_sedg-"+prefix, hoare1::qsort, has, exp);
    test_qsort("qsort-hoare_tony-"+prefix, hoare0::qsort, has, exp);
    test_qsort("qsort-lumoto____-"+prefix, lumoto::qsort, has, exp);
}

int main() {
    {
        test_vector_t vec({ 1, 8, 3, 4, 2, 9, 5, 7, 0, 6 });
        test_vector_t exp({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
        test_qsort("set01", vec, exp);
    }
    {
        test_vector_t vec({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
        test_vector_t exp({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
        test_qsort("set02", vec, exp);
    }
    {
        test_vector_t vec({ 1, 8, 4, 2, 9, 7, 6 });
        test_vector_t exp({ 1, 2, 4, 6, 7, 8, 9 });
        test_qsort("set01", vec, exp);
    }
    {
        test_vector_t vec({ 8, 4 });
        test_vector_t exp({ 4, 8 });
        test_qsort("set01", vec, exp);
    }
    return 0;
}
