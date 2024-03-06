//============================================================================
// Author      : Sven Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 1.1 Memory (lifecycle)
//============================================================================

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cassert>

#include <string>
#include <memory>
#include <atomic>
#include <iostream>

/**
 * Lesson 1.1
 *
 * Memory occupying object lifecycle:
 * - Construction (default, copy, move)
 * - Assigning (copy, move)
 * - Referencing
 * - Destruction.
 */

class sint_t; // forward

std::ostream& operator<<(std::ostream& out, const sint_t& v);

class sint_t {
    private:
        static std::atomic_int last_id;
        int id;
        int64_t store;

    public:
        /** Default constructor (ctor) */
        sint_t() noexcept
        : id(++last_id), store(0) {
            std::cout << "ctor.norm: " << *this << std::endl;
        }

        /** Copy ctor */
        sint_t(const sint_t& o) noexcept
        : id(++last_id), store(o.store) {
            std::cout << "ctor.copy: " << o << " -> " << *this << std::endl;
        }

        /** Move ctor */
        sint_t(sint_t&& o) noexcept
        : id( std::move(o.id) ), store( std::move(o.store) ) {
            std::cout << "ctor.move: " << o << " -> " << *this << ", old ";
            o.id = 0;
            std::cout << o << std::endl;
        }

        /** Destructor (dtor) */
        ~sint_t() noexcept {
            std::cout << "ctor.dtor: " << *this << std::endl;
            id = 0;
        }

        /** Explicit conversion ctor: int64_t -> my_int_t */
        explicit sint_t(const int64_t& o) noexcept
        : id(++last_id), store( o ) {
            std::cout << "ctor.conv: " << std::to_string( o ) << " -> " << *this << std::endl;
        }

        //
        // Assignment operations, including arithmetic assignments
        //

        /** Assignment */
        sint_t& operator=(const sint_t& r) noexcept {
            if( this != &r ) {
                store = r.store;
                std::cout << "assign.copy: " << r << " -> " << *this << std::endl;
            } else {
                std::cout << "assign.self: " << *this << std::endl;
            }
            return *this;
        }

        /** Move-Assignment */
        sint_t& operator=(sint_t&& r) noexcept {
            if( this != &r ) {
                std::cout << "assign.move: " << r << " -> ";
                id = r.id;
                store = r.store;
                r.id = 0;
                std::cout << *this << ", old " << r << std::endl;
            } else {
                std::cout << "assign.self: " << *this << std::endl;
            }
            return *this;
        }

        //
        // Misc operations
        //

        sint_t& set(int64_t v) noexcept { store = v; return *this; }
        int64_t get() const noexcept { return store; }

        std::string to_string() const noexcept { return "[id "+std::to_string(id)+", val "+std::to_string(store)+"]"; }
};
std::atomic_int sint_t::last_id(0);

inline std::ostream& operator<<(std::ostream& out, const sint_t& v) {
    return out << v.to_string();
}

sint_t make_obj(int64_t v) { return sint_t(v); }
const sint_t& filter10(const sint_t& o) { return o; }
sint_t  filter11(const sint_t& o) { return o; }

sint_t& filter20(sint_t& o) { o.set(o.get()+1); return o; }
sint_t  filter21(sint_t& o) { o.set(o.get()+1); return o; }

int main(int, const char* []) {
    std::cout << "Block 1" << std::endl;
    {
        sint_t a;
        sint_t c(a);
        sint_t d = a;
        (void)a; (void)c; (void)d;

        // ctor.norm: [id 1, val 0]
        // ctor.copy: [id 1, val 0] -> [id 2, val 0]
        // ctor.copy: [id 1, val 0] -> [id 3, val 0]
        // ctor.dtor: [id 3, val 0]
        // ctor.dtor: [id 2, val 0]
        // ctor.dtor: [id 1, val 0]
    }
    std::cout << "Block 2" << std::endl;
    {
        const sint_t& a = make_obj(1);
        std::cout << "b2.a: " << a << std::endl;
        const sint_t& b = filter10(a);
        std::cout << "b2.b: " << b << std::endl;
        (void)a; (void)b;

        // ctor.conv: 1 -> [id 4, val 1]
        // b2.a: [id 4, val 1]
        // b2.b: [id 4, val 1]
        // ctor.dtor: [id 4, val 1]
    }
    std::cout << "Block 3" << std::endl;
    {
        sint_t a = make_obj(1);
        std::cout << "b3.a: " << a << std::endl;
        sint_t b = std::move( make_obj(2) );
        std::cout << "b3.b: " << b << std::endl;

        // ctor.conv: 1 -> [id 5, val 1]
        // b3.a: [id 5, val 1]
        // ctor.conv: 2 -> [id 6, val 2]
        // ctor.move: [id 6, val 2] -> [id 6, val 2], old [id 0, val 2]
        // ctor.dtor: [id 0, val 2]
    }
    std::cout << "Block 4" << std::endl;
    {
        sint_t a, b(2);
        std::cout << "b4.0.a: " << a << std::endl;
        std::cout << "b4.0.b: " << b << std::endl;
        a = std::move( b ); // b should not be dereferenced after move
        std::cout << "b4.1.a: " << a << std::endl;
        a = std::move( make_obj(3) );
        std::cout << "b4.2.a: " << a << std::endl;

        // ctor.norm: [id 7, val 0]
        // ctor.conv: 2 -> [id 8, val 2]
        // b4.0.a: [id 7, val 0]
        // b4.0.b: [id 8, val 2]
        // assign.move: [id 8, val 2] ->  [id 8, val 2], old [id 0, val 2]
        // b4.1.a: [id 8, val 2]
        // ctor.conv: 3 -> [id 9, val 3]
        // assign.move: [id 9, val 3] ->  [id 9, val 3], old [id 0, val 3]
        // ctor.dtor: [id 0, val 3]
        // b4.2.a: [id 9, val 3]
        // ctor.dtor: [id 0, val 2]
        // ctor.dtor: [id 9, val 3]
    }
    return 0;
}
