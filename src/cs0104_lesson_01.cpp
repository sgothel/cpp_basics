//============================================================================
// Author      : Sven Gothel
// Copyright   : 2020-2024 Gothel Software e.K.
// License     : MIT
// Description : CS 01.04 C++ 01
//============================================================================

#include <cassert>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <limits>

#include <iostream>
#include <sstream>

#include <jau/test/catch2_ext.hpp>

/**
 * Expressions used in Statements to initialize variables of primitive type.
 * - Variablen mit primitiven Datentypen
 * - Literale (Konstanten)
 * - Operatoren `=`, `+` fuer `int` Werte
 * - Testen der Variablen Inhalte
 */
TEST_CASE( "Test 01", "[basic]" ) {
    /**
     * ℤ Ganzzahlen -> Integer Variable, primitiver datentyp `int`
     * - Hier mit Variablenname `i`
     * - Hier besetzt die `int` Variable 32-bit oder 4 Byte Speicher
     *
     * Genutzte Literale (Konstante)
     * - `1` Wert 1 als `int`
     * - `2` Wert 2 als `int`
     *
     * Genutzte Operatoren
     * - `=` wert-zuweisung, assignment
     * - `+` addition
     */
    int i = 1; // ℤ ganzzahl, variable (veraenderbar, mutable), initialisiert mit dem Wert `1` (literal)
    i = i + 1; // Erhoehe den Inhalt der Variable `i` um `1` (literal).
    REQUIRE(2 == i); // Test das variable `i` den Inhalt `2` (literal) hat.

    /**
     * ℤ Ganzzahlen -> Integer Variable, primitiver datentyp `int`
     * - Hier mit Variablenname `j`
     * - ...
     */
    int j; // Undefiniert, nicht initialisiert.
    j = 1; // Weise der Variable den Wert `1` (literal) zu.
    j = j + 1; // Erhoehe den Wert der Variable `j` um `1` (literal)
    REQUIRE(2 == j); // Test das variable `j` den Inhalt `2` (literal) hat.
}

/**
 * Block-Statements und Lebensbereich lokaler Variablen
 */
TEST_CASE( "Test 02.3 Block Statement", "[basic]" ) {
    // Block-1 Anfang
    {
        const int i = 1; // Neue variable `i` mit dem Wert `1` initialisiert
        REQUIRE(1 == i);

        // Block-2 Anfang
        {
            const int j = 2; // Neue variable `j` mit dem Wert `2` initialisiert
            const int k = i + j; // Neue variable `k` mit `i+j` initialisiert
            REQUIRE(3 == k);
        }
        // Block-2 Ende
        // Variablen `j` und `k` existieren nicht mehr!
    }
    // Block-1 Ende
    // Variable `i` existiert nicht mehr!
    // i = i + 1; FEHLER, `i` gibts nicht mehr

    // Neue verschachtelte Bloecke mit neuen Variablen & Lebensbereich
    {
        const int i = 2;
        {
            const int j = 3;
            const int k = i + j;
            REQUIRE(5 == k);
        }
    }
} // Block-0 Ende

/**
 * Primitive Datentypen und Literale (Konstante)
 */
TEST_CASE( "Test 03.1.1 Literals", "[basic]" ) {
    const bool b0 = false; // boolean-value literal
    const uint8_t o0 = 0b0010;  // unsigned bit-value literal -> decimal 2
    const char c0 = 'C';        // signed 8-bit character value literal
    const char c1 = '0';        // signed 8-bit character value literal
    const short s0 = 0x0A;      // hex-value literal -> decimal 10
    const int i0 = 100;         // int-value literal (default)
    const long l0 = 1000L;      // long-value literal
    const float f0 = 3.14f;     // float-value literal
    const double d0 = 2.14;     // double--value literal (default)

    REQUIRE(false == b0);
    REQUIRE((uint8_t)2 == o0);
    REQUIRE('C' == c0);
    REQUIRE(0x30 == c1);
    REQUIRE(10 == s0);
    REQUIRE(100 == i0);
    REQUIRE(1000 == l0);
    REQUIRE(std::abs(3.14f - f0) <= std::numeric_limits<float>::epsilon()); // Test value mit erlaubter Fehler-Toleranz std::numeric_limits<float>::epsilon()
    REQUIRE(std::abs(2.14 - d0) <= std::numeric_limits<double>::epsilon()); // Test value mit erlaubter Fehler-Toleranz std::numeric_limits<double>::epsilon()
}

/**
 * Binary-Operatoren der 4 Grundrechenarten und Modulo (Divisionsrest) anhand des primitiven Datentyps `int`
 */
TEST_CASE( "Test 03.1.2.a Grundrechenarten", "[basic][math]" ) {
    // Addition
    {
        {
            static_assert(3 == 1+2);
            REQUIRE(3 == 1+2); // NOLINT (intentional)
        }
        {
            const int i = 6; // positiver Wert
            const int j = 2; // positiver Wert
            const int k = i + j;
            REQUIRE(8 == k);
        }
        {
            const int i = +6; // positiver Wert
            const int j = -2; // negativer Wert!!
            const int k = i + j;
            REQUIRE(4 == k);
        }
    }
    // Subtraktion
    {
        const int i = 6;
        const int j = 2;
        const int k = i - j;
        REQUIRE(4 == k);
    }
    // Multiplikation
    {
        const int i = 6;
        const int j = 2;
        const int k = i * j;
        REQUIRE(12 == k);
    }
    // Division
    {
        {
            const int i = 6;
            const int j = 2;
            const int k = i / j;
            REQUIRE(3 == k);
        }
        {
            const int i = 7;
            const int j = 2;
            const int k = i / j; // as real number: 7/2=3.5, but integer simply cuts off the floating point
            REQUIRE(3 == k);
        }
    }
    // Modulo (Divisionsrest)
    {
        {
            const int i = 6;
            const int j = 2;
            const int k = i % j;
            const int l = i - ( i / j ) * j; // Modulo definition, i.e. Divisionsrest
            REQUIRE(l == k);
            REQUIRE(0 == k);
        }
        {
            const int i = 7;
            const int j = 2;
            const int k = i % j;
            const int l = i - ( i / j ) * j; // Modulo definition, i.e. Divisionsrest
            REQUIRE(l == k);
            REQUIRE(1 == k);
        }
    }
}

/**
 * Implement pre- and post-increment and -decrement.
 *
 * Also address call-by-value and call-by-reference semantics.
 */

/**
 * Non-functional pre-increment using call-by-value.
 * @param v integer _value_ to increment, copied from caller
 * @return
 */
static int preIncr_call_by_value(int v) {
    return ++v;
}


/**
 * `return ++v`
 *
 * Call-by-value does not work.
 *
 * Call-by-reference works!
 *
 * @param v
 * @return rueckgabewert der pre-incr operation
 */
static int preIncr(int &ref) {
    ref = ref + 1;
    return ref;
}

static int incrPost(int &ref) {
    const int v = ref;
    ref = v + 1;
    return v;
}

static int preDecr(int &ref) {
    ref = ref - 1;
    return ref;
}

/** decrPost variant 1 */
static int decrPost(int &ref) {
    const int v = ref;
    ref = v - 1;
    return v;
}

/**
 * Unary-Operatoren (1 Argument) anhand des primitiven Datentyps `int`
 */
TEST_CASE( "Test 03.1.2.b Unary Post- Prefix", "[basic][math]" ) {
    {
        int i = 1;
        int j = 1;
        const int c1 = ++i; // c1: rueckgabewert der pre-incr operation
        const int c2 = j++; // c2: rueckgabewert der post-incr operation

        REQUIRE( 2 == c1);
        REQUIRE( 1 == c2);
        REQUIRE( 2 == i);
        REQUIRE( 2 == j);
    }
    // Prefix erhoehe (increment) und veringere (decrement)
    {
        int i = 6;
        ++i;
        REQUIRE(7 == i);
        --i;
        REQUIRE(6 == i);

        // !!!!
        REQUIRE(6 == i);   // Inhalt von `i` ist `6`
        REQUIRE(7 == ++i); // Inhalt von `i` wird erhoeht, dann zurueckgegeben! // NOLINT
        REQUIRE(7 == i);   // Selber Wert
    }
    // Demo preIncr_call_by_value() failure
    {
        const int i = 6;
        REQUIRE(7 == preIncr_call_by_value(i)); // OK
        REQUIRE(6 == i); // Nope, not a valid pre-increment implementation!
    }
    {
        int i = 6;
        preIncr(i); // call-by-reference
        REQUIRE(7 == i);
        preDecr(i); // call-by-reference
        REQUIRE(6 == i);

        // !!!!
        REQUIRE(6 == i);   // Inhalt von `i` ist `6`
        REQUIRE(7 == preIncr(i)); // Inhalt von `i` wird erhoeht, dann zurueckgegeben!
        REQUIRE(7 == i);   // Selber Wert
    }

    // Postfix erhoehe (increment) und veringere (decrement)
    {
        int i = 6;
        i++;
        REQUIRE(7 == i);
        i--;
        REQUIRE(6 == i);

        // !!!!
        REQUIRE(6 == i);   // Inhalt von `i` ist `6`
        REQUIRE(6 == i++); // Inhalt von `i` wird zurueckgegeben, dann erhoeht! // NOLINT
        REQUIRE(7 == i);   // Nun ist der Inhalt von `i`erhoeht
    }
    {
        int i = 6;
        preIncr(i);
        REQUIRE(7 == i);
        decrPost(i);
        REQUIRE(6 == i);

        // !!!!
        REQUIRE(6 == i);   // Inhalt von `i` ist `6`
        REQUIRE(6 == incrPost(i)); // Inhalt von `i` wird zurueckgegeben, dann erhoeht!
        REQUIRE(7 == i);   // Nun ist der Inhalt von `i`erhoeht
    }
}

/**
 * Zuweisungs-Operatoren inklusive der 4 Grundrechenarten anhand des primitiven Datentyps `int`
 */
TEST_CASE( "Test 03.1.2.c Zuweisung", "[basic][math]" ) {
    // Einfache Zuweisung
    {
        int i = 6;
        REQUIRE(6 == i);
        i = 7;
        REQUIRE(7 == i);
    }

    // Addition-Zuweisung
    {
        int i = 6;
        i += 4;
        REQUIRE(10 == i);
    }
    // Subtraktion-Zuweisung
    {
        int i = 6;
        i -= 4;
        REQUIRE(2 == i);
    }
    // Multiplikation-Zuweisung
    {
        int i = 6;
        i *= 4;
        REQUIRE(24 == i);
    }
    // Division-Zuweisung
    {
        int i = 6;
        i /= 2;
        REQUIRE(3 == i);
    }
    // Modulo-Zuweisung
    {
        {
            int i = 6;
            i %= 2;
            REQUIRE(0 == i);
        }
        {
            int i = 7;
            i %= 2;
            REQUIRE(1 == i);
        }
    }
}

/**
 * Operatoren der logischen Vergleiche anhand des primitiven Datentyps `int`
 */
TEST_CASE( "Test 03.1.2.d Vergleich", "[basic][math]" ) {
    // Gleichheit (equality)
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        REQUIRE(i == j);

        REQUIRE(i != k);
    }
    // Relational
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        REQUIRE(false == (i <  j));
        REQUIRE(true  == (i <= j));
        REQUIRE(true  == (i >= j));
        REQUIRE(false == (i >  k));

        REQUIRE(true  == (i <  k));
        REQUIRE(true  == (i <= k));
        REQUIRE(false == (i >= k));
        REQUIRE(false == (i >  k));
    }
}

/**
 * Operatoren der logischen Verknuepfung anhand des primitiven Datentyps `int` und `boolean`
 */
TEST_CASE( "Test 03.1.2.e Logisch", "[basic][logic]" ) {
    // Logisch-Und (and) als auch Logisch-Oder (or)
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        const bool b0 = i==j;
        const bool b1 = i==k;

        REQUIRE(true ==  ( b0 && !b1));
        REQUIRE(false == (!b0 ||  b1));

        REQUIRE(true ==  (i == j && i != k));
        REQUIRE(false == (i != j || i == k));
    }
}

/**
 * Integer overflow aware addition returning true if overflow occurred,
 * otherwise false having the result stored in res.
 *
 * Implementation follows API of [GCC Integer Overflow Builtins](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html).
 *
 * @param a operand a
 * @param b operand b
 * @param res storage for result
 * @return true if overflow, otherwise false
 */
static bool add_overflow(const short a, const short b, short &res)
{
    // overflow:  a + b > R+ -> a > R+ - b, with b >= 0
    // underflow: a + b < R- -> a < R- - b, with b < 0

    if ( ( b >= 0 && a > std::numeric_limits<short>::max() - b ) ||
         ( b  < 0 && a < std::numeric_limits<short>::min() - b ) )
    {
        return true;
    } else {
        res = (short)(a + b);
        return false;
    }
}

/**
 * Primitive data types incl. under- and overflow.
 */
TEST_CASE( "Test 03.1.3 Primitive Underflow", "[basic][math]" ) {
    // trigger (ausloesung) short over- and underflow: no practical use-case
    {
        const short one = 1;
        short i = std::numeric_limits<short>::max();
        i = (short)(i + one); // overflow
        REQUIRE(std::numeric_limits<short>::min() == i);

        i = std::numeric_limits<short>::min();
        i = (short)(i - one); // underflow
        REQUIRE(std::numeric_limits<short>::max() == i);
    }
    // trigger short overflow: no practical use-case
    {
        const short a = std::numeric_limits<short>::max() - 10;
        const short b = 11; // a + b -> overflow
        short c;
        {
            // How to make this safe??
            c = (short)(a + b);
        }
        (void) c;
        // REQUIRE(a + b != c); // NOLINT - tautological-constant-out-of-range-compare
    }
    // Safe math using add_overflow()
    // similar to [GCC Integer Overflow Builtins](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html),
    // i.e. detecting under- and overflow for add operation on two integer.
    {
        short res = 0; // one short value
        REQUIRE(false == add_overflow((short) 0,       (short) 0, res));
        REQUIRE((short)0 == res);

        res = 0;
        REQUIRE(false == add_overflow(std::numeric_limits<short>::max(), (short) 0, res));
        REQUIRE(std::numeric_limits<short>::max() == res);

        res = 0;
        REQUIRE(true  == add_overflow(std::numeric_limits<short>::max(), (short) 1, res));
        REQUIRE((short)0 == res);

        res = 0;
        REQUIRE(false == add_overflow(std::numeric_limits<short>::max(), (short)-1, res));
        REQUIRE(std::numeric_limits<short>::max()-1 == res);

        res = 0;
        REQUIRE(false == add_overflow(std::numeric_limits<short>::min(), (short) 1, res));
        REQUIRE(std::numeric_limits<short>::min()+1 == res);

        res = 0;
        REQUIRE(true == add_overflow(std::numeric_limits<short>::min(), (short)-1, res));
        REQUIRE((short)0 == res);

        res = 0;
        REQUIRE(true == add_overflow((short) -1,std::numeric_limits<short>::min(), res));
        REQUIRE((short)0 == res);
    }
}

/**
 * Primitive Datentypen und Literale (Konstante)
 */
TEST_CASE( "Test 03.1.4 Immutable/Const", "[basic]" ) {
    // Unveraenderbare (Immutable) Variablen, i.e. Konstante
    const int const_i0 = 1;
    // const_i0 = const_i0 + 1; // FEHLER
    REQUIRE(1 == const_i0);
}

struct Class03_2_1 {
    int var = 0;

    int erhoeheUm(const int a) {
        var += a;
        return var;
    }
};

TEST_CASE( "Test 03.2.1 OOP: Method Instance", "[basic][oop]" ) {
    Class03_2_1 instance;
    REQUIRE(2 == instance.erhoeheUm(2));
    REQUIRE(2 == instance.var);
    REQUIRE(5 == instance.erhoeheUm(3));
    REQUIRE(5 == instance.var);
}

struct Class03_2_2 {
    static int addiere(const int a, const int b) {
        return a+b;
    }
};

TEST_CASE( "Test 03.2.2 OOP: Method Static", "[basic][oop]" ) {
    REQUIRE(5 == Class03_2_2::addiere(2, 3));
}

/**
 * Manual definition
 */
class Class03_2_3a {
    public:
        // Konstante .. 3.3.2 Globale (Klassen) Variablen (immutable)
        static const int ONE = 1; // global immutable (constant)
        // 3.3.2 Globale (Klassen) Variablen
        static int global_var01; // global static, storage allocation see below post class declaration (explicit initialization)
        static int global_var02; // global static, storage allocation see below post class declaration (default initialization)

        // 3.3.3 Klassen Instanz Variable (Field)
        int field_member_01 = 1; // explicit initialization (init), belegt speicher pro instanz (Object)
        int field_member_02 = 0; // explicit initialization (init),  belegt speicher pro instanz (Object)

        /**
         * Default Constructor
         *
         * A constructor METHOD is Identified by:
         * - No declared return value, nor return statement
         * - Class name _is_ method name
         */
        Class03_2_3a() noexcept {
            std::cout << "Class03_2_3a: Default-Constructor" << std::endl;
        }

        /**
         * Custom Constructor 1
         * @param a init value for field_member_01
         * @param b init value for field_member_02
         */
        Class03_2_3a(const int a, const int b) noexcept
        : field_member_01(a),
          field_member_02(b)
        {
            std::cout << "Class03_2_3a: Custom-Constructor-1" << std::endl;
        }

        /// Copy Constructor
        /// default: `constexpr Class03_2_3a(const Class03_2_3a &o) noexcept = default;`
        Class03_2_3a(const Class03_2_3a &o) noexcept
        : field_member_01(o.field_member_01),
          field_member_02(o.field_member_02)
        {
            std::cout << "Class03_2_3a: Copy-Constructor" << std::endl;
        }

        /// Move Constructor
        Class03_2_3a(Class03_2_3a &&o) noexcept = default;
        /// Copy-Assignment operator
        Class03_2_3a& operator=(const Class03_2_3a &o) noexcept = default;
        /// Move-Assignment operator
        Class03_2_3a& operator=(Class03_2_3a &&o) noexcept = default;

        /**
         * Destructor
         *
         * A destructor METHOD is Identified by:
         * - No declared return value, nor return statement
         * - Leading ~ (tilde)
         * - Class name _is_ method name
         */
        ~Class03_2_3a() noexcept {
            std::cout << "Class03_2_3a: Destructor" << std::endl;
        }

        void increment() noexcept {
            ++field_member_01;
            // 'this' is a pointer to this instance (Object), where method `incement` is applied to
            ++this->field_member_02;
        }

        void reset() noexcept {
            field_member_01 = 1;
            field_member_02 = 0;
        }

        /** Two way comparison operator */
        bool operator==(const Class03_2_3a& o) const noexcept {
            return field_member_01 + field_member_02 == o.field_member_01 + o.field_member_02;
        }

        /** Three way std::strong_ordering comparison operator */
        std::strong_ordering operator<=>(const Class03_2_3a& o) const noexcept {
            const int s1 = field_member_01 + field_member_02;
            const int s2 = o.field_member_01 + o.field_member_02;
            if( s1 == s2 ) {
                return std::strong_ordering::equal;
            } else if( s1 < s2 ) {
                return std::strong_ordering::less;
            } else {
                return std::strong_ordering::greater;
            }
        }

        /** Compare function returns 0 if equal, -1 if *this < b and 1 if *this > b. */
        int compareTo(const Class03_2_3a& o) const noexcept {
            const int s1 = field_member_01 + field_member_02;
            const int s2 = o.field_member_01 + o.field_member_02;
            if( s1 == s2 ) {
                return 0;
            } else if( s1 < s2 ) {
                return -1;
            } else {
                return 1;
            }
        }

        constexpr std::size_t hashCode() const noexcept {
            // 31 * x == (x << 5) - x
            std::size_t h = 31 + field_member_01;
            h = ( ( h << 5 ) - h ) + field_member_01;
            return h;
        }

        /// std::ostream (stream out) free operator overload
        std::ostream& streamout(std::ostream& out) const {
            return out << "Class03_2_3a[ref 0x" << std::hex << this
                       << std::dec
                       << ", 01 " << field_member_01
                       << ", 02 " << field_member_01
                       << "]";
        }

        std::string toString() const {
            std::stringstream ss;
            streamout(ss);
            return ss.str();
        }
};

int Class03_2_3a::global_var01 = 1; // storage for global static and explicit initialization (init)
int Class03_2_3a::global_var02;     // storage for global static and w/o initialization

/// std::ostream (stream out) free operator overload
std::ostream& operator<<(std::ostream& out, const Class03_2_3a& v) {
    return v.streamout(out);
}

TEST_CASE( "Test 03.2.3a OOP: Class", "[basic][oop]" ) {
    // 0: Analog to instances of primitive types, class instances are shown below this block
    {
        // i1 (locale variable) ist der Name fuer den Speicherbereich der Groesse 'int'
        const int i1 = 0;
        REQUIRE( 0 == i1 );

        int i2 = 0;
        REQUIRE( 0 == i2 );
        i2 = 1;
        REQUIRE( 1 == i2 );
    }
    // 1a: Default Constructor, detailing reference and automatic storage (stack) instance
    {
        // Create 1st instance of type Class03_2_3a in automatic storage (stack)
        //
        // 1) Compiler allocates stack-memory (memO1) for one instance of type Class03_2_3a
        // 2) Initialize this stack-memory (memO1) using Class03_2_3a's default constructor
        Class03_2_3a i1;

        // 3) Use memory (memO1) reference of this instance in o1
        Class03_2_3a &o1 = i1;

        // instance variable i1 and its reference o1 use same memory address
        REQUIRE(&i1 == &o1);

        REQUIRE(1 == i1.field_member_01);
        REQUIRE(0 == i1.field_member_02);
        REQUIRE(1 == o1.field_member_01);
        REQUIRE(0 == o1.field_member_02);

        // Create 2nd instance of type Class03_2_3a in stack-memory
        //
        // 1) Compiler allocates stack-memory (memO2) for one instance of type Class03_2_3a
        // 2) Initialize this stack-memory (memO2) using Class03_2_3a's default constructor
        Class03_2_3a i2;

        // 3) Use memory (memO2) reference of this instance in o2
        Class03_2_3a &o2 = i2;

        //
        // NOTE: This created instance i2 within memO2 in stack-memory (reference stored in o2)
        //       is different than i1 within memO1 above (reference stored in o1)
        REQUIRE(1 == i2.field_member_01);
        REQUIRE(0 == i2.field_member_02);
        REQUIRE(1 == o2.field_member_01);
        REQUIRE(0 == o2.field_member_02);

        REQUIRE(i1 == i2);
        REQUIRE(0 == i1.compareTo(o2));
        REQUIRE(&i1 != &i2); // fast ref-check

        REQUIRE(o1 == o2);
        REQUIRE(0 == o1.compareTo(o2));
        REQUIRE(&o1 != &o2); // fast ref-check

        o1.increment(); // veraendere Object o1
        REQUIRE(2 == o1.field_member_01);
        REQUIRE(1 == o1.field_member_02);
        REQUIRE(o1 != o2);
        REQUIRE( 1 == o1.compareTo(o2));
        REQUIRE(-1 == o2.compareTo(o1));

        o1.reset();
        REQUIRE(o1 == o2);
        REQUIRE(0 == o1.compareTo(o2));
    }
    // 1b: Default Constructor, detailing reference and heap-memory instance
    {
        // Create 1st instance of type Class03_2_3a in heap memory
        //
        // 1) Allocate heap-memory (memO1) for one instance of type Class03_2_3a
        // 2) Initialize this memory (memO1) using Class03_2_3a's default constructor
        // 3) Store memory (memO1) reference of this instance in o1
        //
        // o1 ist ein ZEIGER (Referenz, Pointer) auf eine Instanz (Object) (memO1)
        // des Typs Class03_2_3a.
        //
        // Object o1 (locale variable) selber ist mutable, aber die Referenz o1 ist immutable!
        Class03_2_3a *o1 = new Class03_2_3a();
        REQUIRE(1 == o1->field_member_01);
        REQUIRE(0 == o1->field_member_02);

        // Create 2nd instance of type Class03_2_3a in heap memory
        //
        // 1) Allocate heap-memory (memO2) for one instance of type Class03_2_3a
        // 2) Initialize this memory (memO2) using Class03_2_3a's default constructor
        // 3) Store memory (memO2) reference of this instance in o2
        //
        // NOTE: This created instance memO2 in heap memory (reference stored in o2)
        //       is different than memO1 above (reference stored in o1)
        // 2. Class03_2_3a instance o2 (locale variable): Heap-Memory allocation (-> new reference), initialization via constructor.
        Class03_2_3a *o2 = new Class03_2_3a();
        REQUIRE(1 == o2->field_member_01);
        REQUIRE(0 == o2->field_member_02);

        REQUIRE(*o1 == *o2);
        REQUIRE(0 == o1->compareTo(*o2));
        REQUIRE(o1 != o2); // fast ref-check

        o1->increment(); // veraendere Object o1
        REQUIRE(2 == o1->field_member_01);
        REQUIRE(1 == o1->field_member_02);
        REQUIRE(*o1 != *o2);
        REQUIRE( 1 == o1->compareTo(*o2));
        REQUIRE(-1 == o2->compareTo(*o1));

        o1->reset();
        REQUIRE(*o1 == *o2);
        REQUIRE(0 == o1->compareTo(*o2));

        /// Manually delete heap-memory (unsafe!)
        delete o1;
        delete o2;
    }
    // 2: Default Constructor for one single stack-memory instance, shared by two reference holder
    {
        Class03_2_3a i1;
        Class03_2_3a &o1 = i1;
        REQUIRE(1 == o1.field_member_01);
        REQUIRE(0 == o1.field_member_02);
        {
            // o1_b is assigned the memory (memO1) reference only, i.e. o1!
            // At this point there is only one actual instanz of Class03_2_3a alive, (memO1).
            const Class03_2_3a &o1_b = o1; // shares same instance i1 via reference
            REQUIRE(&o1 == &o1_b); // fast ref-check
            REQUIRE(o1 == o1_b); // deep comparison
            REQUIRE(0 == o1.compareTo(o1_b));

            // because o1 and o1_b point to the same heap-memory instance of type Class03_2_3a
            // ...
            o1.increment(); // veraendere Object o1
            REQUIRE(o1 == o1_b);
        }
    }
    // 3: Custom Constructor only differs to Default Constructor using its custom initialization
    {
        // 1) Allocate stack-memory for one instance of type Class03_2_3a
        // 2) Initialize this memory using Class03_2_3a's custom constructor
        const Class03_2_3a o1(2, 3);
        REQUIRE(2 == o1.field_member_01);
        REQUIRE(3 == o1.field_member_02);

        const Class03_2_3a o2(2, 3);
        REQUIRE(2 == o2.field_member_01);
        REQUIRE(3 == o2.field_member_02);

        REQUIRE(o1 == o2);
        REQUIRE(0 == o1.compareTo(o2));
    }
    // 4: Custom Copy Constructor only differs to Default Constructor using its special custom initialization (deep-copy)
    {
        Class03_2_3a o1;      // default ctor (Constructor): 1st instance in stack-memory of type Class03_2_3a
        Class03_2_3a o2(o1);  // copy ctor: 2nd instance in heap-memory of type Class03_2_3a
        REQUIRE(o1 == o2);     // same content/value, but different heap-memory instances
        REQUIRE(&o1 != &o2);   // different heap-memory instances, hence different references (pointer to heap-memory)
        std::cout << "4.0: o1 " << o1 << std::endl;
        std::cout << "4.0: o2 " << o2 << std::endl;

        o1.increment(); // veraendere Object o1
        REQUIRE(o1 != o2);
        std::cout << "4.1: o1 " << o1 << std::endl;

        o2.increment();
        REQUIRE(o1 == o2);
        std::cout << "4.1: o2 " << o2 << std::endl;
    }
}

/// RAII owner of heap allocated Class03_2_3a instance similar to std::unique_ptr
class Class03_2_3a_Owner
{
    public:
        Class03_2_3a *m_o;

        Class03_2_3a_Owner() noexcept
        : m_o(nullptr) {}

        Class03_2_3a_Owner(Class03_2_3a* o) noexcept
        : m_o(o) {}

        /// Deleted Copy Constructor
        Class03_2_3a_Owner(const Class03_2_3a_Owner &o) noexcept = delete;
        /// Deleted Copy-Assignment operator
        Class03_2_3a_Owner& operator=(const Class03_2_3a_Owner &o) noexcept = delete;

        /// Default Move Constructor
        Class03_2_3a_Owner(Class03_2_3a_Owner &&o) noexcept = default;
        /// Default Move-Assignment operator
        Class03_2_3a_Owner& operator=(Class03_2_3a_Owner &&o) noexcept = default;

        ~Class03_2_3a_Owner() noexcept {
            if( nullptr != m_o ) {
                delete m_o;
                m_o = nullptr;
            }
        }

        /// may return nullptr, no exception
        Class03_2_3a* ptr() noexcept { return m_o; }

        /// throws std::runtime_error if nullptr!
        Class03_2_3a& ref() {
            if( nullptr == m_o ) {
                throw std::runtime_error("nullptr");
            }
            return *m_o;
        }
};

/// Resource acquisition is initialization
TEST_CASE( "Test 03.2.3b OOP: Class + RAII", "[basic][oop][raii]" ) {
    // Lebenszyklus (Lifecycle / Object-Duration)
    std::cout << "test03_2_3b_class_RAII: 0.0" << std::endl;
    {
        std::cout << "test03_2_3b_class_RAII: 1.0" << std::endl;
        // 1) Allocate heap-memory for one instance of type Class03_2_3a
        // 2) Initialize this memory using Class03_2_3a's default constructor
        // 3) Store memory reference of this instance in k1
        const Class03_2_3a_Owner k1(new Class03_2_3a());
        std::cout << "test03_2_3b_class_RAII: 1.1" << std::endl;
    } // delete local reference-holder _WITH_ releasing heap-memory of Class03_2_3a
    std::cout << "test03_2_3b_class_RAII: 0.1" << std::endl;
}

// 3.3 Speicher-Klassen von Variablen
struct Class03_3 {
    // 3.3.2 Globale (Klassen) Variablen
    static int lala;
    // Konstante .. 3.3.2 Globale (Klassen) Variablen (immutable)
    static const int lulu = 0;

    // 3.3.3 Klassen Instanz Variable (Field)
    int lili = 0;
};
int Class03_3::lala = 0;

/**
 * 3.3 Speicher-Klassen von Variablen
 */
TEST_CASE( "Test 03.3 Storage Classes", "[basic][storage]" ) {
    Class03_3 o1;
    Class03_3 o2;

    // 3.3.3 Klassen Instanz Variable (Field)
    REQUIRE(o1.lili == o2.lili); // gleich werte beider instanzen

    // 3.3.2 Globale (Klassen) Variablen
    REQUIRE(o1.lala == o2.lala); // gleicher wert der gemeinsamen globalen instanz
    REQUIRE(Class03_3::lala == Class03_3::lala); // ditto
    REQUIRE(o1.lulu == o2.lulu); // ditto

    // lili wert der instanz o1 auf 10 gesetzt (und nicht von der instanz o1)
    o1.lili = 10;
    REQUIRE(o1.lili != o2.lili); // o1.lili == 10, o2.lili == 0

    // lala wert der Klasse (aller instanzen) auf 10 gesetzt
    o1.lala = 10;
    REQUIRE(o1.lala == o2.lala); // gleich
    REQUIRE(Class03_3::lala == Class03_3::lala); // ditto
}

TEST_CASE( "Test 03.5 Arrays", "[basic][storage][arrays]" ) {
    /**
     * sizeof(int) == 4 (bytes)
     * int a[] = new int[4];
     * 0x0000: a[0]
     * 0x0004: a[1]
     * 0x0008: a[2]
     * 0x000C: a[3]
     */
    {
        const int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        REQUIRE(1+10 == a[0] + a[9]);
        REQUIRE(2 == a[1]);
        REQUIRE(9 == a[8]);
        REQUIRE(10 == sizeof(a) / sizeof(int)); // length of the array == 10! IMMUTABLE! // NOLINT

        {
            const int (&b)[10] = a; // b references array storage instance `a`
            REQUIRE(&a == &b);
        }
    }
    // variant 1
    {
        const int grades[] = { 1, 2, 3, 4, 4, 3, 2, 1, 5, 3,
                               1, 2, 3, 3, 2, 1, 5 };
        const size_t length = sizeof(grades) / sizeof(int);
        REQUIRE(17 == length);
        int sum = 0;
        for(size_t i=0; i<length; ++i /* for-tail-statement */) { // NOLINT (intended)
            sum += grades[i];
        }
        const float median = (float)sum / (float)length;
        std::cout << "Median: " << median << " of " << length << " students. Sum " << sum << std::endl;
    }
    // variant 2
    {
        const int grades[] = { 1, 2, 3, 4, 4, 3, 2, 1, 5, 3,
                               1, 2, 3, 3, 2, 1, 5 };
        const size_t length = sizeof(grades) / sizeof(int);
        REQUIRE(17 == length);
        int sum = 0;
        size_t i = 0;
        while( i<length ) {
            sum += grades[i];
            ++i; // for-tail-statement
        }
        const float median = (float)sum / (float)length;
        std::cout << "Median: " << median << " of " << length << " students. Sum " << sum << std::endl;
    }
    // variant 3
    {
        const int grades[] = { 1, 2, 3, 4, 4, 3, 2, 1, 5, 3,
                               1, 2, 3, 3, 2, 1, 5 };
        const size_t length = sizeof(grades) / sizeof(int);
        REQUIRE(17 == length);
        int sum = 0;
        size_t i = 0;
        while( i<length ) {
            sum += grades[i++];
        }
        const float median = (float)sum / (float)length;
        std::cout << "Median: " << median << " of " << length << " students. Sum " << sum << std::endl;
    }
}

/**
 * Programmfluss-Statement: Branches (if, switch-case, conditional-op)
 */
TEST_CASE( "Test 04.1 Branches", "[basic][programflow][branch]" ) {
    // branches: if
    {
        int state = -1;
        const int a = 0;

        // simple if-else
        {
            // Note that we place the immutable literal (r-value) on the left-side
            // of the equality operation '0 == a',
            // which avoids accidental assignment of a mutable l-value if typo 'a = 0'.
            if( 0 == a ) {
                // true-statement: executed if 'a' contains '0'
                state = 1;
                REQUIRE(true); // NOLINT (intended)
            } else {
                // false-statement: executed if 'a' does not contain '0'
                REQUIRE(false); // unreachable, dead code
            }
            REQUIRE(1 == state);
        }
        state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

        // too complicated if-else-if usage
        {
            if( 0 == a ) {
                // true-statement: executed if 'a' contains '0'
                state = 1;
                REQUIRE(true); // NOLINT (intended)
            } else {
                // false-statement: executed if 'a' does not contain '0'
                if( 1 == a ) {
                    // true-statement: executed if 'a' contains '1'
                    REQUIRE(false); // unreachable, dead code
                } else {
                    // false-statement: executed if 'a' does not contains '0' nor '1'
                    REQUIRE(false); // unreachable, dead code
                }
            }
            REQUIRE(1 == state);
        }
        state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

        // desired if-else-if usage
        {
            if( 0 == a ) {
                // true-statement: executed if 'a' contains '0'
                state = 1;
                REQUIRE(true); // NOLINT (intended)
            } else if( 1 == a ) {
                // true-statement: executed if 'a' contains '1'
                REQUIRE(false); // unreachable, dead code
            } else {
                // false-statement: executed if 'a' does not contains '0' nor '1'
                REQUIRE(false); // unreachable, dead code
            }
            REQUIRE(1 == state);
        }
        state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

        // Note that the expression `0 == a` is a boolean expression,
        // i.e. resolved to either `true` or `false`.
        const bool b0 = 0 == a;
        const bool b1 = 1 == a;

        // Note that the expression for `if` and `while` are boolean expressions.
        //
        // Below we use the pre-computed boolean results.
        if( b0 ) {
            // true-statement: executed if b0 is true, i.e. 'a' contains '0'
            state = 1;
            REQUIRE(true); // NOLINT (intended)
        } else if( b1 ) {
            // false-statement: executed if b1 is true, i.e. 'a' contains '1'
            REQUIRE(false); // unreachable
        }

        REQUIRE(1 == state);
        state = -1;

        // to void errors by missing braces USE braces
        {
            // Desired
            int lala = 0;

            if( b1 ) { state = 1; }
            REQUIRE(-1 == state);
            REQUIRE(0 == lala);

            if( b1 ) { state = 2; lala = 2; }
            REQUIRE(-1 == state);
            REQUIRE(0 == lala);

            // Ugly
            state = -1;
            lala = 0; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

            if( b1 ) state = 1;
            REQUIRE(-1 == state);

            if( b1 ) state = 2; lala = 2;
            REQUIRE(-1 == state);
            // Error: REQUIRE(0, lala); // ERROR: 'lala == 2'
            REQUIRE(2 == lala); // Semantical programming error? Typo? ...
        }
    }

    // branches: switch
    {
        int state = -1;
        const int a = 0;

        // Demo using switch-case instead if-else-if _comparing_ _literals_ (constants)
        {
            // desired if-else-if usage
            {
                if( 0 == a ) {
                    // true-statement: executed if 'a' contains '0'
                    state = 1;
                    REQUIRE(true); // NOLINT (intended)
                } else if( 1 == a ) {
                    // true-statement: executed if 'a' contains '1'
                    REQUIRE(false); // unreachable, dead code
                } else {
                    // false-statement: executed if 'a' does not contains '0' nor '1'
                    REQUIRE(false); // unreachable, dead code
                }
                REQUIRE(1 == state);
            }
            REQUIRE(1 == state);
            state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended
        }
        {
            switch( a ) {
                case 0:
                    // executed if 'a' contains '0'
                    state = 1;
                    REQUIRE(true); // NOLINT (intended)
                    break; // ends code for this case
                case 1:
                    // executed if 'a' contains '1'
                    {
                        // use an inner block-statement to allow local case resources
                        const int v = 1;
                        std::cout << "branch1." << v << std::endl;
                    }
                    REQUIRE(false); // unreachable
                    break; // ends code for this case
                case 2:
                    // executed if 'a' contains '2'
                    // and falls through to default case code
                    REQUIRE(false); // unreachable
                    // [[fallthrough]];
                default:
                    // executed if none of the above cases matches
                    REQUIRE(false); // unreachable
                    break; // ends code for this case
            }
            REQUIRE(1 == state);
        }
    }

    // branches: conditional operator '?'
    {
        {
            const int a = 0;

            // initialized with '0' if 'a' contains '0', otherwise initialized with '1'
            const char c = ( 0 == a ) ? '0' : '1';

            REQUIRE('0' == c);
        }
        {
            int x = 2;

            int l;
            if( 0 < x ) {
                l = 1;
            } else {
                l = 0;
            }
            int r;
            if( x < 0 ) {
                r = 1;
            } else {
                r = 0;
            }
            int result1 = l - r;
            REQUIRE(1 == result1);

            int result2 = (0 < x ? 1 : 0) - (x < 0 ? 1 : 0);
            REQUIRE(result1 == result2);

            // x<-1 -> result=-1
            // x=-1 -> result=-1
            //
            // x= 0 -> result= 0
            //
            // x= 1 -> result= 1
            // x= 2 -> result= 1
            // x> 2 -> result= 1
            //
            // sign(x): Vorzeichen-Funktion
            // Returns -1 bei x<0, 0 bei x==0 und 1 bei x>0
        }
    }
}

/**
 * Programmfluss-Statement: Loops (while, do-while, for, break)
 */
TEST_CASE( "Test 04.2 Loops", "[basic][programflow][loop]" ) {
    const int loop_count = 3;
    // Same loop as while
    {
        // while loop, an exploded for-loop (see below)
        int v=10;
        int i=0;              /* instantiation and initialization of loop variable */
        while( i < loop_count /* while condition */ ) {
            ++v;
            i = i + 1;        /* tail expression */
        }
        REQUIRE(loop_count == i);
        REQUIRE(10+loop_count == v);
    }
    // Same loop as do-while
    {
        // do-while loop - executed at least once
        int v=10;
        int i=0;              /* instantiation and initialization of loop variable */
        do {
            ++v;
            i = i + 1;        /* tail expression */
        } while( i < loop_count /* while condition */ );
        REQUIRE(loop_count == i);
        REQUIRE(10+loop_count == v);
    }
    // Same loop as for (1)
    {
        int v=10;
        int i;                /* instantiation of loop variable*/
        for(i=0               /* initialization of loop variable*/;
            i<loop_count      /* while condition */;
            ++i               /* tail expression */)
        {
            ++v;
        }
        // `i` is still in scope!
        REQUIRE(loop_count == i);
        REQUIRE(10+loop_count == v);
    }
    // Same loop as for (2)
    {
        int v=10;
        /* instantiation and initialization of loop variable; while condition; tail expression */
        for(int i=0; i<loop_count; ++i) {
            ++v;
        }
        // `i` is out of scope!
        // REQUIRE(loop_count, i);
        REQUIRE(10+loop_count == v);
    }
    // Using break within a loop (bad style, rarely unavoidable)
    {
        int v=10;
        int i=0;              /* instantiation and initialization of loop variable */
        while( true /* while condition: forever */ ) {
            if( i >= loop_count ) { // inverted `i<loop_count`
                break; // exit loop
            }
            ++v;
            i = i + 1;        /* tail expression */
        }
        REQUIRE(loop_count == i);
        REQUIRE(10+loop_count == v);
    }
    // loop through an array: forward
    {
        // Calculate the sum of all array elements
        const int a[] = { 1, 2, 3, 4 };
        const size_t length = sizeof(a) / sizeof(int);
        int sum = 0;
        for(size_t i=0; i<length; ++i) { // NOLINT(modernize-loop-convert)
            sum += a[i];
        }
        REQUIRE(10 == sum);
    }
    // loop through an array: backwards
    {
        // Calculate the sum of all array elements
        const int a[] = { 1, 2, 3, 4 };
        const size_t length = sizeof(a) / sizeof(int);
        int sum = 0;
        for(size_t i=length; i-->0; ) { // NOTE: We avoid underflow of unsigned type size_t
            sum += a[i];
        }
        REQUIRE(10 == sum);
    }
}

