#include "Table.h"
#include "NumPy.h"


namespace
{

template <typename T> int Type() { return NPY_NOTYPE; }
template <> int Type<kvs::Int32>() { return NPY_INT32; }
template <> int Type<kvs::Int64>() { return NPY_INT64; }
template <> int Type<kvs::Real32>() { return NPY_FLOAT32; }
template <> int Type<kvs::Real64>() { return NPY_FLOAT64; }

template <typename T>
PyObject* Convert( const kvs::ValueTable<T>& table )
{
    const int ndim = 2;
    const int nrows = table[0].size();
    const int ncols = table.columnSize();
    npy_intp dims[2] = { nrows, ncols };

    PyArrayObject* array = (PyArrayObject*)PyArray_SimpleNew( ndim, dims, Type<T>() );
    for ( int i = 0; i < nrows; i++ )
    {
        for ( int j = 0; j < ncols; j++ )
        {
            *(T*)PyArray_GETPTR2( array, i, j ) = table[j][i];
        }
    }

    Py_XINCREF( array );
    return PyArray_Return( array );
}

template <typename T>
kvs::ValueTable<T> Convert( const PyArrayObject* array )
{
    const int nrows = PyArray_DIMS( (PyArrayObject*)array )[0];
    const int ncols = PyArray_DIMS( (PyArrayObject*)array )[1];

    kvs::ValueTable<T> table( ncols, nrows );
    for ( int i = 0; i < nrows; i++ )
    {
        for ( int j = 0; j < ncols; j++ )
        {
            table[j][i] = *(T*)PyArray_GETPTR2( (PyArrayObject*)array, i, j );
        }
    }

    return table;
}

}

namespace kvs
{

namespace python
{

bool Table::Check( const kvs::python::Object& object )
{
    return
        PyArray_Check( (const PyArrayObject*)object.get() ) &&
        PyArray_NDIM( (const PyArrayObject*)object.get() ) == 2;
}

Table::Table( const kvs::ValueTable<kvs::Int32>& table ):
    kvs::python::Object( ::Convert<kvs::Int32>( table ) )
{
}

Table::Table( const kvs::ValueTable<kvs::Int64>& table ):
    kvs::python::Object( ::Convert<kvs::Int64>( table ) )
{
}

Table::Table( const kvs::ValueTable<kvs::Real32>& table ):
    kvs::python::Object( ::Convert<kvs::Real32>( table ) )
{
}

Table::Table( const kvs::ValueTable<kvs::Real64>& table ):
    kvs::python::Object( ::Convert<kvs::Real64>( table ) )
{
}

Table::Table( const kvs::python::Object& value ):
    kvs::python::Object( value )
{
}

Table::operator kvs::ValueTable<kvs::Int32>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_INT32 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 2 ) { throw ""; }

    return ::Convert<kvs::Int32>( (PyArrayObject*)( get() ) );
}

Table::operator kvs::ValueTable<kvs::Int64>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_INT64 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 2 ) { throw ""; }

    return ::Convert<kvs::Int64>( (PyArrayObject*)( get() ) );
}

Table::operator kvs::ValueTable<kvs::Real32>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_FLOAT32 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 2 ) { throw ""; }

    return ::Convert<kvs::Real32>( (PyArrayObject*)( get() ) );
}

Table::operator kvs::ValueTable<kvs::Real64>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_FLOAT64 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 2 ) { throw ""; }

    return ::Convert<kvs::Real64>( (PyArrayObject*)( get() ) );
}

} // end of namespace python

} // end of namespace kvs
