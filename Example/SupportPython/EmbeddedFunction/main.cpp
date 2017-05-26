#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Float>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>
#include <sstream>
#include <string>


int main( int argc, char** argv )
{
    kvs::python::Interpreter intepreter;

    std::stringstream code;
    code << "def main(a,b):" << std::endl;
    code << "    return a+b" << std::endl;

    const std::string module_name( "add" );
    kvs::python::Module module( code.str(), module_name );
    kvs::python::Dict dict = module.dict();

    const std::string func_name( "main" );
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
