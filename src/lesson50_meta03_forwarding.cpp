//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson
//===============================================================================

#include <jau/test/catch2_ext.hpp>

namespace test_01 {
    //
    // C++11 conform type forwarding
    //

    enum class ValueType {
        rvalue,
        lvalue
    };

    struct A {
        ValueType valueType;
        int value;

        A(int &&n) 
        : valueType(ValueType::rvalue), value(std::forward<int>(n)) {} // fwd

        A(int &n) 
        : valueType(ValueType::lvalue), value(n) {} // copy
    };

    struct B {
        A a1, a2, a3;
        
        template<class T1, class T2, class T3>
        B(T1 &&t1, T2 &&t2, T3 &&t3) 
        : a1{ std::forward<T1>(t1) },
          a2{ std::forward<T2>(t2) },
          a3{ std::forward<T3>(t3) } { }
    };

    template<class T, class U>
    static std::unique_ptr<T> make_unique1(U &&u) {
        return std::unique_ptr<T>(new T(std::forward<U>(u)));
    }

    template<class... U>
    static B make_B1(U &&...u) {
        return B(std::forward<U>(u)...);
    }

    static auto make_B2(auto &&...args)  // since C++20
    {
        return B(std::forward<decltype(args)>(args)...);
    }

}  // namespace test_01

TEST_CASE("Test 03", "[meta][concepts]") {
    using namespace test_01;
    int i = 1;
    {
        A a1(2), a2(i);
        REQUIRE(ValueType::rvalue == a1.valueType);
        REQUIRE(ValueType::lvalue == a2.valueType);
    }
    {
        auto p1 = make_unique1<A>(2);  // rvalue
        REQUIRE(ValueType::rvalue == p1->valueType);
        auto p2 = make_unique1<A>(i);  // lvalue
        REQUIRE(ValueType::lvalue == p2->valueType);
    }
    {
        B b = make_B1(2, i, 3);
        REQUIRE(ValueType::rvalue == b.a1.valueType);
        REQUIRE(ValueType::lvalue == b.a2.valueType);
        REQUIRE(ValueType::rvalue == b.a3.valueType);
    }
    {
        B b = make_B2(4, i, 5);
        REQUIRE(ValueType::rvalue == b.a1.valueType);
        REQUIRE(ValueType::lvalue == b.a2.valueType);
        REQUIRE(ValueType::rvalue == b.a3.valueType);
    }
}
