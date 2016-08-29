/****************************************************************************/
/**
 *  @file   TrilinearInterpolator.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TrilinearInterpolator.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TRILINEAR_INTERPOLATOR_H_INCLUDE
#define KVS__TRILINEAR_INTERPOLATOR_H_INCLUDE

#include <kvs/StructuredVolumeObject>
#include <kvs/Vector3>
#include <kvs/Assert>
#include <cstring>


namespace kvs
{

/*==========================================================================*/
/**
 *  Trilinear interpolation class.
 */
/*==========================================================================*/
class TrilinearInterpolator
{
private:

    kvs::Vector3ui m_grid_index; ///< grid index
    kvs::UInt32 m_index[8]; ///< neighbouring grid index
    kvs::Real32 m_weight[8]; ///< weight for the neighbouring grid index

    const kvs::StructuredVolumeObject* m_reference_volume; ///< reference irregular volume data

public:

    TrilinearInterpolator( const kvs::StructuredVolumeObject* volume );

    void attachPoint( const kvs::Vector3f& point );
    const kvs::UInt32* indices( void ) const;
    template <typename T>
    kvs::Real32 scalar( void ) const;
    template <typename T>
    kvs::Vec3 gradient( void ) const;
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new TrilinearInterpolator class.
 *  @param  volume [in] pointer the input volume
 */
/*===========================================================================*/
inline TrilinearInterpolator::TrilinearInterpolator( const kvs::StructuredVolumeObject* volume ):
    m_grid_index( 0, 0, 0 ),
    m_reference_volume( volume )
{
    std::memset( m_index, 0x00, sizeof( kvs::UInt32 ) * 8 );
    std::memset( m_weight, 0x00, sizeof( kvs::Real32 ) * 8 );
}

/*===========================================================================*/
/**
 *  @brief  Attach a point.
 *  @param  point [in] point
 */
/*===========================================================================*/
inline void TrilinearInterpolator::attachPoint( const kvs::Vector3f& point )
{
    const kvs::Vector3ui resolution = m_reference_volume->resolution();
    KVS_ASSERT( 0.0f <= point.x() && point.x() <= resolution.x() - 1.0f );
    KVS_ASSERT( 0.0f <= point.y() && point.y() <= resolution.y() - 1.0f );
    KVS_ASSERT( 0.0f <= point.z() && point.z() <= resolution.z() - 1.0f );

    // Temporary index.
    const size_t ti = static_cast<size_t>( point.x() );
    const size_t tj = static_cast<size_t>( point.y() );
    const size_t tk = static_cast<size_t>( point.z() );

    // Addjustment index for boundary.
    const size_t i = ( ti >= resolution.x() - 1 ) ? resolution.x() - 2 : ti;
    const size_t j = ( tj >= resolution.y() - 1 ) ? resolution.y() - 2 : tj;
    const size_t k = ( tk >= resolution.z() - 1 ) ? resolution.z() - 2 : tk;

    const size_t line_size  = m_reference_volume->numberOfNodesPerLine();
    const size_t slice_size = m_reference_volume->numberOfNodesPerSlice();

    // Calculate index.
    m_grid_index.set( i, j, k );

    m_index[0] = i + j * line_size + k * slice_size;
    m_index[1] = m_index[0] + 1;
    m_index[2] = m_index[1] + line_size;
    m_index[3] = m_index[0] + line_size;
    m_index[4] = m_index[0] + slice_size;
    m_index[5] = m_index[1] + slice_size;
    m_index[6] = m_index[2] + slice_size;
    m_index[7] = m_index[3] + slice_size;

    // Calculate local coordinate.
    const float x = point.x() - i;
    const float y = point.y() - j;
    const float z = point.z() - k;

    const float xy = x * y;
    const float yz = y * z;
    const float zx = z * x;

    const float xyz = xy * z;

    m_weight[0] = 1.0f - x - y - z + xy + yz + zx - xyz;
    m_weight[1] = x - xy - zx + xyz;
    m_weight[2] = xy - xyz;
    m_weight[3] = y - xy - yz + xyz;
    m_weight[4] = z - zx - yz + xyz;
    m_weight[5] = zx - xyz;
    m_weight[6] = xyz;
    m_weight[7] = yz - xyz;
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the index array.
 *  @return pointer to the index array
 */
/*===========================================================================*/
inline const kvs::UInt32* TrilinearInterpolator::indices( void ) const
{
    return m_index;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interpolated scalar.
 *  @return interpolated scalar
 */
/*===========================================================================*/
template <typename T>
inline float TrilinearInterpolator::scalar( void ) const
{
    const T* const data = reinterpret_cast<const T*>( m_reference_volume->values().data() );

    return(
        static_cast<float>(
            data[ m_index[0] ] * m_weight[0] +
            data[ m_index[1] ] * m_weight[1] +
            data[ m_index[2] ] * m_weight[2] +
            data[ m_index[3] ] * m_weight[3] +
            data[ m_index[4] ] * m_weight[4] +
            data[ m_index[5] ] * m_weight[5] +
            data[ m_index[6] ] * m_weight[6] +
            data[ m_index[7] ] * m_weight[7] ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the gradient vector.
 *  @return gradient vector
 */
/*===========================================================================*/
template <typename T>
inline kvs::Vec3 TrilinearInterpolator::gradient( void ) const
{
    // Calculate the point's gradient.
    float dx[8], dy[8], dz[8];

    const T* const data = reinterpret_cast<const T*>( m_reference_volume->values().data() );

    const kvs::Vector3ui resolution = m_reference_volume->resolution();
    const size_t line_size  = m_reference_volume->numberOfNodesPerLine();
    const size_t slice_size = m_reference_volume->numberOfNodesPerSlice();

    const size_t i = m_grid_index.x();
    const size_t j = m_grid_index.y();
    const size_t k = m_grid_index.z();

    if ( i == 0 )
    {
        dx[0] = static_cast<float>( data[ m_index[1]     ] );
        dx[1] = static_cast<float>( data[ m_index[1] + 1 ] ) - static_cast<float>( data[ m_index[0]     ] );
        dx[2] = static_cast<float>( data[ m_index[2] + 1 ] ) - static_cast<float>( data[ m_index[3]     ] );
        dx[3] = static_cast<float>( data[ m_index[2]     ] );
        dx[4] = static_cast<float>( data[ m_index[5]     ] );
        dx[5] = static_cast<float>( data[ m_index[5] + 1 ] ) - static_cast<float>( data[ m_index[4]     ] );
        dx[6] = static_cast<float>( data[ m_index[6] + 1 ] ) - static_cast<float>( data[ m_index[7]     ] );
        dx[7] = static_cast<float>( data[ m_index[6]     ] );
    }
    else if ( i == resolution.x() - 2 )
    {
        dx[0] = static_cast<float>( data[ m_index[1]     ] ) - static_cast<float>( data[ m_index[0] - 1 ] );
        dx[1] =                                              - static_cast<float>( data[ m_index[0]     ] );
        dx[2] =                                              - static_cast<float>( data[ m_index[3]     ] );
        dx[3] = static_cast<float>( data[ m_index[2]     ] ) - static_cast<float>( data[ m_index[3] - 1 ] );
        dx[4] = static_cast<float>( data[ m_index[5]     ] ) - static_cast<float>( data[ m_index[4] - 1 ] );
        dx[5] =                                              - static_cast<float>( data[ m_index[4]     ] );
        dx[6] =                                              - static_cast<float>( data[ m_index[7]     ] );
        dx[7] = static_cast<float>( data[ m_index[6]     ] ) - static_cast<float>( data[ m_index[7] - 1 ] );
    }
    else
    {
        dx[0] = static_cast<float>( data[ m_index[1]     ] ) - static_cast<float>( data[ m_index[0] - 1 ] );
        dx[1] = static_cast<float>( data[ m_index[1] + 1 ] ) - static_cast<float>( data[ m_index[0]     ] );
        dx[2] = static_cast<float>( data[ m_index[2] + 1 ] ) - static_cast<float>( data[ m_index[3]     ] );
        dx[3] = static_cast<float>( data[ m_index[2]     ] ) - static_cast<float>( data[ m_index[3] - 1 ] );
        dx[4] = static_cast<float>( data[ m_index[5]     ] ) - static_cast<float>( data[ m_index[4] - 1 ] );
        dx[5] = static_cast<float>( data[ m_index[5] + 1 ] ) - static_cast<float>( data[ m_index[4]     ] );
        dx[6] = static_cast<float>( data[ m_index[6] + 1 ] ) - static_cast<float>( data[ m_index[7]     ] );
        dx[7] = static_cast<float>( data[ m_index[6]     ] ) - static_cast<float>( data[ m_index[7] - 1 ] );
    }

    if ( j == 0 )
    {
        dy[0] = static_cast<float>( data[ m_index[3]             ] );
        dy[1] = static_cast<float>( data[ m_index[2]             ] );
        dy[2] = static_cast<float>( data[ m_index[2] + line_size ] ) - static_cast<float>( data[ m_index[1]             ] );
        dy[3] = static_cast<float>( data[ m_index[3] + line_size ] ) - static_cast<float>( data[ m_index[0]             ] );
        dy[4] = static_cast<float>( data[ m_index[7]             ] );
        dy[5] = static_cast<float>( data[ m_index[6]             ] );
        dy[6] = static_cast<float>( data[ m_index[6] + line_size ] ) - static_cast<float>( data[ m_index[5]             ] );
        dy[7] = static_cast<float>( data[ m_index[7] + line_size ] ) - static_cast<float>( data[ m_index[4]             ] );
    }
    else if ( j == resolution.y() - 2 )
    {
        dy[0] = static_cast<float>( data[ m_index[3]             ] ) - static_cast<float>( data[ m_index[0] - line_size ] );
        dy[1] = static_cast<float>( data[ m_index[2]             ] ) - static_cast<float>( data[ m_index[1] - line_size ] );
        dy[2] =                                                      - static_cast<float>( data[ m_index[1]             ] );
        dy[3] =                                                      - static_cast<float>( data[ m_index[0]             ] );
        dy[4] = static_cast<float>( data[ m_index[7]             ] ) - static_cast<float>( data[ m_index[4] - line_size ] );
        dy[5] = static_cast<float>( data[ m_index[6]             ] ) - static_cast<float>( data[ m_index[5] - line_size ] );
        dy[6] =                                                      - static_cast<float>( data[ m_index[5]             ] );
        dy[7] =                                                      - static_cast<float>( data[ m_index[4]             ] );
    }
    else
    {
        dy[0] = static_cast<float>( data[ m_index[3]             ] ) - static_cast<float>( data[ m_index[0] - line_size ] );
        dy[1] = static_cast<float>( data[ m_index[2]             ] ) - static_cast<float>( data[ m_index[1] - line_size ] );
        dy[2] = static_cast<float>( data[ m_index[2] + line_size ] ) - static_cast<float>( data[ m_index[1]             ] );
        dy[3] = static_cast<float>( data[ m_index[3] + line_size ] ) - static_cast<float>( data[ m_index[0]             ] );
        dy[4] = static_cast<float>( data[ m_index[7]             ] ) - static_cast<float>( data[ m_index[4] - line_size ] );
        dy[5] = static_cast<float>( data[ m_index[6]             ] ) - static_cast<float>( data[ m_index[5] - line_size ] );
        dy[6] = static_cast<float>( data[ m_index[6] + line_size ] ) - static_cast<float>( data[ m_index[5]             ] );
        dy[7] = static_cast<float>( data[ m_index[7] + line_size ] ) - static_cast<float>( data[ m_index[4]             ] );
    }

    if ( k == 0 )
    {
        dz[0] = static_cast<float>( data[ m_index[4]              ] );
        dz[1] = static_cast<float>( data[ m_index[5]              ] );
        dz[2] = static_cast<float>( data[ m_index[6]              ] );
        dz[3] = static_cast<float>( data[ m_index[7]              ] );
        dz[4] = static_cast<float>( data[ m_index[4] + slice_size ] ) - static_cast<float>( data[ m_index[0]              ] );
        dz[5] = static_cast<float>( data[ m_index[5] + slice_size ] ) - static_cast<float>( data[ m_index[1]              ] );
        dz[6] = static_cast<float>( data[ m_index[6] + slice_size ] ) - static_cast<float>( data[ m_index[2]              ] );
        dz[7] = static_cast<float>( data[ m_index[7] + slice_size ] ) - static_cast<float>( data[ m_index[3]              ] );
    }
    else if ( k == resolution.z() - 2 )
    {
        dz[0] = static_cast<float>( data[ m_index[4]              ] ) - static_cast<float>( data[ m_index[0] - slice_size ] );
        dz[1] = static_cast<float>( data[ m_index[5]              ] ) - static_cast<float>( data[ m_index[1] - slice_size ] );
        dz[2] = static_cast<float>( data[ m_index[6]              ] ) - static_cast<float>( data[ m_index[2] - slice_size ] );
        dz[3] = static_cast<float>( data[ m_index[7]              ] ) - static_cast<float>( data[ m_index[3] - slice_size ] );
        dz[4] =                                                       - static_cast<float>( data[ m_index[0]              ] );
        dz[5] =                                                       - static_cast<float>( data[ m_index[1]              ] );
        dz[6] =                                                       - static_cast<float>( data[ m_index[2]              ] );
        dz[7] =                                                       - static_cast<float>( data[ m_index[3]              ] );
    }
    else
    {
        dz[0] = static_cast<float>( data[ m_index[4]              ] ) - static_cast<float>( data[ m_index[0] - slice_size ] );
        dz[1] = static_cast<float>( data[ m_index[5]              ] ) - static_cast<float>( data[ m_index[1] - slice_size ] );
        dz[2] = static_cast<float>( data[ m_index[6]              ] ) - static_cast<float>( data[ m_index[2] - slice_size ] );
        dz[3] = static_cast<float>( data[ m_index[7]              ] ) - static_cast<float>( data[ m_index[3] - slice_size ] );
        dz[4] = static_cast<float>( data[ m_index[4] + slice_size ] ) - static_cast<float>( data[ m_index[0]              ] );
        dz[5] = static_cast<float>( data[ m_index[5] + slice_size ] ) - static_cast<float>( data[ m_index[1]              ] );
        dz[6] = static_cast<float>( data[ m_index[6] + slice_size ] ) - static_cast<float>( data[ m_index[2]              ] );
        dz[7] = static_cast<float>( data[ m_index[7] + slice_size ] ) - static_cast<float>( data[ m_index[3]              ] );
    }

    const float x =
        dx[0] * m_weight[0] +
        dx[1] * m_weight[1] +
        dx[2] * m_weight[2] +
        dx[3] * m_weight[3] +
        dx[4] * m_weight[4] +
        dx[5] * m_weight[5] +
        dx[6] * m_weight[6] +
        dx[7] * m_weight[7];

    const float y =
        dy[0] * m_weight[0] +
        dy[1] * m_weight[1] +
        dy[2] * m_weight[2] +
        dy[3] * m_weight[3] +
        dy[4] * m_weight[4] +
        dy[5] * m_weight[5] +
        dy[6] * m_weight[6] +
        dy[7] * m_weight[7];

    const float z =
        dz[0] * m_weight[0] +
        dz[1] * m_weight[1] +
        dz[2] * m_weight[2] +
        dz[3] * m_weight[3] +
        dz[4] * m_weight[4] +
        dz[5] * m_weight[5] +
        dz[6] * m_weight[6] +
        dz[7] * m_weight[7];

//    return( kvs::Vector3f( x, y, z ) );
    return( kvs::Vector3f( -x, -y, -z ) );
}

} // end of namespace kvs

#endif // KVS__TRILINEAR_INTERPOLATOR_H_INCLUDE
