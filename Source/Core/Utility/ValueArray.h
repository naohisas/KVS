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
#pragma once
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Value>
#include <kvs/SharedPointer>
#include <kvs/Deleter>


namespace kvs
{

/*==========================================================================*/
/**
 *  Value array class.
 */
/*==========================================================================*/
template<typename T>
class ValueArray
{
public:
    typedef ValueArray<T> this_type;
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    kvs::SharedPointer<value_type> m_values; ///< values
    size_t m_size; ///< number of values.

public:
    static ValueArray Random( const size_t size, const unsigned int seed = 0 )
    {
        if ( size == 0 ) { return ValueArray(); }
        if ( seed == 0 ) { static bool flag = true; if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; } }
        else { kvs::Value<T>::SetSeed( seed ); }
        this_type v( size );
        const_iterator last = v.end();
        for ( iterator i = v.begin(); i != last; ++i ) { *i = kvs::Value<T>::Random(); }
        return v;
    }

    static ValueArray Random( const size_t size, const T min, const T max, const unsigned int seed = 0 )
    {
        if ( size == 0 ) { return ValueArray(); }
        if ( seed == 0 ) { static bool flag = true; if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; } }
        else { kvs::Value<T>::SetSeed( seed ); }
        this_type v( size );
        const_iterator last = v.end();
        for ( iterator i = v.begin(); i != last; ++i ) { *i = kvs::Value<T>::Random( min, max ); }
        return v;
    }

    static ValueArray Linear( const size_t size, const T start = T(0), const T step = T(1) )
    {
        if ( size == 0 ) { return ValueArray(); }
        this_type v( size );
        const_iterator last = v.end();
        T value = start;
        for ( iterator i = v.begin(); i != last; ++i ) { *i = value; value += step; }
        return v;
    }

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

    ValueArray( std::initializer_list<T> list )
    {
        this->allocate( std::distance( list.begin(), list.end() ) );
        std::copy( list.begin(), list.end(), this->begin() );
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
    std::string format(
        const std::string delim = ", " ) const
    {
        return this->format( delim, "{", "}" );
    }

    std::string format(
        const std::string bracket_l,
        const std::string bracket_r ) const
    {
        return this->format( ", ", bracket_l, bracket_r );
    }

    std::string format(
        const std::string delim,
        const std::string bracket_l,
        const std::string bracket_r ) const
    {
        std::ostringstream os;
        if ( this->empty() ) { os << bracket_l << " " << bracket_r; }
        else
        {
            os << bracket_l << this->front();
            for ( const_iterator i = this->begin() + 1; i != this->end(); i++ ) { os << delim << *i; }
            os << bracket_r;
        }
        return os.str();
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

    friend std::ostream& operator <<( std::ostream& os, const this_type& rhs )
    {
//        if ( rhs.empty() ) { return os << "{ }"; }
//        os << "{" << rhs.front();
//        for ( const_iterator i = rhs.begin() + 1; i != rhs.end(); i++ ) { os << ", " << *i; }
//        return os << "}";
        return os << rhs.format( " ", "", "" );
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
    void allocate( const size_t size )
    {
//        m_values.reset( new value_type[ size ], kvs::temporal::ArrayDeleter<value_type>() );
        m_values.reset( new value_type[ size ], kvs::Deleter<value_type[]>() );
        m_size = size;
    }

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

    T min() const
    {
        return *std::min_element( this->begin(), this->end() );
    }

    T max() const
    {
        return *std::max_element( this->begin(), this->end() );
    }

    T sum() const
    {
        return std::accumulate( this->begin(), this->end(), T(0) );
    }

    size_t argmin() const
    {
        return std::distance( this->begin(), std::min_element( this->begin(), this->end() ) );
    }

    size_t argmax() const
    {
        return std::distance( this->begin(), std::max_element( this->begin(), this->end() ) );
    }

    void sort( const bool ascending = true )
    {
        if ( ascending ) { std::sort( this->begin(), this->end() ); } // 0, 1, 2, 3
        else { std::sort( this->begin(), this->end(), std::greater<T>() ); } // 3, 2, 1, 0
    }

    void shuffle()
    {
        // if ( C++11 )
        // {
        //     std::random_device seed;
        //     std::mt19937 rand( seed );
        //     std::shuffle( this->begin(), this->end(), rand );
        // }
        iterator first = this->begin();
        iterator last = this->end();
        difference_type n = last - first;
        kvs::Value<size_t>::SetRandomSeed();
        for ( difference_type i = n - 1; i > 0; --i )
        {
            const size_t j = kvs::Value<size_t>::Random( 0, i );
            std::swap( first[i], first[j] );
        }
    }

    ValueArray<size_t> argsort( const bool ascending = true )
    {
        // if ( C++11 )
        // {
        //     // in case of ascending order
        //     this_type& array = *this;
        //     std::sort( indices.begin(), indices.end(),
        //                [&array]( size_t i, size_t j ) { return array[i] < array[j]; } );
        // }
        ValueArray<size_t> indices = ValueArray<size_t>::Linear( this->size() );
        if ( ascending )
        {
            std::sort( indices.begin(), indices.end(), argsort_less( this->begin() ) );
        }
        else
        {
            std::sort( indices.begin(), indices.end(), argsort_greater( this->begin() ) );
        }
        return indices;
    }

private:
    struct argsort_less
    {
        const_iterator m_begin; argsort_less( const_iterator begin ): m_begin( begin ) {}
        bool operator () ( size_t i, size_t j ) const { return m_begin[i] < m_begin[j]; }
    };

    struct argsort_greater
    {
        const_iterator m_begin; argsort_greater( const_iterator begin ): m_begin( begin ) {}
        bool operator () ( size_t i, size_t j ) const { return m_begin[i] > m_begin[j]; }
    };
};

} // end of namespace kvs

namespace std
{

template <typename T>
inline void swap( kvs::ValueArray<T>& lhs, kvs::ValueArray<T>& rhs )
{
    lhs.swap( rhs );
}

} // end of namespace std
