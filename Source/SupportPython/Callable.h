#pragma once
#include "Object.h"
#include "Tuple.h"


namespace kvs
{

namespace python
{

class Callable : public kvs::python::Object
{
public:
    Callable( const kvs::python::Object& object );

    kvs::python::Object call();
    kvs::python::Object call( const kvs::python::Tuple& args );
};

} // end of namespace python

} // end of namespace kvs
