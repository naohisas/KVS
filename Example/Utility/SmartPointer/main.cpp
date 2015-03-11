/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::SharedPointer and kvs::WeakPointer class.
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include <iostream>
#define KVS_ENABLE_THREAD_SAFE
#include <kvs/SmartPointer>


void PrintLine()
{
    std::cout << std::endl;
}

template <typename T>
void PrintLine(const T& obj)
{
    std::cout << obj << std::endl;
}

class Base
{
public:
    void func() const
    {
        PrintLine("func is called.");
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        PrintLine("Constructor of Derived is called.");
    }

    ~Derived()
    {
        PrintLine("Destructor of Derived is called.");
    }
};

struct CustumDeleterForDerived
{
    void operator()(const Derived* ptr)
    {
        PrintLine("Custum deleter of Derived is called.");
        delete ptr;
    }
};



int main()
{
    //Basic Usage//
    {
        kvs::SharedPointer<Base> sp1(new Derived); // Constructor of Derived is called.
        sp1->func(); // func is called.
        Base& ref_base = *sp1;
        ref_base.func(); // func is called.
    } // Destructor of Derived is called.
    PrintLine();

    //Assingment and use_count//
    {
        kvs::SharedPointer<Derived> sp1(new Derived); // Constructor of Derived is called.
        PrintLine(sp1.use_count()); // 1

        //(Implicit up-cast)
        kvs::SharedPointer<Base> sp2 = sp1;
        PrintLine(sp2.use_count()); // 2

        //(Explicit down-cast)
        kvs::SharedPointer<Derived> sp3 = kvs::static_pointer_cast<Derived>(sp2);
        PrintLine(sp3.use_count()); // 3

        //(Assignment to shared pointer of void type)
        kvs::SharedPointer<void> sp4 = sp1;
        PrintLine(sp4.use_count()); // 4

        //(Get raw pointer. (not managed))
        Derived* p = sp1.get();
        p->func(); // func is called.

        // (Reset with null pointer.)
        sp1.reset();
        PrintLine(sp4.use_count()); // 3

        //(Reset with another object and custum deleter.)
        sp2.reset(new Derived, CustumDeleterForDerived()); // Constructor of Derived is called.
        PrintLine(sp4.use_count()); // 2

        sp3.reset();
        sp4.reset(); // Destructor of Derived is called.
        PrintLine(sp4.use_count()); // 0

        ////('p' have already been expired. Don't use 'p' here.)
        //p->func();

        sp2.reset(); // Custum deleter of Derived is called. Destructor of Derived is called.
    }
    PrintLine();

    //Weak pointer//
    {
        kvs::SharedPointer<Derived> sp1(new Derived); // Constructor of Derived is called.
        PrintLine(sp1.use_count()); // 1

        //(SharedPointer to WeakPointer.)
        kvs::WeakPointer<Derived> wp1 = sp1;
        PrintLine(sp1.use_count()); // 1 (Reference count is not incremented.)

        std::cout << std::boolalpha;
        PrintLine(wp1.expired()); // false (wp1 is valid.)

        //(WeakPointer to SharedPointer)
        if (kvs::SharedPointer<Derived> sp2 = wp1.lock())
        {
            sp2->func(); // func is called.
        }
        //(Wrong usage)
        //if (!wp1.exprired())
        //{
        //    kvs::SharedPointer<Derived> sp2 = wp1.lock(); // (It it possible 'wp1' have already been expired in multi-thread.)
        //    sp2->func();
        //}

        sp1.reset(); // Destructor of Derived is called.

        PrintLine(wp1.expired()); // true ('wp1' have already been expired.)
        PrintLine(wp1.use_count()); // 0
    }
}
