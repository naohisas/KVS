#pragma once
#include <string>
#include <kvs/Compiler>
#if KVS_COMPILER_SUPPORT_CXX11
#if defined( KVS_COMPILER_GCC )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#include <Python.h>
#pragma GCC diagnostic pop
#else
#include <Python.h>
#endif
#else
#include <Python.h>
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
