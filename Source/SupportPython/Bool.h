#pragma once
#include "Object.h"


namespace kvs
{

namespace python
{

class Bool : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Bool( const bool value );

    operator bool() const;
};

} // end of namespace python

} // end of namespace kvs
