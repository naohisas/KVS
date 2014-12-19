/****************************************************************************/
/**
 *  @file BitArray.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BitArray.cpp 1298 2012-08-28 12:49:02Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "BitArray.h"
#include <algorithm>
#include <utility>
#include <kvs/Type>
#include <kvs/Assert>
#include <kvs/Binary>


namespace
{

const kvs::UInt8 Zeros = 0;
const kvs::UInt8 Ones = kvs::UInt8( -1 );

const kvs::UInt8 SetBitMask[8] =
{
    kvsBinary8( 1000, 0000 ),
    kvsBinary8( 0100, 0000 ),
    kvsBinary8( 0010, 0000 ),
    kvsBinary8( 0001, 0000 ),
    kvsBinary8( 0000, 1000 ),
    kvsBinary8( 0000, 0100 ),
    kvsBinary8( 0000, 0010 ),
    kvsBinary8( 0000, 0001 ),
};

const kvs::UInt8 ResetBitMask[8] =
{
    kvsBinary8( 0111, 1111 ),
    kvsBinary8( 1011, 1111 ),
    kvsBinary8( 1101, 1111 ),
    kvsBinary8( 1110, 1111 ),
    kvsBinary8( 1111, 0111 ),
    kvsBinary8( 1111, 1011 ),
    kvsBinary8( 1111, 1101 ),
    kvsBinary8( 1111, 1110 ),
};

size_t BitToByte( size_t nbits )
{
    return nbits >> 3;
}

size_t ByteToBit( size_t nbytes )
{
    return nbytes << 3;
}

size_t CountBits( kvs::UInt8 val )
{
    //val = ( val & 0x55 ) + ( val >> 1 & 0x55 );
    //val = ( val & 0x33 ) + ( val >> 2 & 0x33 );
    //val = ( val & 0x0F ) + ( val >> 4 & 0x0F );
    //return val;

    val = val - ( ( val >> 1 ) & 0x55 );
    val = ( val & 0x33 ) + ( ( val >> 2 ) & 0x33 );
    val = ( val + ( val >> 4 ) ) & 0x0F;
    return val;
}

#if 0
size_t CountBits( kvs::UInt32 val )
{
    //val = ( val & 0x55555555 ) + ( val >>  1 & 0x55555555 );
    //val = ( val & 0x33333333 ) + ( val >>  2 & 0x33333333 );
    //val = ( val & 0x0F0F0F0F ) + ( val >>  4 & 0x0F0F0F0F );
    //val = ( val & 0x00FF00FF ) + ( val >>  8 & 0x00FF00FF );
    //val = ( val & 0x0000FFFF ) + ( val >> 16 & 0x0000FFFF );
    //return val;

    val = val - ( ( val >>> 1 ) & 0x55555555 );
    val = ( val & 0x33333333 ) + ( ( val >>  2 ) & 0x33333333 );
    val = ( val + ( val >> 4 ) ) & 0x0F0F0F0F;
    val = val + ( val >> 8 );
    val = val + ( val >> 16 );
    return val & 0xFF;
}

size_t CountBits( kvs::UInt64 val )
{
    //val = ( val & 0x5555555555555555 ) + ( val >>  1 & 0x5555555555555555 );
    //val = ( val & 0x3333333333333333 ) + ( val >>  2 & 0x3333333333333333 );
    //val = ( val & 0x0F0F0F0F0F0F0F0F ) + ( val >>  4 & 0x0F0F0F0F0F0F0F0F );
    //val = ( val & 0x00FF00FF00FF00FF ) + ( val >>  8 & 0x00FF00FF00FF00FF );
    //val = ( val & 0x0000FFFF0000FFFF ) + ( val >> 16 & 0x0000FFFF0000FFFF );
    //val = ( val & 0x00000000FFFFFFFF ) + ( val >> 32 & 0x00000000FFFFFFFF );
    //return val;

    return CountBits( (kvs::UInt32)( val >> 32 ) ) + CountBits( (kvs::UInt32)( val ) );

    //val = val - ( ( val >> 1 ) & 0x5555555555555555 );
    //val = ( val & 0x3333333333333333 ) + ( ( val >>  2 ) & 0x3333333333333333 );
    //val = ( val + ( val >> 4 ) ) & 0x0F0F0F0F0F0F0F0F;
    //val = val + ( val >> 8 );
    //val = val + ( val >> 16 );
    //val = val + ( val >> 32 );
    //return val & 0xFF;
}
#endif

} // end of namespace


namespace kvs
{

BitArray::BitArray()
{
    m_size = 0;
}

BitArray::BitArray( size_t size )
{
    this->allocate( size );
}

BitArray::BitArray( const kvs::UInt8* values, const size_t size )
{
    m_size = size;
    m_values.allocate( this->byteSize() );
    std::copy( values, values + this->byteSize(), m_values.data() );
}

BitArray::BitArray( const bool* values, const size_t size )
{
    this->allocate( size );
    for ( size_t index = 0; index < size; index++ )
    {
        if ( values[ index ] ) this->set( index );
        else                   this->reset( index );
    }
}

bool BitArray::operator [] ( size_t index ) const
{
    KVS_ASSERT( index < this->size() );
    return ( m_values[ ::BitToByte( index ) ] & ::SetBitMask[ index % 8 ] ) != 0;
}

BitArray& BitArray::operator &= ( const BitArray& other ) // AND
{
    KVS_ASSERT( this->size() == other.size() );
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] &= other.m_values[ index ];
    }
    return *this;
}

BitArray& BitArray::operator |= ( const BitArray& other ) // OR
{
    KVS_ASSERT( this->size() == other.size() );
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] |= other.m_values[ index ];
    }
    return *this;
}

BitArray& BitArray::operator ^= ( const BitArray& other ) // XOR
{
    KVS_ASSERT( this->size() == other.size() );
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] ^= other.m_values[ index ];
    }
    return *this;
}

// set all 1 to table
void BitArray::set()
{
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] = ::Ones;
    }
}

// set true the "bit" position of table
void BitArray::set( size_t index )
{
    KVS_ASSERT( index < this->size() );
    m_values[ ::BitToByte( index ) ] |= ::SetBitMask[ index % 8 ];
}

// set all 0 to table
void BitArray::reset()
{
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] = ::Zeros;
    }
}

// set false the "bit" position of table
void BitArray::reset( size_t index )
{
    KVS_ASSERT( index < this->size() );
    m_values[ ::BitToByte( index ) ] &= ::ResetBitMask[ index % 8 ];
}

// reverse all value of table
void BitArray::flip()
{
    const size_t size = m_values.size();
    for ( size_t index = 0; index < size; index++ )
    {
        m_values[ index ] ^= ::Ones;
    }
}

// reverse the value of "bit" position
void BitArray::flip( size_t index )
{
    KVS_ASSERT( index < this->size() );
    m_values[ ::BitToByte( index ) ] ^= ::SetBitMask[ index % 8 ];
}

// count the true value
size_t BitArray::count() const
{
    if ( m_values.empty() )
        return 0;

    size_t ret = 0;
    size_t index = 0;
    const size_t size = m_values.size() - 1;
    for ( ; index < size; index++ )
    {
        ret += ::CountBits( m_values[ index ] );
    }
    ret += ::CountBits( m_values[ index ] >> this->paddingBit() );
    return ret;
}

// return the "bit" position value
bool BitArray::test( size_t index ) const
{
    return (*this)[ index ];
}

size_t BitArray::size() const
{
    return m_size;
}

size_t BitArray::byteSize() const
{
    return ::BitToByte( this->size() + 7 );
}

size_t BitArray::bitSize() const
{
    return ::ByteToBit( this->byteSize() );
}

size_t BitArray::paddingBit() const
{
    return this->bitSize() - this->size();
}

#if KVS_ENABLE_DEPRECATED
kvs::UInt8* BitArray::allocate( size_t size )
{
    m_size = size;
    m_values.allocate( this->byteSize() );
    return this->data();
}

#else
void BitArray::allocate( size_t size )
{
    m_size = size;
    m_values.allocate( this->byteSize() );
}
#endif

void BitArray::release()
{
    m_values.release();
    m_size = 0;
}

void BitArray::swap( BitArray& other )
{
    std::swap( m_size, other.m_size );
    std::swap( m_values, other.m_values );
}

ValueArray<kvs::UInt8> BitArray::asValueArray() const
{
    return m_values;
}

BitArray BitArray::clone() const
{
    BitArray ret;
    ret.m_size = m_size;
    ret.m_values = m_values.clone();
    return ret;
}

const kvs::UInt8* BitArray::data() const
{
    return m_values.data();
}

kvs::UInt8* BitArray::data()
{
    return m_values.data();
}

} // end of namespace kvs
