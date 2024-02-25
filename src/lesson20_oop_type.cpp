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
        /** Default constructor (ctor) */
        sint_t() noexcept
        : store(0) { }

        /** Copy ctor */
        sint_t(const sint_t& o) noexcept
        : store(o.store) { }

        /** Destructor (dtor) */
        ~sint_t() noexcept {}

        /** Explicit conversion ctor: int64_t -> my_int_t */
        explicit sint_t(const int64_t& o) noexcept
        : store( o ) { }

        //
        // Logical operations
        //

        bool is_zero() const noexcept { return 0 == store; }

        /** Explicit bool operator, returns `true` iff this is not zero, otherwise false. */
        explicit operator bool() const noexcept { return !is_zero(); }

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
        bool operator==(const sint_t& b) {
            return store == b.store;
        }

        /** Three way std::strong_ordering comparison operator */
        std::strong_ordering operator<=>(const sint_t& b) {
            return store == b.store ? std::strong_ordering::equal : ( store < b.store ? std::strong_ordering::less : std::strong_ordering::greater);
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

        /** Post-increment unary operation */
        sint_t  operator++(int) noexcept { sint_t x(*this); ++(*this); return x; }

        /** Post-decrement unary operation */
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

int main(int, char*[]) {
    sint_t zero(0), ten(10), twenty(20), thirty(30), one(1), two(2);
    std::cout << "zero = " << zero << std::endl;
    std::cout << "ten = " << ten << std::endl;
    std::cout << "twenty = " << twenty << std::endl;
    std::cout << "thirty = " << thirty << std::endl;

    //
    // Logical operations
    //
    assert( !zero );
    assert( ten );
    assert( twenty );
    assert( thirty );

    //
    // Comparison operations
    //
    assert( ten    <  twenty );
    assert( ten    <= ten );
    assert( ten    <= twenty );
    assert( ten    == ten );
    assert( twenty >  ten );
    assert( twenty >= ten );
    assert( twenty >= twenty );

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
        assert( eleven  == ++t );
        assert( eleven  == t );
    }
    {
        sint_t t(ten);
        assert( nine    == --t );
        assert( nine    == t );
    }
    {
        sint_t t(ten);
        assert( ten     == t++ );
        assert( eleven  == t );
    }
    {
        sint_t t(ten);
        assert( ten     == t-- );
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
