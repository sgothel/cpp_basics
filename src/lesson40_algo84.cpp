//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 4.0 A custom interval map using C++
//============================================================================
#include <iostream>
// #include <iterator>
#include <string>
#include <map>

#include <concepts>
#include <functional>

#include <cassert>

namespace test_env {
    /**
     * Test Key value type implementing constraints
     *
     * Properties:
     * - copyable
     * - assignable
     * - comparable via <
     *
     * NOT properties (not implement):
     * - comparable via ==, <=, >, >=
     * - arithmetic operators
     */
    class KeyType {
        private:
            int64_t store;

        public:
            constexpr KeyType() noexcept
            : store(0) { }

            constexpr KeyType(int64_t v) noexcept
            : store(v) { }

            constexpr KeyType(const KeyType& o) noexcept = default;
            constexpr KeyType(KeyType&& o) noexcept = default;
            constexpr KeyType& operator=(const KeyType&) noexcept = default;
            constexpr KeyType& operator=(KeyType&&) noexcept = default;

            constexpr int64_t value() const noexcept { return store; }

            bool operator<(const KeyType& b) const noexcept {
                return store < b.store;
            }

            std::string toString() const noexcept { return std::to_string(store); }
    };

    /**
     * Test Value value type implementing constraints
     *
     * Properties:
     * - copyable
     * - assignable
     * - comparable via ==
     *
     * NOT properties (not implement):
     * - comparable via <, <=, >, >=
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

            std::string toString() const noexcept { return std::to_string(store); }
    };
} // namespace test_env

namespace std {
    inline std::string to_string(const test_env::KeyType& v) noexcept { return std::to_string(v.value()); }

    inline std::ostream& operator<<(std::ostream& out, const test_env::KeyType& v) {
        return out << v.toString();
    }

    inline std::string to_string(const test_env::ValueType& v) noexcept { return std::to_string(v.value()); }

    inline std::ostream& operator<<(std::ostream& out, const test_env::ValueType& v) {
        return out << v.toString();
    }
}

void test_interval_map();

namespace feature {

    template<typename T>
    concept IntervalMapKey = requires(T a)
    {
        // { a < a } -> std::convertible_to<bool>;
        { a < a } -> std::same_as<bool>;
    };

    /**
     * Custom interval map
     *
     * - Using a unique (canonical) K -> V m_map, i.e. mapping to a set of values w/o duplicates
     *   - std map, only using std::less<K>, i.e. operator< for key comparison, see below
     * - Using a dedicated interval begin value
     *   - Mapped to all keys not covered by map
     *   - Not contained in first entry of m_map,
     *   - Initially covers whole range of K
     *
     * Each interval [keyBegin, keyEnd) includes keyBegin, but excludes keyEnd.
     *
     * Example:
     *   m_valBegin = 'A', m_map{ (1,'B'), (3,'A') }
     * where value 'B' is mapped to range [1..3)
     *
     * TODO:
     * - Add corner case of no_value, IFF value is contained in map
     * - Inject constraints of K, V via concepts in class template declaration
     *
     * @tparam K only provides operator<
     * @tparam V only provides operator==
     */
    template<typename K, typename V>
    // requires std::equality_comparable_with<V, V>
    class IntervalMap {
      private:
        typedef std::map<K, V, std::less<K> /* default */> map_t;
        typedef typename map_t::iterator map_iterator_t;
        typedef typename map_t::const_iterator const_map_iterator_t;

        map_t m_map;

        V m_valBegin;

        friend void ::test_interval_map();

      public:
        constexpr IntervalMap(const V& valBegin) noexcept
        : m_map(), m_valBegin(valBegin) {}

        constexpr const V& operator[](const K& key) const noexcept {
            const_map_iterator_t it = m_map.upper_bound(key);
            if ( it == m_map.cend() || it == m_map.cbegin() ) {
                return m_valBegin; // includes empty case: it == cend()
            } else {
                return (--it)->second;
            }
        }

        /**
         * Adds an interval to this map
         *
         * Pre-existing intervals are overwritten or split.
         *
         * Invalid given interval keyBegin >= keyEnd is a nop.
         *
         * Complexity O(log(n)) or O(m) with m = period-len (keyEnd-keyBegin)
         *
         * @param keyBegin interval inclusive start
         * @param keyEnd interval exclusive end
         * @param val mapped value to given interval
         * @return true if interval has been successfully added, otherwise false
         */
        bool add( const K& keyBegin, const K& keyEnd, const V& val ) noexcept {
            if( !( keyBegin < keyEnd ) ) {
                return false;
            }
            map_iterator_t it_b = m_map.lower_bound(keyBegin); // O(log(n))
            if( it_b == m_map.end() ) {
                // no key >= b exists, hence [b, e) not included (includes empty + tail)
                m_map.insert(m_map.end(), { keyBegin, val }); // O(1), inserting just before iterator
                m_map.insert(m_map.end(), { keyEnd, m_valBegin }); // O(1), inserting just before iterator
                return true;
            }
            // it_b >= b (keyBegin)

            V end_val = m_valBegin;
            map_iterator_t it;

            if( keyBegin < it_b->first ) {
                // it_b > b (keyBegin)
                it = it_b;
                m_map.insert_or_assign(it, keyBegin, V(val)); // O(1), inserting just before it
            } else {
                // it_b == b (keyBegin)
                end_val = it_b->second;
                it = it_b;
                ++it;
                if( !( it_b->second == val ) ) {
                    end_val = it_b->second;
                    m_map.insert_or_assign(it, keyBegin, V(val)); // O(1), overwrite just before it
                } else {
                    // it_b->second == val
                    // nop, reuse existing start-point
                }
            }

            while( it != m_map.end() && it->first < keyEnd ) { // O(m) w/ m = period-len (keyEnd-keyBegin)
                end_val = it->second;
                it = m_map.erase(it); // O(1)
            }
            if( it != m_map.end() ) {
                // it >= e (keyEnd)
                if( keyEnd < it->first ) {
                    // it > e (keyEnd)
                    m_map.insert_or_assign(it, keyEnd, end_val); // O(1), inserting just before it
                } else {
                    // it == e (keyEnd)
                    // nop, reuse existing end-point
                }
            } else {
                m_map.insert_or_assign(it, keyEnd, end_val); // O(1), inserting just before it
            }
            return true;
        }

        std::string toString() const noexcept {
            std::string s = "size ";
            s.append(std::to_string(m_map.size())).append(": ");
            for (auto const &pair: m_map) {
                s.append( std::to_string( pair.first )).append(" -> ")
                  .append( std::to_string( pair.second ) )
                  .append(", ");
            }
            return s;
        }
    };

} // namespace feature

namespace std {

    template<typename K, typename V>
    std::string to_string(const feature::IntervalMap<K, V>& v) noexcept {
        return std::to_string(v.value());
    }

    template<typename K, typename V>
    std::ostream& operator<<(std::ostream& out, const feature::IntervalMap<K, V>& v) noexcept {
        return out << v.toString();
    }
}

//
// test code
//

typedef feature::IntervalMap<test_env::KeyType, test_env::ValueType> test_interval_map_t;

void dumpMap(const std::string& prefix, const test_interval_map_t& m, int keyBegin, int keyEnd) {
    std::cout << prefix << ": " << m << std::endl;
    std::cout << "  : ";
    for(int k=keyBegin; k<keyEnd; ++k) {
        std::cout << "[" << k << "] " << m[k] << ", ";
    }
    std::cout << std::endl << std::endl;
}

void rangeTest(const test_interval_map_t& m, int keyBegin, int keyEnd, const test_env::ValueType& v, int line) {
    for(int k=keyBegin; k<keyEnd; ++k) {
        test_env::ValueType v_has = m[k];
        if( !(v == v_has) ) {
            std::cerr << "In line " << line << ": failed test: " << v << " == ( m[" << k << "]: " << v_has << " )" << std::endl;
            exit(1);
        }
    }
}
void test_interval_map() {
    // make assert() macro happy, not req if using Catch2
    const test_env::ValueType v_42(42), v_43(43), v_44(44), v_45(45), v_46(46),
                            v_80(80), v_81(81), v_82(82), v_88(88);

    test_interval_map_t im( v_42 ); // empty
    dumpMap("Empty", im, 0, 0);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() == im.m_map.cbegin() );
    rangeTest(im, 0, 20, v_42, __LINE__);

    // Assign first
    // map entries: [5, 7)=44
    assert( im.add(5, 7, v_44) );
    dumpMap("add-1", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 2 == im.m_map.size() );
    rangeTest(im, 0,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_44, __LINE__);
    rangeTest(im, 7, 22, v_42, __LINE__);

    // Assign end
    // map entries: : [5, 7)=44, [17, 19)=46
    assert( im.add(17, 19, v_46) );
    dumpMap("add-2", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 4 == im.m_map.size() );
    rangeTest(im, 0,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_44, __LINE__);
    rangeTest(im, 7, 17, v_42, __LINE__);
    rangeTest(im,17, 19, v_46, __LINE__);
    rangeTest(im,19, 22, v_42, __LINE__);

    // Assign front
    // map entries: : [1, 3)=43, [5, 7)=44, [17, 19)=46
    assert( im.add(1, 3, v_43) );
    dumpMap("add-3", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 6 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_44, __LINE__);
    rangeTest(im, 7, 17, v_42, __LINE__);
    rangeTest(im,17, 19, v_46, __LINE__);
    rangeTest(im,19, 22, v_42, __LINE__);

    // Assign middle
    // map entries: : [1, 3)=43, [5, 7)=44, [8, 10)=45, [17, 19)=46
    assert( im.add(8, 10, v_45) );
    dumpMap("add-4", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 8 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_44, __LINE__);
    rangeTest(im, 7,  8, v_42, __LINE__);
    rangeTest(im, 8, 10, v_45, __LINE__);
    rangeTest(im,10, 17, v_42, __LINE__);
    rangeTest(im,17, 19, v_46, __LINE__);
    rangeTest(im,19, 22, v_42, __LINE__);

    // Assign: keyEnd dropped for next-interval keyBegin
    // map entries: : [1, 3)=43, [5, 7)=44, [8, 10)=45, [15, 17)=80, [17, 19)=46
    assert( im.add(15, 17, v_80) );
    dumpMap("add-5", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 9 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_44, __LINE__);
    rangeTest(im, 7,  8, v_42, __LINE__);
    rangeTest(im, 8, 10, v_45, __LINE__);
    rangeTest(im,10, 15, v_42, __LINE__);
    rangeTest(im,15, 17, v_80, __LINE__);
    rangeTest(im,17, 19, v_46, __LINE__);
    rangeTest(im,19, 22, v_42, __LINE__);

    // Assign: replace 1 interval
    // map entries: : [1, 3)=43, [5, 7)=81, [8, 10)=45, [15, 17)=80, [17, 19)=46
    assert( im.add(5, 7, v_81) );
    dumpMap("add-6", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 9 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_81, __LINE__);
    rangeTest(im, 7,  8, v_42, __LINE__);
    rangeTest(im, 8, 10, v_45, __LINE__);
    rangeTest(im,10, 15, v_42, __LINE__);
    rangeTest(im,15, 17, v_80, __LINE__);
    rangeTest(im,17, 19, v_46, __LINE__);
    rangeTest(im,19, 22, v_42, __LINE__);

    // Assign: complete overwrite of 2 intervals
    // map entries: : [1, 3)=43, [5, 7)=81, [8, 10)=45, [12, 20)=82
    assert( im.add(12, 20, v_82) );
    dumpMap("add-7", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 8 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_81, __LINE__);
    rangeTest(im, 7,  8, v_42, __LINE__);
    rangeTest(im, 8, 10, v_45, __LINE__);
    rangeTest(im,10, 12, v_42, __LINE__);
    rangeTest(im,12, 20, v_82, __LINE__);
    rangeTest(im,20, 22, v_42, __LINE__);

    // Assign: Splitting pre-existing tail interval
    // map entries: : [1, 3)=43, [5, 7)=81, [8, 10)=45, [11, 17)=88, [17, 20)=82
    assert( im.add(11, 17, v_88) );
    dumpMap("add-8", im, 0, 11);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 9 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  5, v_42, __LINE__);
    rangeTest(im, 5,  7, v_81, __LINE__);
    rangeTest(im, 7,  8, v_42, __LINE__);
    rangeTest(im, 8, 10, v_45, __LINE__);
    rangeTest(im,10, 11, v_42, __LINE__);
    rangeTest(im,11, 17, v_88, __LINE__);
    rangeTest(im,17, 20, v_82, __LINE__);
    rangeTest(im,20, 22, v_42, __LINE__);

    // Assign: complete overwrite of 2 intervals
    // map entries: : [1, 3)=43, [4, 10)=81, [11, 17)=88, [17, 20)=82
    assert( im.add(4, 10, v_81) );
    dumpMap("add-9", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 7 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  4, v_42, __LINE__);
    rangeTest(im, 4, 10, v_81, __LINE__);
    rangeTest(im,10, 11, v_42, __LINE__);
    rangeTest(im,11, 17, v_88, __LINE__);
    rangeTest(im,17, 20, v_82, __LINE__);
    rangeTest(im,20, 22, v_42, __LINE__);

    // Assign: Splitting pre-existing head and tail interval
    // map entries: : [1, 3)=43, [4, 7)=81, [7, 14)=45, [14, 17)=88, [17, 20)=82
    assert( im.add(7, 14, v_45) );
    dumpMap("add-10", im, 0, 22);
    assert( v_42 == im.m_valBegin );
    assert( im.m_map.cend() != im.m_map.cbegin() );
    assert( 7 == im.m_map.size() );
    rangeTest(im, 0,  1, v_42, __LINE__);
    rangeTest(im, 1,  3, v_43, __LINE__);
    rangeTest(im, 3,  4, v_42, __LINE__);
    rangeTest(im, 4,  7, v_81, __LINE__);
    rangeTest(im, 7, 14, v_45, __LINE__);
    rangeTest(im,14, 17, v_88, __LINE__);
    rangeTest(im,17, 20, v_82, __LINE__);
    rangeTest(im,20, 22, v_42, __LINE__);
}

int main() {
    test_interval_map();
    return 0;
}
