/*****************************************************************************/
/**
 *  @file   CellByCellSampling.cpp
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
#include "CellByCellSampling.h"
#include <kvs/Coordinate>
#include <kvs/Xorshift128>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/OpenMP>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a scaling matrix.
 *  @param  s [in] scaling paramters
 *  @return scaling matrix
 */
/*===========================================================================*/
inline kvs::Mat4 ScalingMatrix( const kvs::Vec3& s )
{
    const kvs::Real32 elements[16] = {
        s[0], 0.0f, 0.0f, 0.0f,
        0.0f, s[1], 0.0f, 0.0f,
        0.0f, 0.0f, s[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return kvs::Mat4( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns a translation matrix.
 *  @param  t [in] translation parameters
 *  @return translation matrix
 */
/*===========================================================================*/
inline kvs::Mat4 TranslationMatrix( const kvs::Vec3& t )
{
    const kvs::Real32 elements[16] = {
        1.0f, 0.0f, 0.0f, t[0],
        0.0f, 1.0f, 0.0f, t[1],
        0.0f, 0.0f, 1.0f, t[2],
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return kvs::Mat4( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns a viewport.
 *  @param  camera [in] pointer to camera
 *  @return viewport
 */
/*===========================================================================*/
inline kvs::Vec4 Viewport( const kvs::Camera* camera )
{
    const kvs::Real32 x = 0.0f;
    const kvs::Real32 y = 0.0f;
    const kvs::Real32 w = camera->windowWidth();
    const kvs::Real32 h = camera->windowHeight();
    return kvs::Vec4( x, y, w, h );
}

/*===========================================================================*/
/**
 *  @brief  Returns a projection matrix.
 *  @param  camera [in] pointer to camera
 *  @return projection matrix
 */
/*===========================================================================*/
inline kvs::Mat4 ProjectionMatrix( const kvs::Camera* camera )
{
    return camera->projectionMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Returns a model-view matrix.
 *  @param  camera [in] pointer to camera
 *  @param  object [in] pointer to object
 *  @return model-view matrix
 */
/*===========================================================================*/
inline kvs::Mat4 ModelViewMatrix( const kvs::Camera* camera, const kvs::ObjectBase* object )
{
    const kvs::Vec3 min_ext = object->minExternalCoord();
    const kvs::Vec3 max_ext = object->maxExternalCoord();
    const kvs::Vec3 center = ( min_ext + max_ext ) * 0.5f;
    const kvs::Vec3 d = max_ext - min_ext;
    const kvs::Real32 scale = 6.0f / kvs::Math::Max( d.x(), d.y(), d.z() );

    const kvs::Mat4 T = TranslationMatrix( -center );
    const kvs::Mat4 S = ScalingMatrix( kvs::Vec3::Constant( scale ) );
    const kvs::Mat4 V = camera->viewingMatrix();
    return V * S * T;
}

/*===========================================================================*/
/**
 *  @brief  Maps object coordinates to window coordinates.
 *  @param  p [in] a point in object coordinates
 *  @param  camera [in] pointer to camera
 *  @param  object [in] pointer to object
 *  @return point in window coordinates
 */
/*===========================================================================*/
kvs::Vec3 Obj2Win(
    const kvs::Vec3 p,
    const kvs::Camera* camera,
    const kvs::ObjectBase* object )
{
    const kvs::Vec4 V = Viewport( camera );
    const kvs::Mat4 P = ProjectionMatrix( camera );
    const kvs::Mat4 M = ModelViewMatrix( camera, object );
    const kvs::Vec4 O = P * M * kvs::Vec4( p, 1.0f );

    const kvs::Real32 w = kvs::Math::IsZero( O.w() ) ? 1.0f : O.w();
    const kvs::Real32 x = V[0] + ( 1.0f + O.x() / w ) * V[2] / 2.0f;
    const kvs::Real32 y = V[1] + ( 1.0f + O.y() / w ) * V[3] / 2.0f;
    const kvs::Real32 z = ( 1.0f + O.z() / w ) / 2.0f;
    return kvs::Vec3( x, y, z );
}

/*===========================================================================*/
/**
 *  @brief  Maps window coordinates to object coordinates.
 *  @param  p [in] a point in window coordinates
 *  @param  camera [in] pointer to camera
 *  @param  object [in] pointer to object
 *  @return point in object coordinates
 */
/*===========================================================================*/
kvs::Vec3 Win2Obj(
    const kvs::Vec3 p,
    const kvs::Camera* camera,
    const kvs::ObjectBase* object )
{
    const kvs::Vec4 V = Viewport( camera );
    const kvs::Mat4 P = ProjectionMatrix( camera );
    const kvs::Mat4 M = ModelViewMatrix( camera, object );

    const kvs::Real32 x = ( p.x() - V[0] ) * 2.0f / V[2] - 1.0f;
    const kvs::Real32 y = ( p.y() - V[1] ) * 2.0f / V[3] - 1.0f;
    const kvs::Real32 z = 2.0f * p.z() - 1.0f;
    const kvs::Vec4 O = ( P * M ).inverted() * kvs::Vec4( x, y, z, 1.0f );

    const kvs::Real32 w = kvs::Math::IsZero( O.w() ) ? 1.0f : O.w();
    return kvs::Vec3( O.x(), O.y(), O.z() ) / w;
}

/*===========================================================================*/
/**
 *  @brief  Returns depth value of center of object in window coordinates.
 *  @param  camera [in] pointer to camera
 *  @param  object [in] pointer to object
 *  @return depth value
 */
/*===========================================================================*/
kvs::Real32 ObjectDepth( const kvs::Camera* camera, const kvs::ObjectBase* object )
{
    const kvs::Vec3 p = object->objectCenter();
    return Obj2Win( p, camera, object ).z();
}

/*===========================================================================*/
/**
 *  @brief  Returns length of a pixel in object coordinates.
 *  @param  camera [in] pointer to camera
 *  @param  object [in] pointer to object
 *  @return pixel length in object coordinates
 */
/*===========================================================================*/
kvs::Real32 PixelLength( const kvs::Camera* camera, const kvs::ObjectBase* object )
{
    const kvs::Real32 depth = ObjectDepth( camera, object );
    const kvs::Vec3 p1( 0.0f, 0.0f, depth );
    const kvs::Vec3 p2( 1.0f, 1.0f, depth );
    return Win2Obj( p2, camera, object ).x() - Win2Obj( p1, camera, object ).x();
}

} // end of namespace


namespace kvs
{

namespace CellByCellSampling
{

/*===========================================================================*/
/**
 *  @brief  Returns the density specified by the scalar value.
 *  @param  value [in] scalar value
 *  @return density
 */
/*===========================================================================*/
kvs::Real32 ParticleDensityMap::at( const float value ) const
{
    if ( value == m_min_value ) { return m_table[0]; }
    if ( value == m_max_value ) { return m_table[ m_resolution - 1 ]; }
    if ( value < m_min_value || m_max_value < value ) { return 0.0f; }

    const float r = static_cast<float>( m_resolution - 1 );
    const float v = ( value - m_min_value ) / ( m_max_value - m_min_value ) * r;
    const size_t s0 = static_cast<size_t>( v );
    const size_t s1 = s0 + 1;

    const kvs::Real32 d0 = m_table[ s0 ];
    const kvs::Real32 d1 = m_table[ s1 ];
    return kvs::Math::Mix( d0, d1, v - s0 );
}

/*===========================================================================*/
/**
 *  @brief  Creates the particle density map.
 *  @param  omap [in] opacity map
 */
/*===========================================================================*/
void ParticleDensityMap::create( const kvs::OpacityMap& omap )
{
    m_resolution = omap.resolution();
    m_min_value = omap.minValue();
    m_max_value = omap.maxValue();

    const kvs::Real32 dt = m_sampling_step;
    const kvs::Real32 length = ::PixelLength( m_camera, m_object );

    const kvs::Real32 max_opacity = 1.0f - std::exp( -dt / length );
    const kvs::Real32 max_density = 1.0f / ( length * length * length );
    const kvs::Real32 inv_volume = 1.0f / ( length * length * dt );

    m_table = Table( m_resolution );

    KVS_OMP_PARALLEL()
    {
        KVS_OMP_FOR( schedule(static) )
        for ( size_t i = 0; i < m_resolution; i++ )
        {
            const kvs::Real32 opacity = omap[i];
            if ( opacity < max_opacity )
            {
                m_table[i] = -std::log( 1.0f - opacity ) * inv_volume;
            }
            else
            {
                m_table[i] = max_density;
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns maximum density in scalar range of [s0,s1].
 *  @param  s0 [in] scalar 0
 *  @param  s1 [in] scalar 1
 *  @return maximum density
 */
/*===========================================================================*/
kvs::Real32 ParticleDensityMap::max_density( const kvs::Real32 s0, const kvs::Real32 s1 ) const
{
    const kvs::Real32 min_value = this->minValue();
    const kvs::Real32 max_value = this->maxValue();
    const size_t dims = this->resolution() - 1;
    const kvs::Real32 scale = dims / ( max_value - min_value );
    size_t i0 = static_cast<size_t>( ( s0 - min_value ) * scale );
    size_t i1 = static_cast<size_t>( ( s1 - min_value ) * scale ) + 1;
    i0 = kvs::Math::Clamp( i0, size_t(0), dims );
    i1 = kvs::Math::Clamp( i1, size_t(0), dims );

    kvs::Real32 max_density = this->table().at( i0 );

#if defined(_OPENMP) && (_OPENMP >= 201107)
    KVS_OMP_PARALLEL()
    {
        KVS_OMP_FOR( reduction(max:max_density) )
#endif
        for( size_t i = i0; i <= i1; i++ )
        {
            const kvs::Real32 density =  this->table().at(i);
            max_density = kvs::Math::Max( max_density, density );
        }
#if defined(_OPENMP) && (_OPENMP >= 201107)
    }
#endif
    return max_density;
}

} // end of namespace CellByCellSampling

} // end of namespace kvs
