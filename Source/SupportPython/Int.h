#pragma once
#include "Object.h"


namespace kvs
{

namespace python
{

class Int : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Int( const kvs::Int32 value );
    Int( const kvs::Int64 value );
    Int( const kvs::python::Object& value );

    operator kvs::Int32() const;
    operator kvs::Int64() const;
};

} // end of namespace python

} // end of namespace kvs
