//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson
//===============================================================================

#include <concepts>
#include <type_traits>

#include <jau/test/catch2_ext.hpp>

namespace test_02 {
    //
    // C++20 conform template/meta functions using concepts and type traits (SFINAE)
    //

    /** Concept of type-trait std::is_arithmetic */
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    /** Concept of type-trait std::is_unsigned and std::is_arithmetic */
    template <typename T>
    concept unsigned_arithmetic = std::is_arithmetic_v<T> && std::is_unsigned_v<T>;

    /** Concept of type-trait std::is_signed and std::is_arithmetic */
    template <typename T>
    concept signed_arithmetic = std::is_arithmetic_v<T> && std::is_signed_v<T>;

    /** Query whether type is an arithmetic type */
    template<typename T>
    constexpr bool is_arithmetic() { return std::is_arithmetic_v<T>; }

    /** Query whether type is a signed arithmetic type */
    template<typename T>
    constexpr bool is_signed_arithmetic() { return std::is_arithmetic_v<T> && std::is_signed_v<T>; }

    /** Query whether type is an unsigned arithmetic type */
    template<typename T>
    constexpr bool is_unsigned_arithmetic() { return std::is_arithmetic_v<T> && std::is_unsigned_v<T>; }

    /** Query whether type is an integral type */
    template<typename T>
    constexpr bool is_integral() { return std::is_integral_v<T>; }

    /** Query whether type is a signed integral type */
    template<typename T>
    constexpr bool is_signed_integral() { return std::is_integral_v<T> && std::is_signed_v<T>; }

    /** Query whether type is an unsigned integral type */
    template<typename T>
    constexpr bool is_unsigned_integral() { return std::is_integral_v<T> && std::is_unsigned_v<T>; }

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
    template <typename T>
        requires std::signed_integral<T>
    constexpr int sign(const T x) noexcept
    {
        return (int) ( (T(0) < x) - (x < T(0)) );
    }
    template <typename T>
        requires std::unsigned_integral<T>
    constexpr int sign(const T x) noexcept
    {
        return (int) ( T(0) < x );
    }

    template <typename T>
        requires signed_arithmetic<T>
    constexpr T invert_sign(const T x) noexcept
    {
        return std::numeric_limits<T>::min() == x ? std::numeric_limits<T>::max() : -x;
    }

    template <typename T>
        requires unsigned_arithmetic<T>
    constexpr T invert_sign(const T x) noexcept
    {
        return x;
    }

    template <typename T>
        requires signed_arithmetic<T>
    constexpr T abs(const T x) noexcept
    {
        return sign<T>(x) < 0 ? invert_sign<T>( x ) : x;
    }

    template <typename T>
        requires unsigned_arithmetic<T>
    constexpr T abs(const T x) noexcept
    {
        return x;
    }
}

TEST_CASE( "Test 03", "[meta][concepts]" ) {
    static_assert(true  == test_02::is_integral<bool>());
    static_assert(false == test_02::is_signed_integral<bool>());
    static_assert(true  == test_02::is_unsigned_integral<bool>());
    static_assert(true  == test_02::is_arithmetic<bool>());
    static_assert(false == test_02::is_signed_arithmetic<bool>());
    static_assert(true  == test_02::is_unsigned_arithmetic<bool>());

    static_assert(true  == test_02::is_integral<char>());
    static_assert(true  == test_02::is_signed_integral<char>());
    static_assert(false == test_02::is_unsigned_integral<char>());
    static_assert(true  == test_02::is_arithmetic<char>());
    static_assert(true  == test_02::is_signed_arithmetic<char>());
    static_assert(false == test_02::is_unsigned_arithmetic<char>());

    static_assert(true  == test_02::is_integral<unsigned char>());
    static_assert(false == test_02::is_signed_integral<unsigned char>());
    static_assert(true  == test_02::is_unsigned_integral<unsigned char>());
    static_assert(true  == test_02::is_arithmetic<unsigned char>());
    static_assert(false == test_02::is_signed_arithmetic<unsigned char>());
    static_assert(true  == test_02::is_unsigned_arithmetic<unsigned char>());

    static_assert(true  == test_02::is_integral<int>());
    static_assert(true  == test_02::is_signed_integral<int>());
    static_assert(false == test_02::is_unsigned_integral<int>());
    static_assert(true  == test_02::is_arithmetic<int>());
    static_assert(true  == test_02::is_signed_arithmetic<int>());
    static_assert(false == test_02::is_unsigned_arithmetic<int>());

    static_assert(true  == test_02::is_integral<unsigned int>());
    static_assert(false == test_02::is_signed_integral<unsigned int>());
    static_assert(true  == test_02::is_unsigned_integral<unsigned int>());
    static_assert(true  == test_02::is_arithmetic<unsigned int>());
    static_assert(false == test_02::is_signed_arithmetic<unsigned int>());
    static_assert(true  == test_02::is_unsigned_arithmetic<unsigned int>());

    REQUIRE(  3 == test_02::abs(-3));
    REQUIRE( -1 == test_02::sign(-3));
    REQUIRE(  0 == test_02::sign(0));
    REQUIRE(  1 == test_02::sign(3));
    {
       bool a = true;
       REQUIRE( 1 == test_02::sign(a));
    }
}
