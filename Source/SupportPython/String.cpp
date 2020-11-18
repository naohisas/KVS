/*****************************************************************************/
/**
 *  @file   String.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "String.h"

namespace kvs
{

namespace python
{

bool String::Check( const kvs::python::Object& object )
{
#ifdef KVS_PYTHON3
    return PyBytes_Check( object.get() );
#else
    return PyString_Check( object.get() );
#endif
}

String::String( const std::string& value ):
#ifdef KVS_PYTHON3
    kvs::python::Object( PyUnicode_FromString( const_cast<char*>( value.c_str() ) ) )
#else
    kvs::python::Object( PyString_FromString( const_cast<char*>( value.c_str() ) ) )
#endif
{
}

String::String( const kvs::python::Object& value ):
    kvs::python::Object( value )
{
}

String::operator std::string() const
{
#ifdef KVS_PYTHON3
    return std::string( PyBytes_AsString( get() ) );
#else
    return std::string( PyString_AsString( get() ) );
#endif
}

} // end of namespace python

} // end of namespace kvs
