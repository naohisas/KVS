/*****************************************************************************/
/**
 *  @file   Python.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Python.h"
#include <kvs/String>

#define PY_ARRAY_UNIQUE_SYMBOL KVS_PYTHON_NUMPY_ARRAYOBJECT_H
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#ifdef KVS_PYTHON3
namespace { inline void* ImportArray() { if ( PyArray_API == NULL ) { import_array(); }  return NULL; } }
#else
namespace { inline void ImportArray() { if ( PyArray_API == NULL ) { import_array(); } } }
#endif


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
    const std::string version = kvs::String::From( PY_VERSION );
    return version;
}

void Initialize( const bool import )
{
    Py_Initialize();

    int argc = 1;
    char argv0[] = "Program";
    char* argv[] = { argv0, NULL };
#ifdef KVS_PYTHON3
    wchar_t** converted_argv = (wchar_t**)PyMem_Malloc( sizeof( wchar_t* )*argc );
    for ( int i = 0; i < argc; i++ )
    {
        wchar_t* v = Py_DecodeLocale( argv[i], NULL );
        converted_argv[i] = v;
    }
    PySys_SetArgv( argc, converted_argv );
#else
    PySys_SetArgv( argc, argv );
#endif

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
