//============================================================================
// Author      : Sven Göthel
// Copyright   : 2024 Göthel Software e.K.
// License     : MIT
// Description : C++ Lesson 2.0 OOP (integer class with operation overloading)
//============================================================================

#include <limits>
#include <string>
#include <memory>
#include <cassert>
#include <iostream>

class sint_t {
    private:
        int64_t store;

    public:
        // debug facility
        mutable int cntr_op_equal = 0;
        mutable int cntr_op_spaceship = 0;
        void reset_counter() const noexcept {
            cntr_op_equal = 0;
            cntr_op_spaceship = 0;
        }

        /** Default constructor (ctor) */
        sint_t() noexcept
        : store(0) { }

        /** Copy ctor */
        sint_t(const sint_t& o) noexcept
        : store(o.store) {}

        /** Destructor (dtor) */
        ~sint_t() noexcept = default;

        /** Explicit conversion ctor: int64_t -> my_int_t */
        explicit sint_t(const int64_t& o) noexcept
        : store(o) {}

        //
        // Logical operations
        //

        bool is_zero() const noexcept { return 0 == store; }

        /**
         * Explicit bool conversion operator using target-type-name as method name.
         *
         * Returns `true` iff this is not zero, otherwise false.
         */
        explicit operator bool() const noexcept { return !is_zero(); }

         // operator int() const noexcept { return store; }

        /** Not-bool operator, returns `true` iff this is zero, otherwise false. */
        bool operator !() const noexcept { return is_zero(); }

        //
        // Comparison operations
        //

        /** Compare function returns 0 if equal, -1 if *this < b and 1 if *this > b. */
        int compare(const sint_t& b) const noexcept {
            return store == b.store ? 0 : ( store < b.store ? -1 : 1);
        }

        /** Two way comparison operator */
        bool operator==(const sint_t& b) const noexcept {
            ++cntr_op_equal;
            return store == b.store;
        }

        /** Three way std::strong_ordering comparison operator */
        std::strong_ordering operator<=>(const sint_t& b) const noexcept {
            ++cntr_op_spaceship;
            return store == b.store ? std::strong_ordering::equal :
                                      ( store < b.store ? std::strong_ordering::less : std::strong_ordering::greater );
        }

        //
        // Assignment operations, including arithmetic assignments
        //

        /** Assignment */
        sint_t& operator=(const sint_t& r) noexcept {
            if( this != &r ) {
                store = r.store;
            }
            return *this;
        }

        /** Addition assignment */
        sint_t& operator+=(const sint_t& rhs ) noexcept {
            store += rhs.store;
            return *this;
        }

        /** Subtraction assignment */
        sint_t& operator-=(const sint_t& rhs ) noexcept {
            store -= rhs.store;
            return *this;
        }

        /** Multiplication assignment */
        sint_t& operator*=(const sint_t& rhs ) noexcept {
            store *= rhs.store;
            return *this;
        }

        /** Division assignment */
        sint_t& operator/=(const sint_t& rhs ) noexcept {
            store /= rhs.store;
            return *this;
        }

        /** Remainder assignment */
        sint_t& operator%=(const sint_t& rhs ) noexcept {
            store %= rhs.store;
            return *this;
        }

        //
        // Unary arithmetic operations
        //

        /** Returns the signum, i.e. `-1` if negative, `0` if zero and `+1` if positive */
        int signum() const noexcept { return 0 == store ? 0 : ( 0 > store ? -1 : +1 ); }

        /** Returns `true` if positive, otherwise `false`. */
        bool positive() const noexcept { return 0 <= store; }

        /**
        * Set signum of this integer
        * @param new_signum new sign() to set, ignored if this instance is_zero()
        */
        void set_sign(const int new_signum) noexcept {
           if( !is_zero() && new_signum != 0 ) {
               store = std::abs(store) * new_signum;
           }
        }

        /** Returns opposite of positive(), ignoring signum() `0` */
        int reversed_sign() const noexcept {
            if( positive() ) {
                return -1;
            } else {
                return 1;
            }
        }

        /** Flips the sign of this instance, returns *this */
        sint_t& flip_sign() noexcept {
           set_sign(reversed_sign());
           return *this;
        }

        /** Unary positive operator, returns new instance of this. */
        sint_t operator+() const noexcept { return sint_t(*this); }

        /** Unary negation operator, returns new negative instance of this. */
        sint_t operator-() const noexcept { return sint_t(*this).flip_sign(); }

        /** Pre-increment unary operation */
        sint_t& operator++() noexcept { ++store; return *this; }

        /** Pre-decrement unary operation */
        sint_t& operator--() noexcept { --store; return *this; }

        /** Post-increment unary operation, more expensive than pre counterpart. */
        sint_t  operator++(int) noexcept { sint_t x(*this); ++(*this); return x; }

        /** Post-decrement unary operation, more expensive than pre counterpart. */
        sint_t  operator--(int) noexcept { sint_t x(*this); --(*this); return x; }

        //
        // Misc operations
        //

        std::string to_string() const noexcept { return std::to_string(store); }
};

sint_t operator+(const sint_t& lhs, const sint_t& rhs ) noexcept {
    sint_t r(lhs);
    r += rhs;
    return r;
}

sint_t operator-(const sint_t& lhs, const sint_t& rhs ) noexcept {
    return sint_t(lhs) -= rhs;
}

sint_t operator*(const sint_t& lhs, const sint_t& rhs ) noexcept {
    return sint_t(lhs) *= rhs;
}

sint_t operator/(const sint_t& lhs, const sint_t& rhs ) noexcept {
    return sint_t(lhs) /= rhs;
}

std::ostream& operator<<(std::ostream& out, const sint_t& v) {
    return out << v.to_string();
}

// DANGER: Implicit conversion bool -> int with overloaded functions
int lala(int a, int b) { return a + b; }
int lala(int a, bool b) { if(b) { return a; } else { return -a; } }

// Safe: 'enum class' has no implicit conversion
enum class bool_t : int {
    f,
    t
};
int lala(int a, bool_t b) { if(bool_t::t == b) { return a; } else { return -a; } }

int main(int, char*[]) {
    const sint_t zero, ten(10), twenty(20), thirty(30), one(1), two(2);
    std::cout << "zero = " << zero << std::endl;
    std::cout << "ten = " << ten << std::endl;
    std::cout << "twenty = " << twenty << std::endl;
    std::cout << "thirty = " << thirty << std::endl;

    // implicit vs explicit conversion
    {
        // sint_t lala = 4; // Error, requires implicit conversion but only explicit is allowed
        sint_t lala1 = sint_t(4);
        sint_t lala2 = static_cast<sint_t>(4);
        (void)lala1;
        (void)lala2;
    }
    {
        const sint_t a(2), b(3);
        const sint_t c = a + b;
        (void)c;
        // a += b; // error: passing ‘const sint_t’ as ‘this’ argument discards qualifiers
    }
    if( ten ) {
        // bool b1 = ten; // error: cannot convert ‘sint_t’ to ‘bool’ in initialization
    }

    //
    // Logical operation, using conversion operator `explicit operator bool() ...`
    //
    assert( !zero );
    assert( ten );
    assert( twenty );
    assert( thirty );

    // Implicit conversion of bool -> int is INSECURE!
    {
        int i1 = 1;
        bool b1 = true; // implied 1
        int i2 = i1 + b1; // default implicit conversion bool -> int
        std::cout << "i2.a: " << std::to_string(i1) << " + "
                  << std::to_string(b1) << " = "
                  << std::to_string(i2) << std::endl;
        // Wrong result if
        // i2 = i1 + ten; // operation bool() -> implicit -> int
        std::cout << "i2.b: " << std::to_string(i1) << " + " << ten << " = " << std::to_string(i2) << std::endl;
    }
    // potential conversion issues with overloaded function
    {
        assert(3 == lala(1, 2));
        assert(1 == lala(1, true));
        assert(1 == lala(1, bool_t::t));
    }

    //
    // Comparison operations
    //
    ten.reset_counter();
    twenty.reset_counter();
    assert( ten    == ten );    // operator==
    assert( ten    != twenty ); // operator==
    assert( 2 == ten.cntr_op_equal );
    assert( ten    <  twenty ); // operator<=>
    assert( ten    <= ten );    // operator<=>
    assert( ten    <= twenty ); // operator<=>
    assert( 2 == ten.cntr_op_equal );
    assert( 3 == ten.cntr_op_spaceship );

    assert( twenty >  ten );    // operator<=>
    assert( twenty >= ten );    // operator<=>
    assert( twenty >= twenty ); // operator<=>
    assert( 2 == ten.cntr_op_equal );
    assert( 3 == ten.cntr_op_spaceship );
    assert( 0 == twenty.cntr_op_equal );
    assert( 3 == twenty.cntr_op_spaceship );

    //
    // Assignment operations, including arithmetic assignments
    //
    {
        sint_t t; t = ten;
        assert( t == ten );
    }
    {
        sint_t t(ten); t += twenty;
        assert( t == thirty );
    }
    {
        sint_t t(ten); t += twenty;
        assert( t == thirty );
    }
    {
        sint_t t(ten); t *= two;
        assert( t == twenty );
    }
    {
        sint_t t(twenty); t /= two;
        assert( t == ten );
    }
    {
        sint_t t(thirty); t %= twenty;
        assert( t == ten );
    }

    //
    // Unary arithmetic operations
    //
    sint_t minus_ten(-10), nine(9), eleven(11);
    assert( ten       == +ten );
    assert( minus_ten == -ten );
    assert( ten       == -minus_ten );

    {
        sint_t t(ten);
        assert( eleven  == ++t ); // increments and returns *this; NOLINT(bugprone-assert-side-effect) intended
        assert( eleven  == t );
    }
    {
        sint_t t(ten);
        assert( nine    == --t ); // NOLINT(bugprone-assert-side-effect) intended
        assert( nine    == t );
    }
    {
        sint_t t(ten);
        assert( ten     == t++ ); // copies *this, increments and returns copy; NOLINT(bugprone-assert-side-effect) intended
        assert( eleven  == t );
    }
    {
        sint_t t(ten);
        assert( ten     == t-- ); // NOLINT(bugprone-assert-side-effect) intended
        assert( nine    == t );
    }

    //
    // Binary arithmetic operations
    //
    assert( one + ten == eleven );
    assert( eleven - one == ten );

    assert( two * ten == twenty );
    assert( twenty / two == ten );

    return 0;
}
