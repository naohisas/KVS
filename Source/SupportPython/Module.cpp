#include "Module.h"


namespace
{

kvs::python::Object Compile( const std::string& code, const std::string& name )
{
    return kvs::python::Object( Py_CompileString( code.c_str(), name.c_str(), Py_file_input ) );
}

kvs::python::Object Execute( const std::string& name, const kvs::python::Object& compiled_code )
{
    return kvs::python::Object( PyImport_ExecCodeModule( (char*)name.c_str(), compiled_code.get() ) );
}

kvs::python::Object Import( const std::string& code, const std::string& name )
{
    return Execute( name, Compile( code, name ) );
}

}

namespace kvs
{

namespace python
{

bool Module::Check( const kvs::python::Object& object )
{
    return PyModule_Check( object.get() );
}

Module::Module( const std::string& name ):
    Object( PyImport_ImportModule( name.c_str() ) )
{
}

Module::Module( const kvs::python::String& name ):
    Object( PyImport_Import( name.get() ) )
{
}

Module::Module( const std::string& code, const std::string& name ):
    Object( ::Import( code, name ) )
{
}

kvs::python::Dict Module::dict() const
{
    kvs::python::Object object( PyModule_GetDict( get() ), true );
    return kvs::python::Dict( object );
}

} // end of namespace python

} // end of namespace kvs
