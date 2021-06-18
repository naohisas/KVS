/****************************************************************************/
/**
 *  @file   BitArray.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    size_t m_size; ///< number of values
    kvs::ValueArray<kvs::UInt8> m_values; ///< value array (bit array)

public:
    BitArray();
    BitArray( const size_t size );
    BitArray( const size_t size, const bool flag );
    BitArray( const kvs::UInt8* values, const size_t size );
    BitArray( const bool* values, const size_t size );

public:
    bool operator [] ( size_t index ) const;
    BitArray& operator &= ( const BitArray& other );
    BitArray& operator |= ( const BitArray& other );
    BitArray& operator ^= ( const BitArray& other );

    void set();
    void set( size_t index );
    void reset();
    void reset( size_t index );
    void flip();
    void flip( size_t index );
    size_t count() const;
    bool test( size_t index ) const;
    size_t size() const;
    size_t byteSize() const;
    size_t bitSize() const;
    size_t paddingBit() const;

    void allocate( size_t nvalues );
    void release();
    void swap( BitArray& other );

    ValueArray<kvs::UInt8> asValueArray() const;
    BitArray clone() const;

    const kvs::UInt8* data() const;
    kvs::UInt8* data();
};

} // end of namespace kvs
