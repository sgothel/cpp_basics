//============================================================================
// Author      : Sven Göthel
// Copyright   : 2024 Göthel Software e.K.
// License     : MIT
// Description : C++ Lesson 2.2 OOP (inheritance w/ virtual functions)
//============================================================================

#include <limits>
#include <string>
#include <memory>
#include <cassert>
#include <iostream>

class creature_t {
    private:
        int m_init_lifetime_left;
        int m_lifetime_left;

    protected:
        /** Custom constructor (ctor) */
        creature_t(int lifetime_left) noexcept
        : m_init_lifetime_left(lifetime_left),
          m_lifetime_left(lifetime_left) {}

        /** Copy ctor */
        creature_t(const creature_t& o) noexcept = default;

    public:
        /** Virtual destructor (dtor) */
        virtual ~creature_t() noexcept = default;

        int total_lifetime() const noexcept { return m_init_lifetime_left; }
        int lifetime_left() const noexcept { return m_lifetime_left; }
        int age() const noexcept { return total_lifetime() - lifetime_left(); }
        bool alive() const noexcept { return lifetime_left() > 0; }
        std::string lifesign() const noexcept { return alive() ? "☀" : "✝"; }

        /** Subtract given years of life and return whether there is lifetime left */
        bool tick(int years) noexcept {
            if( m_lifetime_left > years ) {
                m_lifetime_left -= years;
                return true;
            } else {
                m_lifetime_left = 0;
                return false;
            }
        }

        /** Fully virtual to_string() method to be implemented in children classes */
        virtual std::string to_string() const noexcept = 0;

    protected:
        std::string to_substring() const noexcept {
            return "age "+std::to_string(age())+"/"+std::to_string(total_lifetime());
        }
};

std::ostream& operator<<(std::ostream& out, const creature_t& v) {
    return out << v.to_string();
}

class plant_t : public creature_t {
    public:
        /** Custom constructor (ctor) */
        plant_t(int lifetime_left) noexcept
        : creature_t(lifetime_left) {}

        /** Copy ctor */
        plant_t(const plant_t& o) noexcept = default;

        /** Virtual to_string() override  */
        std::string to_string() const noexcept override { return "plant"+lifesign()+"["+creature_t::to_substring()+"]"; }
};

class animal_t : public creature_t {
    public:
        /** Custom constructor (ctor) */
        animal_t(int lifetime_left) noexcept
        : creature_t(lifetime_left) {}

        /** Copy ctor */
        animal_t(const creature_t& o) noexcept
        : creature_t(o) { }

        /** Virtual to_string() override  */
        std::string to_string() const noexcept override { return "animal"+lifesign()+"["+creature_t::to_substring()+"]"; }
};

int main(int, char*[]) {
    plant_t p1(3);
    animal_t a1(80);

    std::cout << "00.p1 = " << p1 << std::endl;
    std::cout << "00.a1 = " << a1 << std::endl;

    creature_t& c1p1 = p1, &c2a1 = a1;
    std::cout << "00.c1p1 = " << c1p1 << std::endl;
    std::cout << "00.c2a1 = " << c2a1 << std::endl;

    p1.tick(1);
    a1.tick(1);
    std::cout << "01.p1 = " << p1 << std::endl;
    std::cout << "01.a1 = " << a1 << std::endl;

    p1.tick(2);
    a1.tick(79);
    std::cout << "03.p1 = " << p1 << std::endl;
    std::cout << "79.a1 = " << a1 << std::endl;

    return 0;
}
