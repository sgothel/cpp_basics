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
            : store(0) {}

            constexpr ValueType(int64_t v) noexcept
            : store(v) {}

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
    size_t qsort(std::vector<V>& A, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        // Partitioning:
        // Stick with using references for comparison, no copy
        size_t l=b;   // left index
        size_t r=e-1; // right index -> pivot point
        const V& p = A[b]; // Pivot, ref only
        while( true ) {
            // b -> low pivot index
            while(A[l] < p) { ++l; }

            while(A[r] > p) { --r; }

            if(l >= r) {
                break;
            }
            std::swap(A[l], A[r]);
        }
        // printVec(array, b, e, pivot);

        // Recursion:
        size_t c = 1;
        c += qsort(A, b,   r+1); // left side of pivot, pivot included
        c += qsort(A, r+1, e);   // right side of pivot
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
    size_t qsort(std::vector<V>& A, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        // Partitioning:
        // Stick with using references for comparison, no copy
        size_t l=b;   // left index  -> pivot-point
        size_t r=e-2; // right index
        const size_t hi = e-1;
        const V& p = A[hi]; // Pivot, ref only
        while( true ) {
            while(A[l] < p) { ++l; }

            while(r > 0 && A[r] > p) { --r; }

            // std::cout << ": [" << l << ".." << r << "]" << std::endl;

            if( r > l ) {
                std::swap(A[l], A[r]);
            } else {
                std::swap(A[l], A[hi]); // move pivot to final position
                break; // done
            }
        }
        // printVec(A, b, e, pivot);

        // Recursion:
        size_t c = 1;
        c += qsort(A, b,   l); // left side of pivot
        c += qsort(A, l+1, e); // right side of pivot
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
    size_t qsort(std::vector<V>& A, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        // Partitioning:
        // Stick with using references for comparison, no copy
        const size_t hi = e - 1;
        const V& p = A[hi]; // Pivot, ref only
        size_t l = b; // pivot point
        for(size_t j = b; j < hi; ++j) {
            if( A[j] <= p ) { // pivot value array[hi]
                std::swap(A[l], A[j]);
                ++l; // move temp pivot index forward
            }
        }
        std::swap(A[l], A[hi]); // move pivot to final position
        // printVec(array, b, e, pivot);

        // Recursion:
        size_t c = 1;
        c += qsort(A, b,   l); // left side of pivot
        c += qsort(A, l+1, e); // right side of pivot
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
     * Hoare alike quicksort of range [b..e).
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
     * @return number of partitioning
     */
    template<typename V>
    size_t qsort(std::vector<V>& A, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        A.reserve( A.size() + 1 );
        // Partitioning:
        // Stick with using references for comparison, no copy
        // size_t l = ( b + e - 1 ) / 2; // pivot point - sum too big?
        size_t l = b + ( e - 1 - b ) / 2; // pivot point - better, also solved with std::midpoint(b, e-1)
        for(size_t i = b; i < l; ) {
            if( A[i] > A[l] ) {
                A.insert(A.begin() + e, A[i]);
                A.erase( A.begin() + i );
                --l;
            } else {
                ++i;
            }
        }
        for(size_t i = l+1; i < e; ) {
            if( A[i] < A[l] ) {
                A.insert(A.begin() + b, A[i]);
                ++i;
                ++l;
                A.erase( A.begin() + i );
            } else {
                ++i;
            }
        }
        // printVec(array, b, e, pivot);

        // Recursion:
        size_t c = 1;
        c += qsort(A, b,   l); // left side of pivot
        c += qsort(A, l+1, e); // right side of pivot
        return c;
    }
    template<typename V>
    size_t qsort(std::vector<V>& array) {
        return qsort(array, 0, array.size());
    }
}

namespace hoare3 {

    using namespace impl_common;

    /**
     * Hoare-Yaroslavskiy dual-pivot quicksort of range [b..e).
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
    size_t qsort(std::vector<V>& A, size_t b, size_t e) {
        if( e - b < 2 ) {
            return 0;
        }
        // Partitioning:
        // Stick with using references for comparison, no copy
        const size_t hi = e-1;
        size_t l = b + 1, g = hi - 1; // pivot points
        {
            if( A[b] > A[hi] ) {
                std::swap(A[b], A[hi]);
            }
            const V& p = A[b];  // Pivot 1, ref only
            const V& q = A[hi]; // Pivot 2, ref only
            size_t k = l;
            while (k <= g) {
                if (A[k] < p) {
                    std::swap(A[k], A[l]); ++l;
                } else if( A[k] >= q ) {
                    while( A[g] > q && k < g ) {
                     --g;
                    }
                    std::swap(A[k], A[g]); --g;
                    if( A[k] < p ) {
                        std::swap(A[k], A[l]); ++l;
                    }
                }
                ++k;
            }
            --l; ++g;
            std::swap(A[b],  A[l]);
            std::swap(A[hi], A[g]);
        }

        // Recursion
        size_t c = 1;
        c += qsort(A, b,   l); // 1st segment, ex-pivot
        c += qsort(A, l+1, g); // 2nd segment, ex-pivot
        c += qsort(A, g+1, e); // 3rd segment, ex-pivot
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
void test_qsort(const std::string& prefix, const test_vector_t& has, const test_vector_t& exp) {
    test_qsort("qsort-hoare_goth-"+prefix, hoare2::qsort, has, exp);
    test_qsort("qsort-hoare_sedg-"+prefix, hoare1::qsort, has, exp);
    test_qsort("qsort-hoare_tony-"+prefix, hoare0::qsort, has, exp);
    test_qsort("qsort-lumoto____-"+prefix, lumoto::qsort, has, exp);
    test_qsort("qsort-hoare_yaro-"+prefix, hoare3::qsort, has, exp);
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
