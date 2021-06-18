#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>
#include <kvs/python/Table>
#include <kvs/ValueTable>


int main( int argc, char** argv )
{
    kvs::python::Interpreter intepreter;

    const char* script_file_name = "table"; // w/o '.py'
    kvs::python::Module module( script_file_name );
    kvs::python::Dict dict = module.dict();

    const char* func_name = "main"; // function defined in 'table.py'
    kvs::python::Callable func( dict.find( func_name ) );

    auto table = kvs::ValueTable<kvs::Real32>::Linear( 2, 3 );
    std::cout << "input table = " << table.format() << std::endl;

    kvs::python::Tuple args( 1 );
    args.set( 0, kvs::python::Table( table ) );

    kvs::python::Table result = func.call( args );
    std::cout << "output table = " << kvs::ValueTable<kvs::Real32>( result ).format() << std::endl;

    return 0;
}
