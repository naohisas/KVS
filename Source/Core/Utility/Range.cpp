#include "Range.h"
#include <cmath>
#include <kvs/Assert>
#include <kvs/Value>
#include <kvs/Type>


namespace
{

template <typename T>
inline T Square( T a )
{
    return a * a;
}

}

namespace kvs
{

template <typename T>
kvs::Range CalculateValueRange( const kvs::ValueArray<T>& ary, int dim )
{
    KVS_ASSERT( dim > 0 );
    KVS_ASSERT( ary.size() % dim == 0 );

    const T* value = ary.data();
    const T* end   = value + ary.size();

    kvs::Range range;
    if ( dim == 1 )
    {
        while ( value < end )
        {
            range.extend( double( *( value++ ) ) );
        }
    }
    else if ( dim == 2 )
    {
        while ( value < end )
        {
            double magnitude
                = ::Square( double( value[0] ) )
                + ::Square( double( value[1] ) );
            value += 2;
            range.extend( magnitude );
        }
    }
    else if ( dim == 3 )
    {
        while ( value < end )
        {
            double magnitude
                = ::Square( double( value[0] ) )
                + ::Square( double( value[1] ) )
                + ::Square( double( value[2] ) );
            value += 3;
            range.extend( magnitude );
        }
    }
    else
    {
        while ( value < end )
        {
            double magnitude = 0;
            for ( int i = 0; i < dim; ++i )
            {
                magnitude += ::Square( double( *( value++ ) ) );
            }
            range.extend( magnitude );
        }
    }
    if ( dim != 1 )
        range = kvs::Range( std::sqrt( range.lower() ), std::sqrt( range.upper() ) );
    return range;
}

template kvs::Range CalculateValueRange<kvs::Int8  >( const kvs::ValueArray<kvs::Int8  >& ary, int dim );
template kvs::Range CalculateValueRange<kvs::Int16 >( const kvs::ValueArray<kvs::Int16 >& ary, int dim );
template kvs::Range CalculateValueRange<kvs::Int32 >( const kvs::ValueArray<kvs::Int32 >& ary, int dim );
template kvs::Range CalculateValueRange<kvs::Int64 >( const kvs::ValueArray<kvs::Int64 >& ary, int dim );
template kvs::Range CalculateValueRange<kvs::UInt8 >( const kvs::ValueArray<kvs::UInt8 >& ary, int dim );
template kvs::Range CalculateValueRange<kvs::UInt16>( const kvs::ValueArray<kvs::UInt16>& ary, int dim );
template kvs::Range CalculateValueRange<kvs::UInt32>( const kvs::ValueArray<kvs::UInt32>& ary, int dim );
template kvs::Range CalculateValueRange<kvs::UInt64>( const kvs::ValueArray<kvs::UInt64>& ary, int dim );
template kvs::Range CalculateValueRange<kvs::Real32>( const kvs::ValueArray<kvs::Real32>& ary, int dim );
template kvs::Range CalculateValueRange<kvs::Real64>( const kvs::ValueArray<kvs::Real64>& ary, int dim );

kvs::Range CalculateValueRange( const kvs::AnyValueArray& ary, int dim )
{
    switch ( ary.typeID() )
    {
    case kvs::Type::TypeInt8:
        return CalculateValueRange( ary.asValueArray<kvs::Int8>(), dim );
    case kvs::Type::TypeInt16:
        return CalculateValueRange( ary.asValueArray<kvs::Int16>(), dim );
    case kvs::Type::TypeInt32:
        return CalculateValueRange( ary.asValueArray<kvs::Int32>(), dim );
    case kvs::Type::TypeInt64:
        return CalculateValueRange( ary.asValueArray<kvs::Int64>(), dim );
    case kvs::Type::TypeUInt8:
        return CalculateValueRange( ary.asValueArray<kvs::UInt8>(), dim );
    case kvs::Type::TypeUInt16:
        return CalculateValueRange( ary.asValueArray<kvs::UInt16>(), dim );
    case kvs::Type::TypeUInt32:
        return CalculateValueRange( ary.asValueArray<kvs::UInt32>(), dim );
    case kvs::Type::TypeUInt64:
        return CalculateValueRange( ary.asValueArray<kvs::UInt64>(), dim );
    case kvs::Type::TypeReal32:
        return CalculateValueRange( ary.asValueArray<kvs::Real32>(), dim );
    case kvs::Type::TypeReal64:
        return CalculateValueRange( ary.asValueArray<kvs::Real64>(), dim );
    default:
        KVS_ASSERT( false );
        return Range();
    }
}

} // end of namespace kvs
