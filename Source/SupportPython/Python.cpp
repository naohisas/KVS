#include "Python.h"
#include <kvs/String>

#define PY_ARRAY_UNIQUE_SYMBOL KVS_PYTHON_NUMPY_ARRAYOBJECT_H
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
namespace { inline void ImportArray() { if ( PyArray_API == NULL ) { import_array(); } } }


namespace kvs
{

namespace python
{

std::string Description()
{
    return "Python - Python Extension Modules in C/C++";
}

std::string Version()
{
    const std::string version = kvs::String::ToString( PY_VERSION );
    return version;
}

void Initialize( const bool import )
{
    Py_Initialize();

    int argc = 1;
    char argv0[] = "Program";
    char* argv[] = { argv0, NULL };
    PySys_SetArgv( argc, argv );

    if ( import ) { ::ImportArray(); }
}

void Finalize()
{
    Py_Finalize();
}

bool IsInitialized()
{
    return Py_IsInitialized();
}

} // end of namespace python

} // end of namespace kvs
