#include "Tuple.h"


namespace kvs
{

namespace python
{

Tuple::Tuple( const size_t size ):
    kvs::python::Object( PyTuple_New( size ) )
{
}

Tuple::Tuple( const kvs::python::Object& object ):
    kvs::python::Object( PyTuple_New( 1 ) )
{
    this->set( 0, object );
}

Tuple::Tuple(
    const kvs::python::Object& object0,
    const kvs::python::Object& object1 ):
    kvs::python::Object( PyTuple_New( 2 ) )
{
    this->set( 0, object0 );
    this->set( 1, object1 );
}

Tuple::Tuple(
    const kvs::python::Object& object0,
    const kvs::python::Object& object1,
    const kvs::python::Object& object2 ):
    kvs::python::Object( PyTuple_New( 3 ) )
{
    this->set( 0, object0 );
    this->set( 1, object1 );
    this->set( 2, object2 );
}

Tuple::Tuple(
    const kvs::python::Object& object0,
    const kvs::python::Object& object1,
    const kvs::python::Object& object2,
    const kvs::python::Object& object3 ):
    kvs::python::Object( PyTuple_New( 4 ) )
{
    this->set( 0, object0 );
    this->set( 1, object1 );
    this->set( 2, object2 );
    this->set( 3, object3 );
}

void Tuple::set( const size_t index, const kvs::python::Object& object )
{
    object.increment();
    PyTuple_SetItem( get(), Py_ssize_t( index ), object.get() );
}

size_t Tuple::size() const
{
    return static_cast<size_t>( PyTuple_Size( get() ) );
}

kvs::python::Object Tuple::operator [] ( const size_t index ) const
{
    const bool borrowed = true;
    return kvs::python::Object( PyTuple_GetItem( get(), Py_ssize_t( index ) ), borrowed );
}

} // end of namespace python

} // end of namespace kvs
