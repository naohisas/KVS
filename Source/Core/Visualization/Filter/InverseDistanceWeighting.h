/*****************************************************************************/
/**
 *  @file   InverseDistanceWeighting.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once

#include <vector>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Matrix33>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Inverse distance weighting calculation class
 */
/*===========================================================================*/
template <typename Value>
class InverseDistanceWeighting
{
public:

    typedef std::pair<Value,kvs::Real32> Pair;
    typedef std::vector<std::vector<Pair> > Bucket;

private:

    Bucket m_bucket;

public:

    InverseDistanceWeighting( const size_t nnodes ) { m_bucket.resize( nnodes ); }

    const Bucket& bucket() const { return m_bucket; }

    void insert( const kvs::UInt32 index, const Value value, const kvs::Real32 distance )
    {
        m_bucket[ index ].push_back( std::make_pair( value, distance ) );
    }

    kvs::ValueArray<kvs::Real32> serialize() const
    {
        // Specialized for
        //    scalar value as kvs::Real32
        //    vector value as kvs::Vec3
        //    tensor value as kvs::Mat3
        return kvs::ValueArray<kvs::Real32>( m_bucket.size() );
    }
};

template <>
inline kvs::ValueArray<kvs::Real32> InverseDistanceWeighting<kvs::Real32>::serialize() const
{
    typedef kvs::Real32 Scalar;
    const size_t veclen = 1;

    // Inverse distance weighting.
    const size_t nnodes = m_bucket.size();
    kvs::ValueArray<kvs::Real32> values( nnodes * veclen );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const size_t n = m_bucket.at(i).size();
        //if ( n == 0 ) { continue; }

        float w = 0.0f;
        for ( size_t j = 0; j < n; j++ )
        {
            const float d = m_bucket.at(i).at(j).second;
            w += 1.0f / d;
        }

        Scalar value = 0.0f;
        for ( size_t j = 0; j < n; j++ )
        {
            const Scalar s = m_bucket.at(i).at(j).first;
            const kvs::Real32 d = m_bucket.at(i).at(j).second;
            value += ( ( 1.0f / d ) / w ) * s;
        }
        value /= n;

        values[i] = value;
    }

    return values;
}

template <>
inline kvs::ValueArray<kvs::Real32> InverseDistanceWeighting<kvs::Vec3>::serialize() const
{
    typedef kvs::Vec3 Vector;
    const size_t veclen = 3;

    // Inverse distance weighting.
    const size_t nnodes = m_bucket.size();
    kvs::ValueArray<kvs::Real32> values( nnodes * veclen );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const size_t n = m_bucket.at(i).size();
        //if ( n == 0 ) { continue; }

        float w = 0.0f;
        for ( size_t j = 0; j < n; j++ )
        {
            const float d = m_bucket.at(i).at(j).second;
            w += 1.0f / d;
        }

        Vector value = Vector::Zero();
        for ( size_t j = 0; j < n; j++ )
        {
            const Vector v = m_bucket.at(i).at(j).first;
            const kvs::Real32 d = m_bucket.at(i).at(j).second;
            value += ( ( 1.0f / d ) / w ) * v;
        }
        value /= n;

        values[ i * veclen + 0 ] = value[0];
        values[ i * veclen + 1 ] = value[1];
        values[ i * veclen + 2 ] = value[2];
    }

    return values;
}

template <>
inline kvs::ValueArray<kvs::Real32> InverseDistanceWeighting<kvs::Mat3>::serialize() const
{
    typedef kvs::Mat3 Tensor;
    const size_t veclen = 9;

    // Inverse distance weighting.
    const size_t nnodes = m_bucket.size();
    kvs::ValueArray<kvs::Real32> values( nnodes * veclen );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const size_t n = m_bucket.at(i).size();
        //if ( n == 0 ) { continue; }

        float w = 0.0f;
        for ( size_t j = 0; j < n; j++ )
        {
            const float d = m_bucket.at(i).at(j).second;
            w += 1.0f / d;
        }

        Tensor value = Tensor::Zero();
        for ( size_t j = 0; j < n; j++ )
        {
            const Tensor t = m_bucket.at(i).at(j).first;
            const kvs::Real32 d = m_bucket.at(i).at(j).second;
            value += ( ( 1.0f / d ) / w ) * t;
        }
        value /= n;

        values[ i * veclen + 0 ] = value[0][0];
        values[ i * veclen + 1 ] = value[0][1];
        values[ i * veclen + 2 ] = value[0][2];
        values[ i * veclen + 3 ] = value[1][0];
        values[ i * veclen + 4 ] = value[1][1];
        values[ i * veclen + 5 ] = value[1][2];
        values[ i * veclen + 6 ] = value[2][0];
        values[ i * veclen + 7 ] = value[2][1];
        values[ i * veclen + 8 ] = value[2][2];
    }

    return values;
}

} // end of namespace kvs
