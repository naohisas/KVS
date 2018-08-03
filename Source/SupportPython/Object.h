#pragma once
#include "Python.h"
#include <string>
#include <kvs/Type>
#include <kvs/ValueArray>


namespace kvs
{

namespace python
{

class Object
{
private:
    PyObject* m_object;

public:
    explicit Object( PyObject* object = 0, bool borrowed = false );
    Object( const Object& object );
    virtual ~Object();

    PyObject* get() const { return m_object; }
    Object& operator = ( const Object& object );

    void increment() const;
    void decrement() const;
};

} // end of namespace python

} // end of namespace kvs
