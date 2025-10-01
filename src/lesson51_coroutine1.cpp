//============================================================================
// Author      : Sven Göthel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson
//===============================================================================

// #include <type_traits>
// #include <concepts>
#include <coroutine>
// #include <exception>
#include <iostream>

#include <cassert>

//
// https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
//
namespace test_01 {
    struct ReturnObject {
        struct promise_type {
            ~promise_type() {
                std::cout << "promise_type1 destroyed" << std::endl;
            }
            ReturnObject get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void unhandled_exception() { }
        };
    };

    struct Awaiter {
        std::coroutine_handle<> *hp_;
        constexpr bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> h) { *hp_ = h; }
        constexpr void await_resume() const noexcept { }
    };

    ReturnObject counter(std::coroutine_handle<> *continuation_out) {
        Awaiter a{ continuation_out };
        for( unsigned i = 0;; ++i ) {
            co_await a;
            std::cout << "counter: " << i << std::endl;
        }
    }

    void test() {
        std::coroutine_handle<> h;
        counter(&h);
        for( int i = 0; i < 3; ++i ) {
            std::cout << "In main1 function\n";
            h();
        }
        h.destroy();
    }
}  // namespace test_01

namespace test_02 {
    struct ReturnObject {
        struct promise_type {
            ~promise_type() {
                std::cout << "promise_type2 destroyed" << std::endl;
            }
            ReturnObject get_return_object() {
                return {
                    // Uses C++20 designated initializer syntax
                    .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
                };
            }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void unhandled_exception() { }
        };

        std::coroutine_handle<promise_type> h_;
        operator std::coroutine_handle<promise_type>() const { return h_; }
        // A coroutine_handle<promise_type> converts to coroutine_handle<>
        operator std::coroutine_handle<>() const { return h_; }
    };

    ReturnObject counter() {
        for( unsigned i = 0;; ++i ) {
            co_await std::suspend_always{};
            std::cout << "counter2: " << i << std::endl;
        }
    }

    void test() {
        std::coroutine_handle<> h = counter();
        for( int i = 0; i < 3; ++i ) {
            std::cout << "In main2 function\n";
            h();
        }
        h.destroy();
    }
}  // namespace test_02

namespace test_05 {
    struct ReturnObject {
        struct promise_type {
            unsigned value_;

            ~promise_type() {
                std::cout << "promise_type5 destroyed" << std::endl;
            }
            ReturnObject get_return_object() {
                return {
                    .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
                };
            }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void unhandled_exception() { }
            std::suspend_always yield_value(unsigned value) {
                value_ = value;
                return {};
            }
            void return_void() { }
        };

        std::coroutine_handle<promise_type> h_;
    };

    ReturnObject counter() {
        for( unsigned i = 0; i < 3; ++i ) {
            co_yield i; // co yield i => co_await promise.yield_value(i)
        }
        // falling off end of function or co_return; => promise.return_void();
        // (co_return value; => promise.return_value(value);)
    }

    void test() {
        auto h = counter().h_;
        auto &promise = h.promise();
        while( !h.done() ) {  // Do NOT use while(h) (which checks h non-NULL)
            std::cout << "counter5: " << promise.value_ << std::endl;
            h();
        }
        h.destroy();
    }
}  // namespace test_05

namespace test_06 {
    template <typename T>
    struct Generator {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type {
            T value_;
            std::exception_ptr exception_;

            ~promise_type() {
                std::cout << "promise_type6 destroyed" << std::endl;
            }
            Generator get_return_object() {
                return Generator(handle_type::from_promise(*this));
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void unhandled_exception() { exception_ = std::current_exception(); }
            template <std::convertible_to<T> From>  // C++20 concept
            std::suspend_always yield_value(From &&from) {
                value_ = std::forward<From>(from);
                return {};
            }
            void return_void() { }
        };

        handle_type h_;

        Generator(handle_type h): h_(h) { }
        Generator(const Generator &) = delete;
        ~Generator() { h_.destroy(); }
        explicit operator bool() {
            fill();
            return !h_.done();
        }
        T operator()() {
            fill();
            full_ = false;
            return std::move(h_.promise().value_);
        }

      private:
        bool full_ = false;

        void fill() {
            if( !full_ ) {
                h_();
                if( h_.promise().exception_ )
                    std::rethrow_exception(h_.promise().exception_);
                full_ = true;
            }
        }
    };

    Generator<unsigned> counter() {
        for( unsigned i = 0; i < 3; ) {
            co_yield i++; // co yield i => co_await promise.yield_value(i)
        }
        // falling off end of function or co_return; => promise.return_void();
        // (co_return value; => promise.return_value(value);)
    }

    void test() {
        auto gen = counter();
        while( gen )
            std::cout << "counter6: " << gen() << std::endl;
    }
}

int main() {
    test_01::test();
    test_02::test();
    test_05::test();

    test_06::test();
    return 0;
}
