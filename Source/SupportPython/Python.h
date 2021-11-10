/*****************************************************************************/
/**
 *  @file   Python.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Compiler>

#if KVS_COMPILER_SUPPORT_CXX11
  #if defined( __clang__ )
    #pragma clang diagnostic push
    #if (KVS_PYTHON_VERSION == 2)
      #pragma clang diagnostic ignored "-Wdeprecated-register"
    #endif
    #include <Python.h>
    #pragma clang diagnostic pop
  #elif defined( __GNUC__ )
    #pragma GCC diagnostic push
    #if (KVS_PYTHON_VERSION == 2) && ( __GNUC__ >= 7 )
      #pragma GCC diagnostic ignored "-Wregister"
    #endif
    #include <Python.h>
    #pragma GCC diagnostic pop
  #else
    #include <Python.h>
  #endif
#else
  #include <Python.h>
#endif

#if (KVS_PYTHON_VERSION == 3)
#define KVS_PYTHON3
#endif

namespace kvs
{

namespace python
{

std::string Description();
std::string Version();
void Initialize( const bool import = true );
void Finalize();
bool IsInitialized();

} // end of namespace python

} // end of namespace kvs
