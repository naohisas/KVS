/*****************************************************************************/
/**
 *  @file   Table.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Table.h"
#include "NumPy.h"
#include <kvs/Message>


namespace
{

template <typename T> int Type() { return NPY_NOTYPE; }
template <> int Type<kvs::Int32>() { return NPY_INT32; }
template <> int Type<kvs::Int64>() { return NPY_INT64; }
template <> int Type<kvs::Real32>() { return NPY_FLOAT32; }
template <> int Type<kvs::Real64>() { return NPY_FLOAT64; }

template <typename IN, typename OUT>
PyObject* Convert( const kvs::ValueTable<IN>& table )
{
    const int ndim = 2;
    const int nrows = table.rowSize();
    const int ncols = table.columnSize();
    npy_intp dims[2] = { nrows, ncols };

    // array: row-major 2D array
    // table: column-major table
    PyArrayObject* array = (PyArrayObject*)PyArray_SimpleNew( ndim, dims, Type<OUT>() );
    for ( int j = 0; j < nrows; ++j )
    {
        for ( int i = 0; i < ncols; ++i )
        {
            *(OUT*)PyArray_GETPTR2( array, j, i ) = static_cast<OUT>( table[i][j] );
        }
    }

    Py_XINCREF( array );
    return PyArray_Return( array );
}

template <typename IN>
PyObject* Convert( const kvs::ValueTable<IN>& table )
{
    using OUT = IN;
    return ::Convert<IN,OUT>( table );
}

template <typename IN, typename OUT>
kvs::ValueTable<OUT> Convert( const PyArrayObject* array )
{
    const int nrows = PyArray_DIMS( (PyArrayObject*)array )[0];
    const int ncols = PyArray_DIMS( (PyArrayObject*)array )[1];

    // array: row-major 2D array
    // table: column-major table
    kvs::ValueTable<OUT> table( nrows, ncols );
    for ( int j = 0; j < nrows; ++j )
    {
        for ( int i = 0; i < ncols; i++ )
        {
            table[i][j] = static_cast<OUT>( *(IN*)PyArray_GETPTR2( (PyArrayObject*)array, j, i ) );
        }
    }

    return table;
}

template <typename OUT>
kvs::ValueTable<OUT> Convert( const PyArrayObject* array )
{
    const int type = PyArray_TYPE( array );
    switch ( type )
    {
    case NPY_INT32:   { using IN = kvs::Int32;  return ::Convert<IN,OUT>( array ); }
    case NPY_INT64:   { using IN = kvs::Int64;  return ::Convert<IN,OUT>( array ); }
    case NPY_FLOAT32: { using IN = kvs::Real32; return ::Convert<IN,OUT>( array ); }
    case NPY_FLOAT64: { using IN = kvs::Real64; return ::Convert<IN,OUT>( array ); }
    default:
        kvsMessageError() << "PyArray type is not supported type." << std::endl;
        break;
    }

    return kvs::ValueTable<OUT>(); // empty table
}

} // end of namespace


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
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 2 )
    {
        kvsMessageError() << "PyArray is not table." << std::endl;
        return kvs::ValueTable<kvs::Int32>(); // empyt table
    }

    return ::Convert<kvs::Int32>( array );
}

Table::operator kvs::ValueTable<kvs::Int64>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 2 )
    {
        kvsMessageError() << "PyArray is not table." << std::endl;
        return kvs::ValueTable<kvs::Int64>(); // empyt table
    }

    return ::Convert<kvs::Int64>( array );
}

Table::operator kvs::ValueTable<kvs::Real32>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 2 )
    {
        kvsMessageError() << "PyArray is not table." << std::endl;
        return kvs::ValueTable<kvs::Real32>(); // empyt table
    }

    return ::Convert<kvs::Real32>( array );
}

Table::operator kvs::ValueTable<kvs::Real64>() const
{
    const auto* array = (const PyArrayObject*)get();

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 2 )
    {
        kvsMessageError() << "PyArray is not table." << std::endl;
        return kvs::ValueTable<kvs::Real64>(); // empyt table
    }

    return ::Convert<kvs::Real64>( array );
}

} // end of namespace python

} // end of namespace kvs
