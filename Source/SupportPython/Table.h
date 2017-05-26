#pragma once
#include "Object.h"
#include <kvs/ValueTable>
#include <kvs/Type>


namespace kvs
{

namespace python
{

class Table : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Table( const kvs::ValueTable<kvs::Int32>& array );
    Table( const kvs::ValueTable<kvs::Int64>& array );
    Table( const kvs::ValueTable<kvs::Real32>& array );
    Table( const kvs::ValueTable<kvs::Real64>& array );
    Table( const kvs::python::Object& array );

    operator kvs::ValueTable<kvs::Int32>() const;
    operator kvs::ValueTable<kvs::Int64>() const;
    operator kvs::ValueTable<kvs::Real32>() const;
    operator kvs::ValueTable<kvs::Real64>() const;
};

} // end of namespace python

} // end of namespace kvs
