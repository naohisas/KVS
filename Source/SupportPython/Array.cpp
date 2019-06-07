#include "Array.h"
#include "NumPy.h"


namespace
{

template <typename T> int Type() { return NPY_NOTYPE; }
template <> int Type<kvs::Int32>() { return NPY_INT32; }
template <> int Type<kvs::Int64>() { return NPY_INT64; }
template <> int Type<kvs::Real32>() { return NPY_FLOAT32; }
template <> int Type<kvs::Real64>() { return NPY_FLOAT64; }

template <typename T>
PyObject* Convert( const kvs::ValueArray<T>& array )
{
    const int ndim = 1;
    npy_intp dims[1] = { (npy_intp)(array.size()) };

    PyArrayObject* object = (PyArrayObject*)PyArray_SimpleNew( ndim, dims, Type<T>() );
    for ( size_t i = 0; i < array.size(); i++ )
    {
        *(T*)PyArray_GETPTR1( object, i ) = array[i];
    }

    Py_XINCREF( object );
    return PyArray_Return( object );
}

template <typename T>
kvs::ValueArray<T> Convert( const PyArrayObject* object )
{
    const size_t size = PyArray_DIMS( (PyArrayObject*)object )[0];

    kvs::ValueArray<T> array( size );
    for ( size_t i = 0; i < size; i++ )
    {
        array[i] = *(T*)PyArray_GETPTR1( (PyArrayObject*)object, i );
    }

    return array;
}

}

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

Array::Array( const kvs::ValueArray<kvs::Int32>& array ):
    kvs::python::Object( ::Convert<kvs::Int32>( array ) )
{
}

Array::Array( const kvs::ValueArray<kvs::Int64>& array ):
    kvs::python::Object( ::Convert<kvs::Int64>( array ) )
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

Array::operator kvs::ValueArray<kvs::Int32>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_INT32 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 1 ) { throw ""; }

    return ::Convert<kvs::Int32>( (PyArrayObject*)( get() ) );
}

Array::operator kvs::ValueArray<kvs::Int64>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_INT64 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 1 ) { throw ""; }

    return ::Convert<kvs::Int64>( (PyArrayObject*)( get() ) );
}

Array::operator kvs::ValueArray<kvs::Real32>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_FLOAT32 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 1 ) { throw ""; }

    return ::Convert<kvs::Real32>( (PyArrayObject*)( get() ) );
}

Array::operator kvs::ValueArray<kvs::Real64>() const
{
    const int type = PyArray_TYPE( (const PyArrayObject*)get() );
    if ( type != NPY_FLOAT64 ) { throw ""; }

    const int ndim = PyArray_NDIM( (const PyArrayObject*)get() );
    if ( ndim != 1 ) { throw ""; }

    return ::Convert<kvs::Real64>( (PyArrayObject*)( get() ) );
}

} // end of namespace python

} // end of namespace kvs
