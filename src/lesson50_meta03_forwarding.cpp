//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson
//===============================================================================

#include <cstring>
#include <functional>
#include <type_traits>

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
    inline std::unique_ptr<T> make_unique1(U &&u) {
        return std::unique_ptr<T>(new T(std::forward<U>(u)));
    }

    template<class... U>
    inline B make_B1(U &&...u) {
        return B(std::forward<U>(u)...);
    }

    static auto make_B2(auto &&...args)  // since C++20
    {
        return B(std::forward<decltype(args)>(args)...);
    }

}  // namespace test_01

TEST_CASE("Test 01", "[meta][forward]") {
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

namespace test_02 {

    class TrivialObj {
      private:
        int m_i1;
        int m_i2;

      public:
        TrivialObj()
        : m_i1(1), m_i2(2)
        {}

        int i1() const noexcept { return m_i1; }
        int i2() const noexcept { return m_i2; }
    };
    typedef std::shared_ptr<TrivialObj> TrivialObjSRef;

    class NonTrivialObj {
      private:
        int m_i1;
        int m_i2;
        void *m_mem;

      public:
        NonTrivialObj()
        : m_i1(1), m_i2(2), m_mem(::malloc(1024))
        {}

        ~NonTrivialObj() {
            if(m_mem) {
                ::free(m_mem);
                m_mem = nullptr;
            }
        }

        NonTrivialObj(const NonTrivialObj& o)
        : m_i1(o.m_i1), m_i2(o.m_i2), m_mem(::malloc(1024)) {
            ::memcpy(m_mem, o.m_mem, 1024);
        }
        NonTrivialObj(NonTrivialObj&& o) noexcept
        : m_i1(o.m_i1), m_i2(o.m_i2), m_mem(o.m_mem) {
            o.m_mem = nullptr;
        }

        int i1() const noexcept { return m_i1; }
        int i2() const noexcept { return m_i2; }
        void* handle() const noexcept { return m_mem; }
    };
    typedef std::shared_ptr<NonTrivialObj> NonTrivialObjSRef;

    template<typename Signature>
    class Invocation;

    template<typename R, typename... A>
    class Invocation<R(A...)> {
      private:
        R m_r;

        R impl(A &&...) const {
            return m_r;
        }
        R impl(A &&...) {
            return m_r;
        }
      public:
        Invocation(R r) noexcept
        : m_r(r) {}

        // lambda ...
        template<typename L>
        Invocation(L) noexcept
        : m_r(0) {}

        R operator()(A ...args) const {
            if constexpr (std::is_void_v<R>)
                impl(std::forward<A>(args)...);
            else
                return impl(std::forward<A>(args)...);
        }
        R operator()(A ...args) {
            if constexpr (std::is_void_v<R>)
                impl(std::forward<A>(args)...);
            else
                return impl(std::forward<A>(args)...);
        }

    };

    typedef Invocation<bool(const TrivialObj &, const char *file, int line)> handler_t;

    inline __attribute__((always_inline))
    bool handle(handler_t &eh, const TrivialObj &e, const char *file, int line) noexcept {
        return eh(e, file, line);
    }

}  // namespace test_02

TEST_CASE("Test 02", "[meta][forward]") {
    using namespace test_02;
    {
        handler_t h = (handler_t) [](const TrivialObj &, char *, int) -> bool {
            return true;
        };
        TrivialObj o;
        const char * t = "hello";
        handle(h, o, t, 1);
    }
    {
        Invocation<int(int, int, int)> f(1);
        REQUIRE( 1 == f(1, 2, 3));
    }
    {
        const int i = 1;
        Invocation<int(int, int, int)> f(1);
        REQUIRE( 1 == f(i, 2, 3));
    }
    {
        TrivialObj o;
        Invocation<int(const TrivialObj&, int, int)> f(1);
        REQUIRE( 1 == f(o, 2, 3));
    }
    {
        NonTrivialObj o;
        Invocation<int(const NonTrivialObj&, int, int)> f(1);
        REQUIRE( 1 == f(o, 2, 3));
    }
    {
        NonTrivialObj o;
        Invocation<int(NonTrivialObj, int, int)> f(1);
        REQUIRE( 1 == f(o, 2, 3));
    }
}
