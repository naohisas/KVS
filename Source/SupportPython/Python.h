#pragma once
#include <Python.h>
#include <string>


namespace kvs
{

namespace python
{

std::string Version();
void Initialize( const bool import = true );
void Finalize();
bool IsInitialized();

} // end of namespace python

} // end of namespace kvs
