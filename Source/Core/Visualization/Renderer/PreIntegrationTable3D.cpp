/*****************************************************************************/
/**
 *  @file   PreIntegrationTable3D.cpp
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
/*----------------------------------------------------------------------------
 *
 * Acknowledgements:
 * Original source code was developped by University of Utah and can be
 * downloaded at http://havs.sourceforge.net/
 */
/*****************************************************************************/
#include "PreIntegrationTable3D.h"
#include <vector>
#include <kvs/Math>
#include <kvs/ValueArray>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns nomralized color as kvs::Vec4.
 *  @param  color [in] RGB color
 *  @param  opacity [in] opacity
 *  @return normalized color
 */
/*===========================================================================*/
inline kvs::Vec4 ColorVec4( const kvs::RGBColor& color, const float opacity )
{
    const float r = static_cast<float>( color.r() ) / 255.0f;
    const float g = static_cast<float>( color.g() ) / 255.0f;
    const float b = static_cast<float>( color.b() ) / 255.0f;
    return kvs::Vec4( r, g, b, opacity );
}

/*===========================================================================*/
/**
 *  @brief  Returns index of the specified scalar in the scalar array.
 *  @param  s [in] scalar value
 *  @param  scalars [in] scalar array
 *  @return index
 */
/*===========================================================================*/
inline size_t Index( const float s, const std::vector<float>& scalars )
{
    size_t index = 0;

    float s0 = scalars[0];
    float s1 = s0;
    for ( index = 1; index < scalars.size(); index++ )
    {
        s1 = scalars[index];
        if ( s0 < s && s <= s1 ) break;

        s0 = s1;
    }

    return index;
}

/*===========================================================================*/
/**
 *  @brief  Returns linear interpolated color value between two color values.
 *  @param  c0 [in] color 0
 *  @param  c1 [in] color 1
 *  @param  w [in] value to used to interpolate between c0 and c1 in [0,1]
 *  @return interpolated color
 */
/*===========================================================================*/
inline kvs::Vec4 Interpolate( const kvs::Vec4& c0, const kvs::Vec4& c1, const float w )
{
    return c0 * ( 1.0f - w ) + c1 * w;
}

/*===========================================================================*/
/**
 *  @brief  Returns linear interpolated color value between two color values.
 *  @param  s [in] scalar value between s0 and s1
 *  @param  s0 [in] scalar 0
 *  @param  s1 [in] scalar 1
 *  @param  c0 [in] color 0
 *  @param  c1 [in] color 1
 *  @return interpolated color
 */
/*===========================================================================*/
inline kvs::Vec4 Interpolate(
    const float s,
    const float s0,
    const float s1,
    const kvs::Vec4& c0,
    const kvs::Vec4& c1 )
{
    const float w = ( s - s0 ) / ( s1 - s0 );
    return Interpolate( c0, c1, w );
}

/*===========================================================================*/
/**
 *  @brief  Returns serialized transfer function as value array.
 *  @param  transfer_function [in] transfer function
 *  @param  min_scalar [in] min. scalar
 *  @param  max_scalar [in] max. scalar
 *  @param  resolution [in] resolution of returned transfer function
 *  @return serialized transfer function
 */
/*===========================================================================*/
inline kvs::ValueArray<float> Serialize(
    const kvs::TransferFunction& transfer_function,
    const float min_scalar,
    const float max_scalar,
    const size_t resolution )
{
    const float S0 = min_scalar;
    const float S1 = max_scalar;
    const size_t N = resolution;

    // Convert the KVS transfer function table to floating-point vectors of the color and opacity.
    const size_t size = transfer_function.resolution();
    const kvs::ColorMap& cmap = transfer_function.colorMap();
    const kvs::OpacityMap& omap = transfer_function.opacityMap();
    std::vector<kvs::Vec4> colors;
    std::vector<float> scalars;
    for ( size_t i = 0; i < size; i++ )
    {
        const kvs::Vec4 color = ColorVec4( cmap[i], omap[i] );
        const float w = i / float( size - 1 );
        const float scalar = kvs::Math::Mix( S0, S1, w );

        colors.push_back( color );
        scalars.push_back( scalar );
    }

    // Adjust the boundary of the scalar array.
    const float epsilon = ( S1 - S0 ) / 100.0f;
    scalars.front() = min_scalar - epsilon;
    scalars.back() = max_scalar + epsilon;

    // Create the transfer function by sampling color and opacity ranges.
    kvs::ValueArray<kvs::Real32> array( 4 * N );
    float s = S0;
    const float ds = ( S1 - S0 ) / float( N - 1 );
    for ( size_t i = 0, i4 = 0; i < N; i++, i4 = i4 + 4 )
    {
        const size_t index = Index( s, scalars );
        const float s0 = scalars[ index - 1 ];
        const float s1 = scalars[ index ];
        const kvs::Vec4 c0 = colors[ index - 1 ];
        const kvs::Vec4 c1 = colors[ index ];
        const kvs::Vec4 c = Interpolate( s, s0, s1, c0, c1 );
        array[ i4 + 0 ] = c[0];
        array[ i4 + 1 ] = c[1];
        array[ i4 + 2 ] = c[2];
        array[ i4 + 3 ] = c[3];

        s += ds;
    }

    return array;
}

/*===========================================================================*/
/**
 *  @brief  Returns corrected opacity value.
 *  @param  alpha [in] opacity value
 *  @param  t [in] relative sampling rate
 *  @return corrected opacity value
 */
/*===========================================================================*/
inline float OpacityCorrection( const float alpha, const float t )
{
    // t = dt' / dt
    // dt  : original sampling step
    // dt' : changed sampling step
    return 1.0f - std::pow( 1.0f - alpha, t );
}

/*===========================================================================*/
/**
 *  @brief  Returns opacity-weighted color.
 *  @param  color [in] color value
 *  @param  t [in] relative sampling rate for alpha correction
 *  @return opacity-weighted color
 */
/*===========================================================================*/
inline kvs::Vec4 OpacityWeightedColor( const kvs::Vec4& color, const float t )
{
    const float a = OpacityCorrection( color[3], t );
    return kvs::Vec4( color[0] * a, color[1] * a, color[2] * a, a );
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PreIntegrationTable3D class.
 */
/*===========================================================================*/
PreIntegrationTable3D::PreIntegrationTable3D()
{
    this->setScalarResolution( 128 );
    this->setDepthResolution( 128 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PreIntegrationTable3D class.
 *  @param  scalar_resolution [in] resolution of the scalar axis
 *  @param  depth_resolution [in] resolution of the depth axis
 */
/*===========================================================================*/
PreIntegrationTable3D::PreIntegrationTable3D( const size_t scalar_resolution, const size_t depth_resolution ):
    m_scalar_resolution( scalar_resolution ),
    m_depth_resolution( depth_resolution )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer function data.
 *  @param  transfer_function [in] transfer function
 *  @param  min_scalar [in] minimum scalar value
 *  @param  max_scalar [in] maximum scalar value
 */
/*===========================================================================*/
void PreIntegrationTable3D::setTransferFunction(
    const kvs::TransferFunction& transfer_function,
    const float min_scalar,
    const float max_scalar )
{
    const float S0 = min_scalar;
    const float S1 = max_scalar;
    const size_t N = m_scalar_resolution;
    m_transfer_function = ::Serialize( transfer_function, S0, S1, N );
}

/*===========================================================================*/
/**
 *  @brief  Creates pre-integration table by numerical integration.
 *  @param  max_size_of_cell [in] maximum size of the cell
 */
/*===========================================================================*/
void PreIntegrationTable3D::create( const float max_size_of_cell )
{
    // Compute pre-integration table.
    const size_t slice_size = 4 * m_scalar_resolution * m_scalar_resolution;
    m_table.allocate( slice_size * m_depth_resolution );
    m_table.fill( 0.0f );

    const float dl = max_size_of_cell / float( m_depth_resolution - 1 );
    kvs::Real32* slice0 = m_table.data();
    this->compute_exact_level( slice0, dl );

    float l = dl;
    for ( size_t i = 1; i < m_depth_resolution; i++ )
    {
        l += dl;
        kvs::Real32* slice = slice0 + i * slice_size;
        const kvs::Real32* slicep = slice0 + ( i - 1 ) * slice_size;
        this->compute_incremental_level( slice, slicep, slice0, l, dl );
    }
}

/*===========================================================================*/
/**
 *  @brief  Computes 2D pre-integration table by numerical integration.
 *  @param  slice0 [in/out] pointer to the head of the first slice
 *  @param  dl [in] thickness of a slice
 */
/*===========================================================================*/
void PreIntegrationTable3D::compute_exact_level( float* slice0, const float dl )
{
    const size_t N = m_scalar_resolution;
    const kvs::ValueArray<kvs::Real32>& TF = m_transfer_function;
    for ( size_t sb = 0, index = 0; sb < N; sb++ )
    {
        for ( size_t sf = 0; sf < N; sf++, index++ )
        {
            kvs::Vec4 c( 0.0f, 0.0f, 0.0f, 0.0f );

            if ( sb == sf )
            {
                const float t = dl;
                c = ::OpacityWeightedColor( kvs::Vec4( &TF[4*sb] ), t );
            }
            else
            {
                const size_t smin = kvs::Math::Min( sb, sf );
                const size_t smax = kvs::Math::Max( sb, sf );

                const size_t M = 32; // supersampling factor
                const float dw = 1.0f / static_cast<float>( M - 1 );
                const float t = dw * dl / static_cast<float>( smax - smin );
                for ( size_t k = smin; k < smax; k++ )
                {
                    // Opacity correction.
                    const kvs::Vec4 c0 = ::OpacityWeightedColor( kvs::Vec4( &TF[4*k] ), t );
                    const kvs::Vec4 c1 = ::OpacityWeightedColor( kvs::Vec4( &TF[4*(k+1)] ), t );

                    // Acutual composition.
                    float w = 0.0f;
                    for ( size_t m = 0; m < M; m++, w += dw )
                    {
                        const kvs::Vec4 ck = ::Interpolate( c0, c1, w );
                        c = c + ck * ( 1.0f - c[3] );
                    }
                }
            }

            slice0[ 4 * index + 0 ] = c[0];
            slice0[ 4 * index + 1 ] = c[1];
            slice0[ 4 * index + 2 ] = c[2];
            slice0[ 4 * index + 3 ] = c[3];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Computes approximate level using previous levels.
 *  @param  slice [in/out] pointer to the head of the current slice
 *  @param  slicep [in] pointer to the head of the previous slice
 *  @param  slice0 [in] pointer to the head of the first slice
 *  @param  l [in] thickness between the first and the current slices
 *  @param  dl [in] thickness of a slice
 */
/*===========================================================================*/
void PreIntegrationTable3D::compute_incremental_level(
    float* slice,
    const float* slicep,
    const float* slice0,
    const float l,
    const float dl )
{
    const size_t N = m_scalar_resolution;
    for ( size_t i = 0, index = 0; i < N; i++ )
    {
        for ( size_t j = 0; j < N; j++, index++ )
        {
            const float sf = ( 2.0f * j + 1.0f ) / ( 2.0f * N );
            const float sb = ( 2.0f * i + 1.0f ) / ( 2.0f * N );
            const float sp = ( ( l - dl ) * sf + ( dl * sb ) ) / l;

            const size_t k = static_cast<size_t>( sp * N - 0.5f );
            const float w = sp * N - ( k + 0.5f );

            kvs::Vec4 c; // current color
            kvs::Vec4 cp; // previous color
            if ( k == N - 1 )
            {
                c = kvs::Vec4( slice0 + 4 * ( k * N + j ) );
                cp = kvs::Vec4( slicep + 4 * ( i * N + k ) );
            }
            else
            {
                const kvs::Vec4 temp0( slice0 + 4 * ( k * N + j ) );
                const kvs::Vec4 temp1( slice0 + 4 * ( ( k + 1 ) * N + j ) );
                c = ::Interpolate( temp0, temp1, w );

                const kvs::Vec4 temp2( slicep + 4 * ( i * N + k ) );
                const kvs::Vec4 temp3( slicep + 4 * ( i * N + ( k + 1 ) ) );
                cp = ::Interpolate( temp2, temp3, w );
            }

            // Composition.
            c = c + cp * ( 1.0f - c[3] );
            slice[ 4 * index + 0 ] = c[0];
            slice[ 4 * index + 1 ] = c[1];
            slice[ 4 * index + 2 ] = c[2];
            slice[ 4 * index + 3 ] = c[3];
        }
    }
}

} // end of namespace kvs
