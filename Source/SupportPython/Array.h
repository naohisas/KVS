#pragma once
#include "Object.h"
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace python
{

class Array : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Array( const kvs::ValueArray<kvs::Int32>& array );
    Array( const kvs::ValueArray<kvs::Int64>& array );
    Array( const kvs::ValueArray<kvs::Real32>& array );
    Array( const kvs::ValueArray<kvs::Real64>& array );
    Array( const kvs::python::Object& array );

    operator kvs::ValueArray<kvs::Int32>() const;
    operator kvs::ValueArray<kvs::Int64>() const;
    operator kvs::ValueArray<kvs::Real32>() const;
    operator kvs::ValueArray<kvs::Real64>() const;
};

} // end of namespace python

} // end of namespace kvs
