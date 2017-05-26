#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Float>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>
#include <kvs/python/List>


int main( int argc, char** argv )
{
    kvs::python::Interpreter intepreter;

    const char* script_file_name = "list"; // w/o '.py'
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

    kvs::python::List result = func.call( args );
    kvs::Real32 sum = kvs::python::Float( result[0] );
    kvs::Real32 dif = kvs::python::Float( result[1] );
    kvs::Real32 mul = kvs::python::Float( result[2] );
    kvs::Real32 div = kvs::python::Float( result[3] );
    std::cout << "a + b = " << sum << std::endl;
    std::cout << "a - b = " << dif << std::endl;
    std::cout << "a * b = " << mul << std::endl;
    std::cout << "a / b = " << div << std::endl;

    return 0;
}
