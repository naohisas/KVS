/****************************************************************************/
/**
 *  @file ValueArray.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ValueArray.h 1422 2013-03-02 06:32:39Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__VALUE_ARRAY_H_INCLUDE
#define KVS__VALUE_ARRAY_H_INCLUDE

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#if KVS_ENABLE_DEPRECATED
#include <cstring>
#endif
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/SharedPointer>
#if KVS_ENABLE_DEPRECATED
#include <kvs/Endian>
#endif

namespace kvs
{

namespace temporal
{

template <typename T>
struct ArrayDeleter
{
    void operator ()( T* ptr )
    {
        delete [] ptr;
    }
};

}

/*==========================================================================*/
/**
 *  Value array class.
 */
/*==========================================================================*/
template<typename T>
class ValueArray
{
public:
    typedef ValueArray<T>                         this_type;
    typedef T                                     value_type;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef std::size_t                           size_type;
    typedef std::ptrdiff_t                        difference_type;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    kvs::SharedPointer<value_type>  m_values;
    size_t                          m_size; ///< Number of values.

public:
    ValueArray()
    {
        m_size = 0;
    }

    explicit ValueArray( const size_t size )
    {
        this->allocate( size );
    }

    ValueArray( const value_type* values, const size_t size )
    {
        this->allocate( size );
        std::copy( values, values + size, this->begin() );
    }

    explicit ValueArray( const std::vector<T>& values )
    {
        this->allocate( values.size() );
        std::copy( values.begin(), values.end(), this->begin() );
    }

    template <typename InIter>
    ValueArray( InIter first, InIter last )
    {
        this->allocate( std::distance( first, last ) );
        std::copy( first, last, this->begin() );
    }

    ValueArray( const kvs::SharedPointer<T>& sp, size_t size ):
        m_values( sp )
    {
        m_size = size;
    }

public:
    void assign( const value_type* values, const size_t size )
    {
        *this = ValueArray( values, size );
    }

    template <typename InIter>
    void assign( InIter first, InIter last )
    {
        *this = ValueArray( first, last );
    }

public:
    iterator begin()
    {
        return this->data();
    }

    const_iterator begin() const
    {
        return this->data();
    }

    iterator end()
    {
        return this->begin() + this->size();
    }

    const_iterator end() const
    {
        return this->begin() + this->size();
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator( this->end() );
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator( this->end() );
    }

    reverse_iterator rend()
    {
        return reverse_iterator( this->begin() );
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator( this->begin() );
    }

public:
    reference operator []( const size_t index )
    {
        KVS_ASSERT( index < this->size() );
        return this->data()[ index ];
    }

    const_reference operator []( const size_t index ) const
    {
        KVS_ASSERT( index < this->size() );
        return this->data()[ index ];
    }

    ValueArray& operator =( const ValueArray& rhs )
    {
        ValueArray temp( rhs );
        temp.swap( *this );
        return *this;
    }

    friend bool operator ==( const this_type& lhs, const this_type& rhs )
    {
        return lhs.size() == rhs.size() &&
               std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    friend bool operator <( const this_type& lhs, const this_type& rhs )
    {
        return std::lexicographical_compare( lhs.begin(), lhs.end(),
                                             rhs.begin(), rhs.end() );
    }

    friend bool operator !=( const this_type& lhs, const this_type& rhs )
    {
        return !( lhs == rhs );
    }

    friend bool operator >( const this_type& lhs, const this_type& rhs )
    {
        return rhs < lhs;
    }

    friend bool operator <=( const this_type& lhs, const this_type& rhs )
    {
        return !( rhs < lhs );
    }

    friend bool operator >=( const this_type& lhs, const this_type& rhs )
    {
        return !( lhs < rhs );
    }

public:
    reference front()
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[0];
    }

    const_reference front() const
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[0];
    }

    reference back()
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[ this->size() - 1 ];
    }

    const_reference back() const
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[ this->size() - 1 ];
    }

    size_type size() const
    {
        return m_size;
    }

    size_type byteSize() const
    {
        return this->size() * sizeof( value_type );
    }

    const kvs::SharedPointer<value_type>& sharedPointer() const
    {
        return m_values;
    }

    value_type* data()
    {
        return m_values.get();
    }

    const value_type* data() const
    {
        return m_values.get();
    }

    bool empty() const
    {
        return this->size() == 0;
    }

    void swap( ValueArray& other )
    {
        std::swap( m_values, other.m_values );
        std::swap( m_size, other.m_size );
    }

    ValueArray clone() const
    {
        return ValueArray( this->data(), this->size() );
    }

    reference at( const size_t index )
    {
        return ( *this )[ index ];
    }

    const_reference at( const size_t index ) const
    {
        return ( *this )[ index ];
    }

    value_type* pointer()
    {
        return this->data();
    }

    const value_type* pointer() const
    {
        return this->data();
    }

public:
#if KVS_ENABLE_DEPRECATED
    value_type* allocate( const size_t size )
    {
        this->release();
        m_values.reset( new value_type[ size ], kvs::temporal::ArrayDeleter<value_type>() );
        m_size = size;
        return this->data();
    }

    void shallowCopy( const this_type& other )
    {
        *this = other;
    }

    void deepCopy( const this_type& other )
    {
        *this = other.clone();
    }

    void deepCopy( const value_type* values, const size_t nvalues )
    {
        *this = ValueArray( values, nvalues );
    }

    bool isEmpty() const
    {
        return this->empty();
    }

    void swapByte()
    {
        kvs::Endian::Swap( this->data(), this->size() );
    }

    void deallocate()
    {
        this->release();
    }

#else
    void allocate( const size_t size )
    {
        m_values.reset( new value_type[ size ], kvs::temporal::ArrayDeleter<value_type>() );
        m_size = size;
    }
#endif

    void fill( const T& value )
    {
        std::fill( this->begin(), this->end(), value );
    }

    void release()
    {
        m_values.reset();
        m_size = 0;
    }

    bool unique() const
    {
        return m_values.unique();
    }
};

} // end of namespace kvs

namespace std
{

template <typename T>
inline void swap( kvs::ValueArray<T>& lhs, kvs::ValueArray<T>& rhs )
{
    lhs.swap( rhs );
}

} // std

#endif // KVS__VALUE_ARRAY_H_INCLUDE
