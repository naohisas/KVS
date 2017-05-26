#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Float>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>


int main( int argc, char** argv )
{
    kvs::python::Interpreter intepreter;

    const char* script_file_name = "add"; // w/o '.py'
    kvs::python::Module module( script_file_name );
    kvs::python::Dict dict = module.dict();

    const char* func_name = "main"; // function defined in 'hello.py'
    kvs::python::Callable func( dict.find( func_name ) );

    kvs::Real32 a = 1.1f;
    kvs::Real32 b = 2.2f;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    kvs::python::Tuple args( 2 );
    args.set( 0, kvs::python::Float(a) );
    args.set( 1, kvs::python::Float(b) );

    kvs::Real32 c = kvs::python::Float( func.call( args ) );
    std::cout << "a + b = " << c << std::endl;

    return 0;
}
