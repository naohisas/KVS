/****************************************************************************/
/**
 *  @file BitArray.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BitArray.h 1233 2012-07-04 13:56:48Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BIT_ARRAY_H_INCLUDE
#define KVS__BIT_ARRAY_H_INCLUDE

#if KVS_ENABLE_DEPRECATED
#include <iostream>
#include <vector>
#endif
#include <kvs/Type>
#include <kvs/ValueArray>


namespace kvs
{

/*==========================================================================*/
/**
 *  Bit array class
 */
/*==========================================================================*/
class BitArray
{
private:
    size_t                      m_size;   ///< number of values
    kvs::ValueArray<kvs::UInt8> m_values; ///< value array (bit array)

public:
    BitArray();
    BitArray( const size_t nvalues );
    BitArray( const kvs::UInt8* values, const size_t nvalues );
    BitArray( const bool* values, const size_t nvalues );

public:
    bool operator [] ( size_t index ) const;

    BitArray& operator &= ( const BitArray& other );
    BitArray& operator |= ( const BitArray& other );
    BitArray& operator ^= ( const BitArray& other );

public:
    void set();
    void set( size_t index );

    void reset();
    void reset( size_t index );

    void flip();
    void flip( size_t index );

public:
    size_t count() const;
    bool test( size_t index ) const;

public:
    size_t size() const;
    size_t byteSize() const;
    size_t bitSize() const;
    size_t paddingBit() const;

public:
#if !KVS_ENABLE_DEPRECATED
    void allocate( size_t nvalues );
#endif
    void release();
    void swap( BitArray& other );

    ValueArray<kvs::UInt8> asValueArray() const;
    BitArray clone() const;

    const kvs::UInt8* data() const;
    kvs::UInt8* data();

#if KVS_ENABLE_DEPRECATED
    kvs::UInt8* allocate( size_t nvalues );

    const void* pointer() const
    {
        return m_values.data();
    }

    void* pointer()
    {
        return m_values.data();
    }

    void shallowCopy( const BitArray& other )
    {
        *this = other;
    }

    void deepCopy( const BitArray& other )
    {
        *this = other.clone();
    }

    void deepCopy( const kvs::UInt8* values, const size_t nvalues )
    {
        *this = BitArray( values, nvalues );
    }

    void deepCopy( const bool* values, const size_t nvalues )
    {
        *this = BitArray( values, nvalues );
    }

    void deallocate()
    {
        this->release();
    }
#endif
};

} // end of namespace kvs

#endif // KVS__BIT_ARRAY_H_INCLUDE
