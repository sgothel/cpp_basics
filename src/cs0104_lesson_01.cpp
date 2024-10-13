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

/**
 * Expressions used in Statements to initialize variables of primitive type.
 * - Variablen mit primitiven Datentypen
 * - Literale (Konstanten)
 * - Operatoren `=`, `+` fuer `int` Werte
 * - Testen der Variablen Inhalte
 */
static void test01() {
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
    assert(2 == i); // Test das variable `i` den Inhalt `2` (literal) hat.

    /**
     * ℤ Ganzzahlen -> Integer Variable, primitiver datentyp `int`
     * - Hier mit Variablenname `j`
     * - ...
     */
    int j; // Undefiniert, nicht initialisiert.
    j = 1; // Weise der Variable den Wert `1` (literal) zu.
    j = j + 1; // Erhoehe den Wert der Variable `j` um `1` (literal)
    assert(2 == j); // Test das variable `j` den Inhalt `2` (literal) hat.
}

/**
 * Block-Statements und Lebensbereich lokaler Variablen
 */
static void test02_3_block_statement() { // Block-0 Anfang
    // Block-1 Anfang
    {
        const int i = 1; // Neue variable `i` mit dem Wert `1` initialisiert
        assert(1 == i);

        // Block-2 Anfang
        {
            const int j = 2; // Neue variable `j` mit dem Wert `2` initialisiert
            const int k = i + j; // Neue variable `k` mit `i+j` initialisiert
            assert(3 == k);
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
            assert(5 == k);
        }
    }
} // Block-0 Ende

/**
 * Primitive Datentypen und Literale (Konstante)
 */
static void test03_1_1_literals() {
    const bool b0 = false; // boolean-value literal
    const uint8_t o0 = 0b0010;     // bit-value literal -> decimal 2
    const char c0 = 'C';        // UTF-16 character value literal
    const char c1 = '0';        // UTF-16 character value literal
    const short s0 = 0x0A;      // hex-value literal -> decimal 10
    const int i0 = 100;         // int-value literal (default)
    const long l0 = 1000L;      // long-value literal
    const float f0 = 3.14f;     // float-value literal
    const double d0 = 2.14;     // double--value literal (default)
    assert(false == b0);
    assert((uint8_t)2 == o0);
    assert('C' == c0);
    assert(0x30 == c1);
    assert(10 == s0);
    assert(100 == i0);
    assert(1000 == l0);
    assert(std::abs(3.14f - f0) <= std::numeric_limits<float>::epsilon()); // Test value mit erlaubter Fehler-Toleranz std::numeric_limits<float>::epsilon()
    assert(std::abs(2.14 - d0) <= std::numeric_limits<double>::epsilon()); // Test value mit erlaubter Fehler-Toleranz std::numeric_limits<double>::epsilon()
}

/**
 * Binary-Operatoren der 4 Grundrechenarten und Modulo (Divisionsrest) anhand des primitiven Datentyps `int`
 */
static void test03_1_2_a_grundrechenarten() {
    // Addition
    {
        {
            static_assert(3 == 1+2);
            assert(3 == 1+2); // NOLINT (intentional)
        }
        {
            const int i = 6; // positiver Wert
            const int j = 2; // positiver Wert
            const int k = i + j;
            assert(8 == k);
        }
        {
            const int i = +6; // positiver Wert
            const int j = -2; // negativer Wert!!
            const int k = i + j;
            assert(4 == k);
        }
    }
    // Subtraktion
    {
        const int i = 6;
        const int j = 2;
        const int k = i - j;
        assert(4 == k);
    }
    // Multiplikation
    {
        const int i = 6;
        const int j = 2;
        const int k = i * j;
        assert(12 == k);
    }
    // Division
    {
        {
            const int i = 6;
            const int j = 2;
            const int k = i / j;
            assert(3 == k);
        }
        {
            const int i = 7;
            const int j = 2;
            const int k = i / j; // as real number: 7/2=3.5, but integer simply cuts off the floating point
            assert(3 == k);
        }
    }
    // Modulo (Divisionsrest)
    {
        {
            const int i = 6;
            const int j = 2;
            const int k = i % j;
            const int l = i - ( i / j ) * j; // Modulo definition, i.e. Divisionsrest
            assert(l == k);
            assert(0 == k);
        }
        {
            const int i = 7;
            const int j = 2;
            const int k = i % j;
            const int l = i - ( i / j ) * j; // Modulo definition, i.e. Divisionsrest
            assert(l == k);
            assert(1 == k);
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
static void test03_1_2_b_unary_post_prefix() {
    {
        int i = 1;
        int j = 1;
        const int c1 = ++i; // c1: rueckgabewert der pre-incr operation
        const int c2 = j++; // c2: rueckgabewert der post-incr operation
        assert( 2 == c1);
        assert( 1 == c2);
        assert( 2 == i);
        assert( 2 == j);
    }
    // Prefix erhoehe (increment) und veringere (decrement)
    {
        int i = 6;
        ++i;
        assert(7 == i);
        --i;
        assert(6 == i);

        // !!!!
        assert(6 == i);   // Inhalt von `i` ist `6`
        assert(7 == ++i); // Inhalt von `i` wird erhoeht, dann zurueckgegeben! // NOLINT
        assert(7 == i);   // Selber Wert
    }
    // Demo preIncr_call_by_value() failure
    {
        const int i = 6;
        assert(7 == preIncr_call_by_value(i)); // OK
        assert(6 == i); // Nope, not a valid pre-increment implementation!
    }
    {
        int i = 6;
        preIncr(i); // call-by-reference
        assert(7 == i);
        preDecr(i); // call-by-reference
        assert(6 == i);

        // !!!!
        assert(6 == i);   // Inhalt von `i` ist `6`
        assert(7 == preIncr(i)); // Inhalt von `i` wird erhoeht, dann zurueckgegeben!
        assert(7 == i);   // Selber Wert
    }

    // Postfix erhoehe (increment) und veringere (decrement)
    {
        int i = 6;
        i++;
        assert(7 == i);
        i--;
        assert(6 == i);

        // !!!!
        assert(6 == i);   // Inhalt von `i` ist `6`
        assert(6 == i++); // Inhalt von `i` wird zurueckgegeben, dann erhoeht! // NOLINT
        assert(7 == i);   // Nun ist der Inhalt von `i`erhoeht
    }
    {
        int i = 6;
        preIncr(i);
        assert(7 == i);
        decrPost(i);
        assert(6 == i);

        // !!!!
        assert(6 == i);   // Inhalt von `i` ist `6`
        assert(6 == incrPost(i)); // Inhalt von `i` wird zurueckgegeben, dann erhoeht!
        assert(7 == i);   // Nun ist der Inhalt von `i`erhoeht
    }
}

/**
 * Zuweisungs-Operatoren inklusive der 4 Grundrechenarten anhand des primitiven Datentyps `int`
 */
static void test03_1_2_c_zuweisung() {
    // Einfache Zuweisung
    {
        int i = 6;
        assert(6 == i);
        i = 7;
        assert(7 == i);
    }

    // Addition-Zuweisung
    {
        int i = 6;
        i += 4;
        assert(10 == i);
    }
    // Subtraktion-Zuweisung
    {
        int i = 6;
        i -= 4;
        assert(2 == i);
    }
    // Multiplikation-Zuweisung
    {
        int i = 6;
        i *= 4;
        assert(24 == i);
    }
    // Division-Zuweisung
    {
        int i = 6;
        i /= 2;
        assert(3 == i);
    }
    // Modulo-Zuweisung
    {
        {
            int i = 6;
            i %= 2;
            assert(0 == i);
        }
        {
            int i = 7;
            i %= 2;
            assert(1 == i);
        }
    }
}

/**
 * Operatoren der logischen Vergleiche anhand des primitiven Datentyps `int`
 */
static void test03_1_2_d_vergleich() {
    // Gleichheit (equality)
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        assert(i == j);

        assert(i != k);
    }
    // Relational
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        assert(false == (i <  j));
        assert(true  == (i <= j));
        assert(true  == (i >= j));
        assert(false == (i >  k));

        assert(true  == (i <  k));
        assert(true  == (i <= k));
        assert(false == (i >= k));
        assert(false == (i >  k));
    }
}

/**
 * Operatoren der logischen Verknuepfung anhand des primitiven Datentyps `int` und `boolean`
 */
void test03_1_2_e_logisch() {
    // Logisch-Und (and) als auch Logisch-Oder (or)
    {
        const int i = 8;
        const int j = 8;
        const int k = 9;
        const bool b0 = i==j;
        const bool b1 = i==k;

        assert(true ==  ( b0 && !b1));
        assert(false == (!b0 ||  b1));

        assert(true ==  (i == j && i != k));
        assert(false == (i != j || i == k));
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
static void test03_1_3_primitive_underoverflow() {
    // trigger (ausloesung) short over- and underflow: no practical use-case
    {
        const short one = 1;
        short i = std::numeric_limits<short>::max();
        i = (short)(i + one); // overflow
        assert(std::numeric_limits<short>::min() == i);

        i = std::numeric_limits<short>::min();
        i = (short)(i - one); // underflow
        assert(std::numeric_limits<short>::max() == i);
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
        // assert(a + b != c); // NOLINT - tautological-constant-out-of-range-compare
    }
    // Safe math using add_overflow()
    // similar to [GCC Integer Overflow Builtins](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html),
    // i.e. detecting under- and overflow for add operation on two integer.
    {
        short res = 0; // one short value
        assert(false == add_overflow((short) 0,       (short) 0, res));
        assert((short)0 == res);

        res = 0;
        assert(false == add_overflow(std::numeric_limits<short>::max(), (short) 0, res));
        assert(std::numeric_limits<short>::max() == res);

        res = 0;
        assert(true  == add_overflow(std::numeric_limits<short>::max(), (short) 1, res));
        assert((short)0 == res);

        res = 0;
        assert(false == add_overflow(std::numeric_limits<short>::max(), (short)-1, res));
        assert(std::numeric_limits<short>::max()-1 == res);

        res = 0;
        assert(false == add_overflow(std::numeric_limits<short>::min(), (short) 1, res));
        assert(std::numeric_limits<short>::min()+1 == res);

        res = 0;
        assert(true == add_overflow(std::numeric_limits<short>::min(), (short)-1, res));
        assert((short)0 == res);

        res = 0;
        assert(true == add_overflow((short) -1,std::numeric_limits<short>::min(), res));
        assert((short)0 == res);
    }
}

/**
 * Primitive Datentypen und Literale (Konstante)
 */
static void test03_1_4_immutable() {
    // Unveraenderbare (Immutable) Variablen, i.e. Konstante
    const int const_i0 = 1;
    // const_i0 = const_i0 + 1; // FEHLER
    assert(1 == const_i0);
}

struct Class03_2_1 {
    int var = 0;
    
    int erhoeheUm(const int a) {
        var += a;
        return var;
    }
};

static void test03_2_1_method_instanz() {
    Class03_2_1 instance;
    assert(2 == instance.erhoeheUm(2));
    assert(2 == instance.var);
    assert(5 == instance.erhoeheUm(3));
    assert(5 == instance.var);
}

struct Class03_2_2 {
    static int addiere(const int a, const int b) {
        return a+b;
    }
};

static void test03_2_2_method_static() {
    assert(5 == Class03_2_2::addiere(2, 3));
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

static void test03_2_3a_class() {
    // 0: Analog to instances of primitive types, class instances are shown below this block
    {
        // i1 (locale variable) ist der Name fuer den Speicherbereich der Groesse 'int'
        const int i1 = 0;
        assert( 0 == i1 );

        int i2 = 0;
        assert( 0 == i2 );
        i2 = 1;
        assert( 1 == i2 );
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
        assert(&i1 == &o1);
        
        assert(1 == i1.field_member_01);
        assert(0 == i1.field_member_02);
        assert(1 == o1.field_member_01);
        assert(0 == o1.field_member_02);

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
        assert(1 == i2.field_member_01);
        assert(0 == i2.field_member_02);
        assert(1 == o2.field_member_01);
        assert(0 == o2.field_member_02);

        assert(i1 == i2);
        assert(0 == i1.compareTo(o2));
        assert(&i1 != &i2); // fast ref-check
        
        assert(o1 == o2);
        assert(0 == o1.compareTo(o2));
        assert(&o1 != &o2); // fast ref-check

        o1.increment(); // veraendere Object o1
        assert(2 == o1.field_member_01);
        assert(1 == o1.field_member_02);
        assert(o1 != o2);
        assert( 1 == o1.compareTo(o2));
        assert(-1 == o2.compareTo(o1));

        o1.reset();
        assert(o1 == o2);
        assert(0 == o1.compareTo(o2));
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
        assert(1 == o1->field_member_01);
        assert(0 == o1->field_member_02);

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
        assert(1 == o2->field_member_01);
        assert(0 == o2->field_member_02);

        assert(*o1 == *o2);
        assert(0 == o1->compareTo(*o2));
        assert(o1 != o2); // fast ref-check

        o1->increment(); // veraendere Object o1
        assert(2 == o1->field_member_01);
        assert(1 == o1->field_member_02);
        assert(*o1 != *o2);
        assert( 1 == o1->compareTo(*o2));
        assert(-1 == o2->compareTo(*o1));

        o1->reset();
        assert(*o1 == *o2);
        assert(0 == o1->compareTo(*o2));
        
        /// Manually delete heap-memory (unsafe!)
        delete o1;
        delete o2;
    }
    // 2: Default Constructor for one single stack-memory instance, shared by two reference holder
    {
        Class03_2_3a i1;
        Class03_2_3a &o1 = i1;
        assert(1 == o1.field_member_01);
        assert(0 == o1.field_member_02);
        {
            // o1_b is assigned the memory (memO1) reference only, i.e. o1!
            // At this point there is only one actual instanz of Class03_2_3a alive, (memO1).
            const Class03_2_3a &o1_b = o1; // shares same instance i1 via reference
            assert(&o1 == &o1_b); // fast ref-check
            assert(o1 == o1_b); // deep comparison
            assert(0 == o1.compareTo(o1_b));

            // because o1 and o1_b point to the same heap-memory instance of type Class03_2_3a
            // ...
            o1.increment(); // veraendere Object o1
            assert(o1 == o1_b);
        }
    }
    // 3: Custom Constructor only differs to Default Constructor using its custom initialization
    {
        // 1) Allocate stack-memory for one instance of type Class03_2_3a
        // 2) Initialize this memory using Class03_2_3a's custom constructor
        const Class03_2_3a o1(2, 3);
        assert(2 == o1.field_member_01);
        assert(3 == o1.field_member_02);

        const Class03_2_3a o2(2, 3);
        assert(2 == o2.field_member_01);
        assert(3 == o2.field_member_02);

        assert(o1 == o2);
        assert(0 == o1.compareTo(o2));
    }
    // 4: Custom Copy Constructor only differs to Default Constructor using its special custom initialization (deep-copy)
    {
        Class03_2_3a o1;      // default ctor (Constructor): 1st instance in stack-memory of type Class03_2_3a
        Class03_2_3a o2(o1);  // copy ctor: 2nd instance in heap-memory of type Class03_2_3a
        assert(o1 == o2);     // same content/value, but different heap-memory instances
        assert(&o1 != &o2);   // different heap-memory instances, hence different references (pointer to heap-memory)
        std::cout << "4.0: o1 " << o1 << std::endl;
        std::cout << "4.0: o2 " << o2 << std::endl;

        o1.increment(); // veraendere Object o1
        assert(o1 != o2);
        std::cout << "4.1: o1 " << o1 << std::endl;

        o2.increment();
        assert(o1 == o2);
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
static void test03_2_3b_class_RAII() {
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
static void test03_3_storage_class() {    
    Class03_3 o1;
    Class03_3 o2;
    
    // 3.3.3 Klassen Instanz Variable (Field)
    assert(o1.lili == o2.lili); // gleich werte beider instanzen

    // 3.3.2 Globale (Klassen) Variablen
    assert(o1.lala == o2.lala); // gleicher wert der gemeinsamen globalen instanz
    assert(Class03_3::lala == Class03_3::lala); // ditto 
    assert(o1.lulu == o2.lulu); // ditto 
    
    // lili wert der instanz o1 auf 10 gesetzt (und nicht von der instanz o1)
    o1.lili = 10;
    assert(o1.lili != o2.lili); // o1.lili == 10, o2.lili == 0
    
    // lala wert der Klasse (aller instanzen) auf 10 gesetzt
    o1.lala = 10;
    assert(o1.lala == o2.lala); // gleich
    assert(Class03_3::lala == Class03_3::lala); // ditto 
}

static void test03_5_arrays() {
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
        assert(1+10 == a[0] + a[9]);
        assert(2 == a[1]);
        assert(9 == a[8]);
        assert(10 == sizeof(a) / sizeof(int)); // length of the array == 10! IMMUTABLE! // NOLINT

        {
            const int (&b)[10] = a; // b references array storage instance `a`
            assert(&a == &b);
        }
    }
    // variant 1
    {
        const int grades[] = { 1, 2, 3, 4, 4, 3, 2, 1, 5, 3,
                               1, 2, 3, 3, 2, 1, 5 };
        const size_t length = sizeof(grades) / sizeof(int);
        assert(17 == length);        
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
        assert(17 == length);
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
        assert(17 == length);
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
static void test04_1_branch() {
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
                assert(true); // NOLINT (intended)
            } else {
                // false-statement: executed if 'a' does not contain '0'
                assert(false); // unreachable, dead code
            }
            assert(1 == state);
        }
        state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

        // too complicated if-else-if usage
        {
            if( 0 == a ) {
                // true-statement: executed if 'a' contains '0'
                state = 1;
                assert(true); // NOLINT (intended)
            } else {
                // false-statement: executed if 'a' does not contain '0'
                if( 1 == a ) {
                    // true-statement: executed if 'a' contains '1'
                    assert(false); // unreachable, dead code
                } else {
                    // false-statement: executed if 'a' does not contains '0' nor '1'
                    assert(false); // unreachable, dead code
                }
            }
            assert(1 == state);
        }
        state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

        // desired if-else-if usage
        {
            if( 0 == a ) {
                // true-statement: executed if 'a' contains '0'
                state = 1;
                assert(true); // NOLINT (intended)
            } else if( 1 == a ) {
                // true-statement: executed if 'a' contains '1'
                assert(false); // unreachable, dead code
            } else {
                // false-statement: executed if 'a' does not contains '0' nor '1'
                assert(false); // unreachable, dead code
            }
            assert(1 == state);
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
            assert(true); // NOLINT (intended)
        } else if( b1 ) {
            // false-statement: executed if b1 is true, i.e. 'a' contains '1'
            assert(false); // unreachable
        }

        assert(1 == state);
        state = -1;

        // to void errors by missing braces USE braces
        {
            // Desired
            int lala = 0;

            if( b1 ) { state = 1; }
            assert(-1 == state);
            assert(0 == lala);

            if( b1 ) { state = 2; lala = 2; }
            assert(-1 == state);
            assert(0 == lala);

            // Ugly
            state = -1;
            lala = 0; // NOLINT(clang-analyzer-deadcode.DeadStores) intended

            if( b1 ) state = 1;
            assert(-1 == state);

            if( b1 ) state = 2; lala = 2;
            assert(-1 == state);
            // Error: assert(0, lala); // ERROR: 'lala == 2'
            assert(2 == lala); // Semantical programming error? Typo? ...
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
                    assert(true); // NOLINT (intended)
                } else if( 1 == a ) {
                    // true-statement: executed if 'a' contains '1'
                    assert(false); // unreachable, dead code
                } else {
                    // false-statement: executed if 'a' does not contains '0' nor '1'
                    assert(false); // unreachable, dead code
                }
                assert(1 == state);
            }
            assert(1 == state);
            state = -1; // NOLINT(clang-analyzer-deadcode.DeadStores) intended
        }
        {
            switch( a ) {
                case 0:
                    // executed if 'a' contains '0'
                    state = 1;
                    assert(true); // NOLINT (intended)
                    break; // ends code for this case
                case 1:
                    // executed if 'a' contains '1'
                    {
                        // use an inner block-statement to allow local case resources
                        const int v = 1;
                        std::cout << "branch1." << v << std::endl;
                    }
                    assert(false); // unreachable
                    break; // ends code for this case
                case 2:
                    // executed if 'a' contains '2'
                    // and falls through to default case code
                    assert(false); // unreachable
                    // [[fallthrough]];
                default:
                    // executed if none of the above cases matches
                    assert(false); // unreachable
                    break; // ends code for this case
            }
            assert(1 == state);
        }
    }

    // branches: conditional operator '?' 
    {
        {
            const int a = 0;

            // initialized with '0' if 'a' contains '0', otherwise initialized with '1'
            const char c = ( 0 == a ) ? '0' : '1';

            assert('0' == c);
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
            assert(1 == result1);
            
            int result2 = (0 < x ? 1 : 0) - (x < 0 ? 1 : 0);
            assert(result1 == result2);
            
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
static void test04_2_loops() {
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
        assert(loop_count == i);
        assert(10+loop_count == v);
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
        assert(loop_count == i);
        assert(10+loop_count == v);
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
        assert(loop_count == i);
        assert(10+loop_count == v);
    }
    // Same loop as for (2)
    {
        int v=10;
        /* instantiation and initialization of loop variable; while condition; tail expression */
        for(int i=0; i<loop_count; ++i) {
            ++v;
        }
        // `i` is out of scope!
        // assert(loop_count, i);
        assert(10+loop_count == v);
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
        assert(loop_count == i);
        assert(10+loop_count == v);
    }
    // loop through an array: forward
    {
        // Calculate the sum of all array elements
        const int a[] = { 1, 2, 3, 4 };
        const size_t length = sizeof(a) / sizeof(int);
        int sum = 0;
        for(size_t i=0; i<length; ++i) {
            sum += a[i];
        }
        assert(10 == sum);
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
        assert(10 == sum);
    }
}

int main(int /*argc*/, const char* /*argv*/[]) {
    std::cout << "cs0104_lesson_01: START" << std::endl;
    test01();
    test02_3_block_statement();
    test03_1_1_literals();
    test03_1_2_a_grundrechenarten();
    test03_1_2_b_unary_post_prefix();
    test03_1_2_c_zuweisung();
    test03_1_2_d_vergleich();
    test03_1_3_primitive_underoverflow();
    test03_1_4_immutable();
    test03_2_1_method_instanz();
    test03_2_2_method_static();
    test03_2_3a_class();
    test03_2_3b_class_RAII();
    test03_3_storage_class();
    test03_5_arrays();
    test04_1_branch();
    test04_2_loops();
    std::cout << "cs0104_lesson_01: END - OK" << std::endl;
}
