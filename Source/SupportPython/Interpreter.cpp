#include "Interpreter.h"
#include "Python.h"


namespace kvs
{

namespace python
{

Interpreter::Interpreter( const bool import )
{
    this->initialize( import );
}

Interpreter::~Interpreter()
{
    if ( this->isInitialized() )
    {
        this->finalize();
    }
}

void Interpreter::initialize( const bool import )
{
    kvs::python::Initialize( import );
}

void Interpreter::finalize()
{
    kvs::python::Finalize();
}

bool Interpreter::isInitialized() const
{
    return kvs::python::IsInitialized();
}

} // end of namespace python

} // end of namespace kvs
