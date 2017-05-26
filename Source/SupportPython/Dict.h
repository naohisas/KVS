#pragma once
#include "Object.h"


namespace kvs
{

namespace python
{

class Dict : public kvs::python::Object
{
public:
    static bool Check( const kvs::python::Object& object );

public:
    Dict( const kvs::python::Object& object );

    kvs::python::Object find( kvs::python::Object& key ) const;
    kvs::python::Object find( const std::string& key ) const;
    size_t size() const;
    void clear();
};

} // end of namespace python

} // end of namespace kvs
