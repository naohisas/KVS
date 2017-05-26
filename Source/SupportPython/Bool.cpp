#include "Bool.h"


namespace kvs
{

namespace python
{

bool Bool::Check( const kvs::python::Object& object )
{
    return PyBool_Check( object.get() );
}

Bool::Bool( const bool value ):
    kvs::python::Object( value ? Py_True : Py_False, true )
{
}

Bool::operator bool() const
{
    return PyLong_AsLong( get() ) != 0;
}

} // end of namespace python

} // end of namespace kvs
