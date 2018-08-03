#include "Callable.h"


namespace kvs
{

namespace python
{

Callable::Callable( const kvs::python::Object& object ):
    kvs::python::Object( object )
{
}

kvs::python::Object Callable::call()
{
    return kvs::python::Object( PyObject_CallObject( get(), NULL ) );
}

kvs::python::Object Callable::call( const kvs::python::Tuple& args )
{
    return kvs::python::Object( PyObject_CallObject( get(), args.get() ) );
}

} // end of namespace python

} // end of namespace kvs
