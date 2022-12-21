/*****************************************************************************/
/**
 *  @file   Array.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Array.h"
#include "NumPy.h"
#include <kvs/Message>


namespace
{

template <typename T> int Type() { return NPY_NOTYPE; }
template <> int Type<kvs::Int8>()   { return NPY_INT8;    }
template <> int Type<kvs::Int16>()  { return NPY_INT16;   }
template <> int Type<kvs::Int32>()  { return NPY_INT32;   }
template <> int Type<kvs::Int64>()  { return NPY_INT64;   }
template <> int Type<kvs::UInt8>()  { return NPY_UINT8;   }
template <> int Type<kvs::UInt16>() { return NPY_UINT16;  }
template <> int Type<kvs::UInt32>() { return NPY_UINT32;  }
template <> int Type<kvs::UInt64>() { return NPY_UINT64;  }
template <> int Type<kvs::Real32>() { return NPY_FLOAT32; }
template <> int Type<kvs::Real64>() { return NPY_FLOAT64; }

template <typename IN, typename OUT>
PyObject* Convert( const kvs::ValueArray<IN>& array )
{
    const int ndim = 1;
    npy_intp dims[1] = { (npy_intp)(array.size()) };

    PyArrayObject* object = (PyArrayObject*)PyArray_SimpleNew( ndim, dims, Type<OUT>() );
    for ( size_t i = 0; i < array.size(); i++ )
    {
        *(OUT*)PyArray_GETPTR1( object, i ) = static_cast<OUT>( array[i] );
    }

    Py_XINCREF( object );
    return PyArray_Return( object );
}

template <typename IN>
PyObject* Convert( const kvs::ValueArray<IN>& array )
{
    using OUT = IN;
    return Convert<IN,OUT>( array );
}

template <typename IN, typename OUT>
kvs::ValueArray<OUT> Convert( const PyArrayObject* object )
{
    const size_t size = PyArray_DIMS( (PyArrayObject*)object )[0];

    kvs::ValueArray<OUT> array( size );
    for ( size_t i = 0; i < size; i++ )
    {
        array[i] = static_cast<OUT>( *(IN*)PyArray_GETPTR1( (PyArrayObject*)object, i ) );
    }

    return array;
}

template <typename OUT>
kvs::ValueArray<OUT> Convert( const PyArrayObject* array )
{
    const int type = PyArray_TYPE( array );
    switch ( type )
    {
    case NPY_INT8:    { using IN = kvs::Int8;   return ::Convert<IN,OUT>( array ); }
    case NPY_INT16:   { using IN = kvs::Int16;  return ::Convert<IN,OUT>( array ); }
    case NPY_INT32:   { using IN = kvs::Int32;  return ::Convert<IN,OUT>( array ); }
    case NPY_INT64:   { using IN = kvs::Int64;  return ::Convert<IN,OUT>( array ); }
    case NPY_UINT8:   { using IN = kvs::UInt8;  return ::Convert<IN,OUT>( array ); }
    case NPY_UINT16:  { using IN = kvs::UInt16; return ::Convert<IN,OUT>( array ); }
    case NPY_UINT32:  { using IN = kvs::UInt32; return ::Convert<IN,OUT>( array ); }
    case NPY_UINT64:  { using IN = kvs::UInt64; return ::Convert<IN,OUT>( array ); }
    case NPY_FLOAT32: { using IN = kvs::Real32; return ::Convert<IN,OUT>( array ); }
    case NPY_FLOAT64: { using IN = kvs::Real64; return ::Convert<IN,OUT>( array ); }
    default:
        kvsMessageError() << "PyArray type is not supported type." << std::endl;
        break;
    }

    return kvs::ValueArray<OUT>(); // empty table
}

} // end of namespace


namespace kvs
{

namespace python
{

bool Array::Check( const kvs::python::Object& object )
{
    return
        PyArray_Check( (const PyArrayObject*)object.get() ) &&
        PyArray_NDIM( (const PyArrayObject*)object.get() ) == 1;
}

Array::Array( const kvs::ValueArray<kvs::Int8>& array ):
    kvs::python::Object( ::Convert<kvs::Int8>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Int16>& array ):
    kvs::python::Object( ::Convert<kvs::Int16>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Int32>& array ):
    kvs::python::Object( ::Convert<kvs::Int32>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Int64>& array ):
    kvs::python::Object( ::Convert<kvs::Int64>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::UInt8>& array ):
    kvs::python::Object( ::Convert<kvs::UInt8>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::UInt16>& array ):
    kvs::python::Object( ::Convert<kvs::UInt16>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::UInt32>& array ):
    kvs::python::Object( ::Convert<kvs::UInt32>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::UInt64>& array ):
    kvs::python::Object( ::Convert<kvs::UInt64>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Real32>& array ):
    kvs::python::Object( ::Convert<kvs::Real32>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Real64>& array ):
    kvs::python::Object( ::Convert<kvs::Real64>( array ) )
{
}

Array::Array( const kvs::python::Object& value ):
    kvs::python::Object( value )
{
}

Array::operator kvs::ValueArray<kvs::Int8>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Int8>(); // empyt array
    }

    return ::Convert<kvs::Int8>( array );
}

Array::operator kvs::ValueArray<kvs::Int16>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Int16>(); // empyt array
    }

    return ::Convert<kvs::Int16>( array );
}

Array::operator kvs::ValueArray<kvs::Int32>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Int32>(); // empyt array
    }

    return ::Convert<kvs::Int32>( array );
}

Array::operator kvs::ValueArray<kvs::Int64>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Int64>(); // empyt array
    }

    return ::Convert<kvs::Int64>( array );
}

Array::operator kvs::ValueArray<kvs::UInt8>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::UInt8>(); // empyt array
    }

    return ::Convert<kvs::UInt8>( array );
}

Array::operator kvs::ValueArray<kvs::UInt16>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::UInt16>(); // empyt array
    }

    return ::Convert<kvs::UInt16>( array );
}

Array::operator kvs::ValueArray<kvs::UInt32>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::UInt32>(); // empyt array
    }

    return ::Convert<kvs::UInt32>( array );
}

Array::operator kvs::ValueArray<kvs::UInt64>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::UInt64>(); // empyt array
    }

    return ::Convert<kvs::UInt64>( array );
}

Array::operator kvs::ValueArray<kvs::Real32>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Real32>(); // empyt array
    }

    return ::Convert<kvs::Real32>( array );
}

Array::operator kvs::ValueArray<kvs::Real64>() const
{
    const auto* array = (const PyArrayObject*)get();
    const int ndim = PyArray_NDIM( array );
    if ( ndim != 1 )
    {
        kvsMessageError() << "PyArray is not array." << std::endl;
        return kvs::ValueArray<kvs::Real64>(); // empyt array
    }

    return ::Convert<kvs::Real64>( array );
}

} // end of namespace python

} // end of namespace kvs
