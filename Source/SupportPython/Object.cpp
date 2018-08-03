#include "Object.h"

namespace kvs
{

namespace python
{

Object::Object( PyObject* object, bool borrowed ):
    m_object( object )
{
    if ( borrowed )
    {
        this->increment();
    }
}

Object::Object( const Object& object ):
    m_object( object.m_object )
{
    this->increment();
}

Object::~Object()
{
    this->decrement();
}

Object& Object::operator = ( const Object& object )
{
    object.increment();
    this->decrement();
    m_object = object.m_object;
    return *this;
}

void Object::increment() const
{
    Py_XINCREF( m_object );
}

void Object::decrement() const
{
    Py_XDECREF( m_object );
}

} // end of namespace python

} // end of namespace kvs
