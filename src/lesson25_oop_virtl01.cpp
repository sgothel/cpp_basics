//============================================================================
// Author      : Sven Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 2.5 OOP (virtual funcation call in ctor/dtor)
//============================================================================

#include <iostream>

#include<iostream>
using namespace std;

class base_base_dog {
    public:
        base_base_dog()
        {
            cout<< "base_dog::ctor begin" <<endl;
            bark() ; //NOLINT(clang-analyzer-optin.cplusplus.VirtualCall): intentional
            
            cout<< "base_dog::ctor end" <<endl;
        }

        virtual ~base_base_dog()
        {
            cout<< "base_dog::dtor begin" <<endl;
            bark(); //NOLINT(clang-analyzer-optin.cplusplus.VirtualCall): intentional
            cout<< "base_dog::dtor end" <<endl;
        }

        virtual void bark()
        {
            cout<< "base_dog::bark" <<endl;
        }
};

class top_dog : public base_base_dog {
    public:
        top_dog()
        {
            cout<< "top_dog::ctor begin" <<endl;
            bark(); //NOLINT(clang-analyzer-optin.cplusplus.VirtualCall): intentional
            cout<< "top_dog::ctor end" <<endl;
        }

        ~top_dog() override
        {
            cout<< "top_dog::dtor" <<endl;
            bark(); //NOLINT(clang-analyzer-optin.cplusplus.VirtualCall): intentional
            cout<< "top_dog::end" <<endl;
        }

        void bark() override
        {
            cout<< "top_dog::bark" <<endl;
        }
};

int main()
{
    {
        cout<< "Main: Init" <<endl;
        top_dog d;
        cout<< "Main: top_dog created" <<endl;
        d.bark();
        cout<< "Main: top_dog to be dtor'ed" <<endl;
    }
    cout<< "Main: Done" <<endl;
}
