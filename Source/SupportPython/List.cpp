#include "List.h"


namespace kvs
{

namespace python
{

bool List::Check( const kvs::python::Object& object )
{
    return PyList_Check( object.get() );
}

List::List( const size_t size ):
    kvs::python::Object( PyList_New( size ) )
{
}

List::List( const kvs::python::Object& object ):
    kvs::python::Object( object )
{
}

bool List::set( const size_t index, const kvs::python::Object& object )
{
    object.increment();
    return PyList_SetItem( get(), Py_ssize_t( index ), object.get() ) == 0;
}

bool List::insert( const size_t index, const kvs::python::Object& object )
{
    return PyList_Insert( get(), Py_ssize_t( index ), object.get() ) == 0;
}

bool List::append( const kvs::python::Object& object )
{
    return PyList_Append( get(), object.get() ) == 0;
}

size_t List::size() const
{
    return static_cast<size_t>( PyList_Size( get() ) );
}

kvs::python::Object List::operator [] ( const size_t index ) const
{
    const bool borrowed = true;
    return kvs::python::Object( PyList_GetItem( get(), Py_ssize_t( index ) ), borrowed );
}

} // end of namespace python

} // end of namespace kvs
