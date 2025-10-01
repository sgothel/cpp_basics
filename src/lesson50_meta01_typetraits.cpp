//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson
//===============================================================================

#include <type_traits>

#include <jau/test/catch2_ext.hpp>

namespace test_01 {
    //
    // C++17 conform template/meta functions using type traits (SFINAE)
    //

    /**
     * Returns the value of the sign function (w/o branching ?) in O(1).
     * <pre>
     * -1 for x < 0
     *  0 for x = 0
     *  1 for x > 0
     * </pre>
     * Implementation is type safe.
     *
     * Branching may occur due to relational operator.
     *
     * @tparam T an arithmetic number type
     * @param x the arithmetic number
     * @return function result
     */
    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                               !std::is_unsigned_v<T>, bool> = true>
    constexpr int sign(const T x) noexcept
    {
        return (int) ( (T(0) < x) - (x < T(0)) );
    }
    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                                std::is_unsigned_v<T>, bool> = true>
    constexpr int sign(const T x) noexcept
    {
        return (int) ( T(0) < x );
    }

    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                               !std::is_unsigned_v<T>, bool> = true>
    constexpr T invert_sign(const T x) noexcept
    {
        return std::numeric_limits<T>::min() == x ? std::numeric_limits<T>::max() : -x;
    }

    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                                std::is_unsigned_v<T>, bool> = true>
    constexpr T invert_sign(const T x) noexcept
    {
        return x;
    }

    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                               !std::is_unsigned_v<T>, bool> = true>
    constexpr T abs(const T x) noexcept
    {
        return sign<T>(x) < 0 ? invert_sign<T>( x ) : x;
    }

    template <typename T,
              std::enable_if_t< std::is_arithmetic_v<T> &&
                                std::is_unsigned_v<T>, bool> = true>
    constexpr T abs(const T x) noexcept
    {
        return x;
    }
}

TEST_CASE( "Test 01", "[meta][typetraits]" ) {
    REQUIRE(  3 == test_01::abs(-3));
    REQUIRE( -1 == test_01::sign(-3));
    REQUIRE(  0 == test_01::sign(0));
    REQUIRE(  1 == test_01::sign(3));
    {
       bool a = true;
       REQUIRE( 1 == test_01::sign(a));
    }
}
