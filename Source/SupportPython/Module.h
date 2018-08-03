#pragma once
#include "Object.h"
#include "String.h"
#include "Dict.h"


namespace kvs
{

namespace python
{

class Module : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Module( const std::string& name );
    Module( const kvs::python::String& name );
    Module( const std::string& code, const std::string& name );

    kvs::python::Dict dict() const;
};

} // end of namespace python

} // end of namespace kvs
