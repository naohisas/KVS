/*****************************************************************************/
/**
 *  @file   FieldSimilarity.cpp
 *  @author Go Tamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  A. Fofonov, L. Linsen, Projected Field Similarity for Comparative Visualization
 *  of Multi-Run Multi-Field Time-Varying Spatial Data, Computer Graphics Forum,
 *  Volume 38, Issue 1, pp.286-299, 2019.
 */
/*****************************************************************************/
#include "FieldSimilarity.h"
#include <kvs/Assert>


namespace
{

template <typename T>
inline float Similarity(
    const kvs::ValueArray<T>& values0,
    const kvs::ValueArray<T>& values1,
    const float min_value,
    const float max_value )
{
    const auto f = 1.0f / ( max_value - min_value );
    auto normalize = [&] ( const float value )
    {
        return ( value - min_value ) * f;
    };

    float min = 0.0f;
    float max = 0.0f;
    const size_t size = values0.size();

    #pragma omp parallel for reduction(+:min,max)
    for ( size_t i = 0; i < size; ++i )
    {
        const auto v0 = normalize( static_cast<float>( values0[i] ) );
        const auto v1 = normalize( static_cast<float>( values1[i] ) );
        min += 1.0f - std::min( v0, v1 );
        max += 1.0f - std::max( v0, v1 );
    }

    return max / min;
}

} // end of namespace

namespace kvs
{

float FieldSimilarity(
    const kvs::VolumeObjectBase& volume0,
    const kvs::VolumeObjectBase& volume1 )
{
    if ( !volume0.hasMinMaxValues() ) { volume0.updateMinMaxValues(); }
    if ( !volume1.hasMinMaxValues() ) { volume1.updateMinMaxValues(); }

    const float min_value = std::min( volume0.minValue(), volume1.minValue() );
    const float max_value = std::max( volume0.maxValue(), volume1.maxValue() );
    return FieldSimilarity( volume0, volume1, min_value, max_value );
}

float FieldSimilarity(
    const kvs::VolumeObjectBase& volume0,
    const kvs::VolumeObjectBase& volume1,
    const float min_value,
    const float max_value )
{
    KVS_ASSERT( volume0.volumeType() == volume1.volumeType() );
    KVS_ASSERT( volume0.values().size() == volume1.values().size() );
    KVS_ASSERT( volume0.values().typeID() == volume1.values().typeID() );

    const auto values0 = volume0.values();
    const auto values1 = volume1.values();
    switch ( volume0.values().typeID() )
    {
    case kvs::Type::TypeInt8:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Int8>(),
            values0.asValueArray<kvs::Int8>(),
            min_value, max_value );
    }
    case kvs::Type::TypeInt16:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Int16>(),
            values0.asValueArray<kvs::Int16>(),
            min_value, max_value );
    }
    case kvs::Type::TypeInt32:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Int32>(),
            values0.asValueArray<kvs::Int32>(),
            min_value, max_value );
    }
    case kvs::Type::TypeInt64:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Int64>(),
            values0.asValueArray<kvs::Int64>(),
            min_value, max_value );
    }
    case kvs::Type::TypeUInt8:
    {
        return ::Similarity(
            values0.asValueArray<kvs::UInt8>(),
            values0.asValueArray<kvs::UInt8>(),
            min_value, max_value );
    }
    case kvs::Type::TypeUInt16:
    {
        return ::Similarity(
            values0.asValueArray<kvs::UInt16>(),
            values0.asValueArray<kvs::UInt16>(),
            min_value, max_value );
    }
    case kvs::Type::TypeUInt32:
    {
        return ::Similarity(
            values0.asValueArray<kvs::UInt32>(),
            values0.asValueArray<kvs::UInt32>(),
            min_value, max_value );
    }
    case kvs::Type::TypeUInt64:
    {
        return ::Similarity(
            values0.asValueArray<kvs::UInt64>(),
            values0.asValueArray<kvs::UInt64>(),
            min_value, max_value );
    }
    case kvs::Type::TypeReal32:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Real32>(),
            values0.asValueArray<kvs::Real32>(),
            min_value, max_value );
    }
    case kvs::Type::TypeReal64:
    {
        return ::Similarity(
            values0.asValueArray<kvs::Real64>(),
            values0.asValueArray<kvs::Real64>(),
            min_value, max_value );
    }
    default: break;
    }

    return 0.0f;
}

} // end of namespace kvs
