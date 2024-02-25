//============================================================================
// Author      : Sven Göthel
// Copyright   : 2024 Göthel Software e.K.
// License     : MIT
// Description : C++ Lesson 2.4 OOP (inheritance w/ virtual lifecycle managment)
//============================================================================

#include <limits>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <cassert>
#include <iostream>

class creature_t {
    private:
        static std::atomic_uint64_t next_instance;
        uint64_t m_id;
        int m_init_lifetime_left;
        int m_init_childs_left;
        int m_lifetime_left;
        int m_childs_left;

    protected:
        /** Custom constructor (ctor) */
        creature_t(int lifetime_left, int childs_left) noexcept
        : m_id(++next_instance),
          m_init_lifetime_left(lifetime_left), m_init_childs_left(childs_left),
          m_lifetime_left(lifetime_left), m_childs_left(childs_left) {}

        /** Copy ctor, actually creating a new life (asexual) w/ a new instance id() */
        creature_t(const creature_t& o) noexcept
        : m_id(++next_instance),
          m_init_lifetime_left(o.m_init_lifetime_left), m_init_childs_left(o.m_init_childs_left),
          m_lifetime_left(o.m_init_lifetime_left), m_childs_left(o.m_init_childs_left) {}

        /** Copy ctor, actually creating a new life (sexual) w/ a new instance id() */
        creature_t(const creature_t& a, const creature_t& b) noexcept
        : m_id(++next_instance),
          m_init_lifetime_left((a.m_init_lifetime_left+b.m_init_lifetime_left)/2),
          m_init_childs_left((a.m_init_childs_left+b.m_init_childs_left)/2),
          m_lifetime_left((a.m_init_lifetime_left+b.m_init_lifetime_left)/2),
          m_childs_left((a.m_init_childs_left+b.m_init_childs_left)/2) {}

        /** Move ctor */
        creature_t(creature_t&& o) noexcept
        : m_id(o.m_id), m_init_lifetime_left(o.m_lifetime_left), m_init_childs_left(o.m_childs_left),
          m_lifetime_left(o.m_lifetime_left), m_childs_left(o.m_childs_left)
        {
            o.m_id = 0;
            o.m_childs_left = 0;
            o.m_lifetime_left = 0;
        }

    public:
        /** Virtual destructor (dtor) */
        virtual ~creature_t() noexcept = default;

        /** reproduction type */
        enum class repro_t {
            /** asexual reproduction only */
            asexual,
            /** sexual reproduction only */
            sexual,
            /** asexual and sexual reproduction */
            dual
        };
        /** Returns the reproduction type */
        virtual repro_t repro_type() const noexcept = 0;

    protected:
        /** Fully virtual copy-ctor function (asexual) on this super class */
        virtual std::shared_ptr<creature_t> new_instance() noexcept = 0;

        /** Fully virtual copy-ctor function (sexual) on this super class */
        virtual std::shared_ptr<creature_t> new_instance(const creature_t& o) noexcept = 0;

    public:
        /** create one offspring (asexual) if childs_left() > 0 and repro_type() is not repro_t::sexual, otherwise returns nullptr. */
        std::shared_ptr<creature_t> procreate() noexcept {
            if( repro_t::sexual != repro_type() &&
                m_childs_left > 0 )
            {
                --m_childs_left;
                return new_instance();
            } else {
                return nullptr;
            }
        }

        /** create one offspring (sexual) if childs_left() > 0 and repro_type() is not repro_t::asexual, otherwise returns nullptr. */
        std::shared_ptr<creature_t> procreate(creature_t& o) noexcept {
            if( repro_t::asexual != repro_type() &&
                m_childs_left > 0 && o.childs_left() > 0 )
            {
                --m_childs_left;
                --o.m_childs_left;
                return new_instance(o);
            } else {
                return nullptr;
            }
        }

        /** returns the unique creature id */
        uint64_t id() const noexcept { return m_id; }

        int total_lifetime() const noexcept { return m_init_lifetime_left; }
        int lifetime_left() const noexcept { return m_lifetime_left; }
        int age() const noexcept { return total_lifetime() - lifetime_left(); }
        bool alive() const noexcept { return lifetime_left() > 0; }
        std::string lifesign() const noexcept { return alive() ? "☀" : "✝"; }

        int total_childs() const noexcept { return m_init_childs_left; }
        int childs_left() const noexcept { return m_childs_left; }
        int children() const noexcept { return total_childs() - childs_left(); }

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
            return "id "+std::to_string(id())+", age "+std::to_string(age())+"/"+std::to_string(total_lifetime())+", childs "+std::to_string(children())+"/"+std::to_string(total_childs());
        }
};
std::atomic_uint64_t creature_t::next_instance;

typedef std::shared_ptr<creature_t> shared_creature_t;

std::ostream& operator<<(std::ostream& out, const creature_t& v) {
    return out << v.to_string();
}

/**
 * A plant_t creature_t type supporting both, asexual and sexual procreation.
 */
class plant_t : public creature_t {
    public:
        /** Custom constructor (ctor) */
        plant_t(int lifetime_left, int childs_left) noexcept
        : creature_t(lifetime_left, childs_left) {}

        /** Move ctor */
        plant_t(plant_t&& o) noexcept
        : creature_t(std::move(o)) {}

        virtual repro_t repro_type() const noexcept override { return repro_t::dual; }

        /** Virtual to_string() override  */
        virtual std::string to_string() const noexcept override { return "plant"+lifesign()+"["+creature_t::to_substring()+"]"; }

    protected:
        /** Copy ctor, actually creating a new life (asexual) w/ a new instance id() */
        plant_t(const plant_t& o) noexcept
        : creature_t(o) { }

        /** Copy ctor, actually creating a new life (sexual) w/ a new instance id() */
        plant_t(const plant_t& a, const plant_t& b) noexcept
        : creature_t(a, b) { }

        /** Virtual new_copy() (asexual) override  */
        virtual std::shared_ptr<creature_t> new_instance() noexcept override {
            return std::shared_ptr<plant_t>( new plant_t(*this) );
        }

        /** Virtual new_instance() (sexual) override */
        virtual std::shared_ptr<creature_t> new_instance(const creature_t& o) noexcept override {
            return std::shared_ptr<plant_t>( new plant_t(*this, *static_cast<const plant_t*>(&o)) );
        }
};

/**
 * An animal_t creature_t type supporting sexual procreation only
 */
class animal_t : public creature_t {
    public:
        /** Custom constructor (ctor) */
        animal_t(int lifetime_left, int childs_left) noexcept
        : creature_t(lifetime_left, childs_left) {}

        /** Move ctor */
        animal_t(animal_t&& o) noexcept
        : creature_t(std::move(o)) {}

        virtual repro_t repro_type() const noexcept override { return repro_t::sexual; }

        /** Virtual to_string() override  */
        virtual std::string to_string() const noexcept override { return "animal"+lifesign()+"["+creature_t::to_substring()+"]"; }

    protected:
        /** Copy ctor, actually creating a new life (asexual) w/ a new instance id() */
        animal_t(const animal_t& o) noexcept
        : creature_t(o) { }

        /** Copy ctor, actually creating a new life (sexual) w/ a new instance id() */
        animal_t(const animal_t& a, const animal_t& b) noexcept
        : creature_t(a, b) { }

        /** Virtual new_copy() (asexual) override - returns nullptr  */
        virtual std::shared_ptr<creature_t> new_instance() noexcept override {
            return std::shared_ptr<animal_t>();
        }
        /** Virtual new_instance() (sexual) override */
        virtual std::shared_ptr<creature_t> new_instance(const creature_t& o) noexcept override {
            return std::shared_ptr<animal_t>( new animal_t(*this, *static_cast<const animal_t*>(&o)) );
        }
};

int main(int, char*[]) {
    plant_t p1(3, 1000);
    animal_t a1(80, 6), a2(40, 3);

    std::cout << "00.p1 = " << p1 << std::endl;
    std::cout << "00.a1 = " << a1 << std::endl;
    std::cout << "00.a2 = " << a2 << std::endl;

    creature_t& c1p1 = p1, &c2a1 = a1;
    std::cout << "00.c1p1 = " << c1p1 << std::endl;
    std::cout << "00.c2a1 = " << c2a1 << std::endl;

    p1.tick(1);
    a1.tick(1);
    a2.tick(2);
    std::cout << "01.p1 = " << p1 << std::endl;
    std::cout << "01.a1 = " << a1 << std::endl;
    std::cout << "01.a2 = " << a2 << std::endl;

    shared_creature_t p2 = p1.procreate();
    assert( nullptr != p2 );
    std::cout << "00.p2 = " << *p2 << std::endl;
    {
        shared_creature_t a12 = a1.procreate(); // only sexual procreation supported for this animal_t
        assert( nullptr == a12 );
    }
    shared_creature_t a12 = a1.procreate(a2);
    assert( nullptr != a12 );
    std::cout << "00.a12 = " << *a12 << std::endl;

    p1.tick(2);
    a1.tick(79);
    std::cout << "03.p1 = " << p1 << std::endl;
    std::cout << "79.a1 = " << a1 << std::endl;

    p2->tick(3);
    std::cout << "03.p2 = " << *p2 << std::endl;
    {
        std::vector<shared_creature_t> a12childs;
        {
            shared_creature_t c;
            while( nullptr != ( c = a12->procreate(a1) ) ) {
                a12childs.push_back(c);
            }
            assert( 0 < a12childs.size() );
        }
        std::cout << "00.a12 = " << *a12 << ", created "+std::to_string(a12childs.size()) << std::endl;
        for(shared_creature_t c : a12childs) {
            std::cout << "00.a12.c = " << *c << std::endl;
        }
        a12->tick(60);
        std::cout << "60.a12 = " << *a12 << std::endl;
    }

    {
        // animal_t a3(80, 4);
        // animal_t a3b(a3); // error, copy-ctor is non-public
    }
    {
        animal_t a3(80, 4);
        animal_t a3b(std::move(a3));
        std::cout << "00.a3 = " << a3 << " -> " << a3b << std::endl;
    }
    {
        animal_t a3(80, 4);
        animal_t a3b = std::move(a3);
        std::cout << "00.a3 = " << a3 << " -> " << a3b << std::endl;
    }

    return 0;
}
