#include <kvs/python/Interpreter>
#include <kvs/python/String>
#include <kvs/python/Module>
#include <kvs/python/Dict>
#include <kvs/python/Callable>


int main( int argc, char** argv )
{
    kvs::python::Interpreter interpreter;

    const char* script_file_name = "hello"; // w/o '.py'
    kvs::python::Module module( script_file_name );
    kvs::python::Dict dict = module.dict();

    const char* func_name = "main"; // function defined in 'hello.py'
    kvs::python::Callable func( dict.find( func_name ) );
    func.call();

    return 0;
}
