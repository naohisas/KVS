/****************************************************************************/
/**
 *  @file CellByCellParticleGenerator.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellByCellParticleGenerator.h 1382 2012-12-03 14:54:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__CELL_BY_CELL_PARTICLE_GENERATOR_H_INCLUDE
#define KVS__CELL_BY_CELL_PARTICLE_GENERATOR_H_INCLUDE

#include <kvs/OpenGL>
#include <kvs/VolumeObjectBase>
#include <kvs/OpacityMap>
#include <kvs/Vector3>
#include <kvs/Math>
#include <kvs/Camera>
#include <kvs/CellBase>


namespace kvs
{

namespace detail
{

template <typename T>
inline kvs::Matrix44<T> ScalingMatrix( T x, T y, T z )
{
    const T elements[ 16 ] =
    {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };

    return kvs::Matrix44<T>( elements );
}

template <typename T>
inline kvs::Matrix44<T> TranslationMatrix( T x, T y, T z )
{
    const T elements[ 16 ] =
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    return kvs::Matrix44<T>( elements );
}

inline void GetViewport( const kvs::Camera* camera, GLint (*viewport)[4] )
{
    (*viewport)[0] = 0;
    (*viewport)[1] = 0;
    (*viewport)[2] = camera->windowWidth();
    (*viewport)[3] = camera->windowHeight();
}

inline void GetProjectionMatrix( const kvs::Camera* camera, GLdouble (*projection)[16] )
{
    const kvs::Matrix44f P = camera->projectionMatrix();
    (*projection)[ 0] = P[0][0];
    (*projection)[ 1] = P[1][0];
    (*projection)[ 2] = P[2][0];
    (*projection)[ 3] = P[3][0];
    (*projection)[ 4] = P[0][1];
    (*projection)[ 5] = P[1][1];
    (*projection)[ 6] = P[2][1];
    (*projection)[ 7] = P[3][1];
    (*projection)[ 8] = P[0][2];
    (*projection)[ 9] = P[1][2];
    (*projection)[10] = P[2][2];
    (*projection)[11] = P[3][2];
    (*projection)[12] = P[0][3];
    (*projection)[13] = P[1][3];
    (*projection)[14] = P[2][3];
    (*projection)[15] = P[3][3];
}

inline void GetModelviewMatrix( const kvs::Camera* camera, const kvs::ObjectBase* object, GLdouble (*modelview)[16] )
{
    const kvs::Vector3f  min_external = object->minExternalCoord();
    const kvs::Vector3f  max_external = object->maxExternalCoord();
    const kvs::Vector3f  center       = ( max_external + min_external ) * 0.5f;
    const kvs::Vector3f  diff         = max_external - min_external;
    const float          normalize    = 6.0f / kvs::Math::Max( diff.x(), diff.y(), diff.z() );

    const kvs::Matrix44f T = TranslationMatrix<float>( -center.x(), -center.y(), -center.z() );
    const kvs::Matrix44f S = ScalingMatrix<float>( normalize, normalize, normalize );
    const kvs::Matrix44f L = camera->viewingMatrix();
    const kvs::Matrix44f M = L * S * T;

    (*modelview)[ 0] = M[0][0];
    (*modelview)[ 1] = M[1][0];
    (*modelview)[ 2] = M[2][0];
    (*modelview)[ 3] = M[3][0];
    (*modelview)[ 4] = M[0][1];
    (*modelview)[ 5] = M[1][1];
    (*modelview)[ 6] = M[2][1];
    (*modelview)[ 7] = M[3][1];
    (*modelview)[ 8] = M[0][2];
    (*modelview)[ 9] = M[1][2];
    (*modelview)[10] = M[2][2];
    (*modelview)[11] = M[3][2];
    (*modelview)[12] = M[0][3];
    (*modelview)[13] = M[1][3];
    (*modelview)[14] = M[2][3];
    (*modelview)[15] = M[3][3];
}

inline void Project(
    const GLdouble obj_x,
    const GLdouble obj_y,
    const GLdouble obj_z,
    const GLdouble modelview[16],
    const GLdouble projection[16],
    const GLint    viewport[4],
    GLdouble*      win_x,
    GLdouble*      win_y,
    GLdouble*      win_z )
{
    const kvs::Vector4d I( obj_x, obj_y, obj_z, 1.0 );

    const kvs::Matrix44d P(
        projection[0], projection[4], projection[8],  projection[12],
        projection[1], projection[5], projection[9],  projection[13],
        projection[2], projection[6], projection[10], projection[14],
        projection[3], projection[7], projection[11], projection[15] );

    const kvs::Matrix44d M(
        modelview[0], modelview[4], modelview[8],  modelview[12],
        modelview[1], modelview[5], modelview[9],  modelview[13],
        modelview[2], modelview[6], modelview[10], modelview[14],
        modelview[3], modelview[7], modelview[11], modelview[15] );

    const kvs::Vector4d O = P * M * I;

    const double w =  ( O.w() == 0.0f ) ? 1.0f : O.w();

    *win_x = viewport[0] + ( 1.0 + O.x() / w ) * viewport[2] / 2.0;
    *win_y = viewport[1] + ( 1.0 + O.y() / w ) * viewport[3] / 2.0;
    *win_z = ( 1.0 + O.z() / w ) / 2.0;
}

inline void UnProject(
    const GLdouble win_x,
    const GLdouble win_y,
    const GLdouble win_z,
    const GLdouble modelview[16],
    const GLdouble projection[16],
    const GLint    viewport[4],
    GLdouble*      obj_x,
    GLdouble*      obj_y,
    GLdouble*      obj_z )
{
    const kvs::Vector4d I(
        ( win_x - viewport[0] ) * 2.0 / viewport[2] - 1.0,
        ( win_y - viewport[1] ) * 2.0 / viewport[3] - 1.0,
        2.0 * win_z - 1.0,
        1.0 );

    const kvs::Matrix44d P(
        projection[0], projection[4], projection[8],  projection[12],
        projection[1], projection[5], projection[9],  projection[13],
        projection[2], projection[6], projection[10], projection[14],
        projection[3], projection[7], projection[11], projection[15] );

    const kvs::Matrix44d M(
        modelview[0], modelview[4], modelview[8],  modelview[12],
        modelview[1], modelview[5], modelview[9],  modelview[13],
        modelview[2], modelview[6], modelview[10], modelview[14],
        modelview[3], modelview[7], modelview[11], modelview[15] );

    const kvs::Vector4d O = ( P * M ).inverted() * I;

    const double w =  ( O.w() == 0.0 ) ? 1.0 : O.w();

    *obj_x = O.x() / w;
    *obj_y = O.y() / w;
    *obj_z = O.z() / w;
}

inline float CalculateObjectDepth(
    const kvs::ObjectBase* object,
    const GLdouble         modelview[16],
    const GLdouble         projection[16],
    const GLint            viewport[4] )
{
    // calculate suitable depth.
    GLdouble x, y, z;

    Project(
        object->objectCenter().x(),
        object->objectCenter().y(),
        object->objectCenter().z(),
        modelview, projection, viewport,
        &x, &y, &z );

    const float object_depth = static_cast<float>( z );

    return object_depth;
}

inline float CalculateSubpixelLength(
    const float    subpixel_level,
    const float    object_depth,
    const GLdouble modelview[16],
    const GLdouble projection[16],
    const GLint    viewport[4] )
{
    GLdouble wx_min, wy_min, wz_min;
    GLdouble wx_max, wy_max, wz_max;

    UnProject(
        0.0, 0.0, GLdouble( object_depth ),
        modelview, projection, viewport,
        &wx_min, &wy_min, &wz_min);

    UnProject(
        1.0, 1.0 , GLdouble( object_depth ),
        modelview, projection, viewport,
        &wx_max, &wy_max, &wz_max);

    const float subpixel_length = static_cast<float>( ( wx_max - wx_min ) / subpixel_level );

    return subpixel_length;
}

} // end of namespace


namespace CellByCellParticleGenerator
{

#if KVS_ENABLE_DEPRECATED
using kvs::detail::CalculateObjectDepth;
using kvs::detail::CalculateSubpixelLength;
#endif

inline float GetRandomNumber()
{
    // xorshift RGNs with period at least 2^128 - 1.
//    static float t24 = 1.0/16777216.0; /* 0.5**24 */
    static kvs::UInt32 x=123456789,y=362436069,z=521288629,w=88675123;
    kvs::UInt32 t;
    t=(x^(x<<11));
    x=y;y=z;z=w;
    w=(w^(w>>19))^(t^(t>>8));

    return w * ( 1.0f / 4294967296.0f ); // = w * ( 1.0f / kvs::Value<kvs::UInt32>::Max() + 1 )
//    return t24 * static_cast<float>( w >> 8 );
}

inline const kvs::Vector3f RandomSamplingInCube( const kvs::Vector3f& v )
{
//    float x = GetRandomNumber(); while ( kvs::Math::Equal( x, 1.0f ) ) x = GetRandomNumber();
//    float y = GetRandomNumber(); while ( kvs::Math::Equal( y, 1.0f ) ) y = GetRandomNumber();
//    float z = GetRandomNumber(); while ( kvs::Math::Equal( z, 1.0f ) ) z = GetRandomNumber();
    const float x = GetRandomNumber();
    const float y = GetRandomNumber();
    const float z = GetRandomNumber();
    const kvs::Vector3f d( x, y, z );
    return v + d;
}

inline float CalculateObjectDepth( 
    const kvs::Camera& camera, 
    const kvs::ObjectBase& object )
{
    // Calculate a transform matrix.
    GLdouble modelview[16];  kvs::detail::GetModelviewMatrix( &camera, &object, &modelview );
    GLdouble projection[16]; kvs::detail::GetProjectionMatrix( &camera, &projection );
    GLint viewport[4];       kvs::detail::GetViewport( &camera, &viewport );

    // Calculate a depth of the center of gravity of the object.
    const float object_depth = kvs::detail::CalculateObjectDepth(
            &object, modelview, projection, viewport );

    return object_depth;
}

inline float CalculateSubpixelLength(
    const float subpixel_level, 
    const kvs::Camera& camera, 
    const kvs::ObjectBase& object )
{
    // Calculate a transform matrix.
    GLdouble modelview[16];  kvs::detail::GetModelviewMatrix( &camera, &object, &modelview );
    GLdouble projection[16]; kvs::detail::GetProjectionMatrix( &camera, &projection );
    GLint viewport[4];       kvs::detail::GetViewport( &camera, &viewport );

    // Calculate a depth of the center of gravity of the object.
    const float object_depth = kvs::detail::CalculateObjectDepth(
            &object, modelview, projection, viewport );

    // Calculate suitable subpixel length.
    const float subpixel_length = kvs::detail::CalculateSubpixelLength(
            subpixel_level, object_depth, modelview, projection, viewport );

    return subpixel_length;
}

inline const kvs::ValueArray<float> CalculateDensityMap(
    const kvs::Camera*     camera,
    const kvs::ObjectBase* object,
    const float            subpixel_level,
    const float            sampling_step,
    const kvs::OpacityMap& opacity_map )
{
    // Calculate suitable subpixel length.
    const float subpixel_length = CalculateSubpixelLength( subpixel_level, *camera, *object );

    // Calculate density map from the subpixel length and the opacity map.
    const float max_opacity = 1.0f - std::exp( -sampling_step / subpixel_length );
    const float max_density = 1.0f / ( subpixel_length * subpixel_length * subpixel_length );

    const float sampling_volume_inverse = 1.0f / ( subpixel_length * subpixel_length * sampling_step );

    const size_t resolution = opacity_map.resolution();

    // Create the density map.
    kvs::ValueArray<float> density_map( resolution );

    for ( size_t i = 0; i < resolution; ++i )
    {
        const float opacity = opacity_map[i];

        if ( opacity < max_opacity )
        {
            density_map[i] = -std::log( 1.0f - opacity ) * sampling_volume_inverse;
        }
        else
        {
            density_map[i] = max_density;
        }
    }

    return density_map;
}

inline kvs::Real32 AveragedScalar( const kvs::CellBase* cell )
{
    const size_t nnodes = cell->numberOfCellNodes();;
    const kvs::Real32* S = cell->values();
    kvs::Real32 Sa = 0; for ( size_t i = 0; i < nnodes; i++ ) { Sa += S[i]; }
    return Sa / nnodes;
}

} // end of namespace CellByCellParticleGenerator

} // end of namespace kvs

#if KVS_ENABLE_DEPRECATED
namespace
{
using kvs::detail::ScalingMatrix;
using kvs::detail::TranslationMatrix;
using kvs::detail::GetViewport;
using kvs::detail::GetProjectionMatrix;
using kvs::detail::GetModelviewMatrix;
using kvs::detail::Project;
using kvs::detail::UnProject;
}
#endif

#endif // KVS__CELL_BY_CELL_PARTICLE_GENERATOR_H_INCLUDE
