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
    size_t m_size = 0; ///< number of values
    kvs::ValueArray<kvs::UInt8> m_values{}; ///< value array (bit array)

public:
    BitArray() = default;
    BitArray( const size_t size ) { this->allocate( size ); }
    BitArray( const size_t size, const bool flag );
    BitArray( const kvs::UInt8* values, const size_t size );
    BitArray( const bool* values, const size_t size );

public:
    size_t size() const { return m_size; }
    ValueArray<kvs::UInt8> asValueArray() const { return m_values; }
    const kvs::UInt8* data() const { return m_values.data(); }
    kvs::UInt8* data() { return m_values.data(); }

    void set();
    void set( size_t index );
    void reset();
    void reset( size_t index );
    void flip();
    void flip( size_t index );
    size_t count() const;
    bool test( size_t index ) const;
    size_t byteSize() const;
    size_t bitSize() const;
    size_t paddingBit() const;

    void allocate( size_t nvalues );
    void release();
    void swap( BitArray& other );
    BitArray clone() const;

    bool operator [] ( size_t index ) const;
    BitArray& operator &= ( const BitArray& other );
    BitArray& operator |= ( const BitArray& other );
    BitArray& operator ^= ( const BitArray& other );
};

} // end of namespace kvs
