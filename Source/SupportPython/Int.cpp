/*****************************************************************************/
/**
 *  @file   Int.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Int.h"


namespace kvs
{

namespace python
{

bool Int::Check( const kvs::python::Object& object )
{
#ifdef KVS_PYTHON3
    return PyLong_Check( object.get() );
#else
    return PyInt_Check( object.get() );
#endif
}

Int::Int( const kvs::Int32 value ):
#ifdef KVS_PYTHON3
    kvs::python::Object( PyLong_FromLong( value ) )
#else
    kvs::python::Object( PyInt_FromLong( value ) )
#endif
{
}

Int::Int( const kvs::Int64 value ):
#ifdef KVS_PYTHON3
    kvs::python::Object( PyLong_FromLong( value ) )
#else
    kvs::python::Object( PyInt_FromLong( value ) )
#endif
{
}

Int::Int( const kvs::python::Object& value ):
#ifdef KVS_PYTHON3
    kvs::python::Object( PyNumber_Long( value.get() ) )
#else
    kvs::python::Object( PyNumber_Int( value.get() ) )
#endif
{
}

Int::operator kvs::Int32() const
{
#ifdef KVS_PYTHON3
    return kvs::Int32( PyLong_AsLong( get() ) );
#else
    return kvs::Int32( PyInt_AsLong( get() ) );
#endif
}

Int::operator kvs::Int64() const
{
#ifdef KVS_PYTHON3
    return kvs::Int64( PyLong_AsLong( get() ) );
#else
    return kvs::Int64( PyInt_AsLong( get() ) );
#endif
}

} // end of namespace python

} // end of namespace kvs
