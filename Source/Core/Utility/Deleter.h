#pragma once


namespace kvs
{

template <typename  T>
struct Deleter
{
    void operator ()( T* p ) const { delete p; }
};

template <typename  T>
struct Deleter<T[]>
{
    void operator ()( T* p ) const { delete [] p; }
};

} // end of namespace kvs
